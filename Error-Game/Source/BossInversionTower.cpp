#include "BossInversionTower.h"
#include "Game.h"
#include "GameplayState.h"

CBossInversionTower::CBossInversionTower(void)
{
	m_bShieldActive = false;

	SetID(OBJ_BOSSINV); 
	SetRadius(INVERSION_RADIUS);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	SetHealth(MAXHP_INVERSION); 
	SetAlive(true);
	SetBeingHit(11);
	SetIsActive(true);
	SetInversionTimer(60.0f);
	m_fInversionAtktimer = 0.0f;
	m_fSpawnTimer = 2.75f;
	m_fMaxRadius = 0.0f;
	m_fAttackTime = 0.0f;
	m_fAttackSpeed = 0.0f;
	m_fHitDuration = 0.0f;
	SetColor(D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
}


CBossInversionTower::~CBossInversionTower(void)
{

}

void CBossInversionTower::Update(float fDelta)
{
	KillCurrFrame();

	float fRadius = m_bShieldActive ? 3.0f : INVERSION_RADIUS;
	SetRadius(fRadius);

	SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	
	if(GetHealth() <= 0.0f)
	{
		D3DXMATRIX transMat1,transMat2,transMat3,transMat4;
		D3DXMatrixTranslation(&transMat1,1.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat2,-2.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat3,1.0f,0.0,1.0);
		D3DXMatrixTranslation(&transMat4,0.0f,0.0,-2.0);
		D3DXMATRIX d3dMat=GetMatrix();
		d3dMat *= transMat1;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);
		d3dMat *= transMat2;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);
		d3dMat *= transMat3;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);
		d3dMat *= transMat4;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,d3dMat);

		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);


		SetAlive(false);
		SetIsActive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
	}

	if(m_bShieldHit)
	{
		m_fHitDuration += fDelta;
		SetColor(COLOR_TRASHBOT_HIT);

		if(HIT_CAP < m_fHitDuration)
		{
			m_bShieldHit = false;
			m_fHitDuration = 0.0f;
			SetColor(COLOR_TRASHBOT);
		}
	}

	if(m_fSpawnTimer <= 0.0f)
	{
		if( m_bShieldActive )
			return;

		m_fInversionAtktimer += fDelta;

		if(m_fInversionAtktimer >= m_fAttackTime - .5f)
			SetBeingHit(10);

		if(m_fInversionAtktimer >= m_fAttackTime)
		{
			m_fInversionAtktimer = 0.0f;
		

			CExplodingBullet* tempBullet = nullptr;
			if(CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_EXP,(IBaseObject**)&tempBullet))
			{
				tempBullet->SetMatrix(GetMatrix());
				tempBullet->SetType(INVERSION_BULLET);
				tempBullet->SetDamage(0.0f);   
				tempBullet->SetMaxRadius(m_fMaxRadius); 
				tempBullet->SetSpeed(m_fAttackSpeed); 

				tempBullet->SetRadius(0.0f);
				tempBullet->SetColor(D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f));
			}

			SetBeingHit(11);
		}
	}
	else
	{
		m_fSpawnTimer -= fDelta;
		D3DXMATRIX tempMat;
		tempMat = this->GetMatrix();
		tempMat[13] += m_fSpawnTimer * 0.01f;
		this->SetMatrix(tempMat);
		if(m_fSpawnTimer <= 0.0f)
			tempMat[13] = 0.0f;
	}
}

void CBossInversionTower::TakeDamage(IBaseObject* _pIn)
{
	if( m_bShieldActive )
		return;

	SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
}

void CBossInversionTower::Render()
{
	IBaseObject::Render();

	if( m_bShieldActive )
	{
		CAssetManager* pAM = CAssetManager::GetInstance();
		D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
		CRenderer* pRenderer = CRenderer::GetInstance();
		IDirect3DDevice9* pD3D = pRenderer->GetDirect3DDevice();

		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		ID3DXEffect* pShader = pAM->GetShader(EFFECT_OBJ_SPHERE);
		unsigned passes(0);
		pShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			pShader->BeginPass(i);

			pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(EFFECT_OBJ_SPHERE));
			pShader->SetMatrix("gViewProjection", &(ViewProj));
			D3DXMATRIX scaleMat, worldMat;
			float scale = GetRadius();
			D3DXMatrixScaling(&scaleMat, scale, scale, scale);
			pShader->SetMatrix("gWorld",&(scaleMat*GetMatrix()));
			if(m_bShieldHit)
			{
				D3DXVECTOR4 color = GetColor();
				color.x += 0.25f;
				color.y += 0.25f;
				color.z += 0.25f;
				color.w += 0.25f;
				pShader->SetFloatArray("gColor", color, 4);
			}
			else
				pShader->SetFloatArray("gColor", GetColor(), 4);
			pShader->CommitChanges();

			pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
			pD3D->SetStreamSource(0, pAM->GetVertBuffer(EFFECT_OBJ_SPHERE), 0, sizeof(tVertex));
			pD3D->SetIndices(pAM->GetIndexBuffer(EFFECT_OBJ_SPHERE));
			pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(EFFECT_OBJ_SPHERE), 0, pAM->GetNumTriangles(EFFECT_OBJ_SPHERE));

			pShader->EndPass();
		}
		pShader->End();
		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}
