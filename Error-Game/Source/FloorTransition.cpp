#include "FloorTransition.h"
#include "GameplayState.h"


CFloorTransition::CFloorTransition(void)
{
	m_fTime = 0.0f;
}


CFloorTransition::~CFloorTransition(void)
{
}

void CFloorTransition::Initialize()
{
	m_fTime = 0.0f;
	CGameplayState::GetInstance()->GetLeveData()->TriggerFloor();
}

void CFloorTransition::Shutdown()
{

}

bool CFloorTransition::Update(float _fTime)
{
	m_fTime += _fTime;

	if( m_fTime > .5f )
	{
		m_fTime = 0.0f;
		CGameplayState::GetInstance()->GetLeveData()->ToggleConveyors();
		return true;
	}

	return false;
}