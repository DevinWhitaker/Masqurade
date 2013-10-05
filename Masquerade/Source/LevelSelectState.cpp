/*
	File:		LevelSelectState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Shows a cutscene at the beginning of a new game
				( Work in progress )
*/
#include "LevelSelectState.h"
#include "FileManager.h"
#include "BitmapFont.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "Cursor.h"

CLevelSelectState* CLevelSelectState::GetInstance(void)
{
	static CLevelSelectState s_Instance;

	return &s_Instance;
}

CLevelSelectState::CLevelSelectState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMF	= nullptr;
	m_pC	= nullptr;
}

void CLevelSelectState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pBMF	= CBitmapFont::GetInstance();
	m_pBMF->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );
	m_pC = CCursor::GetInstance();
}

void CLevelSelectState::Exit(void)
{
	m_pC	= nullptr;
	m_pBMF	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
}

bool CLevelSelectState::Input(void)
{

	if( m_pDI->MouseButtonPressed( 0 ) )
	{

		CGame* pGame = CGame::GetInstance();

		if( m_pC->GetCursorXPosition() >= 620 && m_pC->GetCursorXPosition() <= 700 
					&&	m_pC->GetCursorYPosition() >= 535 && 
					m_pC->GetCursorYPosition()  <= 575  )
					CGame::GetInstance()->PopState( false  );
		return true;
	}

	if( m_pDI->KeyPressed( DIK_ESCAPE ) )
	{
		CGame::GetInstance()->PopState( false );
		return true;
	}
	else if( m_pDI->KeyDown( DIK_RETURN )  && m_pDI->KeyDown( DIK_LALT ) )
	{
		CGame::GetInstance()->FullScreenMode();
		return true;
	}

	return true;
}
void CLevelSelectState::Update(float fElapsedTime)
{

	m_pC->Update( fElapsedTime );
//#if _DEVIN
//	pos.Update(fElapsedTime);
//	pos2.Update(fElapsedTime);
//	test.Update(fElapsedTime);
//	test2.Update(fElapsedTime);
//	CAnimationManager::GetInstance()->CheckCollision(&test,test2);
//#endif


}
void CLevelSelectState::Render(void)
{
	CGame*	pGame	=	CGame::GetInstance();
	CFileManager*	pFM	=	 CFileManager::GetInstance();

	m_pBMF->Print( "Level Select", pGame->GetWidth() / 2, 50, 1.0f,  4294967295UL, 23 );

	RECT buttonTest = { 0, 0, 164, 62 };
	//m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 100, pGame->GetHeight() / 2, 1.0f, 4.0f, &buttonTest);

	int nButtonHeight = m_pTM->GetTextureHeight( pFM->GetImageIdMap()["Button"].nId );
	m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 600, 520 , 0.95f, 0.75f, &buttonTest);
	m_pBMF->Print( "Back", 620,  535, 1.0f,  4294967295UL, 23 );
		m_pC->Render();
	m_pD3D->GetSprite()->Flush();
//
//#if _DEVIN
//	test.Render();
//	test2.Render();
//#endif


}


CLevelSelectState::~CLevelSelectState(void)
{
}
