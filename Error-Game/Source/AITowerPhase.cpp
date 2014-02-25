#include "AITowerPhase.h"
#include "BossInversionTower.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "GameplayState.h"
#include "AIBehaviors.h"

CAITowerPhase::CAITowerPhase(void)
{
	m_fPickupTime = 0.0f;
	m_fTrashTime = 0.0f;
	m_fPatrolTime = 0.0f;
	m_pTower = nullptr;
	m_pOF = nullptr;
	m_nNumPlayers = 0;
	m_nTrashSpawned = 0;
	m_nPatrolSpawned = 0;
	m_nTrashTotal = 0;
	m_nPatrolTotal = 0;
}


CAITowerPhase::~CAITowerPhase(void)
{
}

void CAITowerPhase::Initialize()
{
	if( !m_pTower )
		return;

	m_fPickupTime = 0.0f;
	m_fTrashTime = 4.5f;
	m_fPatrolTime = 4.5f;
	m_nTrashSpawned = 0;
	m_nPatrolSpawned = 0;

	m_nNumPlayers = CGameplayState::GetInstance()->GetPlayers().size();

	m_nTrashTotal = min(m_nNumPlayers*3, 9);
	m_nPatrolTotal = min(m_nNumPlayers*2, 6);

	m_pOF = CGame::GetInstance()->GetOF();

	D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera(), d3dOffset;

	// insert replacement tower create code here
	CGameplayState::GetInstance()->GetHUD()->EnableBoss(m_pTower);

	m_pTower->SetIsActive(true);

	m_d3dCenter = *(D3DXVECTOR3*)&m_pTower->GetMatrix()[12];
	m_d3dCenter.y = 0.0f;

	// TODO - make real spawn points that move and not this hacked crap
	D3DXVECTOR3 d3dSpawn, d3dCamLoc(&d3dCamera[12]);

	// Left
	d3dSpawn.x = d3dCamLoc.x - 14.0f;
	d3dSpawn.z = d3dCamLoc.z + 4.0f;
	m_vSpawnPoints.push_back(d3dSpawn);

	// Right
	d3dSpawn.x = d3dCamLoc.x + 14.0f;
	d3dSpawn.z = d3dCamLoc.z + 4.0f;
	m_vSpawnPoints.push_back(d3dSpawn);

	// Top
	d3dSpawn.x = d3dCamLoc.x;
	d3dSpawn.z = d3dCamLoc.z + 15.0f;
	m_vSpawnPoints.push_back(d3dSpawn);

	// Bottom
	d3dSpawn.x = d3dCamLoc.x;
	d3dSpawn.z = d3dCamLoc.z -3.0f;
	m_vSpawnPoints.push_back(d3dSpawn);
	
}

void CAITowerPhase::Shutdown()
{
	CGameplayState::GetInstance()->GetHUD()->DisableBoss();
}

bool CAITowerPhase::Update(float _fTime)
{
	if( !m_pTower || !m_pTower->GetAlive() )
		return true;

	SpawnPickups(_fTime);
	SpawnTrash(_fTime);
	SpawnPatrol(_fTime);
	
	return false;
}

void CAITowerPhase::SpawnPickups(float _fTime)
{
	m_fPickupTime += _fTime;

	if( m_fPickupTime > 2.0f )
	{
		if( CGameplayState::GetInstance()->GeneratePickup() )
		{
			CPickup* pPickup = nullptr;
			if(m_pOF->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
			{
				float nXOff = rand()%23-9.0f, nZOff = (float)(rand()%12);
				D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
				d3dCamera._42 = 0.0f;
				D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);

				pPickup->SetMatrix(d3dCamera*d3dTranslate);
			}
		}

		m_fPickupTime = 0.0f;
	}
}

void CAITowerPhase::SpawnTrash(float _fTime)
{
	m_fTrashTime += _fTime;

	float fWait = 5.0f + .1f*m_nTrashSpawned;
	unsigned int nIndex = m_nTrashSpawned % m_vSpawnPoints.size();

	if( m_fTrashTime > fWait )
	{
		CTrashBot* pTrash = nullptr;

		if( m_pOF->Create(OBJ_TRASH, (IBaseObject**)&pTrash) )
		{
			D3DXMATRIX d3dTest = pTrash->GetMatrix();
			d3dTest._41 = m_vSpawnPoints[nIndex].x;
			d3dTest._43 = m_vSpawnPoints[nIndex].z;
			pTrash->SetMatrix(d3dTest);
			Movement::LookAt(pTrash, m_d3dCenter);
			++m_nTrashSpawned;
		}

		if( m_nTrashSpawned >= m_nTrashTotal )
		{
			m_nTrashSpawned = 0;
			m_fTrashTime = 0.0f;
		}
	}
}

void CAITowerPhase::SpawnPatrol(float _fTime)
{
	m_fPatrolTime += _fTime;

	float fWait = 8.0f + .1f*m_nPatrolSpawned;
	unsigned int nIndex = m_nPatrolSpawned % m_vSpawnPoints.size();

	if( m_fPatrolTime > fWait )
	{
		CPatrolBot* pPatrol = nullptr;

		if( m_pOF->Create(OBJ_PATROL, (IBaseObject**)&pPatrol) )
		{
			D3DXMATRIX d3dTest = pPatrol->GetMatrix();
			d3dTest._41 = m_vSpawnPoints[nIndex].x;
			d3dTest._43 = m_vSpawnPoints[nIndex].z;
			pPatrol->SetMatrix(d3dTest);
			++m_nPatrolSpawned;
		}

		if( m_nPatrolSpawned >= m_nPatrolTotal )
		{
			m_nPatrolSpawned = 0;
			m_fPatrolTime = 0.0f;
		}
	}
}