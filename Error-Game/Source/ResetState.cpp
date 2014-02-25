#include "ResetState.h"

/***********************************************
* Filename:  		ResetState.cpp
* Date:      		02/08/2013
* Mod. Date: 		3/8/2013
* Mod. Initials:	IA
* Author:    		Joseph Thomas
* Purpose:   		State for handling game reset on death
************************************************/
#include "Input.h"
#include "Game.h"
#include "Renderer.h"
#include <string>
#include "GameplayState.h"
#include "Audio.h"
#include "LevelData.h"
#include "IntroState.h"
using namespace std;

CResetState* CResetState::GetInstance( void )
{
	static CResetState s_Instance;

	return &s_Instance;
}

CResetState::CResetState(void)
{
	m_bVictory = false;
}

CResetState::~CResetState(void)
{

}

void CResetState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pBMF = CBitmapFont::GetInstance();
	m_pAM = CAssetManager::GetInstance();
	m_nChoice = 1;
	m_fTimer = 0.0f;

	d3dResetBG = m_pAM->LoadTexture("Textures/PauseScreen.png");
	d3dBGShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	m_rResetBGRect.left = 0;
	m_rResetBGRect.top = 0;
	m_rResetBGRect.right = 2048;
	m_rResetBGRect.bottom = 2048;
	m_nletterHeight = 32;
	m_nletterWidth = 32;
	m_fLetterScale = 0.35f;

	// Verts for the screen
	tResetVertexs[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	tResetVertexs[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	tResetVertexs[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	tResetVertexs[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	tResetVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tResetVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tResetVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tResetVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TResetVertex)*4,0,0,D3DPOOL_MANAGED,&d3dBGVertBuffer,0);
	void* vram = nullptr;
	d3dBGVertBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tResetVertexs,sizeof(TResetVertex)*4);
	d3dBGVertBuffer->Unlock();

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
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TResetVertex)*4,0,0,D3DPOOL_MANAGED,&d3dCursorBuffer,0);
	vram = nullptr;
	d3dCursorBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tCursorVertex,sizeof(TResetVertex)*4);
	d3dCursorBuffer->Unlock();

	m_fMouseX = m_pInput->GetScreenMouseX();
	m_fMouseY = m_pInput->GetScreenMouseY();
}	 

void CResetState::Shutdown()
{
	m_bVictory = false;
	if(d3dCursorBuffer)
		d3dCursorBuffer->Release();
	if(d3dBGVertBuffer)
		d3dBGVertBuffer->Release();

	CIntroState::GetInstance()->SetSkipTutorial(false);
}

bool CResetState::Input()
{	 
	if(m_fTimer < 0.25f || !m_bisFadingDone)
		return true;

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

	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB()) // enter
	{
		m_fTimer = 0.0f;
		if(m_nChoice == 1) // Retry Yes
		{
			if(m_bVictory)
			{
				CGameplayState::GetInstance()->SetDeathHintCounter(0);
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
				CGame::GetInstance()->ChangeState(CHAR_SELECT_STATE);
				return true;
			}

			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			CGameplayState::GetInstance()->Retry();
			
			m_pInput->TurnOnConfine();
			m_pInput->ResetWorldCursor();
			m_pGame->PopState();
			return true;
		}
		else if(m_nChoice == 2) // Main Menu
		{
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			m_pGame->ChangeState(MAINMENU_STATE);
			return true;
		}

	}
	if((m_pInput->CheckInput(DOWN,4)))
	{
		m_nChoice++;
		m_fTimer = 0.0f;
		if(m_nChoice > 2)
			m_nChoice = 1;
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
	}
	else if(m_pInput->CheckInput(UP,4))
	{
		m_nChoice--;
		m_fTimer = 0.0f;
		if(m_nChoice == 0)
			m_nChoice = 2;
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
	}


	if(moveMouse)
	{
		if(((m_fMouseX >= -0.25f && m_fMouseX <= 0.04218f) && (m_fMouseY <= 0.075f && m_fMouseY >= 0.02777f)))
		{
			if(m_nChoice != 2)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = 2;
			}
		}
		else if(((m_fMouseX >= -0.25f && m_fMouseX <= -0.09218f) && (m_fMouseY <= 0.1638f && m_fMouseY >= 0.1166f)))
		{
			if(m_nChoice != 1)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = 1;
			}
		}
	}

#ifdef _DEBUG
	if(GetAsyncKeyState('7') && 0x01)
		cout << "Mouse PosX = " << m_fMouseX << " Mouse PosY = " << m_fMouseY << endl;
#endif

	return true;
}	 

void CResetState::Update( float fElapsedTime )
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

void CResetState::Render()
{	

	// NEW RENDER CODE
	//m_pRenderer->GetSprite()->Flush();
	//m_pRenderer->Clear();

	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}

	//m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//D3DXMATRIX modify;
	//D3DXMatrixIdentity(&modify);

	//m_pRenderer->GetSprite()->SetTransform(&modify);

	//m_pRenderer->DrawBackground(d3dResetBG,0,0,0.7f,0.7f,&m_rResetBGRect);
	/*if(m_bVictory)
	{
	m_pBMF->PrintBF("You Won!!!",FIRSTBOXX,FIRSTBOXY - 40,m_fLetterScale);
	if(m_nChoice == 1)
	m_pBMF->PrintBF("Retry",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY),m_fLetterScale);
	else
	m_pBMF->PrintBF("Retry",FIRSTBOXX,FIRSTBOXY,m_fLetterScale);
	if(m_nChoice == 2)
	m_pBMF->PrintBF("Main Menu",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY+m_nletterHeight),m_fLetterScale);
	else
	m_pBMF->PrintBF("Main Menu",FIRSTBOXX,FIRSTBOXY+m_nletterHeight,m_fLetterScale);
	}
	else
	{
	m_pBMF->PrintBF("You Lost!!!",FIRSTBOXX,FIRSTBOXY - 40,m_fLetterScale);
	if(m_nChoice == 1)
	m_pBMF->PrintBF("Retry",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY),m_fLetterScale);
	else
	m_pBMF->PrintBF("Retry",FIRSTBOXX,FIRSTBOXY,m_fLetterScale);
	if(m_nChoice == 2)
	m_pBMF->PrintBF("Main Menu",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY+m_nletterHeight),m_fLetterScale);
	else
	m_pBMF->PrintBF("Main Menu",FIRSTBOXX,FIRSTBOXY+m_nletterHeight,m_fLetterScale);
	}*/


}
void CResetState::RenderNormalScene()
{
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	unsigned int passes(0);
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TResetVertex));

		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dResetBG);
		d3dBGShader->SetFloat("fadeAmount",1.0f);
		d3dBGShader->CommitChanges();

		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);


	// Render the mouse stuff
	// Updating the Mouse positions
	m_fMouseX = m_pInput->GetScreenMouseX();
	m_fMouseY = m_pInput->GetScreenMouseY();

	passes = 0;
	D3DXMATRIX mouseCursor;
	D3DXMatrixIdentity(&mouseCursor);
	D3DXMatrixTranslation(&mouseCursor,m_fMouseX,m_fMouseY,0.0f);

	d3dCursorShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dCursorShader->BeginPass(i);
		d3dCursorShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
		d3dCursorShader->SetFloat("gFade", m_fFadingAmount);
		d3dCursorShader->SetTexture("gDiffuseTexture", d3dCursorTexture);
		d3dCursorShader->SetFloat("gRatio", 1.0f);
		d3dCursorShader->SetMatrix("gOffset", &mouseCursor);
		d3dCursorShader->CommitChanges();

		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TResetVertex));
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();
	//m_pRenderer->GetSprite()->Flush();
	float tempWidth,tempHeight;
	tempWidth = (FIRSTBOXX / 1024.0f) * CRenderer::GetInstance()->GetWindowWidth();
	tempHeight = (float)CRenderer::GetInstance()->GetWindowHeight();

	if(m_bVictory)
	{
		m_pBMF->PrintBF("You Won!!!",(int)tempWidth ,(int)(((FIRSTBOXY - 40) / 768.0f) * tempHeight),m_fLetterScale);
		if(m_nChoice == 1)
			m_pBMF->PrintBF("Retry",(int)tempWidth,int(sin(m_fTimer * 3.0f) * 5.0f + ((FIRSTBOXY / 768.0f) * tempHeight)),m_fLetterScale);
		else
			m_pBMF->PrintBF("Retry",(int)tempWidth,int((FIRSTBOXY / 768.0f) * tempHeight),m_fLetterScale);
		if(m_nChoice == 2)
			m_pBMF->PrintBF("Main Menu",(int)tempWidth,int(sin(m_fTimer * 3.0f) * 5.0f + (((FIRSTBOXY+m_nletterHeight) / 768.0f) * tempHeight)),m_fLetterScale);
		else
			m_pBMF->PrintBF("Main Menu",(int)tempWidth,int(((FIRSTBOXY+m_nletterHeight) / 768.0f) * tempHeight),m_fLetterScale);
	}
	else
	{
		m_pBMF->PrintBF("You Lost!!!",(int)tempWidth,(int)(((FIRSTBOXY - 40) / 768.0f) * tempHeight),m_fLetterScale);
		if(m_nChoice == 1)
			m_pBMF->PrintBF("Retry",(int)tempWidth,int(sin(m_fTimer * 3.0f) * 5.0f + ((FIRSTBOXY / 768.0f) * tempHeight)),m_fLetterScale);
		else
			m_pBMF->PrintBF("Retry",(int)tempWidth,int((FIRSTBOXY / 768.0f) * tempHeight),m_fLetterScale);
		if(m_nChoice == 2)
			m_pBMF->PrintBF("Main Menu",(int)tempWidth,int(sin(m_fTimer * 3.0f) * 5.0f + (((FIRSTBOXY+m_nletterHeight) / 768.0f) * tempHeight)),m_fLetterScale);
		else
			m_pBMF->PrintBF("Main Menu",(int)tempWidth,int(((FIRSTBOXY+m_nletterHeight) / 768.0f) * tempHeight),m_fLetterScale);
		

		int nDeathHintCounter = CGameplayState::GetInstance()->GetDeathHintCounter();

		if(nDeathHintCounter == 1)
		{
			m_pBMF->PrintBF("PROTOCOL 5M45H: Trigger the Smashbot attack", FIRSTBOXX-325, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale, D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("then DASH away and attack while it is vulnerable", FIRSTBOXX-375, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale, D3DCOLOR_RGBA(0, 255, 0, 255));
		}
		else if(nDeathHintCounter == 2)
		{
			m_pBMF->PrintBF("PROTOCOL Ty13r: Use special for overwhelming situations", FIRSTBOXX-425, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale, D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("but remember you only have it once per life", FIRSTBOXX-450, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale, D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 3)
		{
			m_pBMF->PrintBF("PROTOCOL S34N: SteamrollerBot kills on contact", FIRSTBOXX-350, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("watch for his tell before he charges", FIRSTBOXX-275, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale, D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 4)
		{
			m_pBMF->PrintBF("PROTOCOL I4N: molten pools damage all ground units", FIRSTBOXX-400, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("make them chase you through them", FIRSTBOXX-200, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 5)
		{
			m_pBMF->PrintBF("PROTOCOL J03: When you need to run through a molten pool", FIRSTBOXX-450, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("use dash to minimize the damage you take", FIRSTBOXX-300, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 6)
		{
			m_pBMF->PrintBF("PROTOCOL M1K3: Turrets and Towers are vulnerable", FIRSTBOXX-400, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("while rising from the ground. Destroy them quickly", FIRSTBOXX-400, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 7)
		{
			m_pBMF->PrintBF("PROTOCOL R0X13: Laser turrets destroy ground enemies", FIRSTBOXX-400, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("lead them through the beams", FIRSTBOXX-150, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
		else if(nDeathHintCounter == 8)
		{
			m_pBMF->PrintBF("PROTOCOL B055: The main core is shielded", FIRSTBOXX-300, FIRSTBOXY+m_nletterHeight*9,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );

			m_pBMF->PrintBF("Destroy the hands to drop the shield", FIRSTBOXX-250, FIRSTBOXY+m_nletterHeight*11,
							m_fLetterScale,D3DCOLOR_RGBA(0, 255, 0, 255) );
		}
	}
}
void CResetState::RenderFadingScene()
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
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TResetVertex));

		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dResetBG);
		d3dBGShader->SetFloat("fadeAmount",m_fFadingAmount);
		d3dBGShader->CommitChanges();

		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,srcBlend);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,destBlend);

	/*if(m_bVictory)
	{
		m_pBMF->PrintBF("You Won!!!",FIRSTBOXX,FIRSTBOXY - 40,m_fLetterScale);
		if(m_nChoice == 1)
			m_pBMF->PrintBF("Retry",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY),m_fLetterScale);
		else
			m_pBMF->PrintBF("Retry",FIRSTBOXX,FIRSTBOXY,m_fLetterScale);
		if(m_nChoice == 2)
			m_pBMF->PrintBF("Main Menu",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY+m_nletterHeight),m_fLetterScale);
		else
			m_pBMF->PrintBF("Main Menu",FIRSTBOXX,FIRSTBOXY+m_nletterHeight,m_fLetterScale);
	}
	else
	{
		m_pBMF->PrintBF("You Lost!!!",FIRSTBOXX,FIRSTBOXY - 40,m_fLetterScale);
		if(m_nChoice == 1)
			m_pBMF->PrintBF("Retry",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY),m_fLetterScale);
		else
			m_pBMF->PrintBF("Retry",FIRSTBOXX,FIRSTBOXY,m_fLetterScale);
		if(m_nChoice == 2)
			m_pBMF->PrintBF("Main Menu",FIRSTBOXX,int(sin(m_fTimer * 3.0f) * 5.0f + FIRSTBOXY+m_nletterHeight),m_fLetterScale);
		else
			m_pBMF->PrintBF("Main Menu",FIRSTBOXX,FIRSTBOXY+m_nletterHeight,m_fLetterScale);
	}*/
}
