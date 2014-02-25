/************************************************************************************
 * Filename:  		CBarrel.h
 * Date:      		02/09/2013
 * Mod. Date: 		04/20/2013
 * Mod. Initials:	IA
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable barrel
 ************************************************************************************/
#ifndef _CBARREL_H_
#define _CBARREL_H_

#include "IBaseObject.h"

class CGame; 

class CBarrel : public IBaseObject
{
	//Sphere m_BarrelSphere;
	CGame*		m_pGame;
	float		m_fBarrelTimer;
	D3DXMATRIX m_d3dPreviousPos;
	bool m_bIsFalling;
	bool m_bResetPos;
	float m_fFallingTimer;
	float m_fDying;
	float m_fDieTime;
	float m_fIsFading;

public:
	//Accessor
	float GetBarrelTimer() {return m_fBarrelTimer;}
	bool GetIsFalling() {return m_bIsFalling;}
	float GetFallingTimer() {return m_fFallingTimer;}

	//Mutator
	void SetBarrelTimer(float fBarrelTimer) {m_fBarrelTimer = fBarrelTimer;}
	void SetIsFalling(bool _isFalling) {m_bIsFalling = _isFalling;}
	void SetFallingTimer(float _timer) {m_fFallingTimer = _timer;}
	void SetReset() {m_bResetPos = false;}

	CBarrel(void);
	~CBarrel(void);
	

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

