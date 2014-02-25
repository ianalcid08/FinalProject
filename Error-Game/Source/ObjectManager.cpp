#include "ObjectManager.h"
#include "IBaseObject.h"
#include "Game.h"

/***********************************************
 * Filename:  		ObjectManager.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/12/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all object updating and collisions
 ************************************************/
std::string GetName(int _eOBJ);

CObjectManager::CObjectManager(void)
{
	m_pCurrentEvent = nullptr;
	m_nNumPickups = 0;
}

CObjectManager::~CObjectManager(void)
{

}

void CObjectManager::Update(float _fTime)
{
	std::list<IBaseObject*>::iterator iter;

	// Update Players
	for( iter = m_lObjects[OM_PLAYERS].begin(); iter != m_lObjects[OM_PLAYERS].end(); ++iter )
		(*iter)->Update(_fTime);

	// Update Enemies
	for( iter = m_lObjects[OM_ENEMIES].begin(); iter != m_lObjects[OM_ENEMIES].end(); ++iter )
	{
		(*iter)->Update(_fTime);
		if( !CheckMatrix((*iter)->GetMatrix()) )
		{
			CGame::GetInstance()->GetOF()->Destroy((*iter));
#if _DEBUG
			std::cout << "DESTROYED BAD " << GetName((*iter)->GetID()) << " !" << endl;
#endif
		}
	}

	// Update Player Bullets
	for( iter = m_lObjects[OM_PLAYER_BULLETS].begin(); iter != m_lObjects[OM_PLAYER_BULLETS].end(); ++iter )
	{
		(*iter)->Update(_fTime);
		if( (*iter)->GetID() == OBJ_BULLET_EXP || (*iter)->GetID() == OBJ_BULLET_TESLA )
			m_lObjects[OM_PLAYER_SP].push_back((*iter));
	}

	// Update Enemy Bullets
	for( iter = m_lObjects[OM_ENEMY_BULLETS].begin(); iter != m_lObjects[OM_ENEMY_BULLETS].end(); ++iter )
	{
		if( (*iter)->GetID() != OBJ_BULLET_EXP )
			(*iter)->Update(_fTime);
	}

	// Update Environment
	for( iter = m_lObjects[OM_ENVIRONMENT].begin(); iter != m_lObjects[OM_ENVIRONMENT].end(); ++iter )
		(*iter)->Update(_fTime);

	if( m_pCurrentEvent )
		m_pCurrentEvent->Update(_fTime);
}

void CObjectManager::AddObject(IBaseObject* _pBase, eObjectGroups _eType)
{
	if( _pBase->GetID() == OBJ_PICKUP )
		++m_nNumPickups;

	m_lObjects[_eType].push_back(_pBase);
}

bool CObjectManager::RemoveObject(IBaseObject* _pBase, eObjectGroups _eType)
{
	if( _pBase->GetID() == OBJ_PICKUP )
		--m_nNumPickups;

	m_lObjects[_eType].remove(_pBase);

	return true;
}

void CObjectManager::CheckCollisions()
{
	std::list<IBaseObject*>::iterator PlayerIter = m_lObjects[OM_PLAYERS].begin();
	std::list<IBaseObject*>::iterator PlayerIter2 = m_lObjects[OM_PLAYERS].begin();
	std::list<IBaseObject*>::iterator iterSP = m_lObjects[OM_PLAYER_SP].begin();
	std::list<IBaseObject*>::iterator EnemyIter = m_lObjects[OM_ENEMIES].begin();
	std::list<IBaseObject*>::iterator EnemyIter2 = m_lObjects[OM_ENEMIES].begin();
	std::list<IBaseObject*>::iterator EnvirnIter = m_lObjects[OM_ENVIRONMENT].begin();
	std::list<IBaseObject*>::iterator EnvirnIter2 = m_lObjects[OM_ENVIRONMENT].begin();
	std::list<IBaseObject*>::iterator PlayerBulletIter = m_lObjects[OM_PLAYER_BULLETS].begin();
	std::list<IBaseObject*>::iterator EnemyBulletIter = m_lObjects[OM_ENEMY_BULLETS].begin();


	while(PlayerIter != m_lObjects[OM_PLAYERS].end())
	{
		while(EnvirnIter != m_lObjects[OM_ENVIRONMENT].end())
		{
			if( (*EnvirnIter)->GetID() == OBJ_POOL || (*EnvirnIter)->GetID() == OBJ_TURRET )
				(*EnvirnIter)->CheckCollision((*PlayerIter));
			else
				(*PlayerIter)->CheckCollision((*EnvirnIter));

			EnvirnIter++;
		}

		while(PlayerIter2 != m_lObjects[OM_PLAYERS].end())
		{
			(*PlayerIter)->CheckCollision( (*PlayerIter2) );
			PlayerIter2++;
		}

		EnvirnIter = m_lObjects[OM_ENVIRONMENT].begin();
		PlayerIter2 = m_lObjects[OM_PLAYERS].begin();
		PlayerIter++;
	}

	while(EnemyIter != m_lObjects[OM_ENEMIES].end())
	{
		EnvirnIter = m_lObjects[OM_ENVIRONMENT].begin();
		EnemyIter2 = m_lObjects[OM_ENEMIES].begin();
		PlayerIter = m_lObjects[OM_PLAYERS].begin();

		while(EnvirnIter != m_lObjects[OM_ENVIRONMENT].end())
		{
			if( (*EnvirnIter)->GetID() == OBJ_POOL || (*EnvirnIter)->GetID() == OBJ_TURRET )
				(*EnvirnIter)->CheckCollision((*EnemyIter));
			else
				(*EnemyIter)->CheckCollision((*EnvirnIter));

			EnvirnIter++;
		}

		while(EnemyIter2 != m_lObjects[OM_ENEMIES].end())
		{
			(*EnemyIter)->CheckCollision( (*EnemyIter2) );
			EnemyIter2++;
		}

		while(PlayerIter != m_lObjects[OM_PLAYERS].end())
		{
			(*EnemyIter)->CheckCollision( (*PlayerIter) );
			PlayerIter++;
		}
	
		PlayerIter = m_lObjects[OM_PLAYERS].begin();
		EnvirnIter = m_lObjects[OM_ENVIRONMENT].begin();
		EnemyIter2 = m_lObjects[OM_ENEMIES].begin();
		EnemyIter++;
	}

	while(EnvirnIter != m_lObjects[OM_ENVIRONMENT].end())
	{
		EnvirnIter2 = m_lObjects[OM_ENVIRONMENT].begin();
		while(EnvirnIter2 != m_lObjects[OM_ENVIRONMENT].end())
		{
			if( (*EnvirnIter)->GetID() != OBJ_CONVEYOR)
				(*EnvirnIter)->CheckCollision( (*EnvirnIter2) );

			EnvirnIter2++;
		}
		EnvirnIter2 = m_lObjects[OM_ENVIRONMENT].begin();
		EnvirnIter++;
	}

	while(EnemyBulletIter != m_lObjects[OM_ENEMY_BULLETS].end())
	{
		PlayerIter = m_lObjects[OM_PLAYERS].begin();

		while(PlayerIter != m_lObjects[OM_PLAYERS].end())
		{
			(*EnemyBulletIter)->CheckCollision( (*PlayerIter) );
			PlayerIter++;
		}

		PlayerIter = m_lObjects[OM_PLAYERS].begin();
		EnemyBulletIter++;
	}

	while(PlayerBulletIter != m_lObjects[OM_PLAYER_BULLETS].end())
	{
		EnvirnIter = m_lObjects[OM_ENVIRONMENT].begin();
		EnemyIter = m_lObjects[OM_ENEMIES].begin();

		while(EnvirnIter != m_lObjects[OM_ENVIRONMENT].end())
		{
			(*PlayerBulletIter)->CheckCollision( (*EnvirnIter) );
			EnvirnIter++;
		}

		while(EnemyIter != m_lObjects[OM_ENEMIES].end())
		{
			(*PlayerBulletIter)->CheckCollision( (*EnemyIter) );
			EnemyIter++;
		}

		PlayerBulletIter++;
	}

	while( iterSP != m_lObjects[OM_PLAYER_SP].end() )
	{
		EnemyBulletIter = m_lObjects[OM_ENEMY_BULLETS].begin();

		while( EnemyBulletIter != m_lObjects[OM_ENEMY_BULLETS].end() )
		{
			(*iterSP)->CheckCollision((*EnemyBulletIter));
			++EnemyBulletIter;
		}

		++iterSP;
	}

	m_lObjects[OM_PLAYER_SP].clear();
}

void CObjectManager::Clear(bool _bClearPlayers)
{
	std::list<IBaseObject*>::iterator iter;

	CObjectFactory* pOF = CGame::GetInstance()->GetOF();

	for( unsigned int i = 0; i < OM_NUM_OBJECTS; ++i )
	{
		if( i == OM_PLAYERS && !_bClearPlayers )
			continue;

		for( iter = m_lObjects[i].begin(); iter != m_lObjects[i].end(); ++iter )
		{
			if((*iter)->GetID() == OBJ_CONVEYOR && !_bClearPlayers)
				continue;

			pOF->Destroy((*iter));
		}
	}

	if( m_pCurrentEvent )
		pOF->StopEvent(m_pCurrentEvent);
}

std::string GetName(int _eOBJ)
{
	std::string szOut = "BAD INDEX";

	switch(_eOBJ)
	{
	case OBJ_TRASH:
		{
			szOut = "TRASH BOT";
		}
		break;

	case OBJ_PATROL:
		{
			szOut = "PATROL BOT";
		}
		break;

	case OBJ_SMASH:
		{
			szOut = "SMASH BOT";
		}
		break;

	case OBJ_BURN:
		{
			szOut = "BURN BOT";
		}
		break;

	default:
		break;
	}

	return szOut;
}

void CObjectManager::CollisionResponses()
{
	std::list<IBaseObject*>::iterator iter;

	for( unsigned int i = 0; i < OM_NUM_OBJECTS; ++i )
	{
		for( iter = m_lObjects[i].begin(); iter != m_lObjects[i].end(); ++iter )
		{
			for(unsigned int i = 0; i < (*iter)->GetReactVector().size(); i++)
			{
				(*iter)->CollisionResponse( (*iter)->GetReactVector()[i] );
			}

			(*iter)->ClearReactVector();
		}
	}
}
