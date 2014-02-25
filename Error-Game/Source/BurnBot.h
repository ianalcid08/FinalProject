/************************************************************************************
 * Filename:  		BurnBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the kamikaze enemy.
 ************************************************************************************/
#ifndef _CBURNBOT_H_
#define _CBURNBOT_H_
#include "EnemyObject.h"
#include "PlayerObject.h"

class CBurnBot : public CEnemyObject
{
	float m_fDying;
	float m_fDieTime;
	bool m_bCloseToTarget;
	CPlayerObject* m_pTarget;
	float m_fExplodeTimer;
	bool m_bDontDropPool;
	float m_fDropWait;

public:
	//Accessors
	CPlayerObject* GetTarget(void)	{return m_pTarget;}
	bool	GetCloseToTarget(void)	{return m_bCloseToTarget;}
	float GetExplodeTimer(void)		{return m_fExplodeTimer;}
	bool GetDontDrop(void)			{return m_bDontDropPool;}
	float GetDropWait(void)			{return m_fDropWait;}

	//Mutators
	void SetTarget(CPlayerObject* pTarget) {m_pTarget = pTarget;}
	void SetCloseToTarget(bool bCloseToTarget) {m_bCloseToTarget = bCloseToTarget;}
	void SetExplodeTimer(float fExplodeTimer) {m_fExplodeTimer = fExplodeTimer;}
	void SetDontDropPool(bool bDontDrop) {m_bDontDropPool = bDontDrop;}
	void SetDropWait(float fDropWait) {m_fDropWait = fDropWait;}
	
	void ResetDying() {m_fDying = 0.0f;}
	CBurnBot(void);
	virtual ~CBurnBot(void);
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
	 * Mod. Date:			02/09/2013
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
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		TS
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
	 * CheckTarget():		Checks the distance from target player to 
	 *						know when to attack.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	 void CheckTarget(void);
};

#endif