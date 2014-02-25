#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_
#include "Definitions.h"

/***********************************************
 * Filename:  		ObjectManager.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/12/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all object updating and collisions
 ************************************************/

#include "IBossEvent.h"
class IBaseObject;

class CObjectManager
{
	friend class CObjectFactory;


	int m_nNumPickups;

	std::list<IBaseObject*> m_lObjects[OM_NUM_OBJECTS];

	IBossEvent* m_pCurrentEvent;

	/*****************************************************************
	* void AddObject(IBaseObject* _pBase): adds an object to the OM
	*
	* Ins:			    IBaseObject* _pBase
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/12/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void AddObject(IBaseObject* _pBase, eObjectGroups _eType);

	/*****************************************************************
	* void RemoveObject(IBaseObject* _pBase): removes an active object from the OM
	*
	* Ins:			    IBaseObject* _pBase
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/12/2013
	* Mod. Initials:	JT
	*****************************************************************/
	bool RemoveObject(IBaseObject* _pBase, eObjectGroups _eType);

public:

	CObjectManager(void);
	~CObjectManager(void);

	int GetNumPickups() const { return m_nNumPickups; }
	int GetSize(eObjectGroups _eType) const { return m_lObjects[_eType].size(); }
	/*****************************************************************
	* void Update(float _fTime): updates all active game objects
	*
	* Ins:			    float _fTime
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/12/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void Update(float _fTime);

	/*****************************************************************
	* void CheckCollisions(): checks all collisions in the OM
	*
	* Ins:			    IBaseObject* _pBase
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void CheckCollisions();

	/*****************************************************************
	* int GetEnemyCount(): returns the number of active enemies
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    unsigned int
	*
	* Mod. Date:		02/12/2013
	* Mod. Initials:	JT
	*****************************************************************/
	unsigned int GetEnemyCount() const {return m_lObjects[OM_ENEMIES].size() + m_pCurrentEvent?1:0;}

	void Clear(bool _bClearPlayers);

	void CollisionResponses();
};

#endif