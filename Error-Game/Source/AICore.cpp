/************************************************************************************
 * Filename:  		AICore.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the shooting final boss.
 ************************************************************************************/
#include "AICore.h"
#include "AIFist.h"
#include "Game.h"
#include "PlayerObject.h"
#include "AIBehaviors.h"
#include "GameplayState.h"
#include "ResetState.h"
#include "Effect.h"

CAICore::CAICore(void)
{
	SetID(OBJ_AICORE);
	m_cAI.Initialize(this);
	m_pLeftFist = nullptr;
	m_pRightFist = nullptr;
	m_pBehavior = nullptr;
	m_fFireTime = 0.0f;
	SetRadius(2.5f);
	SetColor(COLOR_AICORE);
	m_bCanBeHit = false;
	m_fFireRate = 0.0f;
	m_fTime = 0.0f;
	m_nBurstCount = 0;
	m_pSmokeEffect = nullptr;
	m_pBurnEffect = nullptr;
	m_pFireEffect = nullptr;
	m_pMoveBehavior = nullptr;
}


CAICore::~CAICore(void)
{
}

void CAICore::Initialize()
{
	D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();

	D3DXMATRIX d3dPos;
	D3DXMatrixTranslation(&d3dCamera, d3dCamera._41, 10.0f, d3dCamera._43);
	D3DXMatrixIdentity(&d3dPos);
	d3dPos._43 = 11.0f;

	SetMatrix(d3dPos*d3dCamera);
	CObjectFactory* pOF = CGame::GetInstance()->GetOF();

	if( pOF->Create(OBJ_AIFIST, (IBaseObject**)&m_pLeftFist) )
	{
		D3DXMATRIX d3dTest = m_pLeftFist->GetMatrix(), d3dRot;
		d3dTest._41 = GetMatrix()._41 - 3.5f;
		d3dTest._42 = 10.2f;
		d3dTest._43 = GetMatrix()._43 - 4.0f;
		m_pLeftFist->SetMatrix(d3dTest);

		m_pLeftFist->m_pBehavior = AIFist::Enter;

		d3dTest._42 = .2f;
		m_pLeftFist->m_d3dHome = *(D3DXVECTOR3*)&d3dTest[12];
	}

	if( pOF->Create(OBJ_AIFIST, (IBaseObject**)&m_pRightFist) )
	{
		D3DXMATRIX d3dTest = m_pRightFist->GetMatrix();
		d3dTest._41 = GetMatrix()._41 + 3.5f;
		d3dTest._42 = 10.2f;
		d3dTest._43 = GetMatrix()._43 - 4.0f;
		m_pRightFist->SetMatrix(d3dTest);

		m_pRightFist->m_pBehavior = AIFist::Enter;

		d3dTest._42 = .2f;
		m_pRightFist->m_d3dHome = *(D3DXVECTOR3*)&d3dTest[12];
	}

	m_pBehavior = AICore::Enter;
	m_bCanBeHit = false;
	m_fFireRate = CORE_FIRE_SPEED;
	m_fTime = 0.0f;
	m_fBurstTime = 0.0f;
	m_nBurstCount = 0;
	m_pMoveBehavior = nullptr;
	SetVelocity(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));



	for( int i = 0; i < 3; ++i )
		m_d3dBurstLoc[i] = *(D3DXVECTOR3*)&d3dCamera[12];

	m_d3dBurstLoc[0].y = 0.0f;
	m_d3dBurstLoc[1].x -= 5.0f; m_d3dBurstLoc[1].y = 0.0f; m_d3dBurstLoc[1].z += 3.0f;
	m_d3dBurstLoc[2].x += 5.0f; m_d3dBurstLoc[2].y = 0.0f; m_d3dBurstLoc[2].z += 3.0f;

	CGameplayState::GetInstance()->GetHUD()->EnableBoss(this);

	D3DXMATRIX d3dThis = GetMatrix(), d3dRot;
	D3DXMatrixRotationY(&d3dRot, D3DXToRadian(180.0f));
	d3dThis = d3dRot * d3dThis;
	SetMatrix(d3dThis);
}

void CAICore::Update(float fDelta)
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
		CGame::GetInstance()->GetOF()->Destroy(this);
		CResetState::GetInstance()->SetVictory(true);
		CGameplayState::GetInstance()->GetHUD()->DisableBoss();
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,GetMatrix());		
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

	if(GetWasHit() == true)
	{
		float temp = GetHitDuration();
		temp += fDelta;
		SetHitDuration(temp);
		///////////////////
		SetBeingHit(1);
		///////////////////
		//tSphere.m_d3dColor = COLOR_TRASHBOT_HIT;
		//SetSphere(tSphere);
		SetColor(COLOR_TRASHBOT_HIT);

		if(HIT_CAP < temp)
		{
			SetWasHit(false);
			SetHitDuration(0.0f);
			//tSphere.m_d3dColor = COLOR_TRASHBOT;
			SetColor(COLOR_TRASHBOT);
			///////////////
			SetBeingHit(0);
			///////////////
			//SetSphere(tSphere);

		}
	}

	KillCurrFrame();
	//float animTime = GetAnimationTime();
	//animTime += fDelta;
	//CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	//CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	//SetAnimationTime(animTime);


	if( !m_pLeftFist->GetAlive() && !m_pRightFist->GetAlive() )
	{
		m_bCanBeHit = true;
		m_fFireRate = CORE_FIRE_SPEED * .5f;
		m_pMoveBehavior = AICore::Sway;
	}

	m_cAI.Update(fDelta);
}

bool CAICore::CheckCollision(IBaseObject* obj)
{
	if( obj == this )
		return false;

	if(!obj->GetAlive())
		return false;

	if(!GetAlive())
		return false;

	int nType = obj->GetID();

	//if( nType >= OBJ_TRASH && nType <= OBJ_AIFIST )
	//{
	//	if( SphereToSphere(GetSphere(), ((CEnemyObject*)obj)->GetSphere()))
	//	{
	//		obj->CollisionResponse(this);
	//		return true;
	//	}
	//}
	//
	//if( nType == OBJ_PLAYER)
	//{
	//	if( SphereToSphere(GetSphere(), ((CPlayerObject*)obj)->GetSphere()) )
	//	{
	//		obj->CollisionResponse(this);
	//		return true;
	//	}
	//}


	return false;
}

void CAICore::CollisionResponse(IBaseObject* obj)
{
}

void CAICore::TakeDamage(IBaseObject* _pIn)
{
	if( m_pBehavior == AICore::Enter || !m_bCanBeHit )
	{
		//std::cout << "INVULNERABLE" << endl;
		return;
	}

	SetWasHit(true);
	SetHealth(GetHealth()-((CBullet*)_pIn)->GetDamage());
	//std::cout << "CORE: " << GetHealth() << endl;
}

void CAICore::Render()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	ID3DXEffect* pShader = nullptr;
	CRenderer* pRenderer = CRenderer::GetInstance();
	IDirect3DDevice9* D3D_Device = pRenderer->GetDirect3DDevice();
	
	D3DXMATRIX transShield;
	D3DXMatrixTranslation(&transShield, 0.0f, 0.75f, 0.0f);
	if(!m_bCanBeHit)
		pRenderer->RenderSphere(transShield*GetMatrix(), ViewProj, GetColor(), GetRadius() );

	D3D_Device->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	pShader = pAM->GetShader(OBJ_AICORE);
	unsigned passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		// Begin with Monitors
		pShader->SetMatrixArray("gBones", PassCurrFrame(), 15);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_AI_MONITORS));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&(GetMatrix()));
		pShader->SetInt("AmIHit",(GetBeingHit()));

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(OBJ_AI_MONITORS), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(OBJ_AI_MONITORS));
		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_AI_MONITORS), 0, pAM->GetNumTriangles(OBJ_AI_MONITORS));

		// AI Head
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_AI_HEAD));
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(OBJ_AI_HEAD), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(OBJ_AI_HEAD));

		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_AI_HEAD), 0, pAM->GetNumTriangles(OBJ_AI_HEAD));

		// AI Arms
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_AI_ARMS));
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(OBJ_AI_ARMS), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(OBJ_AI_ARMS));

		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_AI_ARMS), 0, pAM->GetNumTriangles(OBJ_AI_ARMS));

		pShader->EndPass();
	}
	pShader->End();
	pRenderer->Render(this);
}
