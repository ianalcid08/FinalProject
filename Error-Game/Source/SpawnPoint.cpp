/************************************************************************************
 * Filename:  		SpawnPoint.cpp
 * Date:      		02/16/2013
 * Mod. Date: 		02/16/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is what will spawn enemies.
 ************************************************************************************/
#include "SpawnPoint.h"
#include "GameplayState.h"
#include "Game.h"
#include <iostream>
CSpawnPoint::CSpawnPoint()
{
	m_fDuration = 0.0f;
	m_pObjectFactory = CGame::GetInstance()->GetOF();
	m_nCurrentSpawn = 0;
}

CSpawnPoint::~CSpawnPoint()
{

}

void CSpawnPoint::Reset()
{
	m_fDuration = 0.0f;
	m_nCurrentSpawn = 0;
}

void CSpawnPoint::Update(float fDelta)
{
	m_fDuration += fDelta;

	if(m_nCurrentSpawn < m_vSpawnObjects.size() && m_fDuration > m_vSpawnObjects[m_nCurrentSpawn].fSpawnTime)
	{
		eObjects nTempID = OBJ_NUM_OBJECTS;
		int nInputID = m_vSpawnObjects[m_nCurrentSpawn].nObjectID;

		switch(nInputID)
		{
		case 1:
			{
				// Crate
				nTempID = OBJ_CRATE;
			}
			break;
		case 2:
			{ 
				// Barrel
				nTempID = OBJ_BARREL;
			}
			break;
		case 5:
			{
				nTempID = OBJ_TRASH;
			}
			break;

		case 6:
			{
				nTempID = OBJ_PATROL;
			}
			break;

		case 7:
			{
				nTempID = OBJ_SMASH;
			}
			break;

		case 8:
			{
				nTempID = OBJ_BURN;
			}
			break;

		case 11:
			{
				nTempID = OBJ_SPIDER;
			}
			break;

		case 13:
			{
				nTempID = OBJ_INVERSION;
			}
			break;

		case 14:
			{
				nTempID = OBJ_CONVEYOR;
			}
			break;

		case 15:
			{
				nTempID = OBJ_VAT;
			}
			break;

		case 16:
			{
				nTempID = OBJ_CRATE;
			}
			break;

		case 17:
			{
				nTempID = OBJ_TURRET;
			}
			break;

		case 18:
			{
				nTempID = OBJ_BARREL;
			}
			break;

		case 20:
			{
				nTempID = OBJ_TUNNEL;
			}
			break;
		case 40:
			{
				m_nCurrentSpawn++;
				CGame::GetInstance()->GetVM()->FadeOut();
				CGameplayState::GetInstance()->SetPlayerTransition(true);
				return;
			}
			break;
		case 41:
			{
				m_nCurrentSpawn++;
				CGame::GetInstance()->GetVM()->FadeIn();
				CGameplayState::GetInstance()->ResetPlayers();
				CGameplayState::GetInstance()->SetPlayerTransition(false);
				return;
			}
			break;
		case 42:
			{
				m_nCurrentSpawn++;
				CGameplayState::GetInstance()->GetLeveData()->ToggleFoundry();
				return;
			}
			break;
		case 43:
			{
				m_nCurrentSpawn++;
				CGameplayState::GetInstance()->GetLeveData()->EnableMidbossFloor();
				return;
			}
			break;
		case 44:
			{
				m_nCurrentSpawn++;
				CGameplayState::GetInstance()->AllRobotsMustDance();
				return;
			}
			break;
		case 45:
			{
				m_nCurrentSpawn++;
				CGameplayState::GetInstance()->GetLeveData()->ToggleConveyors();
				return;
			}
			break;
		case 50:
			{
				m_nCurrentSpawn++;
				m_pObjectFactory->StartEvent(EVENT_AICORE);
				return;
			}
			break;

		case 51:
			{
				m_nCurrentSpawn++;
				m_pObjectFactory->StartEvent(EVENT_ASSBOSS);
				return;
			}
			break;

		case 52:
			{
				m_nCurrentSpawn++;
				m_pObjectFactory->StartEvent(EVENT_FOUNDBOSS);
				return;
			}
			break;
		}

		if( nTempID == OBJ_NUM_OBJECTS )
		{
#if _DEBUG
			std::cout << "BAD ID PASSED IN FROM TOOL" << std::endl;
#endif
			return;
		}

		IBaseObject* pTempObject;

		if( !m_pObjectFactory->Create(nTempID, &pTempObject) )
			return;


		D3DXMATRIX camera = CGameplayState::GetInstance()->GetCamera()->GetCamera();

		switch( nTempID )
		{
		case OBJ_TURRET:
			{
				D3DXMatrixTranslation(&camera, camera._41, -1.0f, camera._43);
			}
			break;

		case OBJ_INVERSION:
			{
				D3DXMatrixTranslation(&camera, camera._41, -2.25f, camera._43);
			}
			break;

		case OBJ_PATROL:
			{
				D3DXMatrixTranslation(&camera, camera._41, 10.0f, camera._43);
			}
			break;

		case OBJ_TUNNEL:
			{
				D3DXMatrixTranslation(&camera, camera._41, 0.0f, camera._43);
				D3DXMATRIX d3dTemp = m_d3dPosition*camera;
				D3DXMATRIX d3dCam = camera;
				d3dCam._43 += 6.5f;
				D3DXVECTOR3 x,y,z = (*(D3DXVECTOR3*)&d3dCam[12]) - (*(D3DXVECTOR3*)&d3dTemp[12]);
				D3DXVec3Normalize(&z,&z);
				D3DXVec3Cross(&x, &D3DXVECTOR3(0.0f,1.0f,0.0f),&z);
				D3DXVec3Normalize(&x,&x);
				D3DXVec3Cross(&y, &z,&x);
				D3DXVec3Normalize(&y,&y);
				*(D3DXVECTOR3*)&m_d3dPosition[0] = x;
				*(D3DXVECTOR3*)&m_d3dPosition[4] = y;
				*(D3DXVECTOR3*)&m_d3dPosition[8] = z;
			}
			break;

		default:
			{
				D3DXMatrixTranslation(&camera, camera._41, 0.0f, camera._43);
			}
			break;
		}

		pTempObject->SetMatrix(m_d3dPosition * camera);
		m_nCurrentSpawn++;		
		
		//if( nTempID == OBJ_AICORE )
		//{
		//	m_pObjectFactory->StartEvent(EVENT_AICORE);
		//}
		//else if(m_pObjectFactory->Create(nTempID, &pTempObject))
		//{
		//	D3DXMATRIX camera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
		//	if(nTempID == OBJ_TURRET)
		//		D3DXMatrixTranslation(&camera, camera._41, -1.0f, camera._43);
		//	else if(nTempID == OBJ_INVERSION)
		//		D3DXMatrixTranslation(&camera, camera._41, -2.25f, camera._43);
		//	else if(nTempID == OBJ_PATROL)
		//	{
		//		D3DXMatrixTranslation(&camera, camera._41, 10.0f, camera._43);
		//	}
		//	else if(nTempID == OBJ_TUNNEL)
		//	{
		//		D3DXMatrixTranslation(&camera, camera._41, 0.0f, camera._43);
		//		D3DXMATRIX d3dTemp = m_d3dPosition*camera;
		//		D3DXMATRIX d3dCam = camera;
		//		d3dCam._43 += 6.5f;
		//		D3DXVECTOR3 x,y,z = (*(D3DXVECTOR3*)&d3dCam[12]) - (*(D3DXVECTOR3*)&d3dTemp[12]);
		//		D3DXVec3Normalize(&z,&z);
		//		D3DXVec3Cross(&x, &D3DXVECTOR3(0.0f,1.0f,0.0f),&z);
		//		D3DXVec3Normalize(&x,&x);
		//		D3DXVec3Cross(&y, &z,&x);
		//		D3DXVec3Normalize(&y,&y);
		//		*(D3DXVECTOR3*)&m_d3dPosition[0] = x;
		//		*(D3DXVECTOR3*)&m_d3dPosition[4] = y;
		//		*(D3DXVECTOR3*)&m_d3dPosition[8] = z;
		//	}
		//	else
		//		D3DXMatrixTranslation(&camera, camera._41, 0.0f, camera._43);
		//	pTempObject->SetMatrix(m_d3dPosition * camera);
		//}
		
	}
}
