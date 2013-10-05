#pragma once
#include <vector>
#include <Windows.h>

// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

#include "IGameState.h"

class CGame
{

public:

	static CGame* GetInstance(void);
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bWindowed );
	bool Main(void);
	void Shutdown(void);
	void PushState(IGameState* pNewState, bool bExitState = true );
	void PopState( bool bEnterState = true );

	int		GetWidth( void ) const	{ return m_nScreenWidth;	}
	int		GetHeight( void ) const	{ return m_nScreenHeight;	}
	bool	IsWindowed( void ) const{ return m_bWindowed;		}

	HWND	&GetWindowHandle( void )		{ return m_hWnd; }


	/*
	Return: void
	Parameters: 
	bool setToFullScreen - Allows toggling fullscreen.
	Description:
	Handles switching between fullscreen mode and windowed mode by the
	use of a boolean parameter.
	True	=	Full Screen.
	False	=	Windowed Mode.
	*/
	void FullScreenMode( bool setToFullScreen );

	/*
	Return: void
	Parameters: void
	Description:
	Handles switching between fullscreen mode and windowed mode 
	automatically.
	*/
	void FullScreenMode( void );

private:
	std::vector<IGameState*> m_vpStates;
	CGame(void);
	CGame( const CGame& );
	CGame& operator=(const CGame&);
	~CGame(void);
	
	bool Input(void);
	void Update();
	void Render(void);
	void	SetWindowHandle( HWND hwnd )	{ m_hWnd = hwnd; }

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	// Screen Info
	int		m_nScreenWidth;
	int		m_nScreenHeight;
	bool	m_bWindowed;
	HWND	m_hWnd;
	// Game Time:
	DWORD m_dwGameTime;
};

