/***********************************************
 * Filename:  		EffectTestState.h
 * Date:      		02/27/2013
 * Mod. Date: 		02/27/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D Springer
 * Purpose:   		State to be used for testing the rendering of effects
 ************************************************/
#ifndef _CEFFECTTESTSTATE_H_
#define	_CEFFECTTESTSTATE_H_
#include "IGameState.h"
#include "FXManager.h"
#include <conio.h>
class IBaseObject;

class CEffectTestState : public IGameState
{
	float m_fTimer;
	IBaseObject* m_pObject;
	D3DXMATRIX m_d3dCam;
	D3DXMATRIX m_d3dNonInvCam;
	D3DXMATRIX m_d3dProj;
	D3DXMATRIX m_d3dRot;
	int m_nCurrentEffect;
	float m_fCurrRotation;
	float m_fCurrRotationUD;
	CFXManager m_fxMan;
	CEffectTestState(void){}
	virtual ~CEffectTestState(void){}
public:
	D3DXMATRIX GetProj(){return m_d3dProj;}
	D3DXMATRIX GetCam(){return m_d3dNonInvCam*m_d3dRot;}
	static CEffectTestState* GetInstance();

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
	* Mod. Initials:	TS
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
	* Mod. Date:		02/27/2013
	* Mod. Initials:	TS
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
	* Mod. Date:		02/27/2013
	* Mod. Initials:	TS
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
	* Mod. Date:		02/27/2013
	* Mod. Initials:	TS
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
	* Mod. Date:		02/27/2013
	* Mod. Initials:	TS
	*****************************************************************/
	void	Render();
};

#endif