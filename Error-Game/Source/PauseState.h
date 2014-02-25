#ifndef _PAUSESTATE_H_
#define _PAUSESTATE_H_

/***********************************************
* Filename:  		PauseState.h
* Date:      		02/08/2013
* Mod. Date: 		03/18/2013
* Mod. Initials:	IA
* Author:    		Joseph Thomas
* Purpose:   		State for handling pause menu
************************************************/

#include "igamestate.h"
#include "Renderer.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;
class CBitmapFont;

class CPauseState : public IGameState
{
	struct TPauseVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};
	CPauseState(void);
	virtual ~CPauseState(void);
	CInput							*m_pInput;
	CGame							*m_pGame;
	CRenderer						*m_pRenderer;
	CAssetManager					*m_pAM;
	CBitmapFont						*m_pBMF;

	int								m_nChoice;
	int								slowdown;
	float							m_fTimer;

	RECT							m_rPauseBGRect;
	RECT							m_rPauseButtonRects[4];
	IDirect3DTexture9*				d3dPauseBG;
	IDirect3DVertexBuffer9*			m_d3dPauseBuff;
	ID3DXEffect*					m_d3dPauseShader;
	D3DXMATRIX						m_d3dMatrix;
	D3DXMATRIX						m_d3dOrthoMatrix;
	TPauseVertex					m_tPauseVerts[4];

	int								m_nfirstBoxX;
	int								m_nfirstBoxY;
	int								m_nletterHeight;
	int								m_nletterWidth;
	float							m_fLetterScale;
	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;

	// Mouse stuff
	float						m_fMouseX;
	float						m_fMouseY;
	D3DXMATRIX					m_d3dOffset[4];
	ID3DXEffect*				d3dCursorShader;
	TPauseVertex				tCursorVertex[4];
	IDirect3DTexture9*			d3dCursorTexture;
	IDirect3DVertexBuffer9*		d3dCursorBuffer;

public:
	static CPauseState*	GetInstance();

	/*****************************************************************
	* void Initialize(): Initializes all data in the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void	Initialize();

	/*****************************************************************
	* void Shutdown(): Handles all shutdown code for exiting the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void	Shutdown();

	/*****************************************************************
	* bool Input(): handles all input in the state
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	bool	Input();

	/*****************************************************************
	* void Update( float fElapsedTime ): updates the state
	*
	* Ins:			    float fElapsedTime
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void	Update( float fElapsedTime );

	/*****************************************************************
	* void Render(): renders the state
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void	Render();

	void	RenderNormalScene();
	void	RenderFadingScene();
};

#endif