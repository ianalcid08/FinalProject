#ifndef _ASSEMBLYBOSSEVENT_H_
#define _ASSEMBLYBOSSEVENT_H_

#include "ibossevent.h"
#include "SpinFight.h"
#include "SteamrollerFight.h"
#include "FloorTransition.h"

class CFloorObject;

class CAssemblyBossEvent : public IBossEvent
{
	CSpinFight			m_cSpinFight;
	CFloorTransition	m_cFloorTrans;

	CFloorObject*		m_pFloor;

public:
	CAssemblyBossEvent(void);
	virtual ~CAssemblyBossEvent(void);

	void Initialize();
	void Update(float _fTime);
};

#endif