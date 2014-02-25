#include "Turret.h"
#include "Definitions.h"
#include "Game.h"
#include "ObjectFactory.h"
#include "GameplayState.h"
#include "Audio.h"
#include "Renderer.h"

CTurret::CTurret(void)
{
	SetID(OBJ_TURRET);
	SetDamage(TURRET_DMG);
	SetRadius(TURRET_RADIUS);
	SetColor(COLOR_TRASHBOT);
	SetHealth(TURRET_HP);
	SetWasHit(false);
	SetBeingHit(0);
	m_fFireTimer = 0.0f;
	m_fRange = 5.0f;
	m_pOF = CGame::GetInstance()->GetOF();
	m_pRenderer = CRenderer::GetInstance();
	//D3DXCreateCylinder(m_pRenderer->GetDirect3DDevice(),0.75f,0.75f,5.0f,25,25,&EnemyBeamCylinderMesh,NULL);
	m_cTurretCap.m_Radius = TURRET_RADIUS;
	m_fSpawnTimer = 2.0f;
	m_fShotTimer = 0.0f;
	//m_fDamage = 10.0f;
	m_pLaser = NULL;
	m_fKillTimer = 60.0f;
}


CTurret::~CTurret(void)
{
}
void CTurret::Update(float fDelta)
{
	
	m_fKillTimer -= fDelta;

	if(m_fKillTimer < 0.0f)
	{
		Frustum tempFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(),tempFrustum))
		{
			cout << "Laser Destroyed" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}

	if(m_fSpawnTimer <= 0.0f)
	{

		// test
		D3DXMATRIX t = GetMatrix();
		t[13] = 0.0f;
		SetMatrix(t);
		m_fShotTimer -= fDelta;
		if(m_fShotTimer <= 0.0f)
		{
			if(m_pLaser == NULL)
				m_pLaser = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_LASER,GetMatrix());
			D3DXMATRIX newRotation = GetMatrix();
			D3DXMATRIX newHolderMatrix = GetMatrix();


			D3DXMatrixRotationY(&newRotation,D3DXToRadian(180.0f * (fDelta*0.5f)));

			newHolderMatrix = newRotation * newHolderMatrix;
			SetMatrix(newHolderMatrix);
			// Shooting logic
			Sphere tBulletSphere = GetSphere();
			m_sNextSphere = tBulletSphere;

			D3DXMATRIX transNext;//,thisNext;

			m_d3dNextMat = newHolderMatrix;

			//D3DXVECTOR3 d3dDirNext = GetMatrix() * 5.0f;
			D3DXMatrixTranslation(&transNext,0.0f,0.0f,5.0f);
			m_d3dNextMat = transNext * m_d3dNextMat;

			m_sNextSphere.m_Center = *(D3DXVECTOR3*)&m_d3dNextMat[12];

			//  MAKE A CAPSULE AND DO COLLISION
			m_cTurretCap.m_Segment.m_Start = tBulletSphere.m_Center;
			m_cTurretCap.m_Segment.m_End = m_sNextSphere.m_Center;
		}
	}
	else
	{
		m_fSpawnTimer -= fDelta;
		D3DXMATRIX spawnUp,tempCurr;
		//D3DXMatrixTranslation(&spawnUp,0.0f,-(m_fSpawnTimer * 0.1f),0.0f);
		tempCurr = this->GetMatrix();
		tempCurr[13] += m_fSpawnTimer * 0.01f;
		this->SetMatrix(tempCurr);
		
	}
	/*D3DXMATRIX viewproj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	
	m_pRenderer->DrawBeamCylinder(GetMatrix(),viewproj,GetColor(),tBulletSphere.m_Radius,EnemyBeamCylinderMesh);*/

	if(m_pLaser)
	{
		m_pLaser->SetMatrix(GetMatrix());
	}

	KillCurrFrame();
	//float animTime = GetAnimationTime();
	//animTime += fDelta;
	//CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	//CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	//SetAnimationTime(animTime);

	if(GetHealth() <= 0.0f)
	{
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_SMALLEXP,GetMatrix());
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);

		SetAlive(false);
		SetIsActive(false);
		CGame::GetInstance()->GetOF()->Destroy(this);
		if(m_pLaser)
			m_pLaser->Reset();
	}
	if(GetWasHit() == true)
	{
		float temp = GetHitDuration();
		temp += fDelta;
		SetHitDuration(temp);
		SetBeingHit(1);
		SetColor(COLOR_TRASHBOT_HIT);

		if(HIT_CAP < temp)
		{
			SetWasHit(false);
			SetHitDuration(0.0f);
			SetColor(COLOR_TRASHBOT);
			SetBeingHit(0);
		}
	}	
}
bool CTurret::CheckCollision(IBaseObject* pObj)
{
	int x = 0;
	if(!GetAlive() || !pObj->GetAlive())
		return false;

	int nType = pObj->GetID();

	if(nType == OBJ_PATROL)
		return true;
	if(nType == OBJ_PLAYER || nType >= OBJ_TRASH && nType <= OBJ_BURN)
	{
		if(CapsuleToSphere(m_cTurretCap,pObj->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
		if(nType == OBJ_PLAYER)
		{
			if(SphereToSphere(GetSphere(),pObj->GetSphere()))
			{
				pObj->AddToReactVector(this);
				//pObj->CollisionResponse(this);
				return true;
			}
		}
	}	

	return false;
}
void CTurret::CollisionResponse(IBaseObject* pObj)
{
	pObj->TakeDamage(this);
}

void CTurret::TakeDamage(IBaseObject* _pIn)
{
	if(((CBullet*)_pIn)->GetType() == SPECIALBULLET)
		SetHealth(0.0f);
	else
		SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
	SetWasHit(true);
}