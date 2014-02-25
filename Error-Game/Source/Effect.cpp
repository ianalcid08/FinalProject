/**********************************************************************************************
 * Filename:  		CEffect.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/25/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that pulls several effects together to create one effect.
 **********************************************************************************************/
#include "Effect.h"

void CEffect::SetColors(D3DXCOLOR d3dColor)
{
	for(int i = 0; i < m_nEmitters; ++i)
		m_cEmitters[i].SetColors(d3dColor);
	for(int i = 0; i < m_nGeometries; ++i)
		m_cGeometries[i].SetColors(d3dColor);
}

void CEffect::SetMatrix(D3DXMATRIX d3dMat)
{
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans,m_d3dOffset.x,m_d3dOffset.y,m_d3dOffset.z);
	m_d3dMatrix = trans*d3dMat;
}

CEffect::CEffect(void)
{
	m_nEmitters = 0;
	m_nGeometries = 0;
	D3DXMatrixIdentity(&m_d3dMatrix);
	m_d3dVelocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

CEffect::~CEffect(void)
{

}

bool CEffect::Update(float fDelta)
{
	m_fLifeTime-=fDelta;
	*(D3DXVECTOR3*)&m_d3dMatrix[12] += m_d3dVelocity*fDelta;
	bool returnBool = false;
	for(int i=0;i<m_nEmitters;i++)
		if(m_cEmitters[i].Update(fDelta))
				returnBool=true;
	for(int i=0;i<m_nGeometries;i++)
		if(m_cGeometries[i].Update(fDelta))
				returnBool=true;
	if(m_fLifeTime < 0.0f && !returnBool)
		return false;
	return true;
}

void CEffect::RenderEmitt(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj)
{
	for(int i=0;i<m_nEmitters;i++)
		m_cEmitters[i].Render(d3dCam,d3dProj,m_d3dMatrix);
}

void CEffect::RenderGeo(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj)
{
	for(int i=0;i<m_nGeometries;i++)
		m_cGeometries[i].Render(d3dCam,d3dProj,m_d3dMatrix);
}

void CEffect::AddEmitter(CEmitter pEffect)
{
	if(m_nEmitters<MAX_EMITTERS)
	{
		m_cEmitters[m_nEmitters]=pEffect;
		m_cEmitters[m_nEmitters++].Init();
	}
}

void CEffect::AddGeometry(CGeometryEffect pEffect)
{
	if(m_nGeometries<MAX_GEOMETRIES)
	{
		m_cGeometries[m_nGeometries]=pEffect;
		m_cGeometries[m_nGeometries++].Init();
	}
}

void CEffect::Reset()
{
	for(int i=0;i<m_nEmitters;i++)
		m_cEmitters[i].ShutDown();
	m_nEmitters = 0;
	m_nGeometries = 0;
	D3DXMatrixIdentity(&m_d3dMatrix);
	m_d3dVelocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
}