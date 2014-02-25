/************************************************************************************
 * Filename:  		InversionTower.cpp
 * Date:      		03/19/2013
 * Mod. Date: 		03/19/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the inversion tower that will invert player controls if 
					they are struck by its inversion wave.
 ************************************************************************************/

#include "InversionTower.h"
#include "GameplayState.h"
#include "Game.h"

CInversionTower::CInversionTower(void)
{
	SetID(OBJ_INVERSION); 
	SetRadius(INVERSION_RADIUS);
	SetColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	SetHealth(MAXHP_INVERSION); 
	SetAlive(true);
	SetBeingHit(11);
	SetIsActive(true);
	SetInversionTimer(60.0f);
	m_fInversionAtktimer = 0.0f;
	m_fSpawnTimer = 2.75f;
	m_fMaxRadius = 0.0f;
	m_fAttackTime = 0.0f;
	m_fAttackSpeed = 0.0f;
}

CInversionTower::~CInversionTower(void)
{
}


void CInversionTower::Update(float fDelta)
{
	KillCurrFrame();
	//float animTime = GetAnimationTime();
	//animTime += fDelta;
	//CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	//CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	//SetAnimationTime(animTime);

	
	
	m_fInversionTimer -= fDelta;

	if(m_fInversionTimer <= 0)
	{
		Frustum tTestFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(), tTestFrustum))
		{
			cout << "Inversion Destroyed" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}
	
	/*if(GetHealth() <= 0.0f)
	{
		D3DXMATRIX transMat1,transMat2,transMat3,transMat4;
		D3DXMatrixTranslation(&transMat1,1.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat2,-2.0f,0.0,0.0);
		D3DXMatrixTranslation(&transMat3,1.0f,0.0,1.0);
		D3DXMatrixTranslation(&transMat4,0.0f,0.0,-2.0);
		D3DXMATRIX d3dMat=GetMatrix();
		d3dMat *= transMat1;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(0,d3dMat);
		d3dMat *= transMat2;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(0,d3dMat);
		d3dMat *= transMat3;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(0,d3dMat);
		d3dMat *= transMat4;
		CGameplayState::GetInstance()->GetFX()->CreateEffect(0,d3dMat);

		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);


		SetAlive(false);
		SetIsActive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
	}*/

	if(m_fSpawnTimer <= 0.0f)
	{
		m_fInversionAtktimer += fDelta;

		if(m_fInversionAtktimer >= m_fAttackTime - .5f)
			SetBeingHit(10);

		if(m_fInversionAtktimer >= m_fAttackTime)
		{
			m_fInversionAtktimer = 0.0f;
		

			CExplodingBullet* tempBullet = nullptr;
			if(CGame::GetInstance()->GetOF()->Create(OBJ_BULLET_EXP,(IBaseObject**)&tempBullet))
			{
				tempBullet->SetMatrix(GetMatrix());
				tempBullet->SetType(INVERSION_BULLET);
				tempBullet->SetDamage(0.0f);   
				tempBullet->SetMaxRadius(m_fMaxRadius); 
				tempBullet->SetSpeed(m_fAttackSpeed); 

				tempBullet->SetRadius(0.0f);
				tempBullet->SetColor(D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f));
			}

			SetBeingHit(11);
		}
	}
	else
	{
		m_fSpawnTimer -= fDelta;
		D3DXMATRIX tempMat;
		tempMat = this->GetMatrix();
		tempMat[13] += m_fSpawnTimer * 0.01f;
		this->SetMatrix(tempMat);
		if(m_fSpawnTimer <= 0.0f)
			tempMat[13] = 0.0f;
	}
}


bool CInversionTower::CheckCollision(IBaseObject* pObj)
{
	return false;
}


void CInversionTower::CollisionResponse(IBaseObject* pObj)
{
}

	
void CInversionTower::TakeDamage(IBaseObject* _pIn)
{
	if(_pIn->GetID() != OBJ_TESLA)
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_INVULSPARK, GetMatrix());
}
