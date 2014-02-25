#include "SpinBot.h"
#include "Game.h"
#include "AIBehaviors.h"

CSpinBot::CSpinBot(void)
{
	SetID(OBJ_SPINBOT);
	m_pBehavior = nullptr;
	m_pTargetFINAL = nullptr;
	m_cAI.Initialize(this);
	m_fTime = 0.0f;
	SetRadius(1.75f);
	SetColor(COLOR_AICORE);
	m_fRotSpeed = 0.0f;
	m_bIsHolding = false;
	m_fHoldTime = 0.0f;
}


CSpinBot::~CSpinBot(void)
{
}

void CSpinBot::Update(float fDelta)
{
	if( GetHealth() <= 0.0f )
	{
		SetAlive(false);
		SetIsActive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}

	float fMaxRot = GetHealth() > m_fMaxHP * .5f ? SPINBOT_ROTSPEED : SPINBOT_ROTSPEED*2.0f;
	float fMaxMove = GetHealth() > m_fMaxHP * .5f ? SPINBOT_MOVESPEED : SPINBOT_MOVESPEED*1.5f;

	if( m_fRotSpeed < fMaxRot )
	{
		m_fRotSpeed += fDelta * (SPINBOT_ROTSPEED*.25f);

		if( m_fRotSpeed > fMaxRot )
			m_fRotSpeed = fMaxRot;
	}

	if( m_fSpeed < fMaxMove )
	{
		m_fSpeed += fDelta * (SPINBOT_MOVESPEED*.5f);

		if( m_fSpeed > fMaxMove )
			m_fSpeed = fMaxMove;
	}
	
	m_fDamage = (m_fRotSpeed*.35f) * fDelta;

	m_cAI.Update(fDelta);

	if(GetWasHit() == true)
	{
		float temp = GetHitDuration();
		temp += fDelta;
		SetHitDuration(temp);
		SetBeingHit(1);
		SetColor(COLOR_TRASHBOT_HIT);

		if(HIT_CAP < temp)
		{
			SetWasHit(false);
			SetHitDuration(0.0f);
			SetColor(COLOR_TRASHBOT);
			SetBeingHit(0);
		}
	}

	KillCurrFrame();
}

bool CSpinBot::CheckCollision(IBaseObject* obj)
{
	if(obj == this || m_pBehavior == SpinBot::Idle)
		return false;

	int nType = obj->GetID();

	if(nType == OBJ_PLAYER)
	{
		if(SphereToSphere(this->GetSphere(), ((CPlayerObject*)obj)->GetSphere()))
		{
			AddToReactVector(obj);
			//CollisionResponse(obj);
			return true;
		}
	}

	return false;
}

void CSpinBot::CollisionResponse(IBaseObject* obj)
{
	int nType = obj->GetID();

	if(nType == OBJ_PLAYER)
	{
		obj->TakeDamage(this);
	}
}

void CSpinBot::TakeDamage(IBaseObject* _pIn)
{
	bool bInvulnerable = m_pBehavior == SpinBot::Idle || m_pBehavior == SpinBot::Enter || m_pBehavior == SpinBot::Reset;

	if( bInvulnerable )
		return;

	SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
	SetWasHit(true);
}