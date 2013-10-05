/*
	File:		MainMenuState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Handles the game's main menu and the states
				it can enter.
*/
#include "MainMenuState.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "BitmapFont.h"
#include "GamePlayState.h"
#include "LoadGameState.h"
#include "LevelSelectState.h"
#include "OptionState.h"
#include "HowToPlayState.h"
#include "CreditState.h"
#include "FileManager.h"
#include "Cursor.h"
#include "AnimationManager.h"

CMainMenuState* CMainMenuState::GetInstance(void)
{
	static CMainMenuState s_Instance;

	return &s_Instance;
}

CMainMenuState::CMainMenuState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMP	= nullptr;
	m_pFM	= nullptr;
	m_pC	= nullptr;
	m_pAM	= nullptr;



	SetMusicPlayingBool( false );
	SetNewGameBool( true );
	SetIntroTimer( 0.0f );
	SetToggleBool( true );
	m_dwMenuColors[0] = D3DCOLOR_ARGB( 100, 155, 155, 155 );
	m_dwMenuColors[1] = D3DCOLOR_ARGB( 255, 255, 255, 155 );
	m_nPointer = 0;

	m_ButtonPosition.fx = 400.0f;
	m_ButtonPosition.fy = 500.0f;
	m_ButtonPosition.unX = 32;
	m_ButtonPosition.unY = 32;

	m_TitlePosition.fx = 0.0f;
	m_TitlePosition.fy = 0.0f;
	m_TitlePosition.unX = 0;
	m_TitlePosition.unY = 0;

	m_vOptions.push_back( "New Game" );
	m_vOptions.push_back( "Load Game" );
	m_vOptions.push_back( "Options" );
	m_vOptions.push_back( "How To Play" );
	m_vOptions.push_back( "Credits" );
	m_vOptions.push_back( "Exit" );


}

void CMainMenuState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pXA	= CSGD_XAudio2::GetInstance();
	m_pBMP	= CBitmapFont::GetInstance();
	m_pFM	= CFileManager::GetInstance();
	m_pC	= CCursor::GetInstance();
	m_pAM	= CAnimationManager::GetInstance();
	//SetNewGameBool( true );

	m_pFM->GetSaves().clear();
	for( unsigned i = 0; i < 3; ++i )
	{
	/*	if( m_pFM->GetSaves().size() == 3 )
			break;*/
		CFileManager::playerProgress sLoad = { };
		std::ostringstream oss;
		oss << "Resources/Scripts/Save_" << i+1 << ".xml";
		if( m_pFM->LoadGame( oss.str().c_str(), sLoad, "", false ) == false )
			continue;
		else
			m_pFM->GetSaves().push_back( sLoad );
	}

	for( unsigned i = 0; i < m_pFM->GetSaves().size(); ++i )
	{
		std::ostringstream oss;
		oss << "Resources/Scripts/Save_" << i+1 << ".xml";
		if( m_pFM->GetSaves()[i].szFileName.compare( oss.str().c_str() ) != 0 )
		{
			m_pFM->GetSaves()[i].szFileName = oss.str().c_str();
			m_pFM->SaveGame(  m_pFM->GetSaves()[i].szFileName.c_str(),
				m_pFM->GetSaves()[i] );
		}
	}

	CGame* pGame = CGame::GetInstance();
	int nWindowX = pGame->GetWidth();
	int nWindowY = pGame->GetHeight();

	m_ButtonPosition.fx = 420.0f;
	m_ButtonPosition.fy = 500.0f;
	m_ButtonPosition.unX = 0;
	m_ButtonPosition.unY = 0;

	m_TitlePosition.fx = ( nWindowX / 2.0f ) + 20.0f;
	m_TitlePosition.fy = 100.0f;
	m_TitlePosition.unX = 0;
	m_TitlePosition.unY = 0;


	m_vButtonIDs = m_pAM->CreateAnimationFromFile("Resources/Scripts/Masquerade_Title.xml" );

	m_frTitle.Init( &m_TitlePosition.fx, &m_TitlePosition.fy, 
		&m_TitlePosition.unX, &m_TitlePosition.unY );

	m_frTitle.SetAnimationID( m_vButtonIDs[0] );
	m_frTitle.Play( true );

	m_pAM->AddFrame( m_frTitle );


	m_pBMP->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );
	m_pFM->AddSoundID("Main_Menu_Music", m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_Menu_Music.xwm") ), true );
	m_pFM->AddSoundID("Cursor_Move", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_CursorMove.wav") )  );
	
	m_vButtonIDs = m_pAM->CreateAnimationFromFile("Resources/Scripts/Menu_Buttons.xml" );

	m_frMainButton.Init( &m_ButtonPosition.fx, &m_ButtonPosition.fy, 
		&m_ButtonPosition.unX, &m_ButtonPosition.unY );
	m_pAM->AddFrame( m_frMainButton );

	m_frMainButton.SetAnimationID( m_vButtonIDs[0] );

	CFileManager::volumeStruct holdVolumeData = {};
	if( m_pFM->LoadSettings( "Resources/Scripts/Settings.xml", holdVolumeData ) == false )
	{
		m_pXA->SFXSetMasterVolume( 1.0f );
		m_pXA->MusicSetMasterVolume( 1.0f );
		CGame::GetInstance()->FullScreenMode( false );
	}
	else
	{
		float fSFX		= (float)holdVolumeData.GetSFXVolume() / 100.0f;
		float fMusic	= (float)holdVolumeData.GetMusicVolume() / 100.0f;

		m_pXA->SFXSetMasterVolume( fSFX );
		m_pXA->MusicSetMasterVolume( fMusic );

		if( holdVolumeData.b_FullScreen == 1 )
		CGame::GetInstance()->FullScreenMode( true );
		else
			CGame::GetInstance()->FullScreenMode( false );
	}

	if( GetMusicPlayingBool() == false )
	{
		m_pXA->MusicPlaySong( m_pFM->GetSoundIdMap()["Main_Menu_Music"].nId, true );
		SetMusicPlayingBool( true );
	}

	//COptionState::GetInstance()->LoadVolume( 
	SetIntroTimer( 0.0f );
	//m_pFM->AddImageID( "Title_Screen", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/SGP_Menu.png") ) );
//	m_pFM->AddImageID( "Maska_01", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/SGP_Maska_01.jpg") ) );
	m_pFM->AddImageID( "Maska_02", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Maska_02.jpg") ) );
	m_pFM->AddImageID( "Cursor", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Cursor.png") ) );
	m_pFM->AddImageID( "Game_Cursor", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_GameCursor.png") ) );
	m_pFM->AddImageID( "Button", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Menu_Buttons.png") ) );
	m_pFM->AddImageID( "JAMDLogo", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_JAMDLogo.png") ) );
	m_pFM->AddImageID( "GameLogo", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_GameLogo.png") ) );
	m_pFM->AddImageID( "Profile", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Profile.png") ) );
	m_pFM->AddImageID( "World_Background", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Background.png") ) );
	m_pFM->AddImageID( "World_Background_2", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Background_2.png") ) );
	m_pFM->AddImageID( "World_Background_3", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Background_3.png") ) );

	m_pC->SetCursorImage( m_pFM->GetImageID( "Game_Cursor" ) );
	m_pC->SetCursorRotationFloat( 4.0f );

	//m_pFM->AddSoundID( "World_1",
	//	m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_1.xwm") ), true );

	//m_pFM->AddSoundID( "World_2",
	//	m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_2.xwm") ), true );

	//m_pFM->AddSoundID( "World_3",
	//	m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_3.xwm") ), true );

	//m_pFM->AddSoundID( "World_0",
	//	m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_0.xwm") ), true );

}

void CMainMenuState::Exit(void)
{
	if( m_nPointer == GAMEPLAY || m_nPointer == EXIT || m_nPointer == HOWTOPLAY || 
		m_nPointer == LOADGAME )
	{
		//SetNewGameBool( true );
		SetToggleBool( true );
		SetMusicPlayingBool( false );
		m_pAM->RemoveFrame( &m_frMainButton );
		m_pAM->RemoveFrame( &m_frTitle );
		m_pBMP->Shutdown();
		if( m_pAM != nullptr )
		{
			m_pAM->Shutdown();
			m_pAM = nullptr;
		}
	}

	if( m_nPointer == EXIT )
		SetToggleBool( false );

	if( GetMusicPlayingBool() == false )
		m_pXA->MusicStopSong( m_pFM->GetSoundIdMap()["Main_Menu_Music"].nId );

	m_pC->SetRenderBool( true );

	m_pC	= nullptr;
	m_pFM	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMP	= nullptr;
}

bool CMainMenuState::Input(void)
{
	CGame* pGame = CGame::GetInstance();

#if !ARCADE
	if( GetToggleBool() == true )
	{
		if( m_pDI->GetDIKCode()  || m_pDI->MouseButtonPressed( 0 ) ||
			m_pDI->MouseButtonPressed( 1 ) ||
			m_pDI->MouseButtonPressed( 2 ) )
			SetToggleBool( false );
		return true;
	}
#else
	if( GetToggleBool() == true )
	{
		if( m_pDI->JoystickCheckBufferedButtons() != -1 )
			SetToggleBool( false );
		return true;
	}
#endif
	else
	{

		if( m_pDI->MouseMovementX() > 0 ||
			m_pDI->MouseMovementY() > 0 )
			m_pC->SetRenderBool( true );

		if( m_pDI->KeyPressed( DIK_ESCAPE ) )
		{
			m_nPointer = EXIT;
			m_pC->SetRenderBool( false );
		}

		else if( m_pDI->KeyDown( DIK_RETURN )  && m_pDI->KeyDown( DIK_LALT ) )
		{
			//CGame::GetInstance()->FullScreenMode();
			//return true;
		}

		if(m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->MouseButtonPressed( 2 ) || m_pDI->JoystickButtonPressed( 0 ) )
		{
			switch( m_nPointer )
			{
			case GAMEPLAY:
				SetTutorialBool( false );
				CGame::GetInstance()->PushState( CLoadGameState::GetInstance() );
				SetNewGameBool( true );
				return true;
				break;
			case LOADGAME:
				CGame::GetInstance()->PushState( CLoadGameState::GetInstance(), false );
				SetNewGameBool( false );
				return true;
				break;
			case OPTIONS:
				CGame::GetInstance()->PushState( COptionState::GetInstance(), false );
				return true;
				break;
			case HOWTOPLAY:
				SetTutorialBool( true );
				CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );
				return true;
				break;
			case CREDITS:
				CGame::GetInstance()->PushState( CCreditState::GetInstance() );
				return true;
				break;
			case EXIT:
				CGame::GetInstance()->PopState();
				return false;
				break;
			default:
				break;
			}
			return true;
		}

		if( m_pC->GetCursorXPosition() >= 338 &&
			m_pC->GetCursorXPosition() <= 338 + 150 &&
			m_pC->GetCursorYPosition() >= 449 &&
			m_pC->GetCursorYPosition() <= 449 + 53 )
		{
			if( m_frMainButton.GetAnimationID() != m_vButtonIDs[1] )
				m_frMainButton.SetAnimationID( m_vButtonIDs[1] );
			if( m_frMainButton.isPlaying() == false )
				m_frMainButton.Play( true );
		}
		else
			m_frMainButton.SetAnimationID( m_vButtonIDs[0] );

		if( m_pDI->MouseButtonPressed( 0 ) &&
			m_pC->GetCursorXPosition() >= 338 &&
			m_pC->GetCursorXPosition() <= 338 + 150 &&
			m_pC->GetCursorYPosition() >= 449 &&
			m_pC->GetCursorYPosition() <= 449 + 53 )
		{
			m_frMainButton.SetAnimationID( m_vButtonIDs[2] );
			m_frMainButton.Play( true );
				switch( m_nPointer )
				{
				case GAMEPLAY:
					SetTutorialBool( false );
						CGame::GetInstance()->PushState( CLoadGameState::GetInstance() );
					SetNewGameBool( true );
	/*				}
					else
						CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );*/
					break;
				case LOADGAME:
					CGame::GetInstance()->PushState( CLoadGameState::GetInstance(), false );
					SetNewGameBool( false );

					break;
				case OPTIONS:
					CGame::GetInstance()->PushState( COptionState::GetInstance(), false );
					break;
				case HOWTOPLAY:
					SetTutorialBool( true );					
					CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );
					break;
				case CREDITS:
					CGame::GetInstance()->PushState( CCreditState::GetInstance() );
					break;
				case EXIT:
					CGame::GetInstance()->PopState();
					return false;
					break;
				default:
					break;
				}
				return true;

		}

#if !ARCADE
		if(m_pDI->KeyPressed( DIK_RIGHT ) || m_pDI->KeyPressed( DIK_D ) ||
			m_pDI->MouseWheelMovement() < 0)
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			if( m_nPointer >= EXIT  )
				m_nPointer = GAMEPLAY;
			else
				m_nPointer++;
			return true;
		}
		if(m_pDI->KeyPressed( DIK_LEFT ) || m_pDI->KeyPressed( DIK_A ) ||
			m_pDI->MouseWheelMovement() > 0)
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			if( m_nPointer <= GAMEPLAY )
				m_nPointer = EXIT;
			else
				m_nPointer--;
			return true;
		}
#else
		if(m_pDI->JoystickGetLStickDirPressed( DIR_UP ) ||
			m_pDI->MouseWheelMovement() < 0)
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			if( m_nPointer >= EXIT  )
				m_nPointer = GAMEPLAY;
			else
				m_nPointer++;
			return true;
		}
		if(m_pDI->JoystickGetLStickDirPressed( DIR_DOWN ) ||
			m_pDI->MouseWheelMovement() > 0)
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			if( m_nPointer <= GAMEPLAY )
				m_nPointer = EXIT;
			else
				m_nPointer--;
			return true;
		}
#endif
	}

	return true;
}
void CMainMenuState::Update(float fElapsedTime)
{
	m_pAM->UpdateAll( fElapsedTime );
	SetIntroTimer( GetIntroTimer() + fElapsedTime );
	m_pC->Update( fElapsedTime );
}
void CMainMenuState::Render(void)
{
	if( GetToggleBool() == false )
	{
		m_pTM->Draw( m_pFM->GetImageIdMap()["Maska_02"].nId, 0, 0, 0.39f, 0.35f );

		m_pD3D->GetSprite()->Flush();

		m_pAM->RenderAll();

		m_pD3D->GetSprite()->Flush();

		m_pBMP->Print( m_vOptions[ m_nPointer ].c_str(), 360, 475, 1.0f,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23 );

	/*	if( m_pC->GetRenderBool() == false )
		{*/
			m_pTM->Draw( m_pFM->GetImageID("Game_Cursor"), (int)m_ButtonPosition.fx + 80,
				(int)m_ButtonPosition.fy - 30 , 0.7f, 0.7f );

			m_pTM->Draw( m_pFM->GetImageID("Game_Cursor"), (int)m_ButtonPosition.fx - 90,
				(int)m_ButtonPosition.fy - 10 , -0.7f, -0.7f );
		//}

		RECT temprect = { 0, 0, 300, 70 };
		m_pTM->Draw( m_pFM->GetImageIdMap()["JAMDLogo"].nId, 10, 475, 0.125f, 0.125f );

		m_pD3D->GetSprite()->Flush();
		m_pC->Render();
	}
	else
	{
		m_pTM->Draw( m_pFM->GetImageIdMap()["Maska_02"].nId, 0, 0, 0.39f, 0.35f );
		m_pTM->Draw( m_pFM->GetImageIdMap()["JAMDLogo"].nId, 10, 475, 0.125f, 0.125f );

		RECT temprect = { 0, 0, 300, 70 };

		m_pTM->Draw( m_pFM->GetImageIdMap()["GameLogo"].nId, 262, 
			33, 1.0f, 1.0f, &temprect );
		m_pBMP->Print( "Press any key to continue. . .", 280, 550, 1.0f,  4294967295UL, 23 );
	}
}


CMainMenuState::~CMainMenuState(void)
{
}