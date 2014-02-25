#ifndef _ANIMATIONPROCESSOR_H_
#define _ANIMATIONPROCESSOR_H_

/***********************************************
 * Filename:  		AnimationProcessor.h
 * Date:      		02/09/2013
 * Mod. Date: 		02/13/2013
 * Mod. Initials:	SD
 * Author:    		Joseph Thomas
 * Purpose:   		Handles all articulated animations
 ************************************************/

#include <d3dx9math.h>

struct TKeyFrame
{
	D3DXMATRIX m_d3dTransform;
	float m_fDuration;
};

struct TBone
{
	TKeyFrame* m_pKeyFrames;
	unsigned int m_nCurrFrame;
	unsigned int m_nNumFrames;
};

struct TAnimation
{
	TBone* m_pBones;
	float m_fDuration;
	unsigned int m_nNumMeshes;
};

class CAnimationProcessor
{
public:

	/*****************************************************************
	* void Interpolator(TMesh* _pMesh, TAnim* _pAnim, TKeyFrame& _tCurFrame): 
	* Interpolates between two Matrices based on Lambda time
	*
	* Ins:			    TAnimation* pAnim
	*					float fCurrDuration
	*					D3DXMATRIX* pCurrFrame
	*
	* Outs:				D3DXMATRIX* pCurrFrame
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	SD
	*****************************************************************/
	static void Interpolator(TAnimation* pAnim, float fCurrDuration, D3DXMATRIX* pCurrFrame);

	/*****************************************************************
	* void UpdateTime(TAnimation* pAnim, float& fCurrTime): 
	* Checks CurrTime vs Animation Duration and changes fCurrTime
	* if it's greater than Animation Duration
	*
	* Ins:			    TMesh* _d3dPrevFrame
	*					TAnim* _pAnim
	*
	* Outs:				TKeyFrame& _tCurFrame
	*					D3DXMATRIX* pCurrFrame
	*
	* Returns:		    void
	*
	* Mod. Date:		02/13/2013
	* Mod. Initials:	SD
	*****************************************************************/
	static void UpdateTime(TAnimation* pAnim, float& fCurrTime);
};
#endif