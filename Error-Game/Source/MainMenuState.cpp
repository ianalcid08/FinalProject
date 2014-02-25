#include "MainMenuState.h"

/***********************************************
* Filename:  		MainMenuState.cpp
* Date:      		02/08/2013
* Mod. Date: 		04/26/2013
* Mod. Initials:	IA
* Author:    		Joseph Thomas
* Purpose:   		State for handling the main menu
************************************************/
#include "Input.h"
#include "Game.h"
#include "Renderer.h"
#include <string>
#include <sstream>
using namespace std;
#include "Audio.h"
#include "MainMenuState.h"
#include "AssetManager.h"
#include "BitmapFont.h"

// 1280x720
//#define FIRSTBOXX 445
//#define FIRSTBOXY 350
// 1024x768
#define FIRSTBOXX 320
#define FIRSTBOXY 390
CMainMenuState* CMainMenuState::GetInstance( void )
{
	static CMainMenuState s_Instance;

	return &s_Instance;
}

CMainMenuState::CMainMenuState(void)
{
}

CMainMenuState::~CMainMenuState(void)
{

}

void CMainMenuState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pAM		= CAssetManager::GetInstance();
	m_pBMF		= CBitmapFont::GetInstance();
	m_pBMF->InitFont();

	m_nChoice = 1;
	m_fTimer = 0.0f;
	this->m_rBGRect.left = 0;
	m_rBGRect.top = 0;
	m_rBGRect.right = 2048;
	m_rBGRect.bottom = 2048;

	d3dBGShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	d3dBackGround = m_pAM->LoadTexture("Textures/MainMenu.png");
	d3dHighlight = m_pAM->LoadTexture("Assets/Textures/Menu_Button_Highlight.png");
	m_fLetterScale = 0.5f;
	m_nletterHeight = 32;
	m_nletterWidth = 32;

	m_rbuttonRects[0].left = 678;
	m_rbuttonRects[0].top = 515;
	m_rbuttonRects[0].right = 1215;
	m_rbuttonRects[0].bottom = 630;

	// Verts for the screen
	tBGVertexs[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	tBGVertexs[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	tBGVertexs[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	tBGVertexs[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	tBGVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tBGVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tBGVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tBGVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TBGVertex)*4,0,0,D3DPOOL_MANAGED,&d3dBGVertBuffer,0);
	void* vram = nullptr;
	d3dBGVertBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tBGVertexs,sizeof(TBGVertex)*4);
	d3dBGVertBuffer->Unlock();

	tBGVertexs[0].d3dPosition = D3DXVECTOR3(-0.32f,0.125f,0.0f);
	tBGVertexs[1].d3dPosition = D3DXVECTOR3(0.30f,0.125f,0.0f);
	tBGVertexs[2].d3dPosition = D3DXVECTOR3(0.30f,-0.125f,0.0f);
	tBGVertexs[3].d3dPosition = D3DXVECTOR3(-0.32f,-0.125f,0.0f);

	tBGVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tBGVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tBGVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tBGVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TBGVertex)*4,0,0,D3DPOOL_MANAGED,&d3dHighlightVertBuffer,0);
	vram = nullptr;
	d3dHighlightVertBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tBGVertexs,sizeof(TBGVertex)*4);
	d3dHighlightVertBuffer->Unlock();

	// Fading Varibles
	m_fFadingTotalTime = 1.0f;
	m_bisFadingDone = false;
	m_fFadingAmount = 0;
	m_fFadingElapsed = 0;

	// Mouse Input things
	// Mouse Offsets
	D3DXMatrixTranslation(&(m_d3dOffset[0]), -1.0f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[1]), -0.5f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[2]), 0.0f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[3]), 0.5f, 1.0f, 0.0f);

	tCursorVertex[0].d3dPosition = D3DXVECTOR3(.05f, 0.0f, 0.0f);
	tCursorVertex[1].d3dPosition = D3DXVECTOR3(.05f, -.1f, 0.0f);
	tCursorVertex[2].d3dPosition = D3DXVECTOR3(0.0f, -.1f, 0.0f);
	tCursorVertex[3].d3dPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tCursorVertex[0].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tCursorVertex[1].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	tCursorVertex[2].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tCursorVertex[3].d3dUV = D3DXVECTOR2(0.0f, 0.0f);

	d3dCursorShader = m_pAM->LoadShader("Assets/Shaders/HUDDraw.fx");
	d3dCursorTexture = m_pAM->LoadTexture("Textures/cursor.png");
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TBGVertex)*4,0,0,D3DPOOL_MANAGED,&d3dCursorBuffer,0);
	vram = nullptr;
	d3dCursorBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tCursorVertex,sizeof(TBGVertex)*4);
	d3dCursorBuffer->Unlock();

	//#ifndef _DEBUG
	// main menu music
	WwiseNS::LevelMusicPlay(WwiseNS::MENU_MUSIC);
	//#endif

	m_fMouseX = m_pInput->GetScreenMouseX();
	m_fMouseY = m_pInput->GetScreenMouseY();
}	 

void CMainMenuState::Shutdown()
{
	if(d3dBGVertBuffer)
		d3dBGVertBuffer->Release();
	if(d3dCursorBuffer)
		d3dCursorBuffer->Release();

	//#ifndef _DEBUG
	WwiseNS::LevelMusicStop(WwiseNS::MENU_MUSIC);
	//#endif
	m_pRenderer->GetSprite()->Flush();
	m_pRenderer->Clear();
}

bool CMainMenuState::Input()
{	 
	if(m_fTimer < 0.20f)
		return true;

	// Mouse stuff
	bool moveMouse = false;
	float tempX, tempY;
	tempX = m_pInput->GetScreenMouseX();
	tempY = m_pInput->GetScreenMouseY();

	if(tempX != m_fMouseX || tempY != m_fMouseY)
	{
		m_fMouseX = tempX;
		m_fMouseY = tempY;
		moveMouse = true;
	}
#ifdef _DEBUG
	if(GetAsyncKeyState(VK_F9)&1)
	{
		CGame::GetInstance()->ChangeState(ANIMTEST_STATE);
		return true;
	}
	else if(GetAsyncKeyState(VK_F10)&1)
	{
		CGame::GetInstance()->ChangeState(EFFECTTEST_STATE);
		return true;
	}
	else if(GetAsyncKeyState(VK_F12) &1)
	{
		CGame::GetInstance()->ChangeState(DIFFICULTY_STATE);
		return true;
	}
#endif
	if(m_pInput->CheckInput(PAUSE, 4))  //GetAsyncKeyState(VK_ESCAPE)&1)
	{
		if(m_nChoice == 4)
			return false;
		m_nChoice = 4;
	}

	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB()) // enter
	{
		m_fTimer = 0.0f;
		if(m_nChoice == 1)
		{	
			m_pGame->ChangeState(DIFFICULTY_STATE);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			return true;
		}
		else if(m_nChoice == 2)
		{
			m_pGame->PushState(OPTIONS_STATE);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			return true;
		}
		else if(m_nChoice == 3)
		{
			m_pGame->PushState(CREDITS_STATE);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			return true;
		}
		else if(m_nChoice == 4)
		{
			return false;
		}

	}

	if(m_pInput->CheckInput(DOWN,4))
	{
		m_nChoice++;
		m_fTimer = 0.0f;
		if(m_nChoice > 4)
			m_nChoice = 1;
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		return true;
	}
	else if(m_pInput->CheckInput(UP,4))
	{
		m_nChoice--;
		m_fTimer = 0.0f;
		if(m_nChoice == 0)
			m_nChoice = 4;
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		return true;
	}

	if(moveMouse)
	{
		//min x			// max x					// min	y				// max y
		if(((m_fMouseX >= -0.289 && m_fMouseX <= 0.2685f) && (m_fMouseY <= 0.05f && m_fMouseY >= -0.15111f)))
		{
			if(m_nChoice != 1)
			{
				m_nChoice = 1;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
		}
		else if(((m_fMouseX >= -0.3 && m_fMouseX <= 0.2734f) && (m_fMouseY <= -0.155f && m_fMouseY >= -0.375f)))
		{
			if(m_nChoice != 2)
			{
				m_nChoice = 2;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
		}
		else if(((m_fMouseX >= -0.2531f && m_fMouseX <= 0.18125) && (m_fMouseY <= -0.48333f && m_fMouseY >= -0.56111f)))
		{
			if(m_nChoice != 3)
			{
				m_nChoice = 3;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
		}
		else if(((m_fMouseX >= -0.1593 && m_fMouseX <= 0.0421f) && (m_fMouseY <= -0.719f && m_fMouseY >= -0.7861f)))
		{
			if(m_nChoice != 4)
			{
				m_nChoice = 4;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
		}
	}

	//  Put this into Screen Space and youll be good
	if(GetAsyncKeyState('7') && 0x01)
		cout << "Mouse PosX = " << m_fMouseX << " Mouse PosY = " << m_fMouseY << endl;

	return true;
}	 

void CMainMenuState::Update( float fElapsedTime )
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



}	 

void CMainMenuState::Render()
{	
	m_pRenderer->Clear();
	D3DXMATRIX modify;
	D3DXMatrixIdentity(&modify);

	m_pRenderer->GetSprite()->SetTransform(&modify);


	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}
}

void CMainMenuState::RenderNormalScene()
{
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
	unsigned int passes(0);
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TBGVertex));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dBackGround);
		d3dBGShader->SetFloat("fadeAmount",1.0f);

		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();

	//int scrollingX = 1024;
	//scrollingX -= (m_fTimer * 100);
	float tempH = CRenderer::GetInstance()->GetWindowHeight() * .908854f;
	m_pBMF->PrintBF("Use W and S to change option then enter to \naccept or Left Stick to change and A to accept",0 , (int)tempH,m_fLetterScale * 0.7f);



	if(m_nChoice == 1) // x = 475 y = 400
	{
		float tempx = 375.0f / 1024.0f; 
		float tempy = 390.0f / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .3515625f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .501f;
		m_pBMF->PrintBF("Play Game",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5 + newScreenHeight),m_fLetterScale );
	}
	else
	{
		float tempx = 375.0f / 1024.0f; 
		float tempy = 390.0f / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .3515625f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .501f;

		m_pBMF->PrintBF("Play Game",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale  );
	}
	if(m_nChoice == 2) // x = 515 y = 495
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + m_nletterHeight + 50) / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .6145f;
		m_pBMF->PrintBF("Options",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .6145f;
		m_pBMF->PrintBF("Options",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}
	if(m_nChoice == 3) // x = 515 y = 585
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*2) + 110.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .734375f;
		m_pBMF->PrintBF("Credits",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*2) + 160.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .734375f;
		m_pBMF->PrintBF("Credits",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}
	if(m_nChoice == 4) // x = 575 y = 665
	{
		float tempx = (360.0f + 116.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*3) + 160.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .425781f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .8411458f;
		m_pBMF->PrintBF("Exit",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .425781f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .8411458f;
		m_pBMF->PrintBF("Exit",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}

	passes = 0;
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dHighlightVertBuffer,0,sizeof(TBGVertex));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		D3DXMatrixTranslation(&temp,0.0f,(0.21f-(m_nChoice/4.0f)),0.0f);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dHighlight);
		d3dBGShader->SetFloat("fadeAmount",1.0f);

		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

	// Render the mouse stuff
	// Updating the Mouse positions
	passes = 0;
	D3DXMATRIX mouseCursor;
	D3DXMatrixIdentity(&mouseCursor);
	D3DXMatrixTranslation(&mouseCursor,m_fMouseX,m_fMouseY,0.0f);

	d3dCursorShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dCursorShader->BeginPass(i);
		d3dCursorShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
		d3dCursorShader->SetFloat("gFade", 1.0f);
		d3dCursorShader->SetTexture("gDiffuseTexture", d3dCursorTexture);
		d3dCursorShader->SetFloat("gRatio", 1.0f);
		d3dCursorShader->SetMatrix("gOffset", &mouseCursor);
		d3dCursorShader->CommitChanges();

		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TBGVertex));
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();
}

void CMainMenuState::RenderFadingScene()
{
	DWORD srcBlend,destBlend;
	m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
	m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND,&destBlend);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	unsigned int passes(0);
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TBGVertex));
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dBackGround);
		d3dBGShader->SetFloat("fadeAmount",m_fFadingAmount);

		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,srcBlend);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,destBlend);

	//m_pRenderer->GetSprite()->Flush();
	//if(m_nChoice == 1) // x = 475 y = 400
	//	m_pBMF->PrintBF("Play Game",FIRSTBOXX+40,int(sin(m_fTimer * 5.0f) * 5 + FIRSTBOXY),m_fLetterScale);
	//else
	//	m_pBMF->PrintBF("Play Game",FIRSTBOXX+40,FIRSTBOXY,m_fLetterScale);
	//if(m_nChoice == 2) // x = 515 y = 495
	//	m_pBMF->PrintBF("Options",FIRSTBOXX + 70,int(sin(m_fTimer * 5.0f) * 5.0f + FIRSTBOXY + m_nletterHeight + 50),m_fLetterScale);
	//else
	//	m_pBMF->PrintBF("Options",FIRSTBOXX + 70,FIRSTBOXY + m_nletterHeight + 50,m_fLetterScale);
	//if(m_nChoice == 3) // x = 515 y = 585
	//	m_pBMF->PrintBF("Credits",FIRSTBOXX + 70,int(sin(m_fTimer * 5.0f) * 5.0f + FIRSTBOXY + (m_nletterHeight * 2) + 110),m_fLetterScale);
	//else
	//	m_pBMF->PrintBF("Credits",FIRSTBOXX + 70,FIRSTBOXY + (m_nletterHeight * 2) + 110,m_fLetterScale);
	//if(m_nChoice == 4) // x = 575 y = 665
	//	m_pBMF->PrintBF("Exit",FIRSTBOXX+116,int(sin(m_fTimer * 5.0f) * 5.0f + FIRSTBOXY + (m_nletterHeight * 3) + 160),m_fLetterScale);
	//else
	//	m_pBMF->PrintBF("Exit",FIRSTBOXX+116,FIRSTBOXY + (m_nletterHeight * 3) + 160,m_fLetterScale);
	float tempH = CRenderer::GetInstance()->GetWindowHeight() * .908854f;
	m_pBMF->PrintBF("Use W and S to change option then enter to \naccept or Left Stick to change and A to accept",0 , (int)tempH,m_fLetterScale * 0.7f);



	if(m_nChoice == 1) // x = 475 y = 400
	{
		float tempx = 375.0f / 1024.0f; 
		float tempy = 390.0f / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .3515625f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .501f;
		m_pBMF->PrintBF("Play Game",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5 + newScreenHeight),m_fLetterScale );
	}
	else
	{
		float tempx = 375.0f / 1024.0f; 
		float tempy = 390.0f / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .3515625f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .501f;

		m_pBMF->PrintBF("Play Game",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale  );
	}
	if(m_nChoice == 2) // x = 515 y = 495
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + m_nletterHeight + 50) / 768.0f;
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .6145f;
		m_pBMF->PrintBF("Options",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .6145f;
		m_pBMF->PrintBF("Options",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}
	if(m_nChoice == 3) // x = 515 y = 585
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*2) + 110.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .734375f;
		m_pBMF->PrintBF("Credits",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float tempx = (360.0f + 70.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*2) + 160.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .380859f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .734375f;
		m_pBMF->PrintBF("Credits",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}
	if(m_nChoice == 4) // x = 575 y = 665
	{
		float tempx = (360.0f + 116.0f) / 1024.0f; 
		float tempy = (390.0f + (m_nletterHeight*3) + 160.0f) / 768.0f;

		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .425781f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .8411458f;
		m_pBMF->PrintBF("Exit",(int)newScreenWidth,int(sin(m_fTimer * 5.0f) * 5.0f + newScreenHeight),m_fLetterScale);
	}
	else
	{
		float newScreenWidth = CRenderer::GetInstance()->GetWindowWidth() * .425781f;
		float newScreenHeight = CRenderer::GetInstance()->GetWindowHeight() * .8411458f;
		m_pBMF->PrintBF("Exit",(int)newScreenWidth,(int)newScreenHeight,m_fLetterScale);
	}
	// Render the mouse stuff
	passes = 0;
	D3DXMATRIX mouseCursor;
	D3DXMatrixIdentity(&mouseCursor);
	D3DXMatrixTranslation(&mouseCursor,m_fMouseX,m_fMouseY,0.0f);

	d3dCursorShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dCursorShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TBGVertex));
		d3dCursorShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
		d3dCursorShader->SetFloat("gFade", m_fFadingAmount);
		d3dCursorShader->SetTexture("gDiffuseTexture", d3dCursorTexture);
		d3dCursorShader->SetFloat("gRatio", 1.0f);
		d3dCursorShader->SetMatrix("gOffset", &mouseCursor);
		d3dCursorShader->CommitChanges();

		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();
}
