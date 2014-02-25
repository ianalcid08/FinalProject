#include "LevelData.h"
#include "Game.h"
#include "Conveyor.h"
#include "../TinyXML/tinyxml.h"
#include "AssetManager.h"
#include "Renderer.h"
#include "GameplayState.h"
#include "ResetState.h"
#include "IntroState.h"
#include "Crate.h"
#include "Barrel.h"
#include "DeathPit.h"

/***********************************************
 * Filename:  		LevelData.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		03/5/2013
 * Mod. Initials:	SD
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all level data
 ************************************************/
#define CONV_SHRINK 0.5f

enum ETilePOS { TILE_TOP, TILE_BOTTOM, TILE_LEFT, TILE_RIGHT };

CLevelData::CLevelData(void)
{
}


CLevelData::~CLevelData(void)
{
}

void CLevelData::DestroyLevelData()
{
	if(m_bIntroState)
		CIntroState::GetInstance()->Shutdown();
	if(m_pConveyorVertexBuffer)
		m_pConveyorVertexBuffer->Release();
	if(m_pTileVertexBuffer)
		m_pTileVertexBuffer->Release();

	for(unsigned int i = 0; i < m_vWaypoints.size(); i++)
	{
		m_vWaypoints[i].vSpawns.clear();
	}

	m_vActiveConveyors.clear();

	m_vWaypoints.clear();
	m_nNextWayPoint = 0;

	for(int i = 0; i < NUM_TILESETS; ++i)
	{
		m_nNumTiles[i] = 0;
		m_TileSets[i] = nullptr;
		m_TileNormals[i] = nullptr;
	}
}

void CLevelData::Init()
{
	m_pIntroState = CIntroState::GetInstance();
	m_bIntroState = true;
	m_pRenderer = CRenderer::GetInstance();

	m_nNextWayPoint = 0;
	m_vWaypoints.clear();
	m_pOF = CGame::GetInstance()->GetOF();
	CAssetManager* pAM = CAssetManager::GetInstance();
	m_TileSets[TILE_ASSEMBLY] = pAM->LoadTexture("Assets/Textures/Environment_Assembly_Floor_Tiles_Together.png");
	m_TileNormals[TILE_ASSEMBLY] = pAM->LoadTexture("Assets/Textures/Environment_Assembly_Floor_Tiles_Normal_Together.png");
	m_d3dAmbientLights[TILE_ASSEMBLY] = D3DXVECTOR4(0.1f, 0.1f, 0.05f, 1.0f);

	m_TileSets[TILE_FOUNDRY] = pAM->LoadTexture("Assets/Textures/Environment_Foundry_Floor_Diffuse.png");
	m_TileNormals[TILE_FOUNDRY] = pAM->LoadTexture("Assets/Textures/Environment_Foundry_Floor_Normal.png");
	m_d3dAmbientLights[TILE_FOUNDRY] = D3DXVECTOR4(0.5f, 0.3f, 0.2f, 1.0f);

	m_TileSets[TILE_AICORE] = pAM->LoadTexture("TestFiles/BossArena_NoPlayers.jpg");
	// NO ART YET
	m_TileNormals[TILE_AICORE] = pAM->LoadTexture("TestFiles/BossArena_NoPlayers.jpg");
	m_d3dAmbientLights[TILE_AICORE] = D3DXVECTOR4(1.0f, 0.8f, 0.8f, 1.0f);
	m_bFoundry = false;
	m_fFoundryScroll = 0.0f;
	m_bDisableConveyors = false;
	m_nTotalTiles = 0;
	m_bRenderFloor = false;
	m_bOpenFloor = false;
	m_fSpinFloorAnimTime = 0;

	D3DXMatrixIdentity(&m_d3dFloorCurrFrame[0]);
	D3DXMatrixIdentity(&m_d3dFloorCurrFrame[1]);
}

void CLevelData::Render()
{
	if(m_bIntroState)
		m_pIntroState->Render();

	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();

	// Light Position offset
	D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
	capture[2] += CAM_ZOFF;

	if(m_bRenderFloor)
		RenderFloor();

	ID3DXEffect* pShader = nullptr;
	unsigned int passes(0);
	if(!m_bDisableConveyors)
	{
		pShader = pAM->LoadShader("Assets/Shaders/Conveyor.fx");
		pShader->SetTechnique("Conveyor");

		pShader->Begin(&passes,0);
		for(unsigned pass(0);pass < passes; pass++)
		{
			pShader->BeginPass(pass);

			pShader->SetTexture("gDiffuseTexture", pAM->LoadTexture("Assets/Textures/Environment_Conveyor_Diffuse.png"));
			pShader->SetTexture("gNormalTexture", pAM->LoadTexture("Assets/Textures/Environment_Conveyor_Normal.png"));
			pShader->SetMatrix("gViewProjection", &ViewProj);
			pShader->SetFloatArray("gLightDir", &capture[0], 3);

			pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
			pD3D->SetStreamSource(0, m_pConveyorVertexBuffer, 0, sizeof(TTileVert));

			for(unsigned int i = 0; i < m_vActiveConveyors.size(); ++i)
			{
				pShader->SetFloat("gTime", ((CConveyor*)m_vActiveConveyors[i])->GetScrollTime());
				pShader->CommitChanges();

				pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, i*4, 2);
			}

			pShader->EndPass();
		}
		pShader->End();
	}

	if(m_bFoundry)
	{
		pShader = pAM->LoadShader("Assets/Shaders/Conveyor.fx");
		pShader->SetTechnique("LakeofFire");
		pShader->SetTexture("gDiffuseTexture", pAM->LoadTexture("Assets/Textures/Environment_Foundry_Lava.png"));
		pShader->SetFloat("gTime", m_fFoundryScroll);
		pShader->SetMatrix("gViewProjection", &ViewProj);

		D3DXMATRIX pos;
		D3DXMatrixIdentity(&pos);
		*(D3DXVECTOR3*)&pos[12] = capture;
		pos[13] = 0.0f;
		pos[14] -= CAM_ZOFF;
		pShader->SetMatrix("gWorld", &pos);
		pShader->CommitChanges();
		
		passes = 0;
		pShader->Begin(&passes,0);
		for(unsigned pass(0);pass < passes; pass++)
		{
			pShader->BeginPass(pass);

			pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
			pD3D->SetStreamSource(0, m_pTileVertexBuffer, 0, sizeof(TTileVert));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

			pShader->EndPass();
		}
		pShader->End();
	}

	pShader = pAM->LoadShader("Assets/Shaders/Tiles.fx");
	passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned pass(0);pass < passes; pass++)
	{
		pShader->BeginPass(pass);

		pShader->SetTexture("gDiffuseTexture", m_TileSets[TILE_ASSEMBLY]);
		pShader->SetTexture("gNormalTexture", m_TileNormals[TILE_ASSEMBLY]);
		pShader->SetFloatArray("gAmbient", m_d3dAmbientLights[TILE_ASSEMBLY], 4);
		pShader->SetFloatArray("gLightDir", &capture[0], 3);
		pShader->SetMatrix("gViewProjection", &ViewProj);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
		pD3D->SetStreamSource(0, m_pTileVertexBuffer, 0, sizeof(TTileVert));
		int changeup = TILE_ASSEMBLY;
		int runningTotal = m_nNumTiles[TILE_ASSEMBLY]+1;
		for(int i = 1; i < m_nTotalTiles; ++i)
		{
			if(i == runningTotal)
			{
				changeup++;
				runningTotal+=m_nNumTiles[changeup];
				pShader->SetTexture("gDiffuseTexture", m_TileSets[changeup]);
				pShader->SetTexture("gNormalTexture", m_TileNormals[changeup]);
				pShader->SetFloatArray("gAmbient", m_d3dAmbientLights[changeup], 4);
				pShader->CommitChanges();
			}

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, i*4, 2);
		}
		
		pShader->EndPass();
	}
	pShader->End();
}

void CLevelData::ResetToCheckpoint()
{
	for(unsigned int i = m_nCheckpointID; i < m_vWaypoints.size(); ++i)
	{
		for(unsigned int x = 0; x < m_vWaypoints[i].vSpawns.size(); ++x)
		{
			m_vWaypoints[i].vSpawns[x].Reset();
		}
	}

	CCamera* tempCam = CGameplayState::GetInstance()->GetCamera();
	tempCam->SetCamera(m_d3dCheckpointCam);
	

	m_d3dRoamingPoint = m_d3dCheckpointCam;

	TWayPoint temp = m_vWaypoints[m_nCheckpointID];
	temp.vPos = *(D3DXVECTOR3*)&m_d3dCheckpointCam[12];
	tempCam->SetCurrWaypoint(temp);

	if(m_nCheckpointID+1 != m_vWaypoints.size())
	{
		temp.vPos += m_vWaypoints[m_nCheckpointID + 1].vPos;
		tempCam->SetNextWaypoint(temp);
		m_nNextWayPoint = m_nCheckpointID + 1;
	}

	tempCam->ResetCurrTime();
	tempCam->Unlock();
	tempCam->Update(0.0f);
	CGame::GetInstance()->GetOM()->Clear(false);
	CObjectFactory* tempOF = CGame::GetInstance()->GetOF();
	tempOF->ProcessDestroy();

	for each(TWorldObject obj in m_vWaypoints[m_nCheckpointID].m_vWorldObjects)
	{
		IBaseObject* pNewObj;
		if(m_pOF->Create((eObjects)obj.m_nObjectID, &pNewObj))
		{
			D3DXMATRIX trans;
			D3DXMatrixIdentity(&trans);
			trans[12] = obj.m_fWorldX;
			trans[14] = obj.m_fWorldZ;
			pNewObj->SetMatrix(trans);
		}
	}

	CGameplayState::GetInstance()->DisplayCheckpoint();
	CGameplayState::GetInstance()->ResetPlayers(true);
}

void CLevelData::SkipToNextWaypoint()
{
	if((unsigned int)m_nNextWayPoint >= m_vWaypoints.size())
			return;
	while(m_vWaypoints[m_nNextWayPoint].fTime < 0)
	{
		m_nNextWayPoint++;
		if((unsigned int)m_nNextWayPoint >= m_vWaypoints.size())
			return;
	}
	*(D3DXVECTOR3*)&m_d3dRoamingPoint[12] += m_vWaypoints[m_nNextWayPoint].vPos;

	CCamera* tempCam = CGameplayState::GetInstance()->GetCamera();
	tempCam->SetCamera(m_d3dRoamingPoint);

	TWayPoint temp = m_vWaypoints[m_nNextWayPoint];
	temp.vPos = *(D3DXVECTOR3*)&m_d3dRoamingPoint[12];
	tempCam->SetCurrWaypoint(temp);

	m_nNextWayPoint++;
	if(m_vWaypoints[m_nNextWayPoint].checkpoint)
	{
		m_nCheckpointID = m_nNextWayPoint;
		m_d3dCheckpointCam = CGameplayState::GetInstance()->GetCamera()->GetCamera();
		CGameplayState::GetInstance()->DisplayCheckpoint();
	}
	
	if(m_nNextWayPoint != m_vWaypoints.size())
	{
		temp.vPos += m_vWaypoints[m_nNextWayPoint].vPos;
		tempCam->SetNextWaypoint(temp);
	}

	tempCam->ResetCurrTime();
	tempCam->Unlock();
	tempCam->Update(0.0f);
	CGame::GetInstance()->GetOM()->Clear(false);
	CObjectFactory* tempOF = CGame::GetInstance()->GetOF();
	tempOF->ProcessDestroy();
	for(unsigned int i = 0; i < m_vActiveConveyors.size(); ++i)
	{
		tempOF->Create(OBJ_CONVEYOR, (IBaseObject**)&m_vActiveConveyors[i]);
	}

	for each(TWorldObject obj in temp.m_vWorldObjects)
	{
		IBaseObject* pNewObj;
		if(m_pOF->Create((eObjects)obj.m_nObjectID, &pNewObj))
		{
			D3DXMATRIX trans;
			D3DXMatrixIdentity(&trans);
			trans[12] = obj.m_fWorldX;
			trans[14] = obj.m_fWorldZ;
			pNewObj->SetMatrix(trans);
			pNewObj->SetRadius(1.0f);
		}
	}

	CGameplayState::GetInstance()->ResetPlayers(true);
}

TWayPoint CLevelData::GetNextWaypoint(void)
{
	if((unsigned int)m_nNextWayPoint < m_vWaypoints.size())
	{
		m_d3dRoamingPoint = CGameplayState::GetInstance()->GetCamera()->GetCamera();

		TWayPoint tempPoint = m_vWaypoints[m_nNextWayPoint];
		if(tempPoint.checkpoint)
		{
			CGameplayState::GetInstance()->ResetPlayers(false);

			m_nCheckpointID = m_nNextWayPoint;
			m_d3dCheckpointCam = CGameplayState::GetInstance()->GetCamera()->GetCamera();
			CGameplayState::GetInstance()->DisplayCheckpoint();
		}

		if((unsigned int)m_nNextWayPoint < m_vWaypoints.size())
			++m_nNextWayPoint;

		if((unsigned int)m_nNextWayPoint < m_vWaypoints.size())
		{
			for each(TWorldObject obj in m_vWaypoints[m_nNextWayPoint].m_vWorldObjects)
			{
				IBaseObject* pNewObj;
				if(m_pOF->Create((eObjects)obj.m_nObjectID, &pNewObj))
				{
					D3DXMATRIX trans;
					D3DXMatrixIdentity(&trans);
					trans[12] = obj.m_fWorldX;
					trans[14] = obj.m_fWorldZ;
					pNewObj->SetMatrix(trans);
				}
			}
		}
		if(tempPoint.checkpoint)
		{
			int nTemp = CGameplayState::GetInstance()->GetDeathHintCounter();
			CGameplayState::GetInstance()->SetDeathHintCounter(++nTemp);
		}
		return tempPoint;
	}

	TWayPoint tempPoint;
	tempPoint.fTime = 0.0f;
	tempPoint.vPos = D3DXVECTOR3(0, 0, 0);

	if(tempPoint.checkpoint)
	{
		int nTemp = CGameplayState::GetInstance()->GetDeathHintCounter();
		CGameplayState::GetInstance()->SetDeathHintCounter(++nTemp);
	}
	return tempPoint;
}

void CLevelData::LoadData(char* szFileName)
{
	m_pIntroState->Initialize();
	{
		IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();

		fstream istream;
		istream.open("Data/Level Data/NewTile.lvl", ios_base::binary|ios_base::in);
		if(istream.is_open())
		{
			istream.read((char*)&m_nNumConveyors, sizeof(int));
			float UVMax[2];
			float VertCorners[4];
			TTileVert verts[4];
			int Direction = 0;

			pD3D->CreateVertexBuffer(sizeof(TTileVert)*m_nNumConveyors*4, 0, 0, D3DPOOL_MANAGED, &m_pConveyorVertexBuffer, 0);
			float* m_pConTileData = (float*)malloc(sizeof(TTileVert)*m_nNumConveyors*4);
			for(int i = 0; i < m_nNumConveyors; ++i)
			{
				istream.read((char*)&UVMax, sizeof(float) * 2);
				istream.read((char*)&VertCorners, sizeof(float) * 4);
				istream.read((char*)&Direction, sizeof(int));

				// Position Data
				*(D3DXVECTOR3*)&verts[0].m_fVerts = D3DXVECTOR3(VertCorners[TILE_LEFT], 0, VertCorners[TILE_TOP]);
				*(D3DXVECTOR3*)&verts[1].m_fVerts = D3DXVECTOR3(VertCorners[TILE_RIGHT], 0, VertCorners[TILE_TOP]);
				*(D3DXVECTOR3*)&verts[2].m_fVerts = D3DXVECTOR3(VertCorners[TILE_RIGHT], 0, VertCorners[TILE_BOTTOM]);
				*(D3DXVECTOR3*)&verts[3].m_fVerts = D3DXVECTOR3(VertCorners[TILE_LEFT], 0, VertCorners[TILE_BOTTOM]);

				// UV Data
				int Corner = Direction;
				*(D3DXVECTOR2*)&verts[Corner++].m_fUVs = D3DXVECTOR2(0, 0);
				if(Corner > 3)
					Corner = 0;
				*(D3DXVECTOR2*)&verts[Corner++].m_fUVs = D3DXVECTOR2(0, UVMax[1]);
				if(Corner > 3)
					Corner = 0;
				*(D3DXVECTOR2*)&verts[Corner++].m_fUVs = D3DXVECTOR2(UVMax[0], UVMax[1]);
				if(Corner > 3)
					Corner = 0;
				*(D3DXVECTOR2*)&verts[Corner].m_fUVs = D3DXVECTOR2(UVMax[0], 0);

				memcpy((char*)m_pConTileData + i*sizeof(TTileVert)*4, verts, sizeof(TTileVert)*4);
				CConveyor* tempConveyor;
				if(m_pOF->Create(OBJ_CONVEYOR, (IBaseObject**)&tempConveyor))
				{
					m_vActiveConveyors.push_back(tempConveyor);
					tempConveyor->BuildAABB(D3DXVECTOR3(VertCorners[TILE_RIGHT]-CONV_SHRINK, 1.0f, VertCorners[TILE_TOP]-CONV_SHRINK), D3DXVECTOR3(VertCorners[TILE_LEFT]+CONV_SHRINK, 0, VertCorners[TILE_BOTTOM]+CONV_SHRINK));
					tempConveyor->SetDirection(Direction);
				}
			}

			istream.read((char*)&m_nNumTiles[TILE_ASSEMBLY], sizeof(int));
			istream.read((char*)&m_nNumTiles[TILE_FOUNDRY], sizeof(int));
			istream.read((char*)&m_nNumTiles[TILE_FOUNDRY], sizeof(int));
			istream.read((char*)&m_nNumTiles[TILE_AICORE], sizeof(int));
			for(int i = 0; i < NUM_TILESETS; ++i)
				m_nTotalTiles += m_nNumTiles[i];
			m_nTotalTiles++;
			float UVs[4];

			pD3D->CreateVertexBuffer(sizeof(TTileVert)*m_nTotalTiles*4, 0, 0, D3DPOOL_MANAGED, &m_pTileVertexBuffer, 0);
			float* pTileData = (float*)malloc(sizeof(TTileVert)*m_nTotalTiles*4);
			// Molten Lake Data
			*(D3DXVECTOR3*)&verts[0].m_fVerts = D3DXVECTOR3(-30.0f, -0.8f, 50.0f);
			*(D3DXVECTOR2*)&verts[0].m_fUVs = D3DXVECTOR2(0.0f, 0.0f);
			*(D3DXVECTOR3*)&verts[1].m_fVerts = D3DXVECTOR3(30.0f, -0.8f, 50.0f);
			*(D3DXVECTOR2*)&verts[1].m_fUVs = D3DXVECTOR2(5.0f, 0.0f);
			*(D3DXVECTOR3*)&verts[2].m_fVerts = D3DXVECTOR3(30.0f, -0.8f, -6.0f);
			*(D3DXVECTOR2*)&verts[2].m_fUVs = D3DXVECTOR2(5.0f, 5.0f);
			*(D3DXVECTOR3*)&verts[3].m_fVerts = D3DXVECTOR3(-30.0f, -0.8f, -6.0f);
			*(D3DXVECTOR2*)&verts[3].m_fUVs = D3DXVECTOR2(0.0f, 5.0f);
			memcpy((char*)pTileData, verts, sizeof(TTileVert)*4);

			for(int i = 1; i < m_nTotalTiles; ++i)
			{
				istream.read((char*)&UVs, sizeof(float) * 4);
				istream.read((char*)&VertCorners, sizeof(float) * 4);

				// Vertex Data
				*(D3DXVECTOR3*)&verts[0].m_fVerts = D3DXVECTOR3(VertCorners[TILE_LEFT], 0, VertCorners[TILE_TOP]);
				*(D3DXVECTOR2*)&verts[0].m_fUVs = D3DXVECTOR2(UVs[TILE_LEFT], UVs[TILE_TOP]);
				*(D3DXVECTOR3*)&verts[1].m_fVerts = D3DXVECTOR3(VertCorners[TILE_RIGHT], 0, VertCorners[TILE_TOP]);
				*(D3DXVECTOR2*)&verts[1].m_fUVs = D3DXVECTOR2(UVs[TILE_RIGHT], UVs[TILE_TOP]);
				*(D3DXVECTOR3*)&verts[2].m_fVerts = D3DXVECTOR3(VertCorners[TILE_RIGHT], 0, VertCorners[TILE_BOTTOM]);
				*(D3DXVECTOR2*)&verts[2].m_fUVs = D3DXVECTOR2(UVs[TILE_RIGHT], UVs[TILE_BOTTOM]);
				*(D3DXVECTOR3*)&verts[3].m_fVerts = D3DXVECTOR3(VertCorners[TILE_LEFT], 0, VertCorners[TILE_BOTTOM]);
				*(D3DXVECTOR2*)&verts[3].m_fUVs = D3DXVECTOR2(UVs[TILE_LEFT], UVs[TILE_BOTTOM]);

				memcpy((char*)pTileData + i*sizeof(TTileVert)*4, verts, sizeof(TTileVert)*4);
			}
			istream.close();

			if(m_nNumConveyors)
			{
				void *vram = nullptr;
				m_pConveyorVertexBuffer->Lock(0, 0, &vram, 0);
				memcpy(vram, m_pConTileData, 4*m_nNumConveyors*sizeof(TTileVert));
				m_pConveyorVertexBuffer->Unlock();
				free(m_pConTileData);
			}

			if(m_nTotalTiles)
			{
				void *vram = nullptr;
				m_pTileVertexBuffer->Lock(0, 0, &vram, 0);
				memcpy(vram, pTileData, 4*m_nTotalTiles*sizeof(TTileVert));
				m_pTileVertexBuffer->Unlock();
				free(pTileData);
			}
		}
	}

	TiXmlDocument doc(szFileName);
	if(!doc.LoadFile())
		return;
	TiXmlElement* pRoot = doc.RootElement();
	if(pRoot == nullptr)
		return;

	// Intro Scene Waypoints
	TWayPoint tempWaypoint;
#ifndef _DEBUG
	tempWaypoint.fTime = 23.0f;
#else
	CIntroState::GetInstance()->SetSkipTutorial(true);
	tempWaypoint.fTime = 1.0f; //change to 25.0f
#endif
	tempWaypoint.vPos = D3DXVECTOR3(0, 0, 0);
	tempWaypoint.checkpoint = false;
	CSpawnPoint::Spawn fadeSpawn;
	fadeSpawn.fSpawnTime = 0.0f;
	fadeSpawn.nObjectID = (eObjects)41;
	CSpawnPoint fadePoint;
	fadePoint.m_vSpawnObjects.push_back(fadeSpawn);
	tempWaypoint.vSpawns.push_back(fadePoint);
	m_vWaypoints.push_back(tempWaypoint);
	fadePoint.m_vSpawnObjects.clear();
	tempWaypoint.vSpawns.clear();

	// Fade Out from Intro
	tempWaypoint.fTime = -1.0f;
	tempWaypoint.vPos = D3DXVECTOR3(0, 0, 0);
	fadeSpawn.fSpawnTime = 0.0f;
	fadeSpawn.nObjectID = (eObjects)40;
	fadePoint.m_vSpawnObjects.push_back(fadeSpawn);
	tempWaypoint.vSpawns.push_back(fadePoint);
	m_vWaypoints.push_back(tempWaypoint);

	fadePoint.m_vSpawnObjects.clear();
	tempWaypoint.vSpawns.clear();

	// Transit from Intro
	tempWaypoint.fTime = 2.0f;
	tempWaypoint.vPos = D3DXVECTOR3(0, 0, 21.0f);
	m_vWaypoints.push_back(tempWaypoint);

	TiXmlElement* pWayPoint = pRoot->FirstChildElement("WayPoint");
	
	while(pWayPoint != nullptr)
	{
		TiXmlElement* pWayPointInfo = pWayPoint->FirstChildElement("WayPointInfo");

		double dVar;
		TWayPoint tempWP;
		int checkpoint;
		D3DXVECTOR3 tempVec = D3DXVECTOR3(0, 0, 0);
		pWayPointInfo->Attribute("PositionX", (double*)&dVar);
		tempVec.x = (float)dVar;
		pWayPointInfo->Attribute("PositionZ", (double*)&dVar);
		tempVec.z = (float)dVar;
		pWayPointInfo->Attribute("Time", (double*)&dVar);

		pWayPointInfo->Attribute("Checkpoint", &checkpoint);
		tempWP.fTime = (float)dVar;
		tempWP.vPos = tempVec;
		if(checkpoint)
			tempWP.checkpoint = true;
		else
			tempWP.checkpoint = false;

		TiXmlElement* pSpawnPoint = pWayPoint->FirstChildElement("SpawnPoint");
		while(pSpawnPoint != nullptr)
		{
			TiXmlElement* pSpawnInfo = pSpawnPoint->FirstChildElement("SpawnInfo");
			if(pSpawnInfo == nullptr)
				break;
			CSpawnPoint tempSpawn;
			D3DXMatrixIdentity(&tempSpawn.m_d3dPosition);
			double capture;
			pSpawnInfo->Attribute("PositionX", (double*)&capture);
			tempSpawn.m_d3dPosition._41 = (float)capture;
			pSpawnInfo->Attribute("PositionZ", (double*)&capture);
			tempSpawn.m_d3dPosition._43 = (float)capture;
			TiXmlElement* pSpawnObject = pSpawnInfo->FirstChildElement();
			while(pSpawnObject != nullptr)
			{
				CSpawnPoint::Spawn tTemp;
				pSpawnObject->Attribute("ObjectID", (int*)&tTemp.nObjectID);
				pSpawnObject->Attribute("SpawnTime", (double*)&capture);
				tTemp.fSpawnTime = (float)capture;
				tempSpawn.m_vSpawnObjects.push_back(tTemp);
				pSpawnObject = pSpawnObject->NextSiblingElement();
			}

			tempWP.vSpawns.push_back(tempSpawn);
			pSpawnPoint = pSpawnPoint->NextSiblingElement();
		}

		TiXmlElement* pWorldObjects = pWayPoint->FirstChildElement("WorldObjects");
		TiXmlElement* pObjectData = pWorldObjects->FirstChildElement("ObjectData");
		int nCapture = 0;
		while(pObjectData != nullptr)
		{
			nCapture++;
			TWorldObject tWO;
			int IDCapture;
			pObjectData->Attribute("ObjectID", (int*)&IDCapture);

			switch(IDCapture)
			{
			case 0:
				tWO.m_nObjectID = OBJ_CRATE;
				break;
			case 1:
				tWO.m_nObjectID = OBJ_BARREL;
				break;
			case 2:
				tWO.m_nObjectID = OBJ_VAT;
				break;
			case 3:
				tWO.m_nObjectID = OBJ_PIT;
				break;
			}

			double capture = 0;
			pObjectData->Attribute("PositionX", (double*)&capture);
			tWO.m_fWorldX = (float)capture;
			pObjectData->Attribute("PositionZ", (double*)&capture);
			tWO.m_fWorldZ = (float)capture;
			tempWP.m_vWorldObjects.push_back(tWO);
			pObjectData = pObjectData->NextSiblingElement();
		}
		m_vWaypoints.push_back(tempWP);
		pWayPoint = pWayPoint->NextSiblingElement();
	}
}

void CLevelData::Update(float fDelta)
{
	if(GetAsyncKeyState(VK_SUBTRACT)&1)
		ResetToCheckpoint();
	else if(GetAsyncKeyState(VK_ADD)&1)
	{
		SkipToNextWaypoint();
	}

	if(m_bRenderFloor && GetAsyncKeyState(VK_NUMPAD0)&1)
		m_bOpenFloor = true;

	if(m_bOpenFloor)
	{
		CAssetManager* pAM = CAssetManager::GetInstance();
		m_fSpinFloorAnimTime += fDelta;
		if(m_fSpinFloorAnimTime > pAM->GetAnimation(OBJ_MIDBOSSFLOOR, 0)->m_fDuration)
		{
			m_bOpenFloor = false;
			m_bRenderFloor = false;
		}
		else
		{
			CAnimationProcessor::Interpolator(pAM->GetAnimation(OBJ_MIDBOSSFLOOR, 0), m_fSpinFloorAnimTime, m_d3dFloorCurrFrame);
		}
	}

	if(m_bFoundry)
		m_fFoundryScroll += fDelta*0.1f;

	if(!m_nNextWayPoint)
		return;

	if(m_bIntroState)
	{
		if(m_nNextWayPoint > 2)
		{
			m_bIntroState = false;
			m_pIntroState->Shutdown();
		}
		else
			m_pIntroState->Update(fDelta);
	}

	for(unsigned int i = 0; i < m_vWaypoints[m_nNextWayPoint-1].vSpawns.size(); i++)
	{
		m_vWaypoints[m_nNextWayPoint-1].vSpawns[i].Update(fDelta);
	}
}

void CLevelData::ToggleFoundry()
{
	m_bFoundry = ! m_bFoundry;
	m_bDisableConveyors = true;

	if(m_vActiveConveyors.size() > 0)
	{
		for(unsigned int i = 0; i < m_vActiveConveyors.size(); ++i)
			m_pOF->Destroy(m_vActiveConveyors[i]);
		m_vActiveConveyors.clear();
	}
}


void CLevelData::RenderFloor()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();

	ID3DXEffect* pShader = pAM->GetShader(OBJ_MIDBOSSFLOOR);
	unsigned int passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		pShader->BeginPass(i);
		D3DXMATRIX pos;
		D3DXMatrixIdentity(&pos);
		*(D3DXVECTOR3*)&pos[12] = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		pos[13] = 0;
		pShader->SetMatrixArray("gBones", m_d3dFloorCurrFrame, 2);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_MIDBOSSFLOOR));
		pShader->SetMatrix("gWorld", &pos);
		pShader->SetMatrix("gViewProjection", &ViewProj);
		pShader->SetInt("AmIHit",(0));

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		pD3D->SetStreamSource(0, pAM->GetVertBuffer(OBJ_MIDBOSSFLOOR), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(OBJ_MIDBOSSFLOOR));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_MIDBOSSFLOOR), 0, pAM->GetNumTriangles(OBJ_MIDBOSSFLOOR));

		pShader->EndPass();
	}
	pShader->End();
}

void CLevelData::ToggleConveyors()
{
	for each(CConveyor* conveyor in m_vActiveConveyors)
	{
		conveyor->ToggleHalt();
	}
}