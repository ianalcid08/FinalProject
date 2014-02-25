/************************************************************************************
 * Filename:  		Vat.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/09/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the destructable Vat that will spill molten steel
 ************************************************************************************/

#include "Vat.h"
#include "GameplayState.h"
#include "Game.h"
#include "Audio.h"

CVat::CVat(void)
{
	SetID(OBJ_VAT);
	SetRadius(0.75f);
	m_fDying = 0.0f;
}


CVat::~CVat(void)
{
}

void CVat::Update(float fDelta)
{
	if(m_fIsFading > 0.0f)
	{
		m_fIsFading -= fDelta;
		D3DXMATRIX tempCurr = GetMatrix();
		tempCurr[13] -= m_fIsFading * 0.05f;
		SetMatrix(tempCurr);

		if(m_fIsFading <= 0.0f)
		{
			CGame::GetInstance()->GetOF()->Destroy(this);
			SetIsActive(false);
		}
		return;
	}

	if(m_fDying)
	{
		m_fDying += fDelta;

		if( m_fDying > m_fDieTime )
		{
			m_fIsFading = 2.0f;
			m_fDying = 0.0f;
			return;
		}
		float animTime = GetAnimationTime();
		animTime += fDelta;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);
		return;
	}
	else
		KillCurrFrame();

	m_fVatTimer -= fDelta;

	if(m_fVatTimer <= 0)
	{
		Frustum tTestFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(), tTestFrustum))
		{
			cout << "Vat destroyed" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			m_fDying = 0.0f;
			SetIsActive(false);
			return;
		}
	}

	if(GetHealth() <= 0.0f)
	{
		SetAlive(false);
		CMoltenPool* pMoltenPool = nullptr;
		if(CGame::GetInstance()->GetOF()->Create(OBJ_POOL, (IBaseObject**)&pMoltenPool))
		{
			pMoltenPool->SetMatrix(GetMatrix());
			pMoltenPool->SetOriginalMatrix(GetMatrix());
			pMoltenPool->SetRadius(1.25f);
			pMoltenPool->SetScale(1.25f);
			pMoltenPool->SetScaleMod(1.4f);
			pMoltenPool->SetScaleTimer(1.0f);
			pMoltenPool->SetPoolStage(1);
			pMoltenPool->SetScalingMod(0.0f);
			pMoltenPool->SetDroppedID(2);
		}

		D3DXMATRIX d3dMat=GetMatrix();
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_EXPLOSION);
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MEDEXP,d3dMat);
		TAnimation* tempAnim = CAssetManager::GetInstance()->GetAnimation(GetID(), 0);
		m_fDieTime = tempAnim->m_fDuration;
		m_fDying = .00001f;
		ChangeAnimation(0);
	}
}

bool CVat::CheckCollision(IBaseObject* pObj)
{
	return true;
}

void CVat::CollisionResponse(IBaseObject* pObj)
{
	
}

 void CVat::TakeDamage(IBaseObject* _pIn)
 {
	 SetHealth(GetHealth() - ((CBullet*)_pIn)->GetDamage());
 }
