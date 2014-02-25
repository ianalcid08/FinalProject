#ifndef CBITMAPFONT_H_
#define CBITMAPFONT_H_
/***********************************************
 * Filename:  		BitmapFont.h
 * Date:      		2/12/2013
 * Mod. Date: 		3/7/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Bitmap Font for screen
 ************************************************/
#include "Renderer.h"



class CGame;
class CAssetManager;

class CBitmapFont
{
private:
	CBitmapFont(void);
	~CBitmapFont(void);
	static CBitmapFont	m_Instance;

	
	CAssetManager		*m_pAM;
	CRenderer			*m_pRenderer;
	IDirect3DTexture9	*m_d3dFontSheet;

	//Cell Algorithm
	int m_nChar_Width;
	int m_nChar_Height;
	int m_nNumCols;
	int m_nNumRows;
	char m_cFirstChar;			// First character in the image
	bool m_bOnlyUppercase;		// Only uppercase letters in the image
public:
	static CBitmapFont* GetInstance();
	/*****************************************************************
	* void InitFont(): Loads the bitmap font sheet
	*
	* Ins:			    void
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void InitFont();
	/*****************************************************************
	* void PrintBF():  Draws strings to screen using the bitmap sheet
	*
	* Ins:			    const char* szText
						int nX 
						int nY 
						float fScale
						DWORD dwColor = D3DCOLOR_XRGB(255,255,255)
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void PrintBF( const char* szText , int nX, int nY, float fScale, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));
	/*****************************************************************
	* void CellAlgorithm():  Based on char finds desired rect on bitmap sheet
	*
	* Ins:			   unsigned int id
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	RECT CellAlgorithm ( unsigned int id);
	/*****************************************************************
	* void DrawTexture():  Draws per character
	*
	* Ins:			    IDirect3DTexture9* backgroundBMP
						int nX, 
						int nY, 
						float fScaleX, 
						float fScaleY,
						RECT* pSection = NULL, 
						float fRotCenterX = 0.0f, 
						float fRotCenterY = 0.0f, 
						float fRotation = 0.0f, 
						DWORD dwColor = 0xFFFFFFFF);
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		03/8/2013
	* Mod. Initials:	IA
	*****************************************************************/
	void	DrawTexture(IDirect3DTexture9* backgroundBMP, int nX, int nY, float fScaleX, float fScaleY,
	RECT* pSection = NULL, float fRotCenterX = 0.0f, float fRotCenterY = 0.0f, float fRotation = 0.0f, DWORD dwColor = 0xFFFFFFFF);
};

#endif