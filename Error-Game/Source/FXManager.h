/**********************************************************************************************
 * Filename:  		FXManager.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/25/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that will manage and create all of the effects.
 **********************************************************************************************/
#ifndef _CFXMANAGER_H_
#define _CFXMANAGER_H_
#include "Effect.h"
#include "EffectDefinitions.h"

class CFXManager
{
	std::vector<IDirect3DTexture9*> m_vTextures;
	CEffect m_cEffects[MAX_EFFECTS];
	std::vector<int> m_vAvailableIndex;
	std::vector<TEffectDefault> m_vDefaults;
	std::list<CEffect*> m_lActiveEffects;
	std::vector<CEffect*> m_vNeedReleased;
public:
	//Accessors
	IDirect3DTexture9* GetTexture(int id) {return m_vTextures[id];}
	CFXManager(void);
	~CFXManager(void);
	/*****************************************************************
	 * ReInit():			Reinitilizes all necessary variables to default values
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/05/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ReInit();
	/*****************************************************************
	 * CreateEffect():		Creates an effect to be presented and
	 *						updated.
	 *
	 * Ins:					nID, d3dMatrix
	 *
	 * Outs:				void
	 *
	 * Returns:				CEffect*
	 *
	 * Mod. Date:			02/25/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	CEffect* CreateEffect(int nID,D3DXMATRIX d3dMatrix);
	/*****************************************************************
	 * Update():			Removes an effect from view and stops
	 *						updating it.
	 *
	 * Ins:					pEffect
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/25/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ReleaseEffect(CEffect* pEffect);
	/*****************************************************************
	 * LoadEffects():		Loads effects from file.
	 *
	 * Ins:					szFilename
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/25/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool LoadEffects(char* szFilename);
	/*****************************************************************
	 * UpdateEffects():		Updates the effects.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void UpdateEffects(float fDelta);
	/*****************************************************************
	 * RenderEffects():		Render the effects.
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
	void RenderEffects(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj);
	/*****************************************************************
	 * ClearData():			Clears all of the data for re-initialization.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/25/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ClearData();
	/*****************************************************************
	 * ReloadData():		Reloads the data for an effect.
	 *
	 * Ins:					pEffect
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/15/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ReloadData(CEffect* pEffect);
};

#endif