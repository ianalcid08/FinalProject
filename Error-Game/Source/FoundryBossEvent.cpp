#include "FoundryBossEvent.h"
#include "Game.h"


CFoundryBossEvent::CFoundryBossEvent(void)
{
}


CFoundryBossEvent::~CFoundryBossEvent(void)
{
}

void CFoundryBossEvent::Initialize()
{
	ChangeState(&m_cSteamrollerFight);
}

void CFoundryBossEvent::Update(float _fTime)
{
	if( m_pCurrState->Update(_fTime) )
		CGame::GetInstance()->GetOF()->StopEvent(this);
}