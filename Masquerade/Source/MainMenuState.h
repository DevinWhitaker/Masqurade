/*
	File:		MainMenuState.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Handles the game's main menu and the states
				it can enter.
*/
#include "IGameState.h"
#include "Game.h"

#ifndef	MAINMENUSTATE_H
#define	MAINMENUSTATE_H

#include "Frame.h"

class	CFileManager;
class	CSGD_Direct3D;
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;
class	CBitmapFont;
class	CCursor;
class	CAnimationManager;

class CMainMenuState : public IGameState
{
public:
	enum STATES { GAMEPLAY, LOADGAME, OPTIONS, HOWTOPLAY, CREDITS, EXIT, MAX_STATES };

	static CMainMenuState* GetInstance(void);

	virtual void Enter(void);						//	Load Resources
	virtual void Exit(void);						//	Unload Resources

	virtual bool Input(void);						//	Handle Input
	virtual void Update(float fElapsedTime);		//	Update game entities
	virtual void Render(void);						//	Render game entities

	bool	GetMusicPlayingBool( void )				{ return m_bIsMusicPlayingAlready;		}
	void	SetMusicPlayingBool( bool musicbool )	{ m_bIsMusicPlayingAlready = musicbool;	}

	float	GetIntroTimer( void )					{ return m_fIntroTimer; }
	void	SetIntroTimer( float t )				{ m_fIntroTimer = t; }

	bool	GetToggleBool( void )					{ return m_bToggleIntro; }
	void	SetToggleBool( bool b )					{ m_bToggleIntro = b; }

	bool GetTutorialBool( void ) const				{ return m_bTutorial; }
	void SetTutorialBool( bool bTutorialBool ) { m_bTutorial = bTutorialBool; }

	bool GetNewGameBool( void ) const				{ return m_bNewProfile; }
	void SetNewGameBool( bool bProfile )			
	{ m_bNewProfile = bProfile; }

private:

	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator=(const CMainMenuState&);
	CMainMenuState(void);
	virtual ~CMainMenuState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBMP;
	CFileManager*			m_pFM;
	CCursor*				m_pC;
	CFrame					m_frMainButton;
	CFrame					m_frTitle;
	CAnimationManager*		m_pAM;

	struct sFrame
	{
		float fx;
		float fy;
		unsigned int unX;
		unsigned int unY;
	};

	sFrame m_ButtonPosition;
	sFrame m_TitlePosition;

	std::vector< std::string > m_vButtonIDs;
	std::vector< std::string > m_vOptions;
	float	m_fIntroTimer;
	bool	m_bToggleIntro;
	int		m_nPointer;
	bool	m_bIsMusicPlayingAlready;
	DWORD	m_dwMenuColors[2];
	int		m_nColorSwap;
	bool	m_bTutorial;
	bool	m_bNewProfile;

};

#endif