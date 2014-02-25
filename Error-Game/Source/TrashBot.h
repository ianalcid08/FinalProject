/************************************************************************************
 * Filename:  		TrashBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		4/20/2013
 * Mod. Initials:	IA
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the basic melee enemy.
 ************************************************************************************/
#ifndef _CTRASHBOT_H_
#define _CTRASHBOT_H_
#include "EnemyObject.h"


class CObjectFactory;
class CPlayerObject;

class CTrashBot : public CEnemyObject
{
	float fDamage;
	float m_fDying;
	float m_fDieTime;
	float m_fIsFading;
	float m_fFallingTimer;
	bool m_bCollided;
	bool m_bStop;
	bool m_bPlayerStop;
	float m_fPlayerStopTimer;
	bool m_bIsFalling;
	float m_fSpawnTimer;

	CObjectFactory* m_pOF;
public:
	CTrashBot(void);
	virtual ~CTrashBot(void);

	void SetDamage(float fDAMAGE) {fDamage = fDAMAGE;}
	void SetFading(float _fade) { m_fIsFading = _fade;}
	void SetFallingTimer(float _timer) { m_fFallingTimer = _timer;}
	void SetCollided(bool bCollided) {m_bCollided = bCollided;}
	void SetPlayerStop(bool bPlayerStop) {m_bPlayerStop = bPlayerStop;}
	void SetIsFalling(bool _isfalling) { m_bIsFalling = _isfalling;}
	void SetSpawnTimer(float fSpawnTimer) {m_fSpawnTimer = fSpawnTimer;}
	
	float GetDamage(void)	{return fDamage;}
	bool GetCollided(void) {return m_bCollided;}
	bool GetPlayerStop() const {return m_bPlayerStop;}
	float GetFallingTimer() const {return m_fFallingTimer;}
	bool GetIsFalling() const {return m_bIsFalling;}
	void ResetDying() {m_fDying = 0.0f;}
	float GetSpawnTimer() const {return m_fSpawnTimer;}
	

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
	* void Render(): renders the state
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		04/20/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	Render();
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
	 virtual void TakeDamage(IBaseObject* _pIn);

	 void InterpolateCollisions(float fDelta);
};

#endif
