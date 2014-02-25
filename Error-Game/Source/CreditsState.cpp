#include "CreditsState.h"

/***********************************************
 * Filename:  		CreditsState.cpp
 * Date:      		02/08/2013
 * Mod. Date: 		03/23/2013
 * Mod. Initials:	IA
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling credits
 ************************************************/
#include "Input.h"
#include "Game.h"
#include "Renderer.h"
#include "../../AudioLibrary/Audio.h"
CCreditsState* CCreditsState::GetInstance( void )
{
	static CCreditsState s_Instance;

	return &s_Instance;
}

CCreditsState::CCreditsState(void)
{

}

CCreditsState::~CCreditsState(void)
{
}
 
void CCreditsState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pBMF = CBitmapFont::GetInstance();
	m_pAM = CAssetManager::GetInstance();
	m_fTimer = 0.0f;
	d3dBGShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	d3dCreditsBG = m_pAM->LoadTexture("TestFiles/BossArena_NoPlayers.jpg");

	// Verts for the screen
	tBGVertexs[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	tBGVertexs[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	tBGVertexs[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	tBGVertexs[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);
	
	tBGVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tBGVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tBGVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tBGVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TCreditVertex)*4,0,0,D3DPOOL_MANAGED,&d3dBGVertBuffer,0);
	void* vram = nullptr;
	d3dBGVertBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tBGVertexs,sizeof(TCreditVertex)*4);
	d3dBGVertBuffer->Unlock();

	// Fading Varibles
	m_fFadingTotalTime = 1.0f;
	m_bisFadingDone = false;
	m_fFadingAmount = 0;
	m_fFadingElapsed = 0;
	m_nScrolling = (float)CGame::GetInstance()->GetScreenHeight();
}	 
	 
void CCreditsState::Shutdown()
{
	if(d3dBGVertBuffer)
		d3dBGVertBuffer->Release();
}
	 
bool CCreditsState::Input()
{	
	if(m_fTimer < 0.25f)
		return true;

	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB())
	{
		CGame::GetInstance()->ChangeState(MAINMENU_STATE);
	}
	 return true;
}	 
	 
void CCreditsState::Update( float fElapsedTime )
{	 
	 m_fTimer += fElapsedTime;

	 if(!m_bisFadingDone)
	{
		m_fFadingElapsed += fElapsedTime;
		if(m_fFadingElapsed < m_fFadingTotalTime)
		{
			m_fFadingAmount = m_fFadingElapsed / m_fFadingTotalTime;
		}
		if(m_fFadingElapsed >= m_fFadingTotalTime)
		{
			m_bisFadingDone = true;
		}
	}
	 else
	 {
		 if(m_nScrolling < -1300)
			m_nScrolling = (float)CGame::GetInstance()->GetScreenHeight();
		else
			m_nScrolling -= fElapsedTime * 75.0f;
	 }
}	 
	 
void CCreditsState::Render()
{	 
	 m_pRenderer->GetSprite()->Flush();
	m_pRenderer->Clear();
	// New Options Code
	D3DXMATRIX modify;
	D3DXMatrixIdentity(&modify);
	m_pRenderer->GetSprite()->SetTransform(&modify);

	if(m_bisFadingDone)
	{
		RenderNormalScene(m_fTimer);
	}
	else
	{
		RenderFadingScene();
	}


}	 
void CCreditsState::RenderNormalScene(float _fDelta)
{
	DWORD srcBlend,destBlend;
		m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
		m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND,&destBlend);
		//m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ZENABLE,false);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		unsigned int passes(0);
		d3dBGShader->Begin(&passes,0);
		for(unsigned i(0); i < passes; i++)
		{
			d3dBGShader->BeginPass(i);
			m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TCreditVertex));
			m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);

			d3dBGShader->SetMatrix("gViewProjection",&temp);
			d3dBGShader->SetTexture("gDiffuseTexture",d3dCreditsBG);
			d3dBGShader->SetFloat("fadeAmount",1.0f);

			d3dBGShader->CommitChanges();
			m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			d3dBGShader->EndPass();
		}
		d3dBGShader->End();
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,srcBlend);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,destBlend);

		// render names
		
		m_pBMF->PrintBF("Executive Producer \n Liam Hislop \n\nAssociate Producers \n Mike Lebo \n\nTech Lead \n Jason Hinders \n\nExternal Producer \n JC Park \n\nArt Director \n Kris Ducote \n\nProgrammers \n Ian Alcid\n Mike Barbato\n Sean Davis\n Tyler Springer\n Joe Thomas\n\n\n     Thank you for \n   all your support!",0,(int)m_nScrolling,0.8f,0xffffffff);
			
}
void CCreditsState::RenderFadingScene()
{
	DWORD srcBlend,destBlend;
		m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
		m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND,&destBlend);
		//m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ZENABLE,false);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		unsigned int passes(0);
		d3dBGShader->Begin(&passes,0);
		for(unsigned i(0); i < passes; i++)
		{
			d3dBGShader->BeginPass(i);
			m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TCreditVertex));
			m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);

			d3dBGShader->SetMatrix("gViewProjection",&temp);
			d3dBGShader->SetTexture("gDiffuseTexture",d3dCreditsBG);
			d3dBGShader->SetFloat("fadeAmount",m_fFadingAmount);

			d3dBGShader->CommitChanges();
			m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			d3dBGShader->EndPass();
		}
		d3dBGShader->End();
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,srcBlend);
		m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,destBlend);
}
