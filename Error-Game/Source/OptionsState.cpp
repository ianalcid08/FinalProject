#include "OptionsState.h"

/***********************************************
* Filename:  		OptionsState.cpp
* Date:      		02/08/2013
* Mod. Date: 		04/17/2013
* Mod. Initials:	TS
* Author:    		Joseph Thomas
* Purpose:   		State for handling options
************************************************/
#include "Input.h"
#include "Game.h"
#include "Renderer.h"
#include <string>
#include <sstream>
using namespace std;
#include "Audio.h"

COptionsState* COptionsState::GetInstance( void )
{
	static COptionsState s_Instance;

	return &s_Instance;
}

COptionsState::COptionsState(void)
{
	m_bMuteAll = false;
	m_bVsyncToggle = true;
}

COptionsState::~COptionsState(void)
{

}

void COptionsState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pBMF = CBitmapFont::GetInstance();
	m_pAM = CAssetManager::GetInstance();
	m_nChoice = 1;
	m_fTimer = 0.0f;
	d3dBGShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	d3dOption = m_pAM->LoadTexture("Assets/Textures/Menu_Options_Background.png");
	d3dOptionBG = m_pAM->LoadTexture("Assets/Textures/Menu_Options_Background_BG.png");

	m_rOptionBGRect.left = 0;
	m_rOptionBGRect.top = 0;
	m_rOptionBGRect.right = 2048;
	m_rOptionBGRect.bottom = 2048;
	m_nfirstBoxX = 270;
	m_nfirstBoxY = 360;
	m_nletterHeight = 32;
	m_nletterWidth = 32;
	m_fLetterScale = 0.35f;
	// Verts for the screen
	tBGVertexs[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	tBGVertexs[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	tBGVertexs[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	tBGVertexs[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	tBGVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tBGVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tBGVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tBGVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TOptVertex)*4,0,0,D3DPOOL_MANAGED,&d3dBGVertBuffer,0);
	void* vram = nullptr;
	d3dBGVertBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tBGVertexs,sizeof(TOptVertex)*4);
	d3dBGVertBuffer->Unlock();

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
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TOptVertex)*4,0,0,D3DPOOL_MANAGED,&d3dCursorBuffer,0);
	vram = nullptr;
	d3dCursorBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tCursorVertex,sizeof(TOptVertex)*4);
	d3dCursorBuffer->Unlock();

	m_bLockOut = false;
	if(m_bMuteAll)
	{
		WwiseNS::SetFXVolume(0);
		WwiseNS::SetMXVolume(0);
	}

	// Fading Varibles
	m_fFadingTotalTime = 1.0f;
	m_bisFadingDone = false;
	m_fFadingAmount = 0;
	m_fFadingElapsed = 0;
	m_fScreenPosition = 2.0f;

	m_fMouseX = m_pInput->GetScreenMouseX();
	m_fMouseY = m_pInput->GetScreenMouseY();

	//float temp = CGame::GetInstance()->GetGameGamma();
	//SetGammaRamp(50.0f);
	//SetGammaRamp(CGame::GetInstance()->GetGameGamma());
	
	m_fGammaAttribute = CGame::GetInstance()->GetGameGamma();
	
	int x = 0;
	
}	 

void COptionsState::Shutdown()
{
	if(d3dBGVertBuffer)
		d3dBGVertBuffer->Release();
	if(d3dCursorBuffer)
		d3dCursorBuffer->Release();
}

bool COptionsState::Input()
{	 
	if(m_fTimer < 0.25f)
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

	// check enter or LMB for things with lockout
	if(!m_bLockOut && m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB()) 
	{
		m_fTimer = 0.0f;
		m_bLockOut = true;
	}
	else if(m_bLockOut && m_pInput->CheckInput(ACCEPT,4))
	{
		m_bLockOut = false;
	}

	// checking if mute is on and someone wants to change volumes
	if(m_fFXVolume > 0.0f || m_fMusicVolume > 0.0f)
	{
		m_bMuteAll = false;
	}
	else if (m_fFXVolume == 0.0f || m_fMusicVolume == 0.0f)
	{
		m_bMuteAll = true;
	}



	// check enter or input for things that toggle
	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB())
	{
		m_fTimer = 0.0f;
		
		if(m_nChoice == VSYNC_CHANGE) // vsync
		{
			m_bVsyncToggle ? m_bVsyncToggle = false : m_bVsyncToggle = true;
			return true;
		}
		else if(m_nChoice == BACK_CHANGE) // Back
		{
			CGame::GetInstance()->SaveOptions("TestFiles/saveOptions.bin");
			CGame::GetInstance()->SaveVideoSettings("TestFiles/saveDisplayOptions.bin");
			m_pGame->PopState();
			return true;
		}
		else if(m_nChoice == HARD_MUTE) // Hard mute
		{
			if(m_bMuteAll)
			{
				WwiseNS::SetFXVolume(m_fFXVolume);
				WwiseNS::SetMXVolume(m_fMusicVolume);
			}
			else
			{
				m_fMusicVolume = 0.0f;
				m_fFXVolume = 0.0f;
				WwiseNS::SetFXVolume(m_fFXVolume);
				WwiseNS::SetMXVolume(m_fMusicVolume);
			}
			m_bMuteAll = !m_bMuteAll;
			m_bLockOut = false;
			return true;
		}
		else if(m_nChoice == DEFAULT_SETTINGS)
		{
			m_fFXVolume = 40.0f;
			m_fMusicVolume = 40.0f;
			m_fGammaAttribute = 50.0f;

			WwiseNS::SetFXVolume(m_fFXVolume);
			WwiseNS::SetMXVolume(m_fMusicVolume);
			CGame::GetInstance()->SetGamma(m_fGammaAttribute);
			// TODO NEED TO PUT RESOLUTION CHANGE WHEN I GET IT
		}
	}
	////////////////////////////////////
	// Keyboard Logic
	if(!m_bLockOut && m_pInput->CheckInput(DOWN,4))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice++;
		m_fTimer = 0.0f;
		if(m_nChoice == NUM_OPT)
			m_nChoice = FX_VOLUME;
	}
	else if(!m_bLockOut && m_pInput->CheckInput(UP,4))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice--;
		m_fTimer = 0.0f;
		if(m_nChoice == 0)
			m_nChoice = NUM_OPT-1;
	}

	if(moveMouse)
	{
		/////////////////////////////////////
		// Mouse Logic
										//min x			// max x					// min	y				// max y
		if(!m_bLockOut && ((m_fMouseX >= -0.464844f && m_fMouseX <= -0.0957f) && (m_fMouseY <= .05729f && m_fMouseY >= 0.01302f)))
		{
			if(m_nChoice != FX_VOLUME)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = FX_VOLUME;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.464844f && m_fMouseX <= -0.0957f) && (m_fMouseY < -0.0234375f && m_fMouseY > -.075527f)))
		{
			if(m_nChoice != MUSIC_VOLUME)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = MUSIC_VOLUME;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.464844f && m_fMouseX <= -0.05078f) && (m_fMouseY < -0.106771f && m_fMouseY > -0.15625f)))
		{
			if(m_nChoice != RESOLUTION_CHANGE)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = RESOLUTION_CHANGE;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.464844f && m_fMouseX <= -0.267578f) && (m_fMouseY < -0.195313f && m_fMouseY > -0.244792f)))
		{
			if(m_nChoice != GAMMA_CHANGE)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = GAMMA_CHANGE;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.464844f && m_fMouseX <= -0.308594f) && (m_fMouseY < -0.270833f && m_fMouseY > -0.325521f)))
		{
			if(m_nChoice != BACK_CHANGE)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = BACK_CHANGE;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.001953f && m_fMouseX <= 0.271484f) && (m_fMouseY <= 0.06771f && m_fMouseY >= 0.03385f)))
		{
			if(m_nChoice != HARD_MUTE)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = HARD_MUTE;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= 0.00193359f && m_fMouseX <= 0.195313f) && (m_fMouseY <= -0.0677f && m_fMouseY >= -0.0963542f)))
		{
			if(m_nChoice != VSYNC_CHANGE)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = VSYNC_CHANGE;
			}
		}
		else if(!m_bLockOut &&((m_fMouseX >= -0.0390625f && m_fMouseX <= 0.330078f) && (m_fMouseY <= -0.184896f && m_fMouseY >= -0.21875f)))
		{
			if(m_nChoice != DEFAULT_SETTINGS)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = DEFAULT_SETTINGS;
			}
		}
	}

	if(m_bLockOut && (m_pInput->CheckInput(LEFT,4) || (m_pInput->GetLMB() && ((m_fMouseX >= -0.462891f && m_fMouseX <= -0.335938f) && (m_fMouseY <= -0.403646f && m_fMouseY >= -0.528646f)))))
	{
		m_fTimer = 0.0f;
		//decrement the option;
		switch(m_nChoice)
		{
		case FX_VOLUME:
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
				m_fFXVolume -= 2.5f;
				if(m_fFXVolume <= 0)
					m_fFXVolume = 0;
				WwiseNS::SetFXVolume(m_fFXVolume);
			}
			break;
		case MUSIC_VOLUME:
			{
				m_fMusicVolume -= 2.5f;
				if(m_fMusicVolume <= 0)
					m_fMusicVolume = 0;
				WwiseNS::SetMXVolume(m_fMusicVolume);
			}
			break;
		case RESOLUTION_CHANGE:
			{
				m_nIndexResolution -= 1;
				if(m_nIndexResolution <= 0)
					m_nIndexResolution = 0;
			}
			break;
		case GAMMA_CHANGE:
			{
				m_fGammaAttribute -= 5.0f;
				if(m_fGammaAttribute <= 5)
					m_fGammaAttribute = 5;
				//CGame::GetInstance()->SetGamma(m_fGammaAttribute * 0.022f);
				CGame::GetInstance()->SetGamma((m_fGammaAttribute / 50.0f) - 1.0f);
			}
			break;
		}
	}
	if(m_bLockOut &&  (m_pInput->CheckInput(RIGHT,4) || (m_pInput->GetLMB() && ((m_fMouseX >= 0.203125f && m_fMouseX <= 0.332031f) && (m_fMouseY <= -0.40625f && m_fMouseY >= -0.526042f)))))
	{
		m_fTimer = 0.0f;
		//increment the option;
		switch(m_nChoice)
		{
		case FX_VOLUME:
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
				m_fFXVolume += 2.5f;
				if(m_fFXVolume >= 50.0f)
					m_fFXVolume = 50.0f;
				WwiseNS::SetFXVolume(m_fFXVolume);
			}
			break;
		case MUSIC_VOLUME:
			{
				m_fMusicVolume += 2.5f;
				if(m_fMusicVolume >= 50.0f)
					m_fMusicVolume = 50.0f;
				WwiseNS::SetMXVolume(m_fMusicVolume);
			}
			break;
		case RESOLUTION_CHANGE:
			{
				m_nIndexResolution += 1;
				if(m_nIndexResolution >= m_pRenderer->GetDisplayVector().size())
					m_nIndexResolution = m_pRenderer->GetDisplayVector().size() - 1;
			}
			break;
		case GAMMA_CHANGE:
			{
				m_fGammaAttribute += 5.0f;
				if(m_fGammaAttribute >= 100.0f)
					m_fGammaAttribute = 100.0f;
				//CGame::GetInstance()->SetGamma(m_fGammaAttribute * .022f);
				CGame::GetInstance()->SetGamma((m_fGammaAttribute / 50.0f) - 1.0f);
			}
			break;
		}
	}
	// What happens when play CLICKS on save
	if(((m_fMouseX >= -0.404297f && m_fMouseX <= -0.246f) && (m_fMouseY <= -0.664f && m_fMouseY >= -0.7135f)))
	{
		if(m_pInput->GetLMB())
			m_bLockOut = false;
	}
	if(GetAsyncKeyState('7') && 0x01)
		cout << "Mouse PosX = " << m_fMouseX << " Mouse PosY = " << m_fMouseY << endl;

	if(m_pInput->CheckInput(PAUSE, 4))
	{
		if(m_nChoice != BACK_CHANGE)
			m_nChoice = BACK_CHANGE;
		else
			CGame::GetInstance()->PopState();
	}

	return true;
}	 

void COptionsState::Update( float fElapsedTime )
{	 
	m_fTimer += fElapsedTime;

	 if(!m_bisFadingDone)
	{
		m_fFadingElapsed += fElapsedTime;
		if(m_fFadingElapsed < m_fFadingTotalTime)
		{
			m_fFadingAmount = m_fFadingElapsed / m_fFadingTotalTime;
			m_fScreenPosition = 2.0f - (2.0f * m_fFadingAmount);
		}
		if(m_fFadingElapsed >= m_fFadingTotalTime)
		{
			m_bisFadingDone = true;
		}
	}
}	 

void COptionsState::Render()
{	 

	m_pRenderer->GetSprite()->Flush();
	m_pRenderer->Clear();
	// New Options Code
	D3DXMATRIX modify;
	D3DXMatrixIdentity(&modify);

	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}
}
void COptionsState::RenderNormalScene()
{
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	unsigned int passes(0);
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TOptVertex));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dOptionBG);
		d3dBGShader->SetFloat("fadeAmount",1.0f);

		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

		d3dBGShader->SetTexture("gDiffuseTexture",d3dOption);
		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

		// All text on the screen
	// This is the code to put variables to print to screen
	/*int asdf = 100;
	string testnum;
	stringstream teststream;
	teststream.str("");
	teststream << asdf;
	testnum = teststream.str();
	m_pBMF->PrintBF(testnum.c_str(), 100,100,1.0f);*/
	// Left side
	int shownOptionBoxLX,shownOptionBoxLY = 0;

	shownOptionBoxLX = m_nfirstBoxX+m_nletterWidth;
	shownOptionBoxLY = m_nfirstBoxY+m_nletterHeight;

	int tempx = CRenderer::GetInstance()->GetWindowWidth();
	int tempy = CRenderer::GetInstance()->GetWindowHeight();

	// if someone has choosen an option a Save button will appear
	// must be click to reverse the lockout
	//m_nfirstBoxX = 270;
	//m_nfirstBoxY = 360;
	m_pBMF->PrintBF("Use W and S to choose option then A and D to change option\nenter to accept or Left Stick to change and A to accept",0 , (int)(((340 + (m_nletterHeight * 3) + 240) / 768.0f) * tempy),m_fLetterScale * 0.7f);
	
	if(m_bLockOut && ((m_fMouseX >= -0.404297f && m_fMouseX <= -0.246f) && (m_fMouseY <= -0.664f && m_fMouseY >= -0.7135f)))
	{
		m_pBMF->PrintBF("Save",int((shownOptionBoxLX / 1024.0f) * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + (((shownOptionBoxLX+m_nletterHeight*10.5f) / 768.0f)) * tempy),m_fLetterScale,0xff000000);
	}
	else if(m_bLockOut)
	{
		m_pBMF->PrintBF("Save",int((shownOptionBoxLX / 1024.0f) * tempx),(int)(((shownOptionBoxLX+m_nletterHeight*10.5f) / 768.0f) * tempy),m_fLetterScale);
	}

	// Mute
	if(m_nChoice == HARD_MUTE && m_bMuteAll)
	{
		m_pBMF->PrintBF("Mute All On",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + ((m_nfirstBoxY / 768.0f) * tempy)),m_fLetterScale*0.6f,0xff000000);
	}
	else if(m_bMuteAll)
	{
		m_pBMF->PrintBF("Mute All On",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),(int)((m_nfirstBoxY / 768.0f) * tempy),m_fLetterScale*0.6f);
	}
	if (m_nChoice == HARD_MUTE && !m_bMuteAll)
	{
		m_pBMF->PrintBF("Mute All Off",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + ((m_nfirstBoxY / 768.0f) * tempy)),m_fLetterScale*0.6f,0xff000000);
	}
	else if(!m_bMuteAll)
	{
		m_pBMF->PrintBF("Mute All Off",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),(int)((m_nfirstBoxY / 768.0f) * tempy),m_fLetterScale*0.6f);
	}

	// Vsync
	if(m_nChoice == VSYNC_CHANGE && m_bVsyncToggle)
	{
		m_pBMF->PrintBF("VSync On",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + (((m_nfirstBoxY+50) / 768.0f) * tempy)),m_fLetterScale*0.6f,0xff000000);
	}
	else if(m_bVsyncToggle)
	{
		m_pBMF->PrintBF("VSync On",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(((m_nfirstBoxY+50) / 768.0f) * tempy),m_fLetterScale*0.6f);
	}
	if(m_nChoice == VSYNC_CHANGE && !m_bVsyncToggle)
	{
		m_pBMF->PrintBF("Vsync Off",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + (((m_nfirstBoxY+50) / 768.0f) * tempy)),m_fLetterScale*0.6f,0xff000000);
	}
	else if(!m_bVsyncToggle)
	{
		m_pBMF->PrintBF("Vsync Off",(int)((m_nfirstBoxX+240) / 1024.0f * tempx),int(((m_nfirstBoxY+50) / 768.0f) * tempy),m_fLetterScale*0.6f);
	}


	if(m_nChoice == FX_VOLUME)
	{
		m_pBMF->PrintBF("FX Volume",int((m_nfirstBoxX / 1024.0f) * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + ((m_nfirstBoxY / 768.0f) * tempy)),m_fLetterScale,0xff000000);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<<"	,int((m_nfirstBoxX / 1024.0f) * tempx),		int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)		,m_fLetterScale);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);

		//Show Value
		string testNum;
		stringstream teststream;
		teststream.str("");
		teststream << (m_fFXVolume * 2);
		testNum = teststream.str();
		m_pBMF->PrintBF(testNum.c_str(),(int)(((m_nfirstBoxX+m_nletterWidth+120) / 1024.0f) * tempx),(int)(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy),m_fLetterScale * 2);

		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>>"	,(int)(((m_nfirstBoxX+m_nletterWidth+320) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);

	}
	else
		m_pBMF->PrintBF("FX Volume",int((m_nfirstBoxX / 1024.0f) * tempx),int((m_nfirstBoxY / 768.0f) * tempy),m_fLetterScale);
	if(m_nChoice == MUSIC_VOLUME)
	{
		m_pBMF->PrintBF("MX Volume",int((m_nfirstBoxX / 1024.0f) * tempx) ,int(sin(m_fTimer * 3.0f) * 5.0f + (((m_nfirstBoxY+m_nletterHeight) / 768.0f) * tempy)),m_fLetterScale,0xff000000);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<<"	,int((m_nfirstBoxX / 1024.0f) * tempx),		int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),		int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);

		// Show value m_fMusicVolume
		string testNum;
		stringstream teststream;
		teststream.str("");
		teststream << (m_fMusicVolume * 2);
		testNum = teststream.str();
		m_pBMF->PrintBF(testNum.c_str(),(int)(((m_nfirstBoxX+m_nletterWidth+120) / 1024.0f) * tempx),(int)(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy),m_fLetterScale * 2);

		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>>"	,(int)(((m_nfirstBoxX+m_nletterWidth+320) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)		,m_fLetterScale);
		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);
	}
	else
		m_pBMF->PrintBF("MX Volume",int((m_nfirstBoxX / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight) / 768.0f) * tempy),m_fLetterScale);
	if(m_nChoice == RESOLUTION_CHANGE)
	{
		m_pBMF->PrintBF("Resolution",int((m_nfirstBoxX / 1024.0f) * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + ((m_nfirstBoxY+m_nletterHeight*2) / 768.0f) * tempy),m_fLetterScale,0xff000000);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<<"	,int((m_nfirstBoxX / 1024.0f) * tempx),		int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)		,m_fLetterScale);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);

		// Show value of the D3DDisplay with index of m_nIndexResolution
		D3DDISPLAYMODE tempmode = m_pRenderer->GetDisplayVector()[m_nIndexResolution];
		string tempRes;
		stringstream teststream;
		teststream.str("");
		teststream << tempmode.Width << "x" << tempmode.Height;
		tempRes = teststream.str();
		m_pBMF->PrintBF(tempRes.c_str(),(int)(((m_nfirstBoxX+m_nletterWidth+60) / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy),m_fLetterScale * 1.3f);

		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>>"	,(int)(((m_nfirstBoxX+m_nletterWidth+320) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);	
	}
	else
		m_pBMF->PrintBF("Resolution",int((m_nfirstBoxX / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*2) / 768.0f) * tempy),m_fLetterScale);
	if(m_nChoice == GAMMA_CHANGE)
	{
		m_pBMF->PrintBF("Gamma",int((m_nfirstBoxX / 1024.0f) * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + (((m_nfirstBoxY+m_nletterHeight*3) / 768.0f) * tempy)),m_fLetterScale,0xff000000);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<<"	,int((m_nfirstBoxX / 1024.0f) * tempx),		int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF("<<"	,int((shownOptionBoxLX / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy)	,m_fLetterScale);

		// Show value
		string testNum;
		stringstream teststream;
		teststream.str("");
		teststream << m_fGammaAttribute;
		testNum = teststream.str();
		m_pBMF->PrintBF(testNum.c_str(),(int)(((m_nfirstBoxX+m_nletterWidth+120) / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy),m_fLetterScale * 2);

		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*5.5f) / 768.0f) * tempy)	,m_fLetterScale);
		m_pBMF->PrintBF(">>>"	,(int)(((m_nfirstBoxX+m_nletterWidth+320) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6) / 768.0f) * tempy)		,m_fLetterScale);
		m_pBMF->PrintBF(">>"	,(int)(((m_nfirstBoxX+m_nletterWidth+310) / 1024.0f) * tempx),	int(((m_nfirstBoxY+m_nletterHeight*6.5f) / 768.0f) * tempy),m_fLetterScale);
	}
	else
		m_pBMF->PrintBF("Gamma",int((m_nfirstBoxX / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*3) / 768.0f) * tempy),m_fLetterScale);

	if(m_nChoice == BACK_CHANGE)
	{
		m_pBMF->PrintBF("Back",int((m_nfirstBoxX / 1024.0f) * tempx),int(sin(m_fTimer * 3.0f) * 5.0f + (m_nfirstBoxY+m_nletterHeight*4)),m_fLetterScale,0xff000000);
	}
	else
		m_pBMF->PrintBF("Back",int((m_nfirstBoxX / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*4) / 768.0f) * tempy),m_fLetterScale);

	if(m_nChoice == DEFAULT_SETTINGS)
	{
		m_pBMF->PrintBF("Default Setting",int(((m_nfirstBoxX + 220) / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*3) / 768.0f) * tempy),m_fLetterScale * 0.6f,0xff000000);
	}
	else
	{
		m_pBMF->PrintBF("Default Setting",int(((m_nfirstBoxX + 220) / 1024.0f) * tempx),int(((m_nfirstBoxY+m_nletterHeight*3) / 768.0f) * tempy),m_fLetterScale * 0.6f);
	}

	// Render the mouse stuff
	// Updating the Mouse positions
	D3DXMATRIX mouseCursor;
	D3DXMatrixIdentity(&mouseCursor);
	D3DXMatrixTranslation(&mouseCursor,m_fMouseX,m_fMouseY,0.0f);
	passes = 0;
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
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TOptVertex));
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();
}

void COptionsState::RenderFadingScene()
{
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	unsigned int passes(0);
	d3dBGShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dBGShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dBGVertBuffer,0,sizeof(TOptVertex));
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dOptionBG);
		d3dBGShader->SetFloat("fadeAmount",m_fFadingAmount);
		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

		D3DXMatrixTranslation(&temp, m_fScreenPosition, 0.0f, 0.0f);
		d3dBGShader->SetMatrix("gViewProjection",&temp);
		d3dBGShader->SetTexture("gDiffuseTexture",d3dOption);
		d3dBGShader->SetFloat("fadeAmount",1.0f);
		d3dBGShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dBGShader->EndPass();
	}
	d3dBGShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}