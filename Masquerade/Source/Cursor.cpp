/*
	File:		Cursor.cpp
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Handles the game's cursor when using making the
				mouse exclusive to the game.
*/

#include "Cursor.h"
//#include "FileManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

CCursor* CCursor::GetInstance(void)
{
	static CCursor s_Instance;

	return &s_Instance;
}

CCursor::CCursor(void)
{
	CGame*	pGame = CGame::GetInstance();
	GetCursorPos(&m_lCursorPosition);
	ScreenToClient(pGame->GetWindowHandle(), &m_lCursorPosition);
	SetCursorRotationFloat( 0.0f );
	SetRenderBool( true );
}

CCursor::~CCursor(void)
{
}

void CCursor::Update( float felapsed )
{
	CGame*	pGame = CGame::GetInstance();
	GetCursorPos(&m_lCursorPosition);
	ScreenToClient(pGame->GetWindowHandle(), &m_lCursorPosition);
}

void CCursor::Render( void )
{
	if( GetRenderBool() == true )
	{
		CSGD_TextureManager*	pTM = CSGD_TextureManager::GetInstance();

		pTM->Draw( GetCursorImageID(), GetCursorXPosition(), GetCursorYPosition(),
			0.8f, 0.8f, nullptr, pTM->GetTextureWidth( GetCursorImageID() ) / 2.0f,
			pTM->GetTextureHeight( GetCursorImageID() ) / 2.0f, GetCursorRotation(),
			D3DCOLOR_ARGB( 200, 255, 255, 255 ) );
	}
}