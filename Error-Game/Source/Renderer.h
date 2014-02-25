#ifndef RENDERER_H
#define RENDERER_H
/***********************************************
 * Filename:  		Renderer.h
 * Date:      		2/8/2013
 * Mod. Date: 		4/23/2013
 * Mod. Initials:	MB
 * Author:    		Ian Alcid
 * Purpose:   		Controls all rendering and DX objects
 ************************************************/

// The include files for Direct3D9
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

// The library files for Direct3D9
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#include "AssetManager.h"
#include "IBaseObject.h"
#include "CollisionLibrary.h"
#include <vector>

class CPlayerObject;

struct tVertex
{
	D3DXVECTOR3 m_d3dpos;		// pos
	D3DXVECTOR3 m_d3dnorm;		// normals
	D3DXVECTOR2 m_d3duv;			// UVs
	unsigned int m_unbone;
};

struct tTri
{
	int one, two, three;
};

class CRenderer
{
private:
	LPDIRECT3D9				D3D_Object;
	LPDIRECT3DDEVICE9		D3D_Device;
	LPD3DXSPRITE			D3D_Sprite;
	LPD3DXFONT				D3D_Font;
	D3DPRESENT_PARAMETERS   D3D_Params;
	HWND					m_hWnd;
	D3DSURFACE_DESC			D3D_Backbuffer;
	vector<D3DDISPLAYMODE>			D3D_DisplayModes;
	//D3DDISPLAYMODE			D3D_DisplayModes[4];
	D3DDISPLAYMODE			D3D_BestDisplay;
	D3DDISPLAYMODE			D3D_CurrDisplay;
	// 800 x 600
	// 1024 x 768
	// 1280 x 720
	// 1920 x 1080

	CAssetManager			*m_pAssetManager;
	int						m_nWindowWidth;
	int						m_nWindowHeight;

	ID3DXEffect*					SphereEffect;
	LPD3DXMESH						SphereMesh;

	ID3DXEffect*					CylinderEffect;
	LPD3DXMESH						CylinderMesh;

	// TODO: To be ultimately deleted - this array will be used to negate animations during asset integration
	D3DXMATRIX						m_d3dKillAnimation[15];

	// singleton stuff
	static CRenderer		m_Instance;
	CRenderer(void);
	~CRenderer(void);
	CRenderer(CRenderer &ref);
	CRenderer &operator=(CRenderer &ref);

	ID3DXEffect*					m_d3dShadowShader;
	IDirect3DVertexDeclaration9*	m_d3dShadowDecl;
	IDirect3DVertexBuffer9*			m_d3dShadowBuffer;
	IDirect3DTexture9*				m_d3dShadowTexture;
	struct TObjectAccentVert
	{
		D3DXVECTOR3 d3dPos;
		D3DXVECTOR2 d3dUV;
	};
	TObjectAccentVert m_tShadow[4];
	IDirect3DVertexBuffer9* ringBuffer;
	TObjectAccentVert ring[4];
	IDirect3DVertexBuffer9* InvulRingBuffer;
	TObjectAccentVert InvulRing[4];
		
public:
	static CRenderer* GetInstance();

	// TODO: To be ultimately deleted - this array will be used to negate animations during asset integration
	D3DXMATRIX& KillAnimation() {return m_d3dKillAnimation[0];}

	/*****************
	// Accessors
	*****************/
	LPDIRECT3D9					CRenderer::GetDirect3DObject(void)			{ return D3D_Object; }
	LPDIRECT3DDEVICE9			CRenderer::GetDirect3DDevice(void)			{ return D3D_Device; }
	LPD3DXSPRITE				CRenderer::GetSprite(void)					{ return D3D_Sprite; }
	LPD3DXFONT					CRenderer::GetFont(void)					{ return D3D_Font; }
	D3DSURFACE_DESC				CRenderer::GetBackBuffer(void)				{ return D3D_Backbuffer; }
	const D3DPRESENT_PARAMETERS*CRenderer::GetPresentParams(void)			{ return &D3D_Params; }
	int							CRenderer::GetWindowWidth()					{return m_nWindowWidth;}
	int							CRenderer::GetWindowHeight()				{return m_nWindowHeight;}
	D3DDISPLAYMODE				CRenderer::GetCurrDisplayMode()				{return D3D_CurrDisplay;}
	D3DDISPLAYMODE				CRenderer::GetBestDisplayMode()				{return D3D_BestDisplay;}
	D3DDISPLAYMODE				CRenderer::GetDisplayMode(int _index)		{return D3D_DisplayModes[_index];}
	vector<D3DDISPLAYMODE>		CRenderer::GetDisplayVector()				{return D3D_DisplayModes;}

	void						CRenderer::SetCurrDisplayModeIndex(int index)	{ D3D_CurrDisplay = D3D_DisplayModes[index];}
	void						CRenderer::SetCurrDisplayMode(D3DDISPLAYMODE _d3ddm)	{ D3D_CurrDisplay = _d3ddm;}
	//D3DDISPLAYMODE				CRenderer::SetBestDisplayMode()				{return D3D_BestDisplay;}
	//D3DDISPLAYMODE				CRenderer::SetDisplayMode(int _index)		{return D3D_DisplayModes[_index];}

	/*****************************************************************
	 * DrawBeamCylinder:	Renders the Pulse Laser Cylinder
	 *
	 * Ins:					D3DXMATRIX _objectsMatrix,D3DXMATRIX _ViewProj,D3DXVECTOR4 _Color,float _scale,LPD3DXMESH _cylinder
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void DrawBeamCylinder(D3DXMATRIX _objectsMatrix,D3DXMATRIX _ViewProj,D3DXVECTOR4 _Color,float _scale,LPD3DXMESH _cylinder);

	/*****************************************************************
	 * InitD3D:				Initializes Directx object and device.
	 *
	 * Ins:					HWND hWnd, int nScreenWidth,int nScreenHeight, bool bIsWindowed, bool bVsync
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool InitD3D(HWND hWnd, int nScreenWidth,int nScreenHeight, bool bIsWindowed = true, bool bVsync = true);

	/*****************************************************************
	 * ShutdownD3D:			Shutsdown directx, release any objects created and sets to nullptr
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void ShutdownD3D();

	/*****************************************************************
	 * Clear:				Clears screen to the color passed in.
	 *
	 * Ins:					unsigned char cRed, unsigned char cGreen, unsigned char cBlue
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void Clear(unsigned char cRed = 0, unsigned char cGreen = 0, unsigned char cBlue = 0);

	/*****************************************************************
	 * DeviceBegin:			Begins the device scene
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool DeviceBegin();

	/*****************************************************************
	 * DeviceEnd:			Ends the device scene
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool DeviceEnd();

	/*****************************************************************
	 * SpriteBegin:			Begins the sprite scene
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool SpriteBegin();

	/*****************************************************************
	 * SPriteEnd:			Ends the sprite scene
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	bool SpriteEnd();

	/*****************************************************************
	 * Present:				Presents everything in the backbuffer to the screen
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				bool
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void Present();

	/*****************************************************************
	 * ChangeDisplayParam:	Changes the directx params to match the given resolution and fullscreen option.
	 *
	 * Ins:					int nWidth, int nHeight, bool bIsWindowed
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void ChangeDisplayParam(int nWidth, int nHeight, bool bIsWindowed);

	/*****************************************************************
	 * DrawRect:			Draws a rect with specified dimensions and color
	 *
	 * Ins:					RECT rRect, unsigned char cRed, unsigned char cGreen, unsigned char cBlue
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void DrawRect(RECT rRect, unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

	/*****************************************************************
	 * InitGraphics:		Creates objects that are not held in the object manager
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void InitGraphics();
	/*****************************************************************
	 * Render:				Takes all the information needed from the asset manager and puts it out onto the screen.
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/12/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void Render(IBaseObject *_object);
	
	
	/*****************************************************************
	 * RenderSphere:		Renders a Sphere
	 *
	 * Ins:					D3DXMATRIX ObjectsMatrix, D3DXMATRIX ViewProj, D3DXVECTOR4 Color, float Scale
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			02/15/2013
	 * Mod. Initials:		MB
	 *****************************************************************/
	void RenderSphere(D3DXMATRIX ObjectsMatrix, D3DXMATRIX ViewProj, D3DXVECTOR4 Color, float Scale);

	/*****************************************************************
	* void DrawBackground(): Draws background
	*
	* Ins:	backgroundBMP	-	The id of the texture to draw.		    
	*		nX				-	The x position to draw the texture at.
	*		nY				-	The y position to draw the texture at.
	*		fScaleX			-	How much to scale the texture in the x.
	*		fScaleY			-	How much to scale the texture in the y.
	*		pSection		-	The section of the bitmap to draw.
	*		fRotCenterX		-	The x center to apply the rotation from.
	*		fRotCenterY		-	The y center to apply the rotation from.
	*		fRotation		-	How much to rotate the texture.
	*		dwColor			-	The color to apply to the texture (use D3DCOLOR_XRGB() Macro).
	*
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		3/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	DrawBackground(IDirect3DTexture9* backgroundBMP, int nX, int nY, float fScaleX, float fScaleY,
	RECT* pSection = NULL, float fRotCenterX = 0.0f, float fRotCenterY = 0.0f, float fRotation = 0.0f, DWORD dwColor = D3DCOLOR_RGBA(255,255,255,255));

	/*****************************************************************
	 * RenderShadow:		Render's and Object's drop shadow
	 *
	 * Ins:					IBaseObject* _pObj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void RenderShadow(IBaseObject* _pObj);

	/*****************************************************************
	 * RenderPlayerRing:		Render's a player Color Ring
	 *
	 * Ins:					CPlayerObject* _pObj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void RenderPlayerRing(CPlayerObject* _pObj);

	/*****************************************************************
	 * RenderDebugMeshes:		Render's debug meshes
	 *
	 * Ins:					IBaseObject* _pObj
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/10/2013
	 * Mod. Initials:		SD
	 *****************************************************************/
	void RenderDebugMeshes(IBaseObject* _pObj);
	/*****************************************************************
	 * PollResolution:		Polls device for supported resolutions/refresh rate
	 *
	 * Ins:					void
	 *
	 * Outs:				void
	 *
	 * Returns:				void
	 *
	 * Mod. Date:			04/23/2013
	 * Mod. Initials:		IA
	 *****************************************************************/
	void PollResolution(void);
};

#endif