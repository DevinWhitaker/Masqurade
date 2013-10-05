/*
	File:		GamePlayState.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Handle gameplay components.
*/

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "ParticleManager.h"
#include "ItemMenu.h"
#include <process.h>
#include "IdleState.h"

class	CFox;
class	CBitmapFont;
class	CFileManager;
class	CAnimationManager;
class	CCorinne;
class	CSGD_Direct3D;
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;
class	CSGD_MessageSystem;
class	CCamera;
class	CObjectManager;
class	CObjectFactory;
class	CTileManager;
class	CHUD;


struct ThreadData
{
	CRITICAL_SECTION* pCSEC;
	float*	pPercent;
};

class CGamePlayState : public IGameState
{
public:
	static CGamePlayState* GetInstance(void);

	virtual void Enter(void);		//	Load Resources
	virtual void Exit(void);		//	Unload Resources

	virtual bool Input(void);		// Handle Input
	virtual void Update(float fElapsedTime);		//Update game entities
	virtual void Render(void);		//Render game entities
	static void MessageProc(CMessage* msg);

	void SetPauseBool( bool bPauseGame ) { m_bPause = bPauseGame;	}
	bool GetPauseBool( void )			 { return m_bPause;			}

	void SetDebugBool( bool bDebug )	 { m_bShowDebugInfo = bDebug;	}
	bool GetDebugBool( void )			 { return m_bShowDebugInfo;			}

	int		GetCursorValue( void )				{ return m_nPointer;		}
	void	SetCursorValue( int cursor )		{ m_nPointer = cursor;		}

	void SetMusicSwitchBool( bool bMusic )	 { m_bSwitchToNewMusic = bMusic;	}
	bool GetMusicBool( void )			 { return m_bSwitchToNewMusic;	}

	void	SetLevelFile( std::string szPath )
	{ m_szLevelFilePath = szPath; }

	std::string GetLevelFile( void ) const
	{ return m_szLevelFilePath; }
	CParticleManager*	&GetParticleManager( void ) { return m_pPM; }

	CFox*		GetFox(void)		{ return m_Fox; }
	CCorinne*	GetCorrine(void)	{ return m_cCorinne; }

	int		&GetCurStage(void)		{ return m_nCurStage; }
	void		SetCurStage(int stage)	{ m_nCurStage = stage; }

	int		&GetNumberOfStages(void)		{ return m_nNumOfStages; }
	void	SetNumberOfStages(int stages)	{ m_nNumOfStages = stages; }

	float	&GetStageEndMarker(void)		{ return m_fStageEndMarker; }
	void	SetStageEndMarker(float Xposition) { m_fStageEndMarker = Xposition; }

	void RespawnFox(void);
	void RespawnCorinne(void);
	void GoToNextStage(int stage);

	void Save( void );

private:
	enum CURSORPOS { RESUME, OPTIONS, LAST_CHECKPOINT, EXIT, SKIP };

	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator=(const CGamePlayState&);
	CGamePlayState(void);
	virtual ~CGamePlayState(void);
	void PauseGameMenu( void );
	void StartLoadingScreen();
	void StopLoadingScreen();
	HANDLE L;
	CRITICAL_SECTION CSEC;
	ThreadData TD;
	void UpdateLoad( float& percent, CRITICAL_SECTION& csec );
	friend unsigned int WINAPI Loading(void* lpvArgList);
	/**********************************/
	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CSGD_MessageSystem*		m_pMS;
	CSGD_EventSystem*		m_pES;
	CObjectFactory*			m_pOF;
	/**********************************/
	CObjectManager*			m_pOM;
	CParticleManager*		m_pPM;
	CTileManager*			m_pTileManager;
	CCamera*				m_pCamera;
	CBitmapFont*			m_pBMF;
	CFileManager*			m_pFM;
	CAnimationManager*		m_pAM;
	/**********************************/
	CCorinne*				m_cCorinne;
	CFox*					m_Fox;
	/**********************************/
	CHUD*					m_pHUD;
	CItemMenu				m_ItemMenu;

	CIdleState* pIdle;

	int		m_nBulletImageTemp;
	bool	m_bMessageTest;
	int		m_nFrames;
	int		m_nFPS;
	float	m_fFrameTimer;
	float	m_fLoadingPercent;

	// Win Loose Conditions
	bool	m_bGameOver;
	bool	m_bStageComplete;
	bool	m_bWin;
	int		m_nNumOfStages;
	int		m_nCurStage;
	float	m_fStageEndMarker;
	float	m_fGameOverTimer;
	float	m_fWinTimer;
	bool	m_bSplash;
	bool	m_bSplashOnce;
	float	m_fFireWorkTimer;
	float	m_fFireWorkDuration;
	float	m_fFireWorkInterval;
	float	m_fFireWorkNewTimer;
	bool	m_bNewFirework;
	float	m_fDeadTime;
	float	m_fPerodicFwTimer;
	float	m_fFwPeroid;

	bool	m_bSwitchToNewMusic;

public:
	// Pika pi counter
	int m_nPikachuKills;
private:

	//	Saving
	std::string m_szLevelFilePath;

	// Camera Mode
	bool	m_bCameraMode;

	//	Pause functionality
	bool	m_bPause;
	POINT	m_POCursorPos[5];
	int		m_nPointer;


	int nId;

	//	Toggle Debug Mode
	bool	m_bShowDebugInfo;
};

