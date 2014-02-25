/************************************************************************************
 * Filename:  		DeathPit.h
 * Date:      		04/17/2013
 * Mod. Date: 		04/20/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		This is for the death pit object
 ************************************************************************************/
#ifndef _CDEATHPIT_H_
#define _CDEATHPIT_H_

#include "IBaseObject.h"




class CDeathPit : public IBaseObject
{
	float m_fFallingTimer;
	bool m_bIsFalling;
	bool m_bTrapOpen;
	bool m_bAnimate;

	float m_fDelayTimer;
	float m_fTrapOpenTimer;
	float m_fTrapCloseTimer;
	
public:
	CDeathPit(void);
	~CDeathPit(void);

	void SetDelayTimer(float _timer) {m_fDelayTimer = _timer;}
	void SetTrapOpenTimer(float _timer) { m_fTrapOpenTimer = _timer;}
	void SetTrapCloseTimer(float _timer) { m_fTrapCloseTimer = _timer;}
	void ResetAnim() {m_bAnimate = false;}
	void SetIsTrapOpen(bool _isOpen) {m_bTrapOpen = _isOpen;}

	float GetDelayTimer() {return m_fDelayTimer;}
	float GetTrapOpenTimer() {return m_fTrapOpenTimer;}
	float GetTrapCloseTimer() {return m_fTrapCloseTimer;}
	bool GetTrapIsOpen()		{return m_bTrapOpen;}

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
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void Update(float fDelta);

	/*****************************************************************
	 * BuildAABB():			Creates the bounds of the Aabb using the location
	 *						
	 *
	 * Ins:					d3dxvector3 d3dmin,d3dmax
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void BuildAABB(D3DXVECTOR3 d3dMax, D3DXVECTOR3 d3dMin);

	/*****************************************************************
	 * CheckCollision():	Checks the collision of the objects around it
	 *						
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
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
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
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
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	virtual void TakeDamage(IBaseObject* _pIn) {};

	/*****************************************************************
	 * Render():			Renders the deathpit
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/17/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	virtual void Render();
};

#endif 