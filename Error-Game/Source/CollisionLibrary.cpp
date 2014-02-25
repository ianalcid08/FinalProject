/************************************************************************************
 * Filename:  		CollisionLibrary.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/18/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the list of collision functions/structs needed
 ************************************************************************************/

#include "CollisionLibrary.h"
#include "Definitions.h"
#include "AIBehaviors.h"

bool CheckMatrix(D3DXMATRIX _d3dIn)
{
	for( int i = 0; i < 16; ++i )
	{
		if( !(_d3dIn[i]==_d3dIn[i]) )
			return false;
	}

	return true;
}

void ComputePlane(Plane& _tPlane, const D3DXVECTOR3& _d3dA, const D3DXVECTOR3& _d3dB, const D3DXVECTOR3& _d3dC)
{
	D3DXVECTOR3 d3dNorm, d3dBA(_d3dB-_d3dA), d3dCB(_d3dC-_d3dB);
	D3DXVec3Cross(&d3dNorm, &d3dBA, &d3dCB);
	Utilities::FastNormalize(d3dNorm);

	_tPlane.normal = d3dNorm;
	_tPlane.offset = D3DXVec3Dot(&_tPlane.normal, &_d3dA);
}

 /*ClassifySphereToPlane

 Perform a sphere-to-plane test. 
 Returns 1 if the sphere is in front of the plane.
 Returns 2 if the sphere is behind the plane.
 Returns 3 if the sphere straddles the plane.*/
int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere)
{
	float TestVal = D3DXVec3Dot(&sphere.m_Center, &plane.normal) - plane.offset;
	if(TestVal > sphere.m_Radius)
		return 1;
	else if(TestVal < -sphere.m_Radius)
		return 2;
	else
		return 3;
}


 /*FrustumToSphere
 Perform a Sphere-to-Frustum check. Returns true if the sphere is outside. False if not.*/
char SphereToFrustum(const Sphere& sphere, const Frustum& frustum)
{
	for(int i = 2; i < 6; i++)
	{
		if(ClassifySphereToPlane(frustum.planes[i], sphere) > 1)
		{
			return i;
		}
	}
	return 0;
}

char SphereToFrustumBehind(const Sphere& sphere, const Frustum& frustum)
{
	for(int i = 2; i < 6; i++)
	{
		if(ClassifySphereToPlane(frustum.planes[i], sphere) == 2)
		{
			return 1;
		}
	}
	return 0;
}

bool TeslaToSphere(const Tesla& tesla, const Sphere& sphere)
{
	if( sphere.m_Center.y > 1.5f )
		return false;

	bool bInside[3] = { false, false, false };

	for( int i = 0; i < 3; ++i )
	{
		if( ClassifySphereToPlane(tesla.m_tPlanes[i], sphere) != 2 )
			bInside[i] = true;
	}

	return bInside[0] && bInside[1] && bInside[2];
}


// SphereToSphere
//
// Returns true if the Spheres collide. False if not.
char SphereToSphere(const Sphere& lhs, const Sphere& rhs)
{
	D3DXVECTOR3 V = rhs.m_Center - lhs.m_Center;
	float D = magnitude(V);

	if(D < (rhs.m_Radius + lhs.m_Radius)*(rhs.m_Radius + lhs.m_Radius))
	{
		if(lhs.m_Center.x < rhs.m_Center.x && lhs.m_Center.z < rhs.m_Center.z) //bottom left
			return 1;
		else if(lhs.m_Center.x < rhs.m_Center.x && lhs.m_Center.z > rhs.m_Center.z) //upper left
			return 2;
		else if(lhs.m_Center.x > rhs.m_Center.x && lhs.m_Center.z > rhs.m_Center.z) //upper right
			return 3;
		else
			return 4; //bottom right
	}
	else
		return 0;
}

// SphereToAABB
//
// Returns true if the sphere collides with the AABB. False if not.
char SphereToAABB(const Sphere& lhs, const AABB& rhs)
{
	D3DXVECTOR3 CP;

	//X
	if(lhs.m_Center.x < rhs.min.x)
		CP.x = rhs.min.x;
	else if(lhs.m_Center.x > rhs.max.x)
		CP.x = rhs.max.x;
	else
		CP.x = lhs.m_Center.x;

	//Y
	if(lhs.m_Center.y < rhs.min.y)
		CP.y = rhs.min.y;
	else if(lhs.m_Center.y > rhs.max.y)
		CP.y = rhs.max.y;
	else
		CP.y = lhs.m_Center.y;

	//Z
	if(lhs.m_Center.z < rhs.min.z)
		CP.z = rhs.min.z;
	else if(lhs.m_Center.z > rhs.max.z)
		CP.z = rhs.max.z;
	else
		CP.z = lhs.m_Center.z;

	D3DXVECTOR3 V = lhs.m_Center - CP;
	float D = magnitude(V);

	if(D < lhs.m_Radius)
		return 1;
	else
		return 0;

}

bool AABBtoAABB(const AABB& lhs, const AABB& rhs)
{
	if( lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x )
		return false;

	if( lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y )
		return false;

	if( lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z )
		return false;

	return true;
}

// CapsuleToSphere
//
// Returns true if the capsule collides with the sphere. False if not.
bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere)
{
	/*Sphere sphere0;
	sphere0.m_Center = capsule.m_Segment.m_Start;
	sphere0.m_Radius = capsule.m_Radius;
	Sphere sphere1;
	sphere1.m_Center = capsule.m_Segment.m_End;
	sphere1.m_Radius = capsule.m_Radius;
	

	if( SphereToSphere(sphere0, sphere) == 1 || SphereToSphere(sphere1, sphere) == 1)
		return true;
	else
		return false;*/
	D3DXVECTOR3 L = capsule.m_Segment.m_End - capsule.m_Segment.m_Start;
	D3DXVECTOR3 N = L;
	//N.normalize();
	D3DXVec3Normalize(&N,&L);
	D3DXVECTOR3 V = sphere.m_Center - capsule.m_Segment.m_Start;
	//float D = dot_product(N,V);
	float D = D3DXVec3Dot(&N,&V);

	D3DXVECTOR3 CP;
	if( D < 0)
		CP = capsule.m_Segment.m_Start;
	//else if( D > L.magnitude())
	else if ( (D * D) > magnitude(L))
		CP = capsule.m_Segment.m_End;
	else
	{
		D3DXVECTOR3 N1 = N * D;
		CP = capsule.m_Segment.m_Start + N1;
	}

	Sphere s1;
	s1.m_Center = CP;
	s1.m_Radius = capsule.m_Radius;

	
	if(SphereToSphere(s1, sphere))
		return true;
	else
		return false;

	//return SphereToSphere(s1,sphere);

}

bool IntersectRaySphere(const D3DXVECTOR3 &p, const D3DXVECTOR3 &d, const D3DXVECTOR3 &center, float radius, float &t, D3DXVECTOR3 &q)
{
	D3DXVECTOR3 m = p - center;
	float b = D3DXVec3Dot(&m, &d);
	float c = D3DXVec3Dot(&m, &m) - radius * radius;

	if( c > 0.0f && b > 0.0f )
		return false;

	float discr = b*b - c;

	if( discr < 0.0f )
		return false;

	t = -b - sqrt(discr);

	if( t < 0.0f )
		return false;

	q = p + d * t;

	return true;
}

float magnitude(D3DXVECTOR3 vec)
{
    float fMagnitude;
    fMagnitude = (vec.x*vec.x) + (vec.y*vec.y) + (vec.z*vec.z);
    return fMagnitude;
}
D3DXVECTOR3 ConfineToFrustum(const Frustum &_frustum,const Sphere &_sphere)
{
	D3DXVECTOR3 offset(0,0,0);
	float remain = 0.0f; // =  D3DXVec3Dot(&_sphere.m_Center, &plane.normal) - plane.offset;
	for(int i = 2; i < 6; i++)
	{
		if(ClassifySphereToPlane(_frustum.planes[i],_sphere) > 1)
		{
			remain = D3DXVec3Dot(&_sphere.m_Center, &_frustum.planes[i].normal) - _frustum.planes[i].offset;
			offset += (_sphere.m_Radius - remain) * _frustum.planes[i].normal;
			offset.y = 0.0f;
			/*return offset;*/
		
		}
	}
	return offset;
}
int MovingSphereToMovingSphere(const Sphere &_sphere0, const Sphere &_sphere1, const D3DXVECTOR3 &_vec0, const D3DXVECTOR3 &_vec1)
{
	D3DXVECTOR3 s = _sphere1.m_Center - _sphere0.m_Center;
	D3DXVECTOR3 v = _vec1 - _vec0;
	float r = _sphere0.m_Radius + _sphere1.m_Radius;
	float c = D3DXVec3Dot(&s,&s) - ( r * r );
	if(c < 0.0f)
	{
		return 1; // spheres were overlapping to begin with
	}
	float a = D3DXVec3Dot(&v,&v);
	if(a < EPISILON)
	{
		return 0; // spheres are not moving relative to each other
	}
	float b = D3DXVec3Dot(&v,&s);
	if(b >= 0.0f)
	{
		return 0; // spheres are not moving toward each other
	}
	float d = (b * b) - (a * c);
	if(d < 0.0f)
		return 0;

	return 1;
}