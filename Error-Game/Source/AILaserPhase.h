#ifndef _AILASERPHASE_H_
#define _AILASERPHASE_H_

#include "iaistate.h"
#include "Definitions.h"

class CObjectFactory;
class CBossInversionTower;
class CBossTurret;

class CAILaserPhase : public IAIState
{
	unsigned int m_nNumPlayers;

	float m_fTime;
	float m_fPickupTime;

	CBossInversionTower* m_pTower;
	CBossTurret* m_pTurrets[4];
	D3DXVECTOR3 m_d3dTurretLoc[4];
	CObjectFactory* m_pOF;

	bool AllTurretsDead();

public:
	CAILaserPhase(void);
	virtual ~CAILaserPhase(void);

	// Access
	CBossInversionTower* GetTower() const { return m_pTower; }

	// Mutate
	void SetTower(CBossInversionTower* _pT) { m_pTower = _pT; }

	void Initialize();
	void Shutdown();
	bool Update(float _fTime);
};

#endif