#include "PauseState.h"

/***********************************************
* Filename:  		PauseState.cpp
* Date:      		02/08/2013
* Mod. Date: 		03/18/2013
* Mod. Initials:	IA
* Author:    		Joseph Thomas
* Purpose:   		State for handling pause menu
************************************************/
#include "Input.h"
#include "Game.h"
#include "Renderer.h"
#include <string>
#include "Audio.h"
using namespace std;

CPauseState* CPauseState::GetInstance( void )
{
	static CPauseState s_Instance;

	return &s_Instance;
}

CPauseState::CPauseState(void)
{

}

CPauseState::~CPauseState(void)
{

}

void CPauseState::Initialize()
{	 
	m_pInput = CInput::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
	m_pBMF = CBitmapFont::GetInstance();
	m_pAM = CAssetManager::GetInstance();

	m_nChoice = 1;
	slowdown = 0;
	m_fTimer = 0.0f;

	d3dPauseBG = m_pAM->LoadTexture("Textures/PauseScreen.png");
	m_rPauseBGRect.left = 0;
	m_rPauseBGRect.top = 0;
	m_rPauseBGRect.right = 2048;
	m_rPauseBGRect.bottom = 2048;
	m_nfirstBoxX = 410;
	m_nfirstBoxY = 300;
	m_nletterHeight = 32;
	m_nletterWidth = 32;
	m_fLetterScale = 0.35f;

	m_tPauseVerts[0].d3dPosition = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	m_tPauseVerts[1].d3dPosition = D3DXVECTOR3(1.0f,1.0f,0.0f);
	m_tPauseVerts[2].d3dPosition = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	m_tPauseVerts[3].d3dPosition = D3DXVECTOR3(-1.0f,-1.0f,0.0f);

	m_tPauseVerts[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	m_tPauseVerts[1].d3dUV = D3DXVECTOR2(1.0f,0.0f);
	m_tPauseVerts[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	m_tPauseVerts[3].d3dUV = D3DXVECTOR2(0.0f,1.0f);

	m_d3dPauseShader = m_pAM->LoadShader("Shaders/StateFading.fx");
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TPauseVertex)*4,0,0,D3DPOOL_MANAGED,&m_d3dPauseBuff,0);
	void* vram = nullptr;
	m_d3dPauseBuff->Lock(0,0,&vram,0);
	memcpy(vram,m_tPauseVerts,sizeof(TPauseVertex)*4);
	m_d3dPauseBuff->Unlock();

	D3DXMATRIX tempMat, temp2;
	D3DXMatrixIdentity(&tempMat);
	D3DXMatrixTranslation(&temp2, 1.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&tempMat, &temp2, &tempMat);

	// Setting up the fading variables
	this->m_bisFadingDone = false;
	this->m_fFadingAmount = 0; // this is a percent
	this->m_fFadingElapsed = 0;
	this->m_fFadingTotalTime = 0.5f;

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
	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TPauseVertex)*4,0,0,D3DPOOL_MANAGED,&d3dCursorBuffer,0);
	vram = nullptr;
	d3dCursorBuffer->Lock(0,0,&vram,0);
	memcpy(vram,tCursorVertex,sizeof(TPauseVertex)*4);
	d3dCursorBuffer->Unlock();

	m_fMouseX = m_pInput->GetScreenMouseX();
	m_fMouseY = m_pInput->GetScreenMouseY();
}	 

void CPauseState::Shutdown()
{
	if(m_d3dPauseBuff)
		m_d3dPauseBuff->Release();
	if(d3dCursorBuffer)
		d3dCursorBuffer->Release();
}

bool CPauseState::Input()
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

	if(m_pInput->CheckInput(ACCEPT,4) || m_pInput->GetLMB()) // enter
	{
		m_fTimer = 0.0f;
		if(m_nChoice == 1)
		{
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			m_pGame->PopState();
			m_pInput->TurnOnConfine();
			m_pInput->ResetWorldCursor();
			return true;
		}
		if(m_nChoice == 2)
		{
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			m_pGame->PushState(OPTIONS_STATE);
			return true;
		}

		if(m_nChoice == 3)
		{
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
			m_pGame->ChangeState(MAINMENU_STATE);
			return true;
		}

	}
	if(m_pInput->CheckInput(PAUSE,4)) // press ESC again
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUSELECT);
		m_fTimer = 0.0f;
		m_pGame->PopState();
		return true;
	}
	if((m_pInput->CheckInput(DOWN,4)))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice++;
		m_fTimer = 0.0f;
		if(m_nChoice > 3)
			m_nChoice = 1;
	}
	else if(m_pInput->CheckInput(UP,4))
	{
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
		m_nChoice--;
		m_fTimer = 0.0f;
		if(m_nChoice == 0)
			m_nChoice = 3;
	}

	if(moveMouse)
	{
		if(((m_fMouseX >= -0.25f && m_fMouseX <= -0.064f) && (m_fMouseY <= 0.1583f && m_fMouseY >= 0.1111f)))
		{
			if(m_nChoice != 1)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = 1;
			}
		}
		if(((m_fMouseX >= -0.25f && m_fMouseX <= -0.02656f) && (m_fMouseY <= 0.0694f && m_fMouseY >= 0.0305f)))
		{
			if(m_nChoice != 2)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = 2;
			}
		}
		if(((m_fMouseX >= -0.25f && m_fMouseX <= 0.0406f) && (m_fMouseY <= -0.0222f && m_fMouseY >= -0.0555f)))
		{
			if(m_nChoice != 3)
			{
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_MENUCHANGE);
				m_nChoice = 3;
			}
		}
	}

#ifdef _DEBUG
	if(GetAsyncKeyState('7') && 0x01)
		cout << "Mouse PosX = " << m_fMouseX << " Mouse PosY = " << m_fMouseY << endl;
#endif
	return true;
}	 

void CPauseState::Update( float fElapsedTime )
{	 
	m_fTimer += fElapsedTime;
	if(!m_bisFadingDone)
	{
		m_fFadingElapsed += fElapsedTime;
		if(m_fFadingElapsed < m_fFadingTotalTime)
		{
			m_fFadingAmount = m_fFadingElapsed / m_fFadingTotalTime;
		}
		if(m_fFadingElapsed == m_fFadingTotalTime)
			m_bisFadingDone = true;
	}

}	 

void CPauseState::Render()
{	 
	// NEW RENDER CODE
	if(m_bisFadingDone)
	{
		RenderNormalScene();
	}
	else
	{
		RenderFadingScene();
	}
}
void CPauseState::RenderNormalScene()
{	 
	IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
	unsigned int passes(0);
	m_d3dPauseShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		m_d3dPauseShader->BeginPass(i);
		pD3D->SetStreamSource(0,m_d3dPauseBuff,0,sizeof(TPauseVertex));

			D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		m_d3dPauseShader->SetMatrix("gViewProjection",&temp);
		m_d3dPauseShader->SetTexture("gDiffuseTexture",d3dPauseBG);
		m_d3dPauseShader->SetFloat("fadeAmount",1.0f);

		m_d3dPauseShader->CommitChanges();
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		m_d3dPauseShader->EndPass();
	}
	m_d3dPauseShader->End();
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

	// Render the mouse stuff
	// Updating the Mouse positions
	passes = 0;
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

		pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		pD3D->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TPauseVertex));
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();
	// x = 410 y = 300
	float tempx = CGame::GetInstance()->GetScreenWidth() * .40039f;
	float tempy = CGame::GetInstance()->GetScreenHeight() * .338541f;
	m_pBMF->PrintBF("Paused",(int)tempx,(int)tempy,m_fLetterScale);
	if(m_nChoice == 1)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .390625f;
		m_pBMF->PrintBF("Resume",(int)tempxx,int(sin(m_fTimer * 3.0f) * 5.0f + tempyy),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .390625f;
		m_pBMF->PrintBF("Resume",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
	if(m_nChoice == 2)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .432292f;
		m_pBMF->PrintBF("Options",(int)tempxx ,int(sin(m_fTimer * 3.0f) * 5.0f + (tempyy)),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .432292f;
		m_pBMF->PrintBF("Options",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
	if(m_nChoice == 3)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .473958f;
		m_pBMF->PrintBF("Main Menu",(int)tempxx,int(sin(m_fTimer * 3.0f) * 5.0f + (tempyy)),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .473958f;
		m_pBMF->PrintBF("Main Menu",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
}	 

void CPauseState::RenderFadingScene()
{
	IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();
	DWORD srcBlend,destBlend;
	pD3D->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
	pD3D->GetRenderState(D3DRS_DESTBLEND,&destBlend);
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pD3D->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
	unsigned int passes(0);
	m_d3dPauseShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		m_d3dPauseShader->BeginPass(i);
		pD3D->SetStreamSource(0,m_d3dPauseBuff,0,sizeof(TPauseVertex));

		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);

		m_d3dPauseShader->SetMatrix("gViewProjection",&temp);
		m_d3dPauseShader->SetTexture("gDiffuseTexture",d3dPauseBG);
		m_d3dPauseShader->SetFloat("fadeAmount",m_fFadingAmount);

		m_d3dPauseShader->CommitChanges();
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		m_d3dPauseShader->EndPass();
	}
	m_d3dPauseShader->End();
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	pD3D->SetRenderState(D3DRS_SRCBLEND,srcBlend);
	pD3D->SetRenderState(D3DRS_DESTBLEND,destBlend);

	passes = 0;
	D3DXMATRIX mouseCursor,cursorScale;
	D3DXMatrixIdentity(&mouseCursor);
	D3DXMatrixScaling(&cursorScale,0.1f,0.1f,0.1f);
	mouseCursor *= cursorScale;
	D3DXMatrixTranslation(&mouseCursor,m_fMouseX,m_fMouseY,0.0f);

	d3dCursorShader->Begin(&passes,0);
	for(unsigned i(0); i < passes; i++)
	{
		d3dCursorShader->BeginPass(i);
		pD3D->SetStreamSource(0,d3dCursorBuffer,0,sizeof(TPauseVertex));
		pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(POS_TEXCOORD));
		d3dCursorShader->SetTexture("gDiffuseTexture", d3dCursorTexture);
		d3dCursorShader->SetFloat("gRatio", 1.0f);
		d3dCursorShader->SetMatrix("gOffset", &mouseCursor);
		d3dCursorShader->CommitChanges();

		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dCursorShader->EndPass();
	}
	d3dCursorShader->End();

	m_pRenderer->GetSprite()->Flush();
	/*m_pBMF->PrintBF("Paused",m_nfirstBoxX,m_nfirstBoxY - 40,m_fLetterScale);
	if(m_nChoice == 1)
		m_pBMF->PrintBF("Resume",m_nfirstBoxX,int(sin(m_fTimer * 3.0f) * 5.0f + m_nfirstBoxY),m_fLetterScale);
	else
		m_pBMF->PrintBF("Resume",m_nfirstBoxX,m_nfirstBoxY,m_fLetterScale);
	if(m_nChoice == 2)
		m_pBMF->PrintBF("Options",m_nfirstBoxX ,int(sin(m_fTimer * 3.0f) * 5.0f + (m_nfirstBoxY+m_nletterHeight)),m_fLetterScale);
	else
		m_pBMF->PrintBF("Options",m_nfirstBoxX,m_nfirstBoxY+m_nletterHeight,m_fLetterScale);
	if(m_nChoice == 3)
		m_pBMF->PrintBF("Main Menu",m_nfirstBoxX,int(sin(m_fTimer * 3.0f) * 5.0f + (m_nfirstBoxY+m_nletterHeight*2)),m_fLetterScale);
	else
		m_pBMF->PrintBF("Main Menu",m_nfirstBoxX,(m_nfirstBoxY+m_nletterHeight*2),m_fLetterScale);*/
		float tempx = CGame::GetInstance()->GetScreenWidth() * .40039f;
	float tempy = CGame::GetInstance()->GetScreenHeight() * .338541f;
	m_pBMF->PrintBF("Paused",(int)tempx,(int)tempy,m_fLetterScale);
	if(m_nChoice == 1)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .390625f;
		m_pBMF->PrintBF("Resume",(int)tempxx,int(sin(m_fTimer * 3.0f) * 5.0f + tempyy),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .390625f;
		m_pBMF->PrintBF("Resume",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
	if(m_nChoice == 2)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .432292f;
		m_pBMF->PrintBF("Options",(int)tempxx ,int(sin(m_fTimer * 3.0f) * 5.0f + (tempyy)),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .432292f;
		m_pBMF->PrintBF("Options",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
	if(m_nChoice == 3)
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .473958f;
		m_pBMF->PrintBF("Main Menu",(int)tempxx,int(sin(m_fTimer * 3.0f) * 5.0f + (tempyy)),m_fLetterScale);
	}
	else
	{
		float tempxx = CGame::GetInstance()->GetScreenWidth() * .40039f;
		float tempyy = CGame::GetInstance()->GetScreenHeight() * .473958f;
		m_pBMF->PrintBF("Main Menu",(int)tempxx,(int)tempyy,m_fLetterScale);
	}
}