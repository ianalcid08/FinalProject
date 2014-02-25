#ifndef _STEAMROLLERFIGHT_H_
#define _STEAMROLLERFIGHT_H_

#include "iaistate.h"
class CSteamrollerBot;
class CObjectFactory;

class CSteamrollerFight : public IAIState
{
	CSteamrollerBot* m_pSteam;
	CObjectFactory*	m_pOF;
	bool m_bImpact;
	float m_fImpactTime;
	int m_nImpactCount;

	float m_fPickupTime;

public:
	CSteamrollerFight(void);
	virtual ~CSteamrollerFight(void);

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif