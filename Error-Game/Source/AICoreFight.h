#ifndef _AICOREFIGHT_H_
#define _AICOREFIGHT_H_

#include "iaistate.h"

class CAICore;

class CAICoreFight : public IAIState
{
	CAICore* m_pCore;
	float m_fPickupTime;

public:
	CAICoreFight(void);
	virtual ~CAICoreFight(void);

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif