#include "ObjectFactory.h"

#include "IBaseObject.h"
#include "Game.h"
#include "AIBehaviors.h"
#include "GameplayState.h"

/***********************************************
* Filename:  		ObjectFactory.cpp
* Date:      		02/09/2013
* Mod. Date: 		02/12/2013
* Mod. Initials:	JT
* Author:    		Joseph Thomas
* Purpose:   		Handles creating and destroying all objects
************************************************/

CObjectFactory::CObjectFactory(void)
{
	m_pOM = nullptr;
	m_pVM = nullptr;
}

CObjectFactory::~CObjectFactory(void)
{
	
}

void CObjectFactory::Initialize(CObjectManager* _pOM, CViewManager* _pVM)
{
	m_pOM = _pOM;
	m_pVM = _pVM;

	for( unsigned int i = 0; i < MAX_PLAYERS; ++i )
		m_vOpen[OBJ_PLAYER].push_back(i);

	for( unsigned int i = 0; i < MAX_BULLETS_PLAYER; ++i )
	{
		m_cPlayerBullets[i].SetID(OBJ_BULLET_PLAYER);
		m_vOpen[OBJ_BULLET_PLAYER].push_back(i);
	}

	for( unsigned int i = 0; i < MAX_BULLETS_ENEMY; ++i )
	{
		m_cEnemyBullets[i].SetID(OBJ_BULLET_ENEMY);
		m_vOpen[OBJ_BULLET_ENEMY].push_back(i);
	}

	for( unsigned int i = 0; i < MAX_BULLETS_EXP; ++i )
		m_vOpen[OBJ_BULLET_EXP].push_back(i);

	for( unsigned int i = 0; i < MAX_BULLETS_TESLA; ++i )
		m_vOpen[OBJ_BULLET_TESLA].push_back(i);

	for( unsigned int i = 0; i < MAX_BULLETS_BEAM; ++i )
		m_vOpen[OBJ_BULLET_BEAM].push_back(i);

	for( unsigned int i = 0; i < MAX_BULLETS_ARC; ++i )
		m_vOpen[OBJ_BULLET_ARC].push_back(i);

	for( unsigned int i = 0; i < MAX_PICKUP; ++i )
		m_vOpen[OBJ_PICKUP].push_back(i);

	for( unsigned int i = 0; i < MAX_TRASH; ++i )
		m_vOpen[OBJ_TRASH].push_back(i);

	for( unsigned int i = 0; i < MAX_PATROL; ++i )
		m_vOpen[OBJ_PATROL].push_back(i);

	for( unsigned int i = 0; i < MAX_SMASH; ++i )
		m_vOpen[OBJ_SMASH].push_back(i);

	for( unsigned int i = 0; i < MAX_BURN; ++i )
		m_vOpen[OBJ_BURN].push_back(i);

	for( unsigned int i = 0; i < MAX_SPIDER; ++i )
		m_vOpen[OBJ_SPIDER].push_back(i);

	for( unsigned int i = 0; i < MAX_STEAM; ++i )
		m_vOpen[OBJ_STEAM].push_back(i);

	for( unsigned int i = 0; i < MAX_AICORE; ++i )
		m_vOpen[OBJ_AICORE].push_back(i);

	for( unsigned int i = 0; i < MAX_AIFIST; ++i )
		m_vOpen[OBJ_AIFIST].push_back(i);

	for( unsigned int i = 0; i < MAX_CONVEYOR; ++i )
		m_vOpen[OBJ_CONVEYOR].push_back(i);

	for( unsigned int i = 0; i < MAX_VAT; ++i )
		m_vOpen[OBJ_VAT].push_back(i);

	for( unsigned int i = 0; i < MAX_CRATE; ++i )
		m_vOpen[OBJ_CRATE].push_back(i);

	for( unsigned int i = 0; i < MAX_BARREL; ++i )
		m_vOpen[OBJ_BARREL].push_back(i);

	for( unsigned int i = 0; i < MAX_POOL; ++i )
		m_vOpen[OBJ_POOL].push_back(i);
	for(unsigned int i = 0; i < MAX_TURRETS; ++i)
		m_vOpen[OBJ_TURRET].push_back(i);
	for(unsigned int i = 0; i < MAX_INVERSION; ++i)
		m_vOpen[OBJ_INVERSION].push_back(i);
	for(unsigned int i = 0; i < MAX_INVERSION; ++i)
		m_vOpen[OBJ_TUNNEL].push_back(i);

	for(unsigned int i = 0; i < MAX_PITS; ++i)
		m_vOpen[OBJ_PIT].push_back(i);

	for( unsigned int i = 0; i < MAX_BOSSINV; ++i )
		m_vOpen[OBJ_BOSSINV].push_back(i);

	for( unsigned int i = 0; i < MAX_BOSSTURRET; ++i )
		m_vOpen[OBJ_BOSSTURRET].push_back(i);

	for( unsigned int i = 0; i < MAX_SPINBOT; ++i )
		m_vOpen[OBJ_SPINBOT].push_back(i);

	for( unsigned int i = 0; i < MAX_FLOOR; ++i )
		m_vOpen[OBJ_FLOOR].push_back(i);
}

bool CObjectFactory::Create(eObjects _eObjectID, IBaseObject** _pOut)
{
	if( !m_vOpen[_eObjectID].size() )
		return false;

	unsigned int nIndex = m_vOpen[_eObjectID].back();
	m_vOpen[_eObjectID].pop_back();

	switch(_eObjectID)
	{
	case OBJ_PLAYER:
		{
			MakeDefault(m_cPlayers[nIndex]);
			*_pOut = &m_cPlayers[nIndex];
			m_pOM->AddObject(&m_cPlayers[nIndex], OM_PLAYERS);
			m_pVM->AddObject(&m_cPlayers[nIndex], RG_MODELS);
		}
		break;
	
	case OBJ_BULLET_PLAYER:
		{
			MakeDefault(m_cPlayerBullets[nIndex]);
			*_pOut = &m_cPlayerBullets[nIndex];
			m_pOM->AddObject(&m_cPlayerBullets[nIndex], OM_PLAYER_BULLETS);
			m_pVM->AddObject(&m_cPlayerBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_BULLET_ENEMY:
		{
			MakeDefault(m_cEnemyBullets[nIndex]);
			*_pOut = &m_cEnemyBullets[nIndex];
			m_pOM->AddObject(&m_cEnemyBullets[nIndex], OM_ENEMY_BULLETS);
			m_pVM->AddObject(&m_cEnemyBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_BULLET_EXP:
		{
			MakeDefault(m_cExplodingBullets[nIndex]);
			*_pOut = &m_cExplodingBullets[nIndex];
			m_pOM->AddObject(&m_cExplodingBullets[nIndex], OM_PLAYER_BULLETS);
			m_pOM->AddObject(&m_cExplodingBullets[nIndex], OM_ENEMY_BULLETS);
			m_pVM->AddObject(&m_cExplodingBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_BULLET_TESLA:
		{
			MakeDefault(m_cTeslaBullets[nIndex]);
			*_pOut = &m_cTeslaBullets[nIndex];
			m_pOM->AddObject(&m_cTeslaBullets[nIndex], OM_PLAYER_BULLETS);
			m_pVM->AddObject(&m_cTeslaBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_BULLET_BEAM:
		{
			MakeDefault(m_cBeamBullets[nIndex]);
			*_pOut = &m_cBeamBullets[nIndex];
			m_pOM->AddObject(&m_cBeamBullets[nIndex], OM_PLAYER_BULLETS);
			m_pVM->AddObject(&m_cBeamBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_BULLET_ARC:
		{
			MakeDefault(m_cArcBullets[nIndex]);
			*_pOut = &m_cArcBullets[nIndex];
			m_pOM->AddObject(&m_cArcBullets[nIndex], OM_ENEMY_BULLETS);
			m_pVM->AddObject(&m_cArcBullets[nIndex], RG_BULLETS);
		}
		break;

	case OBJ_PICKUP:
		{
			MakeDefault(m_cPickups[nIndex]);
			*_pOut = &m_cPickups[nIndex];
			m_pOM->AddObject(&m_cPickups[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cPickups[nIndex], RG_ENVIRONMENT);
		}
		break;
	
	case OBJ_TRASH:
		{
			MakeDefault(m_cTrashBots[nIndex]);
			*_pOut = &m_cTrashBots[nIndex];
			m_pOM->AddObject(&m_cTrashBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cTrashBots[nIndex], RG_MODELS);
		}
		break;

	case OBJ_PATROL:
		{
			MakeDefault(m_cPatrolBots[nIndex]);
			*_pOut = &m_cPatrolBots[nIndex];
			m_pOM->AddObject(&m_cPatrolBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cPatrolBots[nIndex], RG_MODELS); 
		}
		break;

	case OBJ_SMASH:
		{
			MakeDefault(m_cSmashBots[nIndex]);
			*_pOut = &m_cSmashBots[nIndex];
			m_pOM->AddObject(&m_cSmashBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cSmashBots[nIndex], RG_MODELS);
		}
		break;

	case OBJ_BURN:
		{
			MakeDefault(m_cBurnBots[nIndex]);
			*_pOut = &m_cBurnBots[nIndex];
			m_pOM->AddObject(&m_cBurnBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cBurnBots[nIndex], RG_MODELS);
		}
		break;

	case OBJ_SPIDER:
		{
			MakeDefault(m_cSpiderBots[nIndex]);
			*_pOut = &m_cSpiderBots[nIndex];
			m_pOM->AddObject(&m_cSpiderBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cSpiderBots[nIndex], RG_MODELS);
		}
		break;

	case OBJ_STEAM:
		{
			MakeDefault(m_cSteamrollerBots[nIndex]);
			*_pOut = &m_cSteamrollerBots[nIndex];
			m_pOM->AddObject(&m_cSteamrollerBots[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cSteamrollerBots[nIndex], RG_MODELS);
		}
		break;

	case OBJ_SPINBOT:
		{
			MakeDefault(m_cSpinBot[nIndex]);
			*_pOut = &m_cSpinBot[nIndex];
			m_pOM->AddObject(&m_cSpinBot[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cSpinBot[nIndex], RG_MODELS);
		}
		break;

	case OBJ_AICORE:
		{
			MakeDefault(m_cAICores[nIndex]);
			*_pOut = &m_cAICores[nIndex];
			m_pOM->AddObject(&m_cAICores[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cAICores[nIndex], RG_MODELS);
		}
		break;

	case OBJ_AIFIST:
		{
			MakeDefault(m_cAIFists[nIndex]);
			*_pOut = &m_cAIFists[nIndex];
			m_pOM->AddObject(&m_cAIFists[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cAIFists[nIndex], RG_MODELS);
		}
		break;

	case OBJ_CONVEYOR:
		{
			*_pOut = &m_cConveyors[nIndex];
			m_pOM->AddObject(&m_cConveyors[nIndex], OM_ENVIRONMENT);
		}
		break;

	case OBJ_VAT:
		{
			MakeDefault(m_cVats[nIndex]);
			*_pOut = &m_cVats[nIndex];
			m_pOM->AddObject(&m_cVats[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cVats[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_CRATE:
		{
			MakeDefault(m_cCrates[nIndex]);
			*_pOut = &m_cCrates[nIndex];
			m_pOM->AddObject(&m_cCrates[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cCrates[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_BARREL:
		{
			MakeDefault(m_cBarrels[nIndex]);
			*_pOut = &m_cBarrels[nIndex];
			m_pOM->AddObject(&m_cBarrels[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cBarrels[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_POOL:
		{
			MakeDefault(m_cPools[nIndex]);
			*_pOut = &m_cPools[nIndex];
			m_pOM->AddObject(&m_cPools[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cPools[nIndex], RG_ENVIRONMENT);
		}
		break;
	case OBJ_TURRET:
		{
			MakeDefault(m_cTurrets[nIndex]);
			*_pOut = &m_cTurrets[nIndex];
			m_pOM->AddObject(&m_cTurrets[nIndex],OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cTurrets[nIndex],RG_ENVIRONMENT);
		}
		break;
	case OBJ_INVERSION:
		{
			MakeDefault(m_cInversion[nIndex]);
			*_pOut = &m_cInversion[nIndex];
			m_pOM->AddObject(&m_cInversion[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cInversion[nIndex], RG_ENVIRONMENT);
		}
		break;
	case OBJ_TUNNEL:
		{
			MakeDefault(m_cTunnel[nIndex]);
			*_pOut = &m_cTunnel[nIndex];
			m_pOM->AddObject(&m_cTunnel[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cTunnel[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_BOSSINV:
		{
			MakeDefault(m_cBossInv[nIndex]);
			*_pOut = &m_cBossInv[nIndex];
			m_pOM->AddObject(&m_cBossInv[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cBossInv[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_BOSSTURRET:
		{
			MakeDefault(m_cBossTurrets[nIndex]);
			*_pOut = &m_cBossTurrets[nIndex];
			m_pOM->AddObject(&m_cBossTurrets[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cBossTurrets[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_PIT:
		{
			MakeDefault(m_cDeathPits[nIndex]);
			*_pOut = &m_cDeathPits[nIndex];
			m_pOM->AddObject(&m_cDeathPits[nIndex], OM_ENVIRONMENT);
			m_pVM->AddObject(&m_cDeathPits[nIndex], RG_ENVIRONMENT);
		}
		break;

	case OBJ_FLOOR:
		{
			MakeDefault(m_cFloor[nIndex]);
			*_pOut = &m_cFloor[nIndex];
			m_pOM->AddObject(&m_cFloor[nIndex], OM_ENEMIES);
			m_pVM->AddObject(&m_cFloor[nIndex], RG_ENVIRONMENT);
		}
		break;
	}

	return true;
}

bool CObjectFactory::Destroy(IBaseObject* _pObject)
{
	if( !_pObject )
		return false;

	int nTest = -1;

	switch( _pObject->GetID() )
	{
	case OBJ_PLAYER:
		{
			nTest = ((int)_pObject-(int)&m_cPlayers[0])/sizeof(CPlayerObject);
			if( nTest < 0 || nTest >= MAX_PLAYERS )
				return false;
			else if( !CheckValid(OBJ_PLAYER, nTest) )
				return false;

			m_vDestroyed[OBJ_PLAYER].push_back(&m_cPlayers[nTest]);
		}
		break;

	case OBJ_BULLET_PLAYER:
		{
			nTest = ((int)_pObject-(int)&m_cPlayerBullets)/sizeof(CBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_PLAYER )
				return false;
			else if( !CheckValid(OBJ_BULLET_PLAYER, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_PLAYER].push_back(&m_cPlayerBullets[nTest]);
		}
		break;

	case OBJ_BULLET_ENEMY:
		{
			nTest = ((int)_pObject-(int)&m_cEnemyBullets[0])/sizeof(CBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_ENEMY )
				return false;
			else if( !CheckValid(OBJ_BULLET_ENEMY, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_ENEMY].push_back(&m_cEnemyBullets[nTest]);
		}
		break;

	case OBJ_BULLET_EXP:
		{
			nTest = ((int)_pObject-(int)&m_cExplodingBullets[0])/sizeof(CExplodingBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_EXP )
				return false;
			else if( !CheckValid(OBJ_BULLET_EXP, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_EXP].push_back(&m_cExplodingBullets[nTest]);
		}
		break;

	case OBJ_BULLET_TESLA:
		{
			nTest = ((int)_pObject-(int)&m_cTeslaBullets[0])/sizeof(CTeslaBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_TESLA )
				return false;
			else if( !CheckValid(OBJ_BULLET_TESLA, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_TESLA].push_back(&m_cTeslaBullets[nTest]);
		}
		break;

	case OBJ_BULLET_BEAM:
		{
			nTest = ((int)_pObject-(int)&m_cBeamBullets[0])/sizeof(CBeamBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_BEAM )
				return false;
			else if( !CheckValid(OBJ_BULLET_BEAM, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_BEAM].push_back(&m_cBeamBullets[nTest]);
		}
		break;

	case OBJ_BULLET_ARC:
		{
			nTest = ((int)_pObject-(int)&m_cArcBullets[0])/sizeof(CArcBullet);
			if( nTest < 0 || nTest >= MAX_BULLETS_ARC )
				return false;
			else if( !CheckValid(OBJ_BULLET_ARC, nTest) )
				return false;

			m_vDestroyed[OBJ_BULLET_ARC].push_back(&m_cArcBullets[nTest]);
		}
		break;

	case OBJ_PICKUP:
		{
			nTest = ((int)_pObject-(int)&m_cPickups[0])/sizeof(CPickup);
			if( nTest < 0 || nTest >= MAX_PICKUP )
				return false;
			else if( !CheckValid(OBJ_PICKUP, nTest) )
				return false;

			m_vDestroyed[OBJ_PICKUP].push_back(&m_cPickups[nTest]);
		}
		break;

	case OBJ_TRASH:
		{
			nTest = ((int)_pObject-(int)&m_cTrashBots[0])/sizeof(CTrashBot);
			if( nTest < 0 || nTest >= MAX_TRASH )
				return false;
			else if( !CheckValid(OBJ_TRASH, nTest) )
				return false;

			m_vDestroyed[OBJ_TRASH].push_back(&m_cTrashBots[nTest]);
		}
		break;

	case OBJ_PATROL:
		{
			nTest = ((int)_pObject-(int)&m_cPatrolBots[0])/sizeof(CPatrolBot);
			if( nTest < 0 || nTest >= MAX_PATROL )
				return false;
			else if( !CheckValid(OBJ_PATROL, nTest) )
				return false;

			m_vDestroyed[OBJ_PATROL].push_back(&m_cPatrolBots[nTest]);
		}
		break;

	case OBJ_SMASH:
		{
			nTest = ((int)_pObject-(int)&m_cSmashBots[0])/sizeof(CSmashBot);
			if( nTest < 0 || nTest >= MAX_SMASH )
				return false;
			else if( !CheckValid(OBJ_SMASH, nTest) )
				return false;

			m_vDestroyed[OBJ_SMASH].push_back(&m_cSmashBots[nTest]);
		}
		break;

	case OBJ_BURN:
		{
			nTest = ((int)_pObject-(int)&m_cBurnBots[0])/sizeof(CBurnBot);
			if( nTest < 0 || nTest >= MAX_BURN )
				return false;
			else if( !CheckValid(OBJ_BURN, nTest) )
				return false;

			m_vDestroyed[OBJ_BURN].push_back(&m_cBurnBots[nTest]);
		}
		break;

	case OBJ_SPIDER:
		{
			nTest = ((int)_pObject-(int)&m_cSpiderBots[0])/sizeof(CSpiderBot);
			if( nTest < 0 || nTest >= MAX_SPIDER )
				return false;
			else if( !CheckValid(OBJ_SPIDER, nTest) )
				return false;

			m_vDestroyed[OBJ_SPIDER].push_back(&m_cSpiderBots[nTest]);
		}
		break;

	case OBJ_STEAM:
		{
			nTest = ((int)_pObject-(int)&m_cSteamrollerBots[0])/sizeof(CSteamrollerBot);
			if( nTest < 0 || nTest >= MAX_STEAM )
				return false;
			else if( !CheckValid(OBJ_STEAM, nTest) )
				return false;

			m_vDestroyed[OBJ_STEAM].push_back(&m_cSteamrollerBots[nTest]);
		}
		break;

	case OBJ_SPINBOT:
		{
			nTest = ((int)_pObject-(int)&m_cSpinBot[0])/sizeof(CSpinBot);
			if( nTest < 0 || nTest >= MAX_SPINBOT )
				return false;
			else if( !CheckValid(OBJ_SPINBOT, nTest) )
				return false;

			m_vDestroyed[OBJ_SPINBOT].push_back(&m_cSpinBot[nTest]);
		}
		break;

	case OBJ_AICORE:
		{
			nTest = ((int)_pObject-(int)&m_cAICores[0])/sizeof(CAICore);
			if( nTest < 0 || nTest >= MAX_AICORE )
				return false;
			else if( !CheckValid(OBJ_AICORE, nTest) )
				return false;

			m_vDestroyed[OBJ_AICORE].push_back(&m_cAICores[nTest]);
		}
		break;

	case OBJ_AIFIST:
		{
			nTest = ((int)_pObject-(int)&m_cAIFists[0])/sizeof(CAIFist);
			if( nTest < 0 || nTest >= MAX_AIFIST )
				return false;
			else if( !CheckValid(OBJ_AIFIST, nTest) )
				return false;

			m_vDestroyed[OBJ_AIFIST].push_back(&m_cAIFists[nTest]);
		}
		break;

	case OBJ_CONVEYOR:
		{
			nTest = ((int)_pObject-(int)&m_cConveyors[0])/sizeof(CConveyor);
			if( nTest < 0 || nTest > MAX_CONVEYOR )
				return false;
			else if( !CheckValid(OBJ_CONVEYOR, nTest) )
				return false;

			m_vDestroyed[OBJ_CONVEYOR].push_back(&m_cConveyors[nTest]);
		}
		break;

	case OBJ_VAT:
		{
			nTest = ((int)_pObject-(int)&m_cVats[0])/sizeof(CVat);
			if( nTest < 0 || nTest > MAX_VAT )
				return false;
			else if( !CheckValid(OBJ_VAT, nTest) )
				return false;

			m_vDestroyed[OBJ_VAT].push_back(&m_cVats[nTest]);
		}
		break;

	case OBJ_CRATE:
		{
			nTest = ((int)_pObject-(int)&m_cCrates[0])/sizeof(CCrate);
			if( nTest < 0 || nTest > MAX_CRATE )
				return false;
			else if( !CheckValid(OBJ_CRATE, nTest) )
				return false;

			m_vDestroyed[OBJ_CRATE].push_back(&m_cCrates[nTest]);
		}
		break;

	case OBJ_BARREL:
		{
			nTest = ((int)_pObject-(int)&m_cBarrels[0])/sizeof(CBarrel);
			if( nTest < 0 || nTest > MAX_BARREL )
				return false;
			else if( !CheckValid(OBJ_BARREL, nTest) )
				return false;

			m_vDestroyed[OBJ_BARREL].push_back(&m_cBarrels[nTest]);
		}
		break;

	case OBJ_POOL:
		{
			nTest = ((int)_pObject-(int)&m_cPools[0])/sizeof(CMoltenPool);
			if( nTest < 0 || nTest > MAX_POOL )
				return false;
			else if( !CheckValid(OBJ_POOL, nTest) )
				return false;

			m_vDestroyed[OBJ_POOL].push_back(&m_cPools[nTest]);
		}
		break;
	case OBJ_TURRET:
		{
			nTest = ((int)_pObject-(int)&m_cTurrets[0])/sizeof(CTurret);
			if( nTest < 0 || nTest > MAX_TURRETS )
				return false;
			else if( !CheckValid(OBJ_TURRET, nTest) )
				return false;

			m_vDestroyed[OBJ_TURRET].push_back(&m_cTurrets[nTest]);
		}
		break;
	case OBJ_INVERSION:
		{
			nTest = ((int)_pObject-(int)&m_cInversion[0])/sizeof(CInversionTower);
			if( nTest < 0 || nTest > MAX_INVERSION )
				return false;
			else if( !CheckValid(OBJ_INVERSION, nTest) )
				return false;

			m_vDestroyed[OBJ_INVERSION].push_back(&m_cInversion[nTest]);
		}
		break;

	case OBJ_BOSSINV:
		{
			nTest = ((int)_pObject-(int)&m_cBossInv[0])/sizeof(CBossInversionTower);
			if( nTest < 0 || nTest > MAX_BOSSINV )
				return false;
			else if( !CheckValid(OBJ_BOSSINV, nTest) )
				return false;

			m_vDestroyed[OBJ_BOSSINV].push_back(&m_cBossInv[nTest]);
		}
		break;

	case OBJ_BOSSTURRET:
		{
			nTest = ((int)_pObject-(int)&m_cBossTurrets[0])/sizeof(CBossTurret);
			if( nTest < 0 || nTest > MAX_BOSSTURRET )
				return false;
			else if( !CheckValid(OBJ_BOSSTURRET, nTest) )
				return false;

			m_vDestroyed[OBJ_BOSSTURRET].push_back(&m_cBossTurrets[nTest]);
		}
		break;

	case OBJ_TUNNEL:
		{
			nTest = ((int)_pObject-(int)&m_cTunnel[0])/sizeof(CTrashSpawner);
			if( nTest < 0 || nTest > OBJ_TUNNEL )
				return false;
			else if( !CheckValid(OBJ_TUNNEL, nTest) )
				return false;

			m_vDestroyed[OBJ_TUNNEL].push_back(&m_cTunnel[nTest]);
		}
		break;
	case OBJ_PIT:
		{
			nTest = ((int)_pObject-(int)&m_cDeathPits[0])/sizeof(CDeathPit);
			if(nTest < 0 || nTest > MAX_PITS)
				return false;
			else if( !CheckValid(OBJ_PIT,nTest) )
				return false;

			m_vDestroyed[OBJ_PIT].push_back(&m_cDeathPits[nTest]);
		}
		break;

	case OBJ_FLOOR:
		{
			nTest = ((int)_pObject-(int)&m_cFloor[0])/sizeof(CFloorObject);
			if( nTest < 0 || nTest > MAX_FLOOR)
				return false;
			else if( !CheckValid(OBJ_FLOOR, nTest) )
				return false;

			m_vDestroyed[OBJ_FLOOR].push_back(&m_cFloor[nTest]);
		}
		break;
	}

	return true;
}

void CObjectFactory::ProcessDestroy()
{
	unsigned int nSize = 0, nIndex = 0;

	// Players
	nSize = m_vDestroyed[OBJ_PLAYER].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_PLAYER][i]-(int)&m_cPlayers[0])/sizeof(CPlayerObject);
		m_vOpen[OBJ_PLAYER].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_PLAYER][i], OM_PLAYERS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_PLAYER][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_PLAYER].clear();

	// Player Bullets
	nSize = m_vDestroyed[OBJ_BULLET_PLAYER].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_PLAYER][i]-(int)&m_cPlayerBullets[0])/sizeof(CBullet);
		m_vOpen[OBJ_BULLET_PLAYER].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_PLAYER][i], OM_PLAYER_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_PLAYER][i], RG_BULLETS);
	}

	m_vDestroyed[OBJ_BULLET_PLAYER].clear();

	// Enemy Bullets
	nSize = m_vDestroyed[OBJ_BULLET_ENEMY].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_ENEMY][i]-(int)&m_cEnemyBullets[0])/sizeof(CBullet);
		m_vOpen[OBJ_BULLET_ENEMY].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_ENEMY][i], OM_ENEMY_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_ENEMY][i], RG_BULLETS);
	}
	
	m_vDestroyed[OBJ_BULLET_ENEMY].clear();

	// Exploding Bullets
	nSize = m_vDestroyed[OBJ_BULLET_EXP].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_EXP][i]-(int)&m_cExplodingBullets[0])/sizeof(CExplodingBullet);
		m_vOpen[OBJ_BULLET_EXP].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_EXP][i], OM_PLAYER_BULLETS);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_EXP][i], OM_ENEMY_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_EXP][i], RG_BULLETS);
	}

	m_vDestroyed[OBJ_BULLET_EXP].clear();

	// Tesla Bullets
	nSize = m_vDestroyed[OBJ_BULLET_TESLA].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_TESLA][i]-(int)&m_cTeslaBullets[0])/sizeof(CTeslaBullet);
		m_vOpen[OBJ_BULLET_TESLA].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_TESLA][i], OM_PLAYER_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_TESLA][i], RG_BULLETS);
	}

	m_vDestroyed[OBJ_BULLET_TESLA].clear();

	// Beam Bullets
	nSize = m_vDestroyed[OBJ_BULLET_BEAM].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_BEAM][i]-(int)&m_cBeamBullets[0])/sizeof(CBeamBullet);
		m_vOpen[OBJ_BULLET_BEAM].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_BEAM][i], OM_PLAYER_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_BEAM][i], RG_BULLETS);
	}

	m_vDestroyed[OBJ_BULLET_BEAM].clear();

	// Arc Bullets
	nSize = m_vDestroyed[OBJ_BULLET_ARC].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BULLET_ARC][i]-(int)&m_cArcBullets[0])/sizeof(CArcBullet);
		m_vOpen[OBJ_BULLET_ARC].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BULLET_ARC][i], OM_ENEMY_BULLETS);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BULLET_ARC][i], RG_BULLETS);
	}

	m_vDestroyed[OBJ_BULLET_ARC].clear();

	// Pickups
	nSize = m_vDestroyed[OBJ_PICKUP].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_PICKUP][i]-(int)&m_cPickups[0])/sizeof(CPickup);
		m_vOpen[OBJ_PICKUP].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_PICKUP][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_PICKUP][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_PICKUP].clear();

	// Trash Bot
	nSize = m_vDestroyed[OBJ_TRASH].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_TRASH][i]-(int)&m_cTrashBots[0])/sizeof(CTrashBot);
		m_vOpen[OBJ_TRASH].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_TRASH][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_TRASH][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_TRASH].clear();	

	// Patrol Bot
	nSize = m_vDestroyed[OBJ_PATROL].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_PATROL][i]-(int)&m_cPatrolBots[0])/sizeof(CPatrolBot); 
		m_vOpen[OBJ_PATROL].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_PATROL][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_PATROL][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_PATROL].clear();

	// Smash Bot
	nSize = m_vDestroyed[OBJ_SMASH].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_SMASH][i]-(int)&m_cSmashBots[0])/sizeof(CSmashBot);
		m_vOpen[OBJ_SMASH].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_SMASH][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_SMASH][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_SMASH].clear();

	// Burn Bot
	nSize = m_vDestroyed[OBJ_BURN].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BURN][i]-(int)&m_cBurnBots[0])/sizeof(CBurnBot);
		m_vOpen[OBJ_BURN].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BURN][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BURN][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_BURN].clear();

	// Spider Bot
	nSize = m_vDestroyed[OBJ_SPIDER].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_SPIDER][i]-(int)&m_cSpiderBots[0])/sizeof(CSpiderBot);
		m_vOpen[OBJ_SPIDER].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_SPIDER][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_SPIDER][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_SPIDER].clear();

	// Steamroller Bot
	nSize = m_vDestroyed[OBJ_STEAM].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_STEAM][i]-(int)&m_cSteamrollerBots[0])/sizeof(CSteamrollerBot);
		m_vOpen[OBJ_STEAM].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_STEAM][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_STEAM][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_STEAM].clear();

	// Spin Bot
	nSize = m_vDestroyed[OBJ_SPINBOT].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_SPINBOT][i]-(int)&m_cSpinBot[0])/sizeof(CSpinBot);
		m_vOpen[OBJ_SPINBOT].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_SPINBOT][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_SPINBOT][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_SPINBOT].clear();

	// AI Core
	nSize = m_vDestroyed[OBJ_AICORE].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_AICORE][i]-(int)&m_cAICores[0])/sizeof(CAICore);
		m_vOpen[OBJ_AICORE].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_AICORE][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_AICORE][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_AICORE].clear();

	// AI Fist
	nSize = m_vDestroyed[OBJ_AIFIST].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_AIFIST][i]-(int)&m_cAIFists[0])/sizeof(CAIFist);
		m_vOpen[OBJ_AIFIST].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_AIFIST][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_AIFIST][i], RG_MODELS);
	}

	m_vDestroyed[OBJ_AIFIST].clear();

	// Conveyors
	nSize = m_vDestroyed[OBJ_CONVEYOR].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_CONVEYOR][i]-(int)&m_cConveyors[0])/sizeof(CConveyor);
		m_vOpen[OBJ_CONVEYOR].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_CONVEYOR][i], OM_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_CONVEYOR].clear();

	// Vats
	nSize = m_vDestroyed[OBJ_VAT].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_VAT][i]-(int)&m_cVats[0])/sizeof(CVat);
		m_vOpen[OBJ_VAT].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_VAT][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_VAT][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_VAT].clear();

	// Crates
	nSize = m_vDestroyed[OBJ_CRATE].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_CRATE][i]-(int)&m_cCrates[0])/sizeof(CCrate);
		m_vOpen[OBJ_CRATE].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_CRATE][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_CRATE][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_CRATE].clear();

	// Barrel
	nSize = m_vDestroyed[OBJ_BARREL].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BARREL][i]-(int)&m_cBarrels[0])/sizeof(CBarrel);
		m_vOpen[OBJ_BARREL].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BARREL][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BARREL][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_BARREL].clear();

	// Molten Pool
	nSize = m_vDestroyed[OBJ_POOL].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_POOL][i]-(int)&m_cPools[0])/sizeof(CMoltenPool);
		m_vOpen[OBJ_POOL].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_POOL][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_POOL][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_POOL].clear();

	// Turrets
	nSize = m_vDestroyed[OBJ_TURRET].size();
	for(unsigned int i = 0; i < nSize; ++i)
	{
		nIndex = ((int)m_vDestroyed[OBJ_TURRET][i]-(int)&m_cTurrets[0])/sizeof(CTurret);
		m_vOpen[OBJ_TURRET].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_TURRET][i],OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_TURRET][i],RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_TURRET].clear();

	//Inversion Towers
	nSize = m_vDestroyed[OBJ_INVERSION].size();
	for(unsigned int i = 0; i < nSize; ++i)
	{
		nIndex = ((int)m_vDestroyed[OBJ_INVERSION][i]-(int)&m_cInversion[0])/sizeof(CInversionTower);
		m_vOpen[OBJ_INVERSION].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_INVERSION][i],OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_INVERSION][i],RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_INVERSION].clear();

	// Boss Inversion Tower
	nSize = m_vDestroyed[OBJ_BOSSINV].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BOSSINV][i]-(int)&m_cBossInv[0])/sizeof(CBossInversionTower);
		m_vOpen[OBJ_BOSSINV].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BOSSINV][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BOSSINV][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_BOSSINV].clear();

	// Boss Turrets
	nSize = m_vDestroyed[OBJ_BOSSTURRET].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_BOSSTURRET][i]-(int)&m_cBossTurrets[0])/sizeof(CBossTurret);
		m_vOpen[OBJ_BOSSTURRET].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_BOSSTURRET][i], OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_BOSSTURRET][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_BOSSTURRET].clear();

	//Trash Bot Spawners
	nSize = m_vDestroyed[OBJ_TUNNEL].size();
	for(unsigned int i = 0; i < nSize; ++i)
	{
		nIndex = ((int)m_vDestroyed[OBJ_TUNNEL][i]-(int)&m_cTunnel[0])/sizeof(CTrashSpawner);
		m_vOpen[OBJ_TUNNEL].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_TUNNEL][i],OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_TUNNEL][i],RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_TUNNEL].clear();

	// DeathPits
	nSize = m_vDestroyed[OBJ_PIT].size();
	for(unsigned int i = 0; i < nSize; ++i)
	{
		nIndex = ((int)m_vDestroyed[OBJ_PIT][i]-(int)&m_cDeathPits[0])/sizeof(CDeathPit);
		m_vOpen[OBJ_PIT].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_PIT][i],OM_ENVIRONMENT);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_PIT][i],RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_PIT].clear();

	// Floor
	nSize = m_vDestroyed[OBJ_FLOOR].size();
	for( unsigned int i = 0; i < nSize; ++i )
	{
		nIndex = ((int)m_vDestroyed[OBJ_FLOOR][i]-(int)&m_cFloor[0])/sizeof(CFloorObject);
		m_vOpen[OBJ_FLOOR].push_back(nIndex);
		m_pOM->RemoveObject(m_vDestroyed[OBJ_FLOOR][i], OM_ENEMIES);
		m_pVM->RemoveObject(m_vDestroyed[OBJ_FLOOR][i], RG_ENVIRONMENT);
	}

	m_vDestroyed[OBJ_FLOOR].clear();
}

bool CObjectFactory::StartEvent(eEvents _eEventID)
{
	switch(_eEventID)
	{
	case EVENT_AICORE:
		{
			if( m_pOM->m_pCurrentEvent )
				return false;

			m_cAICoreEvent.m_bIsRunning = true;
			m_cAICoreEvent.Initialize();
			m_pOM->m_pCurrentEvent = &m_cAICoreEvent;
		}
		break;

	case EVENT_ASSBOSS:
		{
			if( m_pOM->m_pCurrentEvent )
				return false;

			m_cAssemblyEvent.m_bIsRunning = true;
			m_cAssemblyEvent.Initialize();
			m_pOM->m_pCurrentEvent = &m_cAssemblyEvent;
		}
		break;

	case EVENT_FOUNDBOSS:
		{
			if( m_pOM->m_pCurrentEvent )
				return false;

			m_cFoundryEvent.m_bIsRunning = true;
			m_cFoundryEvent.Initialize();
			m_pOM->m_pCurrentEvent = &m_cFoundryEvent;
		}
		break;
	}

	return true;
}

bool CObjectFactory::StopEvent(IBossEvent* _pEvent)
{
	if( !_pEvent || !_pEvent->m_bIsRunning )
		return false;

	_pEvent->m_bIsRunning = false;
	m_pOM->m_pCurrentEvent = nullptr;
	CGameplayState::GetInstance()->GetHUD()->DisableBoss();

	return true;
}

void CObjectFactory::MakeDefault( IBaseObject& _pObject )
{

	int numPlayers = CGameplayState::GetInstance()->GetPlayers().size();
	int currDifficulty = CGame::GetInstance()->GetDifficulty();

	switch( _pObject.GetID() )
	{

	case OBJ_PLAYER:
		{
			_pObject.SetIsActive(true);

			if(currDifficulty == NORMAL)
				_pObject.SetHealth(150.0f);
			else
				_pObject.SetHealth(100.0f);
			_pObject.SetAlive(true);
			_pObject.SetAnimationTime(0.0f);
			_pObject.ChangeAnimation(0);
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dTranslate;
			D3DXMatrixTranslation(&d3dTranslate, 5.0f, 0.0f, 9.0f);
			_pObject.SetMatrix(d3dTranslate);
			((CPlayerObject&)_pObject).SetColor(COLOR_PLAYER);
			((CPlayerObject&)_pObject).SetLives(2);
			((CPlayerObject&)_pObject).SetWeaponID(STANDARD);
			((CPlayerObject&)_pObject).SetIsFalling(false);
			((CPlayerObject&)_pObject).SetFallingTimer(2.0f);
		}
		break;

	case OBJ_BULLET_PLAYER:
		{
			_pObject.SetIsActive(true);

			((CBullet&)_pObject).SetDuration(0.0f);
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
			((CBullet&)_pObject).SetType(STANDARD);
			((CBullet&)_pObject).SetColor(COLOR_BULLET_PLAYER);
			((CBullet&)_pObject).SetRadius(BULLET_STD_SIZE);
		}
		break;
	case OBJ_BULLET_BEAM:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
			((CBeamBullet&)_pObject).SetDamage(DMG_BEAM);
			((CBeamBullet&)_pObject).SetLifeTime(0.5f);
			((CBeamBullet&)_pObject).SetType(BEAM);
			((CBeamBullet&)_pObject).SetRadius(BEAM_RADIUS);
			((CBeamBullet&)_pObject).SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;
	case OBJ_BULLET_ENEMY:
		{
			_pObject.SetIsActive(true);
			((CBullet&)_pObject).SetDuration(0.0f);
			((CBullet&)_pObject).SetRadius(BULLET_STD_SIZE);
			((CBullet&)_pObject).SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
		}
		break;

	case OBJ_BULLET_EXP:
		{
			_pObject.SetIsActive(true);
			Sphere TempSphere = ((CExplodingBullet&)_pObject).GetSphere();
			((CExplodingBullet&)_pObject).SetRadius(0.0f);
			((CExplodingBullet&)_pObject).SetDuration(0.0f);
			((CExplodingBullet&)_pObject).ResetTargets();
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
		}
		break;

	case OBJ_BULLET_TESLA:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
			((CBullet&)_pObject).SetDuration(0.0f);
		}
		break;

	case OBJ_BULLET_ARC:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(1);
			((CArcBullet&)_pObject).SetIsRising(true);
			((CArcBullet&)_pObject).SetFrame(3);
			((CArcBullet&)_pObject).SetIsMolten(false);
		}
		break;

	case OBJ_PICKUP:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(0);
			((CPickup&)_pObject).SetLifeTime(PICKUP_LIFETIME);
			((CPickup&)_pObject).SetPickupType(rand()%PU_NUM_PICKUPS);
			_pObject.SetBeingHit(((CPickup&)_pObject).GetGunType());
		}
		break;
	case OBJ_TRASH:
		{
			
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_TRASH);
			_pObject.SetHealth(scaledHP);
			D3DXMATRIX d3dID;
			D3DXMatrixIdentity(&d3dID);
			d3dID._41 = 3.0f;
			d3dID._42 = 0.0f;
			d3dID._43 = 3.0f;
			_pObject.SetMatrix(d3dID);
			_pObject.SetAnimationTime(0.0f);
			_pObject.ChangeAnimation(0);
			_pObject.SetBeingHit(0);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_TRASHBOT);
			((CEnemyObject&)_pObject).SetCloseTarget(nullptr);
			((CEnemyObject&)_pObject).SetCloseDist(0.0f);
			((CTrashBot&)_pObject).ResetDying();
			((CTrashBot&)_pObject).SetTargetFINAL(nullptr);
			//((CTrashBot&)_pObject).SetAvoidFlags(0);
			((CTrashBot&)_pObject).SetFading(0.0f);
			((CTrashBot&)_pObject).SetIsFalling(false);
			((CTrashBot&)_pObject).SetFallingTimer(2.0f);
		}
		break;
	case OBJ_PATROL:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_PATROL);
			_pObject.SetHealth(scaledHP);
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			D3DXMatrixIdentity(&d3dID);
			d3dID._41 = 13.0f;
			d3dID._42 = 14.0f;
			d3dID._43 = 10.0f;
			_pObject.SetMatrix(d3dID);
			_pObject.SetAnimationTime(0.0f);
			_pObject.ChangeAnimation(0);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_PATROLBOT);
			((CEnemyObject&)_pObject).SetVelocity( D3DXVECTOR3(-1.0f, 0.0f, -1.0f));
			((CPatrolBot&)_pObject).ResetDying();
			((CPatrolBot&)_pObject).SetFireTime(0.0f);
			((CPatrolBot&)_pObject).SetIsSpawning(true);
		}
		break;
	case OBJ_SMASH:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_SMASH);
			if(currDifficulty == NORMAL && numPlayers < 1)
				_pObject.SetHealth(scaledHP);
			else
				_pObject.SetHealth(scaledHP + 1000.0f);
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			D3DXMatrixIdentity(&d3dID);
			d3dID._41 = 3.0f;
			d3dID._43 = 3.0f;
			_pObject.SetMatrix(d3dID);
			_pObject.SetAnimationTime(0.0f);
			_pObject.ChangeAnimation(0);
			((CEnemyObject&)_pObject).SetColor(COLOR_SMASHBOT);
			((CSmashBot&)_pObject).SetSmash(false);
			((CSmashBot&)_pObject).SetSmashTime(0.0f);
			((CSmashBot&)_pObject).SetBullet(nullptr);
			((CSmashBot&)_pObject).SetWalking(SMASH_WALK);
			((CSmashBot&)_pObject).ResetDying();
			((CSmashBot&)_pObject).SetTargetFINAL(nullptr);
			((CEnemyObject&)_pObject).SetCloseTarget(nullptr);
			((CEnemyObject&)_pObject).SetCloseDist(0.0f);

		}
		break;
	case OBJ_BURN:
		{
			_pObject.SetIsActive(true);
			_pObject.SetID(OBJ_BURN);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_BURN);
			_pObject.SetHealth(scaledHP); 
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			D3DXMatrixIdentity(&d3dID);
			d3dID._41 = 3.0f;
			d3dID._43 = 3.0f;
			_pObject.SetMatrix(d3dID);
			_pObject.SetAnimationTime(0.0f);
			_pObject.ChangeAnimation(0);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_PATROLBOT);
			((CBurnBot&)_pObject).ResetDying();
			((CBurnBot&)_pObject).SetCloseToTarget(false);
			((CBurnBot&)_pObject).SetExplodeTimer(0.0f);
			((CEnemyObject&)_pObject).SetCloseTarget(nullptr);
			((CEnemyObject&)_pObject).SetCloseDist(0.0f);
		}
		break;

	case OBJ_SPIDER:
		{
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_SPIDER);
			if(currDifficulty == NORMAL && numPlayers <= 2)
				_pObject.SetHealth(scaledHP);
			else
				_pObject.SetHealth(scaledHP + 500.0f);
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			D3DXMatrixIdentity(&d3dID);
			_pObject.SetMatrix(d3dID);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_AICORE);
			((CSpiderBot&)_pObject).SetIsWalking(true);
			((CSpiderBot&)_pObject).SetTargetFINAL(nullptr);
		}
		break;

	case OBJ_STEAM:
		{
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			//float fMoreHealth = CGameplayState::GetInstance()->GetPlayers().size() * 2000.0f;
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_STEAM);
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(scaledHP); 
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			_pObject.SetMatrix(d3dID);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_AICORE);
			((CSteamrollerBot&)_pObject).SetBehavior(Steam::Idle);
			((CSteamrollerBot&)_pObject).SetMaxHP(scaledHP);
			((CSteamrollerBot&)_pObject).SetIsEnraged(false);
		}
		break;

	case OBJ_SPINBOT:
		{
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			//float fMoreHealth = CGameplayState::GetInstance()->GetPlayers().size() * 2000.0f;
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_SPINBOT);
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(scaledHP);
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			_pObject.SetMatrix(d3dID);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_AICORE);
			((CSpinBot&)_pObject).SetBehavior(SpinBot::Enter);
			((CSpinBot&)_pObject).SetMaxHP(scaledHP);
			((CSpinBot&)_pObject).SetHoldTime(0.0f);
			((CSpinBot&)_pObject).SetIsHolding(false);
		}
		break;

	case OBJ_AICORE:
		{
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			float fMoreHealth = CGameplayState::GetInstance()->GetPlayers().size() * 2000.0f;
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(MAXHP_AICORE + fMoreHealth); 
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID;
			_pObject.SetMatrix(d3dID);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			((CEnemyObject&)_pObject).SetColor(COLOR_AICORE);
		}
		break;
	case OBJ_AIFIST:
		{
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			float fMoreHealth = CGameplayState::GetInstance()->GetPlayers().size() * 1000.0f;
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(MAXHP_AIFIST + fMoreHealth); 
			_pObject.SetBeingHit(0);
			D3DXMATRIX d3dID, d3dRot;
			D3DXMatrixIdentity(&d3dID);
			D3DXMatrixRotationY(&d3dRot, D3DXToRadian(180.0f));
			d3dID *= d3dRot;
			_pObject.SetMatrix(d3dID);
			((CEnemyObject&)_pObject).SetHitDuration(0.0f);
			((CEnemyObject&)_pObject).SetWasHit(false);
			Sphere tSphere = ((CEnemyObject&)_pObject).GetSphere();
			((CEnemyObject&)_pObject).SetColor(COLOR_AICORE);
		}
		break;

	case OBJ_VAT:
		{
			_pObject.SetAlive(true);
			_pObject.SetIsActive(true);
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			((CVat&)_pObject).SetVatTimer(120.0f);
			_pObject.SetHealth(MAXHP_VAT);
			_pObject.SetColor((D3DXVECTOR4(0.50f, 0.40f, 0.10f, 0.85f)));
		}
		break;
	case OBJ_BARREL:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(25.0f);
			_pObject.SetRadius(0.5f);
			_pObject.SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.ChangeAnimation(0);
			_pObject.SetAnimationTime(0.0f);
			((CBarrel&)_pObject).SetBarrelTimer(60.0f);
			((CBarrel&)_pObject).SetIsFalling(false);
			((CBarrel&)_pObject).SetFallingTimer(2.0f);
			((CBarrel&)_pObject).SetReset();
		}
		break;
	case OBJ_CRATE:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetHealth(25.0f);
			_pObject.SetRadius(0.5f);
			_pObject.SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CCrate&)_pObject).SetDelay(60.0f);
			((CCrate&)_pObject).SetIsFalling(false);
			((CCrate&)_pObject).SetFallingTimer(2.0f);
			((CCrate&)_pObject).SetReset();
		}
		break;
	case OBJ_CONVEYOR:
		{
			_pObject.SetIsActive(true);
			((CConveyor&)_pObject).ResetHalt();
		}
		break;
	case OBJ_POOL:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CMoltenPool&)_pObject).SetMoltenTimer(60.0f);
			((CMoltenPool&)_pObject).ForceAnimation(1.0f);
		}
		break;
	case OBJ_TURRET:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,TURRET_HP);
			_pObject.SetHealth(scaledHP);
			_pObject.SetRadius(0.75f);
			_pObject.SetColor(COLOR_TRASHBOT);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CTurret&)_pObject).SetWasHit(false);
			((CTurret&)_pObject).SetHitDuration(0.0f);
			((CTurret&)_pObject).ResetEffect();
			((CTurret&)_pObject).SetDelay(2.0f);
			((CTurret&)_pObject).SetKillTimer(60.0f);
		}
		break;

	case OBJ_BOSSTURRET:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,TURRET_HP);
			_pObject.SetHealth(scaledHP);
			_pObject.SetRadius(0.75f);
			_pObject.SetColor(COLOR_TRASHBOT);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CTurret&)_pObject).SetWasHit(false);
			((CTurret&)_pObject).SetHitDuration(0.0f);
			//((CTurret&)_pObject).ResetEffect();
			((CTurret&)_pObject).SetDelay(2.0f);
			((CTurret&)_pObject).SetKillTimer(60.0f);
		}
		break;
	case OBJ_INVERSION:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_INVERSION);
			_pObject.SetHealth(scaledHP);
			_pObject.SetRadius(INVERSION_RADIUS);
			_pObject.SetColor(COLOR_PATROLBOT_HIT);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CInversionTower&)_pObject).SetInversionTimer(60.0f);
			((CInversionTower&)_pObject).SetInversionAtkTimer(2.4f);
			((CInversionTower&)_pObject).SetSpawnTimer(2.75f);
			((CInversionTower&)_pObject).SetMaxRadius(6.0f);
			((CInversionTower&)_pObject).SetAttackTime(3.0f);
			((CInversionTower&)_pObject).SetAttackSpeed(15.0f);
		}
		break;

	case OBJ_BOSSINV:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_INVERSION);
			_pObject.SetHealth(scaledHP);
			_pObject.SetRadius(INVERSION_RADIUS);
			_pObject.SetColor(COLOR_PATROLBOT_HIT);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CInversionTower&)_pObject).SetInversionTimer(60.0f);
			((CInversionTower&)_pObject).SetInversionAtkTimer(2.4f);
			((CInversionTower&)_pObject).SetSpawnTimer(2.75f);
			((CInversionTower&)_pObject).SetMaxRadius(5.0f);
			((CInversionTower&)_pObject).SetAttackTime(3.0f);
			((CInversionTower&)_pObject).SetAttackSpeed(15.0f);
			((CBossInversionTower&)_pObject).SetShieldActive(true);
		}
		break;

	case OBJ_TUNNEL:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			float scaledHP = ScaleHealth(currDifficulty,numPlayers,MAXHP_INVERSION);
			_pObject.SetHealth(scaledHP);
			_pObject.SetRadius(2.0f);
			_pObject.SetColor(COLOR_PATROLBOT_HIT);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(0);
			((CTrashSpawner&)_pObject).SetMoveTime(1.0f,0.5f);
			((CTrashSpawner&)_pObject).SetSpawnTime(5.0f);
			_pObject.KillCurrFrame();
		}
		break;
	case OBJ_PIT:
		{
			_pObject.SetIsActive(true);
			_pObject.SetAlive(true);
			_pObject.SetAnimationTime(0);
			_pObject.ChangeAnimation(1);
			_pObject.KillCurrFrame();
			_pObject.SetRadius(1.0f);
			((CDeathPit&)_pObject).SetDelayTimer(120.0f);
			((CDeathPit&)_pObject).SetTrapCloseTimer((float)(rand()%7));
			((CDeathPit&)_pObject).SetIsTrapOpen(false);
			((CDeathPit&)_pObject).ResetAnim();
		}
		break;

	case OBJ_FLOOR:
		{
			_pObject.SetIsActive(false);
			_pObject.SetAlive(true);
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);
			_pObject.SetMatrix(temp);
			_pObject.SetRadius(50.0f);
		}
		break;
	}
}

bool CObjectFactory::LoadObjectData(std::string _szFilename)
{
	return false;
}

bool CObjectFactory::CheckValid( int _nID, int _nIndex )
{
	unsigned int nSize = m_vOpen[_nID].size();

	for( unsigned int i = 0; i < nSize; ++i )
		if( m_vOpen[_nID][i] == _nIndex )
			return false;

	return true;
}
float CObjectFactory::ScaleHealth(int _difficulty, int _numPlayers, float _numHP)
{
	//int tempPlayers = CGameplayState::GetInstance()->GetPlayers().size();
	//int tempDiff = CGame::GetInstance()->GetDifficulty();
	float newHP = _numHP;

	switch(_difficulty)
	{
	case NORMAL:
		{
			switch(_numPlayers) // how many players are playing this difficulty
			{
			case 1:
				{
					newHP = _numHP * 0.75f;
				}
				break;
			case 2:
				{
					newHP = _numHP * 1.0f;
				}
				break;
			case 3:
				{
					newHP = _numHP * 1.25f;
				}
				break;
			case 4:
				{
					newHP = _numHP * 1.5f;
				}
				break;
			};
		}
		break;
	
	case HARD:
		{
			switch(_numPlayers)
			{
			case 1:
				{
					newHP = _numHP * 1.5f;
				}
				break;
			case 2:
				{
					newHP = _numHP * 1.75f;
				}
				break;
			case 3:
				{
					newHP = _numHP * 2.0f;
				}
				break;
			case 4:
				{
					newHP = _numHP * 2.5f;
				}
				break;
			};
		}
		break;
	};

	return newHP;
}
