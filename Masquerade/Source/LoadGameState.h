/*
	File:		LoadGameeSate.h
	Course:		SGP1
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Allows the player load up their saved progress
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
class	CFileManager;

class CLoadGameState :
	public IGameState
{
public:
	static CLoadGameState* GetInstance(void);

	virtual void Enter(void);						//	Load Resources
	virtual void Exit(void);						//	Unload Resources
		
	virtual bool Input(void);						//	Handle Input
	virtual void Update(float fElapsedTime);		//	Update game entities
	virtual void Render(void);						//	Render game entities

	void	SetRender( int nRenderText ) { m_nRenderData = nRenderText; }
	int		GetRender( void ) const { return m_nRenderData; }

	void	SetDeleteMode( bool bDel ) { m_bDelete = bDel; }
	bool	GetDeleteBool( void ) const { return m_bDelete; }

private:

	CLoadGameState(const CLoadGameState&);
	CLoadGameState& operator=(const CLoadGameState&);
	CLoadGameState(void);
	virtual ~CLoadGameState(void);

	// SGD	Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pBMF;
	CCursor*				m_pC;
	CFileManager*			m_pFM;
	POINT					m_PosButtons[3];
	int						m_nRenderData;
	bool					m_bDelete;

};

