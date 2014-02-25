/***********************************************
 * Filename:  		CAssetManager.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/15/2013
 * Mod. Initials:	MB
 * Author:    		Sean W. Davis
 * Purpose:   		Responsible for loading and holding assets
 ************************************************/
#ifndef ASSETMANAGER
#define ASSETMANAGER

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "AnimationProcessor.h"
#include "Definitions.h"

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <fstream>
using std::ios_base;
using std::fstream;

enum eVertDecls {BASEOBJECTDECL, POS_COLOR, POS_TEXCOORD, JUST_POS, NUM_VERTDECLS};

class CAssetManager
{
	struct TObjectData
	{
		IDirect3DVertexBuffer9*				m_pVertexBuffer;
		IDirect3DIndexBuffer9*				m_pIndexBuffer;
		ID3DXEffect*						m_pShader;
		IDirect3DTexture9*					m_pTexture;
		int									m_nNumVerts;
		int									m_nNumTris;
		vector<TAnimation>					m_vAnimations;
	};
	TObjectData								m_tObjectDataArray[OBJ_TOTAL_OBJECTS];

	vector<IDirect3DVertexDeclaration9*>	m_vVertexDeclarations;
	vector<ID3DXEffect*>					m_vShaders;
	vector<IDirect3DTexture9*>				m_vTextures;

	// Maps for ensuring only one instance of loaded assets
	map<char*, IDirect3DTexture9*>			m_mLoadedTextures;
	map<char*, ID3DXEffect*>				m_mLoadedShaders;

	LPDIRECT3DDEVICE9						pD3D;

	/*****************************************************************
	 * LoadAnimation:		Loads the passed in Animation file for loading into the
	 *						Data Array
	 *
	 * Ins:					szFilename
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void LoadAnimation(char* szFilename, int nObjID);
	
	/*****************************************************************
	 * LoadMesh:			Loads a Mesh File and stores Loaded data
	 *						into the ObjectDataArray.
	 *
	 * Ins:					szFilename
	 *
	 * Outs:				void
	 *
	 * Returns:				IDirect3DTexture9*
	 *
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void LoadMesh(char* szFilename, int nObjectID);
	
	/*****************************************************************
	 * LoadEffectAnimations:Loads all the animations for the geometry
	 *						effects to use.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/27/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void LoadEffectAnimations();
	CAssetManager(void);
	~CAssetManager(void);

public:

	static CAssetManager* GetInstance();
	/*****************
	*Accessors
	*****************/
	ID3DXEffect*GetShader(int nObjID) {return m_tObjectDataArray[nObjID].m_pShader;}
	IDirect3DVertexBuffer9*	GetVertBuffer(int nObjID) {return m_tObjectDataArray[nObjID].m_pVertexBuffer;}
	IDirect3DIndexBuffer9*	GetIndexBuffer(int nObjID) {return m_tObjectDataArray[nObjID].m_pIndexBuffer;}
	IDirect3DVertexDeclaration9* GetVertexDeclaration(int nDeclID) {return m_vVertexDeclarations[nDeclID];}
	IDirect3DTexture9* GetTexture(int nObjID) {return m_tObjectDataArray[nObjID].m_pTexture;}
	TAnimation* GetAnimation(int nObjID, int nAnimationNum) {return &m_tObjectDataArray[nObjID].m_vAnimations[nAnimationNum];}
	int	GetNumVerts(int nObjID) {return m_tObjectDataArray[nObjID].m_nNumVerts;}
	int	GetNumTriangles(int nObjID) {return m_tObjectDataArray[nObjID].m_nNumTris;}

	/*****************************************************************
	 * LoadTexture:			Checks the map of Loaded Textures. If Texture
	 *						Is not present, it will Load in the Texture, and
	 *						Update the map. Returns pointer to
	 *						already loaded Texture. 
	 *
	 * Ins:					szFilename
	 *
	 * Outs:				void
	 *
	 * Returns:				IDirect3DTexture9*
	 *
	 * Mod. Date:			02/09/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	IDirect3DTexture9* LoadTexture(char* szFilename);

	/*****************************************************************
	 * LoadShader:			Checks the map of loaded shaders. If shader
	 *						Is not present, it will load in the shader, and
	 *						Update the map. Returns pointer to
	 *						already loaded shader. 
	 *
	 * Ins:					szFilename
	 *
	 * Outs:				void
	 *
	 * Returns:				ID3DXEffect*
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	ID3DXEffect* LoadShader(char* szFilename);

	/*****************************************************************
	 * SwitchAnimation:		Uses Animation index to access and return
	 *						the requested animation from the ObjectDataArray. 
	 *
	 * Ins:					nAnimationIndex
	 *
	 * Outs:				void
	 *
	 * Returns:				TAnimation*
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	TAnimation* SwitchAnimation(int nAnimationIndex, int nObjID);

	/*****************************************************************
	 * void Shutdown():		Deletes all dynamic memory and releases
	 *						d3dpointers
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/13/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void Shutdown();

	/*****************************************************************
	 * initialize:		   initializes the asset manager 
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/15/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void initialize();
};

#endif