#include "AnimationProcessor.h"

/***********************************************
 * Filename:  		AnimationProcessor.cpp
 * Date:      		02/09/2013
 * Mod. Date: 		02/13/2013
 * Mod. Initials:	JT
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all articulated animations
 ************************************************/

/*****************************************************************
* D3DXMATRIX Interpolate(D3DXMATRIX _d3dPrevFrame, D3DXMATRIX _d3dNextFrame, float _fLambda): 
* Interpolates between two Matrices based on Lambda time
*
* Ins:			    D3DXMATRIX _d3dPrevFrame
*					D3DXMATRIX _d3dNextFrame
*					float _fLambda
*
* Outs:				void
*
* Returns:		    D3DXMATRIX
*
* Mod. Date:		02/20/2013
* Mod. Initials:	SD
*****************************************************************/
D3DXMATRIX Interpolate(D3DXMATRIX& d3dMatrixA, D3DXMATRIX& d3dMatrixB, float fLambda)
{
	D3DXQUATERNION quatA, quatB, tempQuat;
	D3DXQuaternionRotationMatrix(&quatA, &d3dMatrixA);
	D3DXQuaternionRotationMatrix(&quatB, &d3dMatrixB);

	tempQuat = quatA;
	D3DXQuaternionNormalize(&quatA, &tempQuat);
	tempQuat = quatB;
	D3DXQuaternionNormalize(&quatB, &tempQuat);
	D3DXQuaternionSlerp(&tempQuat, &quatA, &quatB, fLambda);
	D3DXMATRIX result;
	D3DXMatrixRotationQuaternion(&result, &tempQuat);
	D3DXVec3Lerp((D3DXVECTOR3*)&result[12], (D3DXVECTOR3*)&d3dMatrixA[12], (D3DXVECTOR3*)&d3dMatrixB[12], fLambda);
	D3DXVECTOR3 scaleA = D3DXVECTOR3(d3dMatrixA[0],d3dMatrixA[5],d3dMatrixA[10]);
	D3DXVECTOR3 scaleB = D3DXVECTOR3(d3dMatrixB[0],d3dMatrixB[5],d3dMatrixB[10]);
	D3DXVECTOR3 finalScale;
	D3DXVec3Lerp(&finalScale, &scaleA, &scaleB, fLambda);
	result[0] = finalScale[0];
	result[5] = finalScale[1];
	result[10] = finalScale[2];
	return result;
}


/*****************************************************************
* unsigned int LinearSearch() 
* Helper Function for returning the CurrentFrame from each Bone
*
* Ins:			    TBone* tBone
*					float fCurrTime
*
* Outs:				void
*
* Returns:		    unsigned int
*
* Mod. Date:		02/20/2013
* Mod. Initials:	SD
*****************************************************************/
unsigned int LinearSearch(TBone* tBone, float fCurrTime)
{
	unsigned int index = 1;

	for(; index < tBone->m_nNumFrames; ++index)
	{
		if(tBone->m_pKeyFrames[index].m_fDuration > fCurrTime)
			break;
	}

	return index-1;
}

void CAnimationProcessor::Interpolator(TAnimation* pAnim, float fCurrDuration, D3DXMATRIX* pCurrFrame)
{
	unsigned int LastFrame = 0, nextFrame = 0;

	for(unsigned int currBone = 0; currBone < pAnim->m_nNumMeshes; ++currBone)
	{
		LastFrame = LinearSearch(pAnim->m_pBones + currBone, fCurrDuration);
		if(LastFrame != pAnim->m_pBones[currBone].m_nNumFrames-1)
			nextFrame = LastFrame + 1;

		// Find Lambda value
		float tween = 0;
		float fromlast = fCurrDuration - pAnim->m_pBones[currBone].m_pKeyFrames[LastFrame].m_fDuration;
		if(nextFrame == 0)
			tween = pAnim->m_fDuration - pAnim->m_pBones[currBone].m_pKeyFrames[LastFrame].m_fDuration;
		else
			tween = pAnim->m_pBones[currBone].m_pKeyFrames[nextFrame].m_fDuration - pAnim->m_pBones[currBone].m_pKeyFrames[LastFrame].m_fDuration;
		float lambda = fromlast / tween;
		
		pCurrFrame[currBone] = Interpolate(pAnim->m_pBones[currBone].m_pKeyFrames[LastFrame].m_d3dTransform, pAnim->m_pBones[currBone].m_pKeyFrames[nextFrame].m_d3dTransform, lambda);
	}
}

void CAnimationProcessor::UpdateTime(TAnimation* pAnim, float& fCurrTime)
{
	if(fCurrTime > pAnim->m_fDuration)
		fCurrTime -= pAnim->m_fDuration;
}