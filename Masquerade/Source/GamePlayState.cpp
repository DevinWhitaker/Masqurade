/*
File:		GamePlayState.cpp
Course:		SGP1
Author:		Ja'Michael Garcia
Purpose:	Handle gameplay components.
*/
#define NUM_FUNCTIONS 104

/***********************************************/
#include "GamePlayState.h"
#include "OptionState.h"
#include "MainMenuState.h"
#include "CreditState.h"

#include "Game.h"
/***********************************************/
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
/***********************************************/
#include "Message.h"
#include "CreateEmitterMessage.h"
#include "DestroyEmitterMessage.h"
#include "CreateBulletMessage.h"
#include "DestroyBulletMessage.h"
#include "CreateEnemyMessage.h"
#include "DestroyEnemyMessage.h"
#include "ResetMessage.h"
/***********************************************/
#include "HUD.h"
#include "Fox.h"
#include "Corinne.h"
#include "Clown.h"
#include "Balloon.h"
#include "Checkpoint.h"
#include "Bullet.h"
#include "ITool.h"
#include "SquirtGun.h"
#include "CottonCandyLauncher.h"
#include "BalloonGun.h"
#include "Hoodlum.h"
#include "Gate.h"
#include "PressurePlate.h"
#include "SecurityCamera.h"
#include "SpawnPoint.h"
#include "NPC.h"
/***********************************************/
#include "TileManager.h"
#include "FileManager.h"
#include "AnimationManager.h"
#include "AnchorAnimation.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
/***********************************************/
#include "IdleState.h"
#include "SeekState.h"
/***********************************************/
#include "BitmapFont.h"
#include "Camera.h"
/***********************************************/
#include "Enemy.h"
#include "Rat.h"
#include "Crate.h"
#include "Lever.h"
#include "Spring.h"
/***********************************************/

CGamePlayState* CGamePlayState::GetInstance(void)
{
	static CGamePlayState s_Instance;

	return &s_Instance;
}

CGamePlayState::CGamePlayState(void)
{
	m_nFrames		= 0;
	m_fFrameTimer	= 0;
	m_nFPS = 0;
	SetPauseBool( false );
	SetDebugBool( false );
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;
	m_pMS			= nullptr;
	m_pES			= nullptr;
	m_pOF			= nullptr;
	m_pOM			= nullptr;
	m_pPM			= nullptr;
	m_Fox			= nullptr;
	m_cCorinne		= nullptr;
	m_pTileManager	= nullptr;
	m_pCamera		= nullptr;
	m_pBMF			= nullptr;
	m_pFM			= nullptr;
	m_pAM			= nullptr;

	int initialX = ( CGame::GetInstance()->GetWidth() / 2 ) - 250;
	int initialY = 200;
	m_fLoadingPercent = 0.0f;
	int add = 50;
	m_nPointer = RESUME;
	m_bMessageTest = false;
	SetLevelFile( "" );
	
	m_nCurStage = 0;

	for( int i = 0; i < (SKIP + 1); ++i)
	{
		m_POCursorPos[i].x = initialX;
		m_POCursorPos[i].y = initialY + ( add * i );
	}

	m_nBulletImageTemp = -1;
	SetMusicSwitchBool( true );
}

void CGamePlayState::Enter(void)
{
	// Multi Threaded Loading Screen
	StartLoadingScreen();

	// Creating Fox and Corinne objects
	m_Fox			=	new CFox();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_cCorinne		=	new CCorinne();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);

	// Registering all Objects into the factory in World Engine
	m_pTileManager->RegisterObjects();

	//// =======================
	////	Win Loose Conditions
	//// =======================	
	//m_bGameOver = false;
	//m_bStageComplete = false;
	//m_bWin = false;
	//m_nNumOfStages = 3;
	//m_nCurStage = 0;
	////m_fStageEndMarker = 900.0f;

	SetLevelFile( "" );

	// Access all the SGD Wrappers.
	m_pD3D			=	CSGD_Direct3D::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pDI			=	CSGD_DirectInput::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pTM			=	CSGD_TextureManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pXA			=	CSGD_XAudio2::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pMS			=	CSGD_MessageSystem::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pES			=	CSGD_EventSystem::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	// Access all the Managers, Systems and Factories.
	m_pOF			=	CObjectFactory::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pOM			=	CObjectManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pPM			=	CParticleManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pTileManager	=	CTileManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pCamera		=	CCamera::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pBMF			=	CBitmapFont::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM			=	CFileManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pAM			=	CAnimationManager::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	// misc
	m_pHUD			=	CHUD::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	pIdle			=	new CIdleState();


	//m_pTM
	SetDebugBool( false );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
#if _ADAM
	m_pTileManager->LoadTileWorld( "Resources/Scripts/adams_testworld.xml", 1 );
	m_pTileManager->LoadObjects( "Resources/Scripts/testworld.xml", 1 );
#else if !_ADAM
	/****************************************************************************/
	// Load Garden Level 1
	//m_pTileManager->LoadTileWorld( "Resources/Scripts/testworld.xml", 1 );
	//m_pTileManager->LoadObjects( "Resources/Scripts/testworld.xml", 1 );
	/****************************************************************************/
#endif

	/****************************************************************************/
	//nId = m_pTM->LoadTexture(_T("Resources/Images/Particles/Twinkle.png"));	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pPM->LoadParticles( "Resources/Scripts/Particles.xml" );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	/****************************************************************************/

	/****************************************************************************/
	// Loading Powerup Images
	m_pFM->AddImageID( "normal.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_normal.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "greased_pig.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_greased_pig.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "cheetah.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_cheetah.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "pogo.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_pogo.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "flubber.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_flubber.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "fizzy_lifting_drink.png", m_pTM->LoadTexture( _T("Resources/Images/Powerups/MASQ_fizzy_lifting_drink.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	/****************************************************************************/

	/****************************************************************************/
	// Loading Tool HUD images
	//m_pFM->AddImageID( "Squirt_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Squirt_Gun_HUD.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "Squirt_Gun_Bullet.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Squirt_Gun_Bullet.png")) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	//m_pFM->AddImageID( "Balloon_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Balloon_Gun_HUD.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "MASQ_Balloon_Bullet.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Balloon_Bullet.png")) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	//m_pFM->AddImageID( "CottonCandy_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_CottonCandy_Gun_HUD.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "CottonCandy_Gun_Bullet.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_CottonCandy_Gun_Bullet.png")) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	/****************************************************************************/
	
	/****************************************************************************/
	// Loading Object images
	m_pFM->AddImageID( "MASQ_Pressure_Plate.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Pressure_Plate.png")));
	m_pFM->AddImageID( "MASQ_Spring.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Spring.png")));
	m_pFM->AddImageID( "MASQ_Crate.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Crate.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddImageID( "MASQ_Red_Balloon.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Red_Balloon.png") ) );
	m_pFM->AddImageID( "MASQ_Platform_Balloon.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Platform_Balloon.png") ) );
	
	m_pFM->AddImageID( "MASQ_Pie_Bullet.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Pie_Bullet.png")));
	/****************************************************************************/
		m_pFM->AddImageID( "Squirt_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Squirt_Gun_HUD.png") ) );	 
	m_pFM->AddImageID( "CottonCandy_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_CottonCandy_Gun_HUD.png") ) );	
	m_pFM->AddImageID( "Game_Cursor", m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_GameCursor.png") ) );	
	m_pFM->AddImageID( "Balloon_Gun_HUD.png", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Balloon_Gun_HUD.png") ) );	
	/****************************************************************************/
	// Loading Menu images
	m_pFM->AddImageID( "Pause_Screen", m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Pause_Effect.png") ) );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	// Loading Bitmap Font
	m_pBMF->Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	
	//	Loading Corinne Indicator
	m_pFM->AddImageID( "MASQ_Corinne_Indicator.png",
		m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_Corinne_Indicator.png" ) ) );
	/****************************************************************************/

	/*****************************SFX************************************/
	m_pFM->AddSoundID( "MASQ_Squirt_Shoot", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_Squirt_Shoot.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddSoundID( "MASQ_BalloonGun_Shoot", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_BalloonGun_Shoot.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddSoundID( "MASQ_CCL_Shoot", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_CCL_Shoot.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddSoundID( "MASQ_Gate", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_Gate.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddSoundID( "MASQ_Powerup", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_Powerup.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);
	m_pFM->AddSoundID( "MASQ_Fireworks", m_pXA->SFXLoadSound( _T("Resources/Sound/SFX/MASQ_Fireworks.wav" ) ) );UpdateLoad(m_fLoadingPercent,CSEC);

	
	m_pFM->AddSoundID( "World_1",
		m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_1.xwm") ), true );UpdateLoad(m_fLoadingPercent,CSEC);

	m_pFM->AddSoundID( "World_3",
		m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_2.xwm") ), true );UpdateLoad(m_fLoadingPercent,CSEC);

	m_pFM->AddSoundID( "World_4",
		m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_3.xwm") ), true );UpdateLoad(m_fLoadingPercent,CSEC);

	m_pFM->AddSoundID( "World_0",
		m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_World_0.xwm") ), true );UpdateLoad(m_fLoadingPercent,CSEC);

	m_pFM->AddSoundID( "World_2",
		m_pXA->MusicLoadSong( _T("Resources/Sound/Music/MASQ_Pikachu.xwm") ), true );UpdateLoad(m_fLoadingPercent,CSEC);
	/*******************************************************************/
	m_pMS->InitMessageSystem( &MessageProc );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);

	m_bMessageTest = false;
	


	/*if( m_pFM->LoadGame( "Resources/Scripts/Save.xml", loading )  != false )
	{
	m_Fox->SetX( loading.pFox->GetX() );
	m_Fox->SetY( loading.pFox->GetY() );
	}
	else*/
	


	CMainMenuState* m_pMM			=	CMainMenuState::GetInstance();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	bool	bReturnToMenu = false;
	if( m_pMM->GetTutorialBool() == true )
	{
#if !ARCADE
		SetLevelFile( "Resources/Scripts/Tutorial.xml" );
		SetCurStage( 0 );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/Tutorial.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		m_pTileManager->LoadObjects( "Resources/Scripts/Tutorial.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		//m_pTileManager->LoadTileWorld( "Resources/Scripts/Testing.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		//m_pTileManager->LoadObjects( "Resources/Scripts/Testing.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
#else
		SetLevelFile( "Resources/Scripts/Tutorial_Arcade.xml
			
			" );
		SetCurStage( 0 );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/Tutorial_Arcade.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		m_pTileManager->LoadObjects( "Resources/Scripts/Tutorial_Arcade.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
#endif
	}
	else
	{
		std::string szLevel = "Resources/Scripts/Save_";
		switch ( m_pFM->GetSaveToLoad() )
		{
		case 1:
			{

				std::ostringstream oss;
				oss << m_pFM->GetSaveToLoad();
				szLevel += oss.str().c_str();
				szLevel += ".xml";
				CFileManager::playerProgress loading;
				if( m_pFM->LoadGame( szLevel.c_str(), loading, "", false ) == false ) 
				{
					MessageBox(HWND_DESKTOP, 
						_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
						_T("Masquerade"), MB_OK | MB_ICONERROR);
					bReturnToMenu = true;
				}
				else
				{
					UpdateLoad(m_fLoadingPercent,CSEC);
					SetLevelFile( loading.szLevelPath );
					CFileManager::playerProgress loading2 = {};
					if( m_pFM->LoadGame( loading.szFileName.c_str(),
						loading2, loading.szLevelPath.c_str() ) == false )
					{
						MessageBox(HWND_DESKTOP, 
							_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
							_T("Masquerade"), MB_OK | MB_ICONERROR);
						bReturnToMenu = true;
					}
					else
					{
						SetCurStage( loading2.nLevelNum );
						SetMusicSwitchBool( true );
						UpdateLoad(m_fLoadingPercent,CSEC);
					}
				}
			}
			break;
		case 2:
			{
				std::ostringstream oss;
				oss << m_pFM->GetSaveToLoad();
				szLevel += oss.str().c_str();
				szLevel += ".xml";
				CFileManager::playerProgress loading;
				if( m_pFM->LoadGame( szLevel.c_str(), loading, "", false ) == false ) 
				{
					MessageBox(HWND_DESKTOP, 
						_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
						_T("Masquerade"), MB_OK | MB_ICONERROR);
					bReturnToMenu = true;
				}
				else
				{
					UpdateLoad(m_fLoadingPercent,CSEC);
					SetLevelFile( loading.szLevelPath );
					CFileManager::playerProgress loading2 = {};
					if( m_pFM->LoadGame( loading.szFileName.c_str(),
						loading2, loading.szLevelPath.c_str() ) == false )
					{
						MessageBox(HWND_DESKTOP, 
							_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
							_T("Masquerade"), MB_OK | MB_ICONERROR);
						bReturnToMenu = true;
					}
					else
					{
						UpdateLoad(m_fLoadingPercent,CSEC);
						SetCurStage( loading2.nLevelNum );
						SetMusicSwitchBool( true );
					}
				}
			}
			break;
		case 3:
			{
				std::ostringstream oss;
				oss << m_pFM->GetSaveToLoad();
				szLevel += oss.str().c_str();
				szLevel += ".xml";
				CFileManager::playerProgress loading;
				if( m_pFM->LoadGame( szLevel.c_str(), loading, "", false ) == false ) 
				{
					MessageBox(HWND_DESKTOP, 
						_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
						_T("Masquerade"), MB_OK | MB_ICONERROR);
					bReturnToMenu = true;
				}
				else
				{
					UpdateLoad(m_fLoadingPercent,CSEC);
					SetLevelFile( loading.szLevelPath );
					CFileManager::playerProgress loading2 = {};
					if( m_pFM->LoadGame( loading.szFileName.c_str(),
						loading2, loading.szLevelPath.c_str() ) == false )
					{
						MessageBox(HWND_DESKTOP, 
							_T("ERROR: The save is either missing or corrupted. Going back to the main menu"),
							_T("Masquerade"), MB_OK | MB_ICONERROR);
						bReturnToMenu = true;
					}
					else
					{
						UpdateLoad(m_fLoadingPercent,CSEC);
						SetCurStage( loading2.nLevelNum );
						SetMusicSwitchBool( true );
					}
				}
			}
			break;
		default:
			{
				m_pFM->SetSaveToLoad( m_pFM->GetCurrentSaveToLoad() );
#if !ARCADE
		SetLevelFile( "Resources/Scripts/Tutorial.xml" );
		SetCurStage( 0 );
		//GoToNextStage(GetCurStage());
		SetMusicSwitchBool( true );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/Tutorial.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		m_pTileManager->LoadObjects( "Resources/Scripts/Tutorial.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
#else
		SetLevelFile( "Resources/Scripts/Tutorial_Arcade.xml" );
		m_nCurStage = 0;
		//GoToNextStage(GetCurStage());
		SetMusicSwitchBool( true );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/Tutorial_Arcade.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		m_pTileManager->LoadObjects( "Resources/Scripts/Tutorial_Arcade.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);

#endif
			}
			break;
		};

		//if( m_pFM->GetSaves().size() != 0 )
		//{
			//m_pTileManager->LoadTileWorld( "Resources/Scripts/World_1.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
			//m_pTileManager->LoadObjects( "Resources/Scripts/World_1.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
		//}
	}

	//GoToNextStage(GetCurStage());


	//ITool* squirt = new CSquirtGun();
	//m_pOM->AddObject(squirt);
	//m_Fox->AddATool(squirt);
	//else
	//{
	//	//if( m_pFM->GetSaves().size() != 0 )
	//	//{
	//		m_pTileManager->LoadTileWorld( "Resources/Scripts/World_1.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	//		m_pTileManager->LoadObjects( "Resources/Scripts/World_1.xml", 1 );	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	//	//}
	//}

	
	CBaseObject::sDataToPassIn sData = { m_Fox ,m_cCorinne};
	m_cCorinne->SetCurrentState( pIdle );
	m_cCorinne->GetCurrentState()->SetObjectsInvolved( sData );
	//m_cCorinne->GetCurrentState()->Enter( m_cCorinne->GetCurrentState()->GetObjectsInvolved().pCarried, 
	//	m_cCorinne->GetCurrentState()->GetObjectsInvolved().pCarrier );
	m_cCorinne->ChangeState( pIdle );
	//pIdle->Enter( m_cCorinne, m_Fox );
	m_Fox->SetDaughter( m_cCorinne );
	m_Fox->AddAPowerUp(CPowerup::NORMAL);

//#if TRUE//_DEVIN
	////CAnimationManager::GetInstance()->CreateAnimationFromFile("Resources/Scripts/destroythis.xml");
	//CSpawnPoint* pSpawn = new CSpawnPoint;
	//pSpawn->SetX( 5200 ); pSpawn->SetY( 350 );
	//CObjectManager::GetInstance()->AddObject( pSpawn );
	//pSpawn->SetEvent( "spawn" );//must be the same as the security camera's ID
	//pSpawn->SetEnemySpawType( NME_CLOWN );
	//CSecurityCamera* CTOS = new CSecurityCamera;
	//CTOS->SetX( 5550 ); CTOS->SetY( 350 );
	//CTOS->SetID( "spawn" );//must be the same as the spawnpoints event
	//CObjectManager::GetInstance()->AddObject( CTOS );
	////CGate* pGate = new CGate();
	////pGate->SetX(500);pGate->SetY(375);
	////pGate->SetGravityBool( false );
	////CObjectManager::GetInstance()->AddObject(pGate);
	//CObjectManager::GetInstance()->AddObject( CNPC::GetInstance() );
//#endif

	// Init HUD *AFTER* Fox & Corinne exist
	m_pHUD->Init();	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);
	// Same goes for Item Menu
	m_ItemMenu.Init(400, 400);	/*Call After Every Function*/UpdateLoad(m_fLoadingPercent,CSEC);

	//this->SetStageEndMarker(1900.0f);

	// =======================
	//	Win Loose Conditions
	// =======================	
	m_bGameOver = false;
	m_bStageComplete = false;
	m_bWin = false;
	m_nNumOfStages = 4;
	m_fFireWorkTimer = 0.0f;
	m_fFireWorkDuration = 10.0f;
	m_bNewFirework = true;
	m_fFireWorkInterval = 0.5f;
	m_fFireWorkNewTimer = 0.0f;
	m_fDeadTime = 0.0f;
	m_fPerodicFwTimer = 0.0f;
	m_fFwPeroid = 10.0f;

	m_fGameOverTimer = 0.0f;
	m_fWinTimer = 0.0f;
	m_bCameraMode = false;

	m_bSplash = false;
	m_bSplashOnce = false;

	m_fFrameTimer = 0;
	m_nFrames = 0;
	m_nFPS = 60;

	//Multi Threaded Loading Screen
		if( m_pXA->MusicIsSongPlaying( m_pFM->GetSoundID( "Main_Menu_Music" ) ) == true )
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "Main_Menu_Music" ) );

	StopLoadingScreen();

	if(  bReturnToMenu == true )
		CGame::GetInstance()->PopState( );

	//m_nCurStage = 4;
	m_nPikachuKills = 0;

	/*m_Fox->SetX(5200);
	m_cCorinne->SetX(5200);*/
}

void CGamePlayState::Exit(void)
{
	SetLevelFile( "" );
	//this->SetStageEndMarker( 20000.0f );
	m_ItemMenu.Shutdown();
	m_bMessageTest = false;
	m_pPM->Shutdown();
	m_pBMF->Shutdown();
	m_pCamera->Reset();
	pIdle = nullptr;
	m_fLoadingPercent = 0.0f;

	switch( GetCurStage() )
	{
	case 0:
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_0" ) );
		break;
	case 1:
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_1" ) );
		break;
	case 2:
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_2" ) );
		break;
	case 3:
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_3" ) );
		break;
	case 4:
		m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_4" ) );
		break;
	};

	//m_pFM->ReleaseTexture( nTileidtest );

	if( m_pPM != nullptr )
	{
		m_pPM->Shutdown();
		m_pPM = nullptr;
	}

	//	Clean up the Object Factory
	if( m_pOF != nullptr )
	{
		m_pOF->ShutdownObjectFactory();
		m_pOF = nullptr;
	}

	//	Release all the objects in the Object Manager
	if( m_pOM != nullptr )
	{
		m_pOM->RemoveAllObjects();
		CObjectManager::DeleteInstance();
		m_pOM = nullptr;
	}

	if( m_pES != nullptr )
	{
		m_pES->ClearEvents();
		m_pES->ShutdownEventSystem();
		m_pES = nullptr;
	}

	if( m_pAM != nullptr )
	{
		m_pAM->Shutdown();
		m_pAM = nullptr;
	}

	if( m_pTileManager != nullptr )
	{
		//m_pTileManager->UnloadLevel();
		m_pTileManager->RemoveAllTiles();
		m_pTileManager = nullptr;
	}

	SetPauseBool( false );
	SetDebugBool( false );
	SetCursorValue( RESUME );
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;
	m_pMS			= nullptr;
	m_pES			= nullptr;
	m_pOF			= nullptr;
	m_pOM			= nullptr;
	m_pCamera		= nullptr;
	m_pBMF			= nullptr;
	m_pFM			= nullptr;
	m_fFrameTimer = 0;
	m_nFrames = 0;
	m_nFPS = 0;

	//delete m_Fox;
	//delete m_cCorinne;
}

bool CGamePlayState::Input(void)
{

#if !ARCADE
	if( m_pDI->KeyPressed( DIK_ESCAPE ) )
	{
		if( GetPauseBool() == false )
			SetPauseBool( true );
		else
			SetPauseBool( false );
		SetCursorValue( RESUME );
		m_ItemMenu.setIsMainPause(GetPauseBool());
		return true;
	}
#else
	if( m_pDI->JoystickButtonPressed( 6 ) )
	{
		if( GetPauseBool() == false )
			SetPauseBool( true );
		else
			SetPauseBool( false );
		SetCursorValue( RESUME );
		m_ItemMenu.setIsMainPause(GetPauseBool());
		return true;
	}
#endif


	if( m_pDI->KeyDown( DIK_LALT ) && m_pDI->KeyDown( DIK_TAB ) )
	{
		SetPauseBool( true );
		m_ItemMenu.setIsMainPause(GetPauseBool());
		return true;
	}

	//if( m_pDI->KeyDown( DIK_RETURN )  && (m_pDI->KeyDown( DIK_LALT ) || m_pDI->KeyDown( DIK_RALT ))  )
	//{
	//	CGame::GetInstance()->FullScreenMode();
	//	return true;
	//}

#if _DEBUG
	//if( m_pDI->KeyPressed( DIK_H ) )
	//{
	//	if( m_bMessageTest == false)
	//	{
	//		CCreateEmitterMessage*	pWE = new CCreateEmitterMessage( "Colored Wisps", "Test", 0, 0, m_Fox );
	//		m_pMS->SendMsg( pWE );
	//		pWE = nullptr;
	//		m_bMessageTest = true;
	//	}
	//	else
	//	{
	//		CDestroyEmitterMessage*	pWE = new CDestroyEmitterMessage( "Test" );
	//		m_pMS->SendMsg( pWE );
	//		pWE = nullptr;
	//		m_bMessageTest = false;
	//	}
	//	return true;
	//}


	if( m_pDI->KeyPressed( DIK_F9 ) )
	{
		if( GetDebugBool() == false )
			SetDebugBool( true );
		else
			SetDebugBool( false );
		return true;
	}
#endif

#if !ARCADE
	if( GetPauseBool() == true )
	{
		if( m_pDI->KeyPressed( DIK_ESCAPE ) )
		{
			SetPauseBool( false );
			return true;
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) )
		{
			switch( GetCursorValue() )
			{
			case RESUME:
				SetPauseBool( false );
				break;
			case OPTIONS:
				CGame::GetInstance()->PushState( COptionState::GetInstance(), false );
				break;
			case EXIT:
				CGame::GetInstance()->PopState();
				break;
			case LAST_CHECKPOINT:
				{
					CResetMessage* msg = new CResetMessage;
					CSGD_MessageSystem::GetInstance()->SendMsg( msg );
					SetPauseBool( false );
				}
				break;
			case SKIP:
				{
					m_Fox->clearTools();
					m_Fox->clearPowerups();
					std::ostringstream level;
					level << "Resources/Scripts/World_1.xml";
					m_nCurStage = 1;
					SetLevelFile( level.str().c_str() );
					Save( );
					CGame::GetInstance()->PopState(  );
					CGame::GetInstance()->PushState( CGamePlayState::GetInstance()  );
					//GoToNextStage(1);
					SetPauseBool( false );
				}
				break;
			default:
				break;
			};
			return true;
		}

		CMainMenuState* m_pMM = CMainMenuState::GetInstance();

		if( m_pDI->KeyPressed( DIK_UP ) || m_pDI->MouseWheelMovement() > 0
			||  m_pDI->KeyPressed( DIK_W ))
		{
				if( GetCursorValue() <= RESUME )
					SetCursorValue(  ( m_nCurStage == 0 && m_pMM->GetTutorialBool() == false ) ? SKIP : EXIT );
				else
					SetCursorValue( GetCursorValue() - 1 );
				return true;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->MouseWheelMovement() < 0 
			 ||  m_pDI->KeyPressed( DIK_S ))
		{
			int value = ( m_nCurStage == 0 && m_pMM->GetTutorialBool() == false ) ? SKIP : EXIT;
			if( GetCursorValue() >= value )
				SetCursorValue(  RESUME );
			else
				SetCursorValue( GetCursorValue() + 1 );
			return true;
		}
		return true;
	}
	else
	{
		if( m_pDI->KeyPressed( DIK_TAB ) )
			m_ItemMenu.toggleInMenu();

		if(m_ItemMenu.getInMenu())
			m_ItemMenu.Input();

		if(! m_ItemMenu.getInMenu())
		{
			if( m_pDI->KeyPressed(DIK_F) )
			{
				/*m_bCameraMode = ! m_bCameraMode;

				if(m_bCameraMode)
					m_pCamera->exitMove();*/
			}
		}
	}
#else
	if( GetPauseBool() == true )
	{
		if( m_pDI->JoystickButtonPressed( 6 ) )
		{
			SetPauseBool( false );
			return true;
		}

		if( m_pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) || m_pDI->JoystickButtonPressed( 0 ) )
		{
			switch( GetCursorValue() )
			{
			case RESUME:
				SetPauseBool( false );
				break;
			case OPTIONS:
				CGame::GetInstance()->PushState( COptionState::GetInstance(), false );
				break;
			case EXIT:
				CGame::GetInstance()->PopState();
				break;
			case LAST_CHECKPOINT:
				{
					CResetMessage* msg = new CResetMessage;
					CSGD_MessageSystem::GetInstance()->SendMsg( msg );
					SetPauseBool( false );
				}
				break;
			case SKIP:
			{
				GoToNextStage();
				SetPauseBool( false );
			}
			break;
			default:
				break;
			};
			return true;
		}

		CMainMenuState* m_pMM = CMainMenuState::GetInstance();

		if( m_pDI->JoystickGetLStickDirPressed( DIR_UP ) )
		{
			if( GetCursorValue() <= RESUME )
				SetCursorValue(  ( m_nCurStage == 0 && m_pMM->GetTutorialBool() == false ) ? SKIP : EXIT );
			else
				SetCursorValue( GetCursorValue() - 1 );
			return true;
		}

		if( m_pDI->JoystickGetLStickDirPressed( DIR_DOWN )  )
		{
			int value = ( m_nCurStage == 0 && m_pMM->GetTutorialBool() == false  ) ? SKIP : EXIT;
			if( GetCursorValue() >= value )
				SetCursorValue(  RESUME );
			else
				SetCursorValue( GetCursorValue() + 1 );
			return true;
		}
		return true;
	}
	else
	{
		if( m_pDI->JoystickButtonPressed( 2 ) )
			m_ItemMenu.toggleInMenu();

		if(m_ItemMenu.getInMenu())
			m_ItemMenu.Input();

		if(! m_ItemMenu.getInMenu())
		{
			if( m_pDI->KeyPressed(DIK_F) )
			{
			}
		}
	}
#endif
	

#if _DEBUG || ARCADE
	int powKey = m_pDI->GetDIKCode();
	switch( powKey )
	{
	case DIK_1:
		//m_cCorinne->setPowerup(CPowerup::GREASED_PIG);
		break;
	case DIK_2:
		//m_cCorinne->setPowerup(CPowerup::CHEETAH);
		break;
	case DIK_3:
		//m_cCorinne->setPowerup(CPowerup::POGO);
		break;
	case DIK_4:
		//m_cCorinne->setPowerup(CPowerup::FLUBBER);
		break;
	case DIK_5:
		//m_cCorinne->setPowerup(CPowerup::FIZZY_LIFTING_DRINK);
		break;
	case DIK_6:
		//m_cCorinne->setPowerup(CPowerup::NORMAL);
		break;
	}
#endif

	return true;
}
void CGamePlayState::Update(float fElapsedTime)
{

	switch( GetCurStage() )
	{
	case 0:
		if( GetMusicBool() == true )
		{
			m_pXA->MusicPlaySong( m_pFM->GetSoundID( "World_0" ), true );
			SetMusicSwitchBool( false );
		}
		break;
	case 1:
		if( GetMusicBool() == true )
		{
			m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_0" ) );
			m_pXA->MusicPlaySong( m_pFM->GetSoundID( "World_1" ), true );
			SetMusicSwitchBool( false );
		}
		break;
	case 2:
		if( GetMusicBool() == true )
		{
			m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_1" ) );
			m_pXA->MusicPlaySong( m_pFM->GetSoundID( "World_2" ), true );
			SetMusicSwitchBool( false );
		}
		break;
	case 3:
		if( GetMusicBool() == true )
		{
			if( m_pXA->SFXIsSoundPlaying( m_pFM->GetSoundID( "World_2" ) ) == true ) 
				m_pXA->SFXStopSound( m_pFM->GetSoundID( "World_2" ) );
			
			if( m_pXA->SFXIsSoundPlaying( m_pFM->GetSoundID( "World_1" )) == true ) 
				m_pXA->SFXStopSound( m_pFM->GetSoundID( "World_1" ) );
			//m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_2" ) );
			m_pXA->MusicPlaySong( m_pFM->GetSoundID( "World_3" ), true );
			SetMusicSwitchBool( false );
		}
		break;
	case 4:
		if( GetMusicBool() == true )
		{
			m_pXA->MusicStopSong( m_pFM->GetSoundID( "World_3" ) );
			m_pXA->MusicPlaySong( m_pFM->GetSoundID( "World_4" ), true );
			SetMusicSwitchBool( false );
		}
		break;
	default:
		break;
	};

	if( GetPauseBool() == false && m_ItemMenu.getInMenu() == false )
	{
		if( m_fFireWorkTimer < m_fFireWorkDuration && m_bGameOver == false)
		{
			m_fFrameTimer += fElapsedTime;
			m_pPM->UpdateAllParticles(fElapsedTime);
			m_pCamera->Update(fElapsedTime);
			//m_pTileManager->UpdateAllTiles( fElapsedTime );
			m_pOM->UpdateAllObjects(fElapsedTime);
			m_pOM->CheckCollisions();
			m_pMS->ProcessMessages();
			m_pES->ProcessEvents();

			m_fFrameTimer += fElapsedTime;
			m_pAM->UpdateAll(fElapsedTime);
			m_pAM->CheckAllCollision();
		}

		if(m_Fox->GetY() >= 584)
		{
			m_bSplash = true;
		}

		if(m_cCorinne->GetRect().top >= 584 )
		{
			RespawnCorinne();
		}
		
		if(m_Fox->GetY() <= 584)
		{
			m_bSplash = false;
			m_bSplashOnce = true;
		}

		if(m_bSplash && m_bSplashOnce)
		{
			CCreateEmitterMessage*	pPE = new CCreateEmitterMessage( "Splash", "Splash", m_Fox->GetX(), m_Fox->GetY() );
			CSGD_MessageSystem::GetInstance()->SendMsg( pPE );
			pPE = nullptr;
			m_bSplashOnce = false;
		}


		if(m_Fox->GetX() > m_fStageEndMarker && m_cCorinne->GetX() > m_fStageEndMarker && m_fStageEndMarker > 0.0f )
		{
			m_bStageComplete = true;
			if( m_nCurStage == m_nNumOfStages )
			{
				m_bWin = true;
			}
		}
		else if( m_Fox->GetHearts() <= 0 && m_bStageComplete == false && m_fDeadTime > 0.84f )
		{
			m_bGameOver = true;
			m_fDeadTime = 0.0f;
		}

		if( m_Fox->GetHearts() <= 0 && m_bGameOver == false )
		{
			m_Fox->setInputEnabled(false);
			m_fDeadTime += fElapsedTime;
		}
		if( m_fFireWorkTimer >= m_fFireWorkDuration )
			m_fWinTimer += fElapsedTime;

		if( m_bGameOver )
			m_fGameOverTimer += fElapsedTime;

		if( m_nCurStage == 4 )
		{
			m_fFireWorkInterval = 0.2f;
			m_fFireWorkDuration = 20.0f;
		}

		if( m_bStageComplete )
		{
			if( m_bNewFirework && m_fFireWorkTimer <= m_fFireWorkDuration-3.0f )
			{
				if(rand()%2 == 1)
				{
					CCreateEmitterMessage*	pPE = new CCreateEmitterMessage( "Firework Multi 3", "Firework Multi 3", 
						(GetStageEndMarker()-400)+(rand()%800), (float)(300-rand()%200) );
					CSGD_MessageSystem::GetInstance()->SendMsg( pPE );
					pPE = nullptr;
				}
				else
				{
					CCreateEmitterMessage*	pPE2 = new CCreateEmitterMessage( "Firework Multi 5", "Firework Multi 5", 
						(GetStageEndMarker()-400)+(rand()%800), (float)(300-rand()%100) );
					CSGD_MessageSystem::GetInstance()->SendMsg( pPE2 );
					pPE2 = nullptr;
				}
				m_bNewFirework = false;
				m_fFireWorkNewTimer = 0.0f;
			}
			m_fFireWorkTimer += fElapsedTime;
			m_fFireWorkNewTimer += fElapsedTime;

			if( m_fFireWorkNewTimer >= m_fFireWorkInterval )
				m_bNewFirework = true;
		}

		if( m_fGameOverTimer >= 2.0f )
		{
			RespawnFox();
			m_fGameOverTimer = 0.0f;
			m_bGameOver = false;
		}

		if( m_nCurStage == 1 )
		{
			RECT r;
			RECT HiddenPortal;
			SetRect( &HiddenPortal, 3885, 0, 3900, 122 );
			if( IntersectRect( &r, &m_Fox->GetRect(), &HiddenPortal ) )
			{
				std::ostringstream level;
				m_nCurStage++;

				level << "Resources/Scripts/World_" << m_nCurStage;

				level << ".xml";
				std::string test = level.str().c_str();
				printf( test.c_str() );

				SetLevelFile( level.str().c_str() );
				Save( );
				CGame::GetInstance()->PopState(  );
				CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );
			}
		}

		if( m_fWinTimer >= 5.0f )
		{
			m_fWinTimer = 0.0f;
			/*m_pTileManager->RemoveAllTiles();
			m_pOM->RemoveAllObjects();*/
			//m_bWin = false;
			CMainMenuState* m_pMM = CMainMenuState::GetInstance();
			if( m_pMM->GetTutorialBool() == true )
			{
				CGame::GetInstance()->PopState(  );
			}
			else if( m_nCurStage == m_nNumOfStages )
			{
				CGame::GetInstance()->PopState(  );
				CGame::GetInstance()->PushState( CCreditState::GetInstance()  );
			}
			else
			{
				std::ostringstream level;
				if(m_nCurStage == 1)
					++m_nCurStage;
				m_nCurStage++;
				
				if( m_nCurStage == 0 )
					level << "Resources/Scripts/Tutorial";
				else
					level << "Resources/Scripts/World_" << m_nCurStage;

				level << ".xml";
				std::string test = level.str().c_str();
				printf( test.c_str() );

				SetLevelFile( level.str().c_str() );
				Save( );
				CGame::GetInstance()->PopState(  );
				CGame::GetInstance()->PushState( CGamePlayState::GetInstance() );
				//GoToNextStage(); 
			}
		}

		if( m_nCurStage == 4 )
			m_fPerodicFwTimer += fElapsedTime;

		if( m_fPerodicFwTimer >= m_fFwPeroid )
		{
			m_fFwPeroid = (float)(rand()% ( 10 - 15 ) + 10);
			m_fPerodicFwTimer = 0.0f;
			
			CCamera* pCam = CCamera::GetInstance();
			float FWxPos = (m_Fox->GetX()-400)+(rand()%800);
			int bFW5 = (rand()%2);
			CCreateEmitterMessage*	pPE2 = new CCreateEmitterMessage( (bFW5) ? "Firework Multi 5" : "Firework Multi 3",
				(bFW5) ? "Firework Multi 5" : "Firework Multi 3", 
				FWxPos, (float)(300-rand()%100) );
			CSGD_MessageSystem::GetInstance()->SendMsg( pPE2 );
			pPE2 = nullptr;
		}
		//printf("Fox's X: %f Fox's Y: %f\n", m_Fox->GetX(), m_Fox->GetY() );
	}

}
void CGamePlayState::Render(void)
{
	m_nFrames++;
	if( m_fFrameTimer >= 1.0f )
	{
		m_nFPS = m_nFrames;

		m_nFrames = 0;
		m_fFrameTimer = 0;
	}

	m_pTileManager->RenderBack();

	m_pD3D->GetSprite()->Flush();

	m_pTileManager->Render();

	m_pOM->RenderAllObjects();

	m_pAM->RenderAll();

	m_pTileManager->RenderFront();

	if(m_ItemMenu.getInMenu())
		m_pHUD->SetDarkened(true);
	else
		m_pHUD->SetDarkened(false);

	if(GetPauseBool())
	{		
		m_pTM->Draw( m_pFM->GetImageIdMap()["Pause_Screen"].nId, 0, 0, 1.0f,
			1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 155, 255, 255, 255 ) );
		m_ItemMenu.Render();
	}

	// RENDER HUD LAST!!!
	m_pHUD->Render();

	m_pPM->RenderAllParticles();

	// Well... except for item menu
	if(m_ItemMenu.getInMenu())
		m_ItemMenu.Render();

	m_pD3D->GetSprite()->Flush();

	if( m_bGameOver )
	{
		m_pTM->Draw( m_pFM->GetImageIdMap()["Pause_Screen"].nId, 0, 0, 1.0f,
			1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 155, 255, 255, 255 ) );
		m_pBMF->Print( "You Died", CGame::GetInstance()->GetWidth() / 2,
						 100, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
						 23);
	}

	else if( m_bStageComplete )
	{
		if( m_fFireWorkTimer >= m_fFireWorkDuration )
		{
		m_pTM->Draw( m_pFM->GetImageIdMap()["Pause_Screen"].nId, 0, 0, 1.0f,
			1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 155, 255, 255, 255 ) );
		}
		std::string WinTextbuffer;
		WinTextbuffer = (m_nCurStage == m_nNumOfStages) ? "You Win" : "Stage Complete";
		m_pBMF->Print( WinTextbuffer.c_str(), CGame::GetInstance()->GetWidth() / 2,
						 100, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
						 23);
	}
	else if( m_Fox->GetX() > m_fStageEndMarker && m_cCorinne->GetX() < m_fStageEndMarker &&
		GetPauseBool() == false && m_ItemMenu.getInMenu() == false)
	{
		m_pBMF->Print( "Where's Corinne?", CGame::GetInstance()->GetWidth() / 2 - 100,
						 280, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
						 23);
	}

	/*
		std::string WinTextbuffer;
		WinTextbuffer = (m_nCurStage == m_nNumOfStages) ? "You Win" : "Stage Complete";

		m_pBMF->Print( WinTextbuffer.c_str(), CGame::GetInstance()->GetWidth() / 2,
						 100, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
						 23);*/

	if( GetDebugBool() == true )
	{
		m_pBMF->Print( "DEBUG MODE", CGame::GetInstance()->GetWidth() - 200, 20, 1.0f,
			D3DCOLOR_ARGB( 255, 50, 255, 0 ), 23 );

		TCHAR buffer[100] = {};
		TSPRINTF_S( buffer, 100, _T("FPS: %i"), m_nFPS );
		m_pD3D->DrawText( buffer, CGame::GetInstance()->GetWidth() - 200, 50, 255,255,255);

		//pD3D->GetSprite()->Flush();
		TCHAR buffer2[100] = {};
		TSPRINTF_S( buffer2, 100, _T("Tiles Rendered: %i"), m_pTileManager->GetRenderCount() );
		m_pD3D->DrawText( buffer2, CGame::GetInstance()->GetWidth() - 200, 70, 255,255,255);
	}
	if( GetPauseBool() == true )
		PauseGameMenu();

	if( m_nCurStage == 2 )
	{
		std::ostringstream pikaString;
		pikaString << "Pikachu Kills: " << m_nPikachuKills;
		m_pBMF->Print( pikaString.str().c_str(), 550,
						 100, 1.5f, D3DCOLOR_ARGB( 255, 233, 233, 122 ),
						 23);
	}
}


CGamePlayState::~CGamePlayState(void)
{

}

void CGamePlayState::PauseGameMenu( void )
{
	m_pTM->Draw( m_pFM->GetImageIdMap()["Pause_Screen"].nId, 0, 0, 1.0f,
		1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 155, 255, 255, 255 ) );


	switch( GetCursorValue() )
	{
	case RESUME:
		m_pTM->Draw(  m_pFM->GetImageIdMap()["Cursor"].nId, m_POCursorPos[GetCursorValue()].x,
			m_POCursorPos[GetCursorValue()].y, 0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case OPTIONS:
		m_pTM->Draw(  m_pFM->GetImageIdMap()["Cursor"].nId, m_POCursorPos[GetCursorValue()].x,
			m_POCursorPos[GetCursorValue()].y, 0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case LAST_CHECKPOINT:
		m_pTM->Draw(  m_pFM->GetImageIdMap()["Cursor"].nId, m_POCursorPos[GetCursorValue()].x,
			m_POCursorPos[GetCursorValue()].y, 0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case EXIT:
		m_pTM->Draw(  m_pFM->GetImageIdMap()["Cursor"].nId, m_POCursorPos[GetCursorValue()].x,
			m_POCursorPos[GetCursorValue()].y, 0.5f, 0.5f, nullptr, 0, 0, -0.05f );
		break;
	case SKIP:
		m_pTM->Draw(  m_pFM->GetImageIdMap()["Cursor"].nId, m_POCursorPos[GetCursorValue()].x,
			m_POCursorPos[GetCursorValue()].y, 0.5f, 0.5f, nullptr, 0, 0, -0.05f );
	default:
		break;
	};

	m_pBMF->Print( "Pause", CGame::GetInstance()->GetWidth() / 2,
		100, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);

	m_pBMF->Print( "Resume", ( CGame::GetInstance()->GetWidth() / 2 ) - 100,
		200, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);

	m_pBMF->Print( "Options", ( CGame::GetInstance()->GetWidth() / 2 ) - 100,
		250, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);

	m_pBMF->Print( "Last Checkpoint", ( CGame::GetInstance()->GetWidth() / 2 ) - 100,
		300, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);

	m_pBMF->Print( "Exit to Main Menu", ( CGame::GetInstance()->GetWidth() / 2 ) - 100,
		350, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);

	CMainMenuState* m_pMM = CMainMenuState::GetInstance();
	if( m_nCurStage == 0 && m_pMM->GetTutorialBool() == false )
	{
		m_pBMF->Print( "Skip Tutorial", ( CGame::GetInstance()->GetWidth() / 2 ) - 100,
		400, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ),
		23);
	}
}

void CGamePlayState::MessageProc(CMessage* msg)
{
	CGamePlayState*	pGPS = CGamePlayState::GetInstance();

	switch( msg->GetMessageID() )
	{
	case MSG_CREATE_PEMITTER:
		{
			CParticleEmitter*	pPE =  pGPS->GetParticleManager()->CloneEmitterFromTemplate(
				dynamic_cast< CCreateEmitterMessage* >(msg)->GetEmitterName() );

			if( dynamic_cast< CCreateEmitterMessage* >(msg)->GetBaseObject() != nullptr )
			{
				CBaseObject* pBase = dynamic_cast< CCreateEmitterMessage* >(msg)->GetBaseObject();
				pPE->SetAnchor( pBase );
			}
			else
			{
				pPE->SetPos( (float)dynamic_cast< CCreateEmitterMessage* >(msg)->GetXPos(),
					(float)dynamic_cast< CCreateEmitterMessage* >(msg)->GetYPos() );
			}

			pPE->SetName( dynamic_cast< CCreateEmitterMessage* >(msg)->GetMapName() );
			pPE->SetUseCamera(dynamic_cast< CCreateEmitterMessage* >(msg)->GetUseCamera());
			pGPS->GetParticleManager()->AddEmitter( pPE );

			if( dynamic_cast< CCreateEmitterMessage* >(msg)->GetEmitterName() == "Firework Multi 3" ||
				dynamic_cast< CCreateEmitterMessage* >(msg)->GetEmitterName() == "Firework Multi 5")
			{
				CSGD_XAudio2::GetInstance()->SFXPlaySound( CFileManager::GetInstance()->GetSoundID( "MASQ_Fireworks") );
			}
		}
		break;
	case MSG_DESTROY_PEMITTER:
		{
			std::string temp = dynamic_cast< CDestroyEmitterMessage* >(msg)->GetMapName();
			pGPS->GetParticleManager()->RemoveEmitter( temp );
		}
		break;
	case MSG_CREATE_BULLET:
		{
			CBullet* b = new CBullet();
			b->AddRef();
			b->setBulletType(dynamic_cast< CCreateBulletMessage* >(msg)->GetBulletType());
			CBaseObject* pFiringEntity = dynamic_cast< CCreateBulletMessage* >(msg)->getFiringEntity();

			b->SetOwner(pFiringEntity);

			if(pFiringEntity != nullptr)
			{
				b->SetX( pFiringEntity->GetX() );
				float height = (float)(pFiringEntity->GetRect().bottom - pFiringEntity->GetRect().top);
				b->SetY( pFiringEntity->GetY() - height/2.0f );


				//CBullet* b = dynamic_cast< CBullet* >(pBullet);
				CFox* t =  dynamic_cast< CFox* >(pFiringEntity);
				int bType = b->getBulletType();
				//printf("Bullet Type %i", bType); 
				switch (bType)
				{
				case SQUIRT_BULLET:
					b->SetImageID(pGPS->m_pFM->GetImageID("Squirt_Gun_Bullet.png"));
					//pBullet->SetSourceRect( where is it in the image file );

					b->SetHeight(pGPS->m_pTM->GetTextureHeight(b->GetImageID()));
					b->SetWidth(pGPS->m_pTM->GetTextureWidth(b->GetImageID()));


					if(b != nullptr)
					{
						b->SetOwner( t );
						tVector2D newVelocity = { 0, 0 };
						newVelocity.fX = (t->GetFrame().isFlipped()) ? -1.0f : 1.0f;
						newVelocity = newVelocity * (500);

						b->setIsFlipped( t->GetFrame().isFlipped() ? true : false );
						b->SetX( t->GetFrame().isFlipped() ? b->GetX()+b->GetWidth() : b->GetX() );
						b->SetVelX( newVelocity.fX );
						b->SetVelY( newVelocity.fY );
						b->SetGravityBool(true);
						b->SetGravityFloat(50.0f);
						//b->SetSoundHit( self->getExplosionFX() );
					}
					break;
				case BALLOON_BULLET:

					break;
				case COTTON_BULLET:
					b->SetImageID(pGPS->m_pFM->GetImageID("CottonCandy_Gun_Bullet.png"));
					//pBullet->SetSourceRect( where is it in the image file );

					b->SetHeight(pGPS->m_pTM->GetTextureHeight(b->GetImageID()));
					b->SetWidth(pGPS->m_pTM->GetTextureWidth(b->GetImageID()));


					if(b != nullptr)
					{
						b->SetOwner( t );
						tVector2D newVelocity = { 0, 0 };
						newVelocity.fX = (t->GetFrame().isFlipped()) ? -1.0f : 1.0f;
						//newVelocity.fY = -2.0f;
						newVelocity.fY = dynamic_cast< CCreateBulletMessage* >(msg)->GetVelY();
						newVelocity = newVelocity * (200);

						b->setIsFlipped( t->GetFrame().isFlipped() ? true : false );
						b->SetX( t->GetFrame().isFlipped() ? b->GetX() : b->GetX()+b->GetWidth() );
						b->SetVelX( newVelocity.fX );
						b->SetVelY( newVelocity.fY );
						b->SetGravityBool(true);
						b->SetGravityFloat( 200.0f );

					
						CSGD_XAudio2::GetInstance()->SFXPlaySound( CFileManager::GetInstance()->GetSoundID( "MASQ_CCL_Shoot" ) );
						//b->SetSoundHit( self->getExplosionFX() );
					}
					break;
				case PIE_BULLET:
					{
						CClown* c =  dynamic_cast< CClown* >(pFiringEntity);

						b->SetImageID(pGPS->m_pFM->GetImageID("MASQ_Pie_Bullet.png"));

						b->SetHeight(pGPS->m_pTM->GetTextureHeight(b->GetImageID()));
						b->SetWidth(pGPS->m_pTM->GetTextureWidth(b->GetImageID()));


						if(b != nullptr)
						{
							b->SetOwner( c );
							tVector2D newVelocity = { 0, -1 };
							newVelocity.fX = (c->GetFrame().isFlipped()) ? 1.0f : -1.0f;
							newVelocity = newVelocity * (150);

							b->setIsFlipped( c->GetFrame().isFlipped() ? false : true );

							b->SetVelX( newVelocity.fX + c->GetVelX() );
							b->SetVelY( newVelocity.fY );
							b->SetGravityBool(true);
							b->SetGravityFloat( 200.0f );
						}
					}
					break;
				default:

					break;
				}
				pGPS->m_pOM->AddObject( b );
				b->Release();
			}
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CCreateEnemyMessage* pMsg = dynamic_cast< CCreateEnemyMessage* >( msg );
			CBaseObject* pEnemy;
			switch( pMsg->GetEnemyType() )
			{
			case NME_CLOWN:
				pEnemy = CObjectFactory::GetInstance()->CreateObject( CLOWN );
				break;
			case NME_RAT:
				{
					pEnemy = CObjectFactory::GetInstance()->CreateObject( RAT );
					CRat* pRat = dynamic_cast< CRat* >( pEnemy );
					pRat->Init( pMsg->GetSpawnPoint()->GetX(), pMsg->GetSpawnPoint()->GetY() );
				}
				break;
			case NME_HOODLUM:
				pEnemy = CObjectFactory::GetInstance()->CreateObject( HOODLUM );
				break;
			}

			pEnemy->SetX( pMsg->GetSpawnPoint()->GetX() );
			pEnemy->SetY( pMsg->GetSpawnPoint()->GetY() );
			CObjectManager::GetInstance()->AddObject( pEnemy );
		}
		break;
	case MSG_DESTROY_BULLET:
		{
			// Get the attached bullet
			CBullet* pBullet = dynamic_cast< CDestroyBulletMessage* >( msg )->GetBullet();

			// Remove the object from the Object Manager
			pGPS->m_pOM->RemoveObject(pBullet);
		}
		break;
	case MSG_DESTROY_ENEMY:
		{
			CEnemy* pEnemy = dynamic_cast< CDestroyEnemyMessage* >( msg )->GetEnemy();

			pGPS->m_pOM->RemoveObject(pEnemy);
		}
		break;
	case MSG_RESET:
		{
			CFileManager::playerProgress p;
			if( CFileManager::GetInstance()->LoadGame( "Resources/Scripts/JUNK.xml", p ) == false)
			{
				
				//switch(pGPS->m_nCurStage)
				//{
				//case 0:
				//	CTileManager::GetInstance()->LoadTileWorld( "Resources/Scripts/MASQ_Tutorial_World.xml", CGamePlayState::GetInstance()->GetCurStage() );
				//	CObjectManager::GetInstance()->RemoveAllObjects();
				//	CTileManager::GetInstance()->LoadObjects( "Resources/Scripts/MASQ_Tutorial_World.xml", CGamePlayState::GetInstance()->GetCurStage() );
				//break;
				//case 1:
				//	/*CTileManager::GetInstance()->LoadTileWorld( "Resources/Scripts/testworld.xml", CGamePlayState::GetInstance()->GetCurStage() );
				//	CObjectManager::GetInstance()->RemoveAllObjects();
				//	CTileManager::GetInstance()->LoadObjects( "Resources/Scripts/testworld.xml", CGamePlayState::GetInstance()->GetCurStage() );*/
				//break;
				//}
				/*pGPS->GetFox()->SetX(pGPS->GetFox()->GetStartingXPos());
				pGPS->GetFox()->SetY(pGPS->GetFox()->GetStartingYPos());*/
				pGPS->RespawnFox();
				/*std::ostringstream level;
				level << "Resources/Scripts/World_" << pGPS->m_nCurStage;

				level << ".xml";
				std::string test = level.str().c_str();
				printf( test.c_str() );

				pGPS->SetLevelFile( level.str().c_str() );
				pGPS->Save( );
				CGame::GetInstance()->PopState();
				CGame::GetInstance()->PushState(CGamePlayState::GetInstance());*/
			}
		}
	default:
		break;
	};
}

unsigned int WINAPI Loading(void* lpvArgList)
{
	// The Direct3DX Sprite Interface.
	LPD3DXSPRITE	lpSprite;
	CSGD_Direct3D* D3D = CSGD_Direct3D::GetInstance();
	CCamera* cam = CCamera::GetInstance();

	//int id = TM->LoadTexture(_T("Resources/Images/MenuImages/MASQ_Maska_02.jpg"));

	
	ThreadData* TD = (ThreadData*)lpvArgList;
	CRITICAL_SECTION* pCSEC = TD->pCSEC;
	float* pPercent = TD->pPercent;
	
	bool run = true;
	float start = cam->GetScreenSpaceX() / 10;
	float stop = cam->GetScreenSpaceX() - (cam->GetScreenSpaceX() / 10);
	float posX = start;
	float posY = cam->GetScreenSpaceY() - 30;
	unsigned int size = 1;
	
	// Create Sprite Object.
	HRESULT hr = D3DXCreateSprite(D3D->GetDirect3DDevice(), &lpSprite);

	// A temp texture object.
	LPDIRECT3DTEXTURE9 background;
	LPDIRECT3DTEXTURE9 spritesheet;

	hr = D3DXCreateTextureFromFileEx(
			D3D->GetDirect3DDevice(),
			_T("Resources/Images/MenuImages/MASQ_Maska_02.jpg"),
			0, 0, D3DX_DEFAULT, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			D3DX_DEFAULT, 0, 0, 0, &background);

	hr = D3DXCreateTextureFromFileEx(
			D3D->GetDirect3DDevice(),
			_T("Resources/Images/Graphics/MASQ_Loading.png"),
			0, 0, D3DX_DEFAULT, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			D3DX_DEFAULT, 0, 0, 0, &spritesheet);

	CFrame	frame;
	frame.Init( &posX, &posY, &size, &size );
	frame.SetAnimationID("Loading Sparkle");
	frame.SetXScale( 0.25f );
	frame.SetYScale( 0.25f );
	frame.Play(true);

	CAnchorAnimation anime;
	anime.CreateFromFile("Resources/Scripts/loading_animation.xml");

	D3DXMATRIX combined;
	D3DXMATRIX backscale;
	D3DXMATRIX spritescale;
	D3DXMATRIX translate;
	RECT source;
	
	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&combined);

	// Scale the sprite.
	D3DXMatrixScaling(&backscale, 0.39f, 0.35f, 1.0f);
	D3DXMatrixScaling(&spritescale, 0.5f, 0.5f, 1.0f);

	DWORD Time = 0;

	while( run )
	{
		EnterCriticalSection( pCSEC );
		run = (*pPercent <= 1.0f);
		LeaveCriticalSection( pCSEC );

		
		//DWORD now = GetTickCount();
		//float fElapsed = (now - Time) / 1000.0f;
		//Time = now;
		float fElapsed = 0.008f;

		anime.Update( fElapsed, frame );

		D3D->Clear( 0,0,0 );
		D3D->DeviceBegin();
		lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
		{

			EnterCriticalSection( pCSEC );
			int alpha = (int)Lerp( 0, 200, *pPercent);
			posX = Lerp( start, stop, *pPercent );
			LeaveCriticalSection( pCSEC );

			// Scale the sprite.
			combined *= backscale;
			
			// Apply the transform.
			lpSprite->SetTransform(&combined);

			hr = lpSprite->Draw(background, NULL, NULL, NULL, D3DCOLOR_ARGB( alpha, 255, 255, 255 ));

			D3D->DrawLine( (int)start, (int)posY, (int)stop, (int)posY, 255, 255, 255 );

			// Move the world back to identity.
			D3DXMatrixIdentity(&combined);

			// Scale the sprite.
			combined *= spritescale;

			// Translate the sprite
			D3DXMatrixTranslation(&translate, posX, posY-32, 0.0f);
			combined *= translate;

			// Apply the transform.
			lpSprite->SetTransform(&combined);

			source = anime.GetRect( frame.GetCurFrame() );
			hr = lpSprite->Draw(spritesheet, &source, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ));

			// Move the world back to identity.
			D3DXMatrixIdentity(&combined);
			lpSprite->SetTransform(&combined);

		}
		lpSprite->End();
		D3D->DeviceEnd();
		D3D->Present();
	}

	return 0;
}

void CGamePlayState::StartLoadingScreen()
{
	m_fLoadingPercent = 0.0f;

	CoInitializeEx( NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE );
	InitializeCriticalSectionAndSpinCount( &CSEC, 0x00000400 );

	 
	 TD.pCSEC = &CSEC;
	 TD.pPercent = &m_fLoadingPercent;

	 L = (HANDLE)_beginthreadex(NULL, 0,
		Loading, &TD, 0, NULL);

#ifdef _DEBUG
	Sleep(1);  // Sleep after thread creation for debugging to work properly.
#endif

	//Multi Threaded Loading Screen is Running
}

void CGamePlayState::StopLoadingScreen()
{
	EnterCriticalSection( &CSEC );
	m_fLoadingPercent = 2.0f;
	LeaveCriticalSection( &CSEC );

	CCamera::GetInstance()->Reset();

	/*DeleteCriticalSection( &CSEC );
	TD.pCSEC = nullptr;
	TD.pPercent = nullptr;
	CloseHandle(L);
	CoUninitialize();*/
}

void CGamePlayState::UpdateLoad( float& percent, CRITICAL_SECTION& csec )
{
	EnterCriticalSection( &csec );
	percent += 1.0f /  NUM_FUNCTIONS;
	LeaveCriticalSection( &csec );
};

void CGamePlayState::RespawnFox(void)
{
	bool bHasFoundCheck = false;
	std::vector<IBaseObject*> checks;
	checks = m_pOM->FindObjectsWithType( OBJ_CHECKPOINT );
	for(size_t i = 0; i < checks.size(); ++i)
	{
		CCheckpoint* c = dynamic_cast<CCheckpoint*>(checks[i]);
		if( c->GetCheckpointBool() )
		{
			m_Fox->SetX( c->GetX() );
			m_Fox->SetY( c->GetY() - 128.0f );
			m_cCorinne->SetX( c->GetX() );
			m_cCorinne->SetY( c->GetY() - 128 );
			bHasFoundCheck = true;
			break;
		}
				
	}
	if( ! bHasFoundCheck )
	{
		m_Fox->SetX(m_Fox->GetStartingXPos());
		m_Fox->SetY(m_Fox->GetStartingYPos());
		m_cCorinne->SetX( m_Fox->GetStartingXPos() + 64 );
		m_cCorinne->SetY( m_Fox->GetStartingYPos() );
	}
	m_Fox->SetHearts( 5 );
	m_Fox->SetGravityBool(true);
	//m_Fox->SetVelY( -300.0f );
	m_Fox->SetVelY( 0.0f );
	m_Fox->SetVelX( 0.0f );
	m_Fox->setInputEnabled(true);


	m_cCorinne->SetVelX(0.0f);
	m_cCorinne->SetVelY(0.0f);

	if( m_pES->HasEventTriggered( "idle" ) == false )
		m_pES->SendUniqueEvent( "idle", &m_cCorinne->GetCurrentState()->GetObjectsInvolved() );
	
	if( m_pES->HasEventTriggered( "putdown" ) == false && m_cCorinne)
		m_pES->SendUniqueEvent( "putdown", &m_cCorinne->GetCurrentState()->GetObjectsInvolved() );
	
	m_cCorinne->ResetHurtTimer();
	
}

void CGamePlayState::RespawnCorinne(void)
{
	bool bHasFoundCheck = false;
	std::vector<IBaseObject*> checks;
	checks = m_pOM->FindObjectsWithType( OBJ_CHECKPOINT );
	for(size_t i = 0; i < checks.size(); ++i)
	{
		CCheckpoint* c = dynamic_cast<CCheckpoint*>(checks[i]);
		if( c->GetCheckpointBool() )
		{
			m_cCorinne->SetX( c->GetX() );
			m_cCorinne->SetY( c->GetY() - 128 );
			bHasFoundCheck = true;
			break;
		}
				
	}
	if( ! bHasFoundCheck )
	{
		m_cCorinne->SetX( m_Fox->GetStartingXPos() + 64 );
		m_cCorinne->SetY( m_Fox->GetStartingYPos() );
	}

	m_cCorinne->SetVelX(0.0f);
	m_cCorinne->SetVelY(0.0f);

	if( m_pES->HasEventTriggered( "idle" ) == false )
		m_pES->SendUniqueEvent( "idle", &m_cCorinne->GetCurrentState()->GetObjectsInvolved() );

	m_cCorinne->ResetHurtTimer();
}


void CGamePlayState::GoToNextStage(int stage)
{
	SetMusicSwitchBool( true );

	//m_pTileManager->RemoveAllTiles();
	//m_pOM->RemoveAllObjects();
	//CAnimationManager::GetInstance()->Clear();
		// =======================
		//	Win Loose Conditions
		// =======================	
		m_bGameOver = false;
		m_bStageComplete = false;
		m_bWin = false;
		m_fFireWorkTimer = 0.0f;
		m_fFireWorkDuration = 10.0f;
		m_bNewFirework = true;
		m_fFireWorkInterval = 0.5f;
		m_fFireWorkNewTimer = 0.0f;

		m_fGameOverTimer = 0.0f;
		m_fWinTimer = 0.0f;
		m_bCameraMode = false;

		m_bSplash = false;
		m_bSplashOnce = false;
		ITool* t = new CSquirtGun();
		ITool* t2 = new CCottonCandyLauncher();
		ITool* t3 = new CBalloonGun();

		/*m_Fox = new CFox();
		m_cCorinne = new CCorinne();*/

	switch( m_nCurStage )
	{
	case 0:
		m_pTileManager->LoadTileWorld( "Resources/Scripts/Tutorial.xml", 1 );
		m_pTileManager->LoadObjects( "Resources/Scripts/Tutorial.xml", 1 );	
	case 1:
		//SetLevelFile( "Resources/Scripts/World_1.xml" );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/World_1.xml", 1 );	
		m_pTileManager->LoadObjects( "Resources/Scripts/World_1.xml", 1 );	
		m_cCorinne->setPowerup(CPowerup::NORMAL);
		m_Fox->clearPowerups();
		m_Fox->clearTools();
		m_Fox->SetCurrentTool(-1);
		//Save( );
		break;
	case 2:
		//SetLevelFile( "Resources/Scripts/World_2.xml" );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/World_2.xml", 1 );	
		m_pTileManager->LoadObjects( "Resources/Scripts/World_2.xml", 1 );
		m_cCorinne->setPowerup(CPowerup::NORMAL);
		m_Fox->clearTools();
		m_pOM->AddObject(t);
		m_Fox->AddATool(t);
		m_Fox->SetCurrentTool(0);
		//Save( );
		break;
	case 3:
		//SetLevelFile( "Resources/Scripts/World_3.xml" );
		m_pTileManager->LoadTileWorld( "Resources/Scripts/World_3.xml", 1 );	
		m_pTileManager->LoadObjects( "Resources/Scripts/World_3.xml", 1 );	
		m_cCorinne->setPowerup(CPowerup::NORMAL);
		/*m_Fox->SetX(200);
		m_Fox->SetX(300);*/
		m_Fox->clearTools();
		m_pOM->AddObject(t);
		m_Fox->AddATool(t);
		m_pOM->AddObject(t2);
		m_Fox->AddATool(t2);
		/*m_pOM->AddObject(t3);
		m_Fox->AddATool(t3);*/
		m_Fox->SetCurrentTool(0);
		//Save( );
		break;
	}

	// =============================================
	// BUG FIX 16
	// =============================================
	// Fox needs to have his internals reset to avoid
	// him killing himself at the load of the next stage
	// should he have fallen into the water during fireworks
	m_Fox->ResetWaterDeath();
	++m_nCurStage;
}

void CGamePlayState::Save( void )
{
	CFileManager::playerProgress saveGame;
	std::ostringstream tempss;
	int i = CFileManager::GetInstance()->GetSaveToLoad();
	if( CFileManager::GetInstance()->GetSaveToLoad() < 0 ||
		CFileManager::GetInstance()->GetSaveToLoad() > 3 )
	{
		CFileManager::GetInstance()->SetCurrentSaveToLoad( 1 );
		tempss << CFileManager::GetInstance()->GetCurrentSaveToLoad();
	}
	else
		tempss << CFileManager::GetInstance()->GetCurrentSaveToLoad();

	saveGame.nLevelNum = CGamePlayState::GetInstance()->GetCurStage();
	saveGame.szLevelPath = CGamePlayState::GetInstance()->GetLevelFile();
	saveGame.pFox = m_Fox;
	saveGame.szCheckPoint = "";
	saveGame.szFileName = "Resources/Scripts/Save_";
	saveGame.szFileName += tempss.str();
	saveGame.szFileName += ".xml";
	CFileManager::GetInstance()->SaveGame( saveGame.szFileName.c_str(), saveGame );

	bool bCanSave = true;
	for( unsigned j = 0; j < m_pFM->GetSaves().size(); ++j )
	{
		if( saveGame.szFileName ==  m_pFM->GetSaves()[j].szFileName)
		{
			bCanSave = false;
			break;
		}
	}
	if( bCanSave == true )
		CFileManager::GetInstance()->GetSaves().push_back( saveGame );

}