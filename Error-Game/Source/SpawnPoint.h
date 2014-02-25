/************************************************************************************
 * Filename:  		SpawnPoint.h
 * Date:      		02/16/2013
 * Mod. Date: 		02/16/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is what will spawn enemies.
 ************************************************************************************/
#ifndef _CSPAWNPOINT_H_
#define _CSPAWNPOINT_H_
class CObjectFactory;
enum eObjects;
#include <d3dx9math.h>
#include <vector>
using std::vector;

class CSpawnPoint
{
public:
	struct Spawn
	{
		eObjects nObjectID;
		float fSpawnTime;
	};

	D3DXMATRIX m_d3dPosition;
	CObjectFactory* m_pObjectFactory;
	float m_fDuration;
	std::vector<Spawn> m_vSpawnObjects;
	unsigned int m_nCurrentSpawn;

	CSpawnPoint();
	~CSpawnPoint();
	/*****************************************************************
	 * Update():			Updates time and spawns enemies when needed.
	 *
	 * Ins:					fDelta
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/16/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	void Update(float fDelta);

	/*****************************************************************
	 * Update():			Resets the spawnpoint back to it's start
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			03/06/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void Reset();
};

#endif