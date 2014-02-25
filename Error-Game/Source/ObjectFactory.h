#ifndef _OBJECTFACTORY_H_
#define _OBJECTFACTORY_H_
#include "Definitions.h"

/***********************************************
 * Filename:  		ObjectFactory.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/13/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		Handles creating and destroying all objects
 ************************************************/

#pragma region Includes

#include "PlayerObject.h"
#include "Bullet.h"
#include "Pickup.h"
#include "TrashBot.h"
#include "PatrolBot.h"
#include "SmashBot.h"
#include "BurnBot.h"
#include "SteamrollerBot.h"
#include "AICore.h"
#include "AIFist.h"
#include "Conveyor.h"
#include "Vat.h"
#include "Crate.h"
#include "Barrel.h"
#include "MoltenPool.h"
#include "ExplodingBullet.h"
#include "TeslaBullet.h"
#include "BeamBullet.h"
#include "ArcBullet.h"
#include "Turret.h"
#include "InversionTower.h"
#include "SpiderBot.h"
#include "TrashSpawner.h"
#include "AICoreEvent.h"
#include "BossInversionTower.h"
#include "DeathPit.h"
#include "BossTurret.h"
#include "SpinBot.h"
#include "AssemblyBossEvent.h"
#include "FloorObject.h"
#include "FoundryBossEvent.h"

#pragma endregion

class CObjectManager;
class CViewManager;

///////////////////////////

class CObjectFactory
{
	CObjectManager* m_pOM;
	CViewManager*	m_pVM;

	std::vector<unsigned int> m_vOpen[OBJ_NUM_OBJECTS];
	std::vector<IBaseObject*> m_vDestroyed[OBJ_NUM_OBJECTS];

	CPlayerObject		m_cPlayers[MAX_PLAYERS];
	CBullet				m_cPlayerBullets[MAX_BULLETS_PLAYER];
	CBullet				m_cEnemyBullets[MAX_BULLETS_ENEMY];
	CExplodingBullet	m_cExplodingBullets[MAX_BULLETS_EXP];
	CTeslaBullet		m_cTeslaBullets[MAX_BULLETS_TESLA];
	CBeamBullet			m_cBeamBullets[MAX_BULLETS_BEAM];
	CArcBullet			m_cArcBullets[MAX_BULLETS_ARC];
	CPickup				m_cPickups[MAX_PICKUP];
	CTrashBot			m_cTrashBots[MAX_TRASH];
	CPatrolBot			m_cPatrolBots[MAX_PATROL];
	CSmashBot			m_cSmashBots[MAX_SMASH];
	CBurnBot			m_cBurnBots[MAX_BURN];
	CSpiderBot			m_cSpiderBots[MAX_SPIDER];
	CSteamrollerBot		m_cSteamrollerBots[MAX_STEAM];
	CAICore				m_cAICores[MAX_AICORE];
	CAIFist				m_cAIFists[MAX_AIFIST];
	CConveyor			m_cConveyors[MAX_CONVEYOR];
	CVat				m_cVats[MAX_VAT];
	CCrate				m_cCrates[MAX_CRATE];
	CBarrel				m_cBarrels[MAX_BARREL];
	CMoltenPool			m_cPools[MAX_POOL];
	CTurret				m_cTurrets[MAX_TURRETS];
	CInversionTower		m_cInversion[MAX_INVERSION];
	CTrashSpawner		m_cTunnel[MAX_TUNNEL];
	CBossInversionTower m_cBossInv[MAX_BOSSINV];
	CDeathPit			m_cDeathPits[MAX_PITS];
	CBossTurret			m_cBossTurrets[MAX_BOSSTURRET];
	CSpinBot			m_cSpinBot[MAX_SPINBOT];
	CFloorObject		m_cFloor[MAX_FLOOR];

	CAICoreEvent		m_cAICoreEvent;
	CAssemblyBossEvent	m_cAssemblyEvent;
	CFoundryBossEvent	m_cFoundryEvent;

	/*****************************************************************
	* void MakeDefault(): assigns the default values to the given object
	*
	* Ins:			    IBaseObject& _pObject
	*
	* Outs:				IBaseObject& _pObject
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void MakeDefault( IBaseObject& _pObject );
	bool CheckValid( int _nID, int _nIndex );

	/*****************************************************************
	* void ScaleHealth(): assigns the default values to the given object
	*
	* Ins:			    int _difficulty, int _numPlayers, float _numHP
	*
	* Outs:				
	*
	* Returns:		    float - new amount of HP after scaled
	*
	* Mod. Date:		04/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	float ScaleHealth(int _difficulty, int _numPlayers, float _numHP);
	
public:
	CObjectFactory(void);
	~CObjectFactory(void);

	/*****************************************************************
	* void Initialize(): allocates all memory needed for the factory
	*
	* Ins:			    CObjectManager* _pOM, CViewManager* _pVM
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void Initialize(CObjectManager* _pOM, CViewManager* _pVM);

	/*****************************************************************
	* IBaseObject* Create(int _nObjectID): creates a base object of 
	* the given type
	*
	* Ins:			    int _nObjectID
	*
	* Outs:				IBaseObject* _pOut
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	bool Create(eObjects _eObjectID, IBaseObject** _pOut);

	/*****************************************************************
	* void Destroy(): marks the given object as inactive
	*
	* Ins:			    IBaseObject* _pObject
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	bool Destroy(IBaseObject* _pObject);
	
	
	/*****************************************************************
	* void ProcessDestroy(): Syncs all destroyed objects with the view manager
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void ProcessDestroy();
	
	// Start & Stop Boss Fights
	bool StartEvent(eEvents _eEventID);
	bool StopEvent(IBossEvent* _pEvent);

	/*****************************************************************
	* bool LoadObjectData(std::string _szFilename): loads all default values for each game object
	*
	* Ins:			    std::string _szFilename
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	bool LoadObjectData(std::string _szFilename);

};

#endif
