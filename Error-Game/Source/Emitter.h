/**********************************************************************************************
 * Filename:  		Emitter.h
 * Date:      		02/09/2013
 * Mod. Date: 		03/02/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that will create and update particles.
 **********************************************************************************************/
#ifndef _CEMITTER_H_
#define _CEMITTER_H_
#include "ISubEffect.h"
#include "Particle.h"
#include <vector>

class CEmitter : public ISubEffect
{
	//TODO: Move to manager
	TPointVertex m_tVertexData[4];
	IDirect3DDevice9* pD3D;
	IDirect3DVertexDeclaration9* d3dTempDecl;
	IDirect3DTexture9* d3dTempTexture;
	IDirect3DVertexBuffer9* d3dTempBuff;
	ID3DXEffect* d3dShader;

	CParticle m_cParticles[MAX_PARTICLES];
	std::vector<int> m_nAvailableParticles;
	float m_fSpawnTimer;
	float m_fSpawnRate;
	int m_nRandomPercentPos;
	int m_nRandomPercentVel;
public:
	//Mutators
	void SetColors(D3DXCOLOR d3dColor);
	void SetMatrix(D3DXMATRIX d3dPosition);
	CEmitter();
	virtual ~CEmitter();
	/*****************************************************************
	 * Init():				Initializes all of the data for the subeffect.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Init();
	/*****************************************************************
	 * ShutDown():			Deletes all necessary memory for an effect.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ShutDown();
	/*****************************************************************
	 * Update():			Updates the particles.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool Update(float fDelta);
	/*****************************************************************
	 * Update():			Renders the particles.
	 *
	 * Ins:					d3dCam,d3dProj,d3dForward
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Render(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj,D3DXMATRIX d3dForward);
	/*****************************************************************
	 * Create():			Sets all of the initial data for an effect.
	 *
	 * Ins:					tStruct
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/23/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Create(TEmitterStruct tStruct);
	/*****************************************************************
	 * ReCreate():			Resets all of the initial data for an effect.
	 *
	 * Ins:					tStruct
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/15/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ReCreate(TEmitterStruct tStruct);
};

#endif