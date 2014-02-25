/************************************************************************************
 * Filename:  		HUD.h
 * Date:      		02/11/2013
 * Mod. Date: 		02/13/2013
 * Mod. Initials:	TS
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the heads up display
 ************************************************************************************/
#ifndef _CHUD_H_
#define _CHUD_H_

#include "PlayerObject.h"
#include "Renderer.h"

enum ePortraitColor {hudBLUE = 0, hudRED, hudGREEN, hudYELLOW};

class CHUD
{
	struct THUDVertex
	{
		D3DXVECTOR3 d3dPosition;
		D3DXVECTOR2 d3dUV;
	};

	float m_fDefaultRatio;
	float m_fBossFade;

	IDirect3DVertexBuffer9* d3dHealthBuff;
	IDirect3DVertexBuffer9* d3dGunDurationBuff;
	IDirect3DVertexBuffer9* d3dBossHealthBuff;
	IDirect3DVertexBuffer9* d3dDashBuff;
	IDirect3DVertexBuffer9* d3dLivesBuff;
	IDirect3DVertexBuffer9* d3dSpecialBuff;
	IDirect3DVertexBuffer9* d3dPortraitBuff;
	IDirect3DVertexBuffer9* d3dReticleBuff;
	IDirect3DVertexBuffer9* d3dBlackBuff;
	IDirect3DVertexBuffer9* d3dEmptyBuff;
	ID3DXEffect* d3dHudShader;

	IDirect3DTexture9* d3dHealthTexture;
	IDirect3DTexture9* d3dHealthBackTexture;
	IDirect3DTexture9* d3dBossTexture;
	IDirect3DTexture9* d3dBossBackTexture;
	IDirect3DTexture9* d3dDashTexture;
	IDirect3DTexture9* d3dLifeTexture;
	IDirect3DTexture9* d3dLifeBackTexture;
	IDirect3DTexture9* d3dSpecialTextureOn;
	IDirect3DTexture9* d3dSpecialTextureOff;
	IDirect3DTexture9* d3dGunDurationTexture[PU_NUM_PICKUPS];

	IDirect3DTexture9* d3dPortraitTexture[4];

	IDirect3DTexture9* d3dReticleTexture;
	IDirect3DTexture9* d3dReticleBackTexture;
	IDirect3DTexture9* d3dBackTexture;
	IDirect3DTexture9* d3dEmptyTexture;
	IDirect3DDevice9* pD3D;
	D3DXMATRIX d3dOrthoMatrix;

	std::vector<CPlayerObject*>* m_pPlayers;
	D3DXMATRIX m_d3dOffset[4];
	D3DXMATRIX m_d3dBossOffset;
	IBaseObject* m_pBoss;
	unsigned int m_unNumPlayers;
	bool m_bBossEnabled;
	float m_fMaxBossHP;
public:
	CHUD(void);
	~CHUD(void);

	/*****************************************************************
	 * Render():			Renders the HUD
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void Render(void);

	/*****************************************************************
	 * Init:				Initializes the HUD to setup for how many players
	 *						there are.
	 *
	 * Ins:					vector<CPlayerObject*>* players
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Init(std::vector<CPlayerObject*>* pPlayers);

	/*****************************************************************
	 * EnableBoss:			Marks the boss as ready to fight
	 *
	 * Ins:					IBaseObject* BaseObj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void EnableBoss(IBaseObject* pBaseObj);
	void DisableBoss();
};
#endif

