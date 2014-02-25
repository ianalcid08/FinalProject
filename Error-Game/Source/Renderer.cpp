#include <string>
#include "Renderer.h"
#include "Camera.h"
#include "Definitions.h"
#include "EnemyObject.h"
#include "PlayerObject.h"
#include "Bullet.h"
#include "GameplayState.h"
#include "Conveyor.h"
#include "Pickup.h"
#include "ExplodingBullet.h"
#include "BeamBullet.h"
#include "TeslaBullet.h"
#include "MoltenPool.h"
#include "ArcBullet.h"
#include "Turret.h"
#include "SmashBot.h"
#include "AICore.h"
#include "Game.h"

using namespace std;
/***********************************************
* Filename:  		Renderer.cpp
* Date:      		2/8/2013
* Mod. Date: 		4/23/2013
* Mod. Initials:	MB
* Author:    		Ian Alcid
* Purpose:   		Controls all rendering and DX objects
************************************************/

// init the static object
CRenderer CRenderer::m_Instance;

CRenderer::CRenderer(void)
{
	D3D_Object = NULL;
	D3D_Device = NULL;
	D3D_Sprite = NULL;
	D3D_Font = NULL;
	memset(&D3D_Params,0,sizeof(D3DPRESENT_PARAMETERS));
}


CRenderer::~CRenderer(void)
{
}

CRenderer* CRenderer::GetInstance(void)
{
	return &m_Instance;
}

bool CRenderer::InitD3D(HWND hWnd, int nScreenWidth,int nScreenHeight, bool bIsWindowed , bool bVsync )
{
	for(int i = 0; i < 15; ++i)
	{
		D3DXMatrixIdentity(&m_d3dKillAnimation[i]);
	}

	this->m_nWindowHeight = nScreenHeight;
	this->m_nWindowWidth = nScreenWidth;
	// Make sure the hWnd is valid.
	if (!hWnd) return false;

	// Set the handle to the window.
	m_hWnd = hWnd;

	// Create the Direct3D9 Object.
	D3D_Object = Direct3DCreate9(D3D_SDK_VERSION);

	// Polling device to see for good display modes
	PollResolution();

	
	//CGame::GetInstance()->SaveOptions("TestFiles/saveOptions.bin");
	CGame::GetInstance()->SaveVideoSettings("TestFiles/saveDisplayOptions.bin");
	CGame::GetInstance()->LoadVideoSettings("TestFiles/saveDisplayOptions.bin");
	D3DDISPLAYMODE asdf = D3D_CurrDisplay;
	int x = 0;
	// Setup the parameters for using Direct3D.
	D3D_Params.BackBufferWidth				= (bIsWindowed) ? 0 : D3D_CurrDisplay.Width;//m_nWindowWidth;//D3D_CurrDisplay.Width;
	D3D_Params.BackBufferHeight				= (bIsWindowed) ? 0 : D3D_CurrDisplay.Height;//m_nWindowHeight;// 
	D3D_Params.BackBufferFormat				= (bIsWindowed) ? D3DFMT_UNKNOWN :  D3DFMT_X8R8G8B8;
	D3D_Params.BackBufferCount				= 1;
	D3D_Params.MultiSampleType				= D3DMULTISAMPLE_NONE;
	D3D_Params.MultiSampleQuality			= 0;
	D3D_Params.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	D3D_Params.hDeviceWindow				= hWnd;
	D3D_Params.Windowed						= bIsWindowed;
	D3D_Params.EnableAutoDepthStencil		= true;
	D3D_Params.AutoDepthStencilFormat		= D3DFMT_D24S8;
	D3D_Params.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	D3D_Params.FullScreen_RefreshRateInHz	= (bIsWindowed) ? 0x00000000 : D3D_CurrDisplay.RefreshRate;//(bIsWindowed) ? 0x00000000 : 60;
	D3D_Params.PresentationInterval			= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the Direct3D Device.
	if (FAILED(D3D_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3D_Params, &D3D_Device)))
		MessageBox(m_hWnd,_T("Failed to create device scene."),_T("Failed to create device scene"),MB_OK);

	// Create Sprite Object.
	if (FAILED(D3DXCreateSprite(D3D_Device, &D3D_Sprite)))
		MessageBox(m_hWnd,_T("Failed to create sprite scene."),_T("Failed to create sprite scene"),MB_OK);
	//DXERROR(_T("Failed to Create the Sprite object"));

	D3D_Device->SetRenderState(D3DRS_ZENABLE, true);	
	D3D_Device->SetRenderState(D3DRS_LIGHTING, FALSE);			
	D3D_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Create the font object
	D3DXCreateFont(D3D_Device,20,0,FW_NORMAL,1,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH || FF_DONTCARE, L"Minecraftia",&D3D_Font);

	InitGraphics();

	m_pAssetManager = CAssetManager::GetInstance();

	SphereEffect = m_pAssetManager->LoadShader("TestFiles/flatshade.fx");
	CylinderEffect = m_pAssetManager->LoadShader("TestFiles/flatshade.fx");
	D3DXCreateSphere(D3D_Device, 1.0f, 25, 25, &SphereMesh, NULL);
	D3DXCreateCylinder(D3D_Device,0.5f,0.5f,60.0f,25,25,&CylinderMesh,NULL);

	//	Return success.
	return true;
}
void CRenderer::ShutdownD3D()
{
	if(m_d3dShadowBuffer)
		m_d3dShadowBuffer->Release();
	if(ringBuffer)
		ringBuffer->Release();
	if(SphereMesh)
		SphereMesh->Release();
	if(CylinderMesh)
		CylinderMesh->Release();

	D3D_Font->Release();
	D3D_Font = nullptr;
	D3D_Sprite->Release();
	D3D_Sprite = nullptr;
	D3D_Device->Release();
	D3D_Device = nullptr;
	D3D_Object->Release();
	D3D_Object = nullptr;
}
void CRenderer::Clear(unsigned char cRed, unsigned char cGreen , unsigned char cBlue)
{
	D3D_Device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(cRed, cGreen, cBlue), 1.0f, 0);
	D3D_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(cRed, cGreen, cBlue), 1.0f, 0);

	// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (D3D_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{

		D3D_Sprite->OnLostDevice();
		D3D_Device->Reset(&D3D_Params);
		D3D_Sprite->OnResetDevice();
	}
}

bool CRenderer::DeviceBegin()
{
	if (FAILED(D3D_Device->BeginScene()))
		MessageBox(m_hWnd,_T("Failed to begin device scene."),_T("Failed to begin device scene"),MB_OK);

	return true;
}
bool CRenderer::DeviceEnd()
{
	if (FAILED(D3D_Device->EndScene()))
		MessageBox(m_hWnd,_T("Failed to end device scene."),_T("Failed to end device scene"),MB_OK);

	return true;
}
bool CRenderer::SpriteBegin()
{
	if (FAILED(D3D_Sprite->Begin(D3DXSPRITE_ALPHABLEND)))
		MessageBox(m_hWnd,_T("Failed to begin sprite scene."),_T("Failed to begin sprite scene"),MB_OK);

	return true;
}
bool CRenderer::SpriteEnd()
{
	if (FAILED(D3D_Sprite->End()))
		MessageBox(m_hWnd,_T("Failed to end sprite scene."),_T("Failed to end sprite scene"),MB_OK);

	return true;
}

void CRenderer::Present()
{
	D3D_Device->Present(NULL, NULL, NULL, NULL);
}
void CRenderer::ChangeDisplayParam(int nWidth, int nHeight, bool bIsWindowed)
{
	// Set the new Presentation Parameters.
	D3D_Params.BackBufferFormat	= D3DFMT_X8R8G8B8;//(bIsWindowed) ? D3DFMT_A8R8G8B8 : D3DFMT_A8R8G8B8;
	D3D_Params.Windowed			= bIsWindowed;
	D3D_Params.BackBufferWidth	= nWidth;
	D3D_Params.BackBufferHeight	= nHeight;

	// Reset the device.
	D3D_Sprite->OnLostDevice();
	D3D_Device->Reset(&D3D_Params);
	D3D_Sprite->OnResetDevice();

	DWORD dwWindowStyleFlags = WS_VISIBLE;

	HWND top;
	if (bIsWindowed)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
		//ShowCursor(TRUE); // show the mouse cursor
		top = HWND_NOTOPMOST;
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		//ShowCursor(FALSE); // hide the mouse cursor
		top = HWND_TOP;
	}

	SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

	//	Set the window to the middle of the screen.
	if (bIsWindowed)
	{
		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nWidth;
		rWindow.bottom	= nHeight;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
		AdjustWindowRectEx(&rWindow, 
			dwWindowStyleFlags,
			FALSE, 
			WS_EX_APPWINDOW);

		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, top,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
			(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
			windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{

		// Let windows know the window has changed.
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

	}
}
void CRenderer::DrawRect(RECT rRect, unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	D3DRECT d3dRect;
	d3dRect.x1 = rRect.left;
	d3dRect.y1 = rRect.top;
	d3dRect.x2 = rRect.right;
	d3dRect.y2 = rRect.bottom;

	D3D_Device->Clear(1, &d3dRect, D3DCLEAR_TARGET, D3DCOLOR_XRGB(cRed, cGreen, cBlue), 1.0f, 0);
}
void CRenderer::InitGraphics()
{
	for(int i=0;i<4;i++)
		m_tShadow[i].d3dPos[1] = 0.1f;
	m_tShadow[0].d3dPos[0] =	-1.2f;	m_tShadow[0].d3dPos[2] =	1.2f;
	m_tShadow[1].d3dPos[0] =	1.2f;	m_tShadow[1].d3dPos[2] =	1.2f;
	m_tShadow[2].d3dPos[0] =	1.2f;	m_tShadow[2].d3dPos[2] =	-1.2f;
	m_tShadow[3].d3dPos[0] =	-1.2f;	m_tShadow[3].d3dPos[2] =	-1.2f;
	m_tShadow[0].d3dUV[0] =		0.0f;	m_tShadow[0].d3dUV[1] =	0.0f;
	m_tShadow[1].d3dUV[0] =		1.0f;	m_tShadow[1].d3dUV[1] =	0.0f;
	m_tShadow[2].d3dUV[0] =		1.0f;	m_tShadow[2].d3dUV[1] =	1.0f;
	m_tShadow[3].d3dUV[0] =		0.0f;	m_tShadow[3].d3dUV[1] =	1.0f;
	D3D_Device->CreateVertexBuffer(sizeof(TObjectAccentVert)*4,0,0,D3DPOOL_MANAGED,&m_d3dShadowBuffer,0);
	void* vram = nullptr;
	m_d3dShadowBuffer->Lock(0, 0, &vram, 0);
	memcpy(vram,m_tShadow,sizeof(TObjectAccentVert)*4);
	m_d3dShadowBuffer->Unlock();
	m_d3dShadowShader = CAssetManager::GetInstance()->LoadShader("TestFiles/RingsAndShadows.fx");
	m_d3dShadowTexture = CAssetManager::GetInstance()->LoadTexture("Textures/Shadow.png");

	for(int l=0;l<4;l++)
		ring[l].d3dPos[1]=0.1f;
	ring[0].d3dPos[0]	= -1.6f;	ring[0].d3dPos[2]	= 1.6f;
	ring[1].d3dPos[0]	= 1.6f;		ring[1].d3dPos[2]	= 1.6f;
	ring[2].d3dPos[0]	= 1.6f;		ring[2].d3dPos[2]	= -1.6f;
	ring[3].d3dPos[0]	= -1.6f;	ring[3].d3dPos[2]	= -1.6f;
	ring[0].d3dUV[0]	=0.0f;		ring[0].d3dUV[1]	=0.0f;
	ring[1].d3dUV[0]	=1.0f;		ring[1].d3dUV[1]	=0.0f;
	ring[2].d3dUV[0]	=1.0f;		ring[2].d3dUV[1]	=1.0f;
	ring[3].d3dUV[0]	=0.0f;		ring[3].d3dUV[1]	=1.0f;
	D3D_Device->CreateVertexBuffer(sizeof(TObjectAccentVert)*4,0,0,D3DPOOL_MANAGED,&ringBuffer,0);
	vram = nullptr;
	ringBuffer->Lock(0, 0, &vram, 0);
	memcpy(vram,ring,sizeof(TObjectAccentVert)*4);
	ringBuffer->Unlock();
}

void CRenderer::Render(IBaseObject *_object)
{
	if(!_object->GetIsActive())
		return;

	float randomNum = ((rand()/(float)RAND_MAX));

	CAssetManager* pAM = CAssetManager::GetInstance();
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();

	DWORD srcBlend,destBlend;
	if( _object->GetBeingHit() == 2 || _object->GetBeingHit() == 9)
	{
		D3D_Device->GetRenderState(D3DRS_SRCBLEND,&srcBlend);
		D3D_Device->GetRenderState(D3DRS_DESTBLEND,&destBlend);
		D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		D3D_Device->SetRenderState(D3DRS_SRCBLEND,srcBlend);
		D3D_Device->SetRenderState(D3DRS_DESTBLEND,destBlend);
	}

	D3D_Device->SetVertexDeclaration(m_pAssetManager->GetVertexDeclaration(BASEOBJECTDECL));
	ID3DXEffect* pShader = pAM->GetShader(_object->GetID());
	unsigned passes(0);
	pShader->Begin(&passes,0);
	for(unsigned i(0);i < passes; i++)
	{
		pShader->BeginPass(i);

		pShader->SetMatrixArray("gBones", _object->PassCurrFrame(), _object->GetCurrentAnimation()->m_nNumMeshes);
		pShader->SetTexture("gDiffuseTexture", m_pAssetManager->GetTexture(_object->GetID()));
		pShader->SetMatrix("gViewProjection", &(ViewProj));
		pShader->SetMatrix("gWorld",&(_object->GetMatrix()));
		pShader->SetInt("AmIHit",(_object->GetBeingHit()));
		pShader->SetFloat("randomNum",randomNum);

		D3DXVECTOR3 capture = *(D3DXVECTOR3*)&CGameplayState::GetInstance()->GetCamera()->GetCamera()[12];
		capture[2] += CAM_ZOFF;
		pShader->SetFloatArray("gLightDir", &capture[0],3);
		pShader->CommitChanges();

		D3D_Device->SetVertexDeclaration(m_pAssetManager->GetVertexDeclaration(BASEOBJECTDECL));
		D3D_Device->SetStreamSource(0, m_pAssetManager->GetVertBuffer(_object->GetID()), 0, sizeof(tVertex));
		D3D_Device->SetIndices(m_pAssetManager->GetIndexBuffer(_object->GetID()));
		D3D_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pAssetManager->GetNumVerts(_object->GetID()), 0, m_pAssetManager->GetNumTriangles(_object->GetID()));

		pShader->EndPass();
	}
	pShader->End();

	if( _object->GetBeingHit() == 2 || _object->GetBeingHit() == 9)
	{
		D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		D3D_Device->SetRenderState(D3DRS_SRCBLEND,srcBlend);
		D3D_Device->SetRenderState(D3DRS_DESTBLEND,destBlend);
	}

#ifdef _DEBUG
	RenderDebugMeshes(_object);
#endif
}

void CRenderer::RenderDebugMeshes(IBaseObject* _pObj)
{
	if(!_pObj->GetAlive())
		return;

	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();

	switch(_pObj->GetID())
	{
		//case OBJ_PLAYER:
		//	{
		//		D3DXVECTOR4 Color = ((CPlayerObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX PlayerMat;
		//		PlayerMat = _pObj->GetMatrix();
		//		float scale = ((CPlayerObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(PlayerMat,  ViewProj, Color, scale);
		//	}
		//	break;
		//case OBJ_TRASH:
		//	{
		//		D3DXVECTOR4 Color = ((CEnemyObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX TrashMat;
		//		TrashMat = _pObj->GetMatrix();
		//		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(TrashMat,  ViewProj, Color, scale);
		//	}
		//	break;
		//case OBJ_SMASH:
		//	{
		//		D3DXVECTOR4 Color = ((CEnemyObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX SmashMat;
		//		SmashMat = _pObj->GetMatrix();
		//		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(SmashMat, ViewProj, Color, scale);
		//	}
		//	break;
		//case OBJ_PATROL:
		//	{
		//		D3DXVECTOR4 Color = ((CEnemyObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX PatrolMat;
		//		PatrolMat = _pObj->GetMatrix();
		//		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(PatrolMat, ViewProj, Color, scale);
		//	}
		//	break;
		//case OBJ_BURN:
		//	{
		//		D3DXVECTOR4 Color = ((CEnemyObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX BurnMat;
		//		BurnMat = _pObj->GetMatrix();
		//		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(BurnMat, ViewProj, Color, scale);
		//	}
		//	break;
	case OBJ_STEAM:
		{
			D3DXVECTOR4 d3dColor =_pObj->GetColor();
			D3DXMATRIX d3dMat = _pObj->GetMatrix();
			float fScale = _pObj->GetRadius();
			RenderSphere(d3dMat, ViewProj, d3dColor, fScale);
		}
		break;
		/*case OBJ_AICORE:
		{
		if( !((CAICore*)_pObj)->GetCanBeHit() )
		{
		D3DXVECTOR4 Color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		D3DXMATRIX d3dMat = _pObj->GetMatrix();
		d3dMat._42 = 1.0f;
		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		RenderSphere(d3dMat, ViewProj, Color, scale);
		}
		}
		break;*/

		//case OBJ_AIFIST:
		//	{
		//		D3DXVECTOR4 Color = ((CEnemyObject*)_pObj)->GetSphere().m_d3dColor;
		//		D3DXMATRIX BurnMat;
		//		BurnMat = _pObj->GetMatrix();
		//		float scale = ((CEnemyObject*)_pObj)->GetSphere().m_Radius;
		//		RenderSphere(BurnMat, ViewProj, Color, scale);
		//	}
		//	break;
	case OBJ_BULLET_ENEMY:
		{
			D3DXVECTOR4 Color = ((CBullet*)_pObj)->GetSphere().m_d3dColor;
			D3DXMATRIX BulletMat;
			BulletMat = _pObj->GetMatrix();
			float scale = ((CBullet*)_pObj)->GetSphere().m_Radius;
			RenderSphere(BulletMat,  ViewProj, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), scale);
		}
		break;
	case OBJ_BULLET_PLAYER:
		{
			D3DXVECTOR4 Color = _pObj->GetColor();
			D3DXMATRIX PlayerMat;
			PlayerMat = _pObj->GetMatrix();
			float scale = ((CBullet*)_pObj)->GetSphere().m_Radius;
			RenderSphere(PlayerMat,  ViewProj, Color, scale);
		}
		break;
	case OBJ_BULLET_BEAM:
		{
			D3DXVECTOR4 Color = _pObj->GetColor();
			D3DXMATRIX PlayerMat;
			PlayerMat = _pObj->GetMatrix();
			float scale = ((CBeamBullet*)_pObj)->GetSphere().m_Radius; // + 0.5f;

			D3DXMATRIX tempTrans;
			D3DXVECTOR3 tempV = ((CBeamBullet*)_pObj)->GetVelocity();

			D3DXMatrixTranslation(&tempTrans,tempV.x * 29.0f,tempV.y,tempV.z * 29.0f);
			PlayerMat *= tempTrans;

			this->DrawBeamCylinder(PlayerMat,ViewProj,Color,scale,CylinderMesh);
		}
		break;
	case OBJ_BULLET_EXP:
		{
			D3DXVECTOR4 Color = ((CBullet*)_pObj)->GetSphere().m_d3dColor;
			D3DXMATRIX PlayerMat;
			PlayerMat = _pObj->GetMatrix();
			float scale = ((CExplodingBullet*)_pObj)->GetSphere().m_Radius;
			RenderSphere(PlayerMat,  ViewProj, Color, scale);
		}
		break;
	case OBJ_BULLET_TESLA:
		{
			D3DXMATRIX d3dBalls[2] = { ((CTeslaBullet*)_pObj)->GetFL(), ((CTeslaBullet*)_pObj)->GetFR() };
			RenderSphere(d3dBalls[0], ViewProj, _pObj->GetColor(), .2f);
			RenderSphere(d3dBalls[1], ViewProj, _pObj->GetColor(), .2f);
		}
		break;
	case OBJ_BULLET_ARC:
		{
			D3DXVECTOR4 Color = ((CBullet*)_pObj)->GetSphere().m_d3dColor;
			D3DXMATRIX PlayerMat = _pObj->GetMatrix();
			float scale = ((CExplodingBullet*)_pObj)->GetSphere().m_Radius;
			RenderSphere(PlayerMat,  ViewProj, Color, scale);
		}
		break;
		//case OBJ_BARREL:
		//	{
		//		RenderSphere(_pObj->GetMatrix(),ViewProj,D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),0.8f);
		//	}
		//	break;
		//case OBJ_VAT:
		//	{
		//		RenderSphere(_pObj->GetMatrix(),ViewProj, _pObj->GetColor(), 1.0f);
		//	}
		//	break;

		//case OBJ_CRATE:
		//	{
		//		RenderSphere(_pObj->GetMatrix(),ViewProj,D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),0.8f);

		//	}
		//	break;
		//case OBJ_POOL:
		//	{
		//		RenderSphere(_pObj->GetMatrix(),ViewProj, MOLTEN_COLOR, 1.0f);
		//	}
		//	break;
		//case OBJ_TURRET:
		//	{
		//		RenderSphere(_pObj->GetMatrix(),ViewProj,D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),TURRET_RADIUS);
		//		RenderSphere(((CTurret*)_pObj)->GetNextMatrix(),ViewProj,D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),TURRET_RADIUS);
		//	}
		//	break;
	default:
		break;
	}
}

void CRenderer::RenderShadow(IBaseObject* _pObj)
{
	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	D3D_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	unsigned passes(0);
	m_d3dShadowShader->Begin(&passes,0);
	for(unsigned pass(0);pass < passes; pass++)
	{
		m_d3dShadowShader->BeginPass(pass);
		D3DXMATRIX d3dScale;
		D3DXMATRIX d3dObjectMat = _pObj->GetMatrix();
		float scale = _pObj->GetRadius();
		if(_pObj->GetID() == OBJ_BULLET_ARC)
			scale *= 2.5f - min(1.25f,(d3dObjectMat._42 * 0.1f));
		D3DXMatrixScaling(&d3dScale,scale,scale,scale);
		d3dObjectMat[13] = 0.0f;
		m_d3dShadowShader->SetMatrix("gWorld", &(d3dScale*d3dObjectMat));
		m_d3dShadowShader->SetMatrix("gViewProjection", &CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix());
		m_d3dShadowShader->SetVector("gColor", &D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f));
		m_d3dShadowShader->SetTexture("gDiffuseTexture", m_d3dShadowTexture);
		m_d3dShadowShader->CommitChanges();

		D3D_Device->SetVertexDeclaration(m_pAssetManager->GetVertexDeclaration(POS_TEXCOORD));
		D3D_Device->SetStreamSource(0, m_d3dShadowBuffer, 0, sizeof(TObjectAccentVert));

		D3D_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		m_d3dShadowShader->EndPass();
	}
	m_d3dShadowShader->End();
	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CRenderer::RenderPlayerRing(CPlayerObject* _pObj)
{
	D3DXMATRIX ViewProj = CGameplayState::GetInstance()->GetCamera()->GetViewProjMatrix();
	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	D3DXMATRIX adjustMat, useMat;
	D3DXMatrixIdentity(&useMat);
	adjustMat = _pObj->GetMatrix();
	*(D3DXVECTOR3*)&useMat[12] = *(D3DXVECTOR3*)&adjustMat[12];
	unsigned int passes = 0;

	passes = 0;
	m_d3dShadowShader->Begin(&passes,0);
	for(unsigned pass(0);pass < passes; pass++)
	{
		m_d3dShadowShader->BeginPass(pass);

		m_d3dShadowShader->SetMatrix("gWorld", &(useMat));
		m_d3dShadowShader->SetVector("gColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		m_d3dShadowShader->SetTexture("gDiffuseTexture", m_pAssetManager->LoadTexture("Assets/Textures/PlayerRing.png"));
		m_d3dShadowShader->SetMatrix("gViewProjection", &ViewProj);
		m_d3dShadowShader->CommitChanges();

		D3D_Device->SetVertexDeclaration(m_pAssetManager->GetVertexDeclaration(POS_TEXCOORD));
		D3D_Device->SetStreamSource(0, ringBuffer, 0, sizeof(TObjectAccentVert));
		D3D_Device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		m_d3dShadowShader->EndPass();
	}
	m_d3dShadowShader->End();

	D3D_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}

void CRenderer::RenderSphere(D3DXMATRIX ObjectsMatrix, D3DXMATRIX ViewProj, D3DXVECTOR4 Color, float Scale)
{
	D3DXMATRIX tempScaleMat;
	D3DXMatrixScaling(&tempScaleMat, Scale, Scale, Scale);
	D3DXMatrixMultiply(&ObjectsMatrix, &tempScaleMat, &ObjectsMatrix);
	D3DXMATRIX WorldViewProj = (ObjectsMatrix * ViewProj);


	unsigned passes(0);
	SphereEffect->Begin(&passes, 0);
	for(unsigned i(0); i<passes; ++i)
	{
		SphereEffect->BeginPass(i);

		SphereEffect->SetMatrix("gWorldViewProjection", &WorldViewProj);
		SphereEffect->SetVector("gColor", &Color);
		SphereEffect->CommitChanges();

		SphereMesh->DrawSubset(0);

		SphereEffect->EndPass();
	}
	SphereEffect->End();
}

void CRenderer::DrawBeamCylinder(D3DXMATRIX _objectsMatrix,D3DXMATRIX _ViewProj,D3DXVECTOR4 _Color,float _scale,LPD3DXMESH _cylinder)
{
	D3DXMATRIX tempScaleMat;
	D3DXMatrixScaling(&tempScaleMat, _scale, _scale, _scale);
	D3DXMatrixMultiply(&_objectsMatrix, &tempScaleMat, &_objectsMatrix);
	D3DXMATRIX WorldViewProj = (_objectsMatrix * _ViewProj);

	unsigned passes(0);
	CylinderEffect->Begin(&passes, 0);
	for(unsigned i(0); i<passes; ++i)
	{
		CylinderEffect->BeginPass(i);

		CylinderEffect->SetMatrix("gWorldViewProjection", &WorldViewProj);
		CylinderEffect->SetVector("gColor", &_Color);
		CylinderEffect->CommitChanges();
		// render data
		_cylinder->DrawSubset(0);

		CylinderEffect->EndPass();
	}
	CylinderEffect->End();
}

void	CRenderer::DrawBackground(IDirect3DTexture9* backgroundBMP, int nX, int nY, float fScaleX, float fScaleY,
	RECT* pSection, float fRotCenterX, float fRotCenterY, float fRotation, DWORD dwColor)
{
	D3DXMATRIX scale;
	D3DXMATRIX rotation;
	D3DXMATRIX translate;
	D3DXMATRIX combined;

	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&combined);

	// Scale the sprite.
	D3DXMatrixScaling(&scale, fScaleX, fScaleY, 1.0f);
	combined *= scale;

	// Rotate the sprite.
	D3DXMatrixTranslation(&translate, -fRotCenterX * fScaleX, -fRotCenterY * fScaleY, 0.0f);
	combined *= translate;
	D3DXMatrixRotationZ(&rotation, fRotation);
	combined *= rotation;
	D3DXMatrixTranslation(&translate, fRotCenterX * fScaleX, fRotCenterY * fScaleY, 0.0f);
	combined *= translate;

	// Translate the sprite
	D3DXMatrixTranslation(&translate, (float)nX, (float)nY, 0.0f);
	combined *= translate;

	// Apply the transform.
	GetSprite()->SetTransform(&combined);



	// Draw the sprite.
	GetSprite()->Draw(backgroundBMP, pSection, NULL, NULL, dwColor);


	// Move the world back to identity.
	D3DXMatrixIdentity(&combined);
	GetSprite()->SetTransform(&combined);
}
void CRenderer::PollResolution(void)
{
	D3DDISPLAYMODE tempmode;
//	UINT numAdapter = D3D_Object->GetAdapterCount();
	UINT numModes = D3D_Object->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
	//D3D_Object->EnumAdapterModes( D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8,0,&tempmode);
	vector<D3DDISPLAYMODE> allmodes;
	for(UINT i = 0; i < numModes; i++)
	{
		D3D_Object->EnumAdapterModes(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8,i,&tempmode);
		allmodes.push_back(tempmode);
	}

	for(unsigned int i = 0; i < allmodes.size();i++)
	{
		if(allmodes[i].RefreshRate == 60)
		{
			if(allmodes[i].Width >= 1024 && allmodes[i].Height >= 768)
				D3D_DisplayModes.push_back(allmodes[i]);
		}
	}
	
}
