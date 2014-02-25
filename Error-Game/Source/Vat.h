/************************************************************************************
 * Filename:  		Vat.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable Vat that will spill molten steel
 ************************************************************************************/
#ifndef _CVAT_H_
#define _CVAT_H_

#include "IBaseObject.h"

class CVat : public IBaseObject
{
	float m_fVatTimer;
	float m_fDying;
	float m_fDieTime;
	float m_fIsFading;
public:
	CVat(void);
	~CVat(void);

	//Accessor
	float GetVatTimer() {return m_fVatTimer;}

	//Mutator
	void SetVatTimer(float fVatTimer) {m_fVatTimer = fVatTimer;}

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
};
#endif

