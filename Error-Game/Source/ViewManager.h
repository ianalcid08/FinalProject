/**********************************************************************************************
 * Filename:  		ViewManager.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/13/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that render all the objects in the correct order.
 **********************************************************************************************/
#ifndef _CVIEWMANAGER_H_
#define _CVIEWMANAGER_H_
#include "Definitions.h"
#include "IBaseObject.h"
#include "Effect.h"

class CViewManager
{
	std::list<IBaseObject*> m_lRenderObjects[RG_NUM_GROUPS];
	bool	m_bFade;
	float	m_fFadeDelta;
	bool	m_bFadeIn;

	struct TFadeVertex
	{
		D3DXVECTOR3 d3dPosition;
	};

	TFadeVertex m_tFadeScreen[4];
	IDirect3DVertexBuffer9*  m_d3dBuffer;
	ID3DXEffect* m_d3dFadeShader;
	IDirect3DDevice9* m_pD3D;

public:
	CViewManager(void);
	~CViewManager(void);
	
	/*****************
	*Accessors
	*****************/
	bool GetFade(void) const {return m_bFade;}

	/*****************
	*Mutators
	*****************/
	void FadeIn(void);
	void FadeOut(void);

	/*****************************************************************
	 * Render():			Renders all of the objects and effects in
	 *						the right order.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void Render();
	/*****************************************************************
	 * Clear():				Clears all of the objects and effects out
	 *						of the lists.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void Clear();
	/*****************************************************************
	 * AddEffect():			Adds an effect to be rendered.
	 *
	 * Ins:					pEffect, nLayer
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void AddEffect(CEffect* pEffect, int nLayer);
	/*****************************************************************
	 * AddObject():			Adds an object to be rendered.
	 *
	 * Ins:					pObject, nLayer
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void AddObject(IBaseObject* pObject, eRenderGroups nLayer);
	/*****************************************************************
	 * RemoveEffect():		Removes an effect so it is no longer
	 *						rendered.
	 *
	 * Ins:					pEffect
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void RemoveEffect(CEffect* pEffect);

	/*****************************************************************
	 * RemoveObject():		Removes an effect so it is no longer
	 *						rendered.
	 *
	 * Ins:					pObject
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void RemoveObject(IBaseObject* pObject, eRenderGroups nLayer);

	void Initialize(void);

	/*****************************************************************
	 * UpdateFade():		Updates Fade based on current settings
	 *
	 * Ins:					float fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/28/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void UpdateFade(float fDelta);

	/*****************************************************************
	 * RenderFade():		Renders an alphaed black screen based on current
	 *						fade value
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/28/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void RenderFade(void);
};

#endif