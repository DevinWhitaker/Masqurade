/*
	File:		HowToPlayState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Shows the user how to play the game
*/
#include "HowToPlayState.h"
#include "BitmapFont.h"
#include "FileManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "Cursor.h"

CHowToPlayState* CHowToPlayState::GetInstance(void)
{
	static CHowToPlayState s_Instance;

	return &s_Instance;
}

CHowToPlayState::CHowToPlayState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMF	= nullptr;
	m_pC	= nullptr;
}

void CHowToPlayState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pBMF	= CBitmapFont::GetInstance();
	m_pBMF->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );
	m_pC	=	 CCursor::GetInstance();
}

void CHowToPlayState::Exit(void)
{
	m_pBMF	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pC	= nullptr;
}

bool CHowToPlayState::Input(void)
{

	if( m_pDI->MouseButtonPressed( 0 ) )
	{

		CGame* pGame = CGame::GetInstance();

		if( m_pC->GetCursorXPosition() >= 620 && 
			m_pC->GetCursorXPosition()  <= 700 &&	
			m_pC->GetCursorYPosition() >= 535 && 
			m_pC->GetCursorYPosition()  <= 575  )
					CGame::GetInstance()->PopState( false  );
		return true;
	}

	if( m_pDI->KeyPressed( DIK_ESCAPE ) )
	{
		CGame::GetInstance()->PopState( false  );
		return true;
	}
	else if( m_pDI->KeyDown( DIK_RETURN )  && m_pDI->KeyDown( DIK_LALT ) )
	{
		CGame::GetInstance()->FullScreenMode();
		return true;
	}

	return true;
}
void CHowToPlayState::Update(float fElapsedTime)
{
	m_pC->Update( fElapsedTime );
}
void CHowToPlayState::Render(void)
{
	CGame*	pGame	=	CGame::GetInstance();
	m_pBMF->Print( "How To Play", pGame->GetWidth()  / 2 , 50, 1.0f,  4294967295UL, 23 );

	m_pBMF->Print( "A or Left Arrow - Move Left", 100 , 100, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "D or Right Arrow - Move Right", 100 , 125, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "W or Spacebar - Jump", 100 , 150, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "Left Shift - Pickup Corinne", 100 , 175, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "Right Mouse Click - Throw Corinne", 100 , 200, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "Move the mouse to aim your throw.", 100 , 225, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	m_pBMF->Print( "Escape - Pause game", 100 , 250, 1.0f,  
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	CFileManager*	pFM	=	 CFileManager::GetInstance();

	RECT buttonTest = { 0, 0, 164, 62 };

	int nButtonHeight = m_pTM->GetTextureHeight( pFM->GetImageIdMap()["Button"].nId );
	m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 600, 520 , 0.95f, 0.75f, &buttonTest);
	m_pBMF->Print( "Back", 620,  535, 1.0f,  4294967295UL, 23 );

	m_pD3D->GetSprite()->Flush();
	m_pC->Render();
}


CHowToPlayState::~CHowToPlayState(void)
{
}
