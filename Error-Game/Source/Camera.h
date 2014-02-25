#ifndef CAMERA_H
#define CAMERA_H
/***********************************************
 * Filename:  		CCamera.h
 * Date:      		2/8/2013
 * Mod. Date: 		2/15/2013
 * Mod. Initials:	JT
 * Author:    		Ian Alcid
 * Purpose:   		Controls what you can see
 ************************************************/
#include <d3dx9math.h>
#include "CollisionLibrary.h"
#include "LevelData.h"
#include "ObjectManager.h"

class CCamera
{
	D3DXMATRIX m_D3DCamera;
	D3DXMATRIX m_D3DProjMatrix;

	float m_fNear;
	float m_fFar;
	float m_fFov;
	float m_fAspect;
	Frustum m_tFrustum;
	Frustum m_tPlayerConfine;

	TWayPoint m_tCurPoint;
	TWayPoint m_tNextPoint;
	float m_fCurTime;
	bool m_bLocked;

	CObjectManager*  m_pOM;

public:
#ifdef _DEBUG
	// TEST code for zooming out Camera to a better position
	void MoveCamera(bool bUp = true);
#endif
	
	void Update(float fDelta);
	/*****************
	*Accessors
	*****************/
	D3DXMATRIX GetCamera() {return m_D3DCamera;}
	float GetNextTime() {return m_tNextPoint.fTime;}

	/*****************
	*Mutators
	*****************/
	void SetCamera(D3DXMATRIX& matIn) {m_D3DCamera = matIn;}
	void SetNextWaypoint(TWayPoint wpIn) {m_tNextPoint = wpIn;}
	void SetCurrWaypoint(TWayPoint wpIn) {m_tCurPoint = wpIn;}
	void ResetCurrTime() {m_fCurTime = 0;}
	void Unlock() {m_bLocked = false;}
	
	/*****************************************************************
	* void ComputePlane(): Calculates one plane.
	*
	* Ins:			    Plane plane,D3DXVECTOR3 pointA, D3DXVECTOR3 pointB,D3DXVECTOR3 pointC
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/15/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void ComputePlane(Plane &plane, const D3DXVECTOR3& pointA, const D3DXVECTOR3& pointB, const D3DXVECTOR3 &pointC);
	/*****************************************************************
	* void BuildFrustum(): Builds frustum based on 6 planes.
	*
	* Ins:			     Frustum frustum, float fov, float nearDist, float farDist, float ratio, D3DXMATRIX camXform
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/15/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void BuildFrustum( Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const D3DXMATRIX& camWorld );

	/*****************************************************************
	* const D3DXMATRIX* GetViewProjMatrix(): Returns the ViewProj Matrix.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    D3DXMATRIX
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	SD
	*****************************************************************/
	D3DXMATRIX GetViewProjMatrix();

	/*****************************************************************
	* void Init(): Sets initial values to parameters
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Init();
	
	/*****************
	*Accessors
	*****************/
	Frustum GetFrustum() const { return m_tFrustum; }
	Frustum GetPlayerFrustum() const {return m_tPlayerConfine;}


	/*****************************************************************
	* InterpolateWayPoints: Interpolates the camera between 2 waypoints
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void InterpolateWayPoints();


	/*****************************************************************
	* ShutDown:			Resets the camera to its starting position
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void ShutDown();
};

#endif