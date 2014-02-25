
/***********************************************
 * Filename:  		BitmapFont.cpp
 * Date:      		2/12/2013
 * Mod. Date: 		3/8/2013
 * Mod. Initials:	IA
 * Author:    		Ian Alcid
 * Purpose:   		Bitmap font of the screen
 ************************************************/
#include "BitmapFont.h"
#include "AssetManager.h"
#include "Game.h"

CBitmapFont CBitmapFont::m_Instance;

CBitmapFont::CBitmapFont(void)
{
	m_pAM = CAssetManager::GetInstance();
	m_pRenderer = CRenderer::GetInstance();
}


CBitmapFont::~CBitmapFont(void)
{

}
CBitmapFont* CBitmapFont::GetInstance(void)
{
	return &m_Instance;
}

void CBitmapFont::InitFont()
{
	m_nChar_Width = 64;
	 m_nChar_Height = 64;
	 m_nNumCols = 16;
	 m_nNumRows = 8;

	m_d3dFontSheet = m_pAM->LoadTexture("Textures/Font.png");
	m_bOnlyUppercase = true;
	m_cFirstChar = 32;
}
void CBitmapFont::PrintBF( const char* szText , int nX, int nY, float fScale, DWORD dwColor)
{
	// Keep track of the starting X position (for new lines)
		int nColStart = nX;

		// Iterate through the string 1 character at a time
		for(int i = 0; szText[i] != '\0'; i++)
		{
			// Get the char out of the string
			char ch = szText[i];
			if(m_bOnlyUppercase == true)
				ch = toupper( ch);

			// Check for whitespace
			if ( ch == ' ')
			{
				// move the x over
				nX += int(m_nChar_Width * fScale);
				continue;
			}
			else if( ch == '\n')
			{
				// move the y down
				nY += int(m_nChar_Height * fScale);
				nX = nColStart;
				continue;
			}
			// Calculate the tile id based on the ascii value
			int id = ch;// - m_cFirstChar;

			// Get the source rect
			RECT rTile = CellAlgorithm( id);

			// draw
			//CSGD_TextureManager::GetInstance()->Draw(m_nImageID,nX,nY,fScale,fScale,&rTile,0,0,0,D3DCOLOR_XRGB(255,255,255));
			DrawTexture(this->m_d3dFontSheet,nX,nY,fScale,fScale,&rTile,0.0f,0.0f,0.0f,dwColor);
			// move position to the next char
			nX += int(m_nChar_Width * fScale);
		}
}
RECT CBitmapFont::CellAlgorithm ( unsigned int id)
{
	RECT rCell = {};

	rCell.left	= (id % m_nNumCols) * m_nChar_Width;
	rCell.top	= (id / m_nNumCols) * m_nChar_Height;
	rCell.right = rCell.left + m_nChar_Width;
	rCell.bottom = rCell.top + m_nChar_Height;

	return rCell;
}
void CBitmapFont::DrawTexture(IDirect3DTexture9* backgroundBMP, int nX, int nY, float fScaleX, float fScaleY,
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
	m_pRenderer->GetSprite()->SetTransform(&combined);

	// Draw the sprite.
	m_pRenderer->GetSprite()->Draw(backgroundBMP, pSection, NULL, NULL, dwColor);
		

	// Move the world back to identity.
	D3DXMatrixIdentity(&combined);
	m_pRenderer->GetSprite()->SetTransform(&combined);
}