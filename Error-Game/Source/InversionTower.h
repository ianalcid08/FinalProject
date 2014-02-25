/************************************************************************************
 * Filename:  		InversionTower.h
 * Date:      		03/19/2013
 * Mod. Date: 		03/19/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the inversion tower that will invert player controls if 
					they are struck by its inversion wave.
 ************************************************************************************/
#ifndef _CINVERSIONTOWER_H_
#define _CINVERSIONTOWER_H_

#include "IBaseObject.h"
#include "Audio.h"


class CInversionTower : public IBaseObject
{
protected:

	float	m_fInversionTimer;
	float	m_fInversionAtktimer;
	float	m_fSpawnTimer;
	float	m_fMaxRadius;
	float	m_fAttackTime;
	float	m_fAttackSpeed;

public:
	CInversionTower(void);
	virtual ~CInversionTower(void);

	//Accessors
	float GetSpawnTimer() const {return m_fSpawnTimer;}
	float GetMaxRadius() const { return m_fMaxRadius; }
	float GetAttackTime() const { return m_fAttackTime; }
	float GetAttackSpeed() const { return m_fAttackSpeed; }

	//Mutators
	void SetSpawnTimer(float fSpawnTimer) {m_fSpawnTimer = fSpawnTimer;}
	void SetInversionTimer(float fInversionTimer) {m_fInversionTimer = fInversionTimer;}
	void SetInversionAtkTimer(float fInversionAtkTimer) {m_fInversionAtktimer = fInversionAtkTimer;}
	void SetMaxRadius(float _fR) { m_fMaxRadius = _fR; }
	void SetAttackTime(float _fT) { m_fAttackTime = _fT; }
	void SetAttackSpeed(float _fS) { m_fAttackSpeed = _fS; }


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
	 * Mod. Date:			02/09/2013
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
	 virtual void TakeDamage(IBaseObject* _pIn);
};
#endif