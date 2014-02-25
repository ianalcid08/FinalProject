/***********************************************
 * Filename:  		CCamera.cpp
 * Date:      		2/8/2013
 * Mod. Date: 		2/15/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Controls what you can see
 ************************************************/
#include "Camera.h"
#include "GameplayState.h"
#include "Game.h"
#include "IntroState.h"


void CCamera::Init()
{
	D3DXMatrixIdentity(&m_D3DCamera);
	m_D3DCamera[5] = 0;
	m_D3DCamera[6] = 1.0f;
	m_D3DCamera[9] = -1.0f;
	m_D3DCamera[10] = 0;

	D3DXMATRIX rot;
	D3DXMatrixRotationX(&rot, D3DXToRadian(-20.0f));
	m_D3DCamera *= rot;

	D3DXVECTOR3 StartPos(22.5f, 17.5f, -15.75f);
	*(D3DXVECTOR3*)&m_D3DCamera[12] = StartPos;

	m_fFov = (D3DXToRadian(60));
	m_fAspect = (16.0f/9.0f);
	m_fNear = 0.01f;
	m_fFar = 100.0f; 
	D3DXMatrixPerspectiveFovLH(&m_D3DProjMatrix,m_fFov,m_fAspect,m_fNear,m_fFar);

	m_tCurPoint.vPos = StartPos;
	m_tCurPoint.fTime = 0.0f;
	TWayPoint tempPoint = CGameplayState::GetInstance()->GetLeveData()->GetNextWaypoint();
	m_tNextPoint.vPos.x = m_tCurPoint.vPos.x + tempPoint.vPos.x;
	m_tNextPoint.vPos.z = m_tCurPoint.vPos.z + tempPoint.vPos.z;
	m_tNextPoint.fTime = tempPoint.fTime;

	m_pOM = CGame::GetInstance()->GetOM();
	m_bLocked = false;
	m_fCurTime = 0.0f;
}

#ifdef _DEBUG
void CCamera::MoveCamera(bool bUp)
{
	if(bUp)
		m_D3DCamera[13] += 0.5f;
	else
		m_D3DCamera[13] -= 0.5f;
}
#endif


void CCamera::Update(float fDelta)
{
	BuildFrustum(m_tFrustum,m_fFov,m_fNear,m_fFar,m_fAspect,m_D3DCamera);
	BuildFrustum(m_tPlayerConfine,m_fFov,m_fNear,m_fFar,m_fAspect,m_D3DCamera);

	bool bTutorialCheck = CIntroState::GetInstance()->GetTutorial();

	if(m_tNextPoint.fTime != 0.0f)
	{
		if(!bTutorialCheck)
			m_fCurTime += fDelta;

		if(m_bLocked == true)
		{
			unsigned int count = m_pOM->GetEnemyCount();

			if(count == 0 && m_fCurTime > m_tNextPoint.fTime && !bTutorialCheck)
			{
				m_fCurTime = 0.0f;
				m_tCurPoint = m_tNextPoint;
				TWayPoint tempPoint = CGameplayState::GetInstance()->GetLeveData()->GetNextWaypoint();
				m_tNextPoint.vPos.x = m_tCurPoint.vPos.x + tempPoint.vPos.x;
				m_tNextPoint.vPos.z = m_tCurPoint.vPos.z + tempPoint.vPos.z;
				m_tNextPoint.fTime = tempPoint.fTime;
				m_bLocked = false;
			}
		}
		else if(m_fCurTime > m_tNextPoint.fTime && m_tNextPoint.fTime > 0.0f && !bTutorialCheck)
		{
			m_fCurTime -= m_tNextPoint.fTime;
			m_tCurPoint = m_tNextPoint;
			TWayPoint tempPoint = CGameplayState::GetInstance()->GetLeveData()->GetNextWaypoint();
			m_tNextPoint.vPos.x = m_tCurPoint.vPos.x + tempPoint.vPos.x;
			m_tNextPoint.vPos.z = m_tCurPoint.vPos.z + tempPoint.vPos.z;
			m_tNextPoint.fTime = tempPoint.fTime;
		}
		
		if(m_tNextPoint.fTime < 0.0f)
		{
			m_tNextPoint.vPos = m_tCurPoint.vPos;
			m_tNextPoint.fTime *= -1;
			m_bLocked = !m_bLocked;

			if( CGameplayState::GetInstance()->GetHorde() )
				m_bLocked = false;
		}

		if(m_tNextPoint.fTime > 0.0f)
			InterpolateWayPoints();	
	}
}


void CCamera::ComputePlane(Plane &plane, const D3DXVECTOR3& pointA, const D3DXVECTOR3& pointB, const D3DXVECTOR3 &pointC)
{
	D3DXVECTOR3 norm;
	D3DXVec3Cross(&norm,&(pointB - pointA),&(pointC - pointB));
	D3DXVec3Normalize(&plane.normal, &norm);
	plane.offset = D3DXVec3Dot(&(plane.normal),&pointA);
}

D3DXMATRIX CCamera::GetViewProjMatrix()
{
	D3DXMATRIX view;
	float junk;
	D3DXMatrixInverse(&view, &junk, &m_D3DCamera);
	return view*m_D3DProjMatrix;
}

void CCamera::BuildFrustum( Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const D3DXMATRIX& camXform )
{
	D3DXVECTOR3 camXform_pos(camXform._41,camXform._42,camXform._43);
	D3DXVECTOR3 camXform_zaxis(camXform._31,camXform._32,camXform._33);
	D3DXVECTOR3 camXform_yaxis(camXform._21,camXform._22,camXform._23);
	D3DXVECTOR3 camXform_xaxis(camXform._11,camXform._12,camXform._13);

	D3DXVECTOR3 nc = camXform_pos + camXform_zaxis * nearDist;
	D3DXVECTOR3 fc = camXform_pos + camXform_zaxis * farDist;
	float Hnear = 2.0f * tan(fov*0.5f) * nearDist;
	float Hfar = 2.0f * tan(fov*0.5f) * farDist;
	float Wnear = Hnear * ratio;
	float Wfar = Hfar * ratio;

	// No sense dividing these for every corner
	Hfar = Hfar * 0.5f;
	Hnear = Hnear * 0.5f;
	Wfar = Wfar * 0.5f;
	Wnear = Wnear * 0.5f;

	frustum.corners[FTL] = fc + camXform_yaxis * (Hfar) - camXform_xaxis * (Wfar);
	frustum.corners[FBL] = fc - camXform_yaxis * (Hfar) - camXform_xaxis * (Wfar);
	frustum.corners[FBR] = fc - camXform_yaxis * (Hfar) + camXform_xaxis * (Wfar);
	frustum.corners[FTR] = fc + camXform_yaxis * (Hfar) + camXform_xaxis * (Wfar);

	frustum.corners[NTL] = nc + camXform_yaxis * (Hnear) - camXform_xaxis * (Wnear);
	frustum.corners[NBL] = nc - camXform_yaxis * (Hnear) - camXform_xaxis * (Wnear);
	frustum.corners[NBR] = nc - camXform_yaxis * (Hnear) + camXform_xaxis * (Wnear);
	frustum.corners[NTR] = nc + camXform_yaxis * (Hnear) + camXform_xaxis * (Wnear);

	//Attempt to lower top frustum
	if(&frustum == &m_tPlayerConfine)
	{
		frustum.corners[FTL][0] += 5.0f;
		frustum.corners[FTL][2] -= 30.0f;
		frustum.corners[FTR][0] -= 10.0f;
		frustum.corners[FTR][2] -= 30.0f;
		frustum.corners[FBR][0] -= 10.0f;
		frustum.corners[FBR][2] += 5.0f;
		frustum.corners[FBL][0] += 5.0f;
		frustum.corners[FBL][2] += 5.0f;
	}

	ComputePlane(frustum.planes[NEAR_PLANE],frustum.corners[NBR],frustum.corners[NBL],frustum.corners[NTL]);
	ComputePlane(frustum.planes[FAR_PLANE],frustum.corners[FBL],frustum.corners[FBR],frustum.corners[FTR]);
	ComputePlane(frustum.planes[LEFT_PLANE],frustum.corners[FBL],frustum.corners[NBL],frustum.corners[NTL]);
	ComputePlane(frustum.planes[RIGHT_PLANE],frustum.corners[NBR],frustum.corners[FBR],frustum.corners[FTR]);
	ComputePlane(frustum.planes[TOP_PLANE],frustum.corners[NTL],frustum.corners[NTR],frustum.corners[FTR]);
	ComputePlane(frustum.planes[BOTTOM_PLANE],frustum.corners[NBR],frustum.corners[NBL],frustum.corners[FBR]);
}

void CCamera::InterpolateWayPoints()
{
	D3DXVECTOR3 vPosition;

	float Lamda = m_fCurTime / m_tNextPoint.fTime;

	vPosition.x = m_tCurPoint.vPos.x + ((m_tNextPoint.vPos.x-m_tCurPoint.vPos.x)*Lamda);
	vPosition.z = m_tCurPoint.vPos.z + ((m_tNextPoint.vPos.z-m_tCurPoint.vPos.z)*Lamda);

	m_D3DCamera._41 = vPosition.x;
	m_D3DCamera._43 = vPosition.z;
}

void CCamera::ShutDown()
{
	m_tCurPoint.vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tCurPoint.fTime = 0.0f;
	m_tNextPoint = CGameplayState::GetInstance()->GetLeveData()->GetNextWaypoint();
	m_bLocked = false;
}
