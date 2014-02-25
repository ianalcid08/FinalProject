#ifndef CAMERA_H
#define CAMERA_H
/***********************************************
 * Filename:  		CCamera.h
 * Date:      		2/8/2013
 * Mod. Date: 		2/8/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Controls what you can see
 ************************************************/
#include <d3dx9math.h>


class CCamera
{
public:
	
	/*****************
	*Accessors
	*****************/
	D3DXDMATRIX GetCamera() {return m_D3DCamera;}

	static CCamera* GetInstance();

private:
	// singleton stuff
	
	CCamera(void);
	~CCamera(void);
	static CCamera m_Instance;

	D3DXMATRIX m_D3DCamera;
};

#endif