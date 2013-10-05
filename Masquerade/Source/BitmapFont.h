/*
	File:		BitmapFont.h
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Contain Bitmap Font Related Code
*/
#pragma once
#include <Windows.h>
#include <tchar.h>

class CBitmapFont
{
private:
	// Cell Algorithm data
	int		m_nCharWidth;
	int		m_nCharHeight;
	int		m_nNumRows;
	int		m_nNumCols;

	int		m_nImageID;
	char	m_cFirstChar;

public:
	CBitmapFont(void);
	virtual~CBitmapFont(void);
	CBitmapFont& operator=( const CBitmapFont& );
	static CBitmapFont* GetInstance( void );

	void Init( TCHAR* filename, int width, int height, 
				int rows, int cols, char firstchar );

	void Shutdown(void);

	void	Print( const char* szText,	int nX, int nY,
				   float fScale = 1.0f, DWORD dwColor = 4294967295UL, int nOffset = 23 );

	RECT CellAlgorithm( int id );


};

