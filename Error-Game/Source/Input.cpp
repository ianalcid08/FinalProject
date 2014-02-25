/************************************************************************************
 * Filename:  		Input.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/26/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		This is where all the input will be handled
 ************************************************************************************/
#include "Input.h"
#include "GameplayState.h"

#define MOUSEMOD 0.02f

CInput::CInput(void)
{
	for(int i = 0; i < 5; i++)
	{
		m_bTakenControllers[i] = false;
	}
	XINPUT_STATE xTempState;
	if(XInputGetState(0,&xTempState) != ERROR_DEVICE_NOT_CONNECTED)
	{
		m_tPlayers[0].m_dwPlayerID = 0;
		m_bTakenControllers[0] = true;
	}

	m_bConfineMouse = false;

	m_bLMB = false;
	m_bRMB = false;

	//Direct Input stuff
	m_directInput = 0;
	m_keyboard = 0;

	m_tMouseWorld.m_Radius = 0.5f;

	for(unsigned int i = 0; i < 256; i++)
		m_keyboardState[i] = 0;

	m_bEnterKeyPress = false;
	m_bEscapeKeyPress = false;
}

CInput::~CInput(void)
{
}


CInput* CInput::GetInstance(void)
{
	static CInput s_Instance;

	return &s_Instance;
}

bool CInput::InitializeDI(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_nCenterX = m_screenWidth >> 1;
	m_nCenterY = m_screenHeight >> 1;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void CInput::Shutdown()
{
	// Release the keyboard.
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool CInput::Frame()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	return true;
}

bool CInput::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		
	return true;
}

void CInput::ReadDevices()
{
	m_tKeyboard.m_nBitFlag = 0;
	ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	m_keyboard->GetDeviceState(sizeof(m_keyboardState), m_keyboardState);

	if(m_keyboardState[DIK_NUMPAD1] & 0x80) //GetAsyncKeyState(VK_NUMPAD1))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_DOWN_LEFT;
	}
	else if(m_keyboardState[DIK_NUMPAD2] & 0x80) //GetAsyncKeyState(VK_NUMPAD2))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_DOWN;
	}
	else if(m_keyboardState[DIK_NUMPAD3] & 0x80) //GetAsyncKeyState(VK_NUMPAD3))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_DOWN_RIGHT;
	}
	else if(m_keyboardState[DIK_NUMPAD4] & 0x80) //GetAsyncKeyState(VK_NUMPAD4))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_LEFT;
	}
	else if(m_keyboardState[DIK_NUMPAD6] & 0x80) //GetAsyncKeyState(VK_NUMPAD6))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_RIGHT;
	}
	else if(m_keyboardState[DIK_NUMPAD7] & 0x80) //GetAsyncKeyState(VK_NUMPAD7))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_UP_LEFT;
	}
	else if(m_keyboardState[DIK_NUMPAD8] & 0x80) //GetAsyncKeyState(VK_NUMPAD8))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_UP;
	}
	else if(m_keyboardState[DIK_NUMPAD9] & 0x80) //GetAsyncKeyState(VK_NUMPAD9))
	{
		m_tKeyboard.m_nBitFlag |= SHOOT_UP_RIGHT;
	}
	if(m_keyboardState[DIK_SPACE] & 0x80) //GetAsyncKeyState(VK_SPACE))
	{
		m_tKeyboard.m_nBitFlag |= DASH;
	}
	if(m_bRMB || (m_keyboardState[DIK_E] & 0x80))
	{
		m_tKeyboard.m_nBitFlag |= SPECIAL;
	}
	if( KEYDOWN(DIK_ESCAPE) && !m_bEscapeKeyPress)  //GetAsyncKeyState(VK_ESCAPE) & 0x01)
	{
		m_bEscapeKeyPress = true;
	}
	else
	{
		if( KEYUP(DIK_ESCAPE) && m_bEscapeKeyPress)
		{
			m_bEscapeKeyPress = false;
			m_tKeyboard.m_nBitFlag |= PAUSE;
		}
	}
	if( KEYDOWN(DIK_RETURN) && !m_bEnterKeyPress) //GetAsyncKeyState(VK_RETURN))
	{
		m_bEnterKeyPress = true;
	}
	else
	{
		if( KEYUP(DIK_RETURN) && m_bEnterKeyPress)
		{
			m_bEnterKeyPress = false;
			m_tKeyboard.m_nBitFlag |= ACCEPT;
		}
	}
	if(m_keyboardState[DIK_BACK] & 0x80) //GetAsyncKeyState(VK_BACK))
	{
		m_tKeyboard.m_nBitFlag |= BACK;
	}
	if((m_keyboardState[DIK_D] & 0x80) || (m_keyboardState[DIK_RIGHT] & 0x80)) //GetAsyncKeyState('D'))
	{
		if((m_keyboardState[DIK_W] & 0x80) || (m_keyboardState[DIK_UP] & 0x80)) //GetAsyncKeyState('W'))
		{
			m_tKeyboard.m_nBitFlag |= UP_RIGHT;
		}
		else if((m_keyboardState[DIK_S] & 0x80) || (m_keyboardState[DIK_DOWN] & 0x80)) //GetAsyncKeyState('S'))
		{
			m_tKeyboard.m_nBitFlag |= DOWN_RIGHT;
		}
		else
		{
			m_tKeyboard.m_nBitFlag |= RIGHT;
		}
	}
	else if((m_keyboardState[DIK_A] & 0x80) || (m_keyboardState[DIK_LEFT] & 0x80)) //GetAsyncKeyState('A'))
	{
		if((m_keyboardState[DIK_W] & 0x80) || (m_keyboardState[DIK_UP] & 0x80)) //GetAsyncKeyState('W'))
		{
			m_tKeyboard.m_nBitFlag |= UP_LEFT;
		}
		else if((m_keyboardState[DIK_S] & 0x80) || (m_keyboardState[DIK_DOWN] & 0x80)) //GetAsyncKeyState('S'))
		{
			m_tKeyboard.m_nBitFlag |= DOWN_LEFT;
		}
		else
		{
			m_tKeyboard.m_nBitFlag |= LEFT;
		}
	}
	if((m_keyboardState[DIK_W] & 0x80) || (m_keyboardState[DIK_UP] & 0x80))//GetAsyncKeyState('W'))
	{
		m_tKeyboard.m_nBitFlag |= UP;
	}
	else if((m_keyboardState[DIK_S] & 0x80) || (m_keyboardState[DIK_DOWN] & 0x80)) //GetAsyncKeyState('S'))
	{
		m_tKeyboard.m_nBitFlag |= DOWN;
	}

	//Bit 20 - Numpad 3 or RightStick LeftRight
	//Bit 19 - Numpad 1 or RightStick DownLeft
	//Bit 18 - Numpad 9 or RightStick UpRight
	//Bit 17 - Numpad 7 or RightStick UpLeft
	//Bit 16 - Numpad 6 or RightStick Right
	//Bit 15 - Numpad 4 or RightStick Left
	//Bit 14 - Numpad 2 or RightStick Down
	//Bit 13 - Numpad 8 or RightStick Up
	//Bit 12 - B or Backspace
	//Bit 11 - A or Enter
	//Bit 10 - Start or Escape
	//Bit 9 - Left Bumper or Left Shift
	//Bit 8 - Right Bumper or Space
	//Bit 7 - SD or LeftStick LeftRight
	//Bit 6 - SA or LeftStick DownLeft
	//Bit 5 - WD or LeftStick UpRight
	//Bit 4 - WA or LeftStick UpLeft
	//Bit 3 - D or LeftStick Right
	//Bit 2 - A or LeftStick Left
	//Bit 1 - S or LeftStick Down
	//Bit 0 - W or LeftStick Up
	for(int i = 0; i < 4; i++)
	{
		switch(m_tPlayers[i].m_dwPlayerID)
		{
		case -1:
			{
				continue;
			}
			break;
		case 4:
			{
				m_tPlayers[i].m_nBitFlag = m_tKeyboard.m_nBitFlag;
			}
			break;
		default:
			{
				XINPUT_STATE xTempState;
				XInputGetState(m_tPlayers[i].m_dwPlayerID, &xTempState);
				if(xTempState.dwPacketNumber != m_tPlayers[i].m_xState.dwPacketNumber)
				{
					m_tPlayers[i].m_xState = xTempState;
					m_tPlayers[i].m_nBitFlag = 0;
					short sLX = m_tPlayers[i].m_xState.Gamepad.sThumbLX;
					short sLY = m_tPlayers[i].m_xState.Gamepad.sThumbLY;
					short sRX = m_tPlayers[i].m_xState.Gamepad.sThumbRX;
					short sRY = m_tPlayers[i].m_xState.Gamepad.sThumbRY;
					short sButtons = m_tPlayers[i].m_xState.Gamepad.wButtons;
					if(sButtons & XINPUT_GAMEPAD_B)
					{
						m_tPlayers[i].m_nBitFlag |= BACK;
					}
					if(sButtons & XINPUT_GAMEPAD_A)
					{
						m_tPlayers[i].m_nBitFlag |= ACCEPT;
					}
					if(sButtons & XINPUT_GAMEPAD_START)
					{
						m_tPlayers[i].m_nBitFlag |= PAUSE;
					}
					if(sButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || m_tPlayers[i].m_xState.Gamepad.bLeftTrigger >= 100)
					{
						m_tPlayers[i].m_nBitFlag |= DASH;
					}
					if(sButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || m_tPlayers[i].m_xState.Gamepad.bRightTrigger >= 100)
					{
						m_tPlayers[i].m_nBitFlag |= SPECIAL;
					}
					if(abs(sLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					{
						if(sLX < 0)
						{
							if(abs(sLY) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 1.5f)
							{
								if(sLY < 0)
								{
									m_tPlayers[i].m_nBitFlag |= DOWN_LEFT;
								}
								else
								{
									m_tPlayers[i].m_nBitFlag |= UP_LEFT;
								}
							}
							else
							{
								m_tPlayers[i].m_nBitFlag |= LEFT;
							}
						}
						else
						{
							if(abs(sLY) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 1.5f)
							{
								if(sLY < 0)
								{
									m_tPlayers[i].m_nBitFlag |= DOWN_RIGHT;
								}
								else
								{
									m_tPlayers[i].m_nBitFlag |= UP_RIGHT;
								}
							}
							else
							{
								m_tPlayers[i].m_nBitFlag |= RIGHT;
							}
						}
					}
					if(abs(sLY) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					{
						if(sLY < 0)
						{
							m_tPlayers[i].m_nBitFlag |= DOWN;
						}
						else
						{
							m_tPlayers[i].m_nBitFlag |= UP;
						}
					}
				}
			}
		}
	}
}

bool CInput::KEYDOWN(DWORD dwKey)
{
	if(m_keyboardState[dwKey] & 0x80)
		return true;
	else
		return false;
}

bool CInput::KEYUP(DWORD dwKey)
{
	if(m_keyboardState[dwKey] & 0x80)
		return false;
	else
		return true;
}

int CInput::GetInput(int nPlayer)
{
	if(nPlayer==4)
	{
		if(m_tPlayers[0].m_dwPlayerID != -1)
		{
			return m_tKeyboard.m_nBitFlag | m_tPlayers[0].m_nBitFlag;
		}
		return m_tKeyboard.m_nBitFlag;
	}
	return m_tPlayers[nPlayer].m_nBitFlag;
}

bool CInput::CheckInput(int nInput, int nPlayer)
{
	if(nPlayer == 4)
	{
		if(m_tPlayers[0].m_dwPlayerID != -1)
		{
			return (((m_tKeyboard.m_nBitFlag | m_tPlayers[0].m_nBitFlag) & nInput)?true:false);// == nInput);
		}
		return ((m_tKeyboard.m_nBitFlag & nInput)?true:false);// == nInput);
	}
	else if(m_tPlayers[nPlayer].m_dwPlayerID!=-1)
		return ((m_tPlayers[nPlayer].m_nBitFlag & nInput)?true:false);// == nInput);
	return false;
}

int CInput::PollDevice(void)
{
	for(int i = 0; i < 4; i++)
	{
		if(m_tPlayers[i].m_dwPlayerID == -1)
		{
			for(int j = 0; j < 5; j++)
			{
				if(m_bTakenControllers[j])
				{
					continue;
				}
				if(j != 4)
				{
					XINPUT_STATE xTempState;
					if(XInputGetState(j,&xTempState) != ERROR_DEVICE_NOT_CONNECTED)
					{

						if(xTempState.Gamepad.wButtons & ( XINPUT_GAMEPAD_A) )
						{
							m_tPlayers[i].m_dwPlayerID = j;
							m_tPlayers[i].m_xState = xTempState;
							m_bTakenControllers[j] = true;
							return j;
						}
						if(xTempState.Gamepad.wButtons & (XINPUT_GAMEPAD_START | XINPUT_GAMEPAD_B) )
						{
							m_tPlayers[i].m_dwPlayerID = j;
							m_tPlayers[i].m_xState = xTempState;
							m_bTakenControllers[j] = true;
							return -2;
						}
						continue;
					}
				}
				else if(m_keyboardState[DIK_RETURN] & 0x80)//GetAsyncKeyState(VK_RETURN)&0x01)
				{
					m_tPlayers[i].m_dwPlayerID = j;
					m_bTakenControllers[j] = true;
					return j;
				}
			}
			break;
		}
	}
	return -1;
}

void CInput::DropDevice(int _nPlayer)
{
	m_bTakenControllers[m_tPlayers[_nPlayer].m_dwPlayerID] = false;
	m_tPlayers[_nPlayer].m_dwPlayerID = -1;
	XINPUT_STATE xTempState;
	memset(&xTempState, 0, sizeof(XINPUT_STATE));
	m_tPlayers[_nPlayer].m_xState = xTempState;
	m_tPlayers[_nPlayer].m_nBitFlag = 0;
}

void CInput::ClearPlayers(bool bPlayer0)
{
	for(int i = 0; i < 4; i++)
	{
		m_tPlayers[i].m_dwPlayerID = -1;
		m_bTakenControllers[i] = false;
		m_tPlayers[i].m_nBitFlag = 0;
		XINPUT_STATE xTempState;
		memset(&xTempState, 0, sizeof(XINPUT_STATE));
		m_tPlayers[i].m_xState = xTempState;
	}
	m_bTakenControllers[4] = false;
	if(bPlayer0)
	{
		XINPUT_STATE xTempState;
		if(XInputGetState(0,&xTempState) != ERROR_DEVICE_NOT_CONNECTED)
		{
			m_tPlayers[0].m_dwPlayerID = 0;
			m_bTakenControllers[0] = true;
		}
	}
}

bool CInput::GetShootDir(int _nPlayerNum, D3DXMATRIX _d3dPlayerPos, D3DXVECTOR3& _d3dShootDir)
{
	if( m_tPlayers[_nPlayerNum].m_dwPlayerID == 4 )
	{
		m_tMouseWorld.m_Center += D3DXVECTOR3(m_nDeltaMouseX*MOUSEMOD, 0, m_nDeltaMouseY*MOUSEMOD);
		m_tMouseWorld.m_Center += ConfineToFrustum(CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum(), m_tMouseWorld);

		D3DXVECTOR3 shootDir;
		shootDir = m_tMouseWorld.m_Center - *(D3DXVECTOR3*)&_d3dPlayerPos[12];
		shootDir.y = 0;
		D3DXVec3Normalize(&_d3dShootDir, &shootDir);

		if( !m_bLMB )
			return false;

		return true;
	}

	XINPUT_STATE xTempState = m_tPlayers[_nPlayerNum].m_xState;

	short sRX = xTempState.Gamepad.sThumbRX;
	short sRY = xTempState.Gamepad.sThumbRY;

	D3DXVECTOR3 d3dInput(sRX, 0.0f, sRY);
	float fSq = D3DXVec3LengthSq(&d3dInput);

	if( fSq < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE*XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
		return false;

	D3DXVec3Normalize(&_d3dShootDir, &d3dInput);

	return true;
}

void CInput::ResetWorldCursor()
{
	D3DXMATRIX camPos = CGameplayState::GetInstance()->GetCamera()->GetCamera();
	m_tMouseWorld.m_Center = D3DXVECTOR3(camPos[12], BULLET_HEIGHT_OFFSET, camPos[14] + 5.0f);
	m_nDeltaMouseX = 0;
	m_nDeltaMouseY = 0;
}

void CInput::UpdateMouse(int _nX, int _nY)
{
	if(m_bConfineMouse)
	{
		POINT tempPt;
		tempPt.x = m_nCenterX;
		tempPt.y = m_nCenterY;
		ClientToScreen(CGame::GetInstance()->GetHwnd(), &tempPt);
		SetCursorPos(tempPt.x, tempPt.y);
	}

	m_nDeltaMouseX = _nX - m_nCenterX;
	m_nDeltaMouseY = _nY - m_nCenterY;

	m_fMouseScreenX = ((float)(_nX)/(float)(m_screenWidth))*2.0f - 1.0f;
	m_fMouseScreenY = ((float)(_nY)/(float)(m_screenHeight))*2.0f - 1.0f;
}

void CInput::ToggleMouseConfine()
{
	if(!m_bConfineMouse)
	{
		ResetWorldCursor();
	}
	m_bConfineMouse = !m_bConfineMouse;
}