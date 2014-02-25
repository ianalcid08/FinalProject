/************************************************************************************
 * Filename:  		CPatrolBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the ranged enemy.
 ************************************************************************************/
#include "PatrolBot.h"
#include "PlayerObject.h"
#include "GameplayState.h"
#include "Bullet.h"
#include "Game.h"
#include "Audio.h"
#include "AIBehaviors.h"

CPatrolBot::CPatrolBot(void)
{
	SetID(OBJ_PATROL);
	SetColor(COLOR_PATROLBOT);
	SetRadius(0.5f);
	SetHealth(MAXHP_PATROL);
	SetHitDuration(0.0f);
	SetWasHit(false);
	m_cAI.Initialize((CEnemyObject*)this);
	m_nSideCollided = 0;
	m_fFireTime = 0.0f;
	SetBeingHit(0);
	m_fIsFading = 0.0f;
}

CPatrolBot::~CPatrolBot(void)
{

}

void CPatrolBot::Update(float fDelta)
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

	}

	if(m_fDying)
	{
		float animTime = GetAnimationTime();
		animTime += fDelta;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);
		return;
	}
	else
		KillCurrFrame();

	if(m_bIsSpawning)
	{
		D3DXMATRIX temp = GetMatrix();
		if(temp[13] >= 2.5f)
		{
			temp[13] -= (fDelta * 6.0f);
			SetMatrix(temp);
			D3DXVECTOR3 tempPos = CGameplayState::GetInstance()->GetPlayers()[0]->GetSphere().m_Center;
			Movement::TurnTo(this,tempPos,fDelta,TRASHBOT_ROT_SPEED);
			return;
		}
		else if(temp[13] >= 0.0f)
		{
			temp[13] -= (fDelta * 4.0f);
			SetMatrix(temp);
			D3DXVECTOR3 tempPos = CGameplayState::GetInstance()->GetPlayers()[0]->GetSphere().m_Center;
			Movement::TurnTo(this,tempPos,fDelta,TRASHBOT_ROT_SPEED);
		}
		else
		{
			temp[13] = 0.0f;
			m_bIsSpawning = false;
			m_cAI.Update(fDelta);

		}
	}
	else
		m_cAI.Update(fDelta);

	m_fFireTime += fDelta;



	

	Sphere tSphere = GetSphere();

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

	if( GetHealth() <= 0 )
	{
		if(CGameplayState::GetInstance()->GeneratePickup())
		{
			CPickup* pPickup = nullptr;
			if(CGame::GetInstance()->GetOF()->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
			{
				D3DXMATRIX temp = GetMatrix();
				temp[13] = 0.0f;
				pPickup->SetMatrix(temp);
			}
		}
		SetBeingHit(0);
		D3DXMATRIX d3dMat=GetMatrix();
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);
		TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), 1);
		m_fDieTime = tempAnim->m_fDuration;
		m_fDying = .00001f;
		ChangeAnimation(1);
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		SetAlive(false);
	}
	
}

bool CPatrolBot::CheckCollision(IBaseObject* pObj)
{
	if(pObj == this)
		return false;

	if(!pObj->GetAlive())
		return false;

	if(!GetAlive())
		return false;
	if(m_bIsSpawning)
		return false;

	int nType = pObj->GetID();


	if( nType >= OBJ_TRASH && nType <= OBJ_AIFIST || nType == OBJ_PLAYER ||  nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || nType == OBJ_BOSSINV || nType == OBJ_TUNNEL)
	{
		m_nSideCollided = SphereToSphere(GetSphere(), pObj->GetSphere());

		if(m_nSideCollided)
		{
			CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CPatrolBot::CollisionResponse(IBaseObject* pObj)
{
	D3DXVECTOR3 d3dTempVec = GetVelocity();
		
	int nType = pObj->GetID();

	D3DXVECTOR3 d3dTestvec;

	Sphere tempSphere = pObj->GetSphere();
	D3DXVECTOR3 V = tempSphere.m_Center - this->GetSphere().m_Center;
	float Mag = D3DXVec3Length(&V);
	float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
	float X = Distance/Mag + EPISILON;
	V = V * (X + 0.1f);
	D3DXMATRIX tempMat = pObj->GetMatrix();
	if(nType == OBJ_TUNNEL)
		tempMat._43 *= 0.85f;
	D3DXMATRIX tempSetMat = GetMatrix();
	tempSetMat._41 = tempMat._41 - V.x;
	tempSetMat._43 = tempMat._43 - V.z;
	this->SetMatrix(tempSetMat);

	d3dTestvec =  GetSphere().m_Center - ((CEnemyObject*)pObj)->GetSphere().m_Center;

	if(abs(d3dTestvec.x) < abs(d3dTestvec.z))
	{
		if( (m_nSideCollided == 3 || m_nSideCollided == 2) && d3dTempVec.z < 0)
			d3dTempVec.z *= -1;
		else if( (m_nSideCollided == 1 || m_nSideCollided == 4) && d3dTempVec.z > 0)
			d3dTempVec.z *= -1;
	}
	else 
	{
		if( (m_nSideCollided == 1 || m_nSideCollided == 2) && d3dTempVec.x > 0)
			d3dTempVec.x *= -1;
		else if( (m_nSideCollided == 3 || m_nSideCollided == 4) && d3dTempVec.x < 0)
			d3dTempVec.x *= -1;
	}

	SetVelocity(d3dTempVec);
	
}

void CPatrolBot::TakeDamage(IBaseObject* _pIn)
{
	if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
		SetHealth(0.0f);
	
	else
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
	SetWasHit(true);
}