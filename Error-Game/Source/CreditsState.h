#ifndef _CREDITSSTATE_H_
#define _CREDITSSTATE_H_

/***********************************************
 * Filename:  		CreditsState.h
 * Date:      		02/08/2013
 * Mod. Date: 		03/23/2013
 * Mod. Initials:	IA
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling credits
 ************************************************/

#include "igamestate.h"
#include "Renderer.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;
class CBitmapFont;

class CCreditsState : public IGameState
{
	struct TCreditVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};
	CCreditsState(void);
	virtual ~CCreditsState(void);


	CInput						*m_pInput;
	CGame						*m_pGame;
	CRenderer					*m_pRenderer;
	CAssetManager				*m_pAM;
	CBitmapFont					*m_pBMF;

	// BG stuff
	TCreditVertex					tBGVertexs[4];
	IDirect3DVertexBuffer9*			d3dBGVertBuffer;
	ID3DXEffect*					d3dBGShader;
	IDirect3DTexture9*				d3dCreditsBG;

	float						m_fTimer;

	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;
	float								m_nScrolling;

	void							RenderNormalScene(float _fDelta);
	void							RenderFadingScene();

public:
	static CCreditsState*	GetInstance();

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