#ifndef _AIBEHAVIORS_H_
#define _AIBEHAVIORS_H_
#include <d3dx9math.h>

class IBaseObject;
class CEnemyObject;
class CMovingObject;

namespace Utilities
{
	float FastInvSqrt(float x);
	void FastNormalize(D3DXVECTOR3 &_d3dIn);
	void CheckAvoid(CEnemyObject* _pThis, IBaseObject* _pTarget);
}

namespace Movement
{
	void LookAt(IBaseObject* _pThis, D3DXVECTOR3 _d3dTargetPos);
	void LookAtTarget(CEnemyObject* _pThis);
	bool TurnTo(IBaseObject* _pThis, D3DXVECTOR3 _d3dTargetPos, float _fTime, float _fSpeedk4);
	bool TurnToTarget(CEnemyObject* _pThis, float _fTime, float _fSpeed);
	void Avoid(CEnemyObject* _pThis, float _fTime, float _fSpeed);
	void MoveForward(CMovingObject* _pThis, float _fTime, float _fSpeed);
	void MoveDirection(CMovingObject* _pThis, D3DXVECTOR3 _d3dDir, float _fTime, float _fSpeed);
}

namespace Targeting
{
	bool CheckTargetValid(CEnemyObject* _pThis);
	bool Random(CEnemyObject* _pThis);
	bool Close(CEnemyObject* _pThis);
	bool Far(CEnemyObject* _pThis);
}

namespace Steam
{
	void Idle(IBaseObject* _pThis, float _fTime);
	void Orient(IBaseObject* _pThis, float _fTime);
	void Charge(IBaseObject* _pThis, float _fTime);
	void Reset(IBaseObject* _pThis, float _fTime);
	void Pose(IBaseObject* _pThis, float _fTime);
}

namespace SpinBot
{
	void Enter(IBaseObject* _pThis, float _fTime);
	void Idle(IBaseObject* _pThis, float _fTime);
	void Spin(IBaseObject* _pThis, float _fTime);
	void Reset(IBaseObject* _pThis, float _fTime);
}

namespace AICore
{
	void Enter(IBaseObject* _pThis, float _fTime);
	void Idle(IBaseObject* _pThis, float _fTime);
	void BurstFire(IBaseObject* _pThis, float _fTime);
	void Fire(IBaseObject* _pThis, float _fTime);
	void Sway(IBaseObject* _pThis, float _fTime);
}

namespace AIFist
{
	void Enter(IBaseObject* _pThis, float _fTime);
	void Idle(IBaseObject* _pThis, float _fTime);
	void Approach(IBaseObject* _pThis, float _fTime);
	void Hover(IBaseObject* _pThis, float _fTime);
	void Smash(IBaseObject* _pThis, float _fTime);
	void Return(IBaseObject* _pThis, float _fTime);
}

#endif