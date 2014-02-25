/************************************************************************************
 * Filename:  		Input.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/26/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		This is where all the input will be handled
 ************************************************************************************/
#ifndef _INPUT_H_
#define _INPUT_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "Game.h"
#pragma comment(lib,"xinput.lib")
#include <XInput.h>
#include <dinput.h>


class CInput
{
	int m_nCenterX;
	int m_nCenterY;
	float m_fMouseScreenX;
	float m_fMouseScreenY;
	int m_nDeltaMouseX;
	int m_nDeltaMouseY;
	bool m_bConfineMouse;

	bool m_bLMB;
	bool m_bRMB;

	struct TController
	{
		TController(){m_dwPlayerID=-1;}
		DWORD m_dwPlayerID;
		XINPUT_STATE m_xState;
		int m_nBitFlag;
	};
	TController m_tPlayers[4];
	TController m_tKeyboard;
	bool m_bTakenControllers[5];
	
	//Direct input stuff
	IDirectInput8*		 m_directInput;
	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyboardState[256];
	unsigned char m_LastKeyPressed;
	int m_screenWidth, m_screenHeight;
	bool m_bEnterKeyPress;
	bool m_bEscapeKeyPress;

	Sphere	m_tMouseWorld;

	bool ReadKeyboard();

	CInput(void);
	~CInput(void);
public:

	/*****************
	// Mutators
	*****************/
	void SetLMB(bool _bL) { m_bLMB = _bL; }
	void SetRMB(bool _bR) { m_bRMB = _bR; }
	void TurnOffConfine() {m_bConfineMouse = false;}
	void TurnOnConfine() {m_bConfineMouse = true;}

	/*****************
	// Accessors
	*****************/
	const Sphere& GetMouseWorld() {return m_tMouseWorld;}
	float GetMouseX() {return m_tMouseWorld.m_Center.x;}
	float GetMouseY() {return m_tMouseWorld.m_Center.z;}
	float GetScreenMouseX() {return m_fMouseScreenX;}
	float GetScreenMouseY() {return m_fMouseScreenY;}
	bool GetLMB() {return m_bLMB;}
	bool GetRMB() {return m_bRMB;}
	bool GetIsKeyboard(int _nPlayerNum) { return m_tPlayers[_nPlayerNum].m_dwPlayerID == 4; }
	bool IsMouseConfined() {return m_bConfineMouse;}

	/*****************************************************************
	 * GetInstance():		Used for accessing the singleton
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				Input*
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	static CInput* GetInstance(void);

	/*****************************************************************
	 * ReadDevices():		Encapsulates all the Key/Button checks from the SGD wrapper for 
	 *						each active player and fills out an associated bit flag for 
	 *						use by gameplay code
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ReadDevices(void);

	/*****************************************************************
	 * GetInput():		  Accessor that returns the associated players BitFlag, 
	 *					  containing all active inputs. A parameter of 0 returns keyboard 
	 *					  and Gamepad0 inputs for menu navigation.
	 *
	 * Ins:					int nPlayers
	 *
	 * Outs:				void
	 *
	 * Returns:				int
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	int GetInput(int nPlayers);

	/*****************************************************************
	 * CheckInput():		  Accessor that returns the associated players BitFlag, 
	 *					  containing all active inputs. A parameter of 0 returns keyboard 
	 *					  and Gamepad0 inputs for menu navigation.
	 *
	 * Ins:					int nInput, int nPlayers
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool CheckInput(int nInput, int nPlayers);

	/*****************************************************************
	 * PollDevice():		Checks all available inputs for a button/key check, 
	 *						allowing players to check in during player selection. 
	 *						Players check in the order their input is detected and their 
	 *						input device is recorded as an enum in an array of four ints, 
	 *						one for each player.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				int
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	int PollDevice(void);


	/*****************************************************************
	 * ClearPlayers():		Removes all of the IDs from the players,
	 *						allowing them to be rebound to different
	 *						controllers.
	 *
	 * Ins:					bPlayer0
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void ClearPlayers(bool bPlayer0);

	/*****************************************************************
	 * GetShootDir():		Returns if a player is firing and the direction
	 *
	 * Ins:					_nPlayerNum, _d3dPlayerPos
	 *
	 * Outs:				_d3dShootDir
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/26/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	bool GetShootDir(int _nPlayerNum, D3DXMATRIX _d3dPlayerPos, D3DXVECTOR3& _d3dShootDir);

	/*****************************************************************
	 * DropDevice():		Drops a player from the input
	 *
	 * Ins:					_nPlayer
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/02/2013
	 * Mod. Initials:		MB & TS
	 *****************************************************************/
	void DropDevice(int _nPlayer);

	/*****************************************************************
	 * ResetWorldCursor():	Re-centers cursor
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/08/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void ResetWorldCursor();

	/*****************************************************************
	 * UpdateMouse(float _fX, float _fY):	Updates Mouse and Mouse Delta
	 *
	 * Ins:					_fX, _fY
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/08/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void UpdateMouse(int _nX, int _nY);
	
	/*****************************************************************
	 * ToggleMouseConfine:	Toggles Confine bool and resets MousePos
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/08/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void ToggleMouseConfine();

	bool InitializeDI(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown(void);
	bool Frame(void);

	bool KEYDOWN(DWORD dwKey);
	bool KEYUP(DWORD dwKey);

};
#endif

