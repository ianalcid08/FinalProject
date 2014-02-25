/************************************************************************************
 * Filename:  		TrashSpawner.h
 * Date:      		04/10/2013
 * Mod. Date: 		04/10/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This object will act as another way to get trash bots onto the screen.
 ************************************************************************************/
#ifndef _CTRASHSPAWNER_H_
#define _CTRASHSPAWNER_H_
#include "MovingObject.h"

class CTrashSpawner : public CMovingObject
{
	float m_fMoveOnTime;
	float m_fMoveOffTime;
	float m_fSpawnTime;
public:
	//Mutator
	void SetMoveTime(float on, float off){m_fMoveOnTime = on, m_fMoveOffTime = off;}
	void SetSpawnTime(float spawn){m_fSpawnTime = spawn;}

	CTrashSpawner(void);
	~CTrashSpawner(void);
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
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		TS
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
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		TS
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
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		TS
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
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void TakeDamage(IBaseObject* _pIn);

	void Render(void);

	Sphere GetSphere(void);
};

#endif