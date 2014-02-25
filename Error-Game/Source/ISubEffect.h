/**********************************************************************************************
 * Filename:  		ISubEffect.h
 * Date:      		02/09/2013
 * Mod. Date: 		03/14/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the base class that all forms of effects will inherit from.
 **********************************************************************************************/
#ifndef _ISUBEFFECT_H_
#define _ISUBEFFECT_H_
#include <d3dx9math.h>
#include "EffectDefinitions.h"
class ISubEffect
{
	float m_fStartTime;
	float m_fLife;
	int m_nMeshID;
	int m_nTextureID;
	bool m_bLooping;
	TStage m_tStages[MAX_STAGES];
	D3DXMATRIX m_d3dPosition;
	D3DXVECTOR3 m_d3dOffset;
	D3DXVECTOR3 m_d3dVelocity;
	//D3DXVECTOR3 m_d3dRandVelocity;
	DWORD m_dwSource;
	DWORD m_dwDest;
public:
	/***************
	 *  Accessors
	 ***************/
	float GetStart(){return m_fStartTime;}
	float GetLife(){return m_fLife;}
	//D3DXVECTOR3 GetRandVel(){return m_d3dRandVelocity;}
	int GetMeshID(){return m_nMeshID;}
	int GetTexture(){return m_nTextureID;}
	bool GetLoop(){return m_bLooping;}
	TStage GetStage(int nIndex){return m_tStages[nIndex];}
	D3DXMATRIX GetPosition(){return m_d3dPosition;}
	D3DXVECTOR3 GetOffset(){return m_d3dOffset;}
	D3DXVECTOR3 GetVelocity(){return m_d3dVelocity;}
	DWORD GetSourceBlend(){return m_dwSource;}
	DWORD GetDestBlend(){return m_dwDest;}
	/***************
	 *  Mutators
	 ***************/
	void SetPosition(D3DXMATRIX d3dPosition){m_d3dPosition = d3dPosition;}
	//void SetRandVel(D3DXVECTOR3 d3dRand){m_d3dRandVelocity = d3dRand;}
	void SetOffset(D3DXVECTOR3 d3dOffset){m_d3dOffset = d3dOffset;}
	void SetVelocity(D3DXVECTOR3 d3dVel){m_d3dVelocity = d3dVel;}
	void SetLife(float fLife) {m_fLife = fLife;}
	void SetStart(float fStart) {m_fStartTime = fStart;}
	void SetStage(int nIndex, TStage tStage) {m_tStages[nIndex] = tStage;}
	virtual ~ISubEffect() = 0 {};
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
	virtual void Init() = 0;
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
	virtual void ShutDown() = 0;
	/*****************************************************************
	 * Update():			Updates the sub-effect and the life.
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
	virtual bool Update(float fDelta) = 0;
	/*****************************************************************
	 * Update():			Renders the sub-effect.
	 *
	 * Ins:					d3dCam,d3Proj,d3dForward
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	virtual void Render(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj,D3DXMATRIX d3dForward) = 0;
	/*****************************************************************
	 * Create():			Sets all of the initial data for an effect.
	 *
	 * Ins:					fStartTime,fLife,nMeshID,nTextureID,
	 *						bLooping,tStages,d3dPosition,
	 *						d3dVelocity,dwSource,dwDest
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/14/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	virtual void Create(float fStartTime,float fLife,int nMeshID,int nTextureID,bool bLooping,TStage tStages[MAX_STAGES],D3DXMATRIX d3dPosition,D3DXVECTOR3 d3dOffset,
		D3DXVECTOR3 d3dVelocity,DWORD dwSource,DWORD dwDest)
	{
		m_fStartTime = fStartTime;
		m_fLife = fLife;
		m_nMeshID = nMeshID;
		m_nTextureID = nTextureID;
		m_bLooping = bLooping;
		if(tStages!=nullptr)
		{
			for(int i = 0; i < MAX_STAGES; i++)
				m_tStages[i] = tStages[i];
		}
		m_d3dPosition = d3dPosition;
		m_d3dOffset = d3dOffset;
		m_d3dVelocity = d3dVelocity;
		m_dwSource = dwSource;
		m_dwDest = dwDest;
	}
};

#endif