#include "IntroSplashState.h"
/***********************************************
 * Filename:  		IntroSplashState.cpp
 * Date:      		03/08/2013
 * Mod. Date: 		03/08/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		State for handling the main menu
 ************************************************/
#include "Renderer.h"
#include "Input.h"
#include "AssetManager.h"
#include "Game.h"

enum ETEXTURES {GP_LOGO, STUDIO_LOGO, TEAM_LOGO, START_SPLASH, NUM_TEX};

CIntroSplashState::CIntroSplashState(void)
{
}


CIntroSplashState::~CIntroSplashState(void)
{
}

CIntroSplashState* CIntroSplashState::GetInstance( void )
{
	static CIntroSplashState s_Instance;

	return &s_Instance;
}
void CIntroSplashState::Initialize()
{
	m_pRenderer = CRenderer::GetInstance();
	m_pAM		= CAssetManager::GetInstance();
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();

	m_d3dCollection[GP_LOGO] = m_pAM->LoadTexture("Assets/Textures/GPGames_Logo.png");
	m_d3dCollection[STUDIO_LOGO] = m_pAM->LoadTexture("Assets/Textures/BroSloth_LOGO.png");
	m_d3dCollection[TEAM_LOGO] = m_pAM->LoadTexture("Assets/Textures/LoungeTigerFull.jpg");
	m_d3dCollection[START_SPLASH] = m_pAM->LoadTexture("Assets/Textures/StartScreen.png");
	m_fsplashTimer = 10.0f;
	m_bIdle = true;

	// Verts for the screen
	m_tIntroVerts[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	m_tIntroVerts[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	m_tIntroVerts[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	m_tIntroVerts[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);
	   
	m_tIntroVerts[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	m_tIntroVerts[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	m_tIntroVerts[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	m_tIntroVerts[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_d3dIntroShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TIntroVertex)*4,0,0,D3DPOOL_MANAGED,&m_d3dIntroBuff,0);
	void* vram = nullptr;
	m_d3dIntroBuff->Lock(0,0,&vram,0);
	memcpy(vram,m_tIntroVerts,sizeof(TIntroVertex)*4);
	m_d3dIntroBuff->Unlock();

	// Setting up the fading variables
	m_nCurrScreen = 0;
	m_fFadingTotalTime = 2.0f;
	// Company logo
	m_bisFadingDone[GP_LOGO]  = false;
	m_fFadingAmount[GP_LOGO] = 0; // this is a percent
	m_fFadingElapsed[GP_LOGO] = 0;
	// Studio logo
	m_bisFadingDone[STUDIO_LOGO] = false;
	m_fFadingAmount[STUDIO_LOGO] = 0; // this is a percent
	m_fFadingElapsed[STUDIO_LOGO] = 0;
	// Team Logo
	m_bisFadingDone[TEAM_LOGO] = false;
	m_fFadingAmount[TEAM_LOGO] = 0; // this is a percent
	m_fFadingElapsed[TEAM_LOGO] = 0;
	// Entry Screen
	m_bisFadingDone[START_SPLASH] = false;
	m_fFadingAmount[START_SPLASH] = 0; // this is a percent
	m_fFadingElapsed[START_SPLASH] = 0;
}	 
void CIntroSplashState::Shutdown()
{	 
	 m_d3dIntroBuff->Release();
}	 
bool CIntroSplashState::Input()
{	 
	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->CheckInput(PAUSE, 4))
	{
		m_pGame->ChangeState(MAINMENU_STATE);
		return true;
	}

	return true;
}	 
void CIntroSplashState::Update( float fElapsedTime )
{	
	

	m_fsplashTimer -= fElapsedTime;
	
	if(!m_bisFadingDone[m_nCurrScreen])
	{
		m_fFadingElapsed[m_nCurrScreen] += fElapsedTime;
		m_fFadingAmount[m_nCurrScreen] = m_fFadingElapsed[m_nCurrScreen] / m_fFadingTotalTime;

		if(m_fFadingElapsed[m_nCurrScreen] > m_fFadingTotalTime)
		{
			m_bisFadingDone[m_nCurrScreen] = true;
		}
	}
	else
	{
		if(m_nCurrScreen == START_SPLASH)
			return;
		m_fFadingElapsed[m_nCurrScreen] -= fElapsedTime;
		m_fFadingAmount[m_nCurrScreen] = m_fFadingElapsed[m_nCurrScreen] / m_fFadingTotalTime;
		if(m_fFadingElapsed[m_nCurrScreen] < 0)
		{
			m_nCurrScreen++;
		}
	}

}	 
void CIntroSplashState::Render()
{
	
	m_pRenderer->Clear(0,0,0);

	unsigned int passes(0);
		m_d3dIntroShader->Begin(&passes,0);
		for(unsigned i(0); i < passes; i++)
		{
			m_d3dIntroShader->BeginPass(i);
			m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,m_d3dIntroBuff,0,sizeof(TIntroVertex));
			m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
			D3DXMATRIX temp;
			D3DXMatrixIdentity(&temp);

			m_d3dIntroShader->SetMatrix("gViewProjection",&temp);
			// the different logos
			m_d3dIntroShader->SetTexture("gDiffuseTexture", m_d3dCollection[m_nCurrScreen]);

			m_d3dIntroShader->SetFloat("fadeAmount",m_fFadingAmount[m_nCurrScreen]);

			m_d3dIntroShader->CommitChanges();
			m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			m_d3dIntroShader->EndPass();
		}
		m_d3dIntroShader->End();
}