/************************************************************************************
 * Filename:  		Conveyor.h
 * Date:      		02/11/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the conveyor belt on the floor that will move the players/enemies around
 ************************************************************************************/

#ifndef _CCONVEYOR_H_
#define _CCONVEYOR_H_

#include "IBaseObject.h"

enum eDirection {CONV_RIGHT, CONV_DOWN, CONV_LEFT, CONV_UP};

class CConveyor : public IBaseObject
{
	AABB m_tConveyorAABB;
	float m_fVelocity;
	float m_fScrollTime;
	float m_fSmashStop;
	float m_fDelta;
	int m_nDirection;
	float m_fSpeed;
	bool m_bHalt;

public:
	CConveyor(void);
	~CConveyor(void);

	/*****************
	*Accessors
	*****************/
	float GetVelocity(void) {return m_fVelocity;}
	float GetScrollTime(void) {return m_fScrollTime;}
	AABB GetAABB(void) {return m_tConveyorAABB;}

	/*****************
	*Mutators
	*****************/
	void SetScrollTime(float fTime) {m_fScrollTime = fTime;}
	void SetVelocity(float fVel) {m_fVelocity = fVel;}
	void ResetSmashStop() {m_fSmashStop = -1.0f;}
	void ToggleHalt() {m_bHalt = !m_bHalt;}
	void ResetHalt() {m_bHalt = false;}

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
	 * Update():			Creates the bounds of the Aabb using the location
	 *						
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/19/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void BuildAABB(D3DXVECTOR3 d3dMax, D3DXVECTOR3 d3dMin);

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
	 * SetDirection():	Updates the m_eDirection value, and rotates the
	 *                  World Matrix
	 *
	 * Ins:					eDirection eDir
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/19/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void SetDirection(int nDir);

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
	virtual void TakeDamage(IBaseObject* _pIn) {};


};
#endif

