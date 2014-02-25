#ifndef _LEVELDATA_H_
#define _LEVELDATA_H_

/***********************************************
 * Filename:  		LevelData.h
 * Date:      		02/09/2013
 * Mod. Date: 		03/05/2013
 * Mod. Initials:	MB
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all level data
 ************************************************/
#include <d3dx9.h>
#include <vector>
#include "SpawnPoint.h"
#include "Conveyor.h"

struct TWorldObject
{
	int m_nObjectID;
	float m_fWorldX;
	float m_fWorldZ;
};

struct TWayPoint
{
	D3DXVECTOR3 vPos;
	float fTime;
	bool checkpoint;
	std::vector<CSpawnPoint> vSpawns;
	std::vector<TWorldObject> m_vWorldObjects;
};

struct TTileVert
{
	float m_fVerts[3];
	float m_fUVs[2];
};

class CRenderer;
class CObjectFactory;
class CIntroState;

enum ETileSets { TILE_ASSEMBLY, TILE_FOUNDRY, TILE_AICORE, NUM_TILESETS };

class CLevelData
{
	CRenderer*						m_pRenderer;
	IDirect3DVertexBuffer9*			m_pTileVertexBuffer;
	IDirect3DVertexBuffer9*			m_pConveyorVertexBuffer;
	int								m_nNumTiles[NUM_TILESETS];
	int								m_nTotalTiles;
	int								m_nNumConveyors;
	vector<CConveyor*>				m_vActiveConveyors;
	CObjectFactory*					m_pOF;
	D3DXVECTOR4						m_d3dAmbientLights[NUM_TILESETS];

	bool							m_bIntroState;
	CIntroState*					m_pIntroState;

	bool							m_bFoundry;
	bool							m_bDisableConveyors;
	float							m_fFoundryScroll;

	IDirect3DTexture9*				m_TileSets[NUM_TILESETS];
	IDirect3DTexture9*				m_TileNormals[NUM_TILESETS];
	int								m_nNextWayPoint;
	int								m_tCurrPoint;
	int								m_nCheckpointID;
	D3DXMATRIX						m_d3dCheckpointCam;
	std::vector<TWayPoint>			m_vWaypoints;

	float							m_fSpinFloorAnimTime;
	bool							m_bRenderFloor;
	bool							m_bOpenFloor;
	D3DXMATRIX						m_d3dFloorCurrFrame[2];

	D3DXMATRIX m_d3dRoamingPoint;

public:
	
	/*****************
	*Accessors
	*****************/
	int GetCurWayPoint(void) {return m_tCurrPoint;}
	std::vector<TWayPoint> GetWayPoints(void) {return m_vWaypoints;}

	/*****************
	*Mutators
	*****************/
	void ToggleFoundry();
	void EnableMidbossFloor() {m_bRenderFloor = true;}
	void TriggerFloor() {m_bOpenFloor = true;}

	CLevelData(void);
	~CLevelData(void);

	/*****************************************************************
	* void RenderFloor(): Updates the midboss floor to open and turns off
	*					floor render when finished
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void RenderFloor();

	/*****************************************************************
	* void Render(): renders entire level based on data
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	void Render();

	/*****************************************************************
	* ResetToCheckpoint(): Resets Camera and gameplay state to last checkpoint
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/21/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void ResetToCheckpoint();

	/*****************************************************************
	* SkipToNextWaypoint(): Resets to the next waypoint
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/13/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void SkipToNextWaypoint();

	/*****************************************************************
	* void Init(): Loads Level data
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/19/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void Init();

	/*****************************************************************
	* TWaypoint GetNextWaypoint(int _nCurrentWaypoint): returns the next waypoint
	*
	* Ins:			    int _nCurrentWaypoint
	*
	* Outs:				void
	*
	* Returns:		    TWaypoint
	*
	* Mod. Date:		02/09/2013
	* Mod. Initials:	JT
	*****************************************************************/
	TWayPoint GetNextWaypoint(void);


	/*****************************************************************
	* Update:			Updates the level data
	*
	* Ins:			    float fDelta
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void Update(float fDelta);

	/*****************************************************************
	* LoadData:			Loads the level data
	*
	* Ins:			    szFilename
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/16/2013
	* Mod. Initials:	MB
	*****************************************************************/
	void LoadData(char* szFilename);

	//  Copy Paste to LevelData in Repo
	/*****************************************************************
	* Update:			Removes dynamic memory before Renderer is shutdown
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		02/21/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void DestroyLevelData();

	/*****************************************************************
	* Update:			Stops all conveyors from scrolling
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		04/28/2013
	* Mod. Initials:	SD
	*****************************************************************/
	void ToggleConveyors();
};

#endif