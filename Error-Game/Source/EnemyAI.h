#ifndef _ENEMYAI_H_
#define _ENEMYAI_H_

/************************************************************************************
 * Filename:  		EnemyAI.h
 * Date:      		02/16/2013
 * Mod. Date: 		02/16/2013
 * Mod. Initials:	JT
 * Author:    		Joe Thomas
 * Purpose:   		Handles AI for all enemies
 ************************************************************************************/

#include "Definitions.h"

class CEnemyObject;
class IAIState;
class IBaseObject;

class CEnemyAI
{
	CEnemyObject* m_pThis;

	void (CEnemyAI::*pUpdate)(float _fTime);

	void TrashUpdate(float _fTime);
	void PatrolUpdate(float _fTime);
	void SmashUpdate(float _fTime);
	void BurnUpdate(float _fTime);
	void SpiderUpdate(float _fTime);
	void CoreUpdate(float _fTime);
	void FistUpdate(float _fTime);
	void SteamUpdate(float _fTime);
	void SpinUpdate(float _fTime);

public:
	CEnemyAI(void);
	~CEnemyAI(void);

	void Initialize(CEnemyObject* _pEnemy);
	void Update(float _fTime);
};

#endif