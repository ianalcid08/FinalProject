/************************************************************************************
 * Filename:  		Crate.h
 * Date:      		02/09/2013
 * Mod. Date: 		04/20/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable crate
 ************************************************************************************/
#ifndef _CCRATE_H_
#define _CCRATE_H_

#include "IBaseObject.h"



class CGame;
class CCrate : public IBaseObject
{
	D3DXMATRIX m_d3dPreviousPos;
	float m_fDelayTimer;
	bool m_bResetPos;
	bool m_bIsFalling;
	float m_fFallingTimer;

public:
	float GetTimer() {return m_fDelayTimer;}
	bool GetIsFalling() {return m_bIsFalling;}
	float GetFallingTimer() {return m_fFallingTimer;}

	void SetDelay(float _delay) { m_fDelayTimer = _delay;}
	void SetIsFalling(bool _isFalling) {m_bIsFalling = _isFalling;}
	void SetFallingTimer(float _timer) {m_fFallingTimer = _timer;}
	void SetReset() {m_bResetPos = false;}

	CCrate(void);
	~CCrate(void);

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
	 * Mod. Initials:		MB
	 *****************************************************************/
	bool CheckCollision(IBaseObject* obj);

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
	 void CollisionResponse(IBaseObject* obj);

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

