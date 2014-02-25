/************************************************************************************
 * Filename:  		BeamBullet.h
 * Date:      		03/05/2013
 * Mod. Date: 		03/06/2013
 * Mod. Initials:	IA
 * Author:    		Joe Thomas
 * Purpose:   		The bullets fired by the player and some enemies
 ************************************************************************************/
#include "BeamBullet.h"
#include "Definitions.h"
#include "EnemyObject.h"
#include "PlayerObject.h"
#include "Game.h"
#include "Camera.h"
#include "ObjectFactory.h"
#include "GameplayState.h"

CBeamBullet::CBeamBullet(void)
{
	m_nType = BEAM;
	SetID(OBJ_BULLET_BEAM);
	SetAlive(true);
	SetIsActive(true);
	SetHealth(1);
	m_fLifeTime = 0.5f;
	SetRadius(BEAM_RADIUS);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
}

CBeamBullet::~CBeamBullet(void)
{
}

void CBeamBullet::Update(float fDelta)
{
	if(!GetAlive())
		return;
	
	m_fLifeTime -= fDelta;
	if(m_fLifeTime < 0.0f)
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
		SetAlive(false);
		SetIsActive(false);
		return;
	}

	SetDamage(DMG_BEAM * fDelta);
}

void CBeamBullet::CreateCapsule()
{
	Sphere tStartSphere = GetSphere();
	m_sNextSphere = tStartSphere;
	D3DXMATRIX d3dTranslateNext, d3dResultNext = GetMatrix();
	D3DXVECTOR3 d3dDirNext = GetVelocity() * 40.0f;
	D3DXMatrixTranslation(&d3dTranslateNext, d3dDirNext.x, 0.0f, d3dDirNext.z);
	d3dResultNext *= d3dTranslateNext;
	m_sNextSphere.m_Center = *(D3DXVECTOR3*)&d3dResultNext[12];
	m_fLength = D3DXVec3Length(&d3dDirNext);
	
	m_beamCapsule.m_Radius = BEAM_RADIUS;
	m_beamCapsule.m_Segment.m_Start = tStartSphere.m_Center;
	m_beamCapsule.m_Segment.m_End = m_sNextSphere.m_Center;
}

bool CBeamBullet::CheckCollision(IBaseObject* pObj)
{
	if( !GetAlive() || !pObj->GetAlive())
		return false;
	int nType = pObj->GetID();

	if( nType > OBJ_PLAYER && nType <= OBJ_AIFIST || nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || nType == OBJ_BOSSINV || nType == OBJ_BOSSTURRET)
	{
		Frustum tfrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(CapsuleToSphere(m_beamCapsule, pObj->GetSphere()) && SphereToFrustumBehind(pObj->GetSphere(), tfrustum) == 0)
		{
			CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CBeamBullet::CollisionResponse(IBaseObject* pObj)
{
	if(pObj->GetID() == OBJ_AICORE)
	{
		if(!((CAICore*)pObj)->GetCanBeHit())
			return;
	}
	pObj->TakeDamage(this);
	CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_PULSEHIT,pObj->GetMatrix());
}

void CBeamBullet::Render()
{
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
		pShader->SetFloatArray("gColor", GetColor(), 4);
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
