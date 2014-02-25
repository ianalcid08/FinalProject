#ifndef _CHARACTERSELECTSTATE_H_
#define _CHARACTERSELECTSTATE_H_

/***********************************************
 * Filename:  		CharacterSelectState.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling character selection
 ************************************************/

#include "igamestate.h"
#include "Renderer.h"
#include "Input.h"

class CAssetManager;

struct PlayerSelect
{
	bool bPlayerIn;
	int  nPlayerColor;
	float fInputTimer;
	bool bPlayerLocked;
	bool bSameColor;
};

class CCharacterSelectState : public IGameState
{
	CCharacterSelectState(void);
	virtual ~CCharacterSelectState(void);

	CInput						*m_pInput;
	CRenderer					*m_pRenderer;
	float						m_fTimer;
	int							m_nCounter;
	int							m_nColors[4];
	bool						m_bReadyScreen;
	PlayerSelect				m_tPlayerIN[4];
	CAssetManager				*m_pAM;


	struct TCharScrVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};

	TCharScrVertex m_tCharSelect1[4];
	TCharScrVertex m_tCharSelectMenu[4];

	IDirect3DVertexBuffer9*  m_d3dCharSelectBuff;

	IDirect3DTexture9* m_d3dCharBlueTexture;
	IDirect3DTexture9* m_d3dCharRedTexture;
	IDirect3DTexture9* m_d3dCharGreenTexture;
	IDirect3DTexture9* m_d3dCharYellowTexture;
	IDirect3DTexture9* m_d3dJoinGameTexture;
	IDirect3DTexture9* m_d3dAllReadyTexture;
	IDirect3DTexture9* m_d3dPressStartTexture;

	ID3DXEffect* m_d3dTempShader;
	ID3DXEffect* m_d3dBGShader;
	IDirect3DDevice9* m_pD3D;
	D3DXMATRIX m_d3dMatrix;
	D3DXMATRIX m_d3dOrthoMatrix;

	// Fading in and out
	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;
	float							m_fScreenPosition;

	void							RenderNormalScene();
	void							RenderFadingScene();

public:
	static CCharacterSelectState*	GetInstance();
	//int GetPlayerCount(void) {return m_nCounter;}
	int* GetColors(void) {return m_nColors;}
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