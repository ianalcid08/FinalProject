#include "SpiderBot.h"
#include "Pickup.h"
#include "Game.h"
#include "GameplayState.h"
#include "PlayerObject.h"
#include "AIBehaviors.h"

CSpiderBot::CSpiderBot(void)
{
	SetID(OBJ_SPIDER);
	SetRadius(2.0f);
	SetColor(COLOR_SMASHBOT);
	m_cAI.Initialize((CEnemyObject*)this);
	SetBeingHit(0);
	m_bIsWalking = true;
}


CSpiderBot::~CSpiderBot(void)
{
}

void CSpiderBot::Update(float fDelta)
{
	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);

	if(GetWasHit() == true)
	{
		float temp = GetHitDuration();
		temp += fDelta;
		SetHitDuration(temp);
		SetBeingHit(1);
		SetColor(COLOR_SMASHBOT_HIT);

		if(HIT_CAP < temp)
		{
			SetWasHit(false);
			SetHitDuration(0.0f);
			SetBeingHit(0);
		}
	}

	if( GetHealth() <= 0 )
	{
		CPickup* pPickup = nullptr;
		if(CGame::GetInstance()->GetOF()->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
		{
			pPickup->SetMatrix(GetMatrix());
		}
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,GetMatrix());
		
		SetBeingHit(0);
		SetAlive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}

	m_cAI.Update(fDelta);
}

bool CSpiderBot::CheckCollision(IBaseObject* pObj)
{
	if(pObj == this)
		return false;
	
	int nType = pObj->GetID();

	if( nType == OBJ_TURRET || nType == OBJ_INVERSION )
		Utilities::CheckAvoid(this, pObj);
	
	if(nType == OBJ_PLAYER)
	{
		if(SphereToSphere(this->GetSphere(), ((CPlayerObject*)pObj)->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}

	if(nType >= OBJ_VAT && nType <= OBJ_BARREL)
	{
		if(SphereToSphere(GetSphere(),pObj->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}

	if(nType == OBJ_TUNNEL)
	{
		if(SphereToSphere(GetSphere(),pObj->GetSphere()))
		{
			CollisionResponse(pObj);
			return true;
		}
	}
	
	return false;
}

void CSpiderBot::CollisionResponse(IBaseObject* pObj)
{
	int nType = pObj->GetID();

	if(nType == OBJ_PLAYER)
	{
		Sphere tempSphere = ((CPlayerObject*)pObj)->GetSphere();
		D3DXVECTOR3 V =  this->GetSphere().m_Center - tempSphere.m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = Distance/Mag + EPISILON;
		V = V * X;
		D3DXMATRIX tempMat = pObj->GetMatrix();
		D3DXMATRIX tempSetMat = GetMatrix();
		tempMat._41 = tempSetMat._41 - V.x;
		tempMat._43 = tempSetMat._43 - V.z;
		pObj->SetMatrix(tempMat);
	}

	if(nType == OBJ_TUNNEL)
	{
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
	}

	if(nType >= OBJ_VAT && nType <= OBJ_BARREL)
		pObj->SetHealth(0);
}

 void CSpiderBot::TakeDamage(IBaseObject* _pIn) 
 {
	 if(_pIn->GetID() <= OBJ_BULLET_PLAYER && _pIn->GetID() >= OBJ_BULLET_EXP && _pIn->GetID() != OBJ_BULLET_ENEMY)
	 {
		if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
			SetHealth(0.0f);
		else
			SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
	 }
 }