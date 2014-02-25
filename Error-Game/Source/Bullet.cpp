/************************************************************************************
 * Filename:  		Bullet.cpp
 * Date:      		02/11/2013
 * Mod. Date: 		02/22/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		The bullets fired by the player and some enemies
 ************************************************************************************/

#include "Bullet.h"
#include "Definitions.h"
#include "EnemyObject.h"
#include "PlayerObject.h"
#include "Game.h"
#include "Camera.h"
#include "ObjectFactory.h"
#include "GameplayState.h"
#include "Crate.h"

#define PULSE_INTERVAL_MOD 25

CBullet::CBullet(void)
{
	m_nType = STANDARD;
}


CBullet::~CBullet(void)
{
}

void CBullet::Update(float fDelta)
{
	if( !GetAlive() )
		return;

	D3DXMATRIX d3dTranslate, d3dThis = GetMatrix();
	D3DXVECTOR3 d3dDir = GetVelocity();

	float fSpeed = BULLET_SPEED;
	if( m_nType == ENEMYBULLET )
		fSpeed = PBOT_BULLET_SPEED;
	else if( m_nType == ARC )
		fSpeed = ARCBULLET_SHRAPSPEED;

	d3dDir = fDelta * fSpeed * d3dDir;
	D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, d3dDir.y, d3dDir.z);
	d3dThis *= d3dTranslate;
	SetMatrix(d3dThis);

	m_fDuration += fDelta;

	Frustum test = CGameplayState::GetInstance()->GetCamera()->GetFrustum();
	
	// If bullets collide with frustum, recycle
	if( SphereToFrustum(GetSphere(), test) )
	{
		SetAlive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
	}
}
	
bool CBullet::CheckCollision(IBaseObject* pObj)
{
	if( !GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();

	if( nType >= OBJ_PLAYER && nType <= OBJ_AIFIST || nType >= OBJ_VAT && nType <= OBJ_BARREL
		|| nType == OBJ_INVERSION || nType == OBJ_BOSSINV || nType == OBJ_BOSSTURRET)
	{
		if( SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}	
	return false;
}

void CBullet::CollisionResponse(IBaseObject* pObj)
{
	pObj->TakeDamage(this);
	int nType = GetID();

	if( nType == OBJ_BULLET_PLAYER )
	{
		if(m_nType == STANDARD || m_nType == GATLING)
		{
			SetAlive(false);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
	}
	else if( nType == OBJ_BULLET_ENEMY )
	{
		if(GetType() == ENEMYBULLET)
		{
			SetAlive(false);
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
	}
	if(pObj->GetID() == OBJ_AICORE)
	{
		if(!((CAICore*)pObj)->GetCanBeHit())
			return;
	}
	if(pObj->GetID() != OBJ_SMASH && pObj->GetID() != OBJ_INVERSION)
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_BULLETHIT,GetMatrix());
}

void CBullet::Render()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	CRenderer* pRenderer = CRenderer::GetInstance();
	IDirect3DDevice9* pD3D = pRenderer->GetDirect3DDevice();

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(EFFECT_OBJ_SPHERE);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		IDirect3DTexture9* pTexture = nullptr;
		D3DXMATRIX scaleMat, rotMat, result;
		float scale = GetRadius();
		switch(m_nType)
		{
		case GATLING:
			{
				pTexture = pAM->LoadTexture("Assets/Textures/Weapon_Projectile_Gatling_Gun.png");
				D3DXMatrixScaling(&scaleMat, scale*0.4f, scale, scale*1.5f);
				result = scaleMat*GetMatrix();
			}
			break;
		case ENEMYBULLET:
			{
				scale = scale  * (sin(m_fDuration*PULSE_INTERVAL_MOD) + 2.0f) * 0.5f;
			}
		default:
			{
				pTexture = pAM->LoadTexture("Assets/Textures/Lounge_Tigers_Weapon_Projectile_Defense_Laser.png");
				D3DXMatrixScaling(&scaleMat, scale, scale, scale);
				D3DXMatrixRotationY(&rotMat, m_fDuration*5);
				result = rotMat * GetMatrix();
				result = scaleMat*result;
			}
			break;
		}
		result[13] = BULLET_HEIGHT_OFFSET;
		pShader->SetTexture("gDiffuseTexture", pTexture);
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&result);
		pShader->SetFloatArray("gColor", GetColor(), 4);
		pShader->SetFloat("gTime", m_fDuration);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		pD3D->SetStreamSource(0, pAM->GetVertBuffer(EFFECT_OBJ_SPHERE), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(EFFECT_OBJ_SPHERE));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(EFFECT_OBJ_SPHERE), 0, pAM->GetNumTriangles(EFFECT_OBJ_SPHERE));

		pShader->EndPass();
	}
	pShader->End();

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}