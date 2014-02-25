/************************************************************************************
 * Filename:  		DeathPit.h
 * Date:      		04/17/2013
 * Mod. Date: 		04/17/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		This is for the death pit object
 ************************************************************************************/

#include "DeathPit.h"
#include "Definitions.h"
#include "AssetManager.h"
#include "Game.h"
#include "GameplayState.h"
#include "TrashBot.h"
#include "AIBehaviors.h"

enum EANIM {AGATE_OPEN, AGATE_CLOSE};

CDeathPit::CDeathPit(void)
{
	SetID(OBJ_PIT);
	SetAlive(true);
	m_fFallingTimer = 0.0f;
	m_bIsFalling = false;
	m_fDelayTimer = 120.0f;
	m_fTrapCloseTimer = 5.0f;
	m_fTrapOpenTimer = 10.0f;
	m_bTrapOpen = false;
	
}
CDeathPit::~CDeathPit(void)
{

}

void CDeathPit::Update(float fDelta)
{
	m_fFallingTimer = fDelta;
	m_fDelayTimer -= fDelta;

	if(m_bAnimate)
	{
		float animTime = GetAnimationTime();
		animTime += fDelta;
		if(animTime < GetCurrentAnimation()->m_fDuration)
		{
			CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
			CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
			SetAnimationTime(animTime);
		}
	}

	// If the trap is open 
	if(m_bTrapOpen)
	{
		m_fTrapOpenTimer -= fDelta;
		if(m_fTrapOpenTimer < 0.5f)
		{
			m_bTrapOpen = !m_bTrapOpen;
#ifdef _DEBUG
			cout << "Trap is now closed" << endl;
#endif
			m_fTrapOpenTimer = 2.0f;
			ChangeAnimation(1);
		}
	}
	else // if trap closed
	{
		m_fTrapCloseTimer -= fDelta;
		if(m_fTrapCloseTimer < 0.5f)
		{
			m_bTrapOpen = !m_bTrapOpen;
#ifdef _DEBUG
			cout << "Trap is now open" << endl;
#endif
			m_fTrapCloseTimer = 6.0f;
			ChangeAnimation(0);
			m_bAnimate = true;
		}
	}
	
	if(m_fDelayTimer <= 0)
	{
		Frustum tempFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(),tempFrustum))
		{
			cout << "Deathpit destroyed by frustum" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}
}
void CDeathPit::Render()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	IDirect3DDevice9* pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();

	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(GetID());
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		// Render Gate
		pShader->SetMatrixArray("gBones", PassCurrFrame(), 2);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_GATE));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&(GetMatrix()));
		pShader->SetInt("AmIHit",(0));
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		pD3D->SetStreamSource(0, pAM->GetVertBuffer(OBJ_GATE), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(OBJ_GATE));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_GATE), 0, pAM->GetNumTriangles(OBJ_GATE));

		// Render Pit
		pShader->SetMatrixArray("gBones", &CRenderer::GetInstance()->KillAnimation(), 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(GetID()));
		pShader->CommitChanges();

		pD3D->SetStreamSource(0, pAM->GetVertBuffer(GetID()), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(GetID()));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(GetID()), 0, pAM->GetNumTriangles(GetID()));

		pShader->EndPass();
	}
	pShader->End();
}

bool CDeathPit::CheckCollision(IBaseObject* pObj)
{
	return false;
}

void CDeathPit::CollisionResponse(IBaseObject* pObj)
{
	int type = pObj->GetID();
	switch(type)
	{
	case OBJ_PLAYER:
		{
			if(!((CPlayerObject*)pObj)->GetIsFalling() && !((CPlayerObject*)pObj)->GetTransition())
			{
#ifdef _DEBUG
			cout << "Killed Player" << endl;
#endif
				Movement::LookAt(pObj, *(D3DXVECTOR3*)&GetMatrix()[12]);
				((CPlayerObject*)pObj)->SetIsFalling(true);
			}
		}
		break;
	case OBJ_TRASH:
		{
			if(!((CTrashBot*)pObj)->GetIsFalling())
			{
#ifdef _DEBUG
			cout << "Killed TrashBot" << endl;
#endif
				Movement::LookAt(pObj, *(D3DXVECTOR3*)&GetMatrix()[12]);
				((CTrashBot*)pObj)->SetIsFalling(true);
			}
		}
		break;
	case OBJ_CRATE:
		{
			if(!((CCrate*)pObj)->GetIsFalling())
			{
#ifdef _DEBUG
				cout << "Killed Crate" << endl;
#endif
				Movement::LookAt(pObj, *(D3DXVECTOR3*)&GetMatrix()[12]);
				((CCrate*)pObj)->SetIsFalling(true);
			}
		}
		break;
	case OBJ_BARREL:
		{
			if(!((CBarrel*)pObj)->GetIsFalling())
			{
#ifdef _DEBUG
				cout << "Killed Barrel" << endl;
#endif
				Movement::LookAt(pObj, *(D3DXVECTOR3*)&GetMatrix()[12]);
				((CBarrel*)pObj)->SetIsFalling(true);
			}
		}
		break;
	}
}