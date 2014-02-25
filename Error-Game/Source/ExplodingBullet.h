#ifndef _EXPLODINGBULLET_H_
#define _EXPLODINGBULLET_H_

/************************************************************************************
 * Filename:  		Bullet.h
 * Date:      		02/11/2013
 * Mod. Date: 		03/05/2013
 * Mod. Initials:	JT
 * Author:    		Joe Thomas
 * Purpose:   		The bullets fired by the player and some enemies
 ************************************************************************************/

#include "bullet.h"

class CExplodingBullet : public CBullet
{
	float m_fSpeed;
	float m_fMaxRadius;
	std::list<IBaseObject*> m_lEnemies;

public:
	/***************
	 *  Mutators
	 ***************/
	void SetSpeed(float fS) { m_fSpeed = fS; }
	void SetMaxRadius(float fR) { m_fMaxRadius = fR; }

	void ResetTargets() { m_lEnemies.clear(); }

	CExplodingBullet(void);
	~CExplodingBullet(void);

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
	void Update(float fDelta);

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
	bool CheckCollision(IBaseObject* pObj);

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
	 void CollisionResponse(IBaseObject* pObj); 

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
	 void TakeDamage(IBaseObject* _pIn);

	 /*****************************************************************
	 * TakeDamage():		Renders the exploding bullet sphere
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/12/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 virtual void Render();
};

#endif