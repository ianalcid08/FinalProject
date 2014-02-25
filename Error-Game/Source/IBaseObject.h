/***********************************************
 * Filename:  		IBaseObject.h
 * Date:      		02/08/2013
 * Mod. Date: 		02/20/2013
 * Mod. Initials:	JT
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the base interface.
 ************************************************/
#ifndef _IBASEOBJECT_H_
#define _IBASEOBJECT_H_
#include "CollisionLibrary.h"
#include <d3dx9math.h>
#include <vector>
using std::vector;
#include "AssetManager.h"

class IBaseObject
{
	float m_nHealth;
	int m_nObjectID;
	bool m_bAlive;
	bool m_bIsActive;
	int m_nAmIBeingHit;
	D3DXMATRIX m_d3dMatrix;
	float m_fRadius;
	D3DXVECTOR4 m_vec4Color;

	float m_fAnimationTime;
	int m_unCurrAnimation;
	vector<D3DXMATRIX> m_vd3dCurrFrame;

	std::vector<IBaseObject*> m_vReactVector;

public:

	// TODO: To be ultimately deleted - this array will be used to negate animations during asset integration
	void KillCurrFrame();

	virtual ~IBaseObject() = 0 {};
	/***************
	 *  Accessors
	 ***************/
	std::vector<IBaseObject*> GetReactVector(void) {return m_vReactVector;} 
	float GetRadius() {return m_fRadius;}
	bool GetIsActive(void) {return m_bIsActive;}
	float GetHealth(){return m_nHealth;}
	int GetID(){return m_nObjectID;}
	bool GetAlive(){return m_bAlive;}
	D3DXMATRIX GetMatrix(){return m_d3dMatrix;}
	float GetAnimationTime() {return m_fAnimationTime;}
	TAnimation* GetCurrentAnimation() {return CAssetManager::GetInstance()->GetAnimation(m_nObjectID, m_unCurrAnimation);}
	unsigned int GetAnimationID() {return m_unCurrAnimation;}
	int GetBeingHit() { return m_nAmIBeingHit;}
	virtual Sphere GetSphere();
	AABB  GetAABB();
	D3DXVECTOR4 GetColor() {return m_vec4Color;}
	/***************
	 * Mutators
	 ***************/
	void SetRadius(float fRadius) {m_fRadius=fRadius;}
	void SetIsActive(bool bIsActive) {m_bIsActive = bIsActive;}
	void SetHealth(float nHealth){m_nHealth=nHealth;}
	void SetAlive(bool bAlive){m_bAlive=bAlive;}
	void SetID(int nObjectID) {m_nObjectID = nObjectID;}
	void SetMatrix(D3DXMATRIX d3dMatrix){m_d3dMatrix=d3dMatrix;}
	void SetAnimationTime(float fTime) {m_fAnimationTime = fTime;}
	D3DXMATRIX* PassCurrFrame() {return &m_vd3dCurrFrame[0];}
	void SetBeingHit(int _beinghit) { m_nAmIBeingHit = _beinghit;}
	void SetColor(D3DXVECTOR4 vec4Color) {m_vec4Color = vec4Color;}
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
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	virtual void Update(float fDelta) = 0;
	/*****************************************************************
	 * CheckCollision():	Checks for Collision
	 *
	 * Ins:					obj
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/08/2013
	 * Mod. Initials:		TS
	 *****************************************************************/
	virtual bool CheckCollision(IBaseObject* obj) = 0;

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
	virtual void CollisionResponse(IBaseObject* obj) = 0;

	/*****************************************************************
	 * ChangeAnimation():	Changes between an Object's Animations
	 *
	 * Ins:					nAnimationIndex
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/11/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void ChangeAnimation(int nAnimationIndex);

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
	virtual void TakeDamage(IBaseObject* _pIn) = 0;

	/*****************************************************************
	 * Render():		Virtual call to allow each base object to override render is needed
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	virtual void Render();

	void AddToReactVector(IBaseObject* pObj);
	void ClearReactVector();
};


#endif