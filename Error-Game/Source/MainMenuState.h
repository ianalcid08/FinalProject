#ifndef _MAINMENUSTATE_H_
#define _MAINMENUSTATE_H_

/***********************************************
 * Filename:  		MainMenuState.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/21/2013
 * Mod. Initials:	IA
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling the main menu
 ************************************************/

#include "IGameState.h"
#include "Renderer.h"

class CInput;
class CGame;
class CAssetManager;
class CBitmapFont;

class CMainMenuState : public IGameState
{
	struct TBGVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};

	// TODO DELETE TEST CODE
	float curVol;

	CMainMenuState(void);
	virtual ~CMainMenuState(void);
	CInput						*m_pInput;
	CGame						*m_pGame;
	CRenderer					*m_pRenderer;
	CAssetManager				*m_pAM;
	CBitmapFont					*m_pBMF;
	int							m_nChoice;
	float						m_fTimer;

	// Mouse stuff
	float						m_fMouseX;
	float						m_fMouseY;
	D3DXMATRIX					m_d3dOffset[4];
	ID3DXEffect*				d3dCursorShader;
	TBGVertex					tCursorVertex[4];
	IDirect3DTexture9*			d3dCursorTexture;
	IDirect3DVertexBuffer9*		d3dCursorBuffer;

	TBGVertex						tBGVertexs[4];
	IDirect3DTexture9*				d3dBackGround;
	IDirect3DTexture9*				d3dHighlight;
	IDirect3DVertexBuffer9*			d3dBGVertBuffer;
	IDirect3DVertexBuffer9*			d3dHighlightVertBuffer;
	ID3DXEffect*					d3dBGShader;

	RECT						m_rBGRect;
	RECT						m_rbuttonRects[4];
	int								m_nletterHeight;
	int								m_nletterWidth;
	float							m_fLetterScale;

	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;

	void							RenderNormalScene();
	void							RenderFadingScene();

public:
	static CMainMenuState*	GetInstance();

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
	/*****************************************************************
	* void DrawBackground(): Draws background
	*
	* Ins:	backgroundBMP	-	The id of the texture to draw.		    
	*		nX				-	The x position to draw the texture at.
	*		nY				-	The y position to draw the texture at.
	*		fScaleX			-	How much to scale the texture in the x.
	*		fScaleY			-	How much to scale the texture in the y.
	*		pSection		-	The section of the bitmap to draw.
	*		fRotCenterX		-	The x center to apply the rotation from.
	*		fRotCenterY		-	The y center to apply the rotation from.
	*		fRotation		-	How much to rotate the texture.
	*		dwColor			-	The color to apply to the texture (use D3DCOLOR_XRGB() Macro).
	*
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		3/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	//void	DrawBackground(IDirect3DTexture9* backgroundBMP, int nX, int nY, float fScaleX, float fScaleY,
	//RECT* pSection = NULL, float fRotCenterX = 0.0f, float fRotCenterY = 0.0f, float fRotation = 0.0f, DWORD dwColor = 0xFFFFFFFF);
};

#endif