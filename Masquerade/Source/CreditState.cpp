/*
	File:		CreditState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Credits the people who have contributed
				to the making of the video game.
*/
#include "CreditState.h"
#include "BitmapFont.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "GamePlayState.h"
#include "Cursor.h"
#include "FileManager.h"

CCreditState* CCreditState::GetInstance(void)
{
	static CCreditState s_Instance;

	return &s_Instance;
}

CCreditState::CCreditState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMF	= nullptr;
	m_pC	= nullptr;
	m_pFM	= nullptr;
}

void CCreditState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pBMF	= CBitmapFont::GetInstance();
	m_pC	=	CCursor::GetInstance();
	m_pFM	=	CFileManager::GetInstance();
	m_pC->SetRenderBool( true );
	m_pBMF->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );
	m_fDuration = 0.0f;	m_pFM->ResetAlpha();}

void CCreditState::Exit(void)
{
	m_pBMF->Shutdown();
	m_pFM->ResetAlpha();

	m_pC	= nullptr;
	m_pBMF	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
}

bool CCreditState::Input(void)
{
	if( m_pDI->KeyPressed( DIK_ESCAPE ) || m_pDI->JoystickButtonPressed( 0 ) )
	{
		//if( CGamePlayState::GetInstance()->g
		CGame::GetInstance()->PopState( /*false*/ );
		return true;
	}
	return true;
}
void CCreditState::Update(float fElapsedTime)
{
	m_pC->Update( fElapsedTime );
	if( m_pFM->GetAlpha() < 255 )
		m_pFM->IncrementAlpha();	if(m_fDuration >= 15.0f)
		CGame::GetInstance()->PopState();}
void CCreditState::Render(void)
{
	RECT rBackground = { 750, 0, 
		1880, 725 };

	m_pD3D->GetSprite()->Flush();
	if( m_pFM->GetAlpha() < 255 )
	{
		m_pTM->Draw( m_pFM->GetImageID( "World_Background_2"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(  m_pFM->GetAlpha(), 255, 255, 255 ) );
	}
	else
	{
		m_pTM->Draw( m_pFM->GetImageID( "World_Background_2"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	}

	m_pD3D->GetSprite()->Flush();

	m_pC->Render();

	CGame*	pGame	=	CGame::GetInstance();

	m_pBMF->Print( "Credits", pGame->GetWidth() / 2, 100, 1.0f,  4294967295UL, 23 );

	m_pBMF->Print( "Executive Producer", 200, 200, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "John O'Leske", 200, 220, 1.0f,  4294967295UL, 23 );
	
	m_pBMF->Print( "Associate Producer", pGame->GetWidth() - 200, 200, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Shawn Paris", pGame->GetWidth() - 200, 220, 1.0f,  4294967295UL, 23 );

	m_pBMF->Print( "Progammers", pGame->GetWidth() /2, 300, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Ja'Michael Garcia", ( pGame->GetWidth() /2 ) - 10, 330, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Adam Foy", ( pGame->GetWidth() /2 ) - 10, 360, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Mustafa Aldoori", ( pGame->GetWidth() /2 ) - 10, 390, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Devin Whitaker", ( pGame->GetWidth() /2 ) - 10, 420, 1.0f,  4294967295UL, 23 );

	m_pBMF->Print( "Artists", 150, 400, 1.0f,  4294967295UL, 23 );
	m_pBMF->Print( "Ahmed Aldoori", 140, 430, 1.0f,  4294967295UL, 21 );
	m_pBMF->Print( "Chris Jahosky", 140, 460, 1.0f,  4294967295UL, 21 );

	//m_pD3D->DrawText( _T("Credits"), 200, 200, 100, 100, 100 );
	//m_pD3D->DrawText( _T("Ja'Michael Garcia"), 200, 250, 100, 100, 100 );
	//m_pD3D->DrawText( _T("Adam Foy"), 200, 275, 100, 100, 100 );
	//m_pD3D->DrawText( _T("Mustafa Aldoori"), 200, 300, 100, 100, 100 );
	//m_pD3D->DrawText( _T("Devin Whitaker"), 200, 325, 100, 100, 100 );
	//m_pD3D->DrawText( _T("AP: Shawn Paris"), 200, 350, 100, 100, 100 );
	//m_pD3D->DrawText( _T("EP: John O'Leske"), 200, 375, 100, 100, 100 );
}


CCreditState::~CCreditState(void)
{
}
