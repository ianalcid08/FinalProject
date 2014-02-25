/**********************************************************************************************
 * Filename:  		Effect.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/25/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that pulls several effects together to create one effect.
 **********************************************************************************************/
#ifndef _CEFFECT_H_
#define _CEFFECT_H_
#include "Definitions.h"
#include "Emitter.h"
#include "GeometryEffect.h"

class CEffect
{
	int m_nID;
	CEmitter m_cEmitters[MAX_EMITTERS];
	CGeometryEffect m_cGeometries[MAX_GEOMETRIES];
	float m_fLifeTime;
	bool m_bLooping;
	int m_nEmitters;
	int m_nGeometries;
	D3DXMATRIX m_d3dMatrix;
	D3DXVECTOR3 m_d3dOffset;
	D3DXVECTOR3 m_d3dVelocity;
public:

	/***************
	 *  Accessors
	 ***************/
	float GetLife(){return m_fLifeTime;}
	bool GetLoop(){return m_bLooping;}
	D3DXVECTOR3 GetPosition(){return *(D3DXVECTOR3*)&m_d3dMatrix[12];}
	D3DXMATRIX GetMatrix(){return m_d3dMatrix;}
	CEmitter* GetEmitter(int index){return &m_cEmitters[index];}
	CGeometryEffect* GetGeometry(int index){return &m_cGeometries[index];}
	int GetID(){return m_nID;}
	/***************
	 *  Mutators
	 ***************/
	void SetLife(float fLife){m_fLifeTime = fLife;}
	void SetOffset(float x,float y,float z) {m_d3dOffset=D3DXVECTOR3(x,y,z);}
	void SetOffset(D3DXVECTOR3 pos) {m_d3dOffset=pos;}
	void SetVelocity(D3DXVECTOR3 vel) {m_d3dVelocity=vel;}
	void SetMatrix(D3DXMATRIX mat);
	void SetColors(D3DXCOLOR d3dColor);
	void SetLoop(bool bLoop){m_bLooping = bLoop;}
	void SetID(int nID){m_nID = nID;}
	CEffect(void);
	~CEffect(void);
	/*****************************************************************
	 * Update():			Updates the sub-effects and the life time.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool Update(float fDelta);
	/*****************************************************************
	 * RenderEmitt():		Render the emitters.
	 *
	 * Ins:					d3dCam,d3dProj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void RenderEmitt(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj);
	/*****************************************************************
	 * RenderGeo():			Render the geometry.
	 *
	 * Ins:					d3dCam,d3dProj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void RenderGeo(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj);
	/*****************************************************************
	 * AddEmitter():		Adds an emitter to the list of
	 *						sub-effects for this effect.
	 *
	 * Ins:					pEffect
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/05/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void AddEmitter(CEmitter pEffect);
	/*****************************************************************
	 * AddGeometry():		Adds an emitter to the list of
	 *						sub-effects for this effect.
	 *
	 * Ins:					pEffect
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/05/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void AddGeometry(CGeometryEffect pEffect);
	/*****************************************************************
	 * Reset():				Returns the variables to default values
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
	void Reset();
};

#endif