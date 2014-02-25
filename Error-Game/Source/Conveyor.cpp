/************************************************************************************
 * Filename:  		Conveyor.cpp
 * Date:      		02/11/2013
 * Mod. Date: 		02/12/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the conveyor belt on the floor that will move the players/enemies around
 ************************************************************************************/

#include "Conveyor.h"
#include "Definitions.h"
#include "PlayerObject.h"

CConveyor::CConveyor(void)
{
	SetID(OBJ_CONVEYOR);
	m_fSpeed = 4.0f;
	SetAlive(true);
	m_bHalt = false;
}


CConveyor::~CConveyor(void)
{
}

void CConveyor::BuildAABB(D3DXVECTOR3 d3dMax, D3DXVECTOR3 d3dMin)
{
	D3DXMATRIX loc = GetMatrix();
	m_tConveyorAABB.max = d3dMax;
	m_tConveyorAABB.min = d3dMin;
}

void CConveyor::SetDirection(int nDir)
{
	m_nDirection = nDir;
}


void CConveyor::Update(float fDelta)
{
	if(m_bHalt)
		return;
	else if(m_fSmashStop < 0)
	{
		m_fDelta = fDelta;
		m_fScrollTime += fDelta * m_fSpeed;
	}
	else
	{
		m_fDelta = 0.0f;
		m_fSmashStop -= fDelta;
	}
}

bool CConveyor::CheckCollision(IBaseObject* pObj)
{
	return false;
}

void CConveyor::CollisionResponse(IBaseObject* pObj)
{
	if(m_bHalt)
		return;

	else if(pObj->GetID() == OBJ_SMASH || pObj->GetID() == OBJ_FLOOR)
	{
		m_fSmashStop = 1.0f;
	}

	if(m_fSmashStop > 0)
		return;
	float tempSpeed = m_fSpeed;

	if(pObj->GetID() == OBJ_TRASH)
		tempSpeed *= 0.5f;

	D3DXMATRIX tempMat = pObj->GetMatrix();
	switch(m_nDirection)
	{
	case CONV_UP:
		{
			*(D3DXVECTOR3*)&tempMat[12] += D3DXVECTOR3(0, 0, m_fDelta*tempSpeed);
		}
		break;
	case CONV_DOWN:
		{
			*(D3DXVECTOR3*)&tempMat[12] += D3DXVECTOR3(0, 0, -m_fDelta*tempSpeed);
		}
		break;
	case CONV_LEFT:
		{
			*(D3DXVECTOR3*)&tempMat[12] += D3DXVECTOR3(-m_fDelta*tempSpeed, 0, 0);
		}
		break;
	case CONV_RIGHT:
		{
			*(D3DXVECTOR3*)&tempMat[12] += D3DXVECTOR3(m_fDelta*tempSpeed, 0, 0);
		}
		break;
	}

	pObj->SetMatrix(tempMat);
}
