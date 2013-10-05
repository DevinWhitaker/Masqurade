/*
File:		LoadGameeSate.cpp
Course:		SGP1
Author:		Ja'Michael Garcia
Purpose:	Allows the player load up their saved progress
*/
#include "LoadGameState.h"
#include "BitmapFont.h"
#include "FileManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include <stdio.h>
#include "Game.h"
#include "GamePlayState.h"
#include "Cursor.h"
#include "Fox.h"
#include "MainMenuState.h"

CLoadGameState* CLoadGameState::GetInstance(void)
{
	static CLoadGameState s_Instance;

	return &s_Instance;
}

CLoadGameState::CLoadGameState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMF	= nullptr;
	m_pC	= nullptr;
	m_pFM	= nullptr;

	int nAddingX = 250;
	int nAddingY = 300;
	for(unsigned i = 0; i < 3; ++i )
	{
		m_PosButtons[i].x = 100 + (nAddingX * i );
		m_PosButtons[i].y = 300;
	}

	SetRender( -1 );
	SetDeleteMode( false );

}

void CLoadGameState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pBMF	= CBitmapFont::GetInstance();
	m_pFM	= CFileManager::GetInstance();
	m_pFM->ResetTimer( );
	m_pFM->ResetAlpha();
	m_pBMF->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );

	SetDeleteMode( false );
	m_pFM->AddImageID( "Squirt_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Squirt_Gun_HUD.png") ) );	 
	m_pFM->AddImageID( "CottonCandy_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_CottonCandy_Gun_HUD.png") ) );	
	m_pFM->AddImageID( "Balloon_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Balloon_Gun_HUD.png") ) );	
	m_pFM->AddImageID( "Delete_Cursor", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Delete.png") ) );	

	m_pC	=	CCursor::GetInstance();
	SetRender( -1 );
}

void CLoadGameState::Exit(void)
{
	m_pFM->ResetTimer( );
	m_pFM->ResetAlpha();
	SetDeleteMode( false );
	CMainMenuState::GetInstance()->SetNewGameBool( true );

	//int nWidth = m_pTM->GetTextureWidth( m_pFM->GetImageID( "World_Background"));
	//int nHeight = m_pTM->GetTextureHeight( m_pFM->GetImageID( "World_Background"));
	//RECT rBackground = { nWidth - CGame::GetInstance()->GetWidth(), 0, 
	//	nWidth, nHeight };

	//while( m_pFM->GetAlpha() < 255 )
	//{
	//	m_pTM->Draw( m_pFM->GetImageIdMap()["Maska_02"].nId, 0, 0, 0.39f, 0.35f,
	//		nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( m_pFM->GetAlpha(), 255, 255, 255) );
	//	m_pTM->Draw( m_pFM->GetImageID( "World_Background"), 0, 0, 1.0f, 1.3f, &rBackground,
	//		0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	//	m_pFM->IncrementAlpha();
	//}

	SetRender( -1 );
	m_pFM	= nullptr;
	m_pC	= nullptr;
	m_pBMF	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
}

bool CLoadGameState::Input(void)
{

	/*long x = m_pC->GetCursorXPosition();
	long y = m_pC->GetCursorYPosition();*/

	for( unsigned i = 0; i < 3; ++i )
	{	
		int nWidth = 164;
		int nHeight = 62;
		if( m_pC->GetCursorXPosition() >= m_PosButtons[i].x &&
			m_pC->GetCursorXPosition() <= m_PosButtons[i].x + nWidth &&
			m_pC->GetCursorYPosition() >= m_PosButtons[i].y &&
			m_pC->GetCursorYPosition() <= m_PosButtons[i].y + nHeight )
		{
			SetRender( i );
			break;
		}
		else
			SetRender( -1 );
	}

	if( m_pDI->MouseButtonPressed( 0 ) )
	{
		int nWidth = 164;
		int nHeight = 62;

		if( m_pC->GetCursorXPosition() >= 100 && 
			m_pC->GetCursorXPosition() <= 250 &&	
			m_pC->GetCursorYPosition() >= 450 && 
			m_pC->GetCursorYPosition() <= 570  )
		{
			if( GetDeleteBool() == false )
			{
				m_pC->SetCursorImage( m_pFM->GetImageID( "Delete_Cursor") );
				SetDeleteMode( true );
				return true;
			}
			//else
			//{
			//	m_pC->SetCursorImage( m_pFM->GetImageID( "Game_Cursor") );
			//	SetDeleteMode( false );
			//}
			//return true;
		}
	}

	if( m_pDI->MouseButtonPressed( 0 ) )
	{

		CGame* pGame = CGame::GetInstance();

		int nWidth = 164;
		int nHeight = 62;

		if( m_pC->GetCursorXPosition() >= 600 && 
			m_pC->GetCursorXPosition() <= 750 &&	
			m_pC->GetCursorYPosition() >= 450 && 
			m_pC->GetCursorYPosition() <= 550  )
		{
			if( CMainMenuState::GetInstance()->GetNewGameBool() == true )
				CGame::GetInstance()->PopState(   );
			else
				CGame::GetInstance()->PopState( false  );
		}
		else
		{
			for( unsigned i = 0; i < 3; ++i )
			{
				if( m_pC->GetCursorXPosition() >= m_PosButtons[i].x &&
					m_pC->GetCursorXPosition() <= m_PosButtons[i].x + nWidth &&
					m_pC->GetCursorYPosition() >= m_PosButtons[i].y &&
					m_pC->GetCursorYPosition() <= m_PosButtons[i].y + nHeight )
				{
					if( GetDeleteBool() == true )
					{
						if( m_pFM->GetSaves().size() < i + 1 )
							break;
						else
						{
							std::ostringstream oss;
							oss << "Resources/Scripts/Save_" << i+1 << ".xml";
							if( remove( oss.str().c_str() ) != 0 )
								break;
							else
							{
								m_pFM->GetSaves().erase( m_pFM->GetSaves().begin() + i );
								m_pC->SetCursorImage( m_pFM->GetImageID( "Game_Cursor") );
								SetDeleteMode( false );
								for( unsigned j = 0; j < m_pFM->GetSaves().size(); ++j )
								{
									std::ostringstream oss2;
									oss2 << "Resources/Scripts/Save_" << j+1 << ".xml";
									if( m_pFM->GetSaves()[j].szFileName.compare( oss2.str().c_str() ) != 0 )
									{
										m_pFM->GetSaves()[j].szFileName = oss2.str().c_str();
										m_pFM->SaveGame(  m_pFM->GetSaves()[j].szFileName.c_str(),
											 m_pFM->GetSaves()[j] );
									}
								}
								return true;
							}
						}
					}

					if(  CMainMenuState::GetInstance()->GetNewGameBool() == false &&
						m_pFM->GetSaves().size() < i + 1 )
						break;
					if( CMainMenuState::GetInstance()->GetNewGameBool() == false )
						m_pFM->SetSaveToLoad( i + 1 );
					else if( CMainMenuState::GetInstance()->GetNewGameBool() == true &&
						m_pFM->GetSaves().size() > i )
						break;

					if( CMainMenuState::GetInstance()->GetNewGameBool() == true )
						m_pFM->SetSaveToLoad( -1 );


					/*	if( m_pFM->GetSaves().size() < i +1 )
					break;*/
					m_pFM->SetCurrentSaveToLoad( i + 1 );
					CGame::GetInstance()->PopState( );
					CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );
					//CGamePlayState::GetInstance()->SetCurStage( 0 );
					return true;
				}
			}
		}
		return true;
	}

	if( m_pDI->MouseButtonPressed( 0 ) &&  GetDeleteBool() == true )
	{
		m_pC->SetCursorImage( m_pFM->GetImageID( "Game_Cursor") );
		SetDeleteMode( false );
		return true;
	}
	if( m_pDI->KeyPressed( DIK_ESCAPE ) )
	{
		//m_pFM->ResetAlpha();e
		if( CMainMenuState::GetInstance()->GetNewGameBool() == true )
			CGame::GetInstance()->PopState(  );
		else
			CGame::GetInstance()->PopState( false );
		return true;
	}
	//else if( m_pDI->KeyDown( DIK_RETURN )  && m_pDI->KeyDown( DIK_LALT ) )
	//{
	//	CGame::GetInstance()->FullScreenMode();
	//	return true;
	//}




	return true;
}
void CLoadGameState::Update(float fElapsedTime)
{
	m_pC->Update( fElapsedTime );

	m_pFM->IncrementTimer( fElapsedTime );

	if( m_pFM->GetAlpha() < 255 )
		m_pFM->IncrementAlpha();
}
void CLoadGameState::Render(void)
{
	CGame*	pGame	=	CGame::GetInstance();
	CMainMenuState*	pMM	= CMainMenuState::GetInstance();


	CFileManager*	pFM	=	 CFileManager::GetInstance();

	int nWidth = m_pTM->GetTextureWidth( m_pFM->GetImageID( "World_Background"));
	int nHeight = m_pTM->GetTextureHeight( m_pFM->GetImageID( "World_Background"));
	RECT rBackground = { 11000, 0, 
		12220, 726 };


	if( m_pFM->GetAlpha() < 255 )
	{
		if( pMM->GetNewGameBool() == false )
			m_pBMF->Print( "Load Game", pGame->GetWidth() / 2, 50, 1.0f,  4294967295UL, 23 );
		else
			m_pBMF->Print( "Select a Profile", (pGame->GetWidth() / 2) - 50, 50, 1.0f,  4294967295UL, 23 );

		m_pTM->Draw( m_pFM->GetImageIdMap()["Maska_02"].nId, 0, 0, 0.39f, 0.35f,
			nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255) );
		m_pTM->Draw( m_pFM->GetImageID( "World_Background"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(  m_pFM->GetAlpha(), 255, 255, 255 ) );

	}
	else
	{
		m_pTM->Draw( m_pFM->GetImageID( "World_Background"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(  255, 255, 255, 255 ) );

		if( pMM->GetNewGameBool() == false && GetRender() < 0 )
			m_pBMF->Print( "Load Game", pGame->GetWidth() / 2, 50, 1.0f,  4294967295UL, 23 );
		else if( pMM->GetNewGameBool() == true && GetRender() < 0 )
			m_pBMF->Print( "Select a Profile", (pGame->GetWidth() / 2) - 50 , 50, 1.0f,  4294967295UL, 23 );

		m_pD3D->GetSprite()->Flush();

		RECT buttonTest = { 0, 0, 164, 62 };
		int nButtonHeight = m_pTM->GetTextureHeight( pFM->GetImageIdMap()["Button"].nId );

		m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 600, 520 , 0.95f, 0.75f, &buttonTest);
		m_pBMF->Print( "Back", 620,  535, 1.0f,  4294967295UL, 23 );

		RECT buttonTest2 = { 250, 190, 417, 253 };
		m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 100, 520 , 0.95f, 0.75f, &buttonTest2);
		m_pBMF->Print( "Erase", 120,  535, 1.0f,  4294967295UL, 23 );

		m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 100, 300 , 0.95f, 0.75f, &buttonTest);
		m_pBMF->Print( "Save 1", 150,  310, 1.0f,  4294967295UL, 23 );

		//m_pTM->Draw( pFM->GetImageID( "Profile"), 100, 370 );

		m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 350, 300 , 0.95f, 0.75f, &buttonTest);
		m_pBMF->Print( "Save 2", 400,  310, 1.0f,  4294967295UL, 23 );

		m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 600, 300 , 0.95f, 0.75f, &buttonTest);
		m_pBMF->Print( "Save 3", 650,  310, 1.0f,  4294967295UL, 23 );

		m_pD3D->GetSprite()->Flush();
		m_pC->Render();
		m_pD3D->GetSprite()->Flush();

		std::ostringstream ossWorld;
		switch( GetRender() )
		{
		case 0:
			{
				if( m_pFM->GetSaves().size() < 1 )
					m_pBMF->Print( "Empty Slot",  pGame->GetWidth() / 2,  50, 1.0f,  
					4294967295UL, 23 );
				else
				{
					ossWorld <<  "World " << m_pFM->GetSaves()[ GetRender() ].nLevelNum;
					m_pBMF->Print( ossWorld.str().c_str(), pGame->GetWidth() / 2,  
						50, 1.0f, 4294967295UL, 23 );

					int nYMod = 30;
					for( unsigned int i = 0; i < ITool::MAX_TOOLS; ++i )
					{
						if( m_pFM->GetSaves()[ GetRender() ].pFox->GetATool( i ) == nullptr )
							break;

						int nY = nYMod * i;
						int nTool = m_pFM->GetSaves()[ GetRender() ].pFox->GetATool(i )->getToolType();


						/*	"Squirt_Gun_HUD.png", m_p
						"CottonCandy_Gun_HUD.png"
						"Balloon_Gun_HUD.png", m_*/

						m_pD3D->GetSprite()->Flush();
						switch( nTool )
						{
						case ITool::TOOL_BALLOONGUN:
							m_pTM->Draw( m_pFM->GetImageID( "Balloon_Gun_HUD.png" ),
								100, 400 * nY );
							break;
						case ITool::TOOL_COTTONCANDYLAUNCHER:
							{
								m_pTM->Draw( m_pFM->GetImageID( "CottonCandy_Gun_HUD.png" ),
									100, 400 * nY );
							}
							break;
						case ITool::TOOL_SQUIRTGUN:
							{
								m_pTM->Draw( m_pFM->GetImageID( "Squirt_Gun_HUD.png" ),
									100, 400 * nY );
							}
							break;
						default:
							break;
						};

						m_pD3D->GetSprite()->Flush();
					}

					//m_pFM->GetSaves()[ GetRender() ].pFox->GetATool(
				}
			}
			break;
		case 1:
			{
				if( m_pFM->GetSaves().size() < 2 )
					m_pBMF->Print( "Empty Slot",  pGame->GetWidth() / 2,  50, 1.0f,  
					4294967295UL, 23 );
				else
				{
					ossWorld <<  "World " << m_pFM->GetSaves()[ GetRender() ].nLevelNum;
					m_pBMF->Print( ossWorld.str().c_str(), pGame->GetWidth() / 2,  
						50, 1.0f, 4294967295UL, 23 );
				}
			}
			break;
		case 2:
			{
				if( m_pFM->GetSaves().size() < 3 )
					m_pBMF->Print( "Empty Slot",  pGame->GetWidth() / 2,  50, 1.0f,  
					4294967295UL, 23 );
				else
				{
					ossWorld <<  "World " << m_pFM->GetSaves()[ GetRender() ].nLevelNum;
					m_pBMF->Print( ossWorld.str().c_str(), pGame->GetWidth() / 2,  
						50, 1.0f, 4294967295UL, 23 );
				}
			}
			break;
		default:
			//m_pD3D->GetSprite()->Flush();
			break;
		};
	}
}


CLoadGameState::~CLoadGameState(void)
{
}
