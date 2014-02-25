/**********************************************************************************************
 * Filename:  		ViewManager.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that render all the objects in the correct order.
 **********************************************************************************************/
#include "ViewManager.h"
#include "Renderer.h"

CViewManager::CViewManager(void)
{
}

CViewManager::~CViewManager(void)
{
	m_fFadeDelta = 0.0f;
	m_bFade = false;
	m_bFadeIn = false;
	m_d3dBuffer->Release();
}

void CViewManager::Initialize(void)
{
	m_fFadeDelta = 0.0f;

	float* pFadeData = (float*)malloc(sizeof(TFadeVertex)*4);
	
	//First Rect
	m_tFadeScreen[0].d3dPosition = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				//top left corner
	m_tFadeScreen[1].d3dPosition = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				//top right corner
	m_tFadeScreen[2].d3dPosition = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				//bottom right corner
	m_tFadeScreen[3].d3dPosition = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);				//bottom left corner

	m_pD3D = CRenderer::GetInstance()->GetDirect3DDevice();

	m_pD3D->CreateVertexBuffer( sizeof(TFadeVertex)*4, 0, 0, D3DPOOL_MANAGED, &m_d3dBuffer, 0);
	void* vram = nullptr;
	m_d3dBuffer->Lock(0, (sizeof(TFadeVertex)*4), &vram, 0);
	memcpy(vram, m_tFadeScreen, sizeof(TFadeVertex)*4);
	m_d3dBuffer->Unlock();

	m_d3dFadeShader = CAssetManager::GetInstance()->LoadShader("TestFiles/FadetoBlack.fx");
}

void CViewManager::Render()
{
	CRenderer* pRenderer = CRenderer::GetInstance();
	std::list<IBaseObject*>::iterator iter;

	for( int i = 0; i < RG_NUM_GROUPS; ++i )
	{
		for( iter = m_lRenderObjects[i].begin(); iter != m_lRenderObjects[i].end(); ++iter )
			(*iter)->Render();
	}
}

void CViewManager::Clear()
{
	for( int i = 0; i < RG_NUM_GROUPS; ++i )
		m_lRenderObjects[i].clear();
}

void CViewManager::AddEffect(CEffect* pEffect, int nLayer)
{

}

void CViewManager::AddObject(IBaseObject* pObject, eRenderGroups nLayer)
{
	m_lRenderObjects[nLayer].push_back(pObject);
}

void CViewManager::RemoveEffect(CEffect* pEffect)
{

}

void CViewManager::RemoveObject(IBaseObject* pObject, eRenderGroups nLayer)
{
	m_lRenderObjects[nLayer].remove(pObject);
}

void CViewManager::UpdateFade(float fDelta)
{
	if(m_bFadeIn)
		m_fFadeDelta -= fDelta;
	else
		m_fFadeDelta += fDelta;

	if(m_fFadeDelta < 0.0f)
		m_bFade = false;
}

void CViewManager::RenderFade()
{
	m_pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	m_pD3D->SetRenderState(D3DRS_ZENABLE, false);

	unsigned int passes = 0;
	m_d3dFadeShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		m_d3dFadeShader->BeginPass(i);

		m_d3dFadeShader->SetFloat("FadeAmount", m_fFadeDelta);
		m_d3dFadeShader->CommitChanges();

		m_pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(JUST_POS));
		m_pD3D->SetStreamSource(0, m_d3dBuffer, 0, sizeof(TFadeVertex));
		m_pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		m_d3dFadeShader->EndPass();
	}
	m_d3dFadeShader->End();

	m_pD3D->SetRenderState(D3DRS_ZENABLE, true);
	m_pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}

void CViewManager::FadeOut(void)
{
	m_bFade = true;
	m_fFadeDelta = 0.0f;
	m_bFadeIn = false;
}

void CViewManager::FadeIn(void)
{
	m_bFade = true;
	m_bFadeIn = true;
	m_fFadeDelta = 1.0f;
}

