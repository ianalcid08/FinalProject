#ifndef _AITOWERPHASE_H_
#define _AITOWERPHASE_H_

#include "Definitions.h"
#include "IAIState.h"
class CObjectFactory;
class CBossInversionTower;

class CAITowerPhase : public IAIState
{
	unsigned int m_nNumPlayers;
	unsigned int m_nTrashSpawned;
	unsigned int m_nPatrolSpawned;
	unsigned int m_nTrashTotal;
	unsigned int m_nPatrolTotal;

	float m_fPickupTime;
	float m_fTrashTime;
	float m_fPatrolTime;

	CBossInversionTower* m_pTower;
	CObjectFactory* m_pOF;

	std::vector<D3DXVECTOR3> m_vSpawnPoints;
	D3DXVECTOR3 m_d3dCenter;

	void SpawnPickups(float _fTime);
	void SpawnTrash(float _fTime);
	void SpawnPatrol(float _fTime);

public:
	CAITowerPhase(void);
	virtual ~CAITowerPhase(void);

	// Access
	CBossInversionTower* GetTower() const { return m_pTower; }

	// Mutate
	void SetTower(CBossInversionTower* _pT) { m_pTower = _pT; }

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif