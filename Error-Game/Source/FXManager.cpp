/**********************************************************************************************
 * Filename:  		FXManager.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/25/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that will manage and create all of the effects.
 **********************************************************************************************/
#include "FXManager.h"
#include "Emitter.h"
#include "GeometryEffect.h"
#include "AssetManager.h"
#include "Renderer.h"
#include <fstream>
using namespace std;

CFXManager::CFXManager()
{
	ClearData();
	ReInit();
}

CFXManager::~CFXManager()
{

}

void CFXManager::ReInit()
{
	ClearData();
	CAssetManager* pAM = CAssetManager::GetInstance();
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Fire.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Fire2.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Fire3.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Fire4.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Fire5.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_WeaponTelsa_Electricity_1.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_WeaponTelsa_Electricity_2.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_WeaponTelsa_Electricity_3.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_WeaponTelsa_Electricity_4.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Fire1.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Fire2.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Smoke.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Smoke2.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Smash_Bot_ShockwaveT.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Star.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/White2x2.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Shockwave.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Sparks.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Sparks2.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Effect_Explosion_Sparks3.png"));
	m_vTextures.push_back(pAM->LoadTexture("Assets/Textures/Lounge_Tigers_Weapon_Projectile_Defense_Laser.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Lightning1.png"));
	m_vTextures.push_back(pAM->LoadTexture("TestFiles/Lightning2.png"));
	for(int i=0;i<MAX_EFFECTS;i++)
		m_vAvailableIndex.push_back(i);
}

CEffect* CFXManager::CreateEffect(int nID,D3DXMATRIX d3dMatrix)
{
	if(!m_vAvailableIndex.size())
		return NULL;
	if((unsigned int)nID>=m_vDefaults.size())
		nID=0;
	TEffectDefault temp = m_vDefaults[nID];
	CEffect* pEffect = &m_cEffects[m_vAvailableIndex.back()];
	pEffect->SetID(nID);
	m_vAvailableIndex.pop_back();
	pEffect->SetLife(temp.fLifeTime);
	pEffect->SetLoop(temp.bLooping);
	D3DXMATRIX d3dPosition = temp.d3dPosition;
	if(temp.nRandomPos)
	{
		d3dPosition[12] += (((rand()%temp.nRandomPos)/10.0f)-(temp.nRandomPos/20.0f));
		d3dPosition[13] += (((rand()%temp.nRandomPos)/10.0f)-(temp.nRandomPos/20.0f));
		d3dPosition[14] += (((rand()%temp.nRandomPos)/10.0f)-(temp.nRandomPos/20.0f));
	}
	pEffect->SetOffset(d3dPosition[12], d3dPosition[13], d3dPosition[14]);
	pEffect->SetMatrix(d3dMatrix);
	if(temp.nRandomVel)
		pEffect->SetVelocity(temp.d3dVelocity + D3DXVECTOR3(((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f),((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f),((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f)));
	else
		pEffect->SetVelocity(temp.d3dVelocity);
	for(int i=0;i<MAX_EMITTERS;i++)
	{
		if(temp.vEmitters[i].fLife <= 0.0f)
			break;
		CEmitter newSub;
		TEmitterStruct tEmitter;
		tEmitter.bLooping = temp.vEmitters[i].bLooping;
		tEmitter.nRandomnessP = temp.vEmitters[i].nRandomnessP;
		tEmitter.nRandomnessV = temp.vEmitters[i].nRandomnessV;
		tEmitter.d3dEffectVelocity = temp.vEmitters[i].d3dEffectVelocity;
		if(temp.vEmitters[i].nRandVel)
			tEmitter.d3dEffectVelocity += D3DXVECTOR3(((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f),((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f),((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f));
		tEmitter.fLife = temp.vEmitters[i].fLife;
		tEmitter.fSpawnRate = temp.vEmitters[i].fSpawnRate;
		tEmitter.fStartTime = temp.vEmitters[i].fStartTime;
		tEmitter.nTextureID = temp.vEmitters[i].nTextureID;
		TStage tStages[MAX_STAGES];
		for(int j=0;j<MAX_STAGES;j++)
		{
			tStages[j].d3dColor = temp.vEmitters[i].tStages[j].d3dColor;
			tStages[j].d3dVelocity = temp.vEmitters[i].tStages[j].d3dVelocity;
			tStages[j].d3dRotation = temp.vEmitters[i].tStages[j].d3dRotation;
			tStages[j].d3dScale = temp.vEmitters[i].tStages[j].d3dScale;
			tStages[j].fTime = temp.vEmitters[i].tStages[j].fTime;
			tEmitter.tStages[j] = tStages[j];
		}
		tEmitter.d3dOffset = temp.vEmitters[i].d3dOffset;
		tEmitter.d3dPosition = temp.vEmitters[i].d3dPosition;
		if(temp.vEmitters[i].nRandPos)
		{
			tEmitter.d3dOffset += D3DXVECTOR3(((rand()%temp.vEmitters[i].nRandPos)/1.0f)-(temp.vEmitters[i].nRandPos/2.0f),((rand()%temp.vEmitters[i].nRandPos)/1.0f)-(temp.vEmitters[i].nRandPos/2.0f),((rand()%temp.vEmitters[i].nRandPos)/1.0f)-(temp.vEmitters[i].nRandPos/2.0f));
			D3DXMATRIX trans;
			D3DXMatrixTranslation(&trans,tEmitter.d3dOffset.x,tEmitter.d3dOffset.y,tEmitter.d3dOffset.z);
			tEmitter.d3dPosition = trans * tEmitter.d3dPosition;
		}
		newSub.Create(tEmitter);
		pEffect->AddEmitter(newSub);
	}
	for(int i=0;i<MAX_GEOMETRIES;i++)
	{
		if(temp.vGeometries[i].fLife <= 0.0f)
			break;
		CGeometryEffect newSub;
		TGeometryStruct tGeo;
		tGeo.bLooping = temp.vGeometries[i].bLooping;
		tGeo.nMeshID = temp.vGeometries[i].nMeshID + OBJ_NUM_OBJECTS;
		tGeo.bAnimated = temp.vGeometries[i].bAnimated;
		tGeo.nAnimID = temp.vGeometries[i].nAnimID;
		tGeo.d3dEffectVelocity = temp.vGeometries[i].d3dEffectVelocity;
		if(temp.vGeometries[i].nRandVel)
			tGeo.d3dEffectVelocity += D3DXVECTOR3(((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f),((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f),((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f));
		tGeo.fLife = temp.vGeometries[i].fLife;
		tGeo.fStartTime = temp.vGeometries[i].fStartTime;
		tGeo.nTextureID = temp.vGeometries[i].nTextureID;
		TStage tStages[MAX_STAGES];
		for(int j=0;j<MAX_STAGES;j++)
		{
			tStages[j].d3dColor = temp.vGeometries[i].tStages[j].d3dColor;
			tStages[j].d3dVelocity = temp.vGeometries[i].tStages[j].d3dVelocity;
			tStages[j].d3dRotation = temp.vGeometries[i].tStages[j].d3dRotation;
			tStages[j].d3dScale = temp.vGeometries[i].tStages[j].d3dScale;
			tStages[j].fTime = temp.vGeometries[i].tStages[j].fTime;
			tGeo.tStages[j] = tStages[j];
		}
		tGeo.d3dOffset = temp.vGeometries[i].d3dOffset;
		tGeo.d3dPosition = temp.vGeometries[i].d3dPosition;
		if(temp.vGeometries[i].nRandPos)
		{
			tGeo.d3dOffset += D3DXVECTOR3(((rand()%temp.vGeometries[i].nRandPos)/1.0f)-(temp.vGeometries[i].nRandPos/2.0f),((rand()%temp.vGeometries[i].nRandPos)/1.0f)-(temp.vGeometries[i].nRandPos/2.0f),((rand()%temp.vGeometries[i].nRandPos)/1.0f)-(temp.vGeometries[i].nRandPos/2.0f));
			D3DXMATRIX trans;
			D3DXMatrixTranslation(&trans,tGeo.d3dOffset.x,tGeo.d3dOffset.y,tGeo.d3dOffset.z);
			tGeo.d3dPosition = trans * tGeo.d3dPosition;
		}
		newSub.Create(tGeo);
		pEffect->AddGeometry(newSub);
	}
	m_lActiveEffects.push_back(pEffect);
	return pEffect;
}

void CFXManager::ReleaseEffect(CEffect* pEffect)
{
		m_lActiveEffects.remove(pEffect);
		pEffect->Reset();
		int nID = ((int)pEffect-(int)&m_cEffects[0])/sizeof(CEffect);
		bool check = true;
		for(unsigned int i = 0; i < m_vAvailableIndex.size(); i++)
		{
			if(nID == m_vAvailableIndex[i])
			{
				check=false;
				break;
			}
		}
		if(check)
			m_vAvailableIndex.push_back(nID);
}

bool CFXManager::LoadEffects(char* szFilename)
{
	fstream fs;
	fs.open(szFilename,ios_base::binary|ios_base::in);
	if(fs.is_open())
	{
		int size = sizeof(int);
		int sizeD = sizeof(double);
		double capture = 0.0;
		int numEffects = 0;
		int effectID = -1;
		int numSubs = -1;
		int nStageCount = -1;
		fs.read((char*)&numEffects,size);
		for(int k=0;k<numEffects;k++)
		{
			fs.read((char*)&effectID,size);
			TEffectDefault temp;
			memset(&temp,0,sizeof(TEffectDefault));
			fs.read((char*)&capture,sizeD);
			temp.fLifeTime = (float)capture;
			fs.read((char*)&temp.bLooping,sizeof(bool));
			D3DXMatrixIdentity(&temp.d3dPosition);
			for(int i=0;i<3;i++)
			{
				fs.read((char*)&capture,sizeD);
				temp.d3dPosition[12+i] = (float)capture;
			}
			fs.read((char*)&temp.nRandomPos,size);
			for(int i=0;i<3;i++)
			{
				fs.read((char*)&capture,sizeD);
				temp.d3dVelocity[i] = (float)capture;
			}
			fs.read((char*)&temp.nRandomVel,size);
			fs.read((char*)&numSubs,size);
			int emitts=0;
			int geos=0;
			for(int i=0;i<numSubs;i++)
			{
				int nID = -1;
				fs.read((char*)&nID,size);
				if(nID == 1)
				{
					fs.read((char*)&capture,sizeD);
					temp.vEmitters[emitts].fSpawnRate = (float)capture;
					fs.read((char*)&temp.vEmitters[emitts].nRandomnessP,size);
					fs.read((char*)&temp.vEmitters[emitts].nRandomnessV,size);
					fs.read((char*)&capture,sizeD);
					temp.vEmitters[emitts].fStartTime = (float)capture;
					fs.read((char*)&temp.vEmitters[emitts].bLooping,sizeof(bool));
					fs.read((char*)&capture,sizeD);
					temp.vEmitters[emitts].fLife = (float)capture;
					fs.read((char*)&temp.vEmitters[emitts].nTextureID,size);
					fs.read((char*)&temp.vEmitters[emitts].nMeshID,size);
					fs.read((char*)&temp.vEmitters[emitts].source,size);
					fs.read((char*)&temp.vEmitters[emitts].dest,size);
					D3DXMatrixIdentity(&temp.vEmitters[emitts].d3dPosition);
					for(int l=0;l<3;l++)
					{
						fs.read((char*)&capture,sizeD);
						temp.vEmitters[emitts].d3dPosition[12+l] = (float)capture;
						temp.vEmitters[emitts].d3dOffset[l] = (float)capture;
					}
					fs.read((char*)&temp.vEmitters[emitts].nRandPos,size);
					for(int l=0;l<3;l++)
					{
						fs.read((char*)&capture,sizeD);
						temp.vEmitters[emitts].d3dEffectVelocity[l] = (float)capture;
					}
					fs.read((char*)&temp.vEmitters[i].nRandVel,size);
					fs.read((char*)&nStageCount,size);
					for(int j=0;j<MAX_STAGES;j++)
					{
						if(j < nStageCount)
						{
							fs.read((char*)&capture,sizeD);
							temp.vEmitters[emitts].tStages[j].fTime = (float)capture;
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vEmitters[emitts].tStages[j].d3dScale[l] = (float)capture;
							}
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vEmitters[emitts].tStages[j].d3dRotation[l] = (float)capture;
							}
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vEmitters[emitts].tStages[j].d3dVelocity[l] = (float)capture;
							}
							unsigned char color[4] = {};
							fs.read((char*)&color[0],sizeof(unsigned char));
							fs.read((char*)&color[1],sizeof(unsigned char));
							fs.read((char*)&color[2],sizeof(unsigned char));
							fs.read((char*)&color[3],sizeof(unsigned char));
							temp.vEmitters[emitts].tStages[j].d3dColor.r = color[1] / 255.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.g = color[2] / 255.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.b = color[3] / 255.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.a = color[0] / 255.0f;
						}
						else
						{
							temp.vEmitters[i].tStages[j].fTime = 0.0f;
							for(int l=0;l<3;l++)
							{
								temp.vEmitters[emitts].tStages[j].d3dScale[l] = 0.0f;
								temp.vEmitters[emitts].tStages[j].d3dRotation[l] = 0.0f;
								temp.vEmitters[emitts].tStages[j].d3dVelocity[l] = 0.0f;
							}		
							temp.vEmitters[emitts].tStages[j].d3dColor.r = 0.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.g = 0.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.b = 0.0f;
							temp.vEmitters[emitts].tStages[j].d3dColor.a = 0.0f;
						}
					}
					emitts++;
				}
				else
				{
					fs.read((char*)&temp.vGeometries[geos].bAnimated,sizeof(bool));
					fs.read((char*)&temp.vGeometries[geos].nAnimID,size);
					fs.read((char*)&capture,sizeD);
					temp.vGeometries[geos].fStartTime = (float)capture;
					fs.read((char*)&temp.vGeometries[geos].bLooping,sizeof(bool));
					fs.read((char*)&capture,sizeD);
					temp.vGeometries[geos].fLife = (float)capture;
					fs.read((char*)&temp.vGeometries[geos].nTextureID,size);
					fs.read((char*)&temp.vGeometries[geos].nMeshID,size);
					fs.read((char*)&temp.vGeometries[geos].source,size);
					fs.read((char*)&temp.vGeometries[geos].dest,size);
					D3DXMatrixIdentity(&temp.vGeometries[i].d3dPosition);
					for(int l=0;l<3;l++)
					{
						fs.read((char*)&capture,sizeD);
						temp.vGeometries[geos].d3dPosition[12+l] = (float)capture;
						temp.vGeometries[geos].d3dOffset[l] = (float)capture;
					}
					fs.read((char*)&temp.vGeometries[geos].nRandPos,size);
					for(int l=0;l<3;l++)
					{
						fs.read((char*)&capture,sizeD);
						temp.vGeometries[geos].d3dEffectVelocity[l] = (float)capture;
					}
					fs.read((char*)&temp.vGeometries[geos].nRandVel,size);
					fs.read((char*)&nStageCount,size);
					for(int j=0;j<MAX_STAGES;j++)
					{
						if(j < nStageCount)
						{
							fs.read((char*)&capture,sizeD);
							temp.vGeometries[geos].tStages[j].fTime = (float)capture;
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vGeometries[geos].tStages[j].d3dScale[l] = (float)capture;
							}
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vGeometries[geos].tStages[j].d3dRotation[l] = (float)capture;
							}
							for(int l=0;l<3;l++)
							{
								fs.read((char*)&capture,sizeD);
								temp.vGeometries[geos].tStages[j].d3dVelocity[l] = (float)capture;
							}
							unsigned char color[4] = {};
							fs.read((char*)&color[0],sizeof(unsigned char));
							fs.read((char*)&color[1],sizeof(unsigned char));
							fs.read((char*)&color[2],sizeof(unsigned char));
							fs.read((char*)&color[3],sizeof(unsigned char));
							temp.vGeometries[geos].tStages[j].d3dColor.r = color[1] / 255.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.g = color[2] / 255.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.b = color[3] / 255.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.a = color[0] / 255.0f;
						}
						else
						{
							temp.vGeometries[geos].tStages[j].fTime = 0.0f;
							for(int l=0;l<3;l++)
							{
								temp.vGeometries[geos].tStages[j].d3dScale[l] = 0.0f;
								temp.vGeometries[geos].tStages[j].d3dRotation[l] = 0.0f;
								temp.vGeometries[geos].tStages[j].d3dVelocity[l] = 0.0f;
							}
							temp.vGeometries[geos].tStages[j].d3dColor.r = 0.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.g = 0.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.b = 0.0f;
							temp.vGeometries[geos].tStages[j].d3dColor.a = 0.0f;
						}
					}
					geos++;
				}
			}
			m_vDefaults.push_back(temp);
		}
		fs.close();
		return true;
	}
	return false;
}

void CFXManager::UpdateEffects(float fDelta)
{
	std::list<CEffect*>::iterator iter;
	for(iter=m_lActiveEffects.begin();iter!=m_lActiveEffects.end();++iter)
	{
		if(!(*iter)->Update(fDelta))
			if((*iter)->GetLoop())
				ReloadData(*iter);
			else
				m_vNeedReleased.push_back(*iter);
	}
	for(unsigned int i=0;i<m_vNeedReleased.size();i++)
	{		
		ReleaseEffect(m_vNeedReleased.back());
		m_vNeedReleased.pop_back();
	}
}

void CFXManager::RenderEffects(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj)
{
	IDirect3DDevice9* pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	DWORD srcBlend,destBlend;
	pD3D->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
	pD3D->GetRenderState(D3DRS_DESTBLEND,&destBlend);
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pD3D->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pD3D->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	std::list<CEffect*>::iterator iter;
	for(iter=m_lActiveEffects.begin();iter!=m_lActiveEffects.end();++iter)
		(*iter)->RenderGeo(d3dCam,d3dProj);
	pD3D->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
	for(iter=m_lActiveEffects.begin();iter!=m_lActiveEffects.end();++iter)
		(*iter)->RenderEmitt(d3dCam,d3dProj);
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	pD3D->SetRenderState(D3DRS_SRCBLEND,srcBlend);
	pD3D->SetRenderState(D3DRS_DESTBLEND,destBlend);
}

void CFXManager::ClearData()
{
	std::list<CEffect*>::iterator iter;
	for(iter=m_lActiveEffects.begin();iter!=m_lActiveEffects.end();++iter)
		(*iter)->Reset();
	m_lActiveEffects.clear();
	m_vAvailableIndex.clear();
	m_vDefaults.clear();
	m_vTextures.clear();
}

void CFXManager::ReloadData(CEffect* pEffect)
{
	int nID = pEffect->GetID();
	if((unsigned int)nID>=m_vDefaults.size())
		nID=0;
	TEffectDefault temp = m_vDefaults[nID];
	pEffect->SetLife(temp.fLifeTime);
	if(temp.nRandomVel)
		pEffect->SetVelocity(temp.d3dVelocity + D3DXVECTOR3(((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f),((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f),((rand()%temp.nRandomVel)/1.0f)-(temp.nRandomVel/2.0f)));
	else
		pEffect->SetVelocity(temp.d3dVelocity);
	for(int i=0;i<MAX_EMITTERS;i++)
	{
		if(temp.vEmitters[i].fLife <= 0.0f)
			break;
		CEmitter* newSub = pEffect->GetEmitter(i);
		TEmitterStruct tEmitter;
		tEmitter.fStartTime = temp.vEmitters[i].fStartTime;
		tEmitter.d3dEffectVelocity = temp.vEmitters[i].d3dEffectVelocity;
		if(temp.vEmitters[i].nRandVel)
			tEmitter.d3dEffectVelocity += D3DXVECTOR3(((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f),((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f),((rand()%temp.vEmitters[i].nRandVel)/1.0f)-(temp.vEmitters[i].nRandVel/2.0f));
		tEmitter.fLife = temp.vEmitters[i].fLife;
		tEmitter.fStartTime = temp.vEmitters[i].fStartTime;
		newSub->ReCreate(tEmitter);
	}
	for(int i=0;i<MAX_GEOMETRIES;i++)
	{
		if(temp.vGeometries[i].fLife <= 0.0f)
			break;
		CGeometryEffect* newSub = pEffect->GetGeometry(i);
		TGeometryStruct tGeo;
		tGeo.fStartTime = temp.vGeometries[i].fStartTime;
		tGeo.d3dEffectVelocity = temp.vGeometries[i].d3dEffectVelocity;
		if(temp.vGeometries[i].nRandVel)
			tGeo.d3dEffectVelocity += D3DXVECTOR3(((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f),((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f),((rand()%temp.vGeometries[i].nRandVel)/1.0f)-(temp.vGeometries[i].nRandVel/2.0f));
		tGeo.fLife = temp.vGeometries[i].fLife;
		newSub->ReCreate(tGeo);
	}
}