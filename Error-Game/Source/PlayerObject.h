/************************************************************************************
 * Filename:  		PlayerObject.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/23/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This acts as the player object.
 ************************************************************************************/
#ifndef _CPLAYEROBJECT_H_
#define _CPLAYEROBJECT_H_
#include "MovingObject.h"
#include "Effect.h"

class CObjectFactory;
class CGameplayState;

enum ePLAYERSELECTCOLOR {BLUE = 0, RED, GREEN, YELLOW};

class CPlayerObject : public CMovingObject
{
	float m_fDashEnergy;
	float m_fMaxDashEnergy;
	int m_nLives;
	bool m_bSpecial;
	unsigned int m_nPlayerNum;
	float m_fFireTime;
	float m_fMaxTime;
	float m_fScrollTime;

	CObjectFactory* m_pOF;
	Frustum	m_pFrustum;

	bool m_bDashing;
	float m_fDashTime;
	D3DXVECTOR3 m_d3dDashDir;

	float m_fDyingTime;
	float m_fFadingTime;
	bool m_bDying;
	bool m_bPitDeath;

	bool m_bInvulnerable;
	float m_fInvulTime;
	float m_fDeathInvulTime;
	bool m_bDeath;
	bool m_bPlayerPush;
	bool m_bTransition;

	//Inversion stuff
	bool m_bInverted;
	float m_fInvertTimer;

	D3DXVECTOR3 m_d3dTarget;
	D3DXVECTOR3 m_d3dBottomTarget;
	D3DXMATRIX m_d3dBottom;
	
	Capsule m_cBeamCapsule;

	D3DXVECTOR4 m_d3dPlayerColor;
	int			m_nPlayerColorNum;
	IDirect3DTexture9* m_d3dRingTexture;

	// Bullet and gun members
	int			m_nGunID;
	float		m_fGunDuration;
	CEffect*	m_pTeslaEffect;
	D3DXMATRIX	m_d3dGunMatrix;
	D3DXMATRIX	m_d3dGunOffset;

	void		CreateBullet(int nGunID, float fElapsed);

	float m_fTimer;
	float m_fMouseX;
	float m_fMouseY;
	float m_fFallingTimer;
	bool m_bIsFalling;
	D3DXMATRIX m_d3dFallingMatrix;

	float m_fDancing;

	float m_fPickupEffectTime;

	/*****************************************************************
	 * KillPlayer():		Runs Player's reset code to ready player for
	 *						respawn
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/26/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void KillPlayer();

	/*****************************************************************
	 * RenderPickupEffect(): Renders an image of the equipped weapon when
	 *						a weapon is picked up
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/26/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void RenderPickupEffect();

public:
	CPlayerObject(void);
	virtual ~CPlayerObject(void);

	/***************
	 *  Accessors
	 ***************/
	D3DXVECTOR4 GetColorPlayer() {return m_d3dPlayerColor;}
	IDirect3DTexture9* GetRingTexture() {return m_d3dRingTexture;}
	float GetDashEnergy(){return m_fDashEnergy;}
	float GetMaxDashEnergy(){return m_fMaxDashEnergy;}
	int GetLives(){return m_nLives;}
	bool GetSpecial(){return m_bSpecial;}
	int GetGunID() {return m_nGunID;}
	unsigned int GetPlayerNum() const { return m_nPlayerNum; }
	float GetScrollTime() {return m_fScrollTime;}
	bool GetInvulernable() {return m_bInvulnerable;}
	D3DXMATRIX GetBottomMatrix() {return m_d3dBottom;}
	Capsule GetCapsule()	{ return m_cBeamCapsule;}
	float GetInvulnerableTime() const { return m_fInvulTime; }
	int GetPlayerColorNum() const {return m_nPlayerColorNum; }
	float GetGunDuration() const {return m_fGunDuration;}
	int GetGunID() const {return m_nGunID;}
	bool GetInverted() const {return m_bInverted;}
	float GetInvertTimer() const {return m_fInvertTimer;}
	float GetInvulDeathTimer() const {return m_fDeathInvulTime;}
	float GetMaxTime() const {return m_fMaxTime;}
	float GetFallingTimer() const {return m_fFallingTimer;}
	bool GetIsFalling() const {return m_bIsFalling;}
	D3DXMATRIX GetGunMatrix() const {return m_d3dGunMatrix;}
	bool GetDying() const {return m_bDying;}
	bool GetTransition() const {return m_bTransition;}

	/***************
	 *  Mutators
	 ***************/
	void SetGunID(int nGunID) { m_nGunID = nGunID;}
	void SetLives( int nL ) { m_nLives = nL; }
	void SetWeaponID(int nW) { m_nGunID = nW; }
	void SetColorPlayer(D3DXVECTOR4 d3dColor);
	void SetSpecial(bool bS) { m_bSpecial = bS; }
	void SetInvulTime(float fT) { m_fInvulTime = fT; }
	void SetInvulnerable(bool bI) { m_bInvulnerable = bI; }
	void SetInverted(bool bInverted) {m_bInverted = bInverted;}
	void SetInvertTimer(float fInvertTimer) {m_fInvertTimer = fInvertTimer;}
	void SetDeathInvulTimer(float fDeathTimer) {m_fDeathInvulTime = fDeathTimer;}
	void SetFallingTimer(float fTimer) { m_fFallingTimer = fTimer;}
	void SetIsFalling(bool bIsfalling) { m_bIsFalling = bIsfalling;}
	void SetTransition(bool bIsTrans) {m_bTransition = bIsTrans;}

	/*****************************************************************
	 * SetupDying():			Sets up the player die on command
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/26/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void SetupDying();

	/*****************************************************************
	 * Dance():			Makes the Player Dance
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/26/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void Dance();

	/*****************************************************************
	 * Initialize():		Sets up any player variables
	 *
	 * Ins:					unsigned int _nPlayerNum, int _nPlayerColor
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	void Initialize(unsigned int nPlayerNum, int nPlayerColor, bool bCheckPoint = false);

	/*****************************************************************
	 * Initialize():		Sets player to default start position
	 *
	 * Ins:					unsigned int _nPlayerNum
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/09/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void ResetPosition(unsigned int nPlayerNum);

	/*****************************************************************
	 * Update():			Modifies the objects variables based on
	 *						time.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Update(float fDelta);
	/*****************************************************************
	 * CheckCollision():	Modifies the objects variables based on
	 *						time.
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	bool CheckCollision(IBaseObject* pObj);

	/*****************************************************************
	 * CollisionResponse():	Responds to Collision depending on type
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	 void CollisionResponse(IBaseObject* pObj);

	 /*****************************************************************
	 * TakeDamage():		Tells the IBaseObject to take damage
	 *
	 * Ins:					IBaseObject* _pIn
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/20/2013
	 * Mod. Initials:		JT
	 *****************************************************************/
	 void TakeDamage(IBaseObject* pIn);

	 /*****************************************************************
	 * Render():	Render Player, Scrolling Base, Gun, and Head
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	 virtual void Render();

	 void CheckTutorial(bool bPassed);
};

#endif