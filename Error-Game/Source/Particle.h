/**********************************************************************************************
 * Filename:  		Particle.h
 * Date:      		02/09/2013
 * Mod. Date: 		03/02/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the particle that emitters create.
 **********************************************************************************************/
#ifndef _CPARTICLE_H_
#define _CPARTICLE_H_
#include "EffectDefinitions.h"
class CParticle
{
	D3DXVECTOR3 m_d3dVelocity;
	D3DXVECTOR3 m_d3dCurVelocity;
	D3DXVECTOR3 m_d3dRandVel;
	D3DXVECTOR3 m_d3dCurScale;
	D3DXVECTOR3 m_d3dCurRotation;
	D3DXMATRIX m_d3dPosition;
	D3DXCOLOR m_d3dColor;
	bool m_bIsAlive;
	float m_fDuration;
	int m_nRandomnessPos;
	int m_nRandomnessVel;
	int m_nCurStage;
	TStage m_tPrevStage;
	TStage m_tTargetStage;
public:
	//Accessors
	bool IsAlive() {return m_bIsAlive;}
	D3DXMATRIX GetPosition() {return m_d3dPosition;}
	D3DXCOLOR GetColor() {return m_d3dColor;}
	D3DXMATRIX GetTranslateMatrix();
	D3DXMATRIX GetScaleMatrix();
	D3DXMATRIX GetRotateMatrix();
	//Mutators
	void SetPosition(D3DXMATRIX pos) {m_d3dPosition = pos;}
	void SetNextStage(TStage tStage);
	CParticle(void);
	~CParticle(void);
	/*****************************************************************
	 * Update():			Updates the particle.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/02/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	int Update(float fDelta);
	/*****************************************************************
	 * Create():			Creates a particle.
	 *
	 * Ins:					tStruct
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/02/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Create(TParticleStruct tStruct);
};

#endif