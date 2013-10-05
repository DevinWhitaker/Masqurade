/*
	File:		OptionState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Changes the music and sound volume for the entire game.
*/
#include "OptionState.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "MainMenuState.h"
#include "BitmapFont.h"
#include <tchar.h>
#include <sstream>
#include "GamePlayState.h"
#include "Cursor.h"

using namespace std;


COptionState* COptionState::GetInstance(void)
{
	static COptionState s_Instance;

	return &s_Instance;
}

COptionState::COptionState(void)
{
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	m_pBMP	= nullptr;
	m_pFM	= nullptr;
	m_pC	= nullptr;

	SetCursorPos( 0 );

	int width = CGame::GetInstance()->GetWidth();
	int height = CGame::GetInstance()->GetHeight();
	int subtract = 50;

	for(int i = 0; i < 3; i++)
	{
		m_POcursorPos[i].x	=	width / 2;
		m_POcursorPos[i].y	=	(height / 2) + (i *subtract);
	}
}

void COptionState::Enter(void)
{
	if( m_pFM->LoadSettings( "Resources/Scripts/Settings.xml", GetVolumeStruct() ) == false )
	{
		GetVolumeStruct().b_FullScreen = 0;
		GetVolumeStruct().SetMusicVolume( 100 );
		GetVolumeStruct().SetSFXVolume( 100 );
	}

	SetCursorPos( 0 );
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();	
	m_pBMP = CBitmapFont::GetInstance();
	m_pFM	= CFileManager::GetInstance();
	m_pC	= CCursor::GetInstance();
}

void COptionState::Exit(void)
{
	m_pFM->SaveSettings( "Resources/Scripts/Settings.xml", GetVolumeStruct() );
	SetCursorPos( 0 );

	m_pC	= nullptr;
	m_pFM	= nullptr;
	m_pBMP	= nullptr;
	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
}

bool COptionState::Input(void)
{
	if( m_pDI->MouseMovementX() > 0 ||
		m_pDI->MouseMovementY() > 0 )
		m_pC->SetRenderBool( true );

	if( m_pDI->MouseButtonPressed( 0 ) )
	{

		CGame* pGame = CGame::GetInstance();

		if( m_pC->GetCursorXPosition() >= 620 && 
			m_pC->GetCursorXPosition() <= 700 &&	
			m_pC->GetCursorYPosition() >= 535 && 
			m_pC->GetCursorYPosition() <= 575  )
		{
			if( CGamePlayState::GetInstance()->GetPauseBool() == true )
				CGame::GetInstance()->PopState( false );
			else
				CGame::GetInstance()->PopState(  );
			return true;
		}
		return true;
	}

	if( m_pDI->KeyPressed( DIK_ESCAPE ) || m_pDI->JoystickButtonPressed( 0 ) )
		{
			if( CGamePlayState::GetInstance()->GetPauseBool() == true )
				CGame::GetInstance()->PopState( false );
			else
				CGame::GetInstance()->PopState(  );
			return true;
		}

	if( m_pDI->KeyPressed( DIK_DOWN ) ||  m_pDI->KeyPressed( DIK_S ) || m_pDI->JoystickGetLStickDirPressed( DIR_DOWN ) )
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			SetCursorPos( GetCursorPosition() + 1 );

			if( GetCursorPosition() >= 3 )
				SetCursorPos( 0 );

			return true;
		}

		if( m_pDI->KeyPressed( DIK_UP ) ||  m_pDI->KeyPressed( DIK_W ) || m_pDI->JoystickGetLStickDirPressed( DIR_UP ) )
		{
			m_pC->SetRenderBool( false );
			m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
			SetCursorPos( GetCursorPosition() - 1 );

			if( GetCursorPosition() < 0 )
				SetCursorPos( 2 );

			return true;
		}


		if( GetCursorPosition() == 0  )
		{
			if( m_pDI->KeyPressed( DIK_RIGHT) ||
				m_pDI->KeyPressed( DIK_D) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) )
			{
				if(m_pXA->MusicGetMasterVolume() > 1.0f || 
					GetVolumeStruct().GetMusicVolume() + 5 > 100 )
				{
					GetVolumeStruct().SetMusicVolume( 100 );
					m_pXA->MusicSetMasterVolume( 1.0f );
					return true;
				}
				else
				{
					m_pXA->MusicSetMasterVolume( m_pXA->MusicGetMasterVolume() + 0.050000f );
					GetVolumeStruct().SetMusicVolume( GetVolumeStruct().GetMusicVolume() + 5 );
					return true;
				}
			}
			else if( m_pDI->KeyPressed( DIK_LEFT) ||
				m_pDI->KeyPressed( DIK_A ) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_LEFT ) )
			{
				if(m_pXA->MusicGetMasterVolume() < 0.0f || 
					GetVolumeStruct().GetMusicVolume() - 5 < 0 )
				{
					GetVolumeStruct().SetMusicVolume( 0 );
					m_pXA->MusicSetMasterVolume( 0.0f );
					return true;
				}
				else
				{
					m_pXA->MusicSetMasterVolume( m_pXA->MusicGetMasterVolume() - 0.050000f );
					GetVolumeStruct().SetMusicVolume( GetVolumeStruct().GetMusicVolume() - 5 );
					return true;
				}
			}
		}


		if( GetCursorPosition() == 1  )
		{
			if( m_pDI->KeyPressed( DIK_RIGHT) ||
				m_pDI->KeyPressed( DIK_D) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) )
			{
				if(m_pXA->SFXGetMasterVolume() > 1.0f || 
					GetVolumeStruct().GetSFXVolume() + 5 > 100 )
				{
					GetVolumeStruct().SetSFXVolume( 100 );
					m_pXA->SFXSetMasterVolume( 1.0f );
					return true;
				}
				else
				{
					m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
					m_pXA->SFXSetMasterVolume( m_pXA->SFXGetMasterVolume() + 0.050000f );
					GetVolumeStruct().SetSFXVolume( GetVolumeStruct().GetSFXVolume() + 5 );
					return true;
				}
			}
			else if( m_pDI->KeyPressed( DIK_LEFT) ||
				m_pDI->KeyPressed( DIK_A ) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_LEFT ) )
			{
				if(m_pXA->SFXGetMasterVolume() < 0.0f || 
					GetVolumeStruct().GetSFXVolume() - 5 < 0 )
				{
					GetVolumeStruct().SetSFXVolume( 0 );
					m_pXA->SFXSetMasterVolume( 0.0f );
					return true;
				}
				else
				{
					m_pXA->SFXPlaySound( m_pFM->GetSoundIdMap()["Cursor_Move"].nId );
					m_pXA->SFXSetMasterVolume( m_pXA->SFXGetMasterVolume() - 0.050000f );
					GetVolumeStruct().SetSFXVolume( GetVolumeStruct().GetSFXVolume() - 5 );
					return true;
				}
			}
		}

		if( GetCursorPosition() == 2 )
		{
			if( m_pDI->KeyPressed( DIK_LEFT) ||
				m_pDI->KeyPressed( DIK_A ) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_LEFT ) )
			{
				if( GetVolumeStruct().b_FullScreen == 0 )
				{
					GetVolumeStruct().b_FullScreen = 1;
					CGame::GetInstance()->FullScreenMode( true );
				}
				else
				{
					GetVolumeStruct().b_FullScreen = 0;
					CGame::GetInstance()->FullScreenMode( false );
				}
			}
			else if( m_pDI->KeyPressed( DIK_RIGHT ) ||
				m_pDI->KeyPressed( DIK_D ) ||
				m_pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) )
			{
				if( GetVolumeStruct().b_FullScreen == 0 )
				{
					GetVolumeStruct().b_FullScreen = 1;
					CGame::GetInstance()->FullScreenMode( true );
				}
				else
				{
					GetVolumeStruct().b_FullScreen = 0;
					CGame::GetInstance()->FullScreenMode(false );
				}
			}
			return true;
		}


	return true;
}
void COptionState::Update(float fElapsedTime)
{
	m_pC->Update( fElapsedTime );
	if( m_pFM->GetAlpha() < 255 )
		m_pFM->IncrementAlpha();
}
void COptionState::Render(void)
{

	CGame*	pGame	=	CGame::GetInstance();

	//int	nWidth = C
	RECT rBackground = { 750, 0, 
		1880, 625 };
	if( m_pFM->GetAlpha() < 255 )
	{
		m_pTM->Draw( m_pFM->GetImageID( "World_Background_3"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(  m_pFM->GetAlpha(), 255, 255, 255 ) );

	}
	else
	{
		m_pTM->Draw( m_pFM->GetImageID( "World_Background_3"), 0, 0, 1.0f, 1.3f, &rBackground,
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(  255, 255, 255, 255 ) );
	}

	m_pBMP->Print( "Options", CGame::GetInstance()->GetWidth() / 2, 
		50, 1.0f,  4294967295UL, 23 );


	switch( GetCursorPosition() )
	{
	case MUSIC:
		m_pTM->Draw( 		m_pFM->GetImageIdMap()["Game_Cursor"].nId, 
			m_POcursorPos[GetCursorPosition()].x - 100, m_POcursorPos[GetCursorPosition()].y, 
			0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case SOUNDEFFECTS:
		m_pTM->Draw( 	m_pFM->GetImageIdMap()["Game_Cursor"].nId, 
			m_POcursorPos[GetCursorPosition()].x- 100, m_POcursorPos[GetCursorPosition()].y, 
			0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case FULLSCREEN:
		m_pTM->Draw( 	m_pFM->GetImageIdMap()["Game_Cursor"].nId, 
			m_POcursorPos[GetCursorPosition()].x- 100, m_POcursorPos[GetCursorPosition()].y, 
			0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case GOBACK:
		m_pTM->Draw( 	m_pFM->GetImageIdMap()["Game_Cursor"].nId, 
			m_POcursorPos[GetCursorPosition()].x - 100, m_POcursorPos[GetCursorPosition()].y, 
			0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	default:
		break;
	};

	m_pBMP->Print( "Music Volume:", m_POcursorPos[0].x, m_POcursorPos[0].y, 1.0f,  4294967295UL, 23 );
	m_pBMP->Print( "Sound Volume:", m_POcursorPos[1].x, m_POcursorPos[1].y, 1.0f,  4294967295UL, 23 );
	m_pBMP->Print( "Full Screen:", m_POcursorPos[2].x, m_POcursorPos[2].y, 1.0f,  4294967295UL, 23 );

	if( GetVolumeStruct().b_FullScreen == 1 )
		m_pBMP->Print( "On", m_POcursorPos[2].x + 200, m_POcursorPos[2].y, 1.0f,  4294967295UL, 23 );
	else
		m_pBMP->Print( "Off", m_POcursorPos[2].x + 200, m_POcursorPos[2].y, 1.0f,  4294967295UL, 23 );

	//ShowVolumes();

	std::stringstream tempss;
	tempss << GetVolumeStruct().m_nMusicVolume;
	m_pBMP->Print( tempss.str().c_str(),  m_POcursorPos[0].x + 200,  m_POcursorPos[0].y, 1.0f,  4294967295UL, 23 );

	tempss.clear();
	std::stringstream t;
	t << GetVolumeStruct().m_nSFXVolume;
	m_pBMP->Print( t.str().c_str(), m_POcursorPos[1].x + 200 , m_POcursorPos[1].y , 1.0f,  4294967295UL, 23 );

	CFileManager*	pFM	=	 CFileManager::GetInstance();

	RECT buttonTest = { 0, 0, 164, 62 };
	int nButtonHeight = m_pTM->GetTextureHeight( pFM->GetImageIdMap()["Button"].nId );
	m_pTM->Draw( pFM->GetImageIdMap()["Button"].nId, 600, 520 , 0.95f, 0.75f, &buttonTest);
	m_pBMP->Print( "Back", 620,  535, 1.0f,  4294967295UL, 23 );
	m_pD3D->GetSprite()->Flush();
	m_pC->Render();
}


COptionState::~COptionState(void)
{
}