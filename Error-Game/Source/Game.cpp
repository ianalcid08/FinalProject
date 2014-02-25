#include "Game.h"
#include "Renderer.h"
#include "IGameState.h"

// game states
#include "IntroState.h"
#include "MainMenuState.h"
#include "OptionsState.h"
#include "CreditsState.h"
#include "PauseState.h"
#include "GameplayState.h"
#include "ResetState.h"
#include "AnimationTestState.h"
#include "EffectTestState.h"
#include "CharacterSelectState.h"
#include "IntroSplashState.h"
#include "Difficulty.h"
#include "Input.h"
#include "Audio.h"
#include "../../AudioLibrary/Audio.h"
#include <iostream>
#include <fstream>
using namespace std;

// Init static object
CGame CGame::m_Instance;

IGameState* ProcessEnum(eGameStates _eIn);

CGame::CGame(void)
{
	m_nScreenWidth	= 0;
	m_nScreenHeight	= 0;	
	m_bIsWindowed	= true;
	m_bIsRunning = true;
	m_fDelta = 0.0f;
	m_Renderer = nullptr;
	m_fGammaValue = 0.0f;
}


CGame::~CGame(void)
{

}

CGame* CGame::GetInstance(void)
{
	return &m_Instance;
}
void CGame::SetGamma(float _gamma )
{
	//return;
	// MUST BE IN -1 TO 1
	
	D3DGAMMARAMP NewRamp; 
	WORD NewValue;
	WORD ui;

	//m_fGammaValue = (_gamma / 50.0f) - 1.0f;
	//m_fGammaValue = _gamma;

	//create linear gamma ramp
	for( unsigned int i = 0;i <= 255;i++ )
	{
		NewValue = i * (WORD)_gamma * 255; 
		ui = 0;
		if(NewValue > 32767)
		{ 
			NewValue = NewValue - 32767;
			ui = 1;
		}
		
		//manipulate bits to handle unsigned integers
		NewRamp.red[i] =	unsigned short(min(255,i * (_gamma + 1.0f))) << 8;
		NewRamp.green[i] =	unsigned short(min(255,i * (_gamma + 1.0f))) << 8;
		NewRamp.blue[i] =	unsigned short(min(255,i * (_gamma + 1.0f))) << 8;
	}
	
	

	//send gamma ramp to device
	m_Renderer->GetDirect3DDevice()->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &NewRamp);
	m_fGammaValue = (_gamma + 1.0f) * 50.0f;
} 
void CGame::Init(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	m_nScreenWidth	= nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_bIsWindowed	= bIsWindowed;
	m_hWnd = hWnd;
	m_hInstance = hInstance;

	m_Renderer = CRenderer::GetInstance();

	LoadVideoSettings("TestFiles/saveDisplayOptions.bin");

	m_nScreenWidth = m_Renderer->GetCurrDisplayMode().Width;
	m_nScreenHeight = m_Renderer->GetCurrDisplayMode().Height;
	m_Renderer->InitD3D(m_hWnd,m_nScreenWidth, m_nScreenHeight, m_bIsWindowed);
	m_Renderer->ChangeDisplayParam(m_nScreenWidth, m_nScreenHeight, m_bIsWindowed);

	m_cVM.Initialize();
	
	SetGamma(m_fGammaValue);

	CAssetManager::GetInstance()->initialize();
	m_cTimer.Reset();

#ifndef _DEBUG
	ShowCursor(FALSE);
#else
	ShowCursor(TRUE);
#endif

	m_cOF.Initialize(&m_cOM, &m_cVM);
	m_pInput = CInput::GetInstance();

	// Initialize the DirectInput object.
	bool result = m_pInput->InitializeDI(hInstance, hWnd, m_nScreenWidth, m_nScreenHeight);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the input object.", L"Error", MB_OK);
	}

	WwiseNS::InitSoundEngine("../Resources/Soundbanks/",0,0,0,0,0,0);

	// Load sound options
	LoadOptions("TestFiles/saveOptions.bin");
	PushState(SPLASHINTRO_STATE);
}

bool CGame::Input()
{
	if( !m_pCurrState.size() )
		return false;

	return m_pCurrState.back()->Input();
}

void CGame::Update()
{
	if( !m_pCurrState.size() )
		return;

	//CInput* cInput = CInput::GetInstance();
	//cInput->ReadDevices();

	m_pInput->Frame();
	m_pInput->ReadDevices();
	

	float fTime = m_cTimer.GetDeltaTime();
	m_cTimer.Update();

	if(fTime > .125f)
		fTime = .125f;

	m_fDelta = fTime;
	
	m_pCurrState.back()->Update(fTime);

	//update sound
	WwiseNS::Update();
}

void CGame::Render()
{
	m_Renderer->Clear(255,0,255);
	m_Renderer->GetDirect3DDevice()->BeginScene();
	m_Renderer->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );
	{
		for (auto iter = m_pCurrState.begin(); iter != m_pCurrState.end(); ++iter)
			(*iter)->Render();
	}
	m_Renderer->GetSprite()->End();
	m_Renderer->GetDirect3DDevice()->EndScene();
	
	m_Renderer->Present();
}
bool CGame::Play()
{
	if(Input() == false || m_bIsRunning == false)
		return false;

	Update();
	Render();

	return true;
}

void CGame::Shutdown()
{
	for(unsigned int i = 0; i < m_pCurrState.size(); ++i)
		m_pCurrState[i]->Shutdown();

	CAssetManager::GetInstance()->Shutdown();

	if(m_Renderer != nullptr)
	{
		m_Renderer->ShutdownD3D();
		m_Renderer = nullptr;
	}

	if(m_pInput)
		m_pInput->Shutdown();

	// shutdown the sound engine
	WwiseNS::TermSoundEngine();
}

void CGame::ChangeState(eGameStates _eNewState)
{
	// Exit the current state (if any)
	if (m_pCurrState.size() != 0)
	{
		for (auto iter = m_pCurrState.rbegin(); iter != m_pCurrState.rend(); ++iter)
			(*iter)->Shutdown();
		m_pCurrState.clear();
	}

	IGameState* pNewState = ProcessEnum(_eNewState);

	// Assign the current state
	m_pCurrState.push_back(pNewState);

	// Enter the new state (if any)
	if (m_pCurrState.back() != nullptr)
		m_pCurrState.back()->Initialize();
}

void CGame::PushState(eGameStates _eNewState) 
{
	IGameState* pNewState = ProcessEnum(_eNewState);

	for (auto iter = m_pCurrState.begin(); iter != m_pCurrState.end(); ++iter) 
	{
		if ((*iter) == pNewState) 
		{
			m_pCurrState.erase(iter);
			m_pCurrState.push_back(pNewState);
			return;
		}
	}
	m_pCurrState.push_back(pNewState);

	// Enter the new state (if any)
	if (m_pCurrState.back() != nullptr)
		m_pCurrState.back()->Initialize();
}

void CGame::PopState() 
{
	// If only 1 State is on the Stack dont remove it
	if (m_pCurrState.size() < 2)
		return;

	// Exit the current state
	m_pCurrState.back()->Shutdown();

	// Remove the Current state
	m_pCurrState.pop_back();
}

IGameState* ProcessEnum(eGameStates _eIn)
{
	IGameState* pOut = nullptr;

	switch(_eIn)
	{
	case INTRO_STATE:
		{
			pOut = CIntroState::GetInstance();
		}
		break;
	case MAINMENU_STATE:
		{
			pOut = CMainMenuState::GetInstance();
		}
		break;
	case OPTIONS_STATE:
		{
			pOut = COptionsState::GetInstance();
		}
		break;
	case CREDITS_STATE:
		{
			pOut = CCreditsState::GetInstance();
		}
		break;
	case PAUSE_STATE:
		{
			pOut = CPauseState::GetInstance();
		}
		break;
	case GAMEPLAY_STATE:
		{
			pOut = CGameplayState::GetInstance();
		}
		break;
	case RETRY_STATE:
		{
			pOut = CResetState::GetInstance();
		}
		break;
	case ANIMTEST_STATE:
		{
			pOut = CAnimationTestState::GetInstance();
		}
		break;
	case EFFECTTEST_STATE:
		{
			pOut = CEffectTestState::GetInstance();
		}
		break;
	case CHAR_SELECT_STATE:
		{
			pOut = CCharacterSelectState::GetInstance();
		}
		break;
	case SPLASHINTRO_STATE:
		{
			pOut = CIntroSplashState::GetInstance();
		}
		break;
	case DIFFICULTY_STATE:
		{
			pOut = CDifficulty::GetInstance();
		}
		break;
	}
	return pOut;
}

void CGame::SetCursorPos(int _nX, int _nY)
{
	m_pInput->UpdateMouse(_nX, m_nScreenHeight-_nY);
}

void CGame::SetLMB(bool _bL)
{
	m_pInput->SetLMB(_bL);
}

void CGame::SetRMB(bool _bR)
{
	m_pInput->SetRMB(_bR);
}
void CGame::LoadOptions(const char* _filename)
{
	ifstream loadFile;
	float tempFX =		0.0f;
	float tempMX =		0.0f;
	float tempGamma =	0.0f;
	bool tempMute =		false;
	
	loadFile.open(_filename,ios::binary);
	if(loadFile.is_open())
	{
		// if i add more things to saveoptions remember to how much to seek by for video settings
		loadFile.read((char*)&tempFX,sizeof(float));
		loadFile.read((char*)&tempMX,sizeof(float));
		loadFile.read((char*)&tempGamma,sizeof(float));
		loadFile.read((char*)&tempMute,sizeof(bool));
	}
	else
	{
		tempFX = 50.0f;
		tempMX = 50.0f;
		tempGamma = 50.0f;
		tempMute = false;
	}
	loadFile.close();

	/*if(tempGamma == 0)
		tempGamma = 50.0f;
	if(tempFX == 0 && !tempMute)
		tempFX = 50.0f;
	if(tempMX == 0 && !tempMute)
		tempMX = 50.0f;*/
		

	WwiseNS::SetFXVolume(tempFX);
	WwiseNS::SetMXVolume(tempMX);
	COptionsState::GetInstance()->SetFXVolume(tempFX);
	COptionsState::GetInstance()->SetMXVolume(tempMX);
	COptionsState::GetInstance()->SetMuteAll(tempMute);

	COptionsState::GetInstance()->SetGammaRamp(tempGamma); // to be shown on screen
	//CGame::GetInstance()->SetGamma(tempGamma);				// to be set on startup
	m_fGammaValue = tempGamma;
	
	
	
}
void CGame::LoadVideoSettings(const char* _filename)
{
	int	  tempIndex = 0; //resolution
	D3DDISPLAYMODE tempMode;
	

	ifstream loadfile;
	loadfile.open(_filename,ios::binary);
	if(loadfile.is_open())
	{
		//loadfile.seekg(sizeof(float) * 3); // if i add more things to saveoptions remember to how much to seek by
		//loadfile.read((char*)&tempIndex,sizeof(int));
		loadfile.read((char*)&tempMode,sizeof(D3DDISPLAYMODE));
		loadfile.read((char*)&tempIndex,sizeof(int));
		//loadfile.read((char*)&tempMode.Format,sizeof(D3DFORMAT));
		//loadfile.read((char*)&tempMode.Height,sizeof(UINT));
		//loadfile.read((char*)&tempMode.Width,sizeof(UINT));
		//loadfile.read((char*)&tempMode.RefreshRate,sizeof(UINT));
	}
	else
	{
		tempMode.Format = D3DFMT_X8R8G8B8;
		tempMode.Height = 768;
		tempMode.Width = 1024;
		tempMode.RefreshRate = 60;
	}
	loadfile.close();


	m_Renderer->SetCurrDisplayMode(tempMode);
	COptionsState::GetInstance()->SetIndexRes(tempIndex);
	D3DDISPLAYMODE asdf = m_Renderer->GetCurrDisplayMode();
	
	int n = COptionsState::GetInstance()->GetIndexRes();
}

void CGame::SaveOptions(const char* _filename)
{
	ofstream saveFile;
	float tempFX = COptionsState::GetInstance()->GetFXVolume();
	float tempMX = COptionsState::GetInstance()->GetMXVolume();
	float tempGamma = COptionsState::GetInstance()->GetGammaRamp();
	bool tempMute = COptionsState::GetInstance()->GetMuteAll();
	
	//int tempindex = 
	saveFile.open(_filename,ios::binary|ios::trunc);
	if(saveFile.is_open())
	{
		// if i add more things to saveoptions remember to how much to seek by for video settings
		saveFile.write((char*)&tempFX,sizeof(float));
		saveFile.write((char*)&tempMX,sizeof(float));
		saveFile.write((char*)&tempGamma,sizeof(float));
		saveFile.write((char*)&tempMute,sizeof(bool));
	}
	saveFile.close();
}
void CGame::SaveVideoSettings(const char* _filename)
{
	//int tempIndex = 2;
	D3DDISPLAYMODE temp;
	int tempindex = 2;
	temp.Format = D3DFMT_X8R8G8B8;
	temp.Height = 768;
	temp.Width = 1024;
	temp.RefreshRate = 60;

	ofstream saveFile;

	tempindex = COptionsState::GetInstance()->GetIndexRes();
	temp = m_Renderer->GetDisplayVector()[tempindex];
	

	saveFile.open(_filename,ios::binary|ios::trunc);
	if(saveFile.is_open())
	{
		//saveFile.seekp(sizeof(float) * 3); // if i add more things to saveoptions remember to how much to seek by
		//saveFile.write((char*)&tempIndex,sizeof(int));
		saveFile.write((char*)&temp,sizeof(D3DDISPLAYMODE));
		saveFile.write((char*)&tempindex,sizeof(int));
		//saveFile.write((char*)&temp.Format,sizeof(D3DFORMAT));
		//saveFile.write((char*)&temp.Height,sizeof(UINT));
		//saveFile.write((char*)&temp.Width,sizeof(UINT));
		//saveFile.write((char*)&temp.RefreshRate,sizeof(UINT));
	}
	saveFile.close();
}
