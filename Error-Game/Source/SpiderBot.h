#ifndef _SPIDERBOT_H_
#define _SPIDERBOT_H_

#include "enemyobject.h"

class CPlayerObject;

class CSpiderBot : public CEnemyObject
{
	D3DXVECTOR3 m_d3dAnchor;
	float m_fTime;
	bool m_bIsWalking;

public:

	// Access
	bool			GetIsWalking()	const { return m_bIsWalking; }
	D3DXVECTOR3		GetAnchor()		const { return m_d3dAnchor;	 }
	float			GetTime()		const { return m_fTime;		 }

	// Mutate
	void SetIsWalking(bool _bW)			{ m_bIsWalking = _bW;  }
	void SetAnchor(D3DXVECTOR3 _d3dA)	{ m_d3dAnchor = _d3dA; }
	void SetTime(float _fT)				{ m_fTime = _fT;	   }

	CSpiderBot(void);
	virtual ~CSpiderBot(void);

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
};

#endif