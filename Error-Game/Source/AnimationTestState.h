#ifndef ANIMATIONTESTSTATE_H
#define ANIMATIONTESTSTATE_H
/***********************************************
 * Filename:  		AnimationTestState.h
 * Date:      		2/27/2013
 * Mod. Date: 		2/27/2013
 * Mod. Initials:	SD
 * Author:    		Sean Davis
 * Purpose:   		Used to test individual Animations,
 *					and animation system
 ************************************************/
#include "IGameState.h"
#include "Renderer.h"

class IBaseObject;
class CObjectFactory;
class CObjectManager;
class CViewManager;
class CAssetManager;

class CAnimationTestState : public IGameState
{
	CAnimationTestState();
	~CAnimationTestState();

	IBaseObject*		m_pCurrObject;
	CObjectFactory*		m_pOF;
	CObjectManager*		m_pOM;
	CViewManager*		m_pVM;
	CAssetManager*		m_pAM;
	IDirect3DDevice9*	m_pD3D;
	D3DXMATRIX			m_d3dCam;
	D3DXMATRIX			m_d3dProj;
	D3DXMATRIX			m_d3dRot;
	float				m_fCurrRotation;
	float				m_fIgnoreKeys;
	int					m_nCurrAnimation;
	float				m_fAnimModTime;
	bool				m_bKillAnimation;
	bool				m_bUseKeyFrame;
	int					m_nCurrKeyFrame;

	void LoadKeyFrame();

	int	m_nCurrObject;

public:
	static CAnimationTestState* GetInstance();

	/*****************************************************************
	* void Initialize(): Initializes all data in the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/27/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Initialize();

	/*****************************************************************
	* void Shutdown(): Handles all shutdown code for exiting the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/27/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Shutdown();

	/*****************************************************************
	* bool Input(): handles all input in the state
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/27/2013
	* Mod. Initials:	SD
	*****************************************************************/
	bool Input( void );

	/*****************************************************************
	* void Update( float fElapsedTime ): updates the state
	*
	* Ins:			    float fElapsedTime
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/27/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Update( float fElapsedTime );

	/*****************************************************************
	* void Render(): renders the state
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/27/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Render( void );
};

#endif