#include "SteamrollerFight.h"
#include "GameplayState.h"
#include "SteamrollerBot.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "AIBehaviors.h"
#include "ArcBullet.h"


CSteamrollerFight::CSteamrollerFight(void)
{
	m_pSteam = nullptr;
	m_pOF = nullptr;
	m_fPickupTime = 0.0f;
	m_bImpact = false;
	m_fImpactTime = 0.0f;
	m_nImpactCount = 0;
}


CSteamrollerFight::~CSteamrollerFight(void)
{
}

void CSteamrollerFight::Initialize()
{
	m_fPickupTime = 0.0f;
	m_fImpactTime = 0.3f;
	m_nImpactCount = 0;

	m_pOF = CGame::GetInstance()->GetOF();

	if( m_pOF->Create(OBJ_STEAM, (IBaseObject**)&m_pSteam) )
	{
		CGameplayState::GetInstance()->GetHUD()->EnableBoss(m_pSteam);

		D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera(), d3dLoc, d3dThis = m_pSteam->GetMatrix();
		D3DXMatrixTranslation(&d3dLoc, d3dCamera._41, 0.0f, d3dCamera._43+20.0f);
		m_pSteam->SetMatrix(d3dLoc);

		D3DXVECTOR3 d3dCenter(&d3dCamera[12]);
		d3dCenter.y = 0.0f;
		Movement::LookAt(m_pSteam, d3dCenter);
	}

	m_bImpact = false;
}

void CSteamrollerFight::Shutdown()
{
	CGameplayState::GetInstance()->GetHUD()->DisableBoss();
}

bool CSteamrollerFight::Update(float _fTime)
{
	if( !m_pSteam || !m_pSteam->GetAlive() )
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

	if( !m_pSteam->GetIsEnraged() )
		return false;

	if( m_pSteam->GetBehavior() == Steam::Orient )
		m_nImpactCount = 0;

	if( m_nImpactCount != -1 && !m_bImpact && m_pSteam->GetBehavior() == Steam::Reset )
		m_bImpact = true;

	if( m_nImpactCount != -1 && m_bImpact )
	{
		m_fImpactTime += _fTime;

		if( m_fImpactTime > .3f )
		{
			CArcBullet* pArc = nullptr;
			if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ARC, (IBaseObject**)&pArc) )
			{
				float nXOff = rand()%31-15.0f, nZOff = (float)(rand()%12);
				D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
				d3dCamera._42 = 0.0f;
				D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);
				d3dTranslate = d3dCamera*d3dTranslate;

				Sphere tSphere;
				tSphere.m_Center = *(D3DXVECTOR3*)&d3dTranslate[12];
				tSphere.m_Radius = .05f;
				pArc->SetTargetSphere(tSphere);

				d3dTranslate._42 = 18.1f;
				pArc->SetMatrix(d3dTranslate);
				pArc->SetAlive(true);

				pArc->SetIsMolten(true);
			}

			++m_nImpactCount;
			m_fImpactTime = 0.0f;

			if( m_nImpactCount >= 3 )
			{
				m_bImpact = false;
				m_nImpactCount = -1;
				m_fImpactTime = .3f;
			}
		}
	}

	return false;
}