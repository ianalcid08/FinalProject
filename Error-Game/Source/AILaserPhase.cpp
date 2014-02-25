#include "AILaserPhase.h"
#include "Game.h"
#include "GameplayState.h"
#include "BossInversionTower.h"
#include "BossTurret.h"

bool AllTurretsDead(CBossTurret* _pTurrets);

CAILaserPhase::CAILaserPhase(void)
{
	m_pTower = nullptr;
	m_pOF = nullptr;
	m_nNumPlayers = 0;
	m_fTime = 0.0f;
	m_fPickupTime = 0.0f;

	// TURRET LOCATIONS IN CAMERA SPACE - TL, TR, BL, BR
	m_d3dTurretLoc[0] = D3DXVECTOR3(-12.0f, 0.0f, 12.0f);
	m_d3dTurretLoc[1] = D3DXVECTOR3(12.0f, 0.0f, 12.0f);
	m_d3dTurretLoc[2] = D3DXVECTOR3(-10.5f, 0.0f, 0.0f);
	m_d3dTurretLoc[3] = D3DXVECTOR3(10.5f, 0.0f, 0.0f);
}


CAILaserPhase::~CAILaserPhase(void)
{
}

void CAILaserPhase::Initialize()
{
	m_fPickupTime = 0.0f;
	m_fTime = 0.0f;
	m_pTower = nullptr;
	m_pOF = CGame::GetInstance()->GetOF();

	m_nNumPlayers = CGameplayState::GetInstance()->GetPlayers().size();
	D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera(), d3dTranslate, d3dID;

	float fRot[4] = { 180.0f, 0.0f, 0.0f, 180.0f };

	for( int i = 0; i < 4; ++i )
	{
		if( m_pOF->Create(OBJ_BOSSTURRET, (IBaseObject**)&m_pTurrets[i]) )
		{
			D3DXMATRIX d3dMat;
			D3DXMatrixTranslation(&d3dTranslate, d3dCamera._41 + m_d3dTurretLoc[i].x, -1.0f, d3dCamera._43 + m_d3dTurretLoc[i].z);
			D3DXMatrixIdentity(&d3dID);
			d3dMat = d3dID * d3dTranslate;

			if( fRot[i] )
			{
				D3DXMATRIX d3dRot;
				D3DXMatrixRotationY(&d3dRot, D3DXToRadian(fRot[i]));
				d3dMat = d3dRot * d3dMat;
			}

			m_pTurrets[i]->SetMatrix(d3dMat);


			// TODO - insert actual boss turret params here
		}
	}

	if( m_pOF->Create(OBJ_BOSSINV, (IBaseObject**)&m_pTower) )
	{
		D3DXMatrixTranslation(&d3dTranslate, d3dCamera._41, -2.25f, d3dCamera._43+6.0f);
		D3DXMatrixIdentity(&d3dID);
		m_pTower->SetMatrix(d3dID * d3dTranslate);
		m_pTower->SetMaxRadius(30.0f);
		m_pTower->SetAttackTime(6.0f);
		m_pTower->SetAttackSpeed(25.0f);
		m_pTower->SetHealth(2000.0f + 1500.0f*m_nNumPlayers);
	}
}

void CAILaserPhase::Shutdown()
{
	m_pTower = nullptr;
}

bool CAILaserPhase::Update(float _fTime)
{
	m_fPickupTime += _fTime;

	if( m_fPickupTime > (8.0f-CGameplayState::GetInstance()->GetPlayers().size()) )
	{
		CPickup* pPickup = nullptr;
		if(CGame::GetInstance()->GetOF()->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
		{
			float nXOff = rand()%23-9.0f, nZOff = (float)(rand()%12);
			D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
			d3dCamera._42 = 0.0f;
			D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);

			pPickup->SetMatrix(d3dCamera*d3dTranslate);
		}


		m_fPickupTime = 0.0f;
	}

	if(m_pTurrets[0]->GetBeingHit() || m_pTurrets[1]->GetBeingHit() || m_pTurrets[2]->GetBeingHit() || m_pTurrets[3]->GetBeingHit())
		m_pTower->SetShieldHit(true);

	return AllTurretsDead();
}

bool CAILaserPhase::AllTurretsDead()
{
	bool bAllTurretsDead = true;
	int nTurretsAlive = 0;
	if(m_pTurrets[0]->GetAlive())
	{
		bAllTurretsDead = false;
		nTurretsAlive++;
	}
	if(m_pTurrets[1]->GetAlive())
	{
		bAllTurretsDead = false;
		nTurretsAlive++;
	}
	if(m_pTurrets[2]->GetAlive())
	{
		bAllTurretsDead = false;
		nTurretsAlive++;
	}
	if(m_pTurrets[3]->GetAlive())
	{
		bAllTurretsDead = false;
		nTurretsAlive++;
	}
	switch(nTurretsAlive)
	{
	case 1:
		m_pTower->SetColor(D3DXVECTOR4(1.0f,0.0f,0.0f,0.25f));
		break;
	case 2:
		m_pTower->SetColor(D3DXVECTOR4(1.0f,0.5f,0.0f,0.5f));
		break;
	case 3:
		m_pTower->SetColor(D3DXVECTOR4(1.0f,1.0f,0.0f,0.75f));
		break;
	default:
		m_pTower->SetColor(D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f));
		break;
	}
	return bAllTurretsDead;
}