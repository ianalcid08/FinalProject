#ifndef _SPINFIGHT_H_
#define _SPINFIGHT_H_

#include "iaistate.h"

class CSpinBot;
class CObjectFactory;

class CSpinFight : public IAIState
{
	CSpinBot* m_pSpinBot;
	CObjectFactory* m_pOF;
	float m_fPickupTime;
	bool m_bPhase1;

public:
	CSpinFight(void);
	virtual ~CSpinFight(void);

	// Access
	CSpinBot* GetSpinBot() const { return m_pSpinBot; }
	bool GetIsP1() const { return m_bPhase1; }

	// Mutate
	void SetSpinBot(CSpinBot* _pSB) { m_pSpinBot = _pSB; }
	void SetIsP1(bool _bP1) { m_bPhase1 = _bP1; }

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif