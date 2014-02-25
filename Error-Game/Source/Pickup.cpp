/***********************************************
 * Filename:  		Pickup.cpp
 * Date:      		2/12/2013
 * Mod. Date: 		2/12/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Class for all pickup objects
 ************************************************/
#include "Pickup.h"
#include "Game.h"
#include "GameplayState.h"

#define WEAP_ROT_SPEED 2.0f
#define PICKUP_HOVER_SPEED 3.0f

CPickup::CPickup(void)
{
	this->SetID(OBJ_PICKUP);
	SetRadius(0.5f);
	D3DXMatrixRotationX(&m_d3dTilt, D3DXToRadian(-40.0f));
	m_fHover = 0;
}


CPickup::~CPickup(void)
{
}

void CPickup::Update(float fDelta)
{
	m_fLifetime -= fDelta;
	m_fHover += fDelta*PICKUP_HOVER_SPEED;

	if(m_fLifetime <= 0)
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
		return;
	}

	float deltaRot = WEAP_ROT_SPEED * fDelta;
	m_fRotation += deltaRot;
	D3DXMatrixRotationY(&m_d3dRotation, m_fRotation);

	// if pickup goes off players frustum...destroy
	m_playerFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();
	if(SphereToFrustum(GetSphere(),m_playerFrustum))
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
#ifdef _DEBUG
		cout << "Pickup Destroyed" << endl;
#endif
	}
}
bool CPickup::CheckCollision(IBaseObject* obj)
{
	return false;
}

void CPickup::CollisionResponse(IBaseObject* obj)
{

}

int CPickup::GetGunType()
{
	if( m_nPickUpType == PU_GATLING )
		return GATLING;

	if( m_nPickUpType == PU_BEAM )
		return BEAM;

	return TESLA;
}

void CPickup::Render()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();

	IDirect3DDevice9* pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	D3DXMATRIX iden, trans, result;
	D3DXMatrixIdentity(&iden);
	int gunID = OBJ_DEFENSELASER;
	switch(m_nPickUpType)
	{
	case PU_GATLING:
		{
			gunID = OBJ_GATLING;
			D3DXMatrixTranslation(&trans, 0.0f, .607f, 0.0);
		}
		break;
	case PU_BEAM:
		{
			gunID = OBJ_BEAM;
			D3DXMatrixTranslation(&trans, 0.0f, .766f, 0.0f);
		}
		break;
	case PU_TESLA:
		{
			gunID = OBJ_TESLA;
			D3DXMatrixTranslation(&trans, 0.0f, .766f, 0.0f);
		}
		break;
	default:
		{
			D3DXMatrixTranslation(&trans, 0.0f, .766f, 0.0f);
		}
		break;
	}
	result = m_d3dRotation*trans;
	result = result * GetMatrix();
	result = m_d3dTilt*result;
	result[13] += sin(m_fHover) * 0.25f;

	ID3DXEffect* pShader = pAM->GetShader(gunID);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetMatrixArray("gBones", &iden, 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(gunID));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&result);
		pShader->SetInt("AmIHit", 0);

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		pD3D->SetStreamSource(0, pAM->GetVertBuffer(gunID), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(gunID));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(gunID), 0, pAM->GetNumTriangles(gunID));

		pShader->EndPass();
	}
	pShader->End();

	pShader = pAM->GetShader(GetID());
	passes = 0;
	pShader->Begin(&passes,0);
	result = GetMatrix();
	result[13] += sin(m_fHover) * 0.25f;
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetMatrixArray("gBones", &iden, 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(GetID()));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld", &result);
		pShader->SetInt("AmIHit",(GetBeingHit()));

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		pD3D->SetStreamSource(0, pAM->GetVertBuffer(GetID()), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(GetID()));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(GetID()), 0, pAM->GetNumTriangles(GetID()));

		pShader->EndPass();
	}
	pShader->End();

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}