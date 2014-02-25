#ifndef _SPINBOT_H_
#define _SPINBOT_H_

#include "enemyobject.h"
class CSpinBot : public CEnemyObject
{
	Behavior m_pBehavior;
	float m_fTime;
	float m_fRotSpeed;
	float m_fMaxHP;
	float m_fDamage;
	float m_fHoldTime;

	bool m_bIsHolding;

public:
	CSpinBot(void);
	virtual ~CSpinBot(void);

	// Access
	float GetTime() const { return m_fTime; }
	float GetRotSpeed() const { return m_fRotSpeed; }
	Behavior GetBehavior() const { return m_pBehavior; }
	float GetMaxHP() const { return m_fMaxHP; }
	float GetDamage() const { return m_fDamage; }
	float GetHoldTime() const { return m_fHoldTime; }
	bool GetIsHolding() const { return m_bIsHolding; }

	// Mutate
	void SetTime(float _fT) { m_fTime = _fT; }
	void SetRotSpeed(float _fS) { m_fRotSpeed = _fS; }
	void SetBehavior(Behavior _pB) { m_pBehavior = _pB; }
	void SetMaxHP(float _fHP) { m_fMaxHP = _fHP; }
	void SetDamage(float _fD) { m_fDamage = _fD; }
	void SetHoldTime(float _fT) { m_fHoldTime = _fT; }
	void SetIsHolding(bool _bH) { m_bIsHolding = _bH; }

	void Update(float fDelta);
	bool CheckCollision(IBaseObject* obj);
	void CollisionResponse(IBaseObject* obj);
	virtual void TakeDamage(IBaseObject* _pIn);
};

#endif