/************************************************************************************
 * Filename:  		AIFist.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the melee final boss.
 ************************************************************************************/
#include "AIFist.h"
#include "PlayerObject.h"
#include "Game.h"
#include "GameplayState.h"
#include "AIBehaviors.h"
#include "Effect.h"

CAIFist::CAIFist(void)
{
	SetID(OBJ_AIFIST);
	m_cAI.Initialize(this);
	m_pBehavior = nullptr;
	m_fTime = 0.0f;
	SetRadius(2.0f);
	SetColor(COLOR_AICORE);
	m_pSmokeEffect = nullptr;
	m_pBurnEffect = nullptr;
	m_pFireEffect = nullptr;
}

CAIFist::~CAIFist(void)
{

}

void CAIFist::Update(float fDelta)
{
	if( GetHealth() <= 0.0f )
	{
		if(m_pSmokeEffect)
			CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pSmokeEffect);
		if(m_pBurnEffect)
			CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pBurnEffect);
		if(m_pFireEffect)
			CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pFireEffect);
		SetHealth(0.0f);
		SetAlive(false);
		SetIsActive(false);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,GetMatrix());		
		CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}
	else if( GetHealth() <= MAXHP_AICORE/4.0f && !m_pFireEffect)
	{
		m_pFireEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_BURN,GetMatrix());
	}
	else if( GetHealth() <= MAXHP_AICORE/3.0f && !m_pBurnEffect)
	{
		m_pBurnEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMOKEBURN,GetMatrix());
	}
	else if( GetHealth() <= MAXHP_AICORE/2.0f && !m_pSmokeEffect)
	{
		m_pSmokeEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMOKE,GetMatrix());
	}

	if(m_pSmokeEffect)
		m_pSmokeEffect->SetMatrix(GetMatrix());
	if(m_pBurnEffect)
		m_pBurnEffect->SetMatrix(GetMatrix());
	if(m_pFireEffect)
		m_pFireEffect->SetMatrix(GetMatrix());

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

	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);
}

bool CAIFist::CheckCollision(IBaseObject* obj)
{

	return false;
}

void CAIFist::CollisionResponse(IBaseObject* obj)
{
}

void CAIFist::TakeDamage(IBaseObject* _pIn)
{
	if( m_pBehavior == AIFist::Enter )
		return;

	SetWasHit(true);
	SetHealth(GetHealth()-((CBullet*)_pIn)->GetDamage());
	//std::cout << "FIST: " << GetHealth() << endl;
}