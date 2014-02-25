#ifndef _BOSSINVERSIONTOWER_H_
#define _BOSSINVERSIONTOWER_H_

#include "inversiontower.h"
class CBossInversionTower : public CInversionTower
{
	bool m_bShieldActive;
	bool m_bShieldHit;
	float m_fHitDuration;

public:
	CBossInversionTower(void);
	virtual ~CBossInversionTower(void);

	// Access
	bool GetShieldActive() const { return m_bShieldActive; }

	// Mutate
	void SetShieldActive(bool _bA) { m_bShieldActive = _bA; }
	void SetShieldHit(bool _bH) { m_bShieldHit = _bH; }

	virtual void Update(float fDelta);
	virtual void TakeDamage(IBaseObject* _pIn);
	virtual void Render();
};

#endif