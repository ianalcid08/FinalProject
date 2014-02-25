/************************************************************************************
 * Filename:  		SmashBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the heavy melee enemy.
 ************************************************************************************/
#ifndef _CSMASHBOT_H_
#define _CSMASHBOT_H_
#include "EnemyObject.h"

class CExplodingBullet;

enum ESMASHANIM {SMASH_WALK, SMASH_PREP, SMASH_SMASH, SMASH_RESET, SMASH_DEATH};

class CSmashBot : public CEnemyObject
{
	float fDamage;
	bool m_bIsSmashing;
	float m_fDying;
	float m_fDieTime;
	CExplodingBullet* m_pBullet;
	float m_fSmashTime;
	float m_fScrollTime;
	float m_fIsFading;
	float m_fMaxAnim;
	int m_nWalking;
public:
	CSmashBot(void);
	virtual ~CSmashBot(void);

	/***************
	 *  Mutators
	 ***************/
	void SetDamage(float fDAMAGE) {fDamage = fDAMAGE;}
	void SetSmash(bool _bS) { m_bIsSmashing = _bS; }
	void SetBullet(CExplodingBullet* _pB) { m_pBullet = _pB; }
	void SetSmashTime(float _fT) { m_fSmashTime = _fT; }
	void ResetDying() {m_fDying = 0.0f;}
	void SetMaxAnim(float animMax) {m_fMaxAnim = animMax;}
	void SetWalking(int nWalk) {m_nWalking = nWalk;}

	/***************
	 *  Accessors
	 ***************/
	float GetDamage(void)	{return fDamage;}
	bool GetSmashing() const { return m_bIsSmashing; }
	CExplodingBullet* GetBullet() const { return m_pBullet; }
	float GetSmashTime() const { return m_fSmashTime; }
	float GetScrollTime() {return m_fScrollTime;}
	float GetMaxAnim() {return m_fMaxAnim;}
	int GetWalking() {return m_nWalking;}

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
	 virtual void TakeDamage(IBaseObject* _pIn);

	 /*****************************************************************
	 * Render(): Enables the rendering of the scrolling base
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
	 virtual void Render();
};

#endif