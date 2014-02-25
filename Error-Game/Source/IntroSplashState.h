#ifndef INTROSPLASHSTATE_H_
#define INTROSPLASHSTATE_H_
/***********************************************
 * Filename:  		IntroSplashState.cpp
 * Date:      		3/8/2013
 * Mod. Date: 		3/8/2013
 * Mod. Initials:	IA
 * Author:    		IA
 * Purpose:   		State for handling game introduction
 ************************************************/
#include "igamestate.h"
#include "Renderer.h"

class CInput;
class CRenderer;
class CAssetManager;
class CGame;

class CIntroSplashState :
	public IGameState
{
	struct TIntroVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};
	CInput						*m_pInput;
	CRenderer					*m_pRenderer;
	CAssetManager				*m_pAM;
	CGame						*m_pGame;

	IDirect3DTexture9			*m_d3dCollection[4];

	RECT						m_rBGRect;
	float						m_fsplashTimer;
	bool						m_bIdle;

	IDirect3DVertexBuffer9*			m_d3dIntroBuff;
	ID3DXEffect*					m_d3dIntroShader;
	TIntroVertex					m_tIntroVerts[4];

	bool							m_bisFadingDone[4];
	int								m_nCurrScreen;
	float							m_fFadingAmount[4];
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed[4];

	CIntroSplashState(void);
	virtual ~CIntroSplashState(void);

public:

	static CIntroSplashState*	GetInstance();
	/*****************************************************************
	* void Initialize(): Initializes all data in the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
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
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
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
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
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
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
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
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	Render();
};

#endif