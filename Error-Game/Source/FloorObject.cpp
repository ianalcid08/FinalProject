#include "FloorObject.h"
#include "Game.h"

CFloorObject::CFloorObject(void)
{
	SetID(OBJ_FLOOR);
	SetRadius(50.0f);
	m_fTime = 0.0f;
}


CFloorObject::~CFloorObject(void)
{
}

void CFloorObject::Update(float fDelta)
{
	if( !GetIsActive() )
		return;

	m_fTime += fDelta;

	if( m_fTime > 5.0f )
	{
		CGame::GetInstance()->GetOF()->Destroy(this);
		m_fTime = 0.0f;
	}
}

bool CFloorObject::CheckCollision(IBaseObject* obj)
{
	int nType = obj->GetID();

	if( nType == OBJ_CONVEYOR )
	{
		if( AABBtoAABB(GetAABB(), ((CConveyor*)obj)->GetAABB()) )
		{
			obj->CollisionResponse(this);
			return true;
		}
	}

	return false;
}

void CFloorObject::CollisionResponse(IBaseObject* obj)
{

}

void CFloorObject::TakeDamage(IBaseObject* _pIn)
{

}

void CFloorObject::Render()
{

}