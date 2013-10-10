#include "Game.h"
#include "MainMenuState.h"
#include "IntroState.h"
#include "FileManager.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"

// Singleton
CGame* CGame::GetInstance(void)
{
	// Lazy Instantiation
	static CGame s_Instance;

	return &s_Instance;
}

CGame::CGame(void)
{
	// set pointer to 'safe' values
	m_pD3D = nullptr;
	m_pDI = nullptr;
	m_pTM = nullptr;
	m_pXA = nullptr;

	m_nScreenWidth	= -1;
	m_nScreenHeight = -1;
	m_bWindowed		= true;

	m_dwGameTime = 0;

}


CGame::~CGame(void)
{
}

// Setup
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bWindowed )
{
	SetWindowHandle( hWnd );
	HRGN hrgn = CreateRectRgn( 10, 30, 800, 630 );
	SetWindowRgn( hWnd, hrgn, TRUE );
	// Access all the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();

	// Initialize wrappers
	m_pD3D->Initialize( hWnd, nScreenWidth, nScreenHeight, bWindowed, false );
#if _DEBUG

	m_pDI->Initialize( hWnd, hInstance, DI_KEYBOARD | DI_MOUSE/*, DI_KEYBOARD | DI_MOUSE*/ );
#else
	m_pDI->InitDirectInput( hWnd, hInstance, DI_KEYBOARD | DI_MOUSE | DI_JOYSTICKS, DI_MOUSE | DI_JOYSTICKS );
#endif

	m_pTM->Initialize( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pXA->Initialize();

	// Store the screen info
	m_nScreenWidth	= nScreenWidth;
	m_nScreenHeight = nScreenHeight;
	m_bWindowed		= bWindowed;

	// Start the game!
	PushState( CIntroState::GetInstance() );

	// Store the current time
	m_dwGameTime = GetTickCount();
}

// Play
bool CGame::Main(void)
{
	// Sanity-check
	//if( m_pCurState == nullptr ) 
	if( m_vpStates.size() == 0 )
		return false;

	if(Input() == false)
		return false;

	Update();
	Render();

	return true;
}

bool CGame::Input(void)
{
	// Refresh the Direct Input Buffers
	m_pDI->ReadDevices();

	// Exit the Game when Esc is pressed
	/*if(m_pDI->KeyPressed(DIK_ESCAPE))
		return false;*/

	// Let the Current state handle input
	//return m_pCurState->Input();
	return m_vpStates.back()->Input();
}
void CGame::Update()
{
	// Refresh the XAudio2
	m_pXA->Update();

	DWORD now = GetTickCount();
	float fElapsed = (now - m_dwGameTime) / 1000.0f;
	m_dwGameTime = now;

	if( fElapsed > 0.05f)//0.125f)
		fElapsed = 0.05f;

	// Let the current state update
	//m_pCurState->Update( fElapsed );
	m_vpStates.back()->Update( fElapsed );
}
void CGame::Render(void)
{
	// Clear the background
	m_pD3D->Clear(0x000000);

	// Start D3D rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	//m_pCurState->Render();
	m_vpStates.back()->Render();

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	m_pD3D->Present();

}

// Cleanup
void CGame::Shutdown(void)
{
	// Exit the Current State
	//ChangeState( nullptr );
	m_vpStates.clear();

	CFileManager::GetInstance()->CleanUpIDs();

	// Shutdown the SGD Wrappers in the Opposite order of Init (due to dependancies)
	if( m_pXA != nullptr )
	{
		m_pXA->Terminate();
		m_pXA = nullptr;
	}

	if( m_pTM != nullptr )
	{
		m_pTM->Terminate();
		m_pTM = nullptr;
	}

	if( m_pDI != nullptr )
	{
		m_pDI->Terminate();
		m_pDI = nullptr;
	}

	if( m_pD3D != nullptr )
	{
		m_pD3D->Terminate();
		m_pD3D = nullptr;
	}

	CObjectFactory::GetInstance()->ShutdownObjectFactory();


	CObjectManager::GetInstance()->RemoveAllObjects();
	CObjectManager::GetInstance()->DeleteInstance();

	// Reset the members
	m_nScreenWidth	= -1;
	m_nScreenHeight = -1;
	m_bWindowed		= true;
}


void CGame::PopState( bool bEnterState  )
{
	if( m_vpStates.size() > 0 )
	{
		m_vpStates.back()->Exit();
		m_vpStates.pop_back();
	}

	if( m_vpStates.back() > 0 )
	{
		if( bEnterState == true )
			m_vpStates.back()->Enter();
		else {}
	}
}

void CGame::PushState( IGameState* pNewState, bool bExitState)
{
	if( m_vpStates.size() > 0 )
		if( bExitState == true )
		m_vpStates.back()->Exit();
		else {}

	m_vpStates.push_back( pNewState );

	if( m_vpStates.size() > 0 )
		m_vpStates.back()->Enter();
}

void CGame::FullScreenMode( void )
{
	if( this->IsWindowed() == true )
	{
		//m_pD3D->ChangeDisplayParam( this->GetWidth(), this->GetHeight(), false );ToFix
		this->m_bWindowed = false;
	}
	else if( this->IsWindowed() == false )
	{
		//m_pD3D->ChangeDisplayParam( this->GetWidth(), this->GetHeight(), true );ToFix
		this->m_bWindowed = true;
	}
}

void CGame::FullScreenMode( bool setToFullScreen )
{
	if( this->IsWindowed() == true && setToFullScreen == true )
	{
		//m_pD3D->ChangeDisplayParam( this->GetWidth(), this->GetHeight(), false );ToFix
		this->m_bWindowed = false;
	}
	else if( this->IsWindowed() == false && setToFullScreen == false )
	{
		//m_pD3D->ChangeDisplayParam( this->GetWidth(), this->GetHeight(), true );ToFix
		this->m_bWindowed = true;
	}
}