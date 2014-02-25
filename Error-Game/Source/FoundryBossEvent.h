#ifndef _FOUNDRYBOSSEVENT_H_
#define _FOUNDRYBOSSEVENT_H_

#include "ibossevent.h"
#include "SteamrollerFight.h"

class CFoundryBossEvent : public IBossEvent
{
	CSteamrollerFight	m_cSteamrollerFight;

public:
	CFoundryBossEvent(void);
	virtual ~CFoundryBossEvent(void);

	void Initialize();
	void Update(float _fTime);
};

#endif