#include "ExplodingBullet.h"
#include "Game.h"
#include "GameplayState.h"


CExplodingBullet::CExplodingBullet(void)
{
	SetID(OBJ_BULLET_EXP);
	m_fSpeed = 0.0f;
	m_fMaxRadius = 0.0f;
	SetRadius(0.0f);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
}


CExplodingBullet::~CExplodingBullet(void)
{
}


void CExplodingBullet::Update(float fDelta)
{
	float fRadius = GetSphere().m_Radius;
	fRadius += fDelta * m_fSpeed;

	if( fRadius > m_fMaxRadius )
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
		SetAlive(false);
		m_lEnemies.clear();
		return;
	}

	SetRadius(fRadius);
}

bool CExplodingBullet::CheckCollision(IBaseObject* pObj)
{
	if( !GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();

	switch(m_nType)
	{
	case SPECIALBULLET:
		{
			if( nType == OBJ_BULLET_ENEMY || nType > OBJ_PLAYER && nType <= OBJ_AIFIST || 
				nType >= OBJ_VAT && nType <= OBJ_BARREL || nType == OBJ_INVERSION || nType == OBJ_BOSSINV || nType == OBJ_BOSSTURRET)
			{
				std::list<IBaseObject*>::iterator iter;

				for( iter = m_lEnemies.begin(); iter != m_lEnemies.end(); ++iter )
				{
					if( (*iter) == pObj )
						return false;
				}

				if( SphereToSphere(GetSphere(), pObj->GetSphere()))
				{
					m_lEnemies.push_back(pObj);
					CollisionResponse(pObj);
					CEffect* pEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SPECIALHIT,pObj->GetMatrix());
					if(pEffect)
						pEffect->SetColors((D3DXCOLOR)GetColor());
					return true;
				}
			}
		}
		break;
	case SMASHBULLET:
		{
			if( nType == OBJ_PLAYER || nType == OBJ_TRASH )
			{
				if( SphereToSphere(GetSphere(), pObj->GetSphere()))
				{
					CollisionResponse(pObj);
					return true;
				}
			}
		}
		break;
	case BARRELBULLET:
		{
			if( nType == OBJ_PLAYER || nType >= OBJ_TRASH && nType <= OBJ_STEAM)
			{
				if( SphereToSphere(GetSphere(), pObj->GetSphere()))
				{
					CollisionResponse(pObj);
					return true;
				}
			}
			else if( nType == OBJ_BARREL)
			{
				if( SphereToSphere(GetSphere(), pObj->GetSphere()))
				{
					pObj->CollisionResponse(this);
					return true;
				}
			}
		}
		break;
	case INVERSION_BULLET:
		{
			if(nType == OBJ_PLAYER)
			{
				if(SphereToSphere(GetSphere(), pObj->GetSphere()))
				{
					CollisionResponse(pObj);
					return true;
				}
			}
		}
	}
	return false;
}

void CExplodingBullet::CollisionResponse(IBaseObject* pObj)
{
	if( m_nType == SPECIALBULLET && pObj->GetID() == OBJ_BULLET_ENEMY )
	{
		pObj->SetAlive(false);
		CGame::GetInstance()->GetOF()->Destroy(pObj);
		return;
	}

	if(m_nType == INVERSION_BULLET && pObj->GetID() == OBJ_PLAYER)
	{
		((CPlayerObject*)pObj)->SetInverted(true);
		((CPlayerObject*)pObj)->SetInvertTimer(1.5f);
	}

	pObj->TakeDamage(this);
}

void CExplodingBullet::TakeDamage(IBaseObject* _pIn)
{

}

void CExplodingBullet::Render()
{
	if(GetType() == SMASHBULLET)
		return;
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