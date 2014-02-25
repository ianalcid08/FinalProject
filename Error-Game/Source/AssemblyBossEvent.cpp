#include "AssemblyBossEvent.h"
#include "Game.h"
#include "FloorObject.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "GameplayState.h"


CAssemblyBossEvent::CAssemblyBossEvent(void)
{
	m_pFloor = nullptr;
}


CAssemblyBossEvent::~CAssemblyBossEvent(void)
{
}

void CAssemblyBossEvent::Initialize()
{
	m_pFloor = nullptr;

	//if( CGame::GetInstance()->GetOF()->Create(OBJ_FLOOR, (IBaseObject**)&m_pFloor) )
	//{
	//	D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
	//	d3dCamera._42 = 0.0f;
	//	m_pFloor->SetMatrix(d3dCamera);
	//}

	m_pCurrState = nullptr;
	m_cSpinFight.SetSpinBot(nullptr);
	m_cSpinFight.SetIsP1(true);
	ChangeState(&m_cSpinFight);
}

void CAssemblyBossEvent::Update(float _fTime)
{
	if( m_pCurrState->Update(_fTime) )
	{
		IAIState* pNewState = nullptr;
		bool bP1 = m_cSpinFight.GetIsP1();
		
		if( m_pCurrState == &m_cSpinFight && bP1 )
		{
			pNewState = &m_cFloorTrans;
			//m_pFloor->SetIsActive(true);
		}
		else if( m_pCurrState == &m_cFloorTrans )
		{
			pNewState = &m_cSpinFight;
		}
		else
		{
			pNewState = nullptr;
		}
		
		if( pNewState )
			ChangeState(pNewState);
		else
			CGame::GetInstance()->GetOF()->StopEvent(this);
	}

}
