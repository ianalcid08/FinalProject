/************************************************************************************
 * Filename:  		MoltenPool.cpp
 * Date:      		02/11/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	MB
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the environmental hazard that comes from Vats and Burn Bots 
 ************************************************************************************/

#include "MoltenPool.h"
#include "GameplayState.h"
#include "Game.h"
#include "Effect.h"

CMoltenPool::CMoltenPool(void)
{
	SetID(OBJ_POOL);
	SetColor(MOLTEN_COLOR);
	SetRadius(1.25f);
	m_fMoltenTimer = 60.0f;
	m_fDamage = MOLTEN_DAMAGE;
	SetHealth(0);
	SetAlive(true);
	SetIsActive(true);
	D3DXMatrixIdentity(&m_d3dOriginalMatrix); 
	m_bPoolCollision = false;
	m_bSafetoCollide = false;
	m_fScale = 1.25f;
	m_fScaleModifier = 1.4f;
	m_fScaleTimer = 0.0f;
	D3DXMatrixIdentity(&m_d3dScaleMatrix);
	m_fScalingMod = 1.0f;
	m_nPoolStage = 1;
	m_fDestroyTimer = 0.0f;
	m_nDroppedID = 0;
	m_pBurnEffect = nullptr;
	m_fDamageDelay = 0.3f;
}


CMoltenPool::~CMoltenPool(void)
{
}


void CMoltenPool::Update(float fDelta)
{

	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);

	if(m_fDamageDelay > 0.0f)
		m_fDamageDelay -= fDelta;

	m_fMoltenTimer -= fDelta;

	if(m_fMoltenTimer <= 0)
	{
		Frustum tTestFrustum = CGameplayState::GetInstance()->GetCamera()->GetFrustum();

		if(SphereToFrustumBehind(GetSphere(), tTestFrustum))
		{
			if(m_pBurnEffect)
				CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pBurnEffect);
			m_pBurnEffect = nullptr;
			cout << "pool destroyed" << endl;
			CGame::GetInstance()->GetOF()->Destroy(this);
			return;
		}
	}

	m_fDamage = MOLTEN_DAMAGE * fDelta;

	if(m_fScaleTimer > 0.0f)
	{
		m_fScaleTimer -= fDelta;

		if(m_nPoolStage == 1)
			m_fScalingMod += (float)(fDelta*3.5f);
		else if(m_nDroppedID == 1 && m_nPoolStage == 2)
			m_fScalingMod += (float)(fDelta*0.35f);
		else
			m_fScalingMod += (float)(fDelta*0.20f);

		if(m_fScalingMod >= 0.65f && m_nDroppedID == 1 && m_nPoolStage == 1)
			m_fScalingMod = 0.65f;
		else if(m_fScalingMod >= 0.75f && m_nDroppedID == 1 && m_nPoolStage == 2)
			m_fScalingMod = 0.75f;
		else if(m_fScalingMod >= 1.05f)
			m_fScalingMod = 1.05f;

		if(m_nPoolStage == 4)
			m_fScalingMod = 1.0f;

		D3DXMATRIX ScaleMatrix, NewMatrix, tempScaleMatrix, newOGMatrix;
		if(m_nPoolStage == 1)
		{
			tempScaleMatrix = GetScaleMat();
			tempScaleMatrix._11 = 1.25;
			tempScaleMatrix._33 = 1.25;
			SetScaleMatrix(tempScaleMatrix);
		}
		tempScaleMatrix = GetScaleMat();
		tempScaleMatrix._11 *= m_fScalingMod;
		tempScaleMatrix._33 *= m_fScalingMod;
		NewMatrix = GetOriginalMatrix();

		if(m_nPoolStage != 1)
		{
			newOGMatrix = GetOriginalMatrix();
			newOGMatrix._41 = GetMatrix()._41;
			newOGMatrix._42 = GetMatrix()._42;
			newOGMatrix._43 = GetMatrix()._43;
			SetOriginalMatrix(newOGMatrix);
		}
		
		D3DXMatrixScaling(&ScaleMatrix, tempScaleMatrix._11, 1.0f, tempScaleMatrix._33);
		D3DXMatrixMultiply(&NewMatrix, &ScaleMatrix, &NewMatrix);
		SetMatrix(NewMatrix);

		if(m_fScaleTimer <= 0.0f && m_nPoolStage != 1)
		{
			SetScale(tempScaleMatrix._11);
			m_fScaleTimer = 0.0f;
		}
	}

	if(m_bSafetoCollide == true && m_nPoolStage != 4)
		m_bPoolCollision = false;

	if(m_fDestroyTimer > 0.0f)
	{
		m_fDestroyTimer -= fDelta;
		m_fScalingMod -= fDelta;
		if(m_fScalingMod <= 0.0f)
			m_fScalingMod = 0.0f;

		D3DXMATRIX ScaleMatrix, NewMatrix, tempScaleMatrix, newOGMatrix;
		tempScaleMatrix = GetScaleMat();
		tempScaleMatrix._11 *= m_fScalingMod;
		tempScaleMatrix._33 *= m_fScalingMod;
		NewMatrix = GetOriginalMatrix();

		newOGMatrix = GetOriginalMatrix();
		newOGMatrix._41 = GetMatrix()._41;
		newOGMatrix._42 = GetMatrix()._42;
		newOGMatrix._43 = GetMatrix()._43;
		SetOriginalMatrix(newOGMatrix);
		
		D3DXMatrixScaling(&ScaleMatrix, tempScaleMatrix._11, 1.0f, tempScaleMatrix._33);
		D3DXMatrixMultiply(&NewMatrix, &ScaleMatrix, &NewMatrix);
		SetMatrix(NewMatrix);

		if(m_fDestroyTimer <= 0.0f)
		{
			if(m_pBurnEffect)
				CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pBurnEffect);
			m_pBurnEffect = nullptr;
			m_fDestroyTimer = 0.0f;
			CGame::GetInstance()->GetOF()->Destroy(this);
		}
	}
	if(m_pBurnEffect && m_pBurnEffect->GetLife() <= 0.0f)
		m_pBurnEffect = nullptr;
}

	
bool CMoltenPool::CheckCollision(IBaseObject* pObj)
{
	if(pObj == this || !pObj->GetAlive() || !GetAlive() || m_fDamageDelay > 0.0f)
		return false;

	int nType = pObj->GetID();

	if(nType == OBJ_TRASH || nType == OBJ_SMASH || nType == OBJ_PLAYER ||
		nType == OBJ_POOL || nType == OBJ_BURN)
	{

		if(nType == OBJ_POOL && (m_bPoolCollision || ((CMoltenPool*)pObj)->GetPoolCollision()) )
		{
			m_bSafetoCollide = true;
			return false;
		}

		if(SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			CollisionResponse(pObj);
			return true;
		}
	}

	return false;
}

void CMoltenPool::CollisionResponse(IBaseObject* pObj)
{
	if(pObj->GetID() == OBJ_POOL)
	{
		m_bPoolCollision = true;
		((CMoltenPool*)pObj)->SetPoolCollision(true);

		CMoltenPool* pNewPool = nullptr;
		if( CGame::GetInstance()->GetOF()->Create(OBJ_POOL, (IBaseObject**)&pNewPool) )
			{
				//Change this to check for scale and store it before using it.
				//Also check for scale mods and store them before using.
				float fGrowScaleMod = 0.0f;
				if(m_fScale >= ((CMoltenPool*)pObj)->GetScale())
				{
					pNewPool->SetDroppedID(m_nDroppedID);
					pNewPool->SetRadius(m_fScale*m_fScaleModifier);
					fGrowScaleMod = m_fScaleModifier;
					if(GetPoolStage() >= 4)
						pNewPool->SetPoolStage(4);
					else
						pNewPool->SetPoolStage(GetPoolStage() + 1);
				}
				else
				{
					pNewPool->SetDroppedID(((CMoltenPool*)pObj)->GetDroppedID());
					pNewPool->SetRadius(((CMoltenPool*)pObj)->GetScale()*((CMoltenPool*)pObj)->GetScaleMod());
					fGrowScaleMod = ((CMoltenPool*)pObj)->GetScaleMod();
					if(((CMoltenPool*)pObj)->GetPoolStage() >= 4)
						pNewPool->SetPoolStage(4);
					else
						pNewPool->SetPoolStage(((CMoltenPool*)pObj)->GetPoolStage() + 1);
				}

				float fGrowScale = pNewPool->GetRadius();
				pNewPool->SetScale(fGrowScale);
				fGrowScaleMod *= 0.85f;
				if(fGrowScaleMod <= 1.0f)
					pNewPool->SetScaleMod(1.0f);
				else
					pNewPool->SetScaleMod(fGrowScaleMod);

				pNewPool->SetColor(MOLTEN_COLOR);
				D3DXMATRIX FinalMatrix, ScaleMatrix;
				D3DXMatrixIdentity(&FinalMatrix);
				D3DXMATRIX d3dTargetMatrix = pObj->GetMatrix();
				D3DXMATRIX tempTestMat = GetMatrix();
				if(fGrowScaleMod > 1.0f)
				{
					FinalMatrix._41 = (GetMatrix()._41 + d3dTargetMatrix._41)/2;
					FinalMatrix._42 = (GetMatrix()._42 + d3dTargetMatrix._42)/2;
					FinalMatrix._43 = (GetMatrix()._43 + d3dTargetMatrix._43)/2;
				}
				else
				{
					if( m_fScale > ((CMoltenPool*)pObj)->GetScale())
					{
						FinalMatrix._41 = GetMatrix()._41;
						FinalMatrix._42 = GetMatrix()._42;
						FinalMatrix._43 = GetMatrix()._43;
					}
					else
					{
						FinalMatrix._41 = pObj->GetMatrix()._41;
						FinalMatrix._42 = pObj->GetMatrix()._42;
						FinalMatrix._43 = pObj->GetMatrix()._43;
					}
				}
				D3DXMatrixIdentity(&ScaleMatrix);
				D3DXMatrixScaling(&ScaleMatrix, pNewPool->GetScale(), 1.0f, pNewPool->GetScale());
				pNewPool->SetScaleMatrix(ScaleMatrix); //storing scale matrix to use for over time effect : TODO : over time effect
				//D3DXMatrixMultiply(&FinalMatrix, &ScaleMatrix, &FinalMatrix);
				pNewPool->SetMatrix(FinalMatrix);
				pNewPool->SetOriginalMatrix(FinalMatrix);
				pNewPool->SetDamage(MOLTEN_DAMAGE);
				pNewPool->SetPoolCollision(false);
				m_bSafetoCollide = true; 
				pNewPool->SetScaleTimer(1.0f);
				if(pNewPool->GetDroppedID() == 1 && pNewPool->GetPoolStage() == 2)
					pNewPool->SetScalingMod(0.5f);
				else
					pNewPool->SetScalingMod(1.0f);
				

				if(((CMoltenPool*)pObj)->GetPoolStage() < 4)
				{
					if(pNewPool->GetPoolStage() != 4)
					{
						((CMoltenPool*)pObj)->SetDestroyTimer(1.0f);
						pObj->SetAlive(false);
					}
					else
						pObj->SetAlive(false);
				}

				if(m_nPoolStage < 4)
				{
					if(pNewPool->GetPoolStage() != 4)
					{
						m_fDestroyTimer = 1.0f;
						SetAlive(false);
					}
					else
						SetAlive(false);
				}

				cout << "pool created" << endl;
			}
	}
	
	if(pObj->GetID() != OBJ_POOL && pObj->GetID() != OBJ_BURN)
	{
		if(!m_pBurnEffect)
			m_pBurnEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MOLTENBURN, GetMatrix());
		pObj->TakeDamage(this);
	}

	if(pObj->GetID() == OBJ_BURN)
		if(m_nPoolStage == 4)
		{
			if(!((CBurnBot*)pObj)->GetDontDrop())
			{
				((CBurnBot*)pObj)->SetDropWait(1.0f);
				((CBurnBot*)pObj)->SetDontDropPool(true);
			}	
		}
}

void CMoltenPool::ForceAnimation(float fDelta)
{
	float animTime = GetAnimationTime();
	animTime += fDelta;
	CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
	CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
	SetAnimationTime(animTime);
}

void CMoltenPool::Render(void)
{
	CRenderer* pRenderer = CRenderer::GetInstance();
	pRenderer->Render(this);
}
