#ifndef _AICOREEVENT_H_
#define _AICOREEVENT_H_

#include "ibossevent.h"
#include "AITowerPhase.h"
#include "AICoreFight.h"
#include "AILaserPhase.h"

class CBossInversionTower;

class CAICoreEvent : public IBossEvent
{
	CAILaserPhase	m_cLaserPhase;
	CAITowerPhase	m_cTurretPhase;
	CAICoreFight	m_cCoreFight;

	CBossInversionTower* m_pTower;

public:

	CAICoreEvent(void);
	virtual ~CAICoreEvent(void);

	// Access
	CBossInversionTower* GetTower() const { return m_pTower; }

	// Mutate
	void SetTower(CBossInversionTower* _pT) { m_pTower = _pT; }

	void Initialize();
	void Update(float _fTime);
};

#endif