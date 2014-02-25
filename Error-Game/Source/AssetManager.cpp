/***********************************************
* Filename:  		CAssetManager.cpp
* Date:      		02/11/2013
* Mod. Date: 		02/13/2013
* Mod. Initials:	SD
* Author:    		Sean W. Davis
* Purpose:   		Responsible for loading and holding assets
************************************************/
#include "AssetManager.h"


#include "Renderer.h"
using std::pair;
#include <string>
using std::string;

CAssetManager::CAssetManager(void)
{
}

void CAssetManager::initialize()
{
	pD3D = CRenderer::GetInstance()->GetDirect3DDevice();

	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 32, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};

	IDirect3DVertexDeclaration9* vertDecl;
	pD3D->CreateVertexDeclaration(decl, &vertDecl);
	m_vVertexDeclarations.push_back(vertDecl);

	D3DVERTEXELEMENT9 decl2[] = 
	{ 
		{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{ 0,12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	pD3D->CreateVertexDeclaration(decl2, &vertDecl);
	m_vVertexDeclarations.push_back(vertDecl);

	D3DVERTEXELEMENT9 decl3[] = 
	{ 
		{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION, 0},
		{ 0,12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	pD3D->CreateVertexDeclaration(decl3, &vertDecl);
	m_vVertexDeclarations.push_back(vertDecl);

	D3DVERTEXELEMENT9 decl4[] = 
	{ 
		{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};

	pD3D->CreateVertexDeclaration(decl4, &vertDecl);
	m_vVertexDeclarations.push_back(vertDecl);

	IDirect3DTexture9* breakTexture;
	pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	D3DXCreateTextureFromFileA(pD3D, "TestFiles/Safety.png", &breakTexture);
	m_vTextures.push_back(breakTexture);

	LoadMesh("Assets/Meshes/PlayerBody.msh", OBJ_PLAYER);
	LoadMesh("Assets/Meshes/PlayerBottom.msh", OBJ_PLAYERBOTTOM);
	LoadMesh("Assets/Meshes/PlayerTreads.msh", OBJ_PLAYERTREAD);
	LoadMesh("Assets/Meshes/Player_HeadBlue.msh", OBJ_PLAYERHEAD_BLUE);
	LoadMesh("Assets/Meshes/Player_Head_Green.msh", OBJ_PLAYERHEAD_GREEN);
	LoadMesh("Assets/Meshes/Environment_Pickup.msh", OBJ_PICKUP);
	LoadMesh("Assets/Meshes/Enemy_Trash_Bot.msh", OBJ_TRASH);
	LoadMesh("Assets/Meshes/PatrolBot.msh", OBJ_PATROL);
	LoadMesh("Assets/Meshes/Smashbot.msh", OBJ_SMASH);
	LoadMesh("Assets/Meshes/Enemy_Burn_Bot.msh", OBJ_BURN);
	LoadMesh("Assets/Meshes/SteamrollerBot.msh", OBJ_STEAM);
	LoadMesh("Assets/Meshes/SteamrollerBot_Wheel.msh", OBJ_STEAM_WHEEL);
	LoadMesh("Assets/Meshes/AI_Core.msh", OBJ_AICORE);
	LoadMesh("Assets/Meshes/AI_Monitors.msh", OBJ_AI_MONITORS);
	LoadMesh("Assets/Meshes/AI_Arms.msh", OBJ_AI_ARMS);
	LoadMesh("Assets/Meshes/AI_Face.msh", OBJ_AI_HEAD);
	LoadMesh("TestFiles/Test_Fist.msh", OBJ_AIFIST);
	LoadMesh("Assets/Meshes/Environment_Vat.msh", OBJ_VAT);
	LoadMesh("TestFiles/Test_Crate.msh", OBJ_CRATE);
	LoadMesh("Assets/Meshes/Environment_Barrel.msh", OBJ_BARREL);
	LoadMesh("TestFiles/Test_Pool.msh", OBJ_POOL);
	LoadMesh("Assets/Meshes/Environment_Laser_Turret.msh",OBJ_TURRET);
	LoadMesh("Assets/Meshes/Environment_Laser_Turret.msh",OBJ_BOSSTURRET);
	LoadMesh("TestFiles/Test_SpiderBot.msh",OBJ_SPIDER);
	LoadMesh("Assets/Meshes/Environmental_Inversion_Tower.msh", OBJ_INVERSION);
	LoadMesh("Assets/Meshes/Environmental_Inversion_Tower.msh", OBJ_BOSSINV);
	LoadMesh("Assets/Meshes/Weapon_Defense_Laser.msh", OBJ_DEFENSELASER);
	LoadMesh("Assets/Meshes/Weapon_Gatling_Gun.msh", OBJ_GATLING);
	LoadMesh("Assets/Meshes/Weapon_Pulse_Laser.msh", OBJ_BEAM);
	LoadMesh("Assets/Meshes/Weapon_Tesla_Ray.msh", OBJ_TESLA);
	LoadMesh("Assets/Meshes/Tunnel.msh", OBJ_TUNNEL);
	LoadMesh("Assets/Meshes/DeathPit.msh", OBJ_PIT);
	LoadMesh("Assets/Meshes/Gate.msh", OBJ_GATE);
	LoadMesh("Assets/Meshes/SpinBot.msh", OBJ_SPINBOT);
	LoadMesh("Assets/Meshes/Midboss_Floor.msh", OBJ_MIDBOSSFLOOR);

	//EFFECT LOAD DATA
	LoadMesh("Assets/Meshes/Effect_Circle.msh", EFFECT_OBJ_CIRCLE);
	LoadMesh("Assets/Meshes/Effect_Cube.msh", EFFECT_OBJ_CUBE);
	LoadMesh("Assets/Meshes/CylinderMesh.msh", EFFECT_OBJ_CYLINDER);
	LoadMesh("Assets/Meshes/Effect_Disc.msh", EFFECT_OBJ_DISC);
	LoadMesh("Assets/Meshes/SphereMesh.msh", EFFECT_OBJ_SPHERE);
	LoadMesh("Assets/Meshes/Effect_Torus.msh", EFFECT_OBJ_TORUS);
	LoadMesh("Assets/Meshes/Effect_Smash_Bot_Shockwave.msh", EFFECT_OBJ_SHOCKWAVE);
	LoadMesh("TestFiles/Test_Wave.msh", EFFECT_OBJ_WAVE);
	LoadEffectAnimations();

	// BULLET MESHES
}

CAssetManager* CAssetManager::GetInstance() 
{ 
	static CAssetManager s_Instance;
	return &s_Instance;
}

CAssetManager::~CAssetManager(void)
{
	pD3D = nullptr;
}

void CAssetManager::LoadAnimation(char* szFilename, int nObjID)
{
	fstream istream;
	istream.open(szFilename, ios_base::in|ios_base::binary);
	if(istream.is_open())
	{
		TAnimation newAnimation;
		istream.read((char*)&newAnimation.m_fDuration, sizeof(newAnimation.m_fDuration));
		istream.read((char*)&newAnimation.m_nNumMeshes, sizeof(newAnimation.m_nNumMeshes));
		newAnimation.m_pBones = new TBone[newAnimation.m_nNumMeshes];

		for(unsigned int currBone = 0; currBone < newAnimation.m_nNumMeshes; ++currBone)
		{
			newAnimation.m_pBones[currBone].m_nCurrFrame = 0;
			istream.read((char*)&newAnimation.m_pBones[currBone].m_nNumFrames, sizeof(newAnimation.m_pBones[currBone].m_nNumFrames));
			newAnimation.m_pBones[currBone].m_pKeyFrames = new TKeyFrame[newAnimation.m_pBones[currBone].m_nNumFrames];
			for(unsigned int currFrame = 0; currFrame < newAnimation.m_pBones[currBone].m_nNumFrames; ++currFrame)
			{
				istream.read((char*)&newAnimation.m_pBones[currBone].m_pKeyFrames[currFrame].m_d3dTransform, sizeof(D3DXMATRIX));
				istream.read((char*)&newAnimation.m_pBones[currBone].m_pKeyFrames[currFrame].m_fDuration, sizeof(float));
			}
		}
		m_tObjectDataArray[nObjID].m_vAnimations.push_back(newAnimation);

		istream.close();
	}
}

IDirect3DTexture9* CAssetManager::LoadTexture(char* szFilename)
{
	map<char*, IDirect3DTexture9*>::iterator itr;
	itr = m_mLoadedTextures.find(szFilename);
	if(itr == m_mLoadedTextures.end())
	{
		IDirect3DTexture9* newTexture;
		pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
		D3DXCreateTextureFromFileA(pD3D, szFilename, &newTexture);
		if((int)newTexture == 0xcccccccc)
			return m_vTextures[1];
		m_vTextures.push_back(newTexture);
		m_mLoadedTextures.emplace( pair<char*, IDirect3DTexture9*>(szFilename, newTexture) );
		return newTexture;
	}

	return itr->second;
}

ID3DXEffect* CAssetManager::LoadShader(char* szFilename)
{
	map<char*, ID3DXEffect*>::iterator itr;
	itr = m_mLoadedShaders.find(szFilename);
	if(itr == m_mLoadedShaders.end())
	{
		ID3DXEffect* newShader;
		pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
		D3DXCreateEffectFromFileA(pD3D, szFilename, 0, 0, 0, 0, &newShader, 0);
		m_vShaders.push_back(newShader);
		m_mLoadedShaders.emplace( pair<char*, ID3DXEffect*>(szFilename, newShader) );
		return newShader;
	}

	return itr->second;
}

void CAssetManager::LoadMesh(char* szFilename, int nObjectID)
{
	switch(nObjectID)
	{
	case OBJ_PLAYER:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Player_bodyBottom_Diffuse.png");
			LoadAnimation("Assets/Animations/Player_Death_Body.ani", nObjectID);
			LoadAnimation("Assets/Animations/Player_Dance.ani", nObjectID);
		}
		break;
	case OBJ_PLAYERBOTTOM:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Player_bodyBottom_Diffuse.png");
		}
		break;
	case OBJ_PLAYERHEAD_BLUE:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Player_Head_Diffuse.png");
			LoadAnimation("Assets/Animations/Player_DanceAnimation.ani", nObjectID);
		}
		break;
	case OBJ_PLAYERHEAD_GREEN:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Player_Head_Green.png");
			LoadAnimation("Assets/Animations/Player_DanceAnimation.ani", nObjectID);
		}
		break;
	case OBJ_PLAYERTREAD:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Scrolling.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Treads.png");
		}
		break;
	case OBJ_PICKUP:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/LoungeTigers_Environment_Pickup.png");
		}
		break;
	case OBJ_TRASH:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_Trash_Bot.png");
			LoadAnimation("Assets/Animations/Enemy_Trash_Bot_Walk.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Trash_Bot_Attack.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Trash_Bot_Death.ani", nObjectID);
		}
		break;
	case OBJ_PATROL:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_Patrol_Bot_Diffuse.png");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
			LoadAnimation("TestFiles/Test_Anim2.ani", nObjectID);
		}
		break;
	case OBJ_SMASH:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_Smash_botT.png");
			LoadAnimation("Assets/Animations/Enemy_Smash_Bot_Walk.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Smash_Bot_Attack1.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Smash_Bot_Attack2.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Smash_Bot_Attack3.ani", nObjectID);
			LoadAnimation("TestFiles/Smash_Anim3.ani", nObjectID);
		}
		break;
	case OBJ_BURN:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_Burn_Bot_Diffuse.png");
			LoadAnimation("Assets/Animations/Enemy_Burn_Bot_Attack.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Burn_Bot_Death.ani", nObjectID);
		}
		break;
	case OBJ_STEAM:
	case OBJ_STEAM_WHEEL:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_Steamroller_Bot.png");
			LoadAnimation("Assets/Animations/Enemy_Steamroller_Bot_Charge.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Steamroller_Bot_Attack.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Steamroller_Bot_Ricochet.ani", nObjectID);
			LoadAnimation("Assets/Animations/Enemy_Steamroller_Bot_Death.ani", nObjectID);
		}
		break;
	case OBJ_AICORE:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_AiCore_Body_Diffuse.png");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
		}
		break;
	case OBJ_AI_HEAD:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_AiCore_Monitor_Diffuse.png");
		}
		break;
	case OBJ_AI_MONITORS:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_AiCore_Body_Diffuse.png");
		}
		break;
	case OBJ_AI_ARMS:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enemy_AiCore_Body_Diffuse.png");
		}
		break;
	case OBJ_AIFIST:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/Smash_Texture.png");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
		}
		break;
	case OBJ_SPIDER:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/Smash_Texture.png");
			LoadAnimation("TestFiles\\Test.ani", nObjectID);
		}
		break;
	case OBJ_BOSSINV:
	case OBJ_INVERSION:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/InversionTower.png");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
		}
		break;
	case OBJ_BOSSTURRET:
	case OBJ_TURRET:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/LaserTurret.png");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
		}
		break;
	case OBJ_POOL:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/Environment_Molten_Pool.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_DEFENSELASER:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Lounge_Tigers_Weapon_Defense_Laser.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_GATLING:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Lounge_Tigers_GatlingGun_T.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_BEAM:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Weapon_Pulse_Laser_Diffuse.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_TESLA:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Weapon_Tesla_Ray.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_TUNNEL:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Menu_Main_Tunnel.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_VAT:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enviroment_Vat.png");
			LoadAnimation("Assets/Animations/Environment_Vat_Explode.ani", nObjectID);
		}
		break;
	case OBJ_CRATE:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/robot_color.png");
			TAnimation tempAnim;
			tempAnim.m_fDuration = 10.0f;
			tempAnim.m_nNumMeshes = 1;
			tempAnim.m_pBones = new TBone[1];
			tempAnim.m_pBones->m_nNumFrames = 1;
			tempAnim.m_pBones->m_pKeyFrames = new TKeyFrame[2];
			D3DXMATRIX iden;
			D3DXMatrixIdentity(&iden);
			tempAnim.m_pBones->m_pKeyFrames[0].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[0].m_fDuration = 5.0f;
			tempAnim.m_pBones->m_pKeyFrames[1].m_d3dTransform = iden;
			tempAnim.m_pBones->m_pKeyFrames[1].m_fDuration = 10.0f;
			m_tObjectDataArray[nObjectID].m_vAnimations.push_back(tempAnim);
		}
		break;
	case OBJ_BARREL:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Environment_Barrel.png");
			LoadAnimation("Assets/Animations/Environment_Barrel_Explode.ani", nObjectID);
		}
		break;
	case OBJ_PIT:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/DeathPit.png");
			LoadAnimation("Assets/Animations/Gate_Open.ani", nObjectID);
			LoadAnimation("Assets/Animations/Gate_Close.ani", nObjectID);
		}
		break;
	case OBJ_GATE:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Intro_Floor.jpg");
		}
		break;
	case OBJ_SPINBOT:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/Smash_Texture.png");
			LoadAnimation("Assets/Animations/Midboss_Floor.ani", nObjectID);
		}
		break;
	case OBJ_MIDBOSSFLOOR:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Enviroment_MidBoss_Floor_Diffuse.png");
			LoadAnimation("Assets/Animations/Midboss_Floor.ani", nObjectID);
		}
		break;
	case EFFECT_OBJ_CIRCLE:
	case EFFECT_OBJ_CUBE:
	case EFFECT_OBJ_DISC:
	case EFFECT_OBJ_PLANE:
	case EFFECT_OBJ_TORUS:
	case EFFECT_OBJ_WAVE:
		break;
	case EFFECT_OBJ_CYLINDER:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Beam.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Weapon_Projectile_Pulse_Laser.png");
		}
		break;
	case EFFECT_OBJ_SPHERE:
		{
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Bullet.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("Assets/Textures/Lounge_Tigers_Weapon_Projectile_Defense_Laser.png");
		}
		break;
	default:
		{
			// make other cases for characters
			m_tObjectDataArray[nObjectID].m_pShader = LoadShader("Assets/Shaders/Textured.fx");
			m_tObjectDataArray[nObjectID].m_pTexture = LoadTexture("TestFiles/TestCube.jpg");
			LoadAnimation("TestFiles/Test.ani", nObjectID);
		}
	}

	fstream istream;
	istream.open(szFilename, ios_base::in|ios_base::binary);
	if(istream.is_open())
	{
		int currpos = 0;
		int memsize = 0;
		pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
		m_tObjectDataArray[nObjectID].m_nNumVerts = 0;
		istream.read((char*)&m_tObjectDataArray[nObjectID].m_nNumVerts, 4);
		pD3D->CreateVertexBuffer(sizeof(tVertex)*m_tObjectDataArray[nObjectID].m_nNumVerts, 0, 0, D3DPOOL_MANAGED, &m_tObjectDataArray[nObjectID].m_pVertexBuffer, 0);
		memsize = sizeof(tVertex)*m_tObjectDataArray[nObjectID].m_nNumVerts;
		void *vram = nullptr;
		m_tObjectDataArray[nObjectID].m_pVertexBuffer->Lock(0, 0, &vram, 0);
		currpos = (int)istream.tellg();
		istream.read((char*)vram, m_tObjectDataArray[nObjectID].m_nNumVerts*sizeof(tVertex));
		m_tObjectDataArray[nObjectID].m_pVertexBuffer->Unlock();

		currpos = (int)istream.tellg();
		m_tObjectDataArray[nObjectID].m_nNumTris = 0;
		istream.read((char*)&m_tObjectDataArray[nObjectID].m_nNumTris, 4);
		pD3D->CreateIndexBuffer(sizeof(tTri)*m_tObjectDataArray[nObjectID].m_nNumTris, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_tObjectDataArray[nObjectID].m_pIndexBuffer, 0);
		memsize = sizeof(tTri)*m_tObjectDataArray[nObjectID].m_nNumTris;

		currpos = (int)istream.tellg();
		m_tObjectDataArray[nObjectID].m_pIndexBuffer->Lock(0, 0, &vram, 0);
		istream.read((char*)vram, m_tObjectDataArray[nObjectID].m_nNumTris*sizeof(tTri));
		m_tObjectDataArray[nObjectID].m_pIndexBuffer->Unlock();

		istream.close();
	}
}

TAnimation* CAssetManager::SwitchAnimation(int nAnimationIndex, int nObjID)
{
	if(nAnimationIndex < (int)m_tObjectDataArray[nObjID].m_vAnimations.size())
	{
		return &m_tObjectDataArray[nObjID].m_vAnimations[nAnimationIndex];
	}
	return nullptr;
}


void CAssetManager::Shutdown()
{
	for(int i = 0; i < OBJ_TOTAL_OBJECTS; ++i)
	{
		for(unsigned int animation = 0; animation < m_tObjectDataArray[i].m_vAnimations.size(); ++animation)
		{
			for(unsigned int bone = 0; bone < m_tObjectDataArray[i].m_vAnimations[animation].m_nNumMeshes; ++bone)
			{
				delete [] m_tObjectDataArray[i].m_vAnimations[animation].m_pBones[bone].m_pKeyFrames;
			}
			delete [] m_tObjectDataArray[i].m_vAnimations[animation].m_pBones;
		}

		if(m_tObjectDataArray[i].m_pVertexBuffer)
			m_tObjectDataArray[i].m_pVertexBuffer->Release();

		if(m_tObjectDataArray[i].m_pIndexBuffer)
			m_tObjectDataArray[i].m_pIndexBuffer->Release();
	}

	unsigned int i = 0;
	for(; i < m_vVertexDeclarations.size(); ++i)
	{
		m_vVertexDeclarations[i]->Release();
	}
	m_vVertexDeclarations.clear();

	for(i = 0; i < m_vTextures.size(); ++i)
	{
		m_vTextures[i]->Release();
	}
	m_vTextures.clear();

	for(i = 0; i < m_vShaders.size(); ++i)
	{		
		m_vShaders[i]->Release();
	}
	m_vShaders.clear();
}

void CAssetManager::LoadEffectAnimations()
{
}
