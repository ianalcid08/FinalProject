#include "EnemyAI.h"


#include "GameplayState.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "Game.h"
#include "Audio.h"
#include "ExplodingBullet.h"
#include "AIBehaviors.h"
#include "ArcBullet.h"


CEnemyAI::CEnemyAI(void)
{
	m_pThis = nullptr;
}

CEnemyAI::~CEnemyAI(void)
{
}

void CEnemyAI::Initialize(CEnemyObject* _pEnemy)
{
	m_pThis = _pEnemy;

	switch(m_pThis->GetID())
	{
	case OBJ_TRASH:
		{
			pUpdate = &CEnemyAI::TrashUpdate;
		}
		break;

	case OBJ_PATROL:
		{
			pUpdate = &CEnemyAI::PatrolUpdate;
		}
		break;
		
	case OBJ_SMASH:
		{
			pUpdate = &CEnemyAI::SmashUpdate;
		}
		break;
	case OBJ_BURN:
		{
			pUpdate = &CEnemyAI::BurnUpdate;
		}
		break;

	case OBJ_SPIDER:
		{
			pUpdate = &CEnemyAI::SpiderUpdate;
		}
		break;

	case OBJ_STEAM:
		{
			pUpdate = &CEnemyAI::SteamUpdate;
		}
		break;

	case OBJ_SPINBOT:
		{
			pUpdate = &CEnemyAI::SpinUpdate;
		}
		break;

	case OBJ_AICORE:
		{
			pUpdate = &CEnemyAI::CoreUpdate;
		}
		break;

	case OBJ_AIFIST:
		{
			pUpdate = &CEnemyAI::FistUpdate;
		}
		break;
	}
}

void CEnemyAI::Update(float _fTime)
{
	if( !pUpdate )
		return;

	(this->*(this->pUpdate))(_fTime);
}

void CEnemyAI::TrashUpdate(float _fTime)
{
	bool bMove = Targeting::CheckTargetValid(m_pThis) || Targeting::Random(m_pThis);
	float fSpeed = ((CTrashBot*)m_pThis)->GetSpeed();
	if( bMove )
	{
		Movement::Avoid(m_pThis, _fTime, TRASHBOT_ROT_SPEED*1.25f);
		Movement::TurnToTarget(m_pThis, _fTime, TRASHBOT_ROT_SPEED);
		Movement::MoveForward(m_pThis, _fTime, fSpeed);
	}
}

void CEnemyAI::PatrolUpdate(float _fTime)
{
	D3DXMATRIX d3dThis = m_pThis->GetMatrix();
	D3DXVECTOR3 d3dDir = m_pThis->GetVelocity();

	Frustum ftest = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();

	D3DXVECTOR3 offset = ConfineToFrustum(ftest, m_pThis->GetSphere());
	if(offset.x != 0.0f || offset.z != 0.0f)
	{			
		D3DXMATRIX trans;

		D3DXMatrixTranslation(&trans,offset.x,offset.y,offset.z);
		d3dThis *= trans;
		m_pThis->SetMatrix(d3dThis);
	}

	Movement::MoveDirection(m_pThis, d3dDir, _fTime, PBOT_SPEED);


	// If Patrol bot collides with frustum
	char nPlaneHit = SphereToFrustum(m_pThis->GetSphere(), ftest);
	if(nPlaneHit)
	{
		D3DXVECTOR3 d3dTempVec = m_pThis->GetVelocity();

		switch(nPlaneHit)
		{
		case 2:
			{
				if(d3dTempVec.x < 0)
				{
					d3dTempVec.x = d3dTempVec.x * -1;
					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 3:
			{
				if(d3dTempVec.x > 0)
				{
					d3dTempVec.x = d3dTempVec.x * -1;
					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 4:
			{
				if(d3dTempVec.z > 0)
				{
					d3dTempVec.z = d3dTempVec.z * -1;
					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 5:
			{
				if(d3dTempVec.z < 0)
				{
					d3dTempVec.z = d3dTempVec.z * -1;
					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		}

		
	}


	D3DXVECTOR3 d3dMyPos = &m_pThis->GetMatrix()[12];

	bool bShoot = Targeting::CheckTargetValid(m_pThis) || Targeting::Far(m_pThis);

	if( !bShoot )
		return;

	Movement::LookAtTarget(m_pThis);

	if(((CPatrolBot*)m_pThis)->GetFireTime() > 1.25f)
	{
		CBullet* pBullet = nullptr;
		if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ENEMY, (IBaseObject**)&pBullet) )
		{
			CPlayerObject* pTarget = (CPlayerObject*)m_pThis->GetTargetFINAL();

			D3DXVECTOR3 d3dTargetPos(&pTarget->GetMatrix()[12]);
			D3DXVECTOR3 d3dToTarget = d3dTargetPos - d3dMyPos;

			Utilities::FastNormalize(d3dToTarget);
			pBullet->SetMatrix(m_pThis->GetMatrix());
			pBullet->SetType(ENEMYBULLET);
			pBullet->SetDamage(10.0f);

			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PATROLBOT_ATTACK);
			pBullet->SetVelocity( d3dToTarget );
			pBullet->SetColor(COLOR_ENEMY_BULLET);
		}

		((CPatrolBot*)m_pThis)->SetFireTime(0.0f);
	}

}


void CEnemyAI::SmashUpdate(float _fTime)
{
	CSmashBot* pSmash = (CSmashBot*)m_pThis;

	if( !Targeting::Close(m_pThis) )
		return;

	IBaseObject* pTarget = m_pThis->GetTargetFINAL();


	std::vector<CPlayerObject*> vPlayers = CGameplayState::GetInstance()->GetPlayers();
	unsigned int nSize = vPlayers.size();

	

	if(!pSmash->GetSmashing() && !SphereToFrustum(pSmash->GetSphere(), CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum()))
	{ 
		D3DXMATRIX d3dSmashBall, d3dTrans;
		D3DXMatrixTranslation(&d3dTrans, 0.0f, 0.0f, 3.5f);
		d3dSmashBall = d3dTrans * m_pThis->GetMatrix();
		Sphere tSphere = pSmash->GetSphere();
		tSphere.m_Radius *= .75f;
		tSphere.m_Center = *(D3DXVECTOR3*)&d3dSmashBall[12];

		for( unsigned int i = 0; i < nSize; ++i )
		{
			Sphere tPlayerSphere = vPlayers[i]->GetSphere();
			tPlayerSphere.m_Radius = .01f;

			if( SphereToSphere(tSphere, tPlayerSphere) && vPlayers[i]->GetAlive() )
			{
				pSmash->SetSmash(true);
				pSmash->SetSmashTime(0.0f);
				pSmash->ChangeAnimation(SMASH_PREP);
				pSmash->SetMaxAnim(pSmash->GetCurrentAnimation()->m_fDuration);
				pSmash->SetWalking(SMASH_PREP);
			}
		}
	}

	if( pSmash->GetSmashing())
	{
		if(pSmash->GetAnimationTime() >= pSmash->GetMaxAnim() && pSmash->GetWalking() == SMASH_PREP)
		{
			CExplodingBullet* pBullet = pSmash->GetBullet();
			if( !pBullet )
			{
				pSmash->ChangeAnimation(SMASH_SMASH);
				pSmash->SetMaxAnim(pSmash->GetCurrentAnimation()->m_fDuration);
				pSmash->SetWalking(SMASH_SMASH);
				if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_EXP, (IBaseObject**)&pBullet) )
				{
					pBullet->SetRadius(0.0f);
					pBullet->SetColor(COLOR_SMASHBOT_AOE);

					D3DXMATRIX d3dTranslate, d3dBulletMat = pSmash->GetMatrix();
					D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, 3.5f);
					d3dBulletMat = d3dTranslate * d3dBulletMat;
					pBullet->SetMatrix(d3dBulletMat);

					pBullet->SetDamage(SMASHBOT_DAMAGE);
					pBullet->SetType(SMASHBULLET);
					pBullet->SetMaxRadius(SMASHBOT_RADIUS);
					pBullet->SetSpeed(SMASHBOT_AOESPEED);
					
					pSmash->SetBullet(pBullet);
					CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMASHWAVE,d3dBulletMat);
				}

				// sound for smash attack
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_SMASHBOT_ATTACK);
			}

		}
			
		if(pSmash->GetAnimationTime() >= pSmash->GetMaxAnim() && pSmash->GetWalking() == SMASH_SMASH)
		{
			pSmash->SetSmash(false);
			pSmash->SetBullet(nullptr);
			pSmash->ChangeAnimation(SMASH_RESET);
			pSmash->SetMaxAnim(pSmash->GetCurrentAnimation()->m_fDuration);
			pSmash->SetWalking(SMASH_RESET);
			return;
		}
	}
	else
	{
		Movement::TurnToTarget(m_pThis, _fTime, SMASHBOT_ROT_SPEED);
		pSmash->SetSmash(false);
		pSmash->SetBullet(nullptr);
		Movement::MoveForward(m_pThis, _fTime, SMASHBOT_SPEED);
		if(pSmash->GetAnimationTime() >= pSmash->GetMaxAnim() && pSmash->GetWalking() == SMASH_RESET)
		{
			pSmash->SetSmash(false);
			pSmash->SetBullet(nullptr);
			pSmash->ChangeAnimation(SMASH_WALK);
			pSmash->SetMaxAnim(0.0f);
			pSmash->SetWalking(SMASH_WALK);
			return;
		}
	}
}

void CEnemyAI::BurnUpdate(float _fTime)
{
	CBurnBot* pBurnBot = ((CBurnBot*)m_pThis);
	
	bool bMove = Targeting::CheckTargetValid(m_pThis) || Targeting::Random(m_pThis);

	if( !bMove )
		return;

	if(!pBurnBot->GetCloseToTarget())
	{
		Movement::Avoid(m_pThis, _fTime, BURN_ROT_SPEED*1.5f);
		Movement::TurnToTarget(m_pThis, _fTime, BURN_ROT_SPEED);
		Movement::MoveForward(m_pThis, _fTime, BURN_SPEED);
	}

	if( pBurnBot->GetCloseToTarget() )
	{
		float fExpTimer = pBurnBot->GetExplodeTimer();
		fExpTimer -= _fTime;
		pBurnBot->SetExplodeTimer(fExpTimer);

		if( fExpTimer < 0.0f )
		{
			CExplodingBullet* pBullet = nullptr;

			if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_EXP, (IBaseObject**)&pBullet) )
			{
				 pBullet->SetRadius(0.0f);
				 pBullet->SetColor(COLOR_BURN_AOE);

				D3DXMATRIX d3dBulletMat = pBurnBot->GetMatrix();
				pBullet->SetMatrix(d3dBulletMat);
				pBullet->SetDamage(BURN_DAMAGE);
				pBullet->SetType(BARRELBULLET);
				pBullet->SetMaxRadius(BURN_RADIUS);
				pBullet->SetSpeed(BURN_AOESPEED);

				pBurnBot->SetHealth(0.0f);
			}

			// sound for smash attack
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		}


	}
}

void CEnemyAI::SpiderUpdate(float _fTime)
{
	if( !m_pThis->GetAlive() )
		return;

	CSpiderBot* pThis = (CSpiderBot*)m_pThis;

	if( pThis->GetIsWalking() )
	{
		if( !pThis->GetTargetFINAL() )
		{
			if( !Targeting::Random(pThis) )
				return;

			float nXOff = rand()%24-10.0f, nZOff = (float)(rand()%10);
			D3DXMATRIX d3dTranslate, d3dCamera = CGameplayState::GetInstance()->GetCamera()->GetCamera();
			d3dCamera._42 = 0.0f;
			D3DXMatrixTranslation(&d3dTranslate, nXOff, 0.0f, nZOff);
			d3dTranslate = d3dCamera*d3dTranslate;

			pThis->SetAnchor(&d3dTranslate[12]);
		}

		D3DXVECTOR3 d3dMyPos(&pThis->GetMatrix()[12]), d3dAnchor = pThis->GetAnchor(), d3dTT;
		d3dTT = pThis->GetAnchor() - d3dMyPos;
		float fSqD = D3DXVec3LengthSq(&d3dTT);
		
		if( fSqD < .25f )
		{
			pThis->SetTime(0.0f);
			pThis->SetIsWalking(false);
		}
		else
		{
			Utilities::FastNormalize(d3dTT);
			Movement::MoveDirection(pThis, d3dTT, _fTime, SMASHBOT_SPEED);
		}

		return;
	}

	float fTime = pThis->GetTime();
	fTime += _fTime;
	pThis->SetTime(fTime);

	bool bShoot = Targeting::CheckTargetValid(pThis) || Targeting::Random(pThis);
	if( !bShoot )
		return;

	float fSpeed = SMASHBOT_ROT_SPEED * fTime;
	fSpeed = min(SMASHBOT_ROT_SPEED * 2.0f, fSpeed);
		
	
	if( Movement::TurnToTarget(pThis, _fTime, fSpeed) )
		Movement::LookAtTarget(pThis);
	else
		return;
	
	if( fTime > SPIDER_FIRE_SPEED )
	{
		CArcBullet* pBullet = nullptr;
		if( CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_ARC, (IBaseObject**)&pBullet) )
		{
			IBaseObject* pTarget = pThis->GetTargetFINAL();
			Sphere tSphere = pTarget->GetSphere();
			tSphere.m_Radius = .05f;
			pBullet->SetTargetSphere(tSphere);
			pBullet->SetColor(COLOR_ENEMY_BULLET);

			D3DXMATRIX d3dBulletMat = pThis->GetMatrix();
			d3dBulletMat._42 = 0.5f;
			pBullet->SetMatrix(d3dBulletMat);
			pBullet->SetAlive(true);

			pThis->SetTargetFINAL(nullptr);
			pThis->SetTime(0.0f);
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SPIDERFIRE,pThis->GetMatrix());
		}
	}
}

void CEnemyAI::SteamUpdate(float _fTime)
{
	if( !m_pThis->GetAlive() )
		return;

	Behavior pBehavior = ((CSteamrollerBot*)m_pThis)->GetBehavior();

	if( pBehavior == Steam::Idle || !Targeting::CheckTargetValid(m_pThis) )
	{
		if( !Targeting::Random(m_pThis) )
			return;
	}

	((CSteamrollerBot*)m_pThis)->GetBehavior()(m_pThis,_fTime);
}

void CEnemyAI::SpinUpdate(float _fTime)
{
	if( !m_pThis->GetAlive() )
		return;

	Behavior pBehavior = ((CSpinBot*)m_pThis)->GetBehavior();

	if( pBehavior )
		((CSpinBot*)m_pThis)->GetBehavior()(m_pThis, _fTime);

	bool bReturn = pBehavior == SpinBot::Idle || pBehavior == SpinBot::Reset || pBehavior == SpinBot::Enter;
	if( bReturn )
		return;

	CSpinBot* pThis = (CSpinBot*)m_pThis;

	D3DXMATRIX d3dThis = m_pThis->GetMatrix();
	D3DXVECTOR3 d3dDir = m_pThis->GetVelocity();

	Frustum ftest = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();

	float fSpeed = pThis->GetSpeed();
	Movement::MoveDirection(m_pThis, d3dDir, _fTime, fSpeed);


	char nPlaneHit = SphereToFrustum(m_pThis->GetSphere(), ftest);
	if(nPlaneHit)
	{
		D3DXVECTOR3 d3dTempVec = m_pThis->GetVelocity();
		float fRand = (float)(rand()%40+61)/100.0f;

		switch(nPlaneHit)
		{
		case 2:
			{
				if(d3dTempVec.x < 0)
				{
					d3dTempVec.x = d3dTempVec.x * -1;
					if( d3dTempVec.x < 0.0f )
						fRand *= -1.0f;
					d3dTempVec.x = fRand;

					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 3:
			{
				if(d3dTempVec.x > 0)
				{
					d3dTempVec.x = d3dTempVec.x * -1;
					if( d3dTempVec.x < 0.0f )
						fRand *= -1.0f;
					d3dTempVec.x = fRand;

					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 4:
			{
				if(d3dTempVec.z > 0)
				{
					d3dTempVec.z = d3dTempVec.z * -1;
					if( d3dTempVec.z < 0.0f )
						fRand *= -1.0f;
					d3dTempVec.z = fRand;

					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		case 5:
			{
				if(d3dTempVec.z < 0)
				{
					d3dTempVec.z = d3dTempVec.z * -1;
					if( d3dTempVec.z < 0.0f )
						fRand *= -1.0f;
					d3dTempVec.z = fRand;

					m_pThis->SetVelocity(d3dTempVec);
				}
			}
			break;
		}

		
	}

	
}

void CEnemyAI::CoreUpdate(float _fTime)
{
	CAICore* pThis = (CAICore*)m_pThis;
	
	if( !Targeting::Random(pThis) )
		return;

	if( !pThis->GetLeftFist()->GetAlive() )
		pThis->GetLeftFist()->SetBehavior(AIFist::Idle);

	if( !pThis->GetRightFist()->GetAlive() )
		pThis->GetRightFist()->SetBehavior(AIFist::Idle);

	float fTime = pThis->GetTime();
	Behavior pMyBehavior = pThis->GetBehavior();
	
	if( fTime > 15.0f )
	{
		Behavior pNewBehavior;
		pNewBehavior = pMyBehavior == AICore::Fire ? AICore::BurstFire : AICore::Fire;
		pThis->SetTime(0.0f);
		pThis->SetBehavior(pNewBehavior);
	}

	Behavior bLeft = pThis->GetLeftFist()->GetBehavior();
	Behavior bRight = pThis->GetRightFist()->GetBehavior();

	if( bLeft == AIFist::Idle && bRight == AIFist::Idle )
	{
		if( pThis->GetLeftFist()->GetAlive() )
			pThis->GetLeftFist()->SetBehavior(AIFist::Approach);
		else if( pThis->GetRightFist()->GetAlive() )
			pThis->GetRightFist()->SetBehavior(AIFist::Approach);
	}
	else if( bLeft == AIFist::Return && bRight == AIFist::Idle )
	{
		if( pThis->GetRightFist()->GetAlive() )
			pThis->GetRightFist()->SetBehavior(AIFist::Approach);
	}
	else if( bRight == AIFist::Return && bLeft == AIFist::Idle )
	{
		if( pThis->GetLeftFist()->GetAlive() )
			pThis->GetLeftFist()->SetBehavior(AIFist::Approach);
	}

	if( pThis->GetMoveBehavior() )
		pThis->GetMoveBehavior()(pThis, _fTime);

	if( pThis->GetBehavior() )
		pThis->GetBehavior()(m_pThis, _fTime);
}

void CEnemyAI::FistUpdate(float _fTime)
{
	if( !m_pThis->GetAlive() )
		return;

	Behavior pBehavior = ((CAIFist*)m_pThis)->GetBehavior();

	if( pBehavior == AIFist::Idle || !Targeting::CheckTargetValid(m_pThis) )
	{
		if( !Targeting::Random(m_pThis) )
			return;
	}

	((CAIFist*)m_pThis)->GetBehavior()(m_pThis,_fTime);
}
