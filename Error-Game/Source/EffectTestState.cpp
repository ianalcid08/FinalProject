/***********************************************
 * Filename:  		EffectTestState.h
 * Date:      		02/27/2013
 * Mod. Date: 		03/02/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D Springer
 * Purpose:   		State to be used for testing the rendering of effects
 ************************************************/
#include "EffectTestState.h"
#include "Game.h"
#include "Renderer.h"

CEffectTestState* CEffectTestState::GetInstance()
{
	static CEffectTestState s_Instance;
	return &s_Instance;
}

void CEffectTestState::Initialize()
{
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixIdentity(&m_d3dRot);
	float junk;
	temp[10] = -1.0f;
	*(D3DXVECTOR3*)&temp[12] = D3DXVECTOR3(0.0f, 2.0f, 15.0f);
	m_d3dNonInvCam = temp;
	D3DXMatrixInverse(&m_d3dCam, &junk, &temp);

	float fFov = (D3DXToRadian(60));
	float fAspect = (16.0f/9.0f);
	float fNear = 0.01f;
	float fFar = 100.0f; 
	D3DXMatrixPerspectiveFovLH(&m_d3dProj, fFov, fAspect, fNear, fFar);

	m_fCurrRotation = 0.0f;
	m_fCurrRotationUD = 0.0f;

	m_fxMan.LoadEffects("TestFiles/TestEffectData.vfx");
	m_nCurrentEffect = 0;
}

void CEffectTestState::Shutdown()
{
	m_fxMan.ClearData();
}
	
bool CEffectTestState::Input()
{
	if(GetAsyncKeyState(VK_ESCAPE))
		CGame::GetInstance()->ChangeState(MAINMENU_STATE);
	if(GetAsyncKeyState('1'))
		m_nCurrentEffect = EFFECT_MEDEXP;
	if(GetAsyncKeyState('2'))
		m_nCurrentEffect = EFFECT_INVULSPARK;
	if(GetAsyncKeyState('3'))
		m_nCurrentEffect = EFFECT_TESLASPRAY;
	if(GetAsyncKeyState('4'))
		m_nCurrentEffect = EFFECT_PULSEHIT;
	if(GetAsyncKeyState('5'))
		m_nCurrentEffect = EFFECT_BULLETHIT;
	if(GetAsyncKeyState('6'))
		m_nCurrentEffect = EFFECT_TESLAHIT;
	if(GetAsyncKeyState('7'))
		m_nCurrentEffect = EFFECT_PLAYERDMG;
	if(GetAsyncKeyState('8'))
		m_nCurrentEffect = EFFECT_MOLTENBURN;
	if(GetAsyncKeyState('9'))
		m_nCurrentEffect = EFFECT_DMGEXPLOSION;
	if(GetAsyncKeyState('0'))
		m_nCurrentEffect = EFFECT_SMALLEXP;
	if(GetAsyncKeyState('Q'))
		m_nCurrentEffect = EFFECT_SMOKE;
	if(GetAsyncKeyState('W'))
		m_nCurrentEffect = EFFECT_SMOKEBURN;
	if(GetAsyncKeyState('E'))
		m_nCurrentEffect = EFFECT_BURN;
	if(GetAsyncKeyState('R'))
		m_nCurrentEffect = EFFECT_LASER;
	if(GetAsyncKeyState('T'))
		m_nCurrentEffect = EFFECT_PLAYERDEATH;
	if(GetAsyncKeyState('Y'))
		m_nCurrentEffect = EFFECT_SPECIALHIT;
	if(GetAsyncKeyState('U'))
		m_nCurrentEffect = EFFECT_SMASHWAVE;
	if(GetAsyncKeyState('I'))
		m_nCurrentEffect = EFFECT_SPECIAL;
	if(GetAsyncKeyState('O'))
		m_nCurrentEffect = EFFECT_SPIDERFIRE;
	if(GetAsyncKeyState('P'))
		m_nCurrentEffect = EFFECT_DASH;
	if(GetAsyncKeyState(VK_BACK))
	{
		m_fxMan.ClearData();
		m_fxMan.ReInit();
		m_fxMan.LoadEffects("TestFiles/TestEffectData.vfx");
	}
	return true;
}

void CEffectTestState::Update( float fElapsedTime )
{
	m_fTimer-=fElapsedTime;
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);
	if(m_fTimer<=0.0f)
	{
		m_fTimer=1.0f;
		m_fxMan.CreateEffect(m_nCurrentEffect,d3dMat);
	}
	D3DXMATRIX updown;
	D3DXMatrixIdentity(&updown);
	if(GetAsyncKeyState(VK_LEFT))
	{
		m_fCurrRotation += fElapsedTime;
	}
	else if(GetAsyncKeyState(VK_RIGHT))
	{
		m_fCurrRotation -= fElapsedTime;
	}
	if(GetAsyncKeyState(VK_UP))
	{
		m_fCurrRotationUD += fElapsedTime;
	}
	else if(GetAsyncKeyState(VK_DOWN))
	{
		m_fCurrRotationUD -= fElapsedTime;
	}
	D3DXMatrixRotationY(&m_d3dRot, m_fCurrRotation * D3DXToRadian(60.0f));
	D3DXMatrixRotationX(&updown, m_fCurrRotationUD * D3DXToRadian(60.0f));
	m_d3dRot*=updown;
	m_fxMan.UpdateEffects(fElapsedTime);
}

void CEffectTestState::Render()
{
	D3DXMATRIX d3dViewProj;
	float junk;
	D3DXMatrixInverse(&d3dViewProj,&junk,&(m_d3dNonInvCam*m_d3dRot));
	d3dViewProj*=m_d3dProj;
	D3DXMATRIX cam = m_d3dNonInvCam*m_d3dRot;
	m_fxMan.RenderEffects(cam,d3dViewProj);
}