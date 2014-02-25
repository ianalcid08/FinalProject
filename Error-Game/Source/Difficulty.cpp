/***********************************************
 * Filename:  		Difficulty.cpp
 * Date:      		04/08/2013
 * Mod. Date: 		04/08/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		State in which the difficulty is set for all levels.
 ************************************************/
#include "Difficulty.h"
#include "Game.h"
#include "GameplayState.h"
#include "Input.h"
#include "Renderer.h"
#include "BitmapFont.h"
#include "AssetManager.h"
#include "Definitions.h"
#include "Audio.h"

CDifficulty* CDifficulty::GetInstance()
{
	static CDifficulty s_Instance;

	return &s_Instance;
}
CDifficulty::CDifficulty(void)
{
}


CDifficulty::~CDifficulty(void)
{
}
void CDifficulty::Initialize()
{
	m_pInput = CInput::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pBMF = CBitmapFont::GetInstance();
	m_pD3D = m_pRenderer->GetDirect3DDevice();
	m_pAM = CAssetManager::GetInstance();

	m_d3dShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	// TODO - REPLACE THIS WITH ACTUAL GOOD BACKGROUND
	// JUST A PLACEHOLDER
	d3dDifficultyBG = m_pAM->LoadTexture("Textures/OptionsMenu.png"); 

	// Verts for the screen
	tDiffVertexs[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	tDiffVertexs[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	tDiffVertexs[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	tDiffVertexs[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	tDiffVertexs[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	tDiffVertexs[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	tDiffVertexs[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	tDiffVertexs[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TDifficultyVertex)*4,0,0,D3DPOOL_MANAGED,&m_d3dDifficultyBuff,0);
	void* vram = nullptr;
	m_d3dDifficultyBuff->Lock(0,0,&vram,0);
	memcpy(vram,tDiffVertexs,sizeof(TDifficultyVertex)*4);
	m_d3dDifficultyBuff->Unlock();

	m_nfirstBoxX = 305;
	m_nfirstBoxY = 380;
	m_nletterHeight = 32;
	m_nletterWidth = 32;
	m_fLetterScale = 0.5f;


	// Fading Varibles
	m_fFadingTotalTime = 1.0f;
	m_bisFadingDone = false;
	m_fFadingAmount = 0;
	m_fFadingElapsed = 0;

	m_nChoice = NORMAL;
	m_nDifficulty = NORMAL;
	m_fTimer = 0.0f;

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
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TDifficultyVertex)*4,0,0,D3DPOOL_MANAGED,&d3dCursorBuffer,0);
	vram = nullptr;
	d3dCursorBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tCursorVertex,sizeof(TDifficultyVertex)*4);
	d3dCursorBuffer->Unlock();
}
void CDifficulty::Shutdown()
{
	if(d3dCursorBuffer)
		d3dCursorBuffer->Release();
	if(m_d3dDifficultyBuff)
		m_d3dDifficultyBuff->Release();
}
bool CDifficulty::Input()
{
	if(m_fTimer < 0.25f)
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

	if(m_pInput->CheckInput(DOWN,4))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice++;
		m_fTimer = 0.0f;
		if(m_nChoice == NUM_DIFF + 1)
			m_nChoice = NORMAL;
	}
	if(m_pInput->CheckInput(UP,4))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice--;
		m_fTimer = 0.0f;
		if(m_nChoice == 0)
			m_nChoice = 3; // (back button
	}

	// if user hits enter / accept
	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB())
	{
		if(m_nChoice == 3)
		{
			CGame::GetInstance()->ChangeState(MAINMENU_STATE);
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			return true;
		}
		CGame::GetInstance()->SetDifficulty(m_nChoice); // = m_nChoice;
		CGame::GetInstance()->ChangeState(CHAR_SELECT_STATE);
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
		return true;
	}

	if(moveMouse)
	{
		//min x			// max x								// min	y				// max y
		if(((m_fMouseX >= -0.384 && m_fMouseX <= -0.110f) && (m_fMouseY <= -0.1f && m_fMouseY >= -0.163889f)))
		{
			m_nChoice = NORMAL;
		}
		if(((m_fMouseX >= -0.3906 && m_fMouseX <= -0.1046f) && (m_fMouseY <= -0.1889f && m_fMouseY >= -0.2527f)))
		{
			m_nChoice = HARD;
		}
		if(((m_fMouseX >= -0.389 && m_fMouseX <= -0.2078f) && (m_fMouseY <= -0.275f && m_fMouseY >= -0.3388f)))
		{
			m_nChoice = 3;
		}
		/*if(((m_fMouseX >= -0.3875 && m_fMouseX <= -0.209f) && (m_fMouseY <= -0.3638f && m_fMouseY >= -0.4305f)))
		{
			m_nChoice = 4;
		}*/
	}
	if(m_pInput->CheckInput(PAUSE, 4))  //GetAsyncKeyState(VK_ESCAPE)&1)
	{
		if(m_nChoice == 3)
			CGame::GetInstance()->ChangeState(MAINMENU_STATE);
		m_nChoice = 3;
	}

	if(GetAsyncKeyState('7') && 0x01)
		cout << "Mouse PosX = " << m_fMouseX << " Mouse PosY = " << m_fMouseY << endl;

	return true;
}
void CDifficulty::Update( float fElapsedTime )
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
void CDifficulty::Render()
{
	m_pRenderer->GetSprite()->Flush();
	m_pRenderer->Clear();

	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}
}
void CDifficulty::RenderNormalScene()
{
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	unsigned int passes(0);
	m_d3dShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		m_d3dShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,m_d3dDifficultyBuff,0,sizeof(TDifficultyVertex));
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		m_d3dShader->SetMatrix("gViewProjection",&temp);
		m_d3dShader->SetTexture("gDiffuseTexture",d3dDifficultyBG);
		m_d3dShader->SetFloat("fadeAmount",1.0f);

		m_d3dShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		m_d3dShader->EndPass();
	}
	m_d3dShader->End();
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
			m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TDifficultyVertex));
			d3dCursorShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
			d3dCursorShader->SetFloat("gFade", 1.0f);
			d3dCursorShader->SetTexture("gDiffuseTexture", d3dCursorTexture);
			d3dCursorShader->SetFloat("gRatio", 1.0f);
			d3dCursorShader->SetMatrix("gOffset", &mouseCursor);
			d3dCursorShader->CommitChanges();

			m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TDifficultyVertex));
			m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			
			d3dCursorShader->EndPass();
		}
		d3dCursorShader->End();

	// x - 305         y - 380
		float tempx, tempy;
		tempx = m_nfirstBoxX / 1024.0f;
		tempy = m_nfirstBoxY / 768.0f;
		float tempWidth = CRenderer::GetInstance()->GetWindowWidth() * tempx;
		float tempHeight = CRenderer::GetInstance()->GetWindowHeight() * tempy;
		int windowHeight = CRenderer::GetInstance()->GetWindowHeight();
	m_pBMF->PrintBF("Difficulty",(int)tempWidth,(int)(tempHeight),m_fLetterScale);
	switch(m_nChoice)
	{
	case NORMAL:
		{
			m_pBMF->PrintBF("Normal",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight) / 768.0f) * windowHeight),m_fLetterScale,0xff000000);
			m_pBMF->PrintBF("Hard",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 2) / 768.0f) * windowHeight),m_fLetterScale);
			m_pBMF->PrintBF("Back",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 3) / 768.0f) * windowHeight),m_fLetterScale);
		}
		break;
	case HARD:
		{
			m_pBMF->PrintBF("Normal",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight)/ 768.0f) * windowHeight),m_fLetterScale);
			m_pBMF->PrintBF("Hard",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 2)/ 768.0f) * windowHeight),m_fLetterScale,0xff000000);
			m_pBMF->PrintBF("Back",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 3)/ 768.0f) * windowHeight),m_fLetterScale);
		}
		break;
	case 3:
		{
			m_pBMF->PrintBF("Normal",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight) / 768.0f) * windowHeight),m_fLetterScale);
			m_pBMF->PrintBF("Hard",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 2)/ 768.0f) * windowHeight),m_fLetterScale);
			m_pBMF->PrintBF("Back",(int)tempWidth,(int)(((m_nfirstBoxY + m_nletterHeight * 3)/ 768.0f) * windowHeight),m_fLetterScale,0xff000000);
		}
	}

};


void CDifficulty::RenderFadingScene()
{
		DWORD srcBlend,destBlend;
	m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
	m_pRenderer->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND,&destBlend);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	unsigned int passes(0);
	m_d3dShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		m_d3dShader->BeginPass(i);
		m_pRenderer->GetDirect3DDevice()->SetStreamSource(0,m_d3dDifficultyBuff,0,sizeof(TDifficultyVertex));
		m_pRenderer->GetDirect3DDevice()->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		m_d3dShader->SetMatrix("gViewProjection",&temp);
		m_d3dShader->SetTexture("gDiffuseTexture",d3dDifficultyBG);
		m_d3dShader->SetFloat("fadeAmount",m_fFadingAmount);

		m_d3dShader->CommitChanges();
		m_pRenderer->GetDirect3DDevice()->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		m_d3dShader->EndPass();
	}
	m_d3dShader->End();
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,srcBlend);
	m_pRenderer->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,destBlend);
}