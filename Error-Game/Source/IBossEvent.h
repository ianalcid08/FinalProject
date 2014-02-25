#ifndef _BOSSEVENT_H_
#define _BOSSEVENT_H_

#include "IAIState.h"

class IBossEvent
{
	friend class CObjectFactory;
	bool m_bIsRunning;

protected:
	IAIState* m_pCurrState;

public:

	IBossEvent() : m_bIsRunning(false), m_pCurrState(nullptr) {};
	virtual ~IBossEvent(void) = 0 {};
	
	virtual void Initialize() = 0;
	virtual void Update(float _fTime) = 0;

	void ChangeState(IAIState* _pNewState)
	{
		if( !_pNewState )
			return;

		if( m_pCurrState )
			m_pCurrState->Shutdown();

		m_pCurrState = _pNewState;

		m_pCurrState->Initialize();
	}
};

#endif