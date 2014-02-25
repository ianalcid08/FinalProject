#ifndef _RESETSTATE_H_
#define _RESETSTATE_H_

/***********************************************
 * Filename:  		ResetState.h
 * Date:      		02/08/2013
 * Mod. Date: 		03/08/2013
 * Mod. Initials:	IA
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling game reset on death
 ************************************************/

#include "igamestate.h"
#include "Renderer.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;
class CBitmapFont;

#define FIRSTBOXX 375
#define FIRSTBOXY 300

class CResetState : public IGameState
{
	struct TResetVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};

	CResetState(void);
	virtual ~CResetState(void);

	CInput							*m_pInput;
	CGame							*m_pGame;
	CRenderer						*m_pRenderer;
	CAssetManager					*m_pAM;
	CBitmapFont						*m_pBMF;

	int								m_nChoice;
	float							m_fTimer;
	bool							m_bVictory;

	RECT							m_rResetBGRect;
	RECT							m_rResetButtonRects[4];

	int								m_nletterHeight;
	int								m_nletterWidth;
	float							m_fLetterScale;

	TResetVertex					tResetVertexs[4];
	IDirect3DTexture9*				d3dResetBG;
	IDirect3DVertexBuffer9*			d3dBGVertBuffer;
	ID3DXEffect*					d3dBGShader;

	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;

	void							RenderNormalScene();
	void							RenderFadingScene();

	// Mouse stuff
	float						m_fMouseX;
	float						m_fMouseY;
	D3DXMATRIX					m_d3dOffset[4];
	ID3DXEffect*				d3dCursorShader;
	TResetVertex				tCursorVertex[4];
	IDirect3DTexture9*			d3dCursorTexture;
	IDirect3DVertexBuffer9*		d3dCursorBuffer;

public:
	static CResetState*	GetInstance();
	void SetVictory(bool bVictory) {m_bVictory = bVictory;}
	bool GetVictory() {return m_bVictory;}


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
};

#endif