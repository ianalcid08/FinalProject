#ifndef _FLOORTRANSITION_H_
#define _FLOORTRANSITION_H_

#include "iaistate.h"
class CFloorTransition : public IAIState
{
	float m_fTime;

public:
	CFloorTransition(void);
	virtual ~CFloorTransition(void);

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif