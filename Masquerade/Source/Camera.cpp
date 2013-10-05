// Project:		Masquerade
// Filename:	Camera.cpp
// Author:		Jam'D Team
// Purpose:		Camera implementation.

#include "Camera.h"
#include "TileManager.h"

CCamera* CCamera::GetInstance( )
{
	static CCamera s_Instance;

	return &s_Instance;
}

CCamera::CCamera( )
{
	m_fXOffset = 0.0f;
	m_fYOffset = 0.0f;
	m_bPlayerControlled = false;

	m_pGame = CGame::GetInstance();
}

CCamera::~CCamera( )
{

}

void CCamera::Update( float fElapsed )
{
	//m_fXOffset += 1.0f * fElapsed;
	
	if(m_fXOffset < 0)
		m_fXOffset = 0;
	if(m_fYOffset < 0)
		m_fYOffset = 0;

	CTileManager* TM = CTileManager::GetInstance();

	if( m_fXOffset + GetScreenSpaceX() > TM->GetWorldWidth() )
		m_fXOffset = TM->GetWorldWidth() - GetScreenSpaceX();

	/*if( m_fYOffset + GetScreenSpaceY() > TM->GetWorldHeight() )
		m_fYOffset = TM->GetWorldHeight() - GetScreenSpaceY();*/
}

void CCamera::Input( )
{
	// Panning camera for Fox when in Camera Mode.
}

float CCamera::GetScreenSpaceX()
{

	return (float)(m_pGame->GetWidth());
}

float CCamera::GetScreenSpaceY()
{
	return (float)(m_pGame->GetHeight());

}

void CCamera::Reset( void )
{
	m_fXOffset = 0.0f;
	m_fYOffset = 0.0f;
}