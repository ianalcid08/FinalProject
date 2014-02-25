/************************************************************************************
* Filename:  		HUD.cpp
* Date:      		02/11/2013
* Mod. Date: 		02/13/2013
* Mod. Initials:	TS
* Author:    		Mike A. Barbato
* Purpose:   		This is the heads up display
************************************************************************************/

#include "HUD.h"
#include "AICore.h"
#include "SteamrollerBot.h"
#include "AssetManager.h"
#include "Input.h"
#include "GameplayState.h"

CHUD::CHUD(void)
{
	m_fBossFade = 0.0f;

	THUDVertex tHealth[4];
	THUDVertex tGunDuration[4];
	THUDVertex tBossHealth[4];
	THUDVertex tDash[4];
	THUDVertex tSpecial[4];
	THUDVertex tLives[4];
	THUDVertex tPortrait[4];
	THUDVertex tReticle[4];
	THUDVertex tBack[4];
	THUDVertex tEmpty[4];

	tHealth[0].d3dPosition = D3DXVECTOR3(0.195f, -0.09f, 0.0f);
	tHealth[1].d3dPosition = D3DXVECTOR3(0.195f, -0.03f, 0.0f);
	tHealth[2].d3dPosition = D3DXVECTOR3(0.395f, -0.03f, 0.0f);
	tHealth[3].d3dPosition = D3DXVECTOR3(0.395f, -0.09f, 0.0f);
	tHealth[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tHealth[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tHealth[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tHealth[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	tGunDuration[0].d3dPosition = D3DXVECTOR3(0.4f, -0.05f, 0.0f);
	tGunDuration[1].d3dPosition = D3DXVECTOR3(0.45f, -0.05f, 0.0f);
	tGunDuration[2].d3dPosition = D3DXVECTOR3(0.45f, -0.175f, 0.0f);
	tGunDuration[3].d3dPosition = D3DXVECTOR3(0.4f, -0.175f, 0.0f);
	tGunDuration[0].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tGunDuration[1].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	tGunDuration[2].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tGunDuration[3].d3dUV = D3DXVECTOR2(0.0f, 0.0f);

	tBossHealth[0].d3dPosition = D3DXVECTOR3(-0.6f, 0.2f, 0.0f);
	tBossHealth[1].d3dPosition = D3DXVECTOR3(0.6f, 0.2f, 0.0f);
	tBossHealth[2].d3dPosition = D3DXVECTOR3(0.6f, -0.2f, 0.0f);
	tBossHealth[3].d3dPosition = D3DXVECTOR3(-0.6f, -0.2f, 0.0f);
	tBossHealth[0].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tBossHealth[1].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tBossHealth[2].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	tBossHealth[3].d3dUV = D3DXVECTOR2(0.0f, 1.0f);

	tDash[0].d3dPosition = D3DXVECTOR3(0.195f, -0.14f, 0.0f);
	tDash[1].d3dPosition = D3DXVECTOR3(0.195f, -0.09f, 0.0f);
	tDash[2].d3dPosition = D3DXVECTOR3(0.32f, -0.09f, 0.0f);
	tDash[3].d3dPosition = D3DXVECTOR3(0.32f, -0.14f, 0.0f);
	tDash[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tDash[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tDash[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tDash[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	tPortrait[0].d3dPosition = D3DXVECTOR3(0.025f, -0.18f, 0.0f);
	tPortrait[1].d3dPosition = D3DXVECTOR3(0.025f, -0.03f, 0.0f);
	tPortrait[2].d3dPosition = D3DXVECTOR3(0.175f, -0.03f, 0.0f);
	tPortrait[3].d3dPosition = D3DXVECTOR3(0.175f, -0.18f, 0.0f);
	tPortrait[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tPortrait[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tPortrait[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tPortrait[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	tReticle[0].d3dPosition = D3DXVECTOR3(.05f, .05f, 0.0f);
	tReticle[1].d3dPosition = D3DXVECTOR3(.05f, -.05f, 0.0f);
	tReticle[2].d3dPosition = D3DXVECTOR3(-.05f, -.05f, 0.0f);
	tReticle[3].d3dPosition = D3DXVECTOR3(-.05f, .05f, 0.0f);
	tReticle[0].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tReticle[1].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tReticle[2].d3dUV = D3DXVECTOR2(1.0f, 1.0f);
	tReticle[3].d3dUV = D3DXVECTOR2(1.0f, 0.0f);

	tLives[0].d3dPosition = D3DXVECTOR3(0.195f, -0.2f, 0.0f);
	tLives[1].d3dPosition = D3DXVECTOR3(0.195f, -0.15f, 0.0f);
	tLives[2].d3dPosition = D3DXVECTOR3(0.245f, -0.15f, 0.0f);
	tLives[3].d3dPosition = D3DXVECTOR3(0.245f, -0.2f, 0.0f);
	tLives[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tLives[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tLives[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tLives[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	tSpecial[0].d3dPosition = D3DXVECTOR3(0.3225f, -0.18f, 0.0f);
	tSpecial[1].d3dPosition = D3DXVECTOR3(0.3225f, -0.105f, 0.0f);
	tSpecial[2].d3dPosition = D3DXVECTOR3(0.3975f, -0.105f, 0.0f);
	tSpecial[3].d3dPosition = D3DXVECTOR3(0.3975f, -0.18f, 0.0f);
	tSpecial[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tSpecial[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tSpecial[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tSpecial[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	tBack[0].d3dPosition = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	tBack[1].d3dPosition = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	tBack[2].d3dPosition = D3DXVECTOR3(1.0f, 0.7f, 0.0f);
	tBack[3].d3dPosition = D3DXVECTOR3(-1.0f, 0.7f, 0.0f);
	tBack[0].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tBack[1].d3dUV = D3DXVECTOR2(4.0f, 0.0f);
	tBack[2].d3dUV = D3DXVECTOR2(4.0f, 1.0f);
	tBack[3].d3dUV = D3DXVECTOR2(0.0f, 1.0f);

	tEmpty[0].d3dPosition = D3DXVECTOR3(0.025f, -0.18f, 0.0f);
	tEmpty[1].d3dPosition = D3DXVECTOR3(0.025f, -0.025f, 0.0f);
	tEmpty[2].d3dPosition = D3DXVECTOR3(0.3975f, -0.025f, 0.0f);
	tEmpty[3].d3dPosition = D3DXVECTOR3(0.3975f, -0.18f, 0.0f);
	tEmpty[0].d3dUV = D3DXVECTOR2(0.0f, 1.0f);
	tEmpty[1].d3dUV = D3DXVECTOR2(0.0f, 0.0f);
	tEmpty[2].d3dUV = D3DXVECTOR2(1.0f, 0.0f);
	tEmpty[3].d3dUV = D3DXVECTOR2(1.0f, 1.0f);

	D3DXMatrixTranslation(&(m_d3dOffset[0]), -.97f, 0.98f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[1]), -0.47f, 0.98f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[2]), 0.039f, 0.98f, 0.0f);
	D3DXMatrixTranslation(&(m_d3dOffset[3]), 0.53f, 0.98f, 0.0f);
	D3DXMatrixTranslation(&m_d3dBossOffset, 0.0f, -.8f, 0.0f);

	m_fDefaultRatio = 1.0f;

	pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	CAssetManager* pAM = CAssetManager::GetInstance();
	d3dHudShader = pAM->LoadShader("Assets/Shaders/HUDDraw.fx");

	d3dHealthTexture = pAM->LoadTexture("Assets/Textures/HUD_Player_HP_Bar.png");
	d3dHealthBackTexture = pAM->LoadTexture("Assets/Textures/HUD_Player_HP_Backplate.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dHealthBuff, 0);
	void* vram = nullptr;
	d3dHealthBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tHealth,sizeof(THUDVertex)*4);
	d3dHealthBuff->Unlock();

	d3dBossTexture = pAM->LoadTexture("Assets/Textures/Loading_Screen_Bar.png");
	d3dBossBackTexture = pAM->LoadTexture("Assets/Textures/Loading_Screen_Bar_Box.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dBossHealthBuff, 0);
	vram = nullptr;
	d3dBossHealthBuff->Lock(0, 0, &vram, 0);
	memcpy(vram, tBossHealth, sizeof(THUDVertex)*4);
	d3dBossHealthBuff->Unlock();

	d3dDashTexture = pAM->LoadTexture("Assets/Textures/HUD_Player_Dash_Bar.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dDashBuff, 0);
	vram = nullptr;
	d3dDashBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tDash,sizeof(THUDVertex)*4);
	d3dDashBuff->Unlock();

	d3dPortraitTexture[hudBLUE] = pAM->LoadTexture("Assets/Textures/HUD_Player_BluePortrait.png");
	d3dPortraitTexture[hudRED] = pAM->LoadTexture("Assets/Textures/HUD_Red_Portrait.png");
	d3dPortraitTexture[hudGREEN] = pAM->LoadTexture("Assets/Textures/HUD_Green_Portrait.png");
	d3dPortraitTexture[hudYELLOW] = pAM->LoadTexture("Assets/Textures/HUD_Yellow_Portrait.png");
	pD3D->CreateVertexBuffer(4*sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dPortraitBuff, 0);
	vram = nullptr;
	d3dPortraitBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tPortrait,4*sizeof(THUDVertex)*4);
	d3dPortraitBuff->Unlock();

	d3dLifeTexture = pAM->LoadTexture("Assets/Textures/Lounge_Tigers_HUD_Lives.png");
	d3dLifeBackTexture = pAM->LoadTexture("Assets/Textures/HUD_Player_Lives_Back.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dLivesBuff, 0);
	vram = nullptr;
	d3dLivesBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tLives,sizeof(THUDVertex)*4);
	d3dLivesBuff->Unlock();

	d3dGunDurationTexture[PU_BEAM] = pAM->LoadTexture("Assets/Textures/Weapon_Pulse_Laser_Icon.png");
	d3dGunDurationTexture[PU_TESLA] = pAM->LoadTexture("Assets/Textures/Weapon_Tesla_Ray_Icon.png");
	d3dGunDurationTexture[PU_GATLING] = pAM->LoadTexture("Assets/Textures/Lounge_Tigers_GatlingGun_Icon.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dGunDurationBuff, 0);
	vram = nullptr;
	d3dGunDurationBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tGunDuration,sizeof(THUDVertex)*4);
	d3dGunDurationBuff->Unlock();

	d3dSpecialTextureOn = pAM->LoadTexture("Assets/Textures/HUD_Player_Special_Icon_ON.png");
	d3dSpecialTextureOff = pAM->LoadTexture("Assets/Textures/HUD_Player_Special_Icon_OFF.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dSpecialBuff, 0);
	vram = nullptr;
	d3dSpecialBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tSpecial,sizeof(THUDVertex)*4);
	d3dSpecialBuff->Unlock();

	d3dReticleTexture = pAM->LoadTexture("Assets/Textures/Reticle.png");
	d3dReticleBackTexture = pAM->LoadTexture("Assets/Textures/ReticleBack.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dReticleBuff, 0);
	vram = nullptr;
	d3dReticleBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tReticle,sizeof(THUDVertex)*4);
	d3dReticleBuff->Unlock();

	d3dBackTexture = pAM->LoadTexture("Assets/Textures/Backdrop.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dBlackBuff, 0);
	vram = nullptr;
	d3dBlackBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tBack,sizeof(THUDVertex)*4);
	d3dBlackBuff->Unlock();

	d3dEmptyTexture = pAM->LoadTexture("Assets/Textures/Menu_Main_ERROR.png");
	pD3D->CreateVertexBuffer(sizeof(THUDVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dEmptyBuff, 0);
	vram = nullptr;
	d3dEmptyBuff->Lock(0, 0, &vram, 0);
	memcpy(vram,tEmpty,sizeof(THUDVertex)*4);
	d3dEmptyBuff->Unlock();
}


CHUD::~CHUD(void)
{
	if(d3dHealthBuff)
		d3dHealthBuff->Release();
	if(d3dDashBuff)
		d3dDashBuff->Release();
	if(d3dPortraitBuff)
		d3dPortraitBuff->Release();
	if(d3dLivesBuff)
		d3dLivesBuff->Release();
	if(d3dSpecialBuff)
		d3dSpecialBuff->Release();
	if(d3dGunDurationBuff)
		d3dGunDurationBuff->Release();
	if(d3dBossHealthBuff)
		d3dBossHealthBuff->Release();
	if(d3dReticleBuff)
		d3dReticleBuff->Release();
	if(d3dBlackBuff)
		d3dBlackBuff->Release();
	if(d3dEmptyBuff)
		d3dEmptyBuff->Release();
}


void CHUD::Render(void)
{
	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	CInput* pInput = CInput::GetInstance();
	unsigned int passes(0);
	float fDefaultFade = .75f;

	pD3D->SetVertexDeclaration(CAssetManager::GetInstance()->GetVertexDeclaration(POS_TEXCOORD));
	D3DXMATRIX d3dIdent;
	D3DXMatrixIdentity(&d3dIdent);
	d3dHudShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		d3dHudShader->BeginPass(i);
		d3dHudShader->SetFloat("gFade", 1.0f);
		d3dHudShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f), 4);
		d3dHudShader->SetTexture("gDiffuseTexture", d3dBackTexture);
		d3dHudShader->SetFloat("gRatio", 4.0f);
		d3dHudShader->SetMatrix("gOffset", &d3dIdent);
		d3dHudShader->CommitChanges();

		pD3D->SetStreamSource(0, d3dBlackBuff, 0, sizeof(THUDVertex));
		pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		d3dHudShader->EndPass();
	}
	d3dHudShader->End();
	//d3dHudShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
	for(unsigned int j = 3; j >= m_unNumPlayers; j--)
	{
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);
			d3dHudShader->SetTexture("gDiffuseTexture", d3dEmptyTexture);
			d3dHudShader->SetMatrix("gOffset", &(m_d3dOffset[j]));
			d3dHudShader->SetFloat("gRatio", m_fDefaultRatio);
			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dEmptyBuff, 0, sizeof(THUDVertex));
			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			d3dHudShader->EndPass();
		}
		d3dHudShader->End();
	}
	d3dHudShader->SetFloat("gFade", fDefaultFade);
	for(unsigned int j = 0; j < m_unNumPlayers; j++)
	{
		if( pInput->GetIsKeyboard((*m_pPlayers)[j]->GetPlayerNum()))
		{
			if(!CGame::GetInstance()->GetVM()->GetFade())
			{
				// RETICLE SHIT
				D3DXMATRIX d3dReticle;

				if(pInput->IsMouseConfined())
				{
					Sphere getMouse = pInput->GetMouseWorld();
					D3DXMATRIX mouse;
					D3DXMatrixTranslation(&mouse, getMouse.m_Center.x, getMouse.m_Center.y, getMouse.m_Center.z);
					mouse *= CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
					D3DXMatrixTranslation(&d3dReticle, mouse[12] / mouse[15], mouse[13] / mouse[15], 0.0f);
				}
				else
				{
					float fX = pInput->GetScreenMouseX();
					float fY = pInput->GetScreenMouseY();
					D3DXMatrixTranslation(&d3dReticle, fX, fY, 0.0f);
				}

				d3dHudShader->Begin(&passes,0);
				for(unsigned i(0);i < passes; i++)
				{
					d3dHudShader->BeginPass(i);
					d3dHudShader->SetFloat("gFade", 0.35f);
					d3dHudShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f), 4);
					d3dHudShader->SetTexture("gDiffuseTexture", d3dReticleBackTexture);
					d3dHudShader->SetFloat("gRatio", m_fDefaultRatio);
					d3dHudShader->SetMatrix("gOffset", &d3dReticle);
					d3dHudShader->CommitChanges();

					pD3D->SetStreamSource(0, d3dReticleBuff, 0, sizeof(THUDVertex));
					pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

					d3dHudShader->SetFloat("gFade", fDefaultFade);
					d3dHudShader->SetFloatArray("gColor", (*m_pPlayers)[j]->GetColor(), 4);
					d3dHudShader->SetTexture("gDiffuseTexture", d3dReticleTexture);
					d3dHudShader->CommitChanges();
					pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

					d3dHudShader->EndPass();
				}
				d3dHudShader->End();

				d3dHudShader->SetFloatArray("gColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 4);
				d3dHudShader->CommitChanges();
				/////////////////////////////////RETICLE SHIT ENDS HERE
			}
		}
		float nHealthRatio = 0.0f;
		if(CGame::GetInstance()->GetDifficulty() == NORMAL)
		{
			nHealthRatio = (*m_pPlayers)[j]->GetHealth() / (MAXHP_PLAYER + 50.0f);
		}
		else
		{
			nHealthRatio = (*m_pPlayers)[j]->GetHealth() / MAXHP_PLAYER;
		}
		
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);
			d3dHudShader->SetMatrix("gOffset", &(m_d3dOffset[j]));
			d3dHudShader->SetTexture("gDiffuseTexture", d3dHealthTexture);
			d3dHudShader->SetTexture("gBackTexture", d3dHealthBackTexture);
			d3dHudShader->SetFloat("gRatio", nHealthRatio);

			if((*m_pPlayers)[j]->GetBeingHit() == 2)
				d3dHudShader->SetInt("gHit", 1);
			else
				d3dHudShader->SetInt("gHit", 0);

			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dHealthBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->SetInt("gHit", 0);
			d3dHudShader->CommitChanges();

			d3dHudShader->EndPass();
		}
		d3dHudShader->End();

		if((*m_pPlayers)[j]->GetGunDuration() > 0)
		{
			float nGunRatio = (*m_pPlayers)[j]->GetGunDuration() / WEAPON_DURATION;
			passes = 0;
			d3dHudShader->Begin(&passes,0);
			for(unsigned i(0);i < passes; i++)
			{
				d3dHudShader->BeginPass(i);

				IDirect3DTexture9* capture = d3dGunDurationTexture[PU_GATLING];
				switch((*m_pPlayers)[j]->GetGunID())
				{
				case BEAM:
					{
						capture = d3dGunDurationTexture[PU_BEAM];
					}
					break;
				case TESLA:
					{
						capture = d3dGunDurationTexture[PU_TESLA];
					}
					break;
				}

				d3dHudShader->SetTexture("gDiffuseTexture", capture);
				d3dHudShader->SetFloat("gRatio", nGunRatio);
				d3dHudShader->CommitChanges();

				pD3D->SetStreamSource(0, d3dGunDurationBuff, 0, sizeof(THUDVertex));

				pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

				d3dHudShader->EndPass();
			}
			d3dHudShader->End();
		}

		float fDashRatio = (*m_pPlayers)[j]->GetDashEnergy() / (*m_pPlayers)[j]->GetMaxDashEnergy();
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);


			d3dHudShader->SetTexture("gDiffuseTexture", d3dDashTexture);
			d3dHudShader->SetFloat("gRatio", fDashRatio);
			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dDashBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->EndPass();
		}

		d3dHudShader->End();
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);

			if((*m_pPlayers)[j]->GetColorPlayer() == D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f))
				d3dHudShader->SetTexture("gDiffuseTexture", d3dPortraitTexture[hudBLUE]);
			else if((*m_pPlayers)[j]->GetColorPlayer() == D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f))
				d3dHudShader->SetTexture("gDiffuseTexture", d3dPortraitTexture[hudRED]);
			else if((*m_pPlayers)[j]->GetColorPlayer() == D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f))
				d3dHudShader->SetTexture("gDiffuseTexture", d3dPortraitTexture[hudGREEN]);
			else if((*m_pPlayers)[j]->GetColorPlayer() == D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f))
				d3dHudShader->SetTexture("gDiffuseTexture", d3dPortraitTexture[hudYELLOW]);

			d3dHudShader->SetFloat("gRatio", m_fDefaultRatio);

			if((*m_pPlayers)[j]->GetBeingHit() == 2)
				d3dHudShader->SetInt("gHit", 1);
			else if(!(*m_pPlayers)[j]->GetAlive())
				d3dHudShader->SetInt("gHit", 2);	
			else
				d3dHudShader->SetInt("gHit", 0);

			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dPortraitBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->SetInt("gHit", 0);
			d3dHudShader->CommitChanges();

			d3dHudShader->EndPass();
		}

		d3dHudShader->End();
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);


			if((*m_pPlayers)[j]->GetSpecial())
			{
				d3dHudShader->SetTexture("gDiffuseTexture", d3dSpecialTextureOn);
			}
			else
			{
				d3dHudShader->SetTexture("gDiffuseTexture", d3dSpecialTextureOff);
			}
			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dSpecialBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->EndPass();
		}

		d3dHudShader->End();
		int nLives = (*m_pPlayers)[j]->GetLives();
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);


			d3dHudShader->SetTexture("gBackTexture", d3dLifeBackTexture);
			d3dHudShader->SetTexture("gDiffuseTexture", d3dLifeTexture);
			if(nLives < 1)
				d3dHudShader->SetFloat("gRatio", 0.0f);
			else
				d3dHudShader->SetFloat("gRatio", 1.0f);
			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dLivesBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->EndPass();
		}

		d3dHudShader->End();
		D3DXMATRIX tempOffset;
		D3DXMatrixTranslation(&tempOffset, 0.06f, 0.0f, 0.0f);
		passes = 0;
		d3dHudShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			d3dHudShader->BeginPass(i);

			d3dHudShader->SetMatrix("gOffset", &(m_d3dOffset[j] * tempOffset));
			if(nLives < 2)
				d3dHudShader->SetFloat("gRatio", 0.0f);
			else
				d3dHudShader->SetFloat("gRatio", 1.0f);
			d3dHudShader->CommitChanges();

			pD3D->SetStreamSource(0, d3dLivesBuff, 0, sizeof(THUDVertex));

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->EndPass();
		}

		d3dHudShader->End();
	}

	if( m_bBossEnabled )
	{
		m_fBossFade += CGame::GetInstance()->GetDelta();
		if( m_fBossFade > .75f )
			m_fBossFade = .75f;

		float fHP = m_pBoss->GetHealth();

		if( m_pBoss->GetID() == OBJ_AICORE )
		{
			fHP += ((CAICore*)m_pBoss)->GetLeftFist()->GetHealth();
			fHP += ((CAICore*)m_pBoss)->GetRightFist()->GetHealth();
		}
		

		float fHPRatio = fHP / m_fMaxBossHP;

		passes = 0;
		d3dHudShader->Begin(&passes, 0);
		for( unsigned int i = 0; i < passes; ++i )
		{
			d3dHudShader->BeginPass(i);
		
			d3dHudShader->SetMatrix("gOffset", &m_d3dBossOffset);
			d3dHudShader->SetFloat("gFade", m_fBossFade);
			d3dHudShader->SetTexture("gDiffuseTexture", d3dBossBackTexture);
			d3dHudShader->SetFloat("gRatio", 1.0f);
			d3dHudShader->CommitChanges();
		
			pD3D->SetStreamSource(0, d3dBossHealthBuff, 0, sizeof(THUDVertex));
			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);

			d3dHudShader->SetTexture("gBackTexture", d3dBossBackTexture);
			d3dHudShader->SetTexture("gDiffuseTexture", d3dBossTexture);
			d3dHudShader->SetFloat("gRatio", fHPRatio);
			d3dHudShader->CommitChanges();

			pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
		
			d3dHudShader->EndPass();
		}
		d3dHudShader->End();
	}

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}


void CHUD::Init(std::vector<CPlayerObject*>* pPlayers)
{
	DisableBoss();

	if(pPlayers)
	{
		m_pPlayers = pPlayers;
		m_unNumPlayers = m_pPlayers->size();
	}
	else
	{
		m_pPlayers = nullptr;
		m_unNumPlayers = 4;
	}

	D3DXMatrixOrthoLH(&d3dOrthoMatrix,2.0f,2.0f,0.01f,10.0f);
}


void CHUD::EnableBoss(IBaseObject* pBaseObj)
{
	m_fMaxBossHP = pBaseObj->GetHealth();

	if( pBaseObj->GetID() == OBJ_AICORE )
	{
		m_fMaxBossHP += ((CAICore*)pBaseObj)->GetLeftFist()->GetHealth();
		m_fMaxBossHP += ((CAICore*)pBaseObj)->GetRightFist()->GetHealth();
	}

	m_fBossFade = 0.0f;
	m_pBoss = pBaseObj;
	m_bBossEnabled = true;
}

void CHUD::DisableBoss()
{
	m_bBossEnabled = false; 
	m_pBoss = nullptr;
	m_fBossFade = 0.0f;
}
