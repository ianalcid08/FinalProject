/************************************************************************************
 * Filename:  		CollisionLibrary.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/09/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the list of collision functions/structs needed
 ************************************************************************************/
#ifndef _CCOLLISIONLIBRARY_H_
#define _CCOLLISIONLIBRARY_H_

#include <math.h>
#include <d3dx9math.h>
//#include "Camera.h"

enum FrustumCorners{ FTL = 0, FBL, FBR, FTR, NTL, NTR, NBR, NBL };
enum FrustumPlanes{ NEAR_PLANE = 0, FAR_PLANE, LEFT_PLANE, RIGHT_PLANE, TOP_PLANE, BOTTOM_PLANE };

struct AABB
{
	D3DXVECTOR4 m_d3dColor;
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
};

struct Plane
{
	D3DXVECTOR3 normal;
	float offset;
};

struct Frustum
{	
	Plane planes[6];
	D3DXVECTOR3 corners[8];
};

struct Segment
{
	D3DXVECTOR3 m_Start;
	D3DXVECTOR3 m_End;
};

struct Sphere
{
	D3DXVECTOR4 m_d3dColor;
	D3DXVECTOR3 m_Center;
	float m_Radius;
};

struct Capsule
{
	Segment m_Segment;
	float m_Radius;
};

struct Tesla
{
	Plane m_tPlanes[3];
};

struct Ray
{
	D3DXVECTOR3 m_Start;
	D3DXVECTOR3 m_Direction;
};

/*****************************************************************
	 * CheckMatrix():	Checks for a bad matrix
	 *
	 * Ins:					_d3dIn
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			03/02/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
bool CheckMatrix(D3DXMATRIX _d3dIn);
void ComputePlane(Plane& _tPlane, const D3DXVECTOR3& _d3dA, const D3DXVECTOR3& _d3dB, const D3DXVECTOR3& _d3dC);

/*****************************************************************
	 * SphereToSphere():	Checks Sphere to Sphere Collision
	 *
	 * Ins:					const Sphere& lhs, const Sphere& rhs
	 *
	 * Outs:				void
	 *
	 * Returns:				char
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
char SphereToSphere(const Sphere& lhs, const Sphere& rhs);

/*****************************************************************
	 * SphereToAABB():	Checks Sphere to AABB Collision
	 *
	 * Ins:					const Sphere& lhs, const AABB& rhs
	 *
	 * Outs:				void
	 *
	 * Returns:				char
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
char SphereToAABB(const Sphere& lhs, const AABB& rhs);
bool AABBtoAABB(const AABB& lhs, const AABB& rhs);


/*****************************************************************
	 * SphereToFrustum():	Checks Sphere to Frustum Collision
	 *
	 * Ins:					const Sphere& sphere, const Frustum& frustum
	 *
	 * Outs:				void
	 *
	 * Returns:				char
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
char SphereToFrustum(const Sphere& sphere, const Frustum& frustum);

/*****************************************************************
	 * SphereToFrustum():	Checks Sphere to Frustum Collision
	 *
	 * Ins:					const Sphere& sphere, const Frustum& frustum
	 *
	 * Outs:				void
	 *
	 * Returns:				char
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
char SphereToFrustumBehind(const Sphere& sphere, const Frustum& frustum);

/*****************************************************************
	 * TeslaToSphere():	Checks Tesla to Sphere Collision
	 *
	 * Ins:					const Tesla* tesla, const Sphere& sphere
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
bool TeslaToSphere(const Tesla& tesla, const Sphere& sphere);

/*****************************************************************
	 * RayToFrustum():	Checks Ray to Frustum Collision
	 *
	 * Ins:					const Ray& ray, const Frustum& frustum
	 *
	 * Outs:				void
	 *
	 * Returns:				char
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
char RayToFrustum(const Ray& ray, const Frustum& frustum);

/*****************************************************************
	 * CapsuleToSphere():	Checks Capsule to Sphere Collision
	 *
	 * Ins:					const Capsule& capsule, const Sphere& sphere
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere);


/*****************************************************************
	 * ClassifySphereToPlane():	Checks Sphere to plane Collision
	 *
	 * Ins:					const Plane& plane, const Sphere& sphere
	 *
	 * Outs:				void
	 *
	 * Returns:				int
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere);
/*****************************************************************
	 * ConfineToFrustum():	Keeps objects inside the frustum
	 *
	 * Ins:					const Frustum &_frustum,const Sphere &_sphere
	 *
	 * Outs:				void
	 *
	 * Returns:				D3DXVECTOR3
	 *
	 * Mod. Date:			02/18/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
D3DXVECTOR3 ConfineToFrustum(const Frustum &_frustum,const Sphere &_sphere);
/*****************************************************************
	 * MovingSphereToMovingSphere():	Collision for two moving spheres
	 *
	 * Ins:					const Sphere &_sphere0, const Sphere &_sphere1, const D3DXVECTOR3 &_vec1, const D3DXVECTOR3 &_vec2
	 *
	 * Outs:				void
	 *
	 * Returns:				int
	 *
	 * Mod. Date:			02/18/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
int MovingSphereToMovingSphere(const Sphere &_sphere0, const Sphere &_sphere1, const D3DXVECTOR3 &_vec1, const D3DXVECTOR3 &_vec2);
bool IntersectRaySphere(const D3DXVECTOR3 &p, const D3DXVECTOR3 &d, const D3DXVECTOR3 &center, float radius, float &t, D3DXVECTOR3 &q);

float magnitude(D3DXVECTOR3 vec);

#endif

