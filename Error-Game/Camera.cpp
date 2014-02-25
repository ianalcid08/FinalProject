/***********************************************
 * Filename:  		CCamera.cpp
 * Date:      		2/8/2013
 * Mod. Date: 		2/8/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Controls what you can see
 ************************************************/
#include "Camera.h"

CCamera CCamera::m_Instance;

CCamera::CCamera(void)
{
	// fake hacked camera
	D3DXVECTOR3 eye(0,2,-4), at(0,1,0), up(0,1,0);
	D3DXMatrixLookAtLH(&m_D3DCamera,&eye,&at,&up);
}


CCamera::~CCamera(void)
{
}
CCamera* CCamera::GetInstance()
{
	//static CCamera		m_Instance;
	return &m_Instance;
	
}