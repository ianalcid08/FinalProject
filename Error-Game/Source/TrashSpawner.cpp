/************************************************************************************
 * Filename:  		TrashSpawner.cpp
 * Date:      		04/10/2013
 * Mod. Date: 		04/10/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This object will act as another way to get trash bots onto the screen.
 ************************************************************************************/
#include "TrashSpawner.h"
#include "Game.h"
#include "GameplayState.h"

CTrashSpawner::CTrashSpawner(void)
{
	SetID(OBJ_TUNNEL);
	m_fMoveOnTime = 1.0f;
	m_fMoveOffTime = 0.5f;
	m_fSpawnTime = 5.0f;
	D3DXMATRIX d3dMat;
	D3DXMatrixIdentity(&d3dMat);
	SetMatrix(d3dMat);
}

CTrashSpawner::~CTrashSpawner(void)
{

}

void CTrashSpawner::Update(float fDelta)
{
	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);

	if(m_fMoveOnTime > 0.0f)
	{
		m_fMoveOnTime -= fDelta;
		D3DXMATRIX d3dTranslate;
		D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, 2.0f*fDelta);
		SetMatrix(d3dTranslate * GetMatrix());
	}
	else if(m_fSpawnTime > 0.0f)
	{
		m_fMoveOnTime -= fDelta;
		m_fSpawnTime -= fDelta;
		if(m_fMoveOnTime < -1.1f)
		{
			CTrashBot* pTrash = nullptr;
			if(CGame::GetInstance()->GetOF()->Create(OBJ_TRASH, (IBaseObject**)&pTrash))
			{
				pTrash->SetMatrix(GetMatrix());
				pTrash->SetSpawnTimer(1.25f);
				m_fMoveOnTime = 0.0f;
			}
		}
	}
	else if(m_fMoveOffTime > 0.0f)
	{
		m_fMoveOffTime -= fDelta;
		D3DXMATRIX d3dTranslate;
		D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, -4.0f*fDelta);
		SetMatrix(d3dTranslate * GetMatrix());
	}
	else
	{
		SetAlive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
	}
}

bool CTrashSpawner::CheckCollision(IBaseObject* obj)
{

	return false;
}

void CTrashSpawner::CollisionResponse(IBaseObject* obj)
{

}

void CTrashSpawner::TakeDamage(IBaseObject* _pIn)
{

}

void CTrashSpawner::Render()
{
	CRenderer* pRenderer = CRenderer::GetInstance();
	pRenderer->Render(this);

	// Debug Sphere for testing
	/*D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	D3DXMATRIX d3dSphereMatrix = GetMatrix();
	d3dSphereMatrix._43 *= 0.85f;
	pRenderer->RenderSphere(d3dSphereMatrix, ViewProj, GetColor(), GetRadius() );*/
}

Sphere CTrashSpawner::GetSphere(void)
{
	Sphere tSphere = IBaseObject::GetSphere();
	tSphere.m_Center.z *= 0.85f;
	tSphere.m_Radius = GetRadius();
	tSphere.m_d3dColor = GetColor();
	return tSphere;
}