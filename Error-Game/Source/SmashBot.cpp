/************************************************************************************
* Filename:  		SmashBot.cpp
* Date:      		02/09/2013
* Mod. Date: 		03/12/2013
* Mod. Initials:	IA
* Author:    		Tyler D. Springer
* Purpose:   		This is the heavy melee enemy.
************************************************************************************/
#include "SmashBot.h"
#include "Definitions.h"
#include "Bullet.h"
#include "Game.h"
#include "GameplayState.h"
#include "Audio.h"
#include "Barrel.h"
#include "Crate.h"

CSmashBot::CSmashBot(void)
{
	SetID(OBJ_SMASH);
	m_pBullet = nullptr;
	m_bIsSmashing = false;
	SetRadius(2.0f);
	SetColor(COLOR_SMASHBOT);
	m_cAI.Initialize((CEnemyObject*)this);
	m_fSmashTime = 0.0f;
	SetBeingHit(0);
	m_fIsFading = 0.0f;
}

CSmashBot::~CSmashBot(void)
{

}

void CSmashBot::Update(float fDelta)
{
	if(m_fIsFading > 0.0f)
	{
		m_fIsFading -= fDelta;
		D3DXMATRIX tempCurr = GetMatrix();
		tempCurr[13] -= m_fIsFading * 0.05f;
		SetMatrix(tempCurr);

		if(m_fIsFading <= 0.0f)
			CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}
	if(m_fDying)
	{
		m_fDying += fDelta;

		if( m_fDying > m_fDieTime )
		{
			m_fIsFading = 2.0f;
			return;
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

	if( m_bIsSmashing )
		SetColor(COLOR_SMASHBOT_VUL);
	else
	{
		SetColor(COLOR_SMASHBOT);
		m_fScrollTime -= fDelta;
	}

	float animTime = GetAnimationTime();
	animTime += fDelta;
	if(GetWalking() != SMASH_WALK)
		animTime = min(animTime, m_fMaxAnim);
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

		m_bIsSmashing = false;
		D3DXMATRIX d3dMat=GetMatrix();
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,d3dMat);
		TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), SMASH_DEATH);
		m_fDieTime = tempAnim->m_fDuration;
		m_fDying = .00001f;
		SetBeingHit(0);
		ChangeAnimation(SMASH_DEATH);
		SetAlive(false);
		return;
	}

	m_cAI.Update(fDelta);
}

bool CSmashBot::CheckCollision(IBaseObject* pObj)
{
	if(pObj == this)
		return false;

	int nType = pObj->GetID();

	if(nType == OBJ_SMASH || nType == OBJ_SPIDER || nType == OBJ_INVERSION || nType == OBJ_TUNNEL)
	{
		if(SphereToSphere(this->GetSphere(), pObj->GetSphere()))
		{
			if( m_bIsSmashing == false || nType == OBJ_INVERSION || OBJ_TUNNEL)
			{
				AddToReactVector(pObj);
				//CollisionResponse(pObj);
				return true;
			}
		}
	}
	if(nType == OBJ_PIT)
	{
		Sphere tempSphere = GetSphere();
		tempSphere.m_Radius = 10.0f;
		if(SphereToAABB(tempSphere,((CDeathPit*)pObj)->GetAABB()))
		{
			((CDeathPit*)pObj)->SetTrapCloseTimer(2.0f);
			if(((CDeathPit*)pObj)->GetTrapIsOpen())
			{
				((CDeathPit*)pObj)->SetIsTrapOpen(false);
				((CDeathPit*)pObj)->ChangeAnimation(1);
			}
			return true;
		}
	}
	if(nType == OBJ_PLAYER)
	{
		if(SphereToSphere(this->GetSphere(), ((CPlayerObject*)pObj)->GetSphere()))
		{
			//m_bIsSmashing = true;
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}
	if(nType == OBJ_CONVEYOR)
	{
		if(SphereToAABB(GetSphere(), ((CConveyor*)pObj)->GetAABB()))
		{
			pObj->AddToReactVector(this);
			//pObj->CollisionResponse(this);
			return true;
		}
	}
	if(nType >= OBJ_VAT && nType <= OBJ_BARREL)
	{
		if(SphereToSphere(GetSphere(),pObj->GetSphere()))
		{
			//AddToReactVector(this);
			CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CSmashBot::CollisionResponse(IBaseObject* pObj)
{
	int nType = pObj->GetID();

	if(nType == OBJ_SMASH || nType == OBJ_SPIDER || nType == OBJ_INVERSION || nType == OBJ_TUNNEL)
	{
		Sphere tempSphere = pObj->GetSphere();
		D3DXVECTOR3 V = tempSphere.m_Center - this->GetSphere().m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = Distance/Mag + EPISILON;
		V = V * (X + 0.015f);
		D3DXMATRIX tempMat = pObj->GetMatrix();
		D3DXMATRIX tempSetMat = GetMatrix();
		if(nType == OBJ_TUNNEL)
			tempMat._43 *= 0.85f;
		tempSetMat._41 = tempMat._41 - V.x;
		tempSetMat._43 = tempMat._43 - V.z;
		this->SetMatrix(tempSetMat);
	}

	if(nType == OBJ_PLAYER)
	{
		//m_bIsSmashing = true;
		Sphere tempSphere = ((CPlayerObject*)pObj)->GetSphere();
		D3DXVECTOR3 V =  this->GetSphere().m_Center - tempSphere.m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = Distance/Mag + EPISILON;
		V = V * (X + 0.025f);
		D3DXMATRIX tempMat = pObj->GetMatrix();
		D3DXMATRIX tempSetMat = GetMatrix();
		tempMat._41 = tempSetMat._41 - V.x;
		tempMat._43 = tempSetMat._43 - V.z;
		pObj->SetMatrix(tempMat);
	}

	if(nType >= OBJ_VAT && nType <= OBJ_BARREL)
		pObj->SetHealth(0);


	// TODO - UPDATE DAMAGE AS WEAPON GET DAMAGE WHEN WEPS ADDED
}

void CSmashBot::TakeDamage(IBaseObject* _pIn) 
{
	if(_pIn->GetID() == OBJ_POOL)
	{
		SetHealth(GetHealth() - ((CMoltenPool*)_pIn)->GetDamage());
		SetWasHit(true);
		return;
	}
	else if (_pIn->GetID() == OBJ_TURRET)
	{
		SetHealth(GetHealth() - ((CTurret*)_pIn)->GetDamage());
		SetWasHit(true);
		return;
	}

	if( _pIn->GetID() == OBJ_BULLET_EXP )
	{
		if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
			SetHealth(0.0f);
		else
			SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
		return;
	}

	if( m_bIsSmashing )
	{
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
	}
	else
	{
		if( _pIn->GetID() == OBJ_BULLET_BEAM || _pIn->GetID() == OBJ_BULLET_TESLA )
		{
			SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage()*.5f);
			SetWasHit(true);
			return;
		}
		D3DXVECTOR3 d3dMyZ(GetMatrix()._31, GetMatrix()._32, GetMatrix()._33);
		D3DXVECTOR3 d3dIn(_pIn->GetMatrix()._41, _pIn->GetMatrix()._42, _pIn->GetMatrix()._43);
		D3DXVECTOR3 d3dToTarget = d3dIn - D3DXVECTOR3(GetMatrix()._41, GetMatrix()._42, GetMatrix()._43);
		float fDot = D3DXVec3Dot(&d3dMyZ, &d3dToTarget);

		if( fDot < 0.0f)
		{
			SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
			SetWasHit(true);
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_BULLETHIT,_pIn->GetMatrix());
		}
		else
		{
			float fSafe = GetSphere().m_Radius * GetSphere().m_Radius + ((CBullet*)_pIn)->GetSphere().m_Radius * ((CBullet*)_pIn)->GetSphere().m_Radius + .5f;
			D3DXVECTOR3 d3dFinal = *(D3DXVECTOR3*)&_pIn->GetMatrix()[12], d3dToBullet = *(D3DXVECTOR3*)&_pIn->GetMatrix()[12] - *(D3DXVECTOR3*)&GetMatrix()[12];
			if( D3DXVec3LengthSq(&d3dToBullet) > fSafe )
			{
				D3DXVECTOR3 d3dDir;
				D3DXVec3Normalize(&d3dDir, &d3dToBullet);
				d3dDir *= GetSphere().m_Radius;
				d3dFinal = *(D3DXVECTOR3*)&GetMatrix()[12] + d3dDir;
			}
			D3DXMATRIX d3dMat = GetMatrix();
			d3dMat[12]=d3dFinal.x;
			d3dMat[13]=d3dFinal.y;
			d3dMat[14]=d3dFinal.z;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_INVULSPARK,d3dMat);
		}
	}
}

void CSmashBot::Render()
{
	CRenderer* pRenderer = CRenderer::GetInstance();
	pRenderer->Render(this);
}