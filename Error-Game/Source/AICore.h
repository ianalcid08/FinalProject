/************************************************************************************
 * Filename:  		AICore.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the shooting final boss.
 ************************************************************************************/
#ifndef _CAICORE_H_
#define _CAICORE_H_
#include "EnemyObject.h"

class CAIFist;
class CPlayerObject;
class CEffect;

class CAICore : public CEnemyObject
{
	CAIFist* m_pLeftFist;
	CAIFist* m_pRightFist;

	Behavior m_pBehavior;
	Behavior m_pMoveBehavior;

	D3DXVECTOR3 m_d3dBurstLoc[3];

	int m_nBurstCount;

	float m_fFireTime;
	float m_fFireRate;
	float m_fTime;
	float m_fBurstTime;

	CEffect* m_pSmokeEffect;
	CEffect* m_pBurnEffect;
	CEffect* m_pFireEffect;

	bool m_bCanBeHit;

public:

	// Access
	Behavior		GetBehavior()				const { return m_pBehavior;			   }
	float			GetFireTime()				const { return m_fFireTime;			   }
	CAIFist*		GetLeftFist()				const { return m_pLeftFist;			   }
	CAIFist*		GetRightFist()				const { return m_pRightFist;		   }
	float			GetFireRate()				const { return m_fFireRate;			   }
	float			GetTime()					const { return m_fTime;				   }
	float			GetBurstTime()				const { return m_fBurstTime;		   }
	int				GetBurstCount()				const { return m_nBurstCount;		   }
	D3DXVECTOR3		GetBurstLoc(int _nIndex)	const { return m_d3dBurstLoc[_nIndex]; }
	bool			GetCanBeHit()				const { return m_bCanBeHit;			   }
	Behavior		GetMoveBehavior()			const { return m_pMoveBehavior;		   }

	// Mutate
	void	SetBehavior(Behavior _pB)						{ m_pBehavior = _pB;				}
	void	SetFireTime(float _fT)							{ m_fFireTime = _fT;				}
	void	SetLeftFist(CAIFist* _pF)						{ m_pLeftFist = _pF;				}
	void	SetRightFist(CAIFist* _pF)						{ m_pRightFist = _pF;				}
	void	SetFireRate(float _fR)							{ m_fFireRate = _fR;				}
	void	SetTime(float _fT)								{ m_fTime = _fT;					}
	void	SetBurstTime(float _fT)							{ m_fBurstTime = _fT;				}
	void	SetBurstCount(int _nB)							{ m_nBurstCount = _nB;				}
	void	SetBurstLoc(int _nIndex, D3DXVECTOR3 _d3dLoc)	{ m_d3dBurstLoc[_nIndex] = _d3dLoc;	}

	CAICore(void);
	virtual ~CAICore(void);

	void Initialize();
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

	 virtual void Render();
};

#endif