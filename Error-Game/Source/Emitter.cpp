/**********************************************************************************************
 * Filename:  		Emitter.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		03/02/2013
 * Mod. Initials:	TS
 * Author:    		Tyler D. Springer
 * Purpose:   		This is the class that will create and update particles.
 **********************************************************************************************/
#include "Emitter.h"
#include "EffectTestState.h"
#include "Renderer.h"
#include "GameplayState.h"
#include <iostream>

void CEmitter::SetColors(D3DXCOLOR d3dColor)
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

void CEmitter::SetMatrix(D3DXMATRIX d3dPosition)
{
	D3DXVECTOR3 offset = GetOffset();
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans,offset.x,offset.y,offset.z);
	d3dPosition = trans * d3dPosition;
	SetPosition(d3dPosition);
}

CEmitter::CEmitter()
{
	for(int i=0;i<MAX_PARTICLES;i++)
		m_nAvailableParticles.push_back(i);
	m_tVertexData[0].d3dPosition = D3DXVECTOR3(-0.5f,-0.5f,0.0f);
	m_tVertexData[0].d3dUV = D3DXVECTOR2(0.0f,0.0f);
	m_tVertexData[1].d3dPosition = D3DXVECTOR3(-0.5f,0.5f,0.0f);
	m_tVertexData[1].d3dUV = D3DXVECTOR2(0.0f,1.0f);
	m_tVertexData[2].d3dPosition = D3DXVECTOR3(0.5f,0.5f,0.0f);
	m_tVertexData[2].d3dUV = D3DXVECTOR2(1.0f,1.0f);
	m_tVertexData[3].d3dPosition = D3DXVECTOR3(0.5f,-0.5f,0.0f);
	m_tVertexData[3].d3dUV = D3DXVECTOR2(1.0f,0.0f);
}

CEmitter::~CEmitter()
{
}

void CEmitter::Init()
{
	//TODO: Move to manager
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	pD3D = CRenderer::GetInstance()->GetDirect3DDevice();
	d3dShader = CAssetManager::GetInstance()->LoadShader("TestFiles/Particle.fx");
	pD3D->CreateVertexDeclaration(decl, &d3dTempDecl);
	pD3D->CreateVertexBuffer(sizeof(TPointVertex)*4, 0, 0, D3DPOOL_MANAGED, &d3dTempBuff, 0);
	void* vram = nullptr;
	d3dTempBuff->Lock(0, 0, &vram, 0);
	memcpy(vram, m_tVertexData, sizeof(TPointVertex)*4);
	d3dTempBuff->Unlock();
}

void CEmitter::ShutDown()
{
	//TODO: Move to manager
	if(d3dTempBuff)
		d3dTempBuff->Release();
	if(d3dTempDecl)
		d3dTempDecl->Release();
}

bool CEmitter::Update(float fDelta)
{
	if(GetStart() > 0.0f)
	{
		SetStart(GetStart() - fDelta);
	}
	else if(GetLife() > 0.0f || GetLoop())
	{
		if(!GetLoop() && GetLife() > 0.0f)
			m_fSpawnTimer += min(fDelta,GetLife());
		else
			m_fSpawnTimer += fDelta;
		SetLife(GetLife() - fDelta);
		D3DXVECTOR3 curVelocity = fDelta*GetVelocity();
		D3DXMATRIX translate;
		D3DXMatrixTranslation(&translate,curVelocity.x,curVelocity.y,curVelocity.z);
		SetPosition(GetPosition()*translate);
		while(m_fSpawnTimer >= m_fSpawnRate)
		{
			if(m_nAvailableParticles.size())
			{
				int nAvailable = m_nAvailableParticles.back();
				m_nAvailableParticles.pop_back();
				TParticleStruct tStruct;
				D3DXMATRIX iden;
				D3DXMatrixIdentity(&iden);
				tStruct.d3dStartPos = iden;
				tStruct.nRandomnessPos = m_nRandomPercentPos;
				tStruct.nRandomnessVel = m_nRandomPercentVel;
				tStruct.tStage = GetStage(1);
				tStruct.tStartStage = GetStage(0);
				m_cParticles[nAvailable].Create(tStruct);
				m_fSpawnTimer -= m_fSpawnRate;
			}
		}
	}
	int nNumActive = (MAX_PARTICLES-m_nAvailableParticles.size());
	if(nNumActive <= 0)
		return false;
	for(int index=0;index<MAX_PARTICLES;index++)
	{
		if(!m_cParticles[index].IsAlive())
			continue;
		int nChange = m_cParticles[index].Update(fDelta);
		if(nChange == -1)
			m_nAvailableParticles.push_back(index);
		else if(nChange > 0)
			m_cParticles[index].SetNextStage(GetStage(nChange));
	}
	return true;
}

void CEmitter::Render(D3DXMATRIX d3dCam,D3DXMATRIX d3dProj,D3DXMATRIX d3dForward)
{
	CFXManager* pFM = CGameplayState::GetInstance()->GetFX();
	int nNumActive = (MAX_PARTICLES-m_nAvailableParticles.size());
	if(nNumActive <= 0)
		return;
	unsigned int passes(0);
	int nTexture = GetTexture();
	if(nTexture > 4 && nTexture < 9)
	{
		D3DXHANDLE hTechnique = d3dShader->GetTechniqueByName("MagentaRenderScene");
		d3dShader->SetTechnique(hTechnique);
	}
	d3dShader->Begin(&passes,0);
	d3dShader->SetTexture("g_MeshTexture",pFM->GetTexture(nTexture));
	d3dShader->SetMatrix("g_mViewProjection",&d3dProj);
	d3dShader->CommitChanges();
	for(unsigned i(0);i < passes; i++)
	{
		d3dShader->BeginPass(i);

		pD3D->SetVertexDeclaration(d3dTempDecl);
		pD3D->SetStreamSource(0,d3dTempBuff,0,sizeof(TPointVertex));
		for(int i=0;i<MAX_PARTICLES;i++)
		{
			if(m_cParticles[i].IsAlive())
			{
				D3DXCOLOR color=m_cParticles[i].GetColor();
				d3dShader->SetVector("g_Color",&((D3DXVECTOR4)m_cParticles[i].GetColor()));
				D3DXMATRIX d3dMat;
				D3DXVECTOR3 offsetVec = GetOffset();
				D3DXMATRIX offsetMat;
				D3DXMatrixTranslation(&offsetMat, offsetVec.x, offsetVec.y, offsetVec.z);
				d3dMat=GetPosition() * m_cParticles[i].GetPosition() * offsetMat * d3dForward;
				D3DXVECTOR3 toVector = (*(D3DXVECTOR3*)&d3dMat[12] - *(D3DXVECTOR3*)&d3dCam[12]);
				D3DXVec3Normalize((D3DXVECTOR3*)&d3dMat[8],&toVector);
				D3DXVECTOR3 right,up,tempX,tempY;
				D3DXVec3Cross(&right,(D3DXVECTOR3*)&d3dCam[4],(D3DXVECTOR3*)&d3dMat[8]);
				D3DXVec3Cross(&up,(D3DXVECTOR3*)&d3dMat[8],&right);
				D3DXVec3Cross(&tempX,&up,(D3DXVECTOR3*)&d3dMat[8]);
				D3DXVec3Normalize((D3DXVECTOR3*)&d3dMat[0],&tempX);
				D3DXVec3Cross(&tempY,(D3DXVECTOR3*)&d3dMat[8],(D3DXVECTOR3*)&d3dMat[0]);
				D3DXVec3Normalize((D3DXVECTOR3*)&d3dMat[4],&tempY);
				D3DXMATRIX identity;
				D3DXMatrixIdentity(&identity);
				d3dShader->SetMatrix("g_Billboard",&d3dMat);
				D3DXMATRIX rotate=m_cParticles[i].GetRotateMatrix();
				d3dShader->SetMatrix("g_Rotate",&rotate);
				D3DXMATRIX scale=m_cParticles[i].GetScaleMatrix();
				d3dShader->SetMatrix("g_Scale",&scale);
				d3dShader->CommitChanges();
				pD3D->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
			}
		}
		d3dShader->EndPass();
	}
	d3dShader->End();
}

void CEmitter::Create(TEmitterStruct tStruct)
{
	m_nAvailableParticles.clear();
	for(int i=0;i<MAX_PARTICLES;i++)
		m_nAvailableParticles.push_back(i);
	((ISubEffect*)this)->Create(tStruct.fStartTime,tStruct.fLife,tStruct.nMeshID,tStruct.nTextureID,tStruct.bLooping,tStruct.tStages,
		tStruct.d3dPosition,tStruct.d3dOffset,tStruct.d3dEffectVelocity,tStruct.source,tStruct.dest);
	m_fSpawnTimer = 0.0f;
	m_fSpawnRate = tStruct.fSpawnRate;
	m_nRandomPercentPos = tStruct.nRandomnessP;
	m_nRandomPercentVel = tStruct.nRandomnessV;
}
void CEmitter::ReCreate(TEmitterStruct tStruct)
{
	((ISubEffect*)this)->Create(tStruct.fStartTime,tStruct.fLife,GetMeshID(),GetTexture(),GetLoop(),nullptr,
		GetPosition(),GetOffset(),tStruct.d3dEffectVelocity,tStruct.source,tStruct.dest);
	m_fSpawnTimer = 0.0f;
}