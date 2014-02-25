#include "GameplayState.h"
#include "Input.h"
#include "PlayerObject.h"
#include "HUD.h"
#include "ResetState.h"
#include "CharacterSelectState.h"
#include "IntroState.h"
/***********************************************
 * Filename:  		GameplayState.cpp
 * Date:      		02/08/2013
 * Mod. Date: 		02/15/2013
 * Mod. Initials:	MB
 * Author:    		Joseph Thomas
 * Purpose:   		State for handling gameplay
 ************************************************/
#include "Audio.h"

CGameplayState* CGameplayState::GetInstance( void )
{
	static CGameplayState s_Instance;

	return &s_Instance;
}

CGameplayState::CGameplayState(void)
{
	m_fHordeTimer = 0.0f;
	m_bHorde = false;
}

CGameplayState::~CGameplayState(void)
{

}

void CGameplayState::Initialize()
{	 
	m_pAssetManager = CAssetManager::GetInstance();
	m_pInput		= CInput::GetInstance();
	m_pGame			= CGame::GetInstance();
	m_pRenderer		= CRenderer::GetInstance();

	m_pVM = m_pGame->GetVM();
	m_pOF = m_pGame->GetOF();
	m_pOM = m_pGame->GetOM();

	m_cCamera.Init();
	testData.Init();
	
	m_cFX.ReInit();
	m_cFX.LoadEffects("TestFiles/TestEffectData.vfx");

	CResetState::GetInstance()->SetVictory(false);
	m_fCheckpointTimer = 0.0f;

	WwiseNS::LevelMusicPlay(WwiseNS::LEVEL_MUSIC1);
	m_pInput->ResetWorldCursor();

	m_nDeathHintCounter = 0;
}	 
	 
void CGameplayState::Shutdown()
{
	m_cFX.ClearData();
	m_pInput->ClearPlayers(true);
	m_pOM->Clear(true);
	m_pOF->ProcessDestroy();
	testData.DestroyLevelData();
	m_vPlayer.clear();
	m_vColors.clear();
	WwiseNS::LevelMusicStop(WwiseNS::LEVEL_MUSIC1);	
}
	 
bool CGameplayState::Input()
{
	int temp = m_pInput->GetInput(4);
	if(temp & 1 << 10) // esc to pause
	{
		m_pGame->PushState(PAUSE_STATE);
		m_pInput->TurnOffConfine();
	}

#ifdef _DEBUG
	if(GetAsyncKeyState(VK_DECIMAL)&1)
	{
		m_pInput->ResetWorldCursor();
		m_pInput->ToggleMouseConfine();
	}
#endif
	 return true;
}	 
	 
void CGameplayState::Update( float fElapsedTime )
{	 
	if(m_fCheckpointTimer > 0.0f)
		m_fCheckpointTimer -= fElapsedTime;
	m_cFX.UpdateEffects(fElapsedTime);
	m_cCamera.Update(fElapsedTime);
	testData.Update(fElapsedTime);
	if( CResetState::GetInstance()->GetVictory() && m_pOM->GetEnemyCount() == 0)
	{
		m_fDanceTime -= fElapsedTime;
		if(m_fDanceTime < 0)
		{
			CGame::GetInstance()->PushState(RETRY_STATE);
			m_pInput->TurnOffConfine();
			return;
		}
	}
	 
	m_pOM->Update(fElapsedTime);
	m_pOM->CheckCollisions();
	m_pOM->CollisionResponses();
	m_pOF->ProcessDestroy();

	if(m_pVM->GetFade())
		m_pVM->UpdateFade(fElapsedTime);
	/*******************************************/
	// Camera adjustment //
#ifdef _DEBUG
	if(GetAsyncKeyState(VK_MULTIPLY)&1)
		m_cCamera.MoveCamera();
	else if(GetAsyncKeyState(VK_DIVIDE)&1)
		m_cCamera.MoveCamera(false);
#endif

	/*******************************************/
	// SPAWN SOME TEST ENEMIES WITH THIS SHIT //

	if( GetAsyncKeyState('T') & 0x01 )
	{
		IBaseObject* pTestTrash = nullptr;
		if( m_pOF->Create(OBJ_TRASH, &pTestTrash) )
		{
			D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43;
			pTestTrash->SetMatrix(d3dTest);
		}
	}
	if( GetAsyncKeyState('Y') & 0x01 )
	{
		IBaseObject* pTestTrash = nullptr;
		if( m_pOF->Create(OBJ_PATROL, &pTestTrash) )
		{
			D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43;
			pTestTrash->SetMatrix(d3dTest);
		}
	}
	if( GetAsyncKeyState('U') & 0x01 )
	{
		IBaseObject* pTestTrash = nullptr;
		if( m_pOF->Create(OBJ_SMASH, &pTestTrash) )
		{
			D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43;
			pTestTrash->SetMatrix(d3dTest);
		}
	}
	if( GetAsyncKeyState('B') & 0x01)
	{
		IBaseObject* pTestTrash = nullptr;
		if( m_pOF->Create(OBJ_BURN, &pTestTrash) )
		{
			D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43;
			pTestTrash->SetMatrix(d3dTest);
		}
	}
	if(GetAsyncKeyState('C') & 0x01)
	{
		CCrate *pCrateTest = nullptr;
		if(m_pOF->Create(OBJ_CRATE,(IBaseObject**)&pCrateTest))
		{
			D3DXMATRIX d3dTest1 = m_vPlayer[0]->GetMatrix();
			D3DXMATRIX tempTrans;
			D3DXMatrixTranslation(&tempTrans,-5.0f,-0.1f,0.0f);
			d3dTest1 *= tempTrans;
			pCrateTest->SetMatrix(d3dTest1);
			pCrateTest->SetRadius(0.5f);
			pCrateTest->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	if(GetAsyncKeyState('V') & 0x01)
	{
		CBarrel *pBarrelTest = nullptr;
		if(m_pOF->Create(OBJ_BARREL,(IBaseObject**)&pBarrelTest))
		{
			D3DXMATRIX d3dTest1 = m_vPlayer[0]->GetMatrix();
			D3DXMATRIX tempTrans;
			D3DXMatrixTranslation(&tempTrans,-5.0f,0.0f,0.0f);
			d3dTest1 *= tempTrans;
			pBarrelTest->SetMatrix(d3dTest1);
			pBarrelTest->SetRadius(0.5f);
			pBarrelTest->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	if( GetAsyncKeyState('M') & 0x01 )
	{
		CAICore* pTestCore = nullptr;
		if( m_pOF->Create(OBJ_AICORE, (IBaseObject**)&pTestCore) )
		{
			D3DXMATRIX d3dTest = pTestCore->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43 + 11.0f;
			pTestCore->SetMatrix(d3dTest);
			pTestCore->Initialize();
		}
	}
	if( GetAsyncKeyState('N') & 0x01 )
	{
		CSteamrollerBot* pTestSteam = nullptr;
		if( m_pOF->Create(OBJ_STEAM, (IBaseObject**)&pTestSteam) )
		{
			D3DXMATRIX d3dTest = pTestSteam->GetMatrix();
			d3dTest._41 = m_cCamera.GetCamera()._41;
			d3dTest._43 = m_cCamera.GetCamera()._43 + 7.0f;
			pTestSteam->SetMatrix(d3dTest);
		}
	}
	if(GetAsyncKeyState('K') & 0x01)
	{
		CVat *pVat = nullptr;
		if(m_pOF->Create(OBJ_VAT,(IBaseObject**)&pVat))
		{
			D3DXMATRIX d3dTest1 = m_vPlayer[0]->GetMatrix();
			D3DXMATRIX tempTrans;
			D3DXMatrixTranslation(&tempTrans,5.0f,0.0f,0.0f);
			d3dTest1 *= tempTrans;
			pVat->SetMatrix(d3dTest1);
			pVat->SetRadius(0.5f);
		}
	}
	if(GetAsyncKeyState('L') & 0x01)
	{
		CTurret *pTurret = nullptr;
		if(m_pOF->Create(OBJ_TURRET,(IBaseObject**)&pTurret))
		{
			D3DXMATRIX d3dTest1 = m_vPlayer[0]->GetMatrix();
			D3DXMATRIX tempTrans;
			D3DXMatrixTranslation(&tempTrans,5.0f,0.0f,0.0f);
			d3dTest1 *= tempTrans;
			pTurret->SetMatrix(d3dTest1);
		}
	}
	if(GetAsyncKeyState('I') & 0x01)
	{
		CInversionTower *pITower = nullptr;
		if(m_pOF->Create(OBJ_INVERSION,(IBaseObject**)&pITower))
		{
			D3DXMATRIX d3dTest1 = m_vPlayer[0]->GetMatrix();
			D3DXMATRIX tempTrans;
			D3DXMatrixTranslation(&tempTrans,5.0f,0.0f,0.0f);
			d3dTest1 *= tempTrans;
			pITower->SetMatrix(d3dTest1);
			pITower->SetRadius(0.75f);
		}
	}
	if( GetAsyncKeyState('O') & 0x01 )
	{
		CSpiderBot* pSpider = nullptr;
		if( m_pOF->Create(OBJ_SPIDER, (IBaseObject**)&pSpider) )
		{
			D3DXMATRIX d3dThis = pSpider->GetMatrix();
			d3dThis._41 = m_cCamera.GetCamera()._41;
			d3dThis._43 = m_cCamera.GetCamera()._43;
			pSpider->SetMatrix(d3dThis);
		}
	}
	if( GetAsyncKeyState('Q') & 0x01 )
	{
		CTrashSpawner* pSpawner = nullptr;
		if( m_pOF->Create(OBJ_TUNNEL, (IBaseObject**)&pSpawner) )
		{
			D3DXMATRIX d3dThis = pSpawner->GetMatrix();
			d3dThis._41 = m_cCamera.GetCamera()._41;
			d3dThis._43 = m_cCamera.GetCamera()._43 - 6.0f;
			pSpawner->SetMatrix(d3dThis);
		}
	}

	//if( GetAsyncKeyState('H') & 0x01)
	//{
	//	m_fHordeTimer = 0.0f;
	//	m_bHorde = !m_bHorde;
	//}
	//
	//m_fHordeTimer += fElapsedTime;
	//
	//if( m_bHorde && m_fHordeTimer > 4.0f )
	//{
	//	m_fHordeTimer = 0.0f;
	//	SpawnHorde();
	//}
	/*******************************************/

	// When all players lose all lives, they lose
	
	unsigned int nPlayerSize = m_vPlayer.size();
	bool bRetry	= false;

	for(unsigned int i = 0; i < nPlayerSize; i++)
	{
		if(m_vPlayer[i]->GetAlive() == true)
		{
			break;
		}

		if((i+1) == nPlayerSize)
			bRetry = true;
	}

	if(bRetry)
	{
		m_pGame->PushState(RETRY_STATE);
		m_pInput->TurnOffConfine();
		return;
	}
}

void CGameplayState::Retry(void)
{

	WwiseNS::LevelMusicStop(WwiseNS::LEVEL_MUSIC1);
	WwiseNS::LevelMusicPlay(WwiseNS::LEVEL_MUSIC1);

	m_pHUD.Init(&m_vPlayer);
	CResetState::GetInstance()->SetVictory(false);

	testData.ResetToCheckpoint();
}

void CGameplayState::ResetPlayers(bool bResetPosition)
{
	unsigned int nPlayerNum = m_vPlayer.size();

	for(unsigned int i = 0; i < nPlayerNum; i++)
	{
		if(bResetPosition == true)
			m_vPlayer[i]->Initialize(m_vPlayer[i]->GetPlayerNum(), m_vPlayer[i]->GetPlayerColorNum() );
		else
			m_vPlayer[i]->Initialize(m_vPlayer[i]->GetPlayerNum(), m_vPlayer[i]->GetPlayerColorNum(), true);
		if(bResetPosition)
		{
			m_vPlayer[i]->ResetPosition(i);
			m_pInput->ResetWorldCursor();
		}

		D3DXMATRIX d3dTempMatrix = m_vPlayer[i]->GetMatrix();
		d3dTempMatrix._42 = 0.0f;
		m_vPlayer[i]->SetMatrix(d3dTempMatrix);
	}
}
	 
void CGameplayState::Render()
{
	testData.Render();
	m_pVM->Render();

	m_cFX.RenderEffects(m_cCamera.GetCamera(),m_cCamera.GetViewProjMatrix());


	if(m_pVM->GetFade())
		m_pVM->RenderFade();

	if(m_vPlayer.size() != 0)
		m_pHUD.Render();


	if(m_fCheckpointTimer > 0.0f)
	{
		CBitmapFont::GetInstance()->PrintBF("CHECKPOINT",300,300,1.0f,0xffff0000);
	}
}

void CGameplayState::AddPlayers(int NumPlayers)
{
	CPlayerObject* pPlayer;
	int nValidColor = -1;
	int nPlayerIndex = -1;

	int PlayerColors[4];
	for(int i = 0; i < 4; i++)
	{
		PlayerColors[i] = CCharacterSelectState::GetInstance()->GetColors()[i];
		m_vColors.push_back(PlayerColors[i]);
	}

	for(int i = 0; i < NumPlayers; i++)
	{
		if(m_pOF->Create(OBJ_PLAYER, (IBaseObject**)&pPlayer) == false)
			break;

		for(int j = 0; j < 4; j++)
		{
			if(PlayerColors[j] != -1)
			{
				nValidColor = PlayerColors[j];
				PlayerColors[j] = -1;
				nPlayerIndex = j;
				break;
			}
		}
		pPlayer->Initialize(nPlayerIndex, nValidColor);
		m_vPlayer.push_back(pPlayer);	
	}

	m_pHUD.Init(&m_vPlayer);
}

bool CGameplayState::GeneratePickup() 
{
	int nNumPickups = m_pOM->GetNumPickups();

	if( (unsigned int)nNumPickups >= (m_vPlayer.size()*2) )
		return false;
	
	int nLimit = 20;

	nLimit += m_vPlayer.size()*5;
	nLimit += m_pOM->GetSize(OM_ENEMIES);
	nLimit -= nNumPickups * 10;

	int nRand = rand()%100;

	if( nRand >= nLimit )
		return false;

	return true;
}

void CGameplayState::SpawnHorde() 
{
	IBaseObject* pTestTrash = nullptr;

	if( m_pOF->Create(OBJ_TRASH, &pTestTrash) )
	{
		D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
		d3dTest._41 = m_cCamera.GetCamera()._41 - 16.0f;
		d3dTest._43 = m_cCamera.GetCamera()._43 + 7.0f;
		pTestTrash->SetMatrix(d3dTest);
	}

	if( m_pOF->Create(OBJ_TRASH, &pTestTrash) )
	{
		D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
		d3dTest._41 = m_cCamera.GetCamera()._41;
		d3dTest._43 = m_cCamera.GetCamera()._43 + 17.0f;
		pTestTrash->SetMatrix(d3dTest);
	}

	if( m_pOF->Create(OBJ_TRASH, &pTestTrash) )
	{
		D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
		d3dTest._41 = m_cCamera.GetCamera()._41 + 15.0f;
		d3dTest._43 = m_cCamera.GetCamera()._43 + 7.0f;
		pTestTrash->SetMatrix(d3dTest);
	}

	if( m_pOF->Create(OBJ_TRASH, &pTestTrash) )
	{
		D3DXMATRIX d3dTest = pTestTrash->GetMatrix();
		d3dTest._41 = m_cCamera.GetCamera()._41;
		d3dTest._43 = m_cCamera.GetCamera()._43 - 3.0f;
		pTestTrash->SetMatrix(d3dTest);
	}
}
void CGameplayState::DisplayCheckpoint()
{
	m_fCheckpointTimer = 1.0f;
}
void CGameplayState::LoadMap(int _numPlayers)
{
	int difficulty = CGame::GetInstance()->GetDifficulty();

	switch(difficulty)
	{
	case NORMAL:
		{
			switch(_numPlayers)
			{
			case 1:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 2:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 3:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 4:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			}
			break;
		}
	case HARD:
		{
			switch(_numPlayers)
			{
			case 1:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 2:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 3:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			case 4:
				{
					testData.LoadData("Data/Level Data/NewTileTest.xml");
				}
				break;
			}
			break;
		}
	}
	m_cCamera.Init();
	m_cCamera.Update(0.0f);

	ResetPlayers(true);

}

void CGameplayState::AllRobotsMustDance()
{
	for each(CPlayerObject* player in m_vPlayer)
	{
		player->Dance();
		m_fDanceTime = m_cCamera.GetNextTime();
	}
}

void CGameplayState::SetPlayerTransition(bool bIsTrans)
{
	for each(CPlayerObject* player in m_vPlayer)
	{
		player->SetTransition(bIsTrans);
	}
}