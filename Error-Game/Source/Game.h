#pragma once

#include <Windows.h>
#include <vector>
#include "Timer.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ViewManager.h"
#include "AssetManager.h"
#include "BitmapFont.h"

using namespace std;

// TO BE MOVED TO DEFINITIONS
enum eGameStates { INTRO_STATE, MAINMENU_STATE, OPTIONS_STATE, CREDITS_STATE, PAUSE_STATE, GAMEPLAY_STATE, RETRY_STATE, ANIMTEST_STATE, EFFECTTEST_STATE, CHAR_SELECT_STATE,SPLASHINTRO_STATE,DIFFICULTY_STATE, NUM_STATES };

class CRenderer;
class IGameState;
class CInput;

class CGame
{
public:
	// Singleton Stuff
	CGame(void);
	~CGame(void);	
	CGame( const CGame& );				
	CGame& operator= ( const CGame& );
	static CGame* GetInstance();

	// Accessors & Mutators
	int		GetScreenWidth( void )	const{ return m_nScreenWidth; }
	int		GetScreenHeight( void ) const{ return m_nScreenHeight; }
	bool	IsWindowed( void )		const{ return m_bIsWindowed; }
	HWND	GetHwnd(void)			const {return m_hWnd;}
	HINSTANCE GetHInstance(void)	const {return m_hInstance;}
	bool	GetIsRunning()			const {return m_bIsRunning;}
	float GetDelta() const { return m_fDelta; }

	CObjectFactory* GetOF() { return &m_cOF; }
	CObjectManager* GetOM() { return &m_cOM; }
	CViewManager*	GetVM() { return &m_cVM; }

	void	SetIsWindowed(bool b) {m_bIsWindowed = b;}
	void	SetIsRunning(bool b) {m_bIsRunning = b;}

	void	Init( HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed );
	bool	Play();
	void	Shutdown();

	// State Machine
	void ChangeState(eGameStates _eNewState);
	void PushState(eGameStates _eNewState);
	void PopState();

	void SetCursorPos(int _nX, int _nY);
	void SetLMB(bool _bL);
	void SetRMB(bool _bR);

	// Options menu Loading/Saving
	void LoadOptions(const char* _filename);
	void SaveOptions(const char* _filename);
	void LoadVideoSettings(const char* _filename);
	void SaveVideoSettings(const char* _filename);

	void SetGamma(float _gamma);
	void SetDifficulty(int _difficulty) { m_nGameDifficulty = _difficulty;}

	int	 GetDifficulty() {return m_nGameDifficulty;}
	float GetGameGamma() {return m_fGammaValue;}



private:
	HWND						m_hWnd;
	HINSTANCE					m_hInstance;
	CRenderer*					m_Renderer;
	int							m_nScreenWidth;
	int							m_nScreenHeight;
	bool						m_bIsWindowed;

	bool						m_bIsRunning;

	bool						Input( void );
	void						Update( void );
	void						Render( void );
	static CGame				m_Instance;
	std::vector<IGameState*>	m_pCurrState;
	Timer						m_cTimer;
	
	CObjectFactory				m_cOF;
	CObjectManager				m_cOM;
	CViewManager				m_cVM;
	CInput*						m_pInput;
	float						m_fDelta;
	float						m_fGammaValue; // 0-100
	int							m_nGameDifficulty;
};

