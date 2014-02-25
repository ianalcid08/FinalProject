#include "CharacterSelectState.h"

/***********************************************
 * Filename:  		CharacterSelectState.cpp
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling character selection
 ************************************************/

#include <string>
using namespace std;
#include "Audio.h"
#include "Game.h"
#include "GameplayState.h"

CCharacterSelectState* CCharacterSelectState::GetInstance( void )
{
	static CCharacterSelectState s_Instance;

	return &s_Instance;
}

CCharacterSelectState::CCharacterSelectState(void)
{
	
}

CCharacterSelectState::~CCharacterSelectState(void)
{
	
}

void CCharacterSelectState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pAM = CAssetManager::GetInstance();
	m_nCounter = 0;
	m_fTimer = 0.0f;
	for(int i = 0; i < 4; i++)
	{
		m_tPlayerIN[i].bPlayerIn = false;
		m_tPlayerIN[i].nPlayerColor = BLUE;
		m_tPlayerIN[i].fInputTimer = 0.0f;
		m_tPlayerIN[i].bPlayerLocked = false;
		m_tPlayerIN[i].bSameColor = false;
	}

	for(int i = 0; i < 4; i++)
		m_nColors[i] = -1;

	m_bReadyScreen = false;

	float* pCharSelectData = (float*)malloc(sizeof(TCharScrVertex)*4);
	
	//First Rect
	m_tCharSelect1[0].d3dPosition = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				//top left corner
	m_tCharSelect1[1].d3dPosition = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//top right corner
	m_tCharSelect1[2].d3dPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//bottom right corner
	m_tCharSelect1[3].d3dPosition = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);				//bottom left corner
		
	m_tCharSelect1[0].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	m_tCharSelect1[1].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	m_tCharSelect1[2].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	m_tCharSelect1[3].d3dUV = D3DXVECTOR2(0.0f, 1.0f);

	//Menu Rect
	m_tCharSelectMenu[0].d3dPosition = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);			//Top Left
	m_tCharSelectMenu[1].d3dPosition = D3DXVECTOR3(1.0f, 1.0f, 0.0f);			//Top Right
	m_tCharSelectMenu[2].d3dPosition = D3DXVECTOR3(1.0f, -1.0f, 0.0f);			//Bottom Right
	m_tCharSelectMenu[3].d3dPosition = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);			//Bottom Left

	m_tCharSelectMenu[0].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	m_tCharSelectMenu[1].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	m_tCharSelectMenu[2].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	m_tCharSelectMenu[3].d3dUV = D3DXVECTOR2(0.0f, 1.0f);


	memcpy((char*)pCharSelectData, m_tCharSelect1, sizeof(TCharScrVertex)*4);
	
	m_pD3D = m_pRenderer->GetDirect3DDevice();

	m_d3dTempShader = m_pAM->LoadShader("TestFiles/CharSelectShader.fx");
	m_d3dBGShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	m_d3dCharBlueTexture = m_pAM->LoadTexture("Assets/Textures/HUD_Player_BluePortrait.png");
	m_d3dCharRedTexture = m_pAM->LoadTexture("Assets/Textures/HUD_Red_Portrait.png");
	m_d3dCharGreenTexture = m_pAM->LoadTexture("Assets/Textures/HUD_Green_Portrait.png");
	m_d3dCharYellowTexture = m_pAM->LoadTexture("Assets/Textures/HUD_Yellow_Portrait.png");
	m_d3dJoinGameTexture = m_pAM->LoadTexture("Assets/Textures/Menu_Character_Select_Screen.png");
	m_d3dAllReadyTexture = m_pAM->LoadTexture("Assets/Textures/AllReadyScreen.png");
	m_d3dPressStartTexture = m_pAM->LoadTexture("Assets/Textures/PressStart.png");
	
	m_pD3D->CreateVertexBuffer((sizeof(TCharScrVertex)*4)*2, 0, 0, D3DPOOL_MANAGED, &m_d3dCharSelectBuff, 0);
	void* vram = nullptr;
	m_d3dCharSelectBuff->Lock(0, (sizeof(TCharScrVertex)*4), &vram, 0);
	memcpy(vram,pCharSelectData, sizeof(TCharScrVertex)*4);
	m_d3dCharSelectBuff->Unlock();
	//free(pCharSelectData);

	memcpy((char*)pCharSelectData, m_tCharSelectMenu, sizeof(TCharScrVertex)*4);
	vram = nullptr;
	m_d3dCharSelectBuff->Lock((sizeof(TCharScrVertex)*4), (sizeof(TCharScrVertex)*4), &vram, 0);
	memcpy(vram, pCharSelectData, sizeof(TCharScrVertex)*4);
	m_d3dCharSelectBuff->Unlock();
	free(pCharSelectData);
	
	D3DXMATRIX temp, temp2;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixTranslation(&temp2, 1.0f, 0.0f, 0.0f);
	D3DXMatrixOrthoLH(&m_d3dOrthoMatrix, 2.0f, 2.0f, -1.0f, 1.0f);
	D3DXMatrixMultiply(&temp, &temp2, &temp);
	D3DXMatrixMultiply(&m_d3dMatrix, &m_d3dOrthoMatrix, &temp);

	m_pInput->ClearPlayers(false);

	// Fading Varibles
	m_fFadingTotalTime = 1.0f;
	m_bisFadingDone = false;
	m_fFadingAmount = 0;
	m_fFadingElapsed = 0;
	m_fScreenPosition = 2.0f;
}	 

void CCharacterSelectState::Shutdown()
{
	if(m_d3dCharSelectBuff != nullptr)
		m_d3dCharSelectBuff->Release();

	for(int i = 0; i < 4; i++)
	{
		if(m_tPlayerIN[i].bPlayerIn)
		{
			m_nCounter++;
			m_nColors[i] = m_tPlayerIN[i].nPlayerColor;
		}
		else
			m_nColors[i] = -1;
	}
}
	 
bool CCharacterSelectState::Input()
{	 
	int nDevicePolled = -1;
	nDevicePolled = m_pInput->PollDevice();

	if(nDevicePolled == -2)
	{
			CInput::GetInstance()->ClearPlayers(true);
			CGame::GetInstance()->ChangeState(MAINMENU_STATE);
	}

	if(nDevicePolled != -1)
	{
		for(int i = 0; i < 4; i++)
		{
			if(!m_tPlayerIN[i].bPlayerIn)
			{
				m_tPlayerIN[i].bPlayerIn = true;
				m_bReadyScreen = false;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
				break;
			}
		}
	}
	


	for(int i = 0; i < 4; i++)
	{
		if(m_tPlayerIN[i].bPlayerIn)
		{
			m_tPlayerIN[i].fInputTimer += m_fTimer;

			if(m_pInput->CheckInput(BACK, i) && !m_tPlayerIN[i].bPlayerLocked && m_tPlayerIN[i].fInputTimer >= 0.5f)
			{
				m_tPlayerIN[i].bPlayerIn = false;
				m_tPlayerIN[i].fInputTimer = 0.0f;
				m_tPlayerIN[i].nPlayerColor = BLUE;
				m_tPlayerIN[i].bPlayerLocked = false;
				m_pInput->DropDevice(i);
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			}
			else if(m_pInput->CheckInput(BACK, i) && m_tPlayerIN[i].bPlayerLocked && m_tPlayerIN[i].fInputTimer >= 0.5f)
			{
				m_tPlayerIN[i].bPlayerLocked = false;
				m_bReadyScreen = false;
				m_tPlayerIN[i].fInputTimer = 0.0f;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(m_tPlayerIN[i].bPlayerIn && m_tPlayerIN[i].fInputTimer >= 1.0f && !m_tPlayerIN[i].bPlayerLocked)
		{
			if(m_pInput->CheckInput(UP, i) || m_pInput->CheckInput(LEFT, i))
			{
				m_tPlayerIN[i].nPlayerColor++;
				if(m_tPlayerIN[i].nPlayerColor > 3)
					m_tPlayerIN[i].nPlayerColor = 0;

				m_tPlayerIN[i].fInputTimer = 0.0f;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
			else if(m_pInput->CheckInput(DOWN, i) || m_pInput->CheckInput(RIGHT, i))
			{
				m_tPlayerIN[i].nPlayerColor--;
				if(m_tPlayerIN[i].nPlayerColor < 0)
					m_tPlayerIN[i].nPlayerColor = 3;

				m_tPlayerIN[i].fInputTimer = 0.0f;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(m_tPlayerIN[i].bPlayerIn && m_tPlayerIN[i].fInputTimer >= 1.0f && !m_bReadyScreen)
		{
			if(m_pInput->CheckInput(ACCEPT, i) && !m_tPlayerIN[i].bSameColor)
			{
				m_tPlayerIN[i].bPlayerLocked = true;
				m_tPlayerIN[i].fInputTimer = 0.0f;
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			}
		}
	}


	for(int i = 0; i < 4; i++)
	{
		if(m_pInput->CheckInput(ACCEPT, i) && m_bReadyScreen == true && m_tPlayerIN[i].fInputTimer >= 1.0f)
		{
			m_tPlayerIN[i].fInputTimer = 0.0f;
			
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_GAMESTART);
			CGame::GetInstance()->ChangeState(GAMEPLAY_STATE);
			m_pInput->TurnOnConfine();
			m_pInput->ResetWorldCursor();
			CGameplayState::GetInstance()->ResetPlayers(true);
			CGameplayState::GetInstance()->AddPlayers(m_nCounter);
			int tempCount = m_nCounter;
			CGameplayState::GetInstance()->LoadMap(tempCount);
			return true;
		}
	}

	bool bLeaveGame = true;
	for(unsigned int i = 0; i < 4; i++)
	{
		if(m_tPlayerIN[i].bPlayerIn)
			bLeaveGame = false;
	}


	
		if((m_pInput->CheckInput(PAUSE, 4) || m_pInput->CheckInput(BACK,4)) && bLeaveGame)
		{
			CInput::GetInstance()->ClearPlayers(true);
			CGame::GetInstance()->ChangeState(MAINMENU_STATE);
		}
	
	 return true;
}	 
	 
void CCharacterSelectState::Update( float fElapsedTime )
{	 
	 m_fTimer += fElapsedTime;

	 if(!m_bisFadingDone)
	 {
		 m_fFadingElapsed += fElapsedTime;
		 if(m_fFadingElapsed < m_fFadingTotalTime)
		 {
			 m_fFadingAmount = m_fFadingElapsed / m_fFadingTotalTime;
			 return;
		 }
		 if(m_fFadingElapsed >= m_fFadingTotalTime)
		 {
			 m_bisFadingDone = true;
		 }
	 }
		 

	 if(m_fTimer > 0.125f)
		 m_fTimer = 0.0f;

	 for(int i = 0; i < 4; i++)
	 {
		 if(m_tPlayerIN[i].bPlayerIn && m_tPlayerIN[i].bPlayerLocked)
		 {
			 for(int j = 0; j < 4; j++)
			 {
				 if(j == i)
					 continue;

				 if(m_tPlayerIN[j].bPlayerIn && !m_tPlayerIN[j].bPlayerLocked)
				 {
					 m_bReadyScreen = false;
					 break;
				 }
				 else
					 m_bReadyScreen = true;
			 }
		 }
	 }

	for(int i = 0; i < 4; i++)
	{
		bool bColorCheck = false;

		if(m_tPlayerIN[i].bPlayerIn)
		{
			for(int j = 0; j < 4; j++)
			{
				if(j == i)
					continue;

				if(m_tPlayerIN[i].nPlayerColor == m_tPlayerIN[j].nPlayerColor && m_tPlayerIN[j].bPlayerLocked)
				{
					m_tPlayerIN[i].bSameColor = true;
					bColorCheck = true;
					break;
				}
			}

			if(!bColorCheck)
				m_tPlayerIN[i].bSameColor = false;
		}
	}
}	 
	 
void CCharacterSelectState::Render()
{
	m_pD3D->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0);

	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}
	
}
void CCharacterSelectState::RenderNormalScene()
{
	m_pD3D->SetRenderState(D3DRS_ZENABLE, false);

	unsigned int passes3 = 0;
	m_d3dTempShader->Begin(&passes3,0);
	for(unsigned i(0);i < passes3; i++)
	{
		m_d3dTempShader->BeginPass(i);
		m_pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pD3D->SetStreamSource(0, m_d3dCharSelectBuff, sizeof(TCharScrVertex)*4, sizeof(TCharScrVertex));

		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		D3DXMatrixOrthoLH(&m_d3dOrthoMatrix, 2.0f, 2.0f, -1.0f, 1.0f);
		D3DXMatrixMultiply(&m_d3dMatrix, &m_d3dOrthoMatrix, &temp);
		m_d3dTempShader->SetMatrix("gViewProjection", &m_d3dMatrix);
		m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dJoinGameTexture);
		m_d3dTempShader->SetInt("gGreyScale", 0);
		m_d3dTempShader->SetInt("gLocked", 0);
		m_d3dTempShader->SetVector("gColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		m_d3dTempShader->CommitChanges();
		m_pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		m_d3dTempShader->EndPass();
	}
	m_d3dTempShader->End();


	for(int i = 0; i < 4; i++)
	{
		unsigned int passes = 0;
		m_d3dTempShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			m_d3dTempShader->BeginPass(i);
			m_pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
			m_pD3D->SetStreamSource(0, m_d3dCharSelectBuff, 0, sizeof(TCharScrVertex));

			for(unsigned int i = 0; i < 4; i++)
			{
				D3DXMATRIX temp, temp2, scaleMat;
				D3DXMatrixIdentity(&temp);

				if(i == 0 || i == 1)
					D3DXMatrixTranslation(&temp2, (float)i - 0.307f, 0.116f, 0.0f);
				else if(i == 2)
					D3DXMatrixTranslation(&temp2, -0.307f, -0.89f, 0.0f);
				else
					D3DXMatrixTranslation(&temp2, 0.693f, -0.89f, 0.0f);

				D3DXMatrixScaling(&scaleMat, 0.39f, 0.78f, 1.0f);
				D3DXMatrixOrthoLH(&m_d3dOrthoMatrix, 2.0f, 2.0f, -1.0f, 1.0f);
				D3DXMatrixMultiply(&temp2, &scaleMat, &temp2);
				D3DXMatrixMultiply(&temp, &temp2, &temp);
				D3DXMatrixMultiply(&m_d3dMatrix, &m_d3dOrthoMatrix, &temp);
				m_d3dTempShader->SetMatrix("gViewProjection", &m_d3dMatrix);

				if(m_tPlayerIN[i].bPlayerIn == false)
				{
					m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dPressStartTexture);
				}
				else
				{
					switch(m_tPlayerIN[i].nPlayerColor)
					{
					case BLUE:
						{
						m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dCharBlueTexture);
						m_d3dTempShader->SetVector("gColor", &D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
						}
						break;
					case RED:
						{
						m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dCharRedTexture);
						m_d3dTempShader->SetVector("gColor", &D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
						}
						break;
					case GREEN:
						{
						m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dCharGreenTexture);
						m_d3dTempShader->SetVector("gColor", &D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
						}
						break;
					case YELLOW:
						{
						m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dCharYellowTexture);
						m_d3dTempShader->SetVector("gColor", &D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f));
						}
						break;
					}
				}

				if(m_tPlayerIN[i].bSameColor)
					m_d3dTempShader->SetInt("gGreyScale", 1);
				else
					m_d3dTempShader->SetInt("gGreyScale", 0);

				if(m_tPlayerIN[i].bPlayerLocked)
					m_d3dTempShader->SetInt("gLocked", 1);
				else
					m_d3dTempShader->SetInt("gLocked", 0);

				m_d3dTempShader->CommitChanges();
				m_pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
			}

			m_d3dTempShader->EndPass();
		}
		
		m_d3dTempShader->End();
	}

	if(m_bReadyScreen)
	{
		unsigned int passes2 = 0;
		m_d3dTempShader->Begin(&passes2,0);
		for(unsigned i(0);i < passes2; i++)
		{
			m_d3dTempShader->BeginPass(i);

			m_pD3D->SetStreamSource(0, m_d3dCharSelectBuff, 0, sizeof(TCharScrVertex));

			D3DXMATRIX tempMat, temp;
			D3DXMatrixIdentity(&temp);
			D3DXMatrixTranslation(&tempMat, 0.5f, -0.5f, 0.0f);
			D3DXMatrixOrthoLH(&m_d3dOrthoMatrix, 2.0f, 2.0f, -1.0f, 1.0f);
			D3DXMatrixMultiply(&temp, &tempMat, &temp);
			D3DXMatrixMultiply(&m_d3dMatrix, &m_d3dOrthoMatrix, &temp);
			m_d3dTempShader->SetMatrix("gViewProjection", &m_d3dMatrix);
			m_d3dTempShader->SetTexture("gDiffuseTexture", m_d3dAllReadyTexture);
			m_d3dTempShader->SetInt("gGreyScale", 0);
			m_d3dTempShader->SetInt("gLocked", 0);
			m_d3dTempShader->CommitChanges();
			m_pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		
			m_d3dTempShader->EndPass();
		}
		m_d3dTempShader->End();
	}

	m_pD3D->SetRenderState(D3DRS_ZENABLE, true);
}
void CCharacterSelectState::RenderFadingScene()
{
	m_pD3D->SetRenderState(D3DRS_ZENABLE, false);

	unsigned int passes3 = 0;
	m_d3dBGShader->Begin(&passes3,0);
	for(unsigned i(0);i < passes3; i++)
	{
		m_d3dBGShader->BeginPass(i);
		m_pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		m_pD3D->SetStreamSource(0, m_d3dCharSelectBuff, sizeof(TCharScrVertex)*4, sizeof(TCharScrVertex));

		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		D3DXMatrixOrthoLH(&m_d3dOrthoMatrix, 2.0f, 2.0f, -1.0f, 1.0f);
		D3DXMatrixMultiply(&m_d3dMatrix, &m_d3dOrthoMatrix, &temp);
		m_d3dBGShader->SetMatrix("gViewProjection", &m_d3dMatrix);
		m_d3dBGShader->SetTexture("gDiffuseTexture", m_d3dJoinGameTexture);
		m_d3dBGShader->SetFloat("fadeAmount",m_fFadingAmount);

		m_d3dBGShader->CommitChanges();
		m_pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		m_d3dBGShader->EndPass();
	}
	m_d3dBGShader->End();

	m_pD3D->SetRenderState(D3DRS_ZENABLE, true);
}