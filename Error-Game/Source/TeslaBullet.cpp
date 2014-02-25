#include "TeslaBullet.h"
#include "PlayerObject.h"
#include "Game.h"
#include "GameplayState.h"

CTeslaBullet::CTeslaBullet(void)
{
	SetID(OBJ_BULLET_TESLA);
	m_nType = TESLA;
}


CTeslaBullet::~CTeslaBullet(void)
{
}

void CTeslaBullet::Update(float fDelta)
{
	if( m_fDuration > 0.0f )
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}
	m_fDuration += fDelta;

	SetDamage(DMG_TESLA * fDelta);

	D3DXMATRIX d3dTeslaMatrix = m_pOwner->GetGunMatrix();
	d3dTeslaMatrix._42 = 0.0f;
	SetMatrix(d3dTeslaMatrix);
	D3DXMATRIX d3dTranslate, d3dFL = GetMatrix(), d3dFR = GetMatrix(), d3dF = GetMatrix();
	D3DXMatrixTranslation(&d3dTranslate, -TESLA_WIDTH*.5f, 0.0f, TESLA_HEIGHT);
	d3dFL = d3dTranslate * d3dFL;
	D3DXMatrixTranslation(&d3dTranslate, TESLA_WIDTH*.5f, 0.0f, TESLA_HEIGHT);
	d3dFR = d3dTranslate * d3dFR;
	D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, TESLA_HEIGHT);
	d3dF = d3dTranslate * d3dF;

	D3DXVECTOR3 d3dFPos(&d3dF[12]), d3dFLPos(&d3dFL[12]), d3dFRPos(&d3dFR[12]), d3dMiddle(&GetMatrix()[12]), d3dMidBot(&GetMatrix()[12]);
	d3dMidBot.y -= .5f;
	d3dFPos.y += .5f;
	ComputePlane(m_tTesla.m_tPlanes[0], d3dFLPos, d3dFPos, d3dFRPos);
	ComputePlane(m_tTesla.m_tPlanes[1], d3dFLPos, d3dMidBot, d3dMiddle);
	ComputePlane(m_tTesla.m_tPlanes[2], d3dFRPos, d3dMiddle, d3dMidBot);

	m_d3dFL = d3dFL;
	m_d3dFR = d3dFR;
}

bool CTeslaBullet::CheckCollision(IBaseObject* pObj)
{
	if( !GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();

	if( nType == OBJ_BULLET_ENEMY || nType > OBJ_PLAYER && nType <= OBJ_AIFIST || nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || nType == OBJ_BOSSINV || nType == OBJ_BOSSTURRET)
	{
		if(TeslaToSphere(m_tTesla, pObj->GetSphere()))
		{
			//AddToReactVector(pObj);
			CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CTeslaBullet::CollisionResponse(IBaseObject* pObj)
{
	if( pObj->GetID() == OBJ_BULLET_ENEMY )
	{
		pObj->SetAlive(false);
		CGame::GetInstance()->GetOF()->Destroy(pObj);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_TESLAHIT,pObj->GetMatrix());
		return;
	}

	if(pObj->GetID() == OBJ_AICORE)
	{
		if(!((CAICore*)pObj)->GetCanBeHit())
			return;
	}
	pObj->TakeDamage(this);
	if(pObj->GetID() != OBJ_INVERSION)
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_TESLAHIT,pObj->GetMatrix());
}

void CTeslaBullet::TakeDamage(IBaseObject* _pIn)
{
	
}