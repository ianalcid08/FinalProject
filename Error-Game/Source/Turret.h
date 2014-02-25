#ifndef _CTURRET_H_
#define _CTURRET_H_
/************************************************************************************
 * Filename:  		CTurret.h
 * Date:      		03/19/2013
 * Mod. Date: 		03/19/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Stationary enemy with a laser
 ************************************************************************************/
#include "enemyobject.h"
#include "BeamBullet.h"


class CObjectFactory;
class CRenderer;
class CEffect;

class CTurret : public CEnemyObject
{
protected:
	float						m_fDamage;
	float						m_fFireTimer;
	float						m_fRange;
	float						m_fKillTimer;
	CObjectFactory*				m_pOF;
	Capsule						m_cTurretCap;
	Sphere						m_sNextSphere;
	CRenderer					*m_pRenderer;
	LPD3DXMESH					EnemyBeamCylinderMesh;
	D3DXMATRIX					m_d3dNextMat;
	float						m_fSpawnTimer;
	float						m_fShotTimer;
	CEffect*					m_pLaser;
public:
	CTurret(void);
	virtual ~CTurret(void);

	D3DXMATRIX GetNextMatrix() {return m_d3dNextMat;}
	void SetDamage(float _dmg) {m_fDamage = _dmg;}
	void SetDelay(float _delay) {m_fSpawnTimer = _delay;}
	void SetShotTimer(float _timer) {m_fShotTimer = _timer;}
	void ResetEffect() {m_pLaser = NULL;}
	void SetKillTimer(float _timer) { m_fKillTimer = _timer;}


	float GetKillTimer()		{return m_fKillTimer;}
	float GetDelay()			{return m_fSpawnTimer;}
	float GetDamage()			{return m_fDamage;}
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
	 * Mod. Date:			03/19/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	virtual void Update(float fDelta);
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
	 * Mod. Date:			03/19/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	virtual bool CheckCollision(IBaseObject* pObj);

	/*****************************************************************
	 * CollisionResponse():	Responds to Collision depending on type
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/19/2013
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
	 * Mod. Date:			03/19/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	 virtual void TakeDamage(IBaseObject* _pIn);
};
#endif