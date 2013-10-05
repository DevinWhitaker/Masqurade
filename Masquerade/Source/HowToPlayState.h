/*
	File:		HowToPlayState.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Shows the user how to play the game
*/

#pragma once
#include "IGameState.h"
#include "Game.h"
class	CBitmapFont;
class	CSGD_Direct3D;
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;
class	CCursor;

class CHowToPlayState : public IGameState
{
	CHowToPlayState(const CHowToPlayState&);
	CHowToPlayState& operator=(const CHowToPlayState&);
	CHowToPlayState(void);
	virtual ~CHowToPlayState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBMF;
	CCursor*				m_pC;

public:
	static CHowToPlayState* GetInstance(void);

	virtual void Enter(void);		//	Load Resources
	virtual void Exit(void);		//	Unload Resources

	virtual bool Input(void);		// Handle Input
	virtual void Update(float fElapsedTime);		//Update game entities
	virtual void Render(void);		//Render game entities
};

