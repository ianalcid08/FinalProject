/************************************************************************************
 * Filename:  		CPlayerObject.cpp
 * Date:      		02/08/2013
 * Mod. Date: 		02/22/2013
 * Mod. Initials:	IA
 * Author:    		Tyler D. Springer
 * Purpose:   		This acts as the player object.
 ************************************************************************************/
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "Definitions.h"
#include "TrashBot.h"
#include "Input.h"
#include "GameplayState.h"
#include "CharacterSelectState.h"
#include "Game.h"
#include "Audio.h"
#include <iostream>
#include "Barrel.h"
#include "Crate.h"
#include "Vat.h"
#include "AIBehaviors.h"
#include "IntroState.h"

enum EANIM {APLAYER_DEATH, APLAYER_DANCE};

void CPlayerObject::SetColorPlayer(D3DXVECTOR4 d3dColor)
{
	 m_d3dPlayerColor = d3dColor;
}

CPlayerObject::CPlayerObject(void)
{
	SetID(OBJ_PLAYER);
	m_nPlayerNum = 0;
	m_nPlayerColorNum = 0;
	m_fFireTime = 0.0f;
	SetRadius(0.5f);
	SetColor(COLOR_PLAYER);
	SetHealth(MAXHP_PLAYER);
	m_fDashTime = 0.0f;
	m_bDashing = false;
	m_fDashEnergy = PLAYER_MAX_ENERGY;
	m_d3dDashDir = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fMaxDashEnergy = PLAYER_MAX_ENERGY;
	m_nLives = 2;
	m_bSpecial = true;
	m_bInvulnerable = false;
	m_fInvulTime = 0.0f;
	m_d3dPlayerColor = COLOR_PLAYER;
	D3DXMATRIX d3dID;
	D3DXMatrixIdentity(&d3dID);
	m_d3dGunMatrix = m_d3dBottom = d3dID;
	D3DXMatrixTranslation(&m_d3dGunOffset, -.707f, 0.0f, .326f);
	m_bTransition = false;

	m_nGunID = STANDARD;
	m_fGunDuration = 0.0f;

	m_fTimer = 0.0f;
	m_bPlayerPush = false;
	m_bInverted = false;
	m_fInvertTimer = 0.0f;
	m_fDeathInvulTime = 0.0f;
	m_bDeath = false;
	m_pTeslaEffect = nullptr;

	m_bIsFalling = false;
	m_fFallingTimer = 2.0f;
	m_fDyingTime = 0;
}

CPlayerObject::~CPlayerObject(void)
{

}

void TurnTo(D3DXMATRIX& objMat, D3DXVECTOR3 target, float fDelta)
{
	if(!target[0] && !target[2])
		return;

	float test = 0.0f;
	D3DXVECTOR3 z = D3DXVECTOR3(objMat[8],objMat[9],objMat[10]);
	float dotPZ = D3DXVec3Dot(&z, &target);
	if(dotPZ > 0.95f)
	{
		D3DXVECTOR3 temp = target;
		D3DXVec3Normalize(&target, &temp);
		*(D3DXVECTOR3*)&objMat[8] = target;
		D3DXVECTOR3 newX, xNrm;
		D3DXVec3Cross(&newX, (D3DXVECTOR3*)&objMat[4], (D3DXVECTOR3*)&objMat[8]);
		temp = newX;
		D3DXVec3Normalize(&newX, &temp);
		*(D3DXVECTOR3*)&objMat[0] = newX;
		return;
	}

	D3DXVECTOR3 x = D3DXVECTOR3(objMat[0],objMat[1],objMat[2]);
	float dotPX = D3DXVec3Dot(&x, &target);

	if(dotPX > 0)
	{
		D3DXMATRIX rotation;
		D3DXMatrixRotationY(&rotation, fDelta * D3DXToRadian(PLAYER_TURN));
		objMat = rotation * objMat;
	}
	else
	{
		D3DXMATRIX rotation;
		D3DXMatrixRotationY(&rotation, fDelta * D3DXToRadian(-PLAYER_TURN));
		objMat = rotation * objMat;
	}
}

void CPlayerObject::Initialize(unsigned int _nPlayerNum, int _nPlayerColor, bool bCheckPoint)
{
	m_fPickupEffectTime = -1;
	m_fDancing = -1.0f;
	m_bDying = false;
	m_bIsFalling = false;
	m_nPlayerNum = _nPlayerNum;
	m_nPlayerColorNum = _nPlayerColor;
	m_pOF = CGame::GetInstance()->GetOF();
	m_d3dBottom = GetMatrix();
	m_bSpecial = true;
	SetAlive(true);
	SetLives(2);
	SetIsActive(true);
	SetInvulTime(0.0f);
	m_bTransition = false;
	if(CGame::GetInstance()->GetDifficulty() == NORMAL)
		SetHealth(MAXHP_PLAYER + 50.0f);
	else
		SetHealth(MAXHP_PLAYER);

	if(!bCheckPoint)
	{
		m_nGunID = STANDARD;
		m_fGunDuration = 0.0f;
		m_fMaxTime = STANDARD_WEAPON_SPEED;
	}
	else
		m_fMaxTime = GetMaxTime();
	

	m_bInverted = false;
	m_fInvertTimer = 0.0f;
	m_fDeathInvulTime = 0.0f;
	m_bDeath = false;

	m_d3dRingTexture=CAssetManager::GetInstance()->LoadTexture("Textures/PlayerRing.png");
	switch(_nPlayerColor)
	{
	case BLUE:
		{
			SetColor(D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
			m_d3dPlayerColor =  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		break;
	case RED:
		{
			SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
			m_d3dPlayerColor =  D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		break;
	case GREEN:
		{
			SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
			m_d3dPlayerColor =  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		break;
	case YELLOW:
		{
			SetColor(D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f));
			m_d3dPlayerColor =  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		break;
	}
}

void CPlayerObject::KillPlayer()
{
	m_nLives -= 1;
	m_nGunID = STANDARD;
	m_fMaxTime = STANDARD_WEAPON_SPEED;
	m_fGunDuration = -1.0f;
	m_bDeath = true;
	SetBeingHit(0);
	if(m_nLives < 0)
	{
		SetAlive(false);
		SetIsActive(false);
		SetHealth(0.0f);
		m_fDashEnergy = 0.0f;
		m_bSpecial = false;
		return;
	}

	// This needs to change to a walk animation when we get one
	KillCurrFrame();
	m_bInvulnerable = true;
	m_fGunDuration = -1.0f;
	if(CGame::GetInstance()->GetDifficulty() == NORMAL)
		SetHealth(MAXHP_PLAYER + 50.0f);
	else
		SetHealth(MAXHP_PLAYER);
	SetSpecial(true);
	m_fDeathInvulTime = 3.0f;
	m_bDeath = true;
	ResetPosition(m_nPlayerNum);
	SetVelocity(D3DXVECTOR3(0.0f,0.0f,1.0f));
	m_d3dTarget = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_bInverted = false;
	m_fInvertTimer = 0.0f;
	m_bIsFalling = false;
	m_fFallingTimer = 2.0f;
	m_bDying = false;
}

void CPlayerObject::ResetPosition(unsigned int _nPlayerNum)
{
	D3DXMATRIX tempMat = CGameplayState::GetInstance()->GetCamera()->GetCamera();
	D3DXMATRIX tempSetMat;
	D3DXMatrixIdentity(&tempSetMat);
	tempSetMat._41 = tempMat._41 - 5.0f;
	tempSetMat._41 += (3.0f*_nPlayerNum);
	tempSetMat._42 = 0.0f;
	tempSetMat._43 = tempMat._43-6.0f;
	SetMatrix(tempSetMat);
	m_d3dBottom = tempSetMat;
	m_d3dTarget = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_d3dBottomTarget = D3DXVECTOR3(0.0f,0.0f,1.0f);
	SetVelocity(D3DXVECTOR3(0.0f,0.0f,1.0f));
}

void CPlayerObject::Update(float fDelta)
{
	if(!GetIsActive())
		return;

	if(GetAsyncKeyState(VK_TAB))
		SetLives(2);

	if(m_bDying)
	{
		if(m_fDyingTime > 0)
		{
			m_fDyingTime -= fDelta;
			float animTime = GetAnimationTime();
			animTime += fDelta;
			if(animTime > GetCurrentAnimation()->m_fDuration)
				animTime = GetCurrentAnimation()->m_fDuration;
			CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
			CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
			SetAnimationTime(animTime);
			return;
		}
		if(m_fDyingTime < 0)
		{
			m_fFadingTime = 1.0f;
			m_fDyingTime = 0;
		}

		if(m_fFadingTime > 0)
		{
			m_fFadingTime -= fDelta;
			D3DXMATRIX tempCurr = GetMatrix();
			tempCurr[13] -= m_fFadingTime * 0.05f;
			SetMatrix(tempCurr);
			return;
		}
		else
		{
			KillPlayer();
			return;
		}
	}

	if(m_bIsFalling && m_fFallingTimer == 2.0f)
	{

		m_fFallingTimer -= fDelta;
		D3DXMATRIX rot;
		D3DXMatrixRotationX(&rot,fDelta * 10.0f);
		rot = rot * GetMatrix();
		rot[13] -= m_fFallingTimer * 0.05f;

		SetMatrix(rot);
		m_d3dBottom = rot;
		if(m_pTeslaEffect)
		{
			CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
			m_pTeslaEffect = nullptr;
		}
		return;
	}
	else if(m_bIsFalling && m_fFallingTimer < 2.0f)
	{
		m_fFallingTimer -= fDelta;
		D3DXMATRIX rot;
		D3DXMatrixRotationX(&rot,fDelta * 10.0f);
		rot = rot * GetMatrix();
		rot[13] -= m_fFallingTimer * 0.05f;

		SetMatrix(rot);
		m_d3dBottom = rot;

		if(m_fFallingTimer <= 0.0f)
		{
			D3DXMATRIX d3dBurn = GetMatrix();
			d3dBurn._42 = 0.0f;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_MOLTENBURN,d3dBurn);
			KillPlayer();
			return;
		}
		return;
	}

	// Pickup Effect
	if(m_fPickupEffectTime > 0)
		m_fPickupEffectTime -= fDelta;

	if(m_fDancing > 0)
	{
		m_fDancing -= fDelta;
		float animTime = GetAnimationTime();
		animTime += fDelta;
		if(animTime > GetCurrentAnimation()->m_fDuration)
			animTime = GetCurrentAnimation()->m_fDuration;
		CAnimationProcessor::UpdateTime(GetCurrentAnimation(), animTime);
		CAnimationProcessor::Interpolator(GetCurrentAnimation(), animTime, PassCurrFrame());
		SetAnimationTime(animTime);
		return;
	}

#if _DEBUG
	if( GetAsyncKeyState('P') & 0x01 )
	{
		std::cout << "CAMERA - X: " << CGameplayState::GetInstance()->GetCamera()->GetCamera()._41 << " | Z: " << CGameplayState::GetInstance()->GetCamera()->GetCamera()._43 << endl;
		std::cout << "PLAYER - X: " << GetMatrix()._41 << " | Z: " << GetMatrix()._43 << endl << endl;
	}
#endif

	m_d3dBottom._41 = GetMatrix()._41;
	m_d3dBottom._43 = GetMatrix()._43;

	if(m_bInverted)
		SetBeingHit(8);
	else
		SetBeingHit(0);

	if( m_bDeath )
	{
		float fMinZ = CGameplayState::GetInstance()->GetCamera()->GetCamera()._43;
		bool bOnscreen = GetMatrix()._43 > fMinZ;

		if( !bOnscreen )
		{
			Movement::MoveDirection(this, D3DXVECTOR3(0.0f, 0.0f, 1.0f), fDelta, PLAYER_SPEED);
			return;
		}

		m_bInvulnerable = true;
		m_fInvulTime = -1.5f;
		m_bDeath = false;
	}
	
	if(m_bInvulnerable)
	{
		if(m_bInverted)
			SetBeingHit(9);
		else
			SetBeingHit(2);

		m_fInvulTime += fDelta;

		if(m_fInvulTime > PLAYER_HIT_CAP)
		{
			m_bInvulnerable = false;
			SetBeingHit(0);
			m_fInvulTime = 0.0f;
		}
	}

	m_pFrustum = CGameplayState::GetInstance()->GetCamera()->GetPlayerFrustum();

	if(!GetAlive())
		SetIsActive(false);

	KillCurrFrame();

	CInput* pIn = CInput::GetInstance();
	int nInputFlags = CInput::GetInstance()->GetInput(4);
	D3DXMATRIX d3dTranslate, d3dPlayerMat = GetMatrix();

	m_fFireTime += fDelta;

	if( pIn->CheckInput(DASH, m_nPlayerNum) && !m_bDashing )
	{
		if( m_fDashEnergy > 0 )
		{
			m_bDashing = true;
			m_fDashEnergy -= PLAYER_DASH_COST * fDelta;
			CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_DASH,m_d3dBottom);
		}
	}
	else
	{
		m_fDashEnergy += fDelta * PLAYER_DASH_REGEN;
		if( m_fDashEnergy > PLAYER_MAX_ENERGY )
			m_fDashEnergy = PLAYER_MAX_ENERGY;
	}

	if( pIn->CheckInput(SPECIAL, m_nPlayerNum) && !m_bDashing )
	{
		if( m_bSpecial )
		{
			m_bInvulnerable = true;
			m_bSpecial = false;
			CExplodingBullet* pSPBullet = nullptr;
			if( m_pOF->Create(OBJ_BULLET_EXP, (IBaseObject**)&pSPBullet) )
			{
				pSPBullet->SetMatrix(GetMatrix());
				pSPBullet->SetType(SPECIALBULLET);
				pSPBullet->SetDamage(DMG_SPECIAL);
				pSPBullet->SetMaxRadius(PLAYER_SP_MAXRADIUS);
				pSPBullet->SetSpeed(PLAYER_SP_SPEED);
				
				pSPBullet->SetRadius(0.0f);
				pSPBullet->SetColor(GetColorPlayer());

				// sound for using special
				WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_SPECIAL);
			}			
		}
		
	}

	D3DXVECTOR3 d3dPrev = GetVelocity();

	if( d3dPrev.x || d3dPrev.z )
		m_d3dDashDir = d3dPrev;

	if(m_bInverted)
		m_fInvertTimer -= fDelta;
	

	if(m_fInvertTimer <= 0.0f)
	{
		m_bInverted = false;
		m_fInvertTimer = 0.0f;
	}

	if(pIn->CheckInput(LEFT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,0.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,0.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,0.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,0.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if(pIn->CheckInput(RIGHT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,0.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,0.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,0.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,0.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(UP_LEFT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(UP_RIGHT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(DOWN_LEFT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(DOWN_RIGHT, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(-1.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(-1.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(1.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(1.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(DOWN, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(0.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(0.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(0.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(0.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else if( pIn->CheckInput(UP, m_nPlayerNum))
	{
		if(m_bInverted)
		{
			SetVelocity(D3DXVECTOR3(0.0f,0.0f,-1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(0.0f,0.0f,-1.0f);
			m_d3dDashDir = GetVelocity();
		}
		else
		{
			SetVelocity(D3DXVECTOR3(0.0f,0.0f,1.0f));
			m_d3dBottomTarget = D3DXVECTOR3(0.0f,0.0f,1.0f);
			m_d3dDashDir = GetVelocity();
		}
	}
	else
		SetVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));

	float speed = PLAYER_SPEED;
	if(m_bDashing)
	{
		speed = PLAYER_DASH_SPEED;
		m_bDashing = false;
	}

	if( GetVelocity().x || GetVelocity().z )
	{
			m_fScrollTime += fDelta * speed;
			D3DXVECTOR3 d3dVel = m_d3dDashDir * speed * fDelta;
			D3DXMatrixTranslation(&d3dTranslate, d3dVel.x, d3dVel.y, d3dVel.z);
			d3dPlayerMat *= d3dTranslate;
			SetMatrix(d3dPlayerMat);
			m_d3dBottom *= d3dTranslate;
	}

	TurnTo(m_d3dBottom, m_d3dBottomTarget, fDelta);

	
	D3DXVECTOR3 offset = ConfineToFrustum(m_pFrustum,GetSphere());
	if(offset.x != 0.0f || offset.z != 0.0f)
	{			
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans,offset.x,offset.y,offset.z);
		d3dPlayerMat *= trans;
	}

	SetMatrix(d3dPlayerMat);
	bool moveMouse = false;
	float tempX, tempY;
	tempX = pIn->GetMouseX();
	tempY = pIn->GetMouseY();

	if(tempX != m_fMouseX || tempY != m_fMouseY)
	{
		m_fMouseX = tempX;
		m_fMouseY = tempY;
		moveMouse = true;
	}

	D3DXVECTOR3 d3d360;
	if( pIn->GetShootDir(m_nPlayerNum, GetMatrix(), d3d360) )
	{
		m_d3dTarget = d3d360;
		CreateBullet(m_nGunID, fDelta); 
	}
	else if(moveMouse && pIn->GetIsKeyboard(m_nPlayerNum))
	{
		m_d3dTarget = d3d360;
	}
	else if(pIn->CheckInput(SHOOT_DOWN_LEFT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_DOWN_RIGHT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_DOWN, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_UP_LEFT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_UP_RIGHT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_UP, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_LEFT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(pIn->CheckInput(SHOOT_RIGHT, m_nPlayerNum))
	{
		m_d3dTarget = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		CreateBullet(this->m_nGunID, fDelta);
	}
	else if(m_pTeslaEffect)
	{
		CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
		m_pTeslaEffect = nullptr;
	}
	d3dPlayerMat = GetMatrix();

	TurnTo(d3dPlayerMat, m_d3dTarget, fDelta);
	SetMatrix(d3dPlayerMat);
	m_d3dGunMatrix = m_d3dGunOffset * GetMatrix();

	// gun changing logic
	if(m_fGunDuration > 0.0f)
	{
		m_fGunDuration -= fDelta;
		if(m_fGunDuration <= 0.0f)
		{
			m_nGunID = STANDARD;
			m_fMaxTime = STANDARD_WEAPON_SPEED;
		}
	}
	if(m_pTeslaEffect)
		m_pTeslaEffect->SetMatrix(m_d3dGunMatrix);

	// debug stuff
	if(GetAsyncKeyState('1') & 0x01)
	{
		this->SetHealth(100000);
		cout << "INFINITE HP!" << endl;
	}
	if(GetAsyncKeyState('2') & 0x01)
	{
		m_bSpecial = true;
		cout << "SPECIAL RETURN!" << endl;
	}
	if(GetAsyncKeyState('3') & 0x01)
	{
		if(m_fGunDuration <= 10.0f)
		{
			m_fGunDuration = 10000.0f;
			cout << "INFINITE GUN ON!" << endl;
		}
		else
		{
			cout << "INFINITE GUN OFF!" << endl;
			m_fGunDuration = 0.1f;
		}
	}
	if(GetAsyncKeyState('4') & 0x01)
	{
		m_nGunID = GATLING;
		m_fMaxTime = GATLING_WEAPON_SPEED;
		cout << "GATLING!" << endl;
	}
	if(GetAsyncKeyState('5') & 0x01)
	{
		m_nGunID = BEAM;
		m_fMaxTime = BEAM_WEAPON_SPEED;
		cout << "BEAM!" << endl;
	}
	if(GetAsyncKeyState('6') & 0x01)
	{
		m_nGunID = TESLA;
		m_fMaxTime = TESLA_WEAPON_SPEED;
		cout << "TESLA!" << endl;
	}
}

bool CPlayerObject::CheckCollision(IBaseObject* pObj)
{
	int ntype = pObj->GetID();

	if(!pObj->GetAlive() || pObj == this)
		return false;
	if(ntype == OBJ_PIT)
	{
		// if the trap is open ignore collision
		if(((CDeathPit*)pObj)->GetTrapIsOpen() == false)
			return false;

		Sphere tempSphere = GetSphere();
		tempSphere.m_Radius = 0.1f;
		if(SphereToAABB(tempSphere,((CDeathPit*)pObj)->GetAABB()))
		{
			pObj->CollisionResponse(this);
			return true;
		}
	}
	if(ntype == OBJ_CONVEYOR) 
	{
		if(SphereToAABB(GetSphere(), ((CConveyor*)pObj)->GetAABB()))
		{
			pObj->CollisionResponse(this);
			return true;
		}
	}
	if(ntype == OBJ_PLAYER || ntype == OBJ_PICKUP || ntype >= OBJ_VAT && ntype <= OBJ_BARREL
		|| ntype == OBJ_INVERSION || ntype == OBJ_BOSSINV || ntype == OBJ_TUNNEL || ntype == OBJ_BOSSTURRET)
	{
		if(ntype == OBJ_PICKUP && m_fGunDuration > WEAPON_PICKUP_DELAY)
			return false;

		if( ntype == OBJ_BOSSTURRET )
		{
			return pObj->CheckCollision(this);
		}

		if(ntype == OBJ_TUNNEL)
		{
			if(SphereToSphere(GetSphere(), pObj->GetSphere()))
			{
				CollisionResponse(pObj);
				return true;
			}
			else
				return false;
		}


		if(SphereToSphere(GetSphere(), pObj->GetSphere()))
		{
			AddToReactVector(pObj);
			//CollisionResponse(pObj);
			return true;
		}
	}
	return false;
}

void CPlayerObject::CollisionResponse(IBaseObject* pObj)
{
	int ntype = pObj->GetID();

	if(ntype == OBJ_PLAYER || ntype >= OBJ_VAT && ntype <= OBJ_BARREL || ntype == OBJ_INVERSION || ntype == OBJ_TUNNEL || ntype == OBJ_BOSSTURRET || ntype == OBJ_BOSSINV)
	{		
		Sphere tempSphere;
		tempSphere = pObj->GetSphere();
		D3DXVECTOR3 V = tempSphere.m_Center - this->GetSphere().m_Center;
		float Mag = D3DXVec3Length(&V);
		float Distance = tempSphere.m_Radius + GetSphere().m_Radius;
		float X = Distance/Mag + EPISILON;
		V = V * X;
		D3DXMATRIX tempMat = pObj->GetMatrix();
		if(ntype == OBJ_TUNNEL)
			tempMat._43 *= 0.85f;
		D3DXMATRIX tempSetMat = GetMatrix();
		tempSetMat._41 = tempMat._41 - V.x;
		tempSetMat._43 = tempMat._43 - V.z;
		this->SetMatrix(tempSetMat);

		if(ntype == OBJ_PLAYER)
		{
			Sphere tempSphere = pObj->GetSphere();
			D3DXVECTOR3 V = GetSphere().m_Center - tempSphere.m_Center;
			float fMagnitude = D3DXVec3Length(&V);
			float fRatio = fMagnitude/(GetSphere().m_Radius + tempSphere.m_Radius);
			D3DXMATRIX tempMat = pObj->GetMatrix();
			*(D3DXVECTOR3*)&tempMat[12] -= (0.25f*(fRatio*D3DXVECTOR3(V.x, 0.0f, V.z))); 
			pObj->SetMatrix(tempMat);
		}


		if( m_bDashing )
			m_bDashing = false;
	}

	if(ntype == OBJ_PICKUP)
	{
		m_fPickupEffectTime = 1.0f;
		m_fGunDuration = WEAPON_DURATION;
		m_nGunID = ((CPickup*)pObj)->GetGunType();
		CGame::GetInstance()->GetOF()->Destroy(pObj);
		
		if(m_nGunID == GATLING)
			m_fMaxTime = GATLING_WEAPON_SPEED;
		if(m_nGunID == BEAM)
			m_fMaxTime = BEAM_WEAPON_SPEED;
		if( m_nGunID == TESLA)
			m_fMaxTime = TESLA_WEAPON_SPEED;

		//sound of gettin a pickup
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_GETPICKUP);
	}
}

void CPlayerObject::TakeDamage(IBaseObject* _pIn)
{
	if( m_bDying || (m_bInvulnerable && (_pIn->GetID() != OBJ_POOL && _pIn->GetID() != OBJ_SPINBOT)))
		return;

	float fDamage = 0.0f;

	switch( _pIn->GetID() )
	{
	case OBJ_TRASH:
		{
			fDamage = ((CTrashBot*)_pIn)->GetDamage();
		}
		break;
	case OBJ_BULLET_ENEMY:
		{
			fDamage = ((CBullet*)_pIn)->GetDamage();
		}
		break;
	case OBJ_BULLET_EXP:
		{
			if( ((CExplodingBullet*)_pIn)->GetType() == SMASHBULLET )
				fDamage = ((CExplodingBullet*)_pIn)->GetDamage();
			if(((CExplodingBullet*)_pIn)->GetType() == BARRELBULLET)
				fDamage = ((CExplodingBullet*)_pIn)->GetDamage();
		}
		break;
	case OBJ_POOL:
		{
			fDamage = ((CMoltenPool*)_pIn)->GetDamage();
		}
		break;
	case OBJ_STEAM:
		{
			fDamage = ((CSteamrollerBot*)_pIn)->GetDamage();
		}
		break;
	
	case OBJ_SPINBOT:
		{
			fDamage = ((CSpinBot*)_pIn)->GetDamage();
		}
		break;
	case OBJ_BOSSTURRET:
	case OBJ_TURRET:
		{
			fDamage = ((CTurret*)_pIn)->GetDamage();
		}
		break;
	}

	if(fDamage != 0.0f && _pIn->GetID() != OBJ_POOL)
		CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_PLAYERDMG,GetMatrix());

	SetHealth(GetHealth() - fDamage );
	if(GetHealth() > 0.0f)
	{
		m_bInvulnerable = true;
		WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_HURT);
	}
	else
		SetupDying();
}

void CPlayerObject::SetupDying()
{
	m_bDying  = true;
	ChangeAnimation(APLAYER_DEATH);
	m_fDyingTime = GetCurrentAnimation()->m_fDuration;
	WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_DEATH);
	CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_PLAYERDEATH,GetMatrix());

	if(m_pTeslaEffect)
	{
		CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
		m_pTeslaEffect = nullptr;
	}
}

void CPlayerObject::CreateBullet(int nGunID, float fElapsed)
{
	if(m_fFireTime < m_fMaxTime)
		return;
	
	D3DXVECTOR3 d3dShootDir;
	CInput::GetInstance()->GetShootDir(m_nPlayerNum, m_d3dGunMatrix, d3dShootDir);

	switch(nGunID)
	{
	case STANDARD:
		{
			m_fFireTime = 0.0f;
			CBullet* pBullet = nullptr;
			m_pOF->Create(OBJ_BULLET_PLAYER, (IBaseObject**)&pBullet);
			pBullet->SetVelocity(d3dShootDir);
			pBullet->SetMatrix(m_d3dGunMatrix);
			pBullet->SetType(STANDARD);
			pBullet->SetDamage(DMG_STDWEAPON);
			pBullet->SetColor(GetColor());
			
			// sounds
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_BULLET);

			if(m_pTeslaEffect)
			{
				CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
				m_pTeslaEffect = nullptr;
			}
		}
		break;
	case GATLING:
		{
			m_fFireTime = 0.0f;
			CBullet* pBullet = nullptr;
			m_pOF->Create(OBJ_BULLET_PLAYER,(IBaseObject**)&pBullet);
			D3DXVECTOR3 temp = d3dShootDir;
			D3DXVec3Normalize(&temp, &d3dShootDir);
			D3DXMATRIX objMat;
			D3DXMatrixIdentity(&objMat);
			*(D3DXVECTOR3*)&objMat[8] = temp;
			D3DXVECTOR3 newX, xNrm;
			D3DXVec3Cross(&newX, (D3DXVECTOR3*)&objMat[8], (D3DXVECTOR3*)&objMat[4]);
			temp = newX;
			D3DXVec3Normalize(&newX, &temp);
			*(D3DXVECTOR3*)&objMat[0] = newX;
			D3DXMATRIX shotRot;
			float turning = ((rand() % 3001) - 1500) * 0.01f;
			D3DXMatrixRotationY(&shotRot,D3DXToRadian(turning));
			objMat *= shotRot;
			pBullet->SetVelocity(*(D3DXVECTOR3*)&objMat[8]);

			pBullet->SetMatrix(m_d3dGunMatrix);
			pBullet->SetType(GATLING);
			pBullet->SetDamage(DMG_GATGUN);
			pBullet->SetColor(GetColor());

			// pushback player
			D3DXVECTOR3 shootNorm = pBullet->GetVelocity();
			shootNorm *= (-1.0f * fElapsed * GATLING_PUSHBACK);
			objMat = GetMatrix();
			D3DXMatrixTranslation(&m_d3dGunMatrix, shootNorm.x, 0.0f, shootNorm.z);
			objMat *= m_d3dGunMatrix;
			SetMatrix(objMat);

			// sounds
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PLAYER_BULLET);

			if(m_pTeslaEffect)
			{
				CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
				m_pTeslaEffect = nullptr;
			}
		}
		break;
	case BEAM:
		{
			m_fFireTime = 0.0f;
			CBeamBullet* pBeamBullet = nullptr;
			if(m_pOF->Create(OBJ_BULLET_BEAM,(IBaseObject**)&pBeamBullet))
			{
				pBeamBullet->SetVelocity(d3dShootDir);
				{
					D3DXVECTOR3 cross, xAxis, up(0, 1.0f, 0);
					D3DXVec3Cross(&cross, &up, &d3dShootDir);
					D3DXVec3Normalize(&xAxis, &cross);
					D3DXMATRIX rotBeam = m_d3dGunMatrix;
					*(D3DXVECTOR3*)&rotBeam[0] = xAxis;
					*(D3DXVECTOR3*)&rotBeam[4] = up;
					*(D3DXVECTOR3*)&rotBeam[8] = d3dShootDir;
					pBeamBullet->SetMatrix(rotBeam);
				}

				pBeamBullet->SetType(BEAM);
				pBeamBullet->SetIsActive(true);
				pBeamBullet->SetAlive(true);
				pBeamBullet->SetLifeTime(BEAM_WEAPON_LIFE);
				pBeamBullet->SetColor(GetColor());
				pBeamBullet->CreateCapsule();
			}
			WwiseNS::PlaySFX(WwiseNS::EVENT_SOUND_PULSELASER);
			if(m_pTeslaEffect)
			{
				CGameplayState::GetInstance()->GetFX()->ReleaseEffect(m_pTeslaEffect);
				m_pTeslaEffect = nullptr;
			}
		}
		break;
	case TESLA:
		{
			m_fFireTime = 0.0f;
			CTeslaBullet* pTesla = nullptr;

			if( m_pOF->Create(OBJ_BULLET_TESLA, (IBaseObject**)&pTesla) )
			{
				pTesla->SetDamage(DMG_TESLA);
				pTesla->SetIsActive(true);
				pTesla->SetAlive(true);
				pTesla->SetOwner(this);
				pTesla->SetColor(GetColor());
				if(m_nGunID == TESLA && !m_pTeslaEffect)
				{
					m_pTeslaEffect = CGameplayState::GetInstance()->GetFX()->CreateEffect(EFFECT_TESLASPRAY, m_d3dGunMatrix);
					if(m_pTeslaEffect)
						m_pTeslaEffect->SetColors((D3DXCOLOR)GetColorPlayer());
				}
			}
		}
		break;
	};
}

void CPlayerObject::Render()
{
	if(!GetAlive())
		return;

	if(m_fPickupEffectTime > 0)
		RenderPickupEffect();

	float randomNum = (((rand()/(float)RAND_MAX)*((1.0f)-(0.0f)))+(0.0f));
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	CRenderer* pRenderer = CRenderer::GetInstance();
	IDirect3DDevice9* D3D_Device = pRenderer->GetDirect3DDevice();

	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	D3D_Device->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(GetID());
	IDirect3DTexture9* tempTexture;
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetMatrixArray("gBones", PassCurrFrame(), GetCurrentAnimation()->m_nNumMeshes);
		tempTexture = pAM->LoadTexture("Assets/Textures/Player_bodyBottom_Diffuse.png");
		switch(GetPlayerColorNum())
		{
		case GREEN:
			{
				tempTexture = pAM->LoadTexture("Assets/Textures/Player_Body_Green.png");
			}
			break;
		case RED:
			{
				tempTexture = pAM->LoadTexture("Assets/Textures/Player_Body_Red.png");
			}
			break;
		case YELLOW:
			{
				tempTexture = pAM->LoadTexture("Assets/Textures/Player_Body_Yellow.png");
			}
			break;
		}
		pShader->SetTexture("gDiffuseTexture", tempTexture);
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&(GetMatrix()));
		pShader->SetInt("AmIHit",(GetBeingHit()));
		pShader->SetFloat("randomNum",randomNum);

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(GetID()), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(GetID()));
		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(GetID()), 0, pAM->GetNumTriangles(GetID()));

		pShader->EndPass();
	}
	pShader->End();

	D3DXMATRIX transMatrix;

	int headID = OBJ_PLAYERHEAD_BLUE;
	switch(GetPlayerColorNum())
	{
	case GREEN:
		{
			headID = OBJ_PLAYERHEAD_GREEN;
		}
		break;
	case RED:
		{
		}
		break;
	case YELLOW:
		{
		}
		break;
	}

	pShader = pAM->GetShader(headID);
	passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);
		pShader->SetMatrixArray("gBones", &PassCurrFrame()[7], 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(headID));
		pShader->SetMatrix("gWorld",&(GetMatrix()));
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(headID), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(headID));
		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(headID), 0, pAM->GetNumTriangles(headID));

		pShader->EndPass();
	}
	pShader->End();

	if(m_fDancing < 0 && !m_bDying && !m_bIsFalling)
	{
		int nGunType = OBJ_DEFENSELASER;
		switch(m_nGunID)
		{
		case GATLING:
			{
				nGunType = OBJ_GATLING;
			}
			break;
		case TESLA:
			{
				nGunType = OBJ_TESLA;
			}
			break;
		case BEAM:
			{
				nGunType = OBJ_BEAM;
			}
			break;
		}

		if(!m_bIsFalling && !m_bDeath)
		{
			transMatrix = m_d3dGunMatrix;
			transMatrix[13] = 1.082f;
		}
		pShader = pAM->GetShader(nGunType);
		passes=0;
		pShader->Begin(&passes,0);
		for(unsigned i(0);i < passes; i++)
		{
			pShader->BeginPass(i);

			pShader->SetMatrix("gWorld",&transMatrix);
			pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(nGunType));
			D3DXMatrixIdentity(&transMatrix);
			pShader->SetMatrixArray("gBones", &transMatrix, 1);
			pShader->CommitChanges();

			D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(nGunType), 0, sizeof(tVertex));
			D3D_Device->SetIndices(pAM->GetIndexBuffer(nGunType));

			D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(nGunType), 0, pAM->GetNumTriangles(nGunType));

			pShader->EndPass();
		}
		pShader->End();
	}

	pShader = pAM->GetShader(OBJ_PLAYERTREAD);

	passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		D3DXMatrixIdentity(&transMatrix);
		pShader->SetMatrixArray("gBones", &transMatrix, 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(OBJ_PLAYERTREAD));
		pShader->SetMatrix("gViewProjection", &ViewProj);
		pShader->SetMatrix("gWorld",&(GetBottomMatrix()));
		pShader->SetFloat("gTime", GetScrollTime());
		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		D3D_Device->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));
		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(OBJ_PLAYERTREAD), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(OBJ_PLAYERTREAD));

		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_PLAYERTREAD), 0, pAM->GetNumTriangles(OBJ_PLAYERTREAD));

		pShader->EndPass();
	}
	pShader->End();

	pShader = pAM->GetShader(OBJ_PLAYERBOTTOM);
	passes = 0;
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);
		pShader->SetMatrixArray("gBones", &pRenderer->KillAnimation(), 15);
		pShader->SetMatrix("gWorld", &GetBottomMatrix());
		pShader->SetTexture("gDiffuseTexture", tempTexture);
		pShader->CommitChanges();

		D3D_Device->SetStreamSource(0, pAM->GetVertBuffer(OBJ_PLAYERBOTTOM), 0, sizeof(tVertex));
		D3D_Device->SetIndices(pAM->GetIndexBuffer(OBJ_PLAYERBOTTOM));
		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(OBJ_PLAYERBOTTOM), 0, pAM->GetNumTriangles(OBJ_PLAYERBOTTOM));

		pShader->EndPass();
	}
	pShader->End();
	
	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

	if(!m_bDying && !m_bIsFalling)
		pRenderer->RenderShadow(this);

	if(m_bInvulnerable)
		pRenderer->RenderPlayerRing(this);
}

void CPlayerObject::CheckTutorial(bool bPassed)
{
	if(!bPassed && !m_bInvulnerable && !m_bDying)
	{
		SetupDying();
	}
	else if(bPassed && !m_bDying)
	{
		CIntroState::GetInstance()->SetTutorial(false);
	}
}

void CPlayerObject::RenderPickupEffect()
{
	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	IDirect3DDevice9* pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	pD3D->SetVertexDeclaration(pAM->GetVertexDeclaration(BASEOBJECTDECL));

	D3DXMATRIX iden, trans, result;
	D3DXMatrixIdentity(&iden);
	int gunID = OBJ_DEFENSELASER;
	D3DXMatrixTranslation(&trans, 0.0f, 2.0f, 0);
	switch(m_nGunID)
	{
	case GATLING:
		{
			gunID = OBJ_GATLING;
		}
		break;
	case BEAM:
		{
			gunID = OBJ_BEAM;
		}
		break;
	case TESLA:
		{
			gunID = OBJ_TESLA;
		}
		break;
	}
	D3DXMATRIX rot, tilt, scale;
	D3DXMatrixScaling(&scale, 1.5f, 1.5f, 1.5f);
	D3DXMatrixRotationY(&rot, m_fPickupEffectTime * 4.0f);
	D3DXMatrixRotationX(&tilt, D3DXToRadian(-40.0f));
	result = rot*scale*trans;
	result = result * GetMatrix();
	result = tilt*result;
	result[14] += 1.5f;

	ID3DXEffect* pShader = pAM->GetShader(gunID);
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetMatrixArray("gBones", &iden, 1);
		pShader->SetTexture("gDiffuseTexture", pAM->GetTexture(gunID));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&result);
		pShader->SetInt("AmIHit", 1);

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		pD3D->SetStreamSource(0, pAM->GetVertBuffer(gunID), 0, sizeof(tVertex));
		pD3D->SetIndices(pAM->GetIndexBuffer(gunID));
		pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pAM->GetNumVerts(gunID), 0, pAM->GetNumTriangles(gunID));

		pShader->EndPass();
	}
	pShader->End();
}

void CPlayerObject::Dance()
{
	ChangeAnimation(APLAYER_DANCE);
	m_fDancing = GetCurrentAnimation()->m_fDuration;
}
