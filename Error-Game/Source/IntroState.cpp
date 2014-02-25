#include "IntroState.h"

/***********************************************
 * Filename:  		IntroState.cpp
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling game introduction
 ************************************************/
#include "GameplayState.h"
#include "AssetManager.h"
#include "Game.h"
#include "Camera.h"
#include "Input.h"

CIntroState* CIntroState::GetInstance( void )
{
	static CIntroState s_Instance;

	return &s_Instance;
}

CIntroState::CIntroState(void)
{
	
}

CIntroState::~CIntroState(void)
{

}

void CIntroState::Initialize()
{	 
	 m_pRenderer = CRenderer::GetInstance();
	 m_pTextures[INTRO_BEGIN] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro0.png");
	 m_pTextures[INTRO_MOVE] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro1.png");
	 m_pTextures[INTRO_SHOOT] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro2.png");
	 m_pTextures[INTRO_DASH] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro3.png");
	 m_pTextures[INTRO_SPECIAL] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro4.png");
	 m_pTextures[INTRO_COMPUTE] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro5.png");
	 m_pTextures[INTRO_AWARE] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro6.png");
	 m_pTextures[INTRO_DESTROY] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\Intro7.png");
	 m_pTextures[CORRECT_MARK] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\CorrectMark.png");
	 m_pTextures[WRONG_MARK] = CAssetManager::GetInstance()->LoadTexture("TestFiles\\WrongMark.png");

	 // Room Vects
	 TTileVert twoRects[8];
	 *(D3DXVECTOR3*)&twoRects[0].m_fVerts = D3DXVECTOR3(-1, 0, 1.0f);
	 *(D3DXVECTOR3*)&twoRects[1].m_fVerts = D3DXVECTOR3(46.0f, 0, 1.0f);
	 *(D3DXVECTOR3*)&twoRects[2].m_fVerts = D3DXVECTOR3(46.0f, 0, -19.0f);
	 *(D3DXVECTOR3*)&twoRects[3].m_fVerts = D3DXVECTOR3(-1.0f, 0, -19.0f);
	 *(D3DXVECTOR2*)&twoRects[0].m_fUVs = D3DXVECTOR2(0, 0);
	 *(D3DXVECTOR2*)&twoRects[1].m_fUVs = D3DXVECTOR2(1.0f, 0);
	 *(D3DXVECTOR2*)&twoRects[2].m_fUVs = D3DXVECTOR2(1.0f, 1.0f);
	 *(D3DXVECTOR2*)&twoRects[3].m_fUVs = D3DXVECTOR2(0, 1.0f);

	 *(D3DXVECTOR3*)&twoRects[4].m_fVerts = D3DXVECTOR3(10.0f, -.95f, 0.0f);
	 *(D3DXVECTOR3*)&twoRects[5].m_fVerts = D3DXVECTOR3(35.0f, -.95f, 0.0f);
	 *(D3DXVECTOR3*)&twoRects[6].m_fVerts = D3DXVECTOR3(35.0f, -.95f, -12.0f);
	 *(D3DXVECTOR3*)&twoRects[7].m_fVerts = D3DXVECTOR3(10.0f, -.95f, -12.0f);
	 *(D3DXVECTOR2*)&twoRects[4].m_fUVs = D3DXVECTOR2(0, 0);
	 *(D3DXVECTOR2*)&twoRects[5].m_fUVs = D3DXVECTOR2(1.0f, 0);
	 *(D3DXVECTOR2*)&twoRects[6].m_fUVs = D3DXVECTOR2(1.0f, 1.0f);
	 *(D3DXVECTOR2*)&twoRects[7].m_fUVs = D3DXVECTOR2(0, 1.0f);

	  //Marker Rect
	 TTileVert MarkerRect[4];
	 *(D3DXVECTOR3*)&MarkerRect[0].m_fVerts = D3DXVECTOR3(11.0f, 0.25f, -1.8f);
	 *(D3DXVECTOR3*)&MarkerRect[1].m_fVerts = D3DXVECTOR3(34.0f, 0.25f, -1.8f);
	 *(D3DXVECTOR3*)&MarkerRect[2].m_fVerts = D3DXVECTOR3(34.0f, 0.25f, -11.8f);
	 *(D3DXVECTOR3*)&MarkerRect[3].m_fVerts = D3DXVECTOR3(11.0f, 0.25f, -11.8f);
	 *(D3DXVECTOR2*)&MarkerRect[0].m_fUVs = D3DXVECTOR2(0, 0);
	 *(D3DXVECTOR2*)&MarkerRect[1].m_fUVs = D3DXVECTOR2(1.0f, 0);
	 *(D3DXVECTOR2*)&MarkerRect[2].m_fUVs = D3DXVECTOR2(1.0f, 1.0f);
	 *(D3DXVECTOR2*)&MarkerRect[3].m_fUVs = D3DXVECTOR2(0, 1.0f);


	m_pRenderer->GetDirect3DDevice()->CreateVertexBuffer(sizeof(TTileVert)*12, 0, 0, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);

	void* vram;
	m_pVertexBuffer->Lock(0, 0, &vram, 0);
	memcpy(vram, twoRects, sizeof(TTileVert) * 8);
	m_pVertexBuffer->Unlock();

	m_pVertexBuffer->Lock(sizeof(TTileVert)*8, sizeof(TTileVert)*4, &vram, 0);
	memcpy(vram, MarkerRect, sizeof(TTileVert)*4);
	m_pVertexBuffer->Unlock();

	m_fUpdateTime = 0.0f;
	m_nCurrTexture = 0;

	m_bTutorial = false;
	m_bSkipTutorial = false;
	m_nCorrect = 0;
	m_fMarkTimer = 0.0f;
}	 
	 
void CIntroState::Shutdown()
{
	if(m_pVertexBuffer)
		m_pVertexBuffer->Release();
}
	 
bool CIntroState::Input()
{	 
	 return true;
}	 
	 
void CIntroState::Update( float fElapsedTime )
{
	 m_fUpdateTime += fElapsedTime;

	 if(m_fMarkTimer > 0.0f)
		 m_fMarkTimer -= fElapsedTime;

	 if(m_fMarkTimer <= 0.0f)
		 m_nCorrect = 0;

	 if(m_bSkipTutorial)
	 {
		 m_nCurrTexture = INTRO_AWARE;
		 m_fUpdateTime = 2.6f;
	 }

	 if(m_nCurrTexture < INTRO_AWARE)
	 {
		 if(m_nCurrTexture == INTRO_MOVE && m_bTutorial && m_fMarkTimer <= 0.0f)
		{
			for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
			{
				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetInvulernable() ||
					CGameplayState::GetInstance()->GetPlayers()[i]->GetDying() )
					continue;

				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);

				if(CInput::GetInstance()->CheckInput(UP, i) || CInput::GetInstance()->CheckInput(DOWN, i) ||
					CInput::GetInstance()->CheckInput(LEFT, i) || CInput::GetInstance()->CheckInput(RIGHT, i) ||
					CInput::GetInstance()->CheckInput(UP_LEFT, i) || CInput::GetInstance()->CheckInput(UP_RIGHT, i) || 
					CInput::GetInstance()->CheckInput(DOWN_LEFT, i) || CInput::GetInstance()->CheckInput(DOWN_RIGHT, i))
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(true);
					m_nCorrect = 1;
					m_fUpdateTime = 2.5f;
					m_fMarkTimer = 1.5f;
					break;
				}

				D3DXVECTOR3 d3dtemp;

				if(CInput::GetInstance()->CheckInput(SHOOT_UP, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_UP_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_UP_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_DOWN_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(DASH, i) || CInput::GetInstance()->CheckInput(SPECIAL, i) ||
					CInput::GetInstance()->GetShootDir(i, CGameplayState::GetInstance()->GetPlayers()[i]->GetMatrix(), d3dtemp) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(false);
					m_nCorrect = 2;
					m_fMarkTimer = 1.5f;
					break;
				}
			}
		}

		if(m_nCurrTexture == INTRO_SHOOT && m_bTutorial && m_fMarkTimer <= 0.0f)
		{
			for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
			{
				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetInvulernable() ||
					CGameplayState::GetInstance()->GetPlayers()[i]->GetDying() )
					continue;

				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);

				if(CInput::GetInstance()->CheckInput(DASH, i) || CInput::GetInstance()->CheckInput(SPECIAL, i))
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(false);
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);
					m_nCorrect = 2;
					m_fMarkTimer = 1.5f;
					break;
				}

				D3DXVECTOR3 d3dtemp;

				if(CInput::GetInstance()->CheckInput(SHOOT_UP, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_UP_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_UP_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_DOWN_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN_RIGHT, i) ||
					CInput::GetInstance()->GetShootDir(i, CGameplayState::GetInstance()->GetPlayers()[i]->GetMatrix(), d3dtemp) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(true);
					m_nCorrect = 1;
					m_fUpdateTime = 2.5f;
					m_fMarkTimer = 1.5f;
					break;
				}
			}
		}

		if(m_nCurrTexture == INTRO_DASH && m_bTutorial && m_fMarkTimer <= 0.0f)
		{
			for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
			{
				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetInvulernable() || 
					CGameplayState::GetInstance()->GetPlayers()[i]->GetDying())
					continue;

				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);

				D3DXVECTOR3 d3dtemp;
				if( CInput::GetInstance()->CheckInput(SHOOT_UP, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_UP_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_UP_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_DOWN_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN_RIGHT, i) ||
					CInput::GetInstance()->GetShootDir(i, CGameplayState::GetInstance()->GetPlayers()[i]->GetMatrix(), d3dtemp) ||
					CInput::GetInstance()->CheckInput(SPECIAL, i) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(false);
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);
					m_nCorrect = 2;
					m_fMarkTimer = 1.5f;
					break;
				}

				
				if( CInput::GetInstance()->CheckInput(DASH, i) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(true);
					m_nCorrect = 1;
					m_fUpdateTime = 2.5f;
					m_fMarkTimer = 1.5f;
					break;
				}
			}
		}

		if(m_nCurrTexture == INTRO_SPECIAL && m_bTutorial && m_fMarkTimer <= 0.0f)
		{
			for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
			{
				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetInvulernable() || 
					CGameplayState::GetInstance()->GetPlayers()[i]->GetDying())
					continue;

				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);

				D3DXVECTOR3 d3dtemp;
				if( CInput::GetInstance()->CheckInput(SHOOT_UP, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_UP_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_UP_RIGHT, i) ||
					CInput::GetInstance()->CheckInput(SHOOT_DOWN_LEFT, i) || CInput::GetInstance()->CheckInput(SHOOT_DOWN_RIGHT, i) ||
					CInput::GetInstance()->GetShootDir(i, CGameplayState::GetInstance()->GetPlayers()[i]->GetMatrix(), d3dtemp) ||
					CInput::GetInstance()->CheckInput(DASH, i) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(false);
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);
					m_nCorrect = 2;
					m_fMarkTimer = 1.5f;
					break;
				}

				
				if( CInput::GetInstance()->CheckInput(SPECIAL, i) )
				{
					CGameplayState::GetInstance()->GetPlayers()[i]->CheckTutorial(true);
					m_nCorrect = 1;
					m_fUpdateTime = 2.5f;
					m_fMarkTimer = 1.5f;
					break;
				}
			}
		}

		 if(m_fUpdateTime > 5.0f)
		 {
			if(!m_bTutorial)
			{
				m_fUpdateTime = 0.0f;
				m_nCurrTexture++;
				if(m_nCurrTexture <= INTRO_SPECIAL)
				{
					m_bTutorial = true;

					for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
						if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
							CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);	
				}
			}
		 }
	 }
	 else if(m_nCurrTexture < INTRO_DESTROY)
	 {
		 if(m_fUpdateTime > 2.5f)
		 {
			 for(unsigned int i = 0; i < CGameplayState::GetInstance()->GetPlayers().size(); i++)
				if(CGameplayState::GetInstance()->GetPlayers()[i]->GetLives() < 2)
					CGameplayState::GetInstance()->GetPlayers()[i]->SetLives(2);

			 m_fUpdateTime = 0.0f;
			 m_nCurrTexture++;
		 }
	 }
}	 
	 
void CIntroState::Render()
{	
	CAssetManager* pAM = CAssetManager::GetInstance();

	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	IDirect3DDevice9* pD3D = m_pRenderer->GetDirect3DDevice();

	
	ID3DXEffect* pShader = pAM->LoadShader("Assets/Shaders/Tiles.fx");

	pShader->SetTechnique("StaticScreen");
	unsigned int passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned pass(0);pass < passes; pass++)
	{
		pShader->BeginPass(pass);

		pShader->SetTexture("gDiffuseTexture", m_pTextures[m_nCurrTexture]);

		pShader->SetMatrix("gViewProjection", &ViewProj);
		pShader->CommitChanges();

		pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(POS_TEXCOORD));
		pD3D->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TTileVert));
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 4, 2);

		pShader->EndPass();
	}
	pShader->End();

	pShader->SetTechnique("myTechnique");

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned pass(0);pass < passes; pass++)
	{
		pShader->BeginPass(pass);

		pShader->SetTexture("gDiffuseTexture", pAM->LoadTexture("Assets/Textures/Tutorial_Floor.png"));
		pShader->SetTexture("gNormalTexture", pAM->LoadTexture("Assets/Textures/Tutorial_Floor_Normals.png"));
		float color[] = {.2f, .4f, .2f, 1.0f};
		pShader->SetFloatArray("gAmbient", color, 4);
		
		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;

		pShader->SetFloatArray("gLightDir", &capture[0], 3);
		pShader->CommitChanges();

		pD3D->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TTileVert));
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		
		pShader->EndPass();
	}
	pShader->End();
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	
	if(m_nCorrect != 0)
	{
		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		pShader->SetTechnique("StaticScreen");
		unsigned int passes2 = 0;
		pShader->Begin(&passes2,0);
		for(unsigned pass(0);pass < passes2; pass++)
		{
			pShader->BeginPass(pass);

			if(m_nCorrect == 1 && m_fMarkTimer > 0.0f)
				pShader->SetTexture("gDiffuseTexture", m_pTextures[CORRECT_MARK]);
			else if(m_nCorrect == 2 && m_fMarkTimer > 0.0f)
				pShader->SetTexture("gDiffuseTexture", m_pTextures[WRONG_MARK]);

			pShader->SetMatrix("gViewProjection", &ViewProj);
			pShader->CommitChanges();

			pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(POS_TEXCOORD));
			pD3D->SetStreamSource(0, m_pVertexBuffer, sizeof(TTileVert)*8, sizeof(TTileVert));
			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

			pShader->EndPass();
		}
		pShader->End();

		pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}