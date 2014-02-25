#ifndef _EFFECTDEFINITIONS_H_
#define _EFFECTDEFINITIONS_H_
#include <d3dx9math.h>

#define MAX_EFFECTS 150
#define MAX_PARTICLES 50
#define MAX_STAGES 4
#define MAX_EMITTERS 4
#define MAX_GEOMETRIES 4

enum eAnimations{ ANIM_WAVE };

struct TStage
{
	float fTime;
	D3DXVECTOR3 d3dVelocity;
	D3DXCOLOR d3dColor;
	D3DXVECTOR3 d3dScale;
	D3DXVECTOR3 d3dRotation;
};
struct TEmitterStruct
{
	float fSpawnRate;
	int nRandomnessP;
	int nRandomnessV;
	int nRandPos;
	int nRandVel;
	int nMeshID;
	float fStartTime;
	bool bLooping;
	float fLife;
	int nTextureID;
	TStage tStages[MAX_STAGES];
	D3DXMATRIX d3dPosition;
	D3DXVECTOR3 d3dOffset;
	D3DXVECTOR3 d3dEffectVelocity;
	DWORD source;
	DWORD dest;
};
struct TGeometryStruct
{
	bool bAnimated;
	int nAnimID;
	int nRandPos;
	int nRandVel;
	int nMeshID;
	float fStartTime;
	bool bLooping;
	float fLife;
	int nTextureID;
	TStage tStages[MAX_STAGES];
	D3DXMATRIX d3dPosition;
	D3DXVECTOR3 d3dOffset;
	D3DXVECTOR3 d3dEffectVelocity;
	DWORD source;
	DWORD dest;
};
struct TEffectDefault
{
	int nID;
	float fLifeTime;
	int nRandomPos;
	int nRandomVel;
	bool bLooping;
	D3DXMATRIX d3dPosition;
	D3DXVECTOR3 d3dVelocity;
	TEmitterStruct vEmitters[MAX_EMITTERS];
	TGeometryStruct vGeometries[MAX_GEOMETRIES];
};
struct TParticleStruct
{
	TStage tStartStage;
	TStage tStage;
	D3DXMATRIX d3dStartPos;
	int nRandomnessPos;
	int nRandomnessVel;
};
struct TPointVertex
{
	D3DXVECTOR3 d3dPosition;
	D3DXVECTOR2 d3dUV;
};
#endif