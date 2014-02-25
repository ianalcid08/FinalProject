#ifndef _DIFFICULTY_H_
#define _DIFFICULTY_H_
/***********************************************
 * Filename:  		Difficulty.h
 * Date:      		04/08/2013
 * Mod. Date: 		04/08/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		State in which the difficulty is set for all levels.
 ************************************************/
#include "IGameState.h"
#include "Renderer.h"

class CInput;
class CAssetManager;
class CBitmapFont;
class CAssetManager;

class CDifficulty : public IGameState
{
	struct TDifficultyVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};

	CInput						*m_pInput;
	CRenderer					*m_pRenderer;
	CAssetManager				*m_pAM;
	float						m_fTimer;
	CBitmapFont					*m_pBMF;
	IDirect3DDevice9			*m_pD3D;
	IDirect3DVertexBuffer9		*m_d3dDifficultyBuff;
	ID3DXEffect					*m_d3dShader;
	IDirect3DTexture9			*d3dDifficultyBG;
	TDifficultyVertex			tDiffVertexs[4];

	int							m_nChoice;
	int							m_nDifficulty;

	int							m_nfirstBoxX;
	int							m_nfirstBoxY;
	int							m_nletterHeight;
	int							m_nletterWidth;
	float						m_fLetterScale;
	
	CDifficulty(void);
	virtual ~CDifficulty(void);
	// Fading in and out
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
	TDifficultyVertex			tCursorVertex[4];
	IDirect3DTexture9*			d3dCursorTexture;
	IDirect3DVertexBuffer9*		d3dCursorBuffer;


public:
	static CDifficulty* GetInstance();
	int GetDifficulty()				{return m_nDifficulty;}

	/*****************************************************************
	* void Initialize(): Initializes all data in the state.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		04/08/2013
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
	* Mod. Date:		04/08/2013
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
	* Mod. Date:		04/08/2013
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
	* Mod. Date:		04/08/2013
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
	* Mod. Date:		04/08/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	Render();
};

#endif