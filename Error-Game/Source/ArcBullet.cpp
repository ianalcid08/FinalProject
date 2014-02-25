#include "ArcBullet.h"
#include "Game.h"
#include "GameplayState.h"

CArcBullet::CArcBullet(void)
{
	SetID(OBJ_BULLET_ARC);
	SetRadius(BULLET_STD_SIZE*2.0f);
	SetColor(COLOR_TRASHBOT);
	m_nType = ARC;

	m_d3dShrapnelDir[0] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_d3dShrapnelDir[1] = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	m_d3dShrapnelDir[2] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dShrapnelDir[3] = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	m_d3dShrapnelDir[4] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_d3dShrapnelDir[5] = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	m_d3dShrapnelDir[6] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_d3dShrapnelDir[7] = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);

	m_bIsRising = true;
	m_nFrame = 3;
	m_bIsMolten = false;
}

CArcBullet::~CArcBullet(void)
{
}

void CArcBullet::Update(float fDelta)
{
	if( !GetAlive() )
		return;


	D3DXVECTOR3 d3dMyPos(&GetMatrix()[12]), d3dTT;
	d3dTT = m_tTargetSphere.m_Center - d3dMyPos;
	D3DXVec3Normalize(&m_d3dDir, &d3dTT);

	++m_nFrame;

	if( GetMatrix()._42 > CEILING )
	{
		m_bIsRising = false;
		m_d3dPeak = d3dMyPos;
	}

	float fGravity = GRAVITY;

	if( !m_bIsRising )
	{
		float fTotal = D3DXVec3LengthSq(&(m_tTargetSphere.m_Center-m_d3dPeak));
		float fCurrent = D3DXVec3LengthSq(&(m_tTargetSphere.m_Center-d3dMyPos));
		float fRatio = fCurrent/fTotal;
		fRatio = 1.0f - fRatio;
		if( fRatio < .2f )
			fRatio = .2f;

		fGravity *= fRatio;
	}

	D3DXMATRIX d3dTranslate, d3dThis = GetMatrix();
	m_d3dDir = fDelta * ARCBULLET_SPEED * m_d3dDir;
	float fYDir = m_bIsRising ? ANITIGRAVITY*fDelta : fGravity*fDelta;
	m_d3dDir.y = fYDir;

	D3DXMatrixTranslation(&d3dTranslate, m_d3dDir.x, m_d3dDir.y, m_d3dDir.z);
	d3dThis *= d3dTranslate;
	SetMatrix(d3dThis);

	if( GetMatrix()._42 <= 0.05f )
	{
		CObjectFactory* pOF = CGame::GetInstance()->GetOF();

		D3DXMATRIX d3dThis;
		D3DXMatrixIdentity(&d3dThis);
		*(D3DXVECTOR3*)&d3dThis[12] = *(D3DXVECTOR3*)&GetMatrix()[12];
		d3dThis._42 = 0.0f;
		SetMatrix(d3dThis);

		if( m_bIsMolten )
		{
			CMoltenPool* pMoltenPool = nullptr;
			if(pOF->Create(OBJ_POOL, (IBaseObject**)&pMoltenPool))
			{
				pMoltenPool->SetMatrix(d3dThis);
				pMoltenPool->SetOriginalMatrix(d3dThis);
				pMoltenPool->SetRadius(1.0f);
				pMoltenPool->SetScale(1.25f);
				pMoltenPool->SetScaleMod(1.4f);
				pMoltenPool->SetScaleTimer(1.0f);
				pMoltenPool->SetPoolStage(1);
				pMoltenPool->SetScalingMod(0.0f);
				pMoltenPool->SetDroppedID(1);
			}
		}

		CBullet* pBullet = nullptr;
		for( int i = 0; i < 8; ++i )
		{
			if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ENEMY, (IBaseObject**)&pBullet) )
			{
				pBullet->SetMatrix(d3dThis);
				pBullet->SetType(ENEMYBULLET);
				pBullet->SetDamage(20.0f);
				pBullet->SetVelocity( m_d3dShrapnelDir[i] );
				pBullet->SetColor(COLOR_ENEMY_BULLET);
				Sphere tSPSphere = pBullet->GetSphere();
				pBullet->SetType(ARC);
			}
		}

		pOF->Destroy(this);
	}
}

bool CArcBullet::CheckCollision(IBaseObject* pObj)
{
	if( !GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();

	if( nType == OBJ_PLAYER )
	{
		if( SphereToSphere(GetSphere(), pObj->GetSphere()) )
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CArcBullet::CollisionResponse(IBaseObject* pObj)
{
	pObj->TakeDamage(this);
}

void CArcBullet::TakeDamage(IBaseObject* _pIn)
{

}

void CArcBullet::Render()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	CRenderer* pRenderer = CRenderer::GetInstance();
	IDirect3DDevice9* pD3D = pRenderer->GetDirect3DDevice();

	pRenderer->RenderShadow(this);
	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(EFFECT_OBJ_SPHERE);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(EFFECT_OBJ_SPHERE));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		D3DXMATRIX scaleMat;
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
}