/*
	File:		CreditState.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Credits the people who have contributed
				to the making of the video game.
*/
#include "IGameState.h"
#include "Game.h"

#ifndef CREDITSTATE_H
#define CREDITSTATE_H

class	CBitmapFont;
class	CSGD_Direct3D;
class	CSGD_DirectInput;
class	CSGD_TextureManager;
class	CSGD_XAudio2;
class	CCursor;
class	CFileManager;

class CCreditState :
	public IGameState
{
public:
	static CCreditState* GetInstance(void);

	virtual void Enter(void);		//	Load Resources
	virtual void Exit(void);		//	Unload Resources

	virtual bool Input(void);		// Handle Input
	virtual void Update(float fElapsedTime);		//Update game entities
	virtual void Render(void);		//Render game entities

private:

	CCreditState(const CCreditState&);
	CCreditState& operator=(const CCreditState&);
	CCreditState(void);
	virtual ~CCreditState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBMF;
	CCursor*				m_pC;
	CFileManager*			m_pFM;	
	float					m_fDuration;
};


#endif