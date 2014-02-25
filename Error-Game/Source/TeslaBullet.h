#ifndef _TESLABULLET_H_
#define _TESLABULLET_H_

class CPlayerObject;

#include "bullet.h"
class CTeslaBullet : public CBullet
{
	Tesla m_tTesla;
	CPlayerObject* m_pOwner;
	D3DXMATRIX m_d3dFL;
	D3DXMATRIX m_d3dFR;

public:
	CTeslaBullet(void);
	~CTeslaBullet(void);

	void SetOwner(CPlayerObject* _pO) { m_pOwner = _pO; }
	D3DXMATRIX GetFL() const { return m_d3dFL; }
	D3DXMATRIX GetFR() const { return m_d3dFR; }

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