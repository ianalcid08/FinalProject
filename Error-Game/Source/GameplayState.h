#ifndef _GAMEPLAYSTATE_H_
#define _GAMEPLAYSTATE_H_

/***********************************************
 * Filename:  		GameplayState.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/08/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling gameplay
 ************************************************/
#include "Renderer.h"
#include "IGameState.h"
#include "AssetManager.h"
#include "LevelData.h"
#include "Camera.h"
#include "HUD.h"
#include "FXManager.h"

class CInput;
class CGame;
class CPlayerObject;
class CObjectFactory;
class CViewManager;
class CObjectManager;
class CConveyor;

class CGameplayState : public IGameState
{
private:
	vector<CPlayerObject*> m_vPlayer;
	vector<int> m_vColors;
		
	CHUD							m_pHUD;
	CAssetManager					*m_pAssetManager;
	CInput							*m_pInput;
	CGame							*m_pGame;
	CViewManager					*m_pVM;
	CObjectFactory					*m_pOF;
	CObjectManager					*m_pOM;
	CRenderer						*m_pRenderer;
	CCamera							m_cCamera;
	bool							m_bHorde;
	float							m_fHordeTimer;
	float							m_fCheckpointTimer;
	int								m_nDeathHintCounter;
	float							m_fDanceTime;

	CLevelData testData;

	CFXManager m_cFX;

	CGameplayState(void);
	virtual ~CGameplayState(void);

	void SpawnHorde();

public:

	/*****************
	*Accessors
	*****************/
	CCamera* GetCamera() {return &m_cCamera;}
	bool GetHorde() const { return m_bHorde; }
	CHUD* GetHUD() { return &m_pHUD; }
	std::vector<CPlayerObject*> GetPlayers() const { return m_vPlayer; }
	CLevelData* GetLeveData(void) {return &testData;}
	CFXManager* GetFX() {return &m_cFX;}
	int GetDeathHintCounter() {return m_nDeathHintCounter;}

	/*****************
	*Mutators
	*****************/
	void SetDeathHintCounter(int nDeathHintCounter) {m_nDeathHintCounter = nDeathHintCounter;}

	static CGameplayState*	GetInstance();

	bool GeneratePickup();

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
	* void AddPlayers(): Specifics how many players are in the game
	*
	* Ins:			    int nNumPlayers
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/04/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void	AddPlayers(int NumPlayers);


	/*****************************************************************
	* void Retry(): Reinitalizes the objects in gameplaystate for a retry 
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/05/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void	Retry(void);

	/*****************************************************************
	* void ResetPlayers(): Returns a player's life, lives, and special
	*						If the bool is true, resets position
	*
	* Ins:			    bool bResetPosition
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/09/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void	ResetPlayers(bool bResetPosition = true);

	/*****************************************************************
	* void DisplayCheckpoint(): Displays a message that lets the
	*							player know that they have reached a
	*							checkpoint.
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/22/2013
	* Mod. Initials:	TS
	*****************************************************************/
	void	DisplayCheckpoint();

	/*****************************************************************
	* void LoadMap(): Loads map based on how many players
	*						
	*
	* Ins:			    int _numPlayers
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/14/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	LoadMap(int _numPlayers);

	/*****************************************************************
	* void AllRobotsMustDance(): Makes Player robots dance
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		04/28/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void	AllRobotsMustDance();

	/*****************************************************************
	* void AllRobotsMustDance(): Makes Player robots dance
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		04/28/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void SetPlayerTransition(bool bIsTrans);
};

#endif