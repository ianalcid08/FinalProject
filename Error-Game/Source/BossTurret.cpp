#include "BossTurret.h"
#include "Game.h"
#include "GameplayState.h"

CBossTurret::CBossTurret(void)
{
	SetID(OBJ_BOSSTURRET);
	SetDamage(TURRET_DMG);
	SetRadius(TURRET_RADIUS);
	SetColor(COLOR_TRASHBOT);
	SetHealth(TURRET_HP);
	SetWasHit(false);
	SetBeingHit(0);
	m_fFireTimer = 0.0f;
	m_fRange = 5.0f;
	m_cTurretCap.m_Radius = TURRET_RADIUS;
	m_fSpawnTimer = 2.0f;
	m_fShotTimer = 0.0f;
	m_pLaser = NULL;
	m_fKillTimer = 60.0f;
	m_fHold = 0.0f;
	m_fHoldTimer = 0.0f;
}


CBossTurret::~CBossTurret(void)
{
}

void CBossTurret::Update(float fDelta)
{

	if(GetHealth() <= 0.0f)
	{
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,GetMatrix());
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		m_fHoldTimer = 0.0f;
		SetAlive(false);
		SetIsActive(false);
		SetBeingHit(0);
		SetWasHit(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
		//if(m_pLaser)
		//	m_pLaser->Reset();
	}

	m_fKillTimer -= fDelta;

	//if(m_fKillTimer < 0.0f)
	//{
	//	Frustum tempFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();
	//
	//	if(SphereToFrustumBehind(GetSphere(),tempFrustum))
	//	{
	//		//cout << "Laser Destroyed" << endl;
	//		CGame::GetInstance()->GetOF()->Destroy(this);
	//		return;
	//	}
	//}

	if(m_fSpawnTimer <= 0.0f)
	{
		m_fHoldTimer += fDelta;

		if( m_fHoldTimer > m_fHold )
		{

			// test
			D3DXMATRIX t = GetMatrix();
			t[13] = 0.0f;
			SetMatrix(t);

			//if(m_pLaser == NULL)
			//	m_pLaser = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_LASER,GetMatrix());
			D3DXMATRIX newRotation = GetMatrix();
			D3DXMATRIX newHolderMatrix = GetMatrix();


			D3DXMatrixRotationY(&newRotation,D3DXToRadian(180.0f * (fDelta*0.25f)));

			newHolderMatrix = newRotation * newHolderMatrix;
			SetMatrix(newHolderMatrix);
			// Shooting logic
			Sphere tBulletSphere = GetSphere();
			m_cTurretCap.m_Segment.m_Start = tBulletSphere.m_Center;

			if( !m_d3dNewImpact.x && !m_d3dNewImpact.z )
			{
				m_sNextSphere = tBulletSphere;

				D3DXMATRIX transNext;//,thisNext;

				m_d3dNextMat = newHolderMatrix;

				//D3DXVECTOR3 d3dDirNext = GetMatrix() * 5.0f;
				D3DXMatrixTranslation(&transNext,0.0f,0.0f,40.0f);
				m_d3dNextMat = transNext * m_d3dNextMat;

				m_sNextSphere.m_Center = *(D3DXVECTOR3*)&m_d3dNextMat[12];

				//  MAKE A CAPSULE AND DO COLLISION
				m_cTurretCap.m_Segment.m_End = m_sNextSphere.m_Center;

				m_d3dNewImpact.y = 0.0f;
				m_d3dNewImpact.x = 0.0f;
				m_d3dNewImpact.z = 0.0f;
				m_fLength = 40.0f;
			}
			else
			{
				m_d3dNewImpact.y = 0.0f;
				m_cTurretCap.m_Segment.m_End = m_d3dNewImpact;
				m_d3dNewImpact.x = 0.0f;
				m_d3dNewImpact.z = 0.0f;
			}
		}
	}
	else
	{
		m_fSpawnTimer -= fDelta;
		D3DXMATRIX spawnUp,tempCurr;
		//D3DXMatrixTranslation(&spawnUp,0.0f,-(m_fSpawnTimer * 0.1f),0.0f);
		tempCurr = this->GetMatrix();
		tempCurr[13] += m_fSpawnTimer * 0.01f;
		this->SetMatrix(tempCurr);
		
	}
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
	/*D3DXMATRIX viewproj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	
	m_pRenderer->DrawBeamCylinder(GetMatrix(),viewproj,GetColor(),tBulletSphere.m_Radius,EnemyBeamCylinderMesh);*/

	//if(m_pLaser)
	//{
	//	m_pLaser->SetMatrix(GetMatrix());
	//}

	KillCurrFrame();
	//float animTime = GetAnimationTime();
	//animTime += fDelta;
	//CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	//CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	//SetAnimationTime(animTime);

	
}

bool CBossTurret::CheckCollision(IBaseObject* pObj)
{
	if(!GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();
	if(nType == OBJ_PLAYER)
	{
		if(CapsuleToSphere(m_cTurretCap,pObj->GetSphere()))
		{
			pObj->TakeDamage(this);
			return true;
		}

		if(SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			pObj->AddToReactVector(this);
			return true;
		}
	}

	if( nType == OBJ_BOSSINV )
	{
		float fT = 0.0f;
		D3DXVECTOR3 d3dImpact;
		if(CapsuleToSphere(m_cTurretCap,pObj->GetSphere()))
		{
			if( IntersectRaySphere(&GetMatrix()[12], &GetMatrix()[8], &pObj->GetMatrix()[12], pObj->GetRadius(), fT, d3dImpact) )
			{
				m_d3dNewImpact = d3dImpact;
				m_fLength = fT;
			}
		}
	}

	return false;
}

void CBossTurret::Render()
{
	IBaseObject::Render();

	//D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	//D3DXMATRIX d3dSphere;
	//D3DXMatrixTranslation(&d3dSphere, m_cTurretCap.m_Segment.m_End.x, m_cTurretCap.m_Segment.m_End.y, m_cTurretCap.m_Segment.m_End.z);
	//CRenderer::GetInstance()->RenderSphere(d3dSphere, ViewProj, D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),.25f);

	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	CRenderer* pRenderer = CRenderer::GetInstance();
	IDirect3DDevice9* pD3D = pRenderer->GetDirect3DDevice();

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	ID3DXEffect* pShader = pAM->GetShader(EFFECT_OBJ_CYLINDER);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(EFFECT_OBJ_CYLINDER));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		D3DXMATRIX scaleMat;
		D3DXMatrixScaling(&scaleMat, GetRadius(), 1.0f, m_fLength);

		D3DXMATRIX transUp = GetMatrix();
		transUp[13] = BULLET_HEIGHT_OFFSET;
		pShader->SetMatrix("gWorld",&(scaleMat*transUp));
		pShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), 4);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		pD3D->SetStreamSource(0, pAM->GetVertBuffer(EFFECT_OBJ_CYLINDER), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(EFFECT_OBJ_CYLINDER));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(EFFECT_OBJ_CYLINDER), 0, pAM->GetNumTriangles(EFFECT_OBJ_CYLINDER));

		pShader->EndPass();
	}
	pShader->End();
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}

void CBossTurret::TakeDamage(IBaseObject* _pIn)
{
	SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
	SetWasHit(true);
}