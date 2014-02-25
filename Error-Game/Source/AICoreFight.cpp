#include "AICoreFight.h"
#include "Game.h"
#include "GameplayState.h"


CAICoreFight::CAICoreFight(void)
{
	m_pCore = nullptr;
	m_fPickupTime = 0.0f;
}


CAICoreFight::~CAICoreFight(void)
{
}

void CAICoreFight::Initialize()
{
	if( CGame::GetInstance()->GetDifficulty() < HARD )
		CGameplayState::GetInstance()->ResetPlayers(false);

	m_fPickupTime = 0.0f;

	if( CGame::GetInstance()->GetOF()->Create(OBJ_AICORE, (IBaseObject**)&m_pCore) )
		m_pCore->Initialize();
}

void CAICoreFight::Shutdown()
{

}

bool CAICoreFight::Update(float _fTime)
{
	m_fPickupTime += _fTime;

	if( m_fPickupTime > (8.0f-CGameplayState::GetInstance()->GetPlayers().size()) )
	{
		CPickup* pPickup = nullptr;
		if(CGame::GetInstance()->GetOF()->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
		{
			float nXOff = rand()%23-9.0f, nZOff = (float)(rand()%12);
			D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
			d3dCamera._42 = 0.0f;
			D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);

			pPickup->SetMatrix(d3dCamera*d3dTranslate);
		}


		m_fPickupTime = 0.0f;
	}
	
	bool bDead = !m_pCore->GetAlive();

	return bDead;
}