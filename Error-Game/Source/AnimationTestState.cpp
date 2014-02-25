/***********************************************
 * Filename:  		AnimationTestState.cpp
 * Date:      		2/27/2013
 * Mod. Date: 		2/27/2013
 * Mod. Initials:	SD
 * Author:    		Sean Davis
 * Purpose:   		Used to test individual Animations,
 *					and animation system
 ************************************************/

#include "AnimationTestState.h"
#include "IBaseObject.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "AssetManager.h"
#include "Game.h"

CAnimationTestState* CAnimationTestState::GetInstance( void )
{
	static CAnimationTestState s_Instance;

	return &s_Instance;
}

CAnimationTestState::CAnimationTestState(void)
{
	
}

CAnimationTestState::~CAnimationTestState(void)
{

}

void CAnimationTestState::Initialize()
{
	m_pOF = CGame::GetInstance()->GetOF();
	m_pOM = CGame::GetInstance()->GetOM();
	m_pVM = CGame::GetInstance()->GetVM();
	m_pAM = CAssetManager::GetInstance();
	m_pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	
    m_pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_nCurrObject = 0;
	m_pOF->Create((eObjects)m_nCurrObject, &m_pCurrObject);
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	m_pCurrObject->SetMatrix(iden);
	
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixIdentity(&m_d3dRot);
	temp[10] = -1.0f;
	*(D3DXVECTOR3*)&temp[12] = D3DXVECTOR3(0.0f, 3.0f, 7.0f);
	m_d3dCam = temp;

	float fFov = (D3DXToRadian(60));
	float fAspect = (16.0f/9.0f);
	float fNear = 0.01f;
	float fFar = 100.0f; 
	D3DXMatrixPerspectiveFovLH(&m_d3dProj, fFov, fAspect, fNear, fFar);
	m_fIgnoreKeys = 0.0f;
	m_fCurrRotation = 0.0f;
	m_nCurrAnimation = 0;
	m_fAnimModTime = 1.0f;
	m_bKillAnimation = true;
}

void CAnimationTestState::Shutdown()
{
	m_pOM->Clear(true);
	m_pVM->Clear();
	m_pOF->Destroy(m_pCurrObject);
	m_pOF->ProcessDestroy();

	m_pOF = nullptr;
	m_pOM = nullptr;
	m_pAM = nullptr;
	m_pVM = nullptr;
	m_pD3D = nullptr;
}

bool CAnimationTestState::Input()
{
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		CGame::GetInstance()->ChangeState(MAINMENU_STATE);
	}
	return true;


}

void CAnimationTestState::Update(float fElapsedTime)
{
	// Model/Animation switching
	if(m_fIgnoreKeys < 0)
	{
		if(GetAsyncKeyState(VK_ADD)&1)
		{
			if(m_bUseKeyFrame)
			{
				++m_nCurrKeyFrame;
				if(!((unsigned int)m_nCurrKeyFrame < m_pAM->GetAnimation(m_nCurrObject, m_nCurrAnimation)->m_pBones[0].m_nNumFrames))
					m_nCurrKeyFrame = 0;
				LoadKeyFrame();
			}
			else
			{
				m_fIgnoreKeys = 0.5f;
				m_pCurrObject->ChangeAnimation(++m_nCurrAnimation);
			}
		}
		else if(GetAsyncKeyState(VK_SUBTRACT)&1)
		{
			if(m_bUseKeyFrame)
			{
				--m_nCurrKeyFrame;
				if(m_nCurrKeyFrame < 0)
					m_nCurrKeyFrame = m_pAM->GetAnimation(m_nCurrObject, m_nCurrAnimation)->m_pBones[0].m_nNumFrames-1;
				LoadKeyFrame();
			}
			else
			{
				m_fIgnoreKeys = 0.5f;
				if(m_nCurrAnimation > 0)
					m_pCurrObject->ChangeAnimation(--m_nCurrAnimation);
			}
		}
		else if(GetAsyncKeyState('1'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_PLAYER;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_PLAYER, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('2'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_PLAYERBOTTOM;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_PLAYERBOTTOM, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('3'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_SPIDER;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_SPIDER, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('4'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_PICKUP;
		}
		else if(GetAsyncKeyState('5'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_TRASH;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_TRASH, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('6'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_PATROL;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_PATROL, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('7'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_SMASH;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_SMASH, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('8'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_BURN;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_BURN, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('9'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_STEAM;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_STEAM, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('0'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_AICORE;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_AICORE, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('F'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_AIFIST;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_AIFIST, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('Q'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_VAT;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_VAT, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('W'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_CRATE;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_CRATE, &m_pCurrObject);
		}
		else if(GetAsyncKeyState('E'))
		{
			m_fIgnoreKeys = 0.5f;
			m_nCurrObject = OBJ_BARREL;
			m_pOF->Destroy(m_pCurrObject);
			m_pOF->ProcessDestroy();
			m_pOF->Create(OBJ_BARREL, &m_pCurrObject);
		}

		if(GetAsyncKeyState(VK_DIVIDE))
		{
			m_fIgnoreKeys = 0.5f;
			m_fAnimModTime *= 0.5f;
		}
		else if(GetAsyncKeyState(VK_MULTIPLY))
		{
			m_fIgnoreKeys = 0.5f;
			m_fAnimModTime += m_fAnimModTime;
		}
		if(GetAsyncKeyState('X')&1)
		{
			m_bKillAnimation = !m_bKillAnimation;
		}
		else if(GetAsyncKeyState('Z')&1)
		{
			if(m_bKillAnimation)
				m_bKillAnimation= false;
			m_bUseKeyFrame = !m_bUseKeyFrame;
			m_nCurrKeyFrame = 0;
		}
	}
	else
		m_fIgnoreKeys -= fElapsedTime;

	// Rotate model
	if(GetAsyncKeyState(VK_LEFT))
	{
		m_fCurrRotation += fElapsedTime;
		D3DXMatrixRotationY(&m_d3dRot, m_fCurrRotation * D3DXToRadian(60.0f));
	}
	else if(GetAsyncKeyState(VK_RIGHT))
	{
		m_fCurrRotation -= fElapsedTime;
		D3DXMatrixRotationY(&m_d3dRot, m_fCurrRotation * D3DXToRadian(60.0f));
	}

	if(!m_bUseKeyFrame)
	{
		float animTime = m_pCurrObject->GetAnimationTime();
		animTime += fElapsedTime*m_fAnimModTime;
		CAnimationProcessor::UpdateTime(m_pCurrObject->GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(m_pCurrObject->GetCurrentAnimation(), animTime, m_pCurrObject->PassCurrFrame());
		m_pCurrObject->SetAnimationTime(animTime);
	}
}

void CAnimationTestState::Render(void)
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	CRenderer* tmpRndr = CRenderer::GetInstance();
	tmpRndr->Clear(50,50,50);
	D3DXMATRIX preinv, inv;
	preinv = m_d3dCam*m_d3dRot;
	float junk = 0;
	D3DXMatrixInverse(&inv, &junk, &preinv);
	D3DXMATRIX ViewProj = inv * m_d3dProj;
	ID3DXEffect* pShader = nullptr;
	pShader = pAM->GetShader(m_nCurrObject);
	
	unsigned int passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		if(m_bKillAnimation)
			pShader->SetMatrixArray("gBones", &tmpRndr->KillAnimation(), 15);
		else
			pShader->SetMatrixArray("gBones", m_pCurrObject->PassCurrFrame(), m_pCurrObject->GetCurrentAnimation()->m_nNumMeshes);

		pShader->SetTexture("gDiffuseTexture", m_pAM->GetTexture(m_nCurrObject));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		D3DXMATRIX iden;
		D3DXMatrixIdentity(&iden);
		iden[0] = -1.0f;
		pShader->SetMatrix("gWorld", &iden);
		pShader->SetFloatArray("gLightDir", &preinv[12], 3);
		pShader->CommitChanges();

		m_pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(BASEOBJECTDECL));
		m_pD3D->SetStreamSource(0, m_pAM->GetVertBuffer(m_nCurrObject), 0, sizeof(tVertex));
		m_pD3D->SetIndices(m_pAM->GetIndexBuffer(m_nCurrObject));

		m_pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pAM->GetNumVerts(m_nCurrObject), 0, m_pAM->GetNumTriangles(m_nCurrObject));

		pShader->EndPass();
	}

	pShader->End();

	char buffer[150];
	int stringCount = 0;
	stringCount = sprintf_s(buffer, "Current Object ID: %i\nCurrent Animation ID: %i\nAnimation Speed: %f\nAnimation Time: %f\nKeyFrame: %i",
		m_nCurrObject, m_nCurrAnimation, m_fAnimModTime, m_pCurrObject->GetAnimationTime(), m_nCurrKeyFrame);
	RECT textRect;
	textRect.left = 0;
	textRect.top = 0;
	textRect.right = 300;
	textRect.bottom = 100;
	CRenderer::GetInstance()->GetFont()->DrawTextA(NULL, buffer, stringCount, &textRect, DT_LEFT, D3DCOLOR_ARGB(255,255,255,255));
}

void CAnimationTestState::LoadKeyFrame()
{
	D3DXMATRIX* getCurr = m_pCurrObject->PassCurrFrame();

	for(unsigned int i = 0; i < m_pAM->GetAnimation(m_nCurrObject, m_nCurrAnimation)->m_nNumMeshes; ++i)
	{
		getCurr[i] = m_pAM->GetAnimation(m_nCurrObject, m_nCurrAnimation)->m_pBones[i].m_pKeyFrames[m_nCurrKeyFrame].m_d3dTransform;
	}
}