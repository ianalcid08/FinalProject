/************************************************************************************
 * Filename:  		BurnBot.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the kamikaze enemy.
 ************************************************************************************/
#include "BurnBot.h"
#include "Game.h"
#include "GameplayState.h"
#include "MoltenPool.h"
#include "Audio.h"
#include "AIBehaviors.h"

CBurnBot::CBurnBot(void)
{
	SetID(OBJ_BURN);
	SetRadius(0.75f);
	SetColor(COLOR_PATROLBOT);
	SetHealth(75);
	SetHitDuration(0.0f);
	SetWasHit(false);
	m_cAI.Initialize((CEnemyObject*)this);
	SetBeingHit(0);
	m_bCloseToTarget = false;
	m_pTarget = nullptr;
	m_fExplodeTimer = 0.0f;
	m_bDontDropPool = false;
	m_fDropWait = 0.0f;
}

CBurnBot::~CBurnBot(void)
{
}

void CBurnBot::Update(float fDelta)
{
	if(m_fDying)
	{
		m_fDying += fDelta;

		if( m_fDying > m_fDieTime )
		{
			CGame::GetInstance()->GetOF()->Destroy(this);
			SetIsActive(false);
		}
		float animTime = GetAnimationTime();
		animTime += fDelta;
		if(animTime > m_fDieTime)
			animTime = m_fDieTime;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);
		return;
	}

	if(m_bDontDropPool && m_fDropWait > 0.0f)
	{
		m_fDropWait -= fDelta;
		if(m_fDropWait <= 0.0f)
		{
			m_fDropWait = 0.0f;
			m_bDontDropPool = false;
		}
	}

	KillCurrFrame();

	if(GetWasHit() == true)
	{
		float ftemp = GetHitDuration();
		ftemp += fDelta;
		SetHitDuration(ftemp);
		SetBeingHit(1);
		
		SetColor(COLOR_PATROLBOT_HIT);

		if(HIT_CAP < ftemp)
		{
			SetWasHit(false);
			SetHitDuration(0.0f);
			SetBeingHit(0);
			SetColor(COLOR_PATROLBOT);
		}
	}

	m_cAI.Update(fDelta);

	if( GetHealth() <= 0 )
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DMGEXPLOSION,GetMatrix());
		if(!m_bDontDropPool)
		{
			CMoltenPool* pMoltenPool = nullptr;
			if(CGame::GetInstance()->GetOF()->Create(OBJ_POOL, (IBaseObject**)&pMoltenPool))
			{
				pMoltenPool->SetMatrix(GetMatrix());
				pMoltenPool->SetOriginalMatrix(GetMatrix());
				pMoltenPool->SetRadius(1.0f);
				pMoltenPool->SetScale(1.25f);
				pMoltenPool->SetScaleMod(1.4f);
				pMoltenPool->SetScaleTimer(1.0f);
				pMoltenPool->SetPoolStage(1);
				pMoltenPool->SetScalingMod(0.0f);
				pMoltenPool->SetDroppedID(1);
			}
		}
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,GetMatrix());
		SetBeingHit(0);
		TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), 1);
		m_fDieTime = tempAnim->m_fDuration;
		m_fDying = .00001f;
		ChangeAnimation(1);
		SetAlive(false);
	}

	if(m_bCloseToTarget)
	{
		int nTemp = ((int)(m_fExplodeTimer*10)%2);

		float animTime = GetAnimationTime();
		animTime += fDelta;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);

		if(nTemp)
			SetBeingHit(NOTHIT);
		else
			SetBeingHit(BURNEXPLODE);
	}
}

bool CBurnBot::CheckCollision(IBaseObject* pObj)
{
	if(pObj == this)
		return false;

	if(!pObj->GetAlive())
		return false;

	if(!GetAlive())
		return false;

	int nType = pObj->GetID();

	bool bAvoid = nType == OBJ_SMASH || nType == OBJ_SPIDER || nType >= OBJ_VAT && nType <= OBJ_BARREL || nType == OBJ_BOSSINV || nType == OBJ_INVERSION;

	if( bAvoid )
		Utilities::CheckAvoid(this, pObj);

	if(nType >= OBJ_TRASH && nType <= OBJ_AIFIST ||  nType == OBJ_PLAYER ||  nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || OBJ_TUNNEL)
	{
		if(SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			if(OBJ_TUNNEL)
				CollisionResponse(pObj);
			else
				AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CBurnBot::CollisionResponse(IBaseObject* pObj)
{
	int nType = pObj->GetID();

	Sphere tempSphere = pObj->GetSphere();
	D3DXVECTOR3 V = tempSphere.m_Center - this->GetSphere().m_Center;
	float Mag = D3DXVec3Length(&V);
	float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
	float X = Distance/Mag + EPISILON;
	V = V * X;
	D3DXMATRIX tempMat = pObj->GetMatrix();
	if(nType == OBJ_TUNNEL)
		tempMat._43 *= 0.85f;
	D3DXMATRIX tempSetMat = GetMatrix();
	tempSetMat._41 = tempMat._41 - V.x;
	tempSetMat._43 = tempMat._43 - V.z;
	this->SetMatrix(tempSetMat);


	if( nType == OBJ_PLAYER )
	{
		if(!m_bCloseToTarget)
		{
			m_bCloseToTarget = true;
			m_fExplodeTimer = BURN_CHARGETIME;
			SetBeingHit(BURNEXPLODE);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_BURNALERT);
			ChangeAnimation(0);
		}
	}
}

void CBurnBot::TakeDamage(IBaseObject* _pIn)
{
	 if (_pIn->GetID() == OBJ_TURRET)
	 {
		 SetHealth(GetHealth() - ((CTurret*)_pIn)->GetDamage());
		 SetWasHit(true);
	 }
	if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
		SetHealth(0.0f);
	else
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
	SetWasHit(true);
}