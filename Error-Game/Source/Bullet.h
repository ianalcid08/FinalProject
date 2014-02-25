/************************************************************************************
 * Filename:  		Bullet.h
 * Date:      		02/11/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		The bullets fired by the player and some enemies
 ************************************************************************************/
#ifndef _CBULLET_H_
#define _CBULLET_H_

#include "MovingObject.h"

class CCrate;

class CBullet : public CMovingObject
{
protected:
	int m_nType;
	float fDamage;
	bool isActive;
	float m_fDuration;

public:
	CBullet(void);
	virtual ~CBullet(void);
  
	/*****************
	// Accessors
	*****************/
	int GetType(void) {return m_nType;}
	float GetDuration() const { return m_fDuration; }
	float GetDamage(void) {return fDamage;}
	
	/*****************
	// Mutators
	*****************/
	void SetType(int type) {m_nType = type;}
	void SetDamage(float fdamage) {fDamage = fdamage;}
	void SetDuration(float _fD) { m_fDuration = _fD; }

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
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
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
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	virtual bool CheckCollision(IBaseObject* pObj);

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
	 virtual void CollisionResponse(IBaseObject* pObj); 

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
	 virtual void TakeDamage(IBaseObject* _pIn){};

	 /*****************************************************************
	 * Render():			Renders Bullet with uniqe Bullet Data
	 *						
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 void Render();
};
#endif
