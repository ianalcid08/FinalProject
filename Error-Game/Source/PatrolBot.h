/************************************************************************************
 * Filename:  		CPatrolBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the ranged enemy.
 ************************************************************************************/
#ifndef _CPatrolBot_H_
#define _CPatrolBot_H_
#include "EnemyObject.h"

class CPatrolBot : public CEnemyObject
{
	char m_nSideCollided;
	float m_fFireTime;
	float m_fDying;
	float m_fDieTime;
	float m_fIsFading;
	bool m_bIsSpawning;
	
public:
	float GetFireTime() const { return m_fFireTime; }
	bool GetIsSpawning() const {return m_bIsSpawning;}

	void SetIsSpawning(bool _spawn) {m_bIsSpawning = _spawn;}
	void SetFireTime(float _fT) { m_fFireTime = _fT; }


	void ResetDying() {m_fDying = 0.0f;}
	CPatrolBot(void);
	virtual ~CPatrolBot(void);
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
};

#endif