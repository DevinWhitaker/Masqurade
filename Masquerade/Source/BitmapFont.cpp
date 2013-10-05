/*
	File:		BitmapFont.cpp
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Contain Bitmap Font Related Code
*/
#include "BitmapFont.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

// SINGLETON!
CBitmapFont* CBitmapFont::GetInstance( void )
{
	// Lazy instantiation
	static CBitmapFont s_Instance;

	return &s_Instance;
}

CBitmapFont::CBitmapFont(void)
{
	m_nImageID = -1;

	m_nCharWidth	= 1;
	m_nCharHeight	= 1;
	m_nNumRows		= 1;
	m_nNumCols		= 1;
	
	m_cFirstChar	= ' ';
}


CBitmapFont::~CBitmapFont(void)
{
	
}

void CBitmapFont::Init( TCHAR* filename, int width, int height, 
				int rows, int cols, char firstchar )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	m_nImageID		= pTM->LoadTexture( filename );

	m_nCharWidth	= width;
	m_nCharHeight	= height;
	m_nNumRows		= rows;
	m_nNumCols		= cols;
	
	m_cFirstChar	= firstchar;
}

void CBitmapFont::Shutdown(void)
{
	if( m_nImageID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageID);
		m_nImageID = -1;
	}
}


void CBitmapFont::Print( const char* szText, int nX, int nY,
			float fScale, DWORD dwColor, int nOffset )
{
	// Keep track of the starting x position (handle newlines)
	int nColStart = nX;
	static bool bSpacing = false;
	// Iterate through the text string 1 char at a time
	int nLen = strlen( szText );

	for( int i = 0; i < nLen; ++i )
	{
		// Get the char out of the string
		char ch = toupper(szText[i]);

		// Adjust for whitespace:
		if( ch == ' ' )
		{
			nX += (int)( ( m_nCharWidth - nOffset) * fScale);
			continue;
		}
		else if( ch == '\n' )
		{
			nY += (int)(m_nCharHeight *fScale);
			nX = (int)nColStart;
			continue;
		}

		// Calculate the tile ID

		int id = ch - m_cFirstChar;
		// Validate the tile
		if( id < 0 || id > (m_nNumRows *m_nNumCols ) ) continue;


		// Get sourc rect on the image
		RECT rTile = CellAlgorithm( id );

		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		// Draw!
		pTM->Draw( m_nImageID, nX, nY, fScale, fScale,
					&rTile, 0, 0, 0, dwColor );

		//if( ch == 69 )
		//	nOffset += 5;
		// Move to the next position on screen
		//if( bSpacing == true )
		//	nX += (int)( ( m_nCharWidth - ( nOffset + 5 ) ) * fScale);
		//else
			nX += (int)( ( m_nCharWidth - nOffset) * fScale);

		if( ch == 69 )
			bSpacing = true;
		else
			bSpacing = false;
	}
	bSpacing = false;

}

RECT CBitmapFont::CellAlgorithm( int id )
{
	RECT rCell;

	rCell.left = (id % m_nNumCols) * m_nCharWidth;
	rCell.top  = (id / m_nNumCols) * m_nCharHeight;

	rCell.right = rCell.left + m_nCharHeight;
	rCell.bottom = rCell.top + m_nCharWidth;

	return rCell;
}
