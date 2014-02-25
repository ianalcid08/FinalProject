/************************************************************************************
 * Filename:  		EnemyObject.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that all enemy game objects will inherit from.
 ************************************************************************************/
#ifndef _CENEMYOBJECT_H_
#define _CENEMYOBJECT_H_
#include "MovingObject.h"
#include "EnemyAI.h"

class CEnemyObject : public CMovingObject
{
	//Sphere	m_EnemySphere;
	float	m_fHitDuration;
	bool	m_bWasHit;
protected:
	CEnemyAI m_cAI;
	IBaseObject* m_pTargetFINAL;
	//char m_cAvoidFlags;
	IBaseObject* m_pCloseTarget;
	float m_fCloseDist;

public:
	IBaseObject* GetTargetFINAL() const { return m_pTargetFINAL; }
	void SetTargetFINAL(IBaseObject* _pT) { m_pTargetFINAL = _pT; }
	//char GetAvoidFlags() const { return m_cAvoidFlags; }
	//void SetAvoidFlags(char _cF) { m_cAvoidFlags = _cF; }
	IBaseObject* GetCloseTarget() const { return m_pCloseTarget; }
	float GetCloseDist() const { return m_fCloseDist; }
	void SetCloseTarget(IBaseObject* _pT) { m_pCloseTarget = _pT; }
	void SetCloseDist(float _fD) { m_fCloseDist = _fD; }

	CEnemyObject(void) : m_pTargetFINAL(nullptr), m_pCloseTarget(nullptr), m_fCloseDist(0.0f) {};
	//Sphere GetSphere(){return m_EnemySphere;}
	//void SetSphere(Sphere sphere){m_EnemySphere = sphere;}
	float GetHitDuration(){return m_fHitDuration;}
	void SetHitDuration(float fHitDuration) {m_fHitDuration = fHitDuration;}
	bool GetWasHit(){return m_bWasHit;}
	void SetWasHit(bool bWasHit){m_bWasHit = bWasHit;}
	virtual ~CEnemyObject(void) = 0 {};
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