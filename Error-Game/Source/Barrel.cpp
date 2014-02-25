/************************************************************************************
 * Filename:  		CBarrel.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		3/12/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable barrel
 ************************************************************************************/
#include "Barrel.h"
#include "Game.h"
#include "GameplayState.h"
#include "Audio.h"

CBarrel::CBarrel(void)
{
	SetID(OBJ_BARREL);
	m_pGame = CGame::GetInstance();
	SetRadius(0.5f);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	SetHealth(25.0f);
	SetAlive(true);
	SetBeingHit(NOTHIT);
	SetIsActive(true);
	m_fBarrelTimer = 120.0f;
	D3DXMatrixIdentity(&m_d3dPreviousPos);
	m_bIsFalling = false;
	m_fFallingTimer = 2.0f;
	m_fDying = 0.0f;
	m_bResetPos = false;
}


CBarrel::~CBarrel(void)
{
}

void CBarrel::Update(float fDelta)
{
	if(m_bIsFalling)
	{
		m_fFallingTimer -= fDelta;
		D3DXMATRIX temp = GetMatrix();
		D3DXMATRIX tempRot;
		D3DXMatrixRotationX(&tempRot,fDelta * 10.0f);
		tempRot *= temp;
		tempRot[13] -= m_fFallingTimer * 0.08f;
		SetMatrix(tempRot);

		if(m_fFallingTimer <= 0.0f)
		{
			m_fDying = 0.0f;
			D3DXMATRIX d3dBurn = GetMatrix();
			d3dBurn._42 = 0.0f;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MOLTENBURN,d3dBurn);
			SetIsActive(false);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
		return;
	}
	else if(m_fIsFading > 0.0f)
	{
		m_fIsFading -= fDelta;
		D3DXMATRIX tempCurr = GetMatrix();
		tempCurr[13] -= m_fIsFading * 0.05f;
		SetMatrix(tempCurr);

		if(m_fIsFading <= 0.0f)
		{
			m_fDying = 0.0f;
			SetIsActive(false);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
		return;
	}
	else if(m_fDying)
	{
		m_fDying += fDelta;

		if( m_fDying > m_fDieTime )
		{
			m_fIsFading = 2.0f;
			m_fDying = 0.0f;
			return;
		}
		float animTime = GetAnimationTime();
		animTime += fDelta;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);
		return;
	}
	else if(m_bResetPos)
	{
		SetMatrix(m_d3dPreviousPos);
		m_bResetPos = false;
	}
	KillCurrFrame();

	m_fBarrelTimer -= fDelta;

	if(m_fBarrelTimer <= 0)
	{
		Frustum tTestFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(), tTestFrustum))
		{
			m_fDying = 0.0f;
			SetIsActive(false);
			SetAlive(false);
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}

	if(GetHealth() <= 0.0f)
	{
		CExplodingBullet* tempBullet = nullptr;
		if(m_pGame->GetOF()->Create(OBJ_BULLET_EXP,(IBaseObject**)&tempBullet))
		{
			tempBullet->SetMatrix(GetMatrix());
			tempBullet->SetType(BARRELBULLET);
			tempBullet->SetDamage(BARREL_DAMAGE);   
			tempBullet->SetMaxRadius(BARREL_RADIUS); 
			tempBullet->SetSpeed(BARREL_SPEED); 

			tempBullet->SetRadius(0.0f);
			tempBullet->SetColor(D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f));
		}

		D3DXMATRIX transMat1,transMat2,transMat3,transMat4;
		D3DXMatrixTranslation(&transMat1,1.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat2,-2.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat3,1.0f,0.0,1.0);
		D3DXMatrixTranslation(&transMat4,0.0f,0.0,-2.0);
		D3DXMATRIX d3dMat=GetMatrix();
		d3dMat *= transMat1;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DMGEXPLOSION,d3dMat);
		d3dMat *= transMat2;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DMGEXPLOSION,d3dMat);
		d3dMat *= transMat3;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DMGEXPLOSION,d3dMat);
		d3dMat *= transMat4;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DMGEXPLOSION,d3dMat);

		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		SetAlive(false);
		SetIsActive(false);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,d3dMat);
		TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), 0);
		m_fDieTime = tempAnim->m_fDuration;
		m_fDying = .00001f;
		ChangeAnimation(0);
	}
	m_d3dPreviousPos = GetMatrix();
}
void CBarrel::Render()
{
	if(m_bIsFalling )
		CRenderer::GetInstance()->Render(this);
	else if(GetHealth() <= 0.0f)
	{
		CRenderer::GetInstance()->Render(this);
	}
	else
	{
		CRenderer::GetInstance()->Render(this);
		CRenderer::GetInstance()->RenderShadow(this);
	}

}
bool CBarrel::CheckCollision(IBaseObject* obj)
{
	if(obj == this)
		return false;
	if(obj->GetID() == OBJ_PIT)
	{
		if(((CDeathPit*)obj)->GetTrapIsOpen() == false)
			return false;

		if(SphereToAABB(GetSphere(),obj->GetAABB()))
		{
			obj->CollisionResponse(this);
			return true;
		}
	}
	else if(obj->GetID() == OBJ_CRATE || obj->GetID() == OBJ_BARREL)
	{
		if(SphereToSphere(GetSphere(), obj->GetSphere()))
		{
			m_bResetPos = true;
			return true;
		}
	}
	else if(obj->GetID() == OBJ_CONVEYOR)
	{
		if(SphereToAABB(GetSphere(), ((CConveyor*)obj)->GetAABB()))
		{
			obj->AddToReactVector(this);
			//obj->CollisionResponse(this);
			return true;
		}
	}


	return false;
}

void CBarrel::CollisionResponse(IBaseObject* obj)
{

}

void CBarrel::TakeDamage(IBaseObject* _pIn)
{
	 SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
}
