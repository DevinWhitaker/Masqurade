/*
	File:		OptionState.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Changes the music and sound volume for the entire game.
*/
#pragma once
#include "IGameState.h"
#include "Game.h"
#include "FileManager.h"

class	CBitmapFont;
class	CSGD_Direct3D;	
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;		
class	CCursor;

class COptionState : public IGameState
{
public:
	static COptionState* GetInstance(void);

	virtual void Enter(void);		//	Load Resources
	virtual void Exit(void);		//	Unload Resources

	virtual bool Input(void);		// Handle Input
	virtual void Update(float fElapsedTime);		//Update game entities
	virtual void Render(void);		//Render game entities


	//void ShowVolumes( void );

	
private:

	enum CURSOR { MUSIC, SOUNDEFFECTS, FULLSCREEN, GOBACK };

	COptionState(const COptionState&);
	COptionState& operator=(const COptionState&);
	COptionState(void);
	virtual ~COptionState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBMP;
	CFileManager*			m_pFM;
	CCursor*				m_pC;


	//	Acessors
	short	GetCursorPosition( void )			const	{ return m_sCursorPos;			}

	//	Mutators
	
	void	SetCursorPos( short posnumber )			{ m_sCursorPos = posnumber;			}

	CFileManager::volumeStruct m_vVolumes;
	CFileManager::volumeStruct &GetVolumeStruct( void ) { return m_vVolumes; }

	void SetVolumeStruct( CFileManager::volumeStruct vol )	{ m_vVolumes = vol; }
	
	//	Booleans to toggle volume modification
	bool	m_bModifyMusicVolume;
	bool	m_bModifySFXVolume;

	//	Cursor Position
	//int		
	POINT	m_POcursorPos[3];
	short	m_sCursorPos;
};

