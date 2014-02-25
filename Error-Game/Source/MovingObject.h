/************************************************************************************
 * Filename:  		MovingObject.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that all moving game objects will inherit from.
 ************************************************************************************/
#ifndef _CMOVINGOBJECT_H_
#define _CMOVINGOBJECT_H_
#include "IBaseObject.h"

class CMovingObject : public IBaseObject
{
protected:
	D3DXVECTOR3 m_d3dVelocity;
	float m_fSpeed;

public:
	float GetSpeed() const { return m_fSpeed; }
	void SetSpeed(float _fS) { m_fSpeed = _fS; }

	D3DXVECTOR3 GetVelocity(){return m_d3dVelocity;}
	void SetVelocity(D3DXVECTOR3 d3dVelocity){m_d3dVelocity=d3dVelocity;}
	CMovingObject(void){};
	virtual ~CMovingObject(void) = 0 {};
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
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	virtual void Update(float fDelta) = 0;
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
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool CheckCollision(IBaseObject* obj) = 0;

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
	 * Mod. Initials:		MB
	 *****************************************************************/
	 void CollisionResponse(IBaseObject* obj) = 0;

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
	virtual void TakeDamage(IBaseObject* _pIn) = 0;
};

#endif