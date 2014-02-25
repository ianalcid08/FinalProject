/************************************************************************************
 * Filename:  		SteamrollerBot.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the charging mid-boss.
 ************************************************************************************/
#include "SteamrollerBot.h"
#include "Game.h"
#include "PlayerObject.h"
#include "AIBehaviors.h"
#include "GameplayState.h"


CSteamrollerBot::CSteamrollerBot(void)
{
	SetID(OBJ_STEAM);
	SetRadius(10.0f);
	m_pBehavior = nullptr;
	m_pTargetFINAL = nullptr;
	m_cAI.Initialize(this);
	m_fTime = 0.0f;
	SetRadius(1.75f);
	SetColor(COLOR_AICORE);
	m_fTargetTime = 0.0f;
	m_fSwitchTime = 0.0f;
	m_fDamage = 200.0f;
	m_fStunTime = 2.0f;
	m_nWheelAnim = 0;
	D3DXMatrixIdentity(&m_d3dWheelCurrFrame);
	m_fMaxHP = 0.0f;
	m_bEnraged = false;
	m_fDying = 0.0f;
}

CSteamrollerBot::~CSteamrollerBot(void)
{

}

void CSteamrollerBot::Update(float fDelta)
{
	if(m_fDying)
	{
		m_fDying += fDelta;

		if( m_fDying > GetCurrentAnimation()->m_fDuration )
		{
			CGame::GetInstance()->GetOF()->Destroy(this);
			m_fDying = 0.0f;
			SetIsActive(false);
			return;
		}

		float fAnimTime = GetAnimationTime();
		fAnimTime += fDelta;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), fAnimTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), fAnimTime, PassCurrFrame());
		SetAnimationTime(fAnimTime);
		return;
	}

	if( GetHealth() < 0.0f )
	{
		m_fDying = .00001f;
		SetAlive(false);
		ChangeAnimation(ASTEAM_DEATH);
		return;
	}

	m_fTargetTime = rand()%3+1.5f;

	float fHalfHP = m_fMaxHP * .5f;

	if( !m_bEnraged && GetHealth() < fHalfHP )
	{
		m_bEnraged = true;
		m_pBehavior = Steam::Pose;
	}

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
	float animTime = GetAnimationTime();
	animTime += fDelta;
	animTime = min(animTime, GetCurrentAnimation()->m_fDuration-.001f);

	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);
}

bool CSteamrollerBot::CheckCollision(IBaseObject* obj)
{
	if(obj == this || m_pBehavior == Steam::Idle)
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

void CSteamrollerBot::CollisionResponse(IBaseObject* obj)
{
	int nType = obj->GetID();

	if(nType == OBJ_PLAYER)
	{
		if( m_pBehavior == Steam::Charge )
			m_fStunTime = 0.5f;

		obj->TakeDamage(this);
		Sphere tempSphere = ((CPlayerObject*)obj)->GetSphere();
		D3DXVECTOR3 V =  this->GetSphere().m_Center - tempSphere.m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = Distance/Mag + EPISILON;
		V = V * (X + 0.025f);
		D3DXMATRIX tempMat = obj->GetMatrix();
		D3DXMATRIX tempSetMat = GetMatrix();
		tempMat._41 = tempSetMat._41 - V.x;
		tempMat._43 = tempSetMat._43 - V.z;
		obj->SetMatrix(tempMat);
	}
}

void CSteamrollerBot::TakeDamage(IBaseObject* _pIn)
{
	if( m_pBehavior == Steam::Idle || m_pBehavior == Steam::Pose )
		return;

	if( _pIn->GetID() == OBJ_BULLET_EXP )
	{
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
		return;
	}

	D3DXVECTOR3 d3dMyZ(GetMatrix()._31, GetMatrix()._32, GetMatrix()._33);
	D3DXVECTOR3 d3dIn(_pIn->GetMatrix()._41, _pIn->GetMatrix()._42, _pIn->GetMatrix()._43);
	D3DXVECTOR3 d3dToTarget = d3dIn - D3DXVECTOR3(GetMatrix()._41, GetMatrix()._42, GetMatrix()._43);
	float fDot = D3DXVec3Dot(&d3dMyZ, &d3dToTarget);

	if( _pIn->GetID() == OBJ_BULLET_BEAM)
	{
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage()*.75f);
		SetWasHit(true);
	}
	else if( fDot < 0.0f  || _pIn->GetID() == OBJ_BULLET_TESLA)
	{
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
		SetWasHit(true);
	}
}

void CSteamrollerBot::Render()
{
	CRenderer* pRenderer = CRenderer::GetInstance();
	pRenderer->Render(this);
	float randomNum = ((rand()/(float)RAND_MAX));

	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	IDirect3DDevice9* pD3D = pRenderer->GetDirect3DDevice();
	if( GetBeingHit() == 2 || GetBeingHit() == 9)
	{
		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	}

	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(OBJ_STEAM_WHEEL);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);
		pShader->SetMatrixArray("gBones", &m_d3dWheelCurrFrame, 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_STEAM_WHEEL));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&(GetMatrix()));
		pShader->SetInt("AmIHit",GetBeingHit());
		pShader->SetFloat("randomNum",randomNum);

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		pD3D->SetStreamSource(0, pAM->GetVertBuffer(OBJ_STEAM_WHEEL), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(OBJ_STEAM_WHEEL));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_STEAM_WHEEL), 0, pAM->GetNumTriangles(OBJ_STEAM_WHEEL));

		pShader->EndPass();
	}
	pShader->End();

	if( GetBeingHit() == 2 || GetBeingHit() == 9)
	{
		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	}

#if _DEBUG
	pRenderer->RenderSphere(GetMatrix(), ViewProj, GetColor(), GetRadius() );
#endif
}