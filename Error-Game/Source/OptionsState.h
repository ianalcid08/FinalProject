#ifndef _OPTIONSSTATE_H_
#define _OPTIONSSTATE_H_

/***********************************************
 * Filename:  		OptionsState.h
 * Date:      		02/08/2013
 * Mod. Date: 		03/16/2013
 * Mod. Initials:	IA
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling options
 ************************************************/

#include "igamestate.h"
#include "Renderer.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;
class CBitmapFont;

class COptionsState : public IGameState
{
	struct TOptVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};
	enum eOptionsEnum {FX_VOLUME = 1, MUSIC_VOLUME,RESOLUTION_CHANGE,GAMMA_CHANGE,BACK_CHANGE,HARD_MUTE,VSYNC_CHANGE,DEFAULT_SETTINGS,NUM_OPT};
	COptionsState(void);
	virtual ~COptionsState(void);

	CInput						*m_pInput;
	CGame						*m_pGame;
	CRenderer					*m_pRenderer;
	CAssetManager				*m_pAM;
	CBitmapFont					*m_pBMF;

	bool						m_bLockOut;
	int							m_nChoice;
	float						m_fTimer;

	// sound params
	float						m_fFXVolume;
	float						m_fMusicVolume;
	bool						m_bMuteAll;
	//Video params
	//float						m_fGammaAttribute;
	/*D3DDISPLAYMODE				modes[100];
	D3DFORMAT					formats[20];
	DWORD						dwNumFormats;
	DWORD						dwNumModes;
	DWORD						dwNumAdapterModes;*/
	//vector<D3DDISPLAYMODE>		*m_vResolutions;
	unsigned int				m_nIndexResolution;
	bool						m_bVsyncToggle;
	float						m_fGammaAttribute; // 1-100
	D3DDISPLAYMODE				m_d3dCurrMode;

	RECT						m_rOptionBGRect;
	RECT						m_rbuttonRects[4];
	IDirect3DTexture9*			d3dOption;
	IDirect3DTexture9*			d3dOptionBG;

	int							m_nfirstBoxX;
	int							m_nfirstBoxY;
	int							m_nletterHeight;
	int							m_nletterWidth;
	float						m_fLetterScale;

	// BG stuff
	TOptVertex						tBGVertexs[4];
	IDirect3DVertexDeclaration9*	d3dBGDecl;
	IDirect3DTexture9*				d3dHighlight;
	IDirect3DVertexBuffer9*			d3dBGVertBuffer;
	ID3DXEffect*					d3dBGShader;

	// Mouse stuff
	float						m_fMouseX;
	float						m_fMouseY;
	D3DXMATRIX					m_d3dOffset[4];
	ID3DXEffect*				d3dCursorShader;
	TOptVertex					tCursorVertex[4];
	IDirect3DTexture9*			d3dCursorTexture;
	IDirect3DVertexBuffer9*		d3dCursorBuffer;

	// Fading in and out
	bool							m_bisFadingDone;
	float							m_fFadingAmount;
	float							m_fFadingTotalTime;
	float							m_fFadingElapsed;
	float							m_fScreenPosition;

	void							RenderNormalScene();
	void							RenderFadingScene();

public:

	void SetFXVolume(float _volume) {m_fFXVolume = _volume;}
	void SetMXVolume(float _volume) {m_fMusicVolume = _volume;}
	void SetGammaRamp(float _gamma) {m_fGammaAttribute = _gamma;}
	void SetIndexRes(int _index)	{m_nIndexResolution = _index;}
	void SetMuteAll(bool _mute)		{m_bMuteAll = _mute;}

	float GetFXVolume()				{return m_fFXVolume;}
	float GetMXVolume()				{return m_fMusicVolume;}
	float GetGammaRamp()			{return m_fGammaAttribute;}
	int GetIndexRes()				{return m_nIndexResolution;}
	bool GetMuteAll()				{return m_bMuteAll;}


	static COptionsState*	GetInstance();

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