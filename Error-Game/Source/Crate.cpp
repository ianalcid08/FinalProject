/************************************************************************************
 * Filename:  		Crate.h
 * Date:      		02/09/2013
 * Mod. Date: 		3/11/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable crate
 ************************************************************************************/
#include "Crate.h"
#include "game.h"
#include "GameplayState.h"
#include "Audio.h"

CCrate::CCrate(void)
{
	SetID(OBJ_CRATE);
	SetRadius(0.5f);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_fDelayTimer = 60.0f;
	SetHealth(25.0f);
	SetAlive(true);
	SetIsActive(true);
	D3DXMatrixIdentity(&m_d3dPreviousPos);
	m_fFallingTimer = 2.0f;
	m_bIsFalling = false;
	m_bResetPos = false;
}

CCrate::~CCrate(void)
{
}

void CCrate::Update(float fDelta)
{

	m_fDelayTimer -= fDelta;

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
			D3DXMATRIX d3dBurn = GetMatrix();
			d3dBurn._42 = 0.0f;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MOLTENBURN,d3dBurn);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
		return;
	}
	else if(m_fDelayTimer <= 0)
	{
		Frustum tempFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(),tempFrustum))
		{
			cout << "crate destroyed" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}
	else if(m_bResetPos)
	{
		SetMatrix(m_d3dPreviousPos);
		m_bResetPos = false;
	}
	KillCurrFrame();

	if(GetHealth() <= 0.0f)
	{
		D3DXMATRIX d3dMat=GetMatrix();
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);

		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);

		SetAlive(false);
		SetIsActive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
	}
	m_d3dPreviousPos = GetMatrix();
}
void CCrate::Render()
{
	if(m_bIsFalling)
		CRenderer::GetInstance()->Render(this);
	else
	{
		CRenderer::GetInstance()->Render(this);
		CRenderer::GetInstance()->RenderShadow(this);
	}

}
bool CCrate::CheckCollision(IBaseObject* obj)
{
	if(obj == this)
		return false;
	if(obj->GetID() == OBJ_PIT)
	{
		if(((CDeathPit*)obj)->GetTrapIsOpen() == false)
			return false;

		if(SphereToAABB(GetSphere(),((CDeathPit*)obj)->GetAABB()))
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

void CCrate::CollisionResponse(IBaseObject* obj)
{

}

 void CCrate::TakeDamage(IBaseObject* _pIn)
 {
	 SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
 }
