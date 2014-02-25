/**********************************************************************************************
 * Filename:  		Particle.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		03/02/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the particle that emitters create.
 **********************************************************************************************/
#include "Particle.h"

D3DXMATRIX CParticle::GetTranslateMatrix()
{
	D3DXMATRIX d3dMat;
	D3DXMatrixTranslation(&d3dMat,m_d3dCurVelocity.x,m_d3dCurVelocity.y,m_d3dCurVelocity.z);
	return d3dMat;
}

D3DXMATRIX CParticle::GetScaleMatrix()
{
	D3DXMATRIX d3dScale;
	D3DXMatrixScaling(&d3dScale,m_d3dCurScale[0],m_d3dCurScale[1],m_d3dCurScale[2]);
	return d3dScale;
}

D3DXMATRIX CParticle::GetRotateMatrix()
{
	D3DXMATRIX d3dRotX,d3dRotY,d3dRotZ;
	D3DXMatrixRotationZ(&d3dRotX,D3DXToRadian(m_d3dCurRotation[0]));
	D3DXMatrixRotationZ(&d3dRotY,D3DXToRadian(m_d3dCurRotation[1]));
	D3DXMatrixRotationZ(&d3dRotZ,D3DXToRadian(m_d3dCurRotation[2]));
	return d3dRotX*d3dRotY*d3dRotZ;
}

CParticle::CParticle(void)
{
	m_d3dVelocity = D3DXVECTOR3();
	m_d3dPosition = D3DXMATRIX();
	m_d3dColor = D3DXCOLOR();
	m_bIsAlive = false;
	m_fDuration = 0.0f;
	for(int i=0;i<3;i++)
	{
		m_d3dCurScale[i] = 1.0f;
		m_d3dCurRotation[i] = 0.0f;
	}
	m_nCurStage = 0;
	memset(&m_tPrevStage,0,sizeof(TStage));
	memset(&m_tTargetStage,0,sizeof(TStage));
}

CParticle::~CParticle(void)
{

}

int CParticle::Update(float fDelta)
{
	if(m_tTargetStage.fTime == 0.0f)
	{
		m_bIsAlive = false;
		return -1;
	}
	m_fDuration += fDelta;
	if(m_fDuration >= m_tTargetStage.fTime)
	{
		m_nCurStage++;
		return m_nCurStage;
	}
	float tween = m_tTargetStage.fTime - m_tPrevStage.fTime;
	float lambda = (m_fDuration - m_tPrevStage.fTime)/tween;
	m_d3dVelocity = ((m_tTargetStage.d3dVelocity - m_tPrevStage.d3dVelocity) * lambda) + m_tPrevStage.d3dVelocity + m_d3dRandVel;
	m_d3dCurVelocity = (m_d3dVelocity * fDelta);
	m_d3dPosition *= GetTranslateMatrix();
	m_d3dColor = ((m_tTargetStage.d3dColor - m_tPrevStage.d3dColor) * lambda) + m_tPrevStage.d3dColor;
	m_d3dCurScale = ((m_tTargetStage.d3dScale - m_tPrevStage.d3dScale) * lambda) + m_tPrevStage.d3dScale;
	m_d3dCurRotation = ((m_tTargetStage.d3dRotation - m_tPrevStage.d3dRotation) * lambda) + m_tPrevStage.d3dRotation;
	return 0;
}

void CParticle::Create(TParticleStruct tStruct)
{
	m_tPrevStage = tStruct.tStartStage;
	m_tTargetStage = tStruct.tStage;
	m_nRandomnessPos = tStruct.nRandomnessPos;
	m_nRandomnessVel = tStruct.nRandomnessVel;
	m_bIsAlive = true;
	m_fDuration = 0.0f;
	m_nCurStage = 0;
		m_d3dCurScale = m_tPrevStage.d3dScale;
		m_d3dCurRotation = m_tPrevStage.d3dRotation;
		m_d3dColor = m_tPrevStage.d3dColor;
		m_d3dVelocity = m_tPrevStage.d3dVelocity;
	if(m_nRandomnessPos)
	{
		m_d3dPosition = tStruct.d3dStartPos;
		m_d3dPosition[12] += (((rand()%m_nRandomnessPos)/10.0f)-(m_nRandomnessPos/20.0f));
		m_d3dPosition[13] += (((rand()%m_nRandomnessPos)/10.0f)-(m_nRandomnessPos/20.0f));
		m_d3dPosition[14] += (((rand()%m_nRandomnessPos)/10.0f)-(m_nRandomnessPos/20.0f));
	}
	else
		m_d3dPosition = tStruct.d3dStartPos;
	if(m_nRandomnessVel)
		m_d3dRandVel = D3DXVECTOR3((((rand()%m_nRandomnessVel)/1.0f)-(m_nRandomnessVel/2.0f)),(((rand()%m_nRandomnessVel)/1.0f)-(m_nRandomnessVel/2.0f)),(((rand()%m_nRandomnessVel)/1.0f)-(m_nRandomnessVel/2.0f)));
	else
		m_d3dRandVel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_d3dVelocity = m_tPrevStage.d3dVelocity + m_d3dRandVel;
}

void CParticle::SetNextStage(TStage tStage)
{
	m_tPrevStage = m_tTargetStage;
	m_tTargetStage = tStage;
}