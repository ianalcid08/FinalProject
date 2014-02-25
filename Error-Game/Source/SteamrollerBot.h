/************************************************************************************
 * Filename:  		SteamrollerBot.h
 * Date:      		02/09/2013
 * Mod. Date: 		03/15/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the charging mid-boss.
 ************************************************************************************/
#ifndef _CSTEAMROLLERBOT_H_
#define _CSTEAMROLLERBOT_H_
#include "EnemyObject.h"
#include "Definitions.h"

class CPlayerObject;

enum ENIM {ASTEAM_CHARGE, ASTEAM_ATTACK, ASTEAM_RICO, ASTEAM_DEATH};

class CSteamrollerBot : public CEnemyObject
{
	D3DXVECTOR3 m_d3dImpactPoint;
	Behavior m_pBehavior;
	float m_fTime;
	float m_fSwitchTime;
	float m_fTargetTime;
	float m_fPickupTime;
	float m_fDamage;
	float m_fStunTime;
	float m_fMaxHP;
	bool m_bEnraged;
	float m_fDying;

	D3DXMATRIX m_d3dWheelCurrFrame;
	int m_nWheelAnim;

public:

	// Access
	Behavior		GetBehavior()	const { return m_pBehavior;		 }
	float			GetTime()		const { return m_fTime;			 }
	float			GetTargetTime() const { return m_fTargetTime;	 }
	D3DXVECTOR3		GetImpact()		const { return m_d3dImpactPoint; }
	float			GetSwitchTime() const { return m_fSwitchTime;	 }
	float			GetPickupTime() const { return m_fPickupTime;	 }
	float			GetDamage()		const { return m_fDamage;		 }
	float			GetStunTime()	const { return m_fStunTime;		 }
	float			GetMaxHP()		const { return m_fMaxHP;		 }
	bool			GetIsEnraged()	const { return m_bEnraged;		 }
	
	// Mutate
	void SetBehavior(Behavior _pB)		{ m_pBehavior = _pB;		}
	void SetTime(float _fT)				{ m_fTime = _fT;			}
	void SetTargetTime(float _fT)		{ m_fTargetTime = _fT;		}
	void SetImpact(D3DXVECTOR3 _d3dI)	{ m_d3dImpactPoint = _d3dI; }
	void SetSwitchTime(float _fT)		{ m_fSwitchTime = _fT;		}
	void SetPickupTime(float _fT)		{ m_fPickupTime = _fT;		}
	void SetDamage(float _fD)			{ m_fDamage = _fD;			}
	void SetStunTime(float _fT)			{ m_fStunTime = _fT;		}
	void SetMaxHP(float _fHP)			{ m_fMaxHP = _fHP;			}
	void SetIsEnraged(bool _bE)			{ m_bEnraged = _bE;			}

	CSteamrollerBot(void);
	virtual ~CSteamrollerBot(void);
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

	 /*****************************************************************
	 * Render():		    Render steamroller bot
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/15/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	 virtual void Render();
};

#endif