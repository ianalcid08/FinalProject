/************************************************************************************
 * Filename:  		AIFist.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the melee final boss.
 ************************************************************************************/
#ifndef _CAIFIST_H_
#define _CAIFIST_H_
#include "EnemyObject.h"

class CPlayerObject;
class CEffect;

class CAIFist : public CEnemyObject
{
	friend class CAICore;

	D3DXVECTOR3 m_d3dHome;
	Behavior m_pBehavior;
	float m_fTime;

	CEffect* m_pSmokeEffect;
	CEffect* m_pBurnEffect;
	CEffect* m_pFireEffect;

public:

	// Access
	Behavior		GetBehavior()	const { return m_pBehavior; }
	float			GetTime()		const { return m_fTime;		}
	D3DXVECTOR3		GetHome()		const { return m_d3dHome;	}
	
	// Mutate
	void SetBehavior(Behavior _pB)		{ m_pBehavior = _pB; }
	void SetTime(float _fT)				{ m_fTime = _fT;	 }
	void SetHome(D3DXVECTOR3 _d3dH)		{ m_d3dHome = _d3dH; }

	CAIFist(void);
	virtual ~CAIFist(void);
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
	virtual void TakeDamage(IBaseObject* _pIn);
};

#endif