/************************************************************************************
 * Filename:  		MoltenPool.h
 * Date:      		02/11/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Mike A. Barbato
 * Purpose:   		This is the environmental hazard that comes from Vats and Burn Bots
 ************************************************************************************/
#ifndef _MOLTENPOOL_H_
#define _MOLTENPOOL_H_

#include "IBaseObject.h"
class CEffect;

class CMoltenPool : public IBaseObject
{
	D3DXMATRIX m_d3dOriginalMatrix;
	float m_fMoltenTimer;
	float m_fDamage;
	bool m_bPoolCollision;
	bool m_bSafetoCollide;
	float m_fScale;
	float m_fScaleModifier;
	float m_fScaleTimer;
	float m_fScalingMod;
	D3DXMATRIX m_d3dScaleMatrix;
	int m_nPoolStage;
	float m_fDestroyTimer;
	bool m_bDestroyPool;
	int m_nDroppedID;
	CEffect* m_pBurnEffect;
	float m_fDamageDelay;

public:
	//Accessor
	float GetMoltenTimer() {return m_fMoltenTimer;}
	float GetDamage() {return m_fDamage;}
	bool GetPoolCollision() const {return m_bPoolCollision;}
	bool GetSafeToCollide() const {return m_bSafetoCollide;}
	float GetScale() const {return m_fScale;}
	float GetScaleMod() const {return m_fScaleModifier;}
	float GetScaleTimer() const {return m_fScaleTimer;}
	D3DXMATRIX GetScaleMat() const {return m_d3dScaleMatrix;}
	D3DXMATRIX GetOriginalMatrix() const {return m_d3dOriginalMatrix;}
	int GetPoolStage() const {return m_nPoolStage;}
	float GetScalingMod() const {return m_fScalingMod;}
	float GetDestroyTimer() const {return m_fDestroyTimer;}
	int GetDroppedID() const {return m_nDroppedID;}

	//Mutator
	void SetMoltenTimer(float fMoltenTimer) {m_fMoltenTimer = fMoltenTimer;}
	void SetDamage(float fDamage) {m_fDamage = fDamage;}
	void SetPoolCollision(bool bPoolCollision) {m_bPoolCollision = bPoolCollision;}
	void SetSafeToCollide(bool bSafeToCollide) {m_bSafetoCollide = bSafeToCollide;}
	void SetScale(float fScale) {m_fScale = fScale;}
	void SetScaleMod(float fScaleModifier) {m_fScaleModifier = fScaleModifier;}
	void SetScaleTimer(float fScaleTimer) {m_fScaleTimer = fScaleTimer;}
	void SetScaleMatrix(D3DXMATRIX d3dScaleMatrix) {m_d3dScaleMatrix = d3dScaleMatrix;}
	void SetOriginalMatrix(D3DXMATRIX d3dOGMatrix) {m_d3dOriginalMatrix = d3dOGMatrix;}
	void SetPoolStage(int nPoolStage) {m_nPoolStage = nPoolStage;}
	void SetScalingMod(float fScalingMod) {m_fScalingMod = fScalingMod;}
	void SetDestroyTimer(float fDestroyTimer) {m_fDestroyTimer = fDestroyTimer;}
	void SetDroppedID(int nDroppedID) {m_nDroppedID = nDroppedID;}

	CMoltenPool(void);
	~CMoltenPool(void);

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
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
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
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
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
	 virtual void TakeDamage(IBaseObject* _pIn) {};

	 void ForceAnimation(float fDelta);

	 void Render(void);

};
#endif
