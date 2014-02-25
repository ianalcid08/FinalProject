#ifndef _INTROSTATE_H_
#define _INTROSTATE_H_

/***********************************************
 * Filename:  		IntroState.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling game introduction
 ************************************************/
#include "igamestate.h"
#include "Renderer.h"


enum eINTRO {INTRO_BEGIN, INTRO_MOVE, INTRO_SHOOT, INTRO_DASH, INTRO_SPECIAL,
	INTRO_COMPUTE, INTRO_AWARE, INTRO_DESTROY, CORRECT_MARK, WRONG_MARK, INTRO_NUMSTAGES};

class CRenderer;

class CIntroState : public IGameState
{
	CIntroState(void);
	virtual ~CIntroState(void);

	CRenderer*						m_pRenderer;
	IDirect3DVertexBuffer9*			m_pVertexBuffer;
	IDirect3DTexture9*				m_pTextures[INTRO_NUMSTAGES];

	unsigned int					m_nCurrTexture;
	float							m_fUpdateTime;
	bool							m_bTutorial;
	bool							m_bSkipTutorial;
	int								m_nCorrect;
	float							m_fMarkTimer;

public:
	static CIntroState*	GetInstance();

	bool GetTutorial() const {return m_bTutorial;}
	bool GetSkipTutorial() const {return m_bSkipTutorial;}


	void SetTutorial(bool bTutorial) {m_bTutorial = bTutorial;}
	void SetSkipTutorial(bool bSkipTutorial) {m_bSkipTutorial = bSkipTutorial;}

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