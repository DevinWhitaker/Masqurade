/*
	File:		Cursor.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Handles the game's cursor when using making the
				mouse exclusive to the game.
*/

#pragma once
#include "Game.h"

class CCursor
{
public:
	static CCursor* GetInstance(void);

	/////////////////////////////////
	void	Update( float felapsed );
	void	Render( void );
	/////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	long	GetCursorXPosition( void )	const { return m_lCursorPosition.x; }
	long	GetCursorYPosition( void )	const { return m_lCursorPosition.y; }
	void	SetCursorImage( int nImageID )	  { m_nCursorImage = nImageID;	}
	void	SetCursorRotationFloat( float fRotate )	  { m_fRotateCursor = fRotate;	}
	bool	GetRenderBool( void )	{ return m_bRenderCursor; }
	void	SetRenderBool( bool bRender ) { m_bRenderCursor = bRender; }
	/////////////////////////////////////////////////////////////////////////

private:
	///////////////////////////////////////////
	CCursor(void);
	CCursor(const CCursor&);
	CCursor& operator=(const CCursor&);
	virtual~CCursor(void);
	///////////////////////////////////////////

	//////////////////////////
	int		m_nCursorImage;
	float	m_fRotateCursor;
	bool	m_bRenderCursor;
	POINT	m_lCursorPosition;
	/////////////////////////

	/////////////////////////////////////////////////////////////////////////
	int		GetCursorImageID( void ) const		{ return m_nCursorImage; }
	float	GetCursorRotation( void ) const		{ return m_fRotateCursor; }
	/////////////////////////////////////////////////////////////////////////

};

