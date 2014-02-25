#include "AIBehaviors.h"
#include "IBaseObject.h"
#include "AICore.h"
#include "Game.h"
#include "ArcBullet.h"
#include "AIFist.h"
#include "ExplodingBullet.h"
#include "GameplayState.h"
#include "EnemyObject.h"

namespace Utilities
{
	float FastInvSqrt(float x) 
	{
		float xhalf = 0.5f * x;
		int i = *(int*)&x;         // evil floating point bit level hacking
		i = 0x5f3759df - (i >> 1);  // what the fuck?
		x = *(float*)&i;
		x = x*(1.5f-(xhalf*x*x));
		return x;
	}
	void FastNormalize(D3DXVECTOR3 &_d3dIn)
	{
		float fSq = D3DXVec3LengthSq(&_d3dIn);
		if( fSq <= 1.0f )
			return;

		fSq = FastInvSqrt(fSq);

		_d3dIn *= fSq;
	}
	void CheckAvoid(CEnemyObject* _pThis, IBaseObject* _pTarget)
	{
		Tesla tCone;

		float fAvoidWidth = 3.5f, fAvoidHeight = 3.5f;

		D3DXMATRIX d3dTeslaMatrix = _pThis->GetMatrix(), d3dTranslate;
		d3dTeslaMatrix._42 = 0.0f;
		float fTranslate = _pThis->GetRadius() * -1.0f;
		D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, fTranslate);
		d3dTeslaMatrix = d3dTranslate * d3dTeslaMatrix;

		D3DXMATRIX d3dFL = d3dTeslaMatrix, d3dFR = d3dTeslaMatrix, d3dF = d3dTeslaMatrix;
		D3DXMatrixTranslation(&d3dTranslate, -fAvoidWidth*.5f, 0.0f, fAvoidHeight);
		d3dFL = d3dTranslate * d3dFL;
		D3DXMatrixTranslation(&d3dTranslate, fAvoidWidth*.5f, 0.0f, fAvoidHeight);
		d3dFR = d3dTranslate * d3dFR;
		D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, fAvoidHeight);
		d3dF = d3dTranslate * d3dF;

		D3DXVECTOR3 d3dFPos(&d3dF[12]), d3dFLPos(&d3dFL[12]), d3dFRPos(&d3dFR[12]), d3dMiddle(&d3dTeslaMatrix[12]), d3dMidBot(&d3dTeslaMatrix[12]);
		d3dMidBot.y -= .5f;
		d3dFPos.y += .5f;
		ComputePlane(tCone.m_tPlanes[0], d3dFLPos, d3dFPos, d3dFRPos);
		ComputePlane(tCone.m_tPlanes[1], d3dFLPos, d3dMidBot, d3dMiddle);
		ComputePlane(tCone.m_tPlanes[2], d3dFRPos, d3dMiddle, d3dMidBot);

		Sphere tSphere = _pTarget->GetSphere();

		if( _pTarget->GetID() == OBJ_PIT )
			tSphere.m_Radius *= 1.75f;

		if( TeslaToSphere(tCone, tSphere) )
		{
			D3DXVECTOR3 d3dMyPos(&_pThis->GetMatrix()[12]), d3dTargetPos(&_pTarget->GetMatrix()[12]), d3dTT, d3dMyZ(&_pThis->GetMatrix()[8]);
			d3dTT = d3dTargetPos - d3dMyPos;

			float fSqD = D3DXVec3LengthSq(&d3dTT) - (_pTarget->GetRadius()*_pTarget->GetRadius()), fCurrD = _pThis->GetCloseDist();

			if( !fCurrD || fSqD < fCurrD )
			{
				_pThis->SetCloseDist(fSqD);
				_pThis->SetCloseTarget(_pTarget);
			}
		}
	}
}

namespace Movement
{
	void LookAt(IBaseObject* _pThis, D3DXVECTOR3 _d3dTargetPos)
	{
		D3DXVECTOR3 d3dMyX(&_pThis->GetMatrix()[0]), d3dMyY(0.0f, 1.0f, 0.0f), d3dMyZ;
		d3dMyZ = _d3dTargetPos - *(D3DXVECTOR3*)&_pThis->GetMatrix()[12];

		if( D3DXVec3LengthSq(&d3dMyZ) > 1.0f )
			Utilities::FastNormalize(d3dMyZ);

		D3DXMATRIX d3dThis = _pThis->GetMatrix();
		
		D3DXVec3Cross(&d3dMyX, &d3dMyY, &d3dMyZ);

		*(D3DXVECTOR3*)&d3dThis[0] = d3dMyX;
		*(D3DXVECTOR3*)&d3dThis[4] = d3dMyY;
		*(D3DXVECTOR3*)&d3dThis[8] = d3dMyZ;

		_pThis->SetMatrix(d3dThis);
	}

	void LookAtTarget(CEnemyObject* _pThis)
	{
		CPlayerObject* pTarget = (CPlayerObject*)_pThis->GetTargetFINAL();

		if( !pTarget )
			return;

		D3DXVECTOR3 d3dTargetPos(&_pThis->GetTargetFINAL()->GetMatrix()[12]);
		Movement::LookAt(_pThis, d3dTargetPos);
	}

	bool TurnTo(IBaseObject* _pThis, D3DXVECTOR3 _d3dTargetPos, float _fTime, float _fSpeed)
	{
		D3DXVECTOR3 d3dMyX(&_pThis->GetMatrix()[0]), d3dMyY(0.0f, 1.0f, 0.0f), d3dMyZ(&_pThis->GetMatrix()[8]), d3dTT;
		d3dTT = _d3dTargetPos - *(D3DXVECTOR3*)&_pThis->GetMatrix()[12];
		Utilities::FastNormalize(d3dTT);

		float fDotX = D3DXVec3Dot(&d3dMyX, &d3dTT);
		float fRot = 0.0f;

		if( fDotX < .13f && fDotX > -.13f )
		{
			float fDotZ = D3DXVec3Dot(&d3dMyZ, &d3dTT);
			if( fDotZ < 0.0f )
			{
				fRot = _fTime*_fSpeed*(fDotX>0.0f?1.0f:-1.0f);
			}
			else
			{
				return true;
			}
		}
		else
		{
			fDotX < 0.0f ? fRot = -_fTime*_fSpeed : fRot = _fTime*_fSpeed;
		}

		fRot = D3DXToRadian(fRot);

		D3DXMATRIX d3dThis = _pThis->GetMatrix(), d3dRot;
		D3DXMatrixRotationY(&d3dRot, fRot);
		d3dThis = d3dRot * d3dThis;

		d3dMyZ = D3DXVECTOR3(&d3dThis[8]);
		Utilities::FastNormalize(d3dMyZ);

		D3DXVec3Cross(&d3dMyX, &d3dMyY, &d3dMyZ);

		*(D3DXVECTOR3*)&d3dThis[0] = d3dMyX;
		*(D3DXVECTOR3*)&d3dThis[4] = d3dMyY;
		*(D3DXVECTOR3*)&d3dThis[8] = d3dMyZ;

		_pThis->SetMatrix(d3dThis);

		return false;
	}

	bool TurnToTarget(CEnemyObject* _pThis, float _fTime, float _fSpeed)
	{
		CPlayerObject* pTarget = (CPlayerObject*)_pThis->GetTargetFINAL();

		if( !pTarget || !pTarget->GetAlive() || pTarget->GetInvulnerableTime() < 0.0f )
		{
			return false;
		}

		D3DXVECTOR3 d3dTargetPos(&_pThis->GetTargetFINAL()->GetMatrix()[12]);
		return Movement::TurnTo(_pThis, d3dTargetPos, _fTime, _fSpeed);
	}

	void Avoid(CEnemyObject* _pThis, float _fTime, float _fSpeed)
	{
		IBaseObject* pAvoidTarget = _pThis->GetCloseTarget();

		if( !pAvoidTarget )
			return;

		D3DXVECTOR3 d3dMyPos(&_pThis->GetMatrix()[12]), d3dTargetPos(&_pThis->GetTargetFINAL()->GetMatrix()[12]), d3dTT, d3dMyZ(&_pThis->GetMatrix()[8]), d3dMyX(&_pThis->GetMatrix()[0]);
		d3dTT = d3dTargetPos - d3dMyPos;
		
		if( D3DXVec3LengthSq(&d3dTT) < _pThis->GetCloseDist() )
			return;
				
		d3dTargetPos = *(D3DXVECTOR3*)&pAvoidTarget->GetMatrix()[12];
		d3dTT = d3dTargetPos - d3dMyPos;

		float fDotX = D3DXVec3Dot(&d3dTT, &d3dMyX);

		float fRot = fDotX < 0.0f ? _fTime * _fSpeed : -_fTime * _fSpeed;

		D3DXVECTOR3 d3dMyY(0.0f, 1.0f, 0.0f);
		D3DXMATRIX d3dThis = _pThis->GetMatrix(), d3dRot;
		D3DXMatrixRotationY(&d3dRot, D3DXToRadian(fRot));
		d3dThis = d3dRot * d3dThis;
		
		d3dMyZ = D3DXVECTOR3(d3dThis[8], d3dThis[9], d3dThis[10]);
		Utilities::FastNormalize(d3dMyZ);
		
		D3DXVec3Cross(&d3dMyX, &d3dMyY, &d3dMyZ);
		
		*(D3DXVECTOR3*)&d3dThis[0] = d3dMyX;
		*(D3DXVECTOR3*)&d3dThis[4] = d3dMyY;
		*(D3DXVECTOR3*)&d3dThis[8] = d3dMyZ;
		
		_pThis->SetMatrix(d3dThis);

		_pThis->SetCloseDist(0.0f);
		_pThis->SetCloseTarget(nullptr);
	}

	void MoveForward(CMovingObject* _pThis, float _fTime, float _fSpeed)
	{
		D3DXMATRIX d3dThis = _pThis->GetMatrix();
		D3DXVECTOR3 d3dDir(&_pThis->GetMatrix()[8]);
		d3dDir *= (_fTime*_fSpeed);
		*(D3DXVECTOR3*)&d3dThis[12] += d3dDir;
		_pThis->SetMatrix(d3dThis);
	}

	void MoveDirection(CMovingObject* _pThis, D3DXVECTOR3 _d3dDir, float _fTime, float _fSpeed)
	{
		D3DXMATRIX d3dThis = _pThis->GetMatrix();

		if( D3DXVec3LengthSq(&_d3dDir) > 1.0f )
			Utilities::FastNormalize(_d3dDir);

		_d3dDir *= (_fTime*_fSpeed);
		*(D3DXVECTOR3*)&d3dThis[12] += _d3dDir;
		_pThis->SetMatrix(d3dThis);
	}
}

namespace Targeting
{
	bool CheckTargetValid(CEnemyObject* _pThis)
	{
		CPlayerObject* pTarget = (CPlayerObject*)_pThis->GetTargetFINAL();
		return pTarget && pTarget->GetAlive();
	}

	bool DoPlayersExist()
	{
		std::vector<CPlayerObject*> m_vPlayers = CGameplayState::GetInstance()->GetPlayers();

		unsigned int nSize = m_vPlayers.size();
		bool bAlive = false;

		for( unsigned int i = 0; i < nSize; ++i )
		{
			if( m_vPlayers[i]->GetAlive() )
			{
				bAlive = true;
				break;
			}
		}

		return bAlive;
	}

	bool Random(CEnemyObject* _pThis)
	{
		if( !DoPlayersExist() )
		{
			_pThis->SetTargetFINAL(nullptr);
			return false;
		}

		std::vector<CPlayerObject*> m_vPlayers = CGameplayState::GetInstance()->GetPlayers();
		unsigned int nSize = m_vPlayers.size();

		CPlayerObject* pTarget = nullptr;

		do
		{
			pTarget = m_vPlayers[rand()%nSize];
		}
		while( !pTarget->GetAlive() );

		_pThis->SetTargetFINAL(pTarget);

		return true;
	}

	bool Close(CEnemyObject* _pThis)
	{
		if( !DoPlayersExist() )
		{
			_pThis->SetTargetFINAL(nullptr);
			return false;
		}

		std::vector<CPlayerObject*> m_vPlayers = CGameplayState::GetInstance()->GetPlayers();
		unsigned int nSize = m_vPlayers.size();

		D3DXVECTOR3 d3dThis(&_pThis->GetMatrix()[12]);
		D3DXVECTOR3 d3dToPl(0.0f, 0.0f, 0.0f);
		float fSq = 0.0f;

		IBaseObject* pTarget = nullptr;

		for( unsigned int i = 0; i < nSize; ++i )
		{
			if( !m_vPlayers[i]->GetAlive() )
				continue;

			D3DXVECTOR3 d3dPl(&m_vPlayers[i]->GetMatrix()[12]);
			d3dToPl = d3dPl-d3dThis;
			float fSqI = D3DXVec3LengthSq(&d3dToPl);
			
			if( !fSq || fSqI < fSq )
			{
				fSq = fSqI;
				pTarget = m_vPlayers[i];
			}
		}

		_pThis->SetTargetFINAL(pTarget);

		return true;
	}

	bool Far(CEnemyObject* _pThis)
	{
		if( !DoPlayersExist() )
		{
			_pThis->SetTargetFINAL(nullptr);
			return false;
		}

		std::vector<CPlayerObject*> m_vPlayers = CGameplayState::GetInstance()->GetPlayers();
		unsigned int nSize = m_vPlayers.size();

		D3DXVECTOR3 d3dThis(&_pThis->GetMatrix()[12]);
		D3DXVECTOR3 d3dToPl(0.0f, 0.0f, 0.0f);
		float fSq = 0.0f;

		IBaseObject* pTarget = nullptr;

		for( unsigned int i = 0; i < nSize; ++i )
		{
			if( !m_vPlayers[i]->GetAlive() )
				continue;

			D3DXVECTOR3 d3dPl(&m_vPlayers[i]->GetMatrix()[12]);
			d3dToPl = d3dPl-d3dThis;
			float fSqI = D3DXVec3LengthSq(&d3dToPl);

			if( !fSq || fSqI > fSq )
			{
				fSq = fSqI;
				pTarget = m_vPlayers[i];
			}
		}

		_pThis->SetTargetFINAL(pTarget);

		return true;
	}
}

namespace Steam
{
	void Idle(IBaseObject* _pThis, float _fTime)
	{
		CSteamrollerBot* pThis = (CSteamrollerBot*)_pThis;
		pThis->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		
		Frustum tFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();

		Sphere tSphere = pThis->GetSphere();
		tSphere.m_Radius *= 2.0f;
		if( SphereToFrustum(tSphere, tFrustum) )
		{
			float fSpeed = TRASHBOT_SPEED*2.0f;

			//D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();

			//if( d3dThis._33 > 0.0f )
			//{
			//	D3DXMATRIX  d3dRot;
			//	D3DXMatrixRotationY(&d3dRot, D3DXToRadian(180.0f));
			//	d3dThis = d3dRot * d3dThis;
			//	d3dThis._13 = 0.0f;
			//	d3dThis._31 = 0.0f;
			//	pThis->SetMatrix(d3dThis);
			//	return;
			//}

			//pThis->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
			//D3DXVECTOR3 d3dDir = pThis->GetVelocity();
			//d3dDir = _fTime * fSpeed * d3dDir;
			//D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, 0.0f, d3dDir.z);
			//d3dThis *= d3dTranslate;
			//pThis->SetMatrix(d3dThis);

			Movement::MoveForward(pThis, _fTime, fSpeed);
			return;
		}

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime > 2.0f )
		{
			pThis->SetTime(0.0f);
			pThis->SetBehavior(Steam::Orient);
			return;
		}
	}

	void Orient(IBaseObject* _pThis, float _fTime)
	{
		CSteamrollerBot* pThis = (CSteamrollerBot*)_pThis;
		//D3DXVECTOR3 d3dMyLoc(&pThis->GetMatrix()[12]), d3dTargetLoc(&pThis->GetTarget()->GetMatrix()[12]), d3dTT, d3dFinal;
		//d3dTT = d3dTargetLoc - d3dMyLoc;
		//D3DXVec3Normalize(&d3dFinal, &d3dTT);

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		D3DXVECTOR4 d3dMyColor = pThis->GetColor();
		d3dMyColor.y -= _fTime;
		d3dMyColor.z -= _fTime;
		pThis->SetColor(d3dMyColor);

		float fSpeed = SMASHBOT_ROT_SPEED*.75f + fTime*20.0f;

		if( pThis->GetIsEnraged() )
			fSpeed *= 1.75f;

		if( fSpeed > 270.0f )
			fSpeed = 270.0f;

		Movement::TurnToTarget(pThis, _fTime, fSpeed);// && fTime > pThis->GetTargetTime()

		if( fTime > pThis->GetCurrentAnimation()->m_fDuration )
		{
			pThis->SetTime(0.0f);
			pThis->SetBehavior(Steam::Charge);
			pThis->ChangeAnimation(ASTEAM_ATTACK);
			return;
		}

		fTime = pThis->GetSwitchTime();
		fTime += _fTime;
		pThis->SetSwitchTime(fTime);

		if( fTime > .25f )
		{
			pThis->SetTargetFINAL(nullptr);
			pThis->SetSwitchTime(0.0f);
		}
	}

	void Charge(IBaseObject* _pThis, float _fTime)
	{
		CSteamrollerBot* pThis = (CSteamrollerBot*)_pThis;
		pThis->SetColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		
		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime < .5f )
			return;

		Frustum tFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();
		Sphere tSphere = pThis->GetSphere();
		tSphere.m_Radius *= 1.5f;

		if( SphereToFrustum(tSphere, tFrustum) )
		{
			pThis->SetImpact(D3DXVECTOR3(&pThis->GetMatrix()[12]));
			pThis->SetTime(0.0f);
			pThis->SetBehavior(Steam::Reset);
			pThis->ChangeAnimation(ASTEAM_RICO);
			return;
		}

		//D3DXVECTOR3 d3dTarget(&pThis->GetTargetFINAL()->GetMatrix()[12]), d3dMyPos(&pThis->GetMatrix()[12]), d3dTT;
		//d3dTT = d3dTarget - d3dMyPos;

		float fSpeed = pThis->GetIsEnraged() ? TRASHBOT_SPEED*20.0f*1.75f : TRASHBOT_SPEED*20.0f;

		//D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();
		//D3DXVECTOR3 d3dDir = pThis->GetVelocity();
		//d3dDir = _fTime * fSpeed * d3dDir;
		//D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, 0.0f, d3dDir.z);
		//d3dThis *= d3dTranslate;
		//pThis->SetMatrix(d3dThis);

		Movement::MoveForward(pThis, _fTime, fSpeed);
	}

	void Reset(IBaseObject* _pThis, float _fTime)
	{
		CSteamrollerBot* pThis = (CSteamrollerBot*)_pThis;
		pThis->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		
		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		float fModStunTime = pThis->GetStunTime();

		if( pThis->GetIsEnraged() )
			fModStunTime *= .75f;

		if( fTime < fModStunTime )
			return;

		pThis->SetStunTime(2.0f);
		pThis->SetTime(2.0f);

		D3DXMATRIX d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
		d3dCamera._42 = 0.0f;
		d3dCamera._43 += 7.0f;
		D3DXVECTOR3 d3dCenter(&d3dCamera[12]), d3dMyPos(&pThis->GetMatrix()[12]);

		float fRotSpeed = pThis->GetIsEnraged() ? TRASHBOT_ROT_SPEED*1.75f : TRASHBOT_ROT_SPEED;

		if( !Movement::TurnTo(pThis, d3dCenter, _fTime, fRotSpeed) )
			return;


		Frustum tFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();
		Sphere tSphere = pThis->GetSphere();
		tSphere.m_Radius *= 2.0f;

		if( !SphereToFrustum(tSphere, tFrustum) )
		{
			pThis->SetTime(0.0f);
			pThis->SetBehavior(Steam::Orient);
			pThis->ChangeAnimation(ASTEAM_CHARGE);
			return;
		}


		float fSpeed = pThis->GetIsEnraged() ? TRASHBOT_SPEED*1.75f : TRASHBOT_SPEED;

		//D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();
		//d3dDir = pThis->GetVelocity();
		//d3dDir = _fTime * fSpeed * d3dDir;
		//D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, 0.0f, d3dDir.z);
		//d3dThis *= d3dTranslate;
		//pThis->SetMatrix(d3dThis);

		Movement::MoveForward(pThis, _fTime, fSpeed);

	}

	void Pose(IBaseObject* _pThis, float _fTime)
	{
		if( !_pThis || _pThis->GetID() != OBJ_STEAM )
			return;

		CSteamrollerBot* pThis = (CSteamrollerBot*)_pThis;

		D3DXVECTOR3 d3dCenter(&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12]); 
		d3dCenter.y = 0.0f;
		d3dCenter.z += 8.0f;

		Sphere tCenter, tThis = pThis->GetSphere();
		tCenter.m_Center = d3dCenter;
		tCenter.m_Radius = .35f;
		tThis.m_Radius = .35f;

		if( !SphereToSphere(tCenter, tThis) )
		{
			if( !Movement::TurnTo(pThis, d3dCenter, _fTime, TRASHBOT_ROT_SPEED*1.75f) )
				return;

			Movement::MoveForward(pThis, _fTime, TRASHBOT_SPEED*1.75f);
				return;
		}

		d3dCenter.z = pThis->GetMatrix()._43-10.0f;
		d3dCenter.x = pThis->GetMatrix()._41;

		if( !Movement::TurnTo(pThis, d3dCenter, _fTime, TRASHBOT_ROT_SPEED*.5f) )
			return;

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime )
		{
			pThis->ChangeAnimation(ASTEAM_CHARGE);
			pThis->SetBehavior(Steam::Orient);
			pThis->SetTime(0.0f);
		}
	}
}

namespace SpinBot
{
	void Enter(IBaseObject* _pThis, float _fTime)
	{
		if( _pThis->GetID() != OBJ_SPINBOT )
			return;

		CSpinBot* pThis = (CSpinBot*)_pThis;
		pThis->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		
		Frustum tFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();

		Sphere tSphere = pThis->GetSphere();
		tSphere.m_Radius *= 1.25f;
		if( SphereToFrustum(tSphere, tFrustum) )
		{
			float fSpeed = TRASHBOT_SPEED*2.0f;
			Movement::MoveForward(pThis, _fTime, fSpeed);
			return;
		}

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime > 2.0f )
		{
			pThis->SetRotSpeed(0.0f);
			pThis->SetSpeed(0.0f);
			pThis->SetTime(0.0f);
			pThis->SetBehavior(SpinBot::Spin);
			return;
		}
	}

	void Idle(IBaseObject* _pThis, float _fTime)
	{
		if(!_pThis || _pThis->GetID() != OBJ_SPINBOT )
			return;

		CSpinBot* pThis = (CSpinBot*)_pThis;

		SpinBot::Spin(_pThis, _fTime);

		if( pThis->GetIsHolding() )
		{
			float fHoldTime = pThis->GetHoldTime();
			fHoldTime += _fTime;
			pThis->SetHoldTime(fHoldTime);

			float fTotal = 1.0f;

			if( fHoldTime > fTotal )
			{
				pThis->SetIsHolding(false);
				pThis->SetHoldTime(0.0f);
				pThis->SetBehavior(SpinBot::Spin);
				pThis->SetSpeed(0.0f);
			}

			return;
		}
	}

	void Spin(IBaseObject* _pThis, float _fTime)
	{
		if( _pThis->GetID() != OBJ_SPINBOT )
			return;

		CSpinBot* pThis = (CSpinBot*)_pThis;

		float fRot = D3DXToRadian(pThis->GetRotSpeed()*_fTime);

		D3DXVECTOR3 d3dMyX(&_pThis->GetMatrix()[0]), d3dMyY(0.0f, 1.0f, 0.0f), d3dMyZ(&_pThis->GetMatrix()[8]);
		D3DXMATRIX d3dThis = _pThis->GetMatrix(), d3dRot;
		D3DXMatrixRotationY(&d3dRot, fRot);
		d3dThis = d3dRot * d3dThis;

		d3dMyZ = D3DXVECTOR3(&d3dThis[8]);
		Utilities::FastNormalize(d3dMyZ);

		D3DXVec3Cross(&d3dMyX, &d3dMyY, &d3dMyZ);

		*(D3DXVECTOR3*)&d3dThis[0] = d3dMyX;
		*(D3DXVECTOR3*)&d3dThis[4] = d3dMyY;
		*(D3DXVECTOR3*)&d3dThis[8] = d3dMyZ;

		_pThis->SetMatrix(d3dThis);
	}

	void Reset(IBaseObject* _pThis, float _fTime)
	{
		if( _pThis->GetID() != OBJ_SPINBOT )
			return;

		CSpinBot* pThis = (CSpinBot*)_pThis;

		SpinBot::Spin(_pThis, _fTime);

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime < 2.0f )
			return;

		D3DXVECTOR3 d3dCenter(&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12]);
		d3dCenter.y = 0.0f;
		d3dCenter.z += 6.0f;

		Sphere tCenter, tThis = pThis->GetSphere();
		
		tCenter.m_Center = d3dCenter;
		tCenter.m_Radius = .1f;

		tThis.m_Radius = .1f;

		if( SphereToSphere(tCenter, tThis) )
		{
			pThis->SetBehavior(SpinBot::Idle);
			return;
		}

		D3DXVECTOR3 d3dThis(&pThis->GetMatrix()[12]), d3dTT;
		d3dTT = d3dCenter - d3dThis;

		Movement::MoveDirection(pThis, d3dTT, _fTime, TRASHBOT_SPEED);
	}
}

namespace AICore
{
	void Enter(IBaseObject* _pThis, float _fTime)
	{
		CAICore* pThis = (CAICore*)_pThis;
		
		Movement::MoveDirection(pThis, D3DXVECTOR3(0.0f,-1.0f,0.0f), _fTime, TRASHBOT_SPEED*.5f);

		D3DXMATRIX d3dThis= pThis->GetMatrix();
		float fY = d3dThis._42;

		if( fY < 0.0f )
		{
			pThis->SetBehavior(AICore::Fire);
			d3dThis._42 = 0.0f;
			pThis->SetMatrix(d3dThis);
		}
	}

	void Idle(IBaseObject* _pThis, float _fTime)
	{
		CAICore* pThis = (CAICore*)_pThis;
		pThis->SetFireTime(0.0f);
	}

	void Fire(IBaseObject* _pThis, float _fTime)
	{
		CAICore* pThis = (CAICore*)_pThis;

		if( !Targeting::CheckTargetValid(pThis) )
			return;

		IBaseObject* pTarget = pThis->GetTargetFINAL();

		pThis->SetColor(COLOR_TRASHBOT);

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		fTime = pThis->GetFireTime();
		fTime += _fTime;
		pThis->SetFireTime(fTime);

		if( fTime > pThis->GetFireRate() )
		{
			CArcBullet* pBullet = nullptr;
			if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ARC, (IBaseObject**)&pBullet) )
			{
				Sphere tSphere = pTarget->GetSphere();
				tSphere.m_Radius = .05f;
				pBullet->SetTargetSphere(tSphere);
				pBullet->SetColor(COLOR_ENEMY_BULLET);

				D3DXMATRIX d3dBulletMat = pThis->GetMatrix();
				d3dBulletMat._42 = 0.5f;
				pBullet->SetMatrix(d3dBulletMat);
				pBullet->SetAlive(true);
			}

			pThis->SetFireTime(0.0f);
		}
	}

	void BurstFire(IBaseObject* _pThis, float _fTime)
	{
		CAICore* pThis = (CAICore*)_pThis;

		if( !Targeting::CheckTargetValid(pThis) )
			return;

		IBaseObject* pTarget = pThis->GetTargetFINAL();

		pThis->SetColor(COLOR_PLAYER);

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		fTime = pThis->GetFireTime();
		fTime += _fTime;
		pThis->SetFireTime(fTime);

		float fFireRate = pThis->GetFireRate();

		if( fTime > fFireRate )
		{
			float fBurst = pThis->GetBurstTime();
			fBurst += _fTime;
			pThis->SetBurstTime(fBurst);

			CArcBullet* pBullet = nullptr;

			if( fBurst > .4f )
			{
				if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ARC, (IBaseObject**)&pBullet) )
				{
					int nCount = pThis->GetBurstCount();

					Sphere tSphere;
					tSphere.m_Center = pThis->GetBurstLoc(nCount);
					tSphere.m_Radius = .05f;
					pBullet->SetTargetSphere(tSphere);
					pBullet->SetColor(COLOR_ENEMY_BULLET);

					D3DXMATRIX d3dBulletMat = pThis->GetMatrix();
					d3dBulletMat._42 = 0.5f;
					pBullet->SetMatrix(d3dBulletMat);
					pBullet->SetAlive(true);

					pThis->SetBurstTime(0.0f);
					++nCount;
					pThis->SetBurstCount(nCount);

					if( nCount >= 3 )
					{
						pThis->SetFireTime(0.0f);
						pThis->SetBurstCount(0);
					}
				}
			}
		}
	}

	void Sway(IBaseObject* _pThis, float _fTime)
	{
		if( _pThis->GetID() != OBJ_AICORE )
			return;

		CAICore* pThis = (CAICore*)_pThis;

		D3DXVECTOR3 d3dVel = pThis->GetVelocity(), d3dPos(&pThis->GetMatrix()[12]), d3dCamera;
		d3dCamera = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];

		Movement::MoveDirection(pThis, d3dVel, _fTime, TRASHBOT_SPEED);

		float fMin = d3dCamera.x - 5.0f, fMax = d3dCamera.x + 5.0f;

		bool bLeft = d3dVel.x < 0.0f;
		
		D3DXMATRIX d3dThis = pThis->GetMatrix();

		if( bLeft && d3dPos.x < fMin )
		{
			d3dVel.x = 1.0f;
			d3dThis._41 = fMin;
			pThis->SetMatrix(d3dThis);
		}
		else if( !bLeft && d3dPos.x > fMax )
		{
			d3dVel.x = -1.0f;
			d3dThis._41 = fMax;
			pThis->SetMatrix(d3dThis);

		}

		pThis->SetVelocity(d3dVel);
	}
}

namespace AIFist
{
	void Enter(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;
		
		Movement::MoveDirection(pThis, D3DXVECTOR3(0.0f,-1.0f,0.0f), _fTime, TRASHBOT_SPEED*.5f);

		D3DXMATRIX d3dThis= pThis->GetMatrix();
		float fY = d3dThis._42;

		if( fY < .2f )
		{
			pThis->SetBehavior(AIFist::Idle);
			d3dThis._42 = .2f;
			pThis->SetMatrix(d3dThis);
		}
	}

	void Idle(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;

		/*Sphere test = pThis->GetSphere();
		test.m_d3dColor = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
		pThis->SetSphere(test);*/
		((CAIFist*)_pThis)->SetColor(D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));

		pThis->SetTime(0.0f);
	}

	void Approach(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;

		if( !Targeting::CheckTargetValid(pThis) )
		{
			pThis->SetBehavior(AIFist::Return);
			pThis->SetTargetFINAL(nullptr);
			pThis->SetTime(0.0f);
			return;
		}

		/*Sphere test = pThis->GetSphere();
		test.m_d3dColor = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
		pThis->SetSphere(test);*/
		((CAIFist*)_pThis)->SetColor(D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f));

		D3DXVECTOR3 d3dMyPos(&pThis->GetMatrix()[12]), d3dTargetPos(&pThis->GetTargetFINAL()->GetMatrix()[12]), d3dTT;
		d3dTargetPos.y = 3.0f;
		d3dTT = d3dTargetPos - d3dMyPos;
		float fSqD = D3DXVec3LengthSq(&d3dTT);

		if( fSqD < .25f )
		{
			pThis->SetTime(0.0f);
			pThis->SetBehavior(AIFist::Hover);
			return;
		}

		// TODO - MAKE SOME REAL SPEEDS FOR AIFIST
		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		float fSpeed = TRASHBOT_SPEED + fTime*2.0f;

		D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();
		D3DXVECTOR3 d3dDir;
		D3DXVec3Normalize(&d3dDir, &d3dTT);
		d3dDir = _fTime * fSpeed * d3dDir;
		D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, d3dDir.y, d3dDir.z);
		d3dThis *= d3dTranslate;
		pThis->SetMatrix(d3dThis);
	}

	void Hover(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;

		if( !Targeting::CheckTargetValid(pThis) )
		{
			pThis->SetTargetFINAL(nullptr);
			pThis->SetBehavior(AIFist::Return);
			pThis->SetTime(0.0f);
			return;
		}

		/*Sphere test = pThis->GetSphere();
		test.m_d3dColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		pThis->SetSphere(test);*/
		((CAIFist*)_pThis)->SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		if( fTime > .25f )
		{
			pThis->SetBehavior(AIFist::Smash);
			pThis->SetTime(0.0f);
			return;
		}
		//else if( fTime > .25f )
		//{
		//	return;
		//}
		//
		//D3DXVECTOR3 d3dTargetPos(&pThis->GetTargetFINAL()->GetMatrix()[12]);
		//D3DXMATRIX d3dMyMat = pThis->GetMatrix();
		//d3dMyMat._41 = d3dTargetPos.x;
		//d3dMyMat._43 = d3dTargetPos.z;
		//pThis->SetMatrix(d3dMyMat);
	}

	void Smash(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;

		/*Sphere test = pThis->GetSphere();
		test.m_d3dColor = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		pThis->SetSphere(test);*/
		((CAIFist*)_pThis)->SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

		D3DXMATRIX d3dMyMat(pThis->GetMatrix());
		if( d3dMyMat._42 <= 0.0f )
		{
			d3dMyMat._42 = 0.0f;
			pThis->SetMatrix(d3dMyMat);

			CExplodingBullet* pBullet = nullptr;
			if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_EXP, (IBaseObject**)&pBullet) )
			{
				Sphere tSphere = pBullet->GetSphere();
				tSphere.m_Radius = 0.0f;
				tSphere.m_Center = D3DXVECTOR3(&d3dMyMat[12]);
				pBullet->SetMatrix(d3dMyMat);
				pBullet->SetColor(COLOR_SMASHBOT_AOE);
				pBullet->SetDamage(SMASHBOT_DAMAGE);
				pBullet->SetType(SMASHBULLET);
				//pBullet->SetSphere(tSphere);
				pBullet->SetMaxRadius(SMASHBOT_RADIUS);
				pBullet->SetSpeed(SMASHBOT_AOESPEED);
				CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMASHWAVE,d3dMyMat);
			}

			pThis->SetBehavior(AIFist::Return);
			pThis->SetTime(0.0f);
			return;
		}

		float fTime = pThis->GetTime();
		fTime += _fTime;
		pThis->SetTime(fTime);

		float fSpeed = _fTime * TRASHBOT_SPEED * 6.0f;
		if( fTime < .1f )
			fSpeed *= -1.0f;

		D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();
		D3DXVECTOR3 d3dDir(0.0f, -1.0f, 0.0f);
		d3dDir *= fSpeed;
		D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, d3dDir.y, d3dDir.z);
		d3dThis *= d3dTranslate;
		pThis->SetMatrix(d3dThis);
	}

	void Return(IBaseObject* _pThis, float _fTime)
	{
		CAIFist* pThis = (CAIFist*)_pThis;

		/*Sphere test = pThis->GetSphere();
		test.m_d3dColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
		pThis->SetSphere(test);*/
		((CAIFist*)_pThis)->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));

		D3DXVECTOR3 d3dMyPos(&pThis->GetMatrix()[12]), d3dHome(pThis->GetHome()), d3dTT;
		d3dTT = d3dHome - d3dMyPos;
		float fSqD = D3DXVec3LengthSq(&d3dTT);

		if( fSqD < .05f )
		{
			pThis->SetTime(0.0f);
			pThis->SetBehavior(AIFist::Idle);
			return;
		}

		// TODO - MAKE SOME REAL SPEEDS FOR AIFIST
		D3DXMATRIX d3dTranslate, d3dThis = pThis->GetMatrix();
		D3DXVECTOR3 d3dDir;
		D3DXVec3Normalize(&d3dDir, &d3dTT);
		d3dDir = _fTime * TRASHBOT_SPEED * d3dDir;
		D3DXMatrixTranslation(&d3dTranslate, d3dDir.x, d3dDir.y, d3dDir.z);
		d3dThis *= d3dTranslate;
		pThis->SetMatrix(d3dThis);	
	}
}
