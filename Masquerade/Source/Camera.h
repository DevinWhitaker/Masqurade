// Project:		Masquerade
// Filename:	Camera.cpp
// Author:		Jam'D Team
// Purpose:		Camera engine declaration.

#pragma once
#include <Windows.h>
#include "Game.h"

class CCamera
{
	friend class CFox;
public:
	static CCamera* GetInstance( );

	void Update( float fElapsed );
	void Input( );

	float OffsetX( ){ return m_fXOffset; }
	float OffsetY( ){ return m_fYOffset; }
	
	float GetScreenSpaceX();
	float GetScreenSpaceY();
	void  Reset( void );

private:
	CCamera( );
	CCamera( CCamera& );
	CCamera& operator=( CCamera& );
	~CCamera( );

	float m_fXOffset;
	float m_fYOffset;
	bool m_bPlayerControlled;

	CGame* m_pGame;

protected:
};