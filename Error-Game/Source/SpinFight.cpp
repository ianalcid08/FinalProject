#include "SpinFight.h"
#include "SpinBot.h"
#include "GameplayState.h"
#include "Game.h"
#include "AIBehaviors.h"


CSpinFight::CSpinFight(void)
{
	m_bPhase1 = true;
	m_fPickupTime = 0.0f;
}


CSpinFight::~CSpinFight(void)
{
}

void CSpinFight::Initialize()
{
	m_fPickupTime = 0.0f;

	m_pOF = CGame::GetInstance()->GetOF();

	if( !m_bPhase1 )
	{
		m_pSpinBot->SetSpeed(0.0f);
		m_pSpinBot->SetIsHolding(true);
		return;
	}

	if( m_pOF->Create(OBJ_SPINBOT, (IBaseObject**)&m_pSpinBot) )
	{
		CGameplayState::GetInstance()->GetHUD()->EnableBoss(m_pSpinBot);

		D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera(), d3dLoc, d3dThis = m_pSpinBot->GetMatrix();
		D3DXMatrixTranslation(&d3dLoc, d3dCamera._41, 0.0f, d3dCamera._43+20.0f);
		m_pSpinBot->SetMatrix(d3dLoc);

		D3DXVECTOR3 d3dCenter(&d3dCamera[12]);
		d3dCenter.y = 0.0f;
		Movement::LookAt(m_pSpinBot, d3dCenter);


		D3DXVECTOR3 d3dVel(1.0f, 0.0f, -1.0f);
		m_pSpinBot->SetVelocity(d3dVel);
	}
}

void CSpinFight::Shutdown()
{
	if( m_bPhase1 )
	{
		m_pSpinBot->SetBehavior(SpinBot::Reset);
		float fHalfHP = m_pSpinBot->GetMaxHP() * .5f;
		m_pSpinBot->SetHealth(fHalfHP);
		m_bPhase1 = false;
	}
	else
	{
		CGameplayState::GetInstance()->GetHUD()->DisableBoss();
		m_pSpinBot = nullptr;
	}

	
}

bool CSpinFight::Update(float _fTime)
{
	if( !m_pSpinBot || !m_pSpinBot->GetAlive() )
		return true;

	float fTransitionHP = m_pSpinBot->GetMaxHP()*.5f;

	if( m_bPhase1 && m_pSpinBot->GetHealth() < fTransitionHP )
		return true;

	m_fPickupTime += _fTime;

	if( m_fPickupTime > (8.0f-CGameplayState::GetInstance()->GetPlayers().size()) )
	{

		CPickup* pPickup = nullptr;
		if(CGame::GetInstance()->GetOF()->Create(OBJ_PICKUP,(IBaseObject**)&pPickup))
		{
			float nXOff = rand()%27-13.0f, nZOff = (float)(rand()%12);
			D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
			d3dCamera._42 = 0.0f;
			D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);
			pPickup->SetMatrix(d3dCamera*d3dTranslate);
		}

		m_fPickupTime = 0.0f;
	}

	return false;
}