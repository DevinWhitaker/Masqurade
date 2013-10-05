/*
	File:		Checkpoint.h
	Course:		SGP1
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Allows auto-saving when Fox collides with it.
*/
#pragma once
#include "baseobject.h"
#include "Frame.h"

class	CFileManager;

class CCheckpoint :
	public CBaseObject
{
public:
	CCheckpoint(void);
	virtual ~CCheckpoint(void);
	virtual bool CheckCollision( IBaseObject * pBase );
	virtual void Update( float fElapsed );
	virtual void Render( void );

	bool	GetCheckpointBool( void )		const	{ return m_bCheckpointUsed; }
	void	SetCheckpointBool( bool bCheck )		{ m_bCheckpointUsed = bCheck; }


	CFrame		&GetFrame( void )					{ return m_pCheck;		}
	void AddAnimation( const char* szFileName, int nAnimationToUse = 0 );
	RECT GetRect();

private:
	CFrame			m_pCheck;
	bool			m_bCheckpointUsed;
	bool			m_bSaveGame;
};

