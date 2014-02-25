#ifndef _BOSSTURRET_H_
#define _BOSSTURRET_H_

#include "turret.h"
class CBossTurret : public CTurret
{
	D3DXVECTOR3 m_d3dNewImpact;
	float m_fHold;
	float m_fHoldTimer;
	float m_fLength;

public:
	CBossTurret(void);
	virtual ~CBossTurret(void);

	void SetHoldAmt(float _fT) { m_fHold = _fT; }

	virtual void Update(float fDelta);
	virtual bool CheckCollision(IBaseObject* pObj);
	virtual void Render();
	virtual void TakeDamage(IBaseObject* _pIn);
};

#endif