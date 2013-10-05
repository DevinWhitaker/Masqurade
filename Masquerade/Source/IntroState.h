//	File:		IntroState.h
//	Course:		SGP1
//	Author:		Jam'D Team
//	Purpose:	Shows a cutscene at the beginning of a new game

#pragma once
#include "IGameState.h"
#include "Game.h"

class	CBitmapFont;
class	CSGD_Direct3D;
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;
class	CFileManager;


class CIntroState :	public IGameState
{
public:
	static CIntroState* GetInstance(void);

	virtual void Enter(void);		//	Load Resources
	virtual void Exit(void);		//	Unload Resources

	virtual bool Input(void);		// Handle Input
	virtual void Update(float fElapsedTime);		//Update game entities
	virtual void Render(void);		//Render game entities

private:

	CIntroState(const CIntroState&);
	CIntroState& operator=(const CIntroState&);
	CIntroState(void);
	virtual ~CIntroState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBM;
	CGame*					m_pGame;
	POINT					m_MousePos;
	CFileManager*			m_pFM;

	// Timer
	float fTime;
	float fShrinkX, fShrinkY, fMoveX, fMoveY;
	int nFrame;
	bool isLogoCornered;
	bool isOutro;

	float fMoveX1, fMoveX2;
	int nLogoImageID;
	int nCutSceneID[ 10 ];
};

