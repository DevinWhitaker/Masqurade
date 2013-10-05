/*
	File:		IntroState.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Shows a cutscene at the beginning of a new game
				( Work in progress )
*/
#include "IntroState.h"
#include "MainMenuState.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "BitmapFont.h"
#include "FileManager.h"


CIntroState* CIntroState::GetInstance(void)
{
	static CIntroState s_Instance;

	return &s_Instance;
}

CIntroState::CIntroState(void)
{
	m_pD3D	 = nullptr;
	m_pDI 	 = nullptr;
	m_pTM 	 = nullptr;
	m_pXA 	 = nullptr;
	m_pBM	 = nullptr;

	m_MousePos.x = 0;
	m_MousePos.y = 0;

	fTime = 0.0f;
	fMoveX = 0.0f;
	fMoveY = 0.0f;
	fShrinkX = 1.0f;
	fShrinkY = 1.0f;

	fMoveX1 = -850.0f;

	isOutro = false;

	nLogoImageID = -1;
	isLogoCornered = false;

	for( int i = 0; i < 10; i++ )
		nCutSceneID[ i ] = -1;
}

void CIntroState::Enter(void)
{
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	m_pBM = CBitmapFont::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pFM	= CFileManager::GetInstance();

	CFileManager::volumeStruct holdVolumeData;
	if( m_pFM->LoadSettings( "Resources/Scripts/Settings.xml", holdVolumeData ) == false )
		CGame::GetInstance()->FullScreenMode( false );
	else
	{
		if( holdVolumeData.b_FullScreen == 1 )
			CGame::GetInstance()->FullScreenMode( true );
		else
			CGame::GetInstance()->FullScreenMode( false );
	}

	nLogoImageID		= m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_JAMDLogoSmall.png") );
	nCutSceneID[ 0 ]	=  m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Scene_1.png") );
	nCutSceneID[ 1 ]	=  m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Scene_2.png") );
}

void CIntroState::Exit(void)
{	
	m_pFM	 = nullptr;
	m_pD3D	 = nullptr;
	m_pDI 	 = nullptr;
	m_pTM 	 = nullptr;
	m_pXA 	 = nullptr;
	m_pBM    = nullptr;
	m_MousePos.x = 0;
	m_MousePos.y = 0;	
}

bool CIntroState::Input(void)
{
#if !ARCADE
	if( m_pDI->GetDIKCode()  || m_pDI->MouseButtonPressed( 0 ) ||
			m_pDI->MouseButtonPressed( 1 ) ||
			m_pDI->MouseButtonPressed( 2 ) )
	{
		CGame::GetInstance()->PushState( CMainMenuState::GetInstance() );
	}
	return true;
#else
	if( m_pDI->JoystickCheckBufferedButtons() != -1 )
	{
		CGame::GetInstance()->PushState( CMainMenuState::GetInstance() );
	}
	return true;
#endif
}
void CIntroState::Update(float fElapsedTime)
{
	if( fMoveX < 205 )
		fMoveX = fMoveX + 80.0f * fElapsedTime;
	if( fMoveY < 325 )
		fMoveY = fMoveY + 85.0f * fElapsedTime;
	if( fShrinkY > 0.45f )
		fShrinkY = fShrinkY - 0.15f * fElapsedTime;

	if( fShrinkY == 0.45 )
	{
		isLogoCornered = true;
	}

	if( fMoveX1 < 1400 )
			fMoveX1 = fMoveX1 + 50.0f * fElapsedTime;
	else
		CGame::GetInstance()->PushState( CMainMenuState::GetInstance() );
}

void CIntroState::Render(void)
{
	float fX, fY;
	fX = (float)(m_pGame->GetWidth() / 2.0f);
	fY = (float)(m_pGame->GetHeight() / 2.0f);

	m_pTM->Draw( nCutSceneID[ 0 ], (int)(fMoveX1), 50, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DXCOLOR( 255, 255, 255, 255 ) );
	m_pTM->Draw( nCutSceneID[ 1 ], (int)(fMoveX1 - 600), 50, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DXCOLOR( 255, 255, 255, 255 ) );


	// Draw the the X and Y from a set number from Constructor then decrease over time. Also scale to a certain amount.
	if( nLogoImageID != -1 )
	{
		m_pTM->Draw( nLogoImageID, (int)(225 - fMoveX), (int)(150 + fMoveY), fShrinkY, fShrinkY, 0, 0.0f, 0.0f, 0.0f, D3DXCOLOR( 255, 255, 255, 255 ) );
	}

	
}

CIntroState::~CIntroState(void)
{

}
