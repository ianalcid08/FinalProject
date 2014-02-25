#include "IBaseObject.h"
#include "Renderer.h"

// TODO: To be ultimately deleted - this array will be used to negate animations during asset integration
void IBaseObject::KillCurrFrame()
{
	memcpy(&m_vd3dCurrFrame[0], &CRenderer::GetInstance()->KillAnimation(), 4*16*15);
}

void IBaseObject::ChangeAnimation(int nAnimationIndex)
{
	TAnimation* newAnim = CAssetManager::GetInstance()->SwitchAnimation(nAnimationIndex, m_nObjectID);
		if(newAnim)
		{
			m_unCurrAnimation = nAnimationIndex;
			m_fAnimationTime = 0.0;
		}
		else
		{
			m_unCurrAnimation = 0;
		}

		TAnimation* readBones = CAssetManager::GetInstance()->GetAnimation(m_nObjectID, m_unCurrAnimation);
		m_vd3dCurrFrame.resize(15);
		for(unsigned int i = 0; i < readBones->m_nNumMeshes; ++i)
		{
			readBones->m_pBones[i].m_nCurrFrame = 0;
		}
}

Sphere IBaseObject::GetSphere()
{
	Sphere tSphere;
	tSphere.m_Center = *(D3DXVECTOR3*)&m_d3dMatrix[12];
	tSphere.m_d3dColor = m_vec4Color;
	tSphere.m_Radius = m_fRadius;

	return tSphere;
}

AABB IBaseObject::GetAABB()
{
	AABB aabb;
	aabb.m_d3dColor = m_vec4Color;
	aabb.max = D3DXVECTOR3(m_d3dMatrix._41 + m_fRadius, 1.0f, m_d3dMatrix._43 + m_fRadius);
	aabb.min = D3DXVECTOR3(m_d3dMatrix._41 - m_fRadius, 0.0f, m_d3dMatrix._43 - m_fRadius);
	return aabb;
}

void IBaseObject::Render()
{
	if(GetAlive())
		CRenderer::GetInstance()->RenderShadow(this);

	CRenderer::GetInstance()->Render(this);
}

void IBaseObject::AddToReactVector(IBaseObject* pObj)
{
	bool bInVector = false;

	for(unsigned int i = 0; i < m_vReactVector.size(); i++)
	{
		if(m_vReactVector[i] == pObj)
		{
			bInVector = true;
			break;
		}
	}

	if(!bInVector)
		m_vReactVector.push_back(pObj);
}

void IBaseObject::ClearReactVector()
{
	m_vReactVector.clear();
}