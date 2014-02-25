/**********************************************************************************************
 * Filename:  		GeometryEffect.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/11/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that uses meshes and textures for effects.
 **********************************************************************************************/
#include "GeometryEffect.h"
#include "Renderer.h"
#include "GameplayState.h"

void CGeometryEffect::SetColors(D3DXCOLOR d3dColor)
{
	for(int i = 0; i < MAX_STAGES; ++i)
	{
		TStage tStage = GetStage(i);
		float alpha = tStage.d3dColor.a;
		tStage.d3dColor = d3dColor;
		tStage.d3dColor.a = alpha;
		SetStage(i, tStage);
	}
}

CGeometryEffect::CGeometryEffect(void)
{

}

CGeometryEffect::~CGeometryEffect(void)
{

}

void CGeometryEffect::Init()
{
	m_pAM = CAssetManager::GetInstance();
	m_pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	m_d3dShader = m_pAM->LoadShader("TestFiles/Geometry.fx");
}

void CGeometryEffect::ShutDown()
{

}

bool CGeometryEffect::Update(float fDelta)
{
	if(GetStart() > 0.0f)
	{
		SetStart(GetStart() - fDelta);
	}
	else if(GetLife() > 0.0f)
	{
		SetLife(GetLife() - fDelta);
		m_fDuration += fDelta;
		if(m_fDuration >= GetStage(m_nTargetStage).fTime)
		{
			m_nPrevStage++;
			m_nTargetStage++;
		}
		if(GetStage(m_nTargetStage).fTime <= 0.0f)
		{
			if(GetLoop())
			{
				m_nPrevStage = 0;
				m_nTargetStage = 1;
				m_fAnimationTime = 0.0f;
				m_fDuration = 0.0f;
				D3DXMatrixIdentity(&m_d3dBone);
			}
			else
			{
				m_bIsAlive = false;
				return m_bIsAlive;
			}
		}
		if(m_bAnimated)
		{
			float animTime = m_fAnimationTime;
			animTime += fDelta;
			CAnimationProcessor::UpdateTime(m_pAM->GetAnimation(EFFECT_OBJ_ANIMATION, m_unCurAnimation), animTime);
			CAnimationProcessor::Interpolator(m_pAM->GetAnimation(EFFECT_OBJ_ANIMATION, m_unCurAnimation), animTime, &m_d3dBone);
			m_fAnimationTime = animTime;
		}
		float tween = GetStage(m_nTargetStage).fTime - GetStage(m_nPrevStage).fTime;
		float lambda = (m_fDuration - GetStage(m_nPrevStage).fTime)/tween;
		SetVelocity(((GetStage(m_nTargetStage).d3dVelocity - GetStage(m_nPrevStage).d3dVelocity) * lambda) + GetStage(m_nPrevStage).d3dVelocity);
		m_d3dColor = ((GetStage(m_nTargetStage).d3dColor - GetStage(m_nPrevStage).d3dColor) * lambda) + GetStage(m_nPrevStage).d3dColor;
		m_d3dCurScale = ((GetStage(m_nTargetStage).d3dScale - GetStage(m_nPrevStage).d3dScale) * lambda) + GetStage(m_nPrevStage).d3dScale;
		m_d3dCurRotation = ((GetStage(m_nTargetStage).d3dRotation - GetStage(m_nPrevStage).d3dRotation) * lambda) + GetStage(m_nPrevStage).d3dRotation;
		D3DXVECTOR3 curVelocity = fDelta*GetVelocity();
		D3DXMATRIX translate;
		D3DXMatrixTranslation(&translate,curVelocity.x,curVelocity.y,curVelocity.z);
		SetPosition(GetPosition()*translate);
	}
	return m_bIsAlive;
}

void CGeometryEffect::Render(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj,D3DXMATRIX d3dForward)
{
	CFXManager* pFM = CGameplayState::GetInstance()->GetFX();
	unsigned int passes = 0;
	m_d3dShader->Begin(&passes,0);
	m_d3dShader->SetTexture("g_MeshTexture",pFM->GetTexture(GetTexture()));
	m_d3dShader->SetMatrix("g_mViewProjection",&d3dProj);
	m_d3dShader->CommitChanges();
	for(unsigned int i = 0; i < passes; i++)
	{
		m_d3dShader->BeginPass(i);
		m_d3dShader->SetVector("g_Color",&((D3DXVECTOR4)m_d3dColor));
		D3DXMATRIX d3dRotX, d3dRotY, d3dRotZ, d3dScale;
		D3DXMatrixRotationX(&d3dRotX,m_d3dCurRotation[0]);
		D3DXMatrixRotationX(&d3dRotY,m_d3dCurRotation[1]);
		D3DXMatrixRotationX(&d3dRotZ,m_d3dCurRotation[2]);
		D3DXMatrixScaling(&d3dScale,m_d3dCurScale[0],m_d3dCurScale[1],m_d3dCurScale[2]);
		m_d3dShader->SetMatrix("g_Bone",&m_d3dBone);
		D3DXVECTOR3 offsetVec = GetOffset();
		D3DXMATRIX offsetMat;
		D3DXMatrixTranslation(&offsetMat, offsetVec.x, offsetVec.y, offsetVec.z);
		D3DXMATRIX d3dPosition = offsetMat * GetPosition() * d3dForward;
		m_d3dShader->SetMatrix("g_Translate",&d3dPosition);
		m_d3dShader->SetMatrix("g_Rotate",&(d3dRotX*d3dRotY*d3dRotZ));
		m_d3dShader->SetMatrix("g_Scale",&d3dScale);
		m_d3dShader->CommitChanges();
		m_pD3D->SetVertexDeclaration(m_pAM->GetVertexDeclaration(BASEOBJECTDECL));
		m_pD3D->SetStreamSource(0, m_pAM->GetVertBuffer(GetMeshID()),0,sizeof(tVertex));
		m_pD3D->SetIndices(m_pAM->GetIndexBuffer(GetMeshID()));
		m_pD3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_pAM->GetNumVerts(GetMeshID()),0,m_pAM->GetNumTriangles(GetMeshID()));
		m_d3dShader->EndPass();
	}
	m_d3dShader->End();
}

void CGeometryEffect::Create(TGeometryStruct tStruct)
{
	m_nPrevStage = 0;
	m_nTargetStage = 1;
	m_fAnimationTime = 0.0f;
	m_fDuration = 0.0f;
	D3DXMatrixIdentity(&m_d3dBone);
	m_bAnimated = tStruct.bAnimated;
	if(m_bAnimated)
	{
		TAnimation* newAnim = CAssetManager::GetInstance()->SwitchAnimation(tStruct.nAnimID, EFFECT_OBJ_ANIMATION);
		if(newAnim)
		{
			m_unCurAnimation = tStruct.nAnimID;
			m_fAnimationTime = 0.0;
		}
		else
		{
			m_unCurAnimation = 0;
		}

		TAnimation* readBones = CAssetManager::GetInstance()->GetAnimation(EFFECT_OBJ_ANIMATION, m_unCurAnimation);
		for(unsigned int i = 0; i < readBones->m_nNumMeshes; ++i)
		{
			readBones->m_pBones[i].m_nCurrFrame = 0;
		}
	}
	((ISubEffect*)this)->Create(tStruct.fStartTime,tStruct.fLife,tStruct.nMeshID,tStruct.nTextureID,tStruct.bLooping,tStruct.tStages,
		tStruct.d3dPosition,tStruct.d3dOffset,tStruct.d3dEffectVelocity,tStruct.source,tStruct.dest);
}

void CGeometryEffect::ReCreate(TGeometryStruct tStruct)
{
	m_nPrevStage = 0;
	m_nTargetStage = 1;
	m_fAnimationTime = 0.0f;
	m_fDuration = 0.0f;
	D3DXMatrixIdentity(&m_d3dBone);
	if(m_bAnimated)
	{
		TAnimation* newAnim = CAssetManager::GetInstance()->SwitchAnimation(tStruct.nAnimID, EFFECT_OBJ_ANIMATION);
		if(newAnim)
		{
			m_fAnimationTime = 0.0;
		}
		else
		{
			m_unCurAnimation = 0;
		}

		TAnimation* readBones = CAssetManager::GetInstance()->GetAnimation(EFFECT_OBJ_ANIMATION, m_unCurAnimation);
		for(unsigned int i = 0; i < readBones->m_nNumMeshes; ++i)
		{
			readBones->m_pBones[i].m_nCurrFrame = 0;
		}
	}
	((ISubEffect*)this)->Create(tStruct.fStartTime,tStruct.fLife,GetMeshID(),GetTexture(),GetLoop(),nullptr,
		GetPosition(),GetOffset(),tStruct.d3dEffectVelocity,tStruct.source,tStruct.dest);
}