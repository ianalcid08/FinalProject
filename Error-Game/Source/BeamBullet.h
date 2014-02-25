/************************************************************************************
 * Filename:  		BeamBullet.h
 * Date:      		03/05/2013
 * Mod. Date: 		03/06/2013
 * Mod. Initials:	IA
 * Author:    		Joe Thomas
 * Purpose:   		The bullets fired by the player and some enemies
 ************************************************************************************/
#ifndef _BEAMBULLET_H_
#define _BEAMBULLET_H_

#include "bullet.h"

class CPlayerObject;

class CBeamBullet : public CBullet
{
private:
	Capsule m_beamCapsule;
	Sphere m_sNextSphere;
	float m_fLifeTime;
	float m_fLength;
	
public:
	CBeamBullet(void);
	~CBeamBullet(void);

	/*****************
	// Mutators
	*****************/
	void SetLifeTime(float fLifeTime) { m_fLifeTime = fLifeTime;}

	/*****************
	// Accessors
	*****************/
	Sphere GetNextSphere(void) { return m_sNextSphere;}
	float GetLifeTime() {return m_fLifeTime;}

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
	 * Render():		Renders the beam cylinder
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/11/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 virtual void Render();

	 /*****************************************************************
	 * CreateCapsule():		Creates Beam cylinder
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/11/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 void CreateCapsule();
};

#endif