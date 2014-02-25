#ifndef PICKUP_H_
#define PICKUP_H_
/***********************************************
 * Filename:  		Pickup.h
 * Date:      		2/12/2013
 * Mod. Date: 		2/20/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Class for all pickup objects
 ************************************************/
#include "ibaseobject.h"
#include <vector>
using std::vector;

class CObjectFactory;
class CPickup : public IBaseObject
{

private:
	int			m_nPickUpType;
	bool		m_bCollided;
	float		m_fLifetime;
	float		m_fRotation;
	float		m_fHover;
	D3DXMATRIX	m_d3dMatrix;
	D3DXMATRIX	m_d3dRotation;
	Frustum		m_playerFrustum;
	D3DXMATRIX	m_d3dTilt;

	// animation stuff
	float m_fAnimationTime;
	unsigned int m_unCurrAnimation;
	vector<D3DXMATRIX> m_vd3dCurrFrame;

public:
	CPickup(void);
	virtual ~CPickup(void);

	/***************
	 *  Accessors
	 ***************/
	int		GetGunType();
	bool	GetCollided()	{return m_bCollided;}
	float	GetLifetime()	{return m_fLifetime;}

	/***************
	 * Mutators
	 ***************/
	void SetPickupType(int fType)		{m_nPickUpType = fType;}
	void SetCollided(bool fCollided)	{m_bCollided = fCollided;}
	void SetLifeTime(float fLifetime)	{m_fLifetime = fLifetime;}
	void SetRotation(float fRotation)	{m_fRotation = fRotation;}

	/*****************************************************************
	 * Update():			Modifies the objects variables based on
	 *						time.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/012/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	virtual void Update(float fDelta);
	/*****************************************************************
	 * CheckCollision():	Modifies the objects variables based on
	 *						time.
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool CheckCollision(IBaseObject* obj);

	/*****************************************************************
	 * CollisionResponse():	Responds to Collision depending on type
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void CollisionResponse(IBaseObject* obj);

	/*****************************************************************
	 * TakeDamage():		Tells the IBaseObject to take damage
	 *
	 * Ins:					IBaseObject* _pIn
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/20/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	virtual void TakeDamage(IBaseObject* _pIn) {};

	/*****************************************************************
	 * Render():		Renders the pickup with weapon inside
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/15/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	virtual void Render();
};

#endif