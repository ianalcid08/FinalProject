/************************************************************************************
 * Filename:  		TrashBot.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		04/20/2013
 * Mod. Initials:	IA
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the basic melee enemy.
 ************************************************************************************/
#include "TrashBot.h"
#include "Definitions.h"
#include "Bullet.h"
#include "PlayerObject.h"
#include "Game.h"
#include "Pickup.h"
#include "Audio.h"
#include "GameplayState.h"
#include "Crate.h"
#include "Barrel.h"
#include "AIBehaviors.h"

enum EANIM {ATRASH_WALK, ATRASH_ATTACK, ATRASH_DIE};

CTrashBot::CTrashBot(void)
{
	SetID(OBJ_TRASH);
	SetDamage(10);
	SetRadius(0.5f);
	SetColor(COLOR_TRASHBOT);
	m_cAI.Initialize((CEnemyObject*)this);
	SetHealth(MAXHP_TRASH);
	SetHitDuration(0.0f);
	SetWasHit(false);
	SetBeingHit(0);
	m_pOF = CGame::GetInstance()->GetOF();
	m_pTargetFINAL = nullptr;
	m_fIsFading = 0.0f;
	m_bCollided = false;
	m_bStop = false;
	m_bPlayerStop = false;
	m_fPlayerStopTimer = 0.0f;
	m_fFallingTimer = 2.0f;
	m_fSpawnTimer = 0.0f;
}

CTrashBot::~CTrashBot(void)
{

}

void CTrashBot::Update(float fDelta)
{
	// translate stuff down.
	if(m_bIsFalling)
	{
		m_fFallingTimer -= fDelta;
		D3DXMATRIX temp = GetMatrix();
		D3DXMATRIX tempRot;
		D3DXMatrixRotationX(&tempRot,fDelta * 10.0f);
		tempRot *= temp;
		tempRot[13] -= m_fFallingTimer * 0.05f;
		SetMatrix(tempRot);

		if(m_fFallingTimer <= 0.0f)
		{
			D3DXMATRIX d3dBurn = GetMatrix();
			d3dBurn._42 = 0.0f;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MOLTENBURN,d3dBurn);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
		return;
	}

	if(m_fIsFading > 0.0f)
	{
		m_fIsFading -= fDelta;
		D3DXMATRIX tempCurr = GetMatrix();
		tempCurr[13] -= m_fIsFading * 0.05f;
		SetMatrix(tempCurr);

		if(m_fIsFading <= 0.0f)
			CGame::GetInstance()->GetOF()->Destroy(this);
		//KillCurrFrame();
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

	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);

	if(m_bCollided)
		m_bCollided = false;


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

	if( m_fSpeed < TRASHBOT_SPEED )
	{
		m_fSpeed += fDelta * TRASHBOT_SPEED;

		if( m_fSpeed > TRASHBOT_SPEED )
			m_fSpeed = TRASHBOT_SPEED;
	}

	if(m_fSpawnTimer > 0.0f)
	{
		m_fSpawnTimer -= fDelta;
		Movement::MoveForward(this, fDelta, m_fSpeed);
	}
	else if(!m_bPlayerStop)
		m_cAI.Update(fDelta);

	if( GetHealth() <= 0 )
	{
		if(CGameplayState::GetInstance()->GeneratePickup())
		{
			CPickup* pPickup = nullptr;
			if(m_pOF->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
			{
				pPickup->SetMatrix(GetMatrix());
			}
		}

		// EXPLOSION
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);

		D3DXMATRIX d3dMat=GetMatrix();
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);

		ChangeAnimation(ATRASH_DIE);
		m_fDieTime = GetCurrentAnimation()->m_fDuration;
		m_fDying = .00001f;
		SetBeingHit(0);
		SetAlive(false);
		return;
	}

	if(GetReactVector().size() <= 2)
		m_bStop = false;

	if(m_bPlayerStop)
	{
		m_fPlayerStopTimer -= fDelta;

		if(m_fPlayerStopTimer <= 0.0f)
		{
			ChangeAnimation(ATRASH_WALK);
			m_bPlayerStop = false;
		}
	}
}

void CTrashBot::Render()
{
	if(m_bIsFalling || GetHealth() <= 0 )
		CRenderer::GetInstance()->Render(this);
	else
	{
		CRenderer::GetInstance()->Render(this);
		CRenderer::GetInstance()->RenderShadow(this);
	}

}
bool CTrashBot::CheckCollision(IBaseObject* pObj)
{
	if( pObj == this )
		return false;

	if(!pObj->GetAlive())
		return false;

	if(!GetAlive())
		return false;

	int nType = pObj->GetID();

	bool bAvoid = nType == OBJ_SMASH || nType == OBJ_SPIDER || nType >= OBJ_VAT && nType <= OBJ_BARREL || nType == OBJ_BOSSINV || nType == OBJ_INVERSION;

	if( bAvoid )
		Utilities::CheckAvoid(this, pObj);

	if( nType >= OBJ_TRASH && nType <= OBJ_AIFIST || nType == OBJ_PLAYER ||  nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || nType == OBJ_BOSSINV)
	{
		if( SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}

	if( nType == OBJ_CONVEYOR )
	{
		if( SphereToAABB(GetSphere(), ((CConveyor*)pObj)->GetAABB()) )
		{
			pObj->CollisionResponse(this);
			return true;
		}
	}
	if(nType == OBJ_PIT)
	{
		// is the trap isn't open ignore the pit
		if(((CDeathPit*)pObj)->GetTrapIsOpen() == false)
			return false;

		Sphere tempSphere = GetSphere();
		tempSphere.m_Radius = 0.1f;
		if(SphereToAABB(tempSphere,((CDeathPit*)pObj)->GetAABB()))
		{
			pObj->CollisionResponse(this);
			return true;
		}
	}
	return false;
}

void CTrashBot::CollisionResponse(IBaseObject* pObj)
{
	int nType = pObj->GetID();

	if(nType != OBJ_PATROL && !m_bCollided)
	{
		Sphere tempSphere = pObj->GetSphere();
		D3DXVECTOR3 V = tempSphere.m_Center - this->GetSphere().m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = (Distance/Mag) + EPISILON;
		V = V * X;
		D3DXMATRIX ObjMatrix = pObj->GetMatrix();
		D3DXMATRIX MyMatrix = GetMatrix();
		MyMatrix._41 = ObjMatrix._41 - V.x;
		MyMatrix._43 = ObjMatrix._43 - V.z;
		//if(GetReactVector().size() >= 3)
		//	m_bStop = true;
		SetMatrix(MyMatrix);
	
		if( nType == OBJ_PLAYER )
		{
			m_fSpeed = 0.0f;
			m_bPlayerStop = true;
			m_pTargetFINAL = (CPlayerObject*)pObj;
			pObj->TakeDamage(this);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_TRASHBOT_ATTACK);
			TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), ATRASH_ATTACK);
			m_fPlayerStopTimer = tempAnim->m_fDuration;
			ChangeAnimation(ATRASH_ATTACK);
		}
	}
}

void CTrashBot::TakeDamage(IBaseObject* _pIn)
{
	 if(_pIn->GetID() == OBJ_POOL)
	 {
		SetHealth(GetHealth() - ((CMoltenPool*)_pIn)->GetDamage());
		SetWasHit(true);
	 }
	 else if (_pIn->GetID() == OBJ_TURRET)
	 {
		 SetHealth(GetHealth() - ((CTurret*)_pIn)->GetDamage());
		 SetWasHit(true);
	 }
	 else
	 {
		if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
			SetHealth(0.0f);
		else
			SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
	 }
}
