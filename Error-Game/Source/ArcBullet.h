#ifndef _ARCBULLET_H_
#define _ARCBULLET_H_

#include "bullet.h"
class CArcBullet : public CBullet
{
	D3DXVECTOR3 m_d3dShrapnelDir[8];
	D3DXVECTOR3 m_d3dDir;
	D3DXVECTOR3 m_d3dPeak;

	Sphere m_tTargetSphere;
	unsigned int m_nFrame;
	bool m_bIsRising;
	bool m_bIsMolten;

public:

	/***************
	 *  Accessors
	 ***************/
	Sphere	GetTargetSphere()	const { return m_tTargetSphere; }
	bool	GetIsRising()		const { return m_bIsRising;		}
	bool	GetIsMolten()		const { return m_bIsMolten;		}

	/***************
	 *  Mutators
	 ***************/
	void	SetTargetSphere(Sphere tS)	{m_tTargetSphere = tS;}
	void	SetIsRising(bool bR)		{m_bIsRising = bR;}
	void	SetFrame(unsigned int nF)	{m_nFrame = nF;}
	void	SetIsMolten(bool _bM)		{m_bIsMolten = _bM;}

	CArcBullet(void);
	~CArcBullet(void);

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

	 /*****************************************************************
	 * Render():		Renders the arcing bullet
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/12/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 virtual void Render();
};

#endif