#include "AICoreEvent.h"
#include "Game.h"
#include "BossInversionTower.h"

CAICoreEvent::CAICoreEvent(void)
{
}


CAICoreEvent::~CAICoreEvent(void)
{
}

void CAICoreEvent::Initialize()
{
	m_pCurrState = nullptr;
	ChangeState(&m_cLaserPhase);

	m_pTower = m_cLaserPhase.GetTower();
	if( m_pTower )
		m_cTurretPhase.SetTower(m_pTower);
}

void CAICoreEvent::Update(float _fTime)
{
	if( !m_pCurrState )
		return;

	if( m_pCurrState->Update(_fTime) )
	{
		IAIState* pNewState = nullptr;
		
		if( m_pCurrState == &m_cLaserPhase )
		{
			m_pTower->SetShieldActive(false);
			pNewState = &m_cTurretPhase;
		}
		else if( m_pCurrState == &m_cTurretPhase )
			pNewState = &m_cCoreFight;
		
		if( pNewState )
			ChangeState(pNewState);
		else
			CGame::GetInstance()->GetOF()->StopEvent(this);
	}
}