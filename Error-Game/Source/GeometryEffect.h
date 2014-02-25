/**********************************************************************************************
 * Filename:  		GeometryEffect.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that uses meshes and textures for effects.
 **********************************************************************************************/
#ifndef _CGEOMETRYEFFECT_H_
#define _CGEOMETRYEFFECT_H_
#include "ISubEffect.h"
class CAssetManager;
class CGeometryEffect : public ISubEffect
{
	int m_nPrevStage;
	int m_nTargetStage;
	bool m_bIsAlive;
	bool m_bAnimated;
	float m_fAnimationTime;
	float m_fDuration;
	D3DXVECTOR3 m_d3dCurScale;
	D3DXVECTOR3 m_d3dCurRotation;
	unsigned int m_unCurAnimation;
	D3DXCOLOR m_d3dColor;
	D3DXMATRIX m_d3dBone;
	IDirect3DDevice9* m_pD3D;
	ID3DXEffect* m_d3dShader;
	CAssetManager* m_pAM;
public:
	//Mutator
	void SetColors(D3DXCOLOR d3dColor);
	CGeometryEffect(void);
	virtual ~CGeometryEffect(void);
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
	 * Mod. Date:			02/11/2013
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
	 * Mod. Date:			03/23/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Create(TGeometryStruct tStruct);
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
	void ReCreate(TGeometryStruct tStruct);

};

#endif