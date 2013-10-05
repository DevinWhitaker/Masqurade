// Project:		Masquerade
// Filename:	Tile.cpp
// Author:		Jam'D Team
// Purpose:		Tile Object implementation.

#include "Tile.h"
#include <string>
#include "BaseObject.h"
#include "Camera.h"
#include "GamePlayState.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"

CTile::CTile( void )
{
	CBaseObject::m_nType = OBJ_TILE;
	m_nTileCollideType = -1;
	m_nTileLayerType = -1;

	m_pTM = CSGD_TextureManager::GetInstance();
	m_nTileImageID = -1;
	m_nTileX = -1;
	m_nTileY = -1;

	SetX( -32.0f );
	SetY( -32.0f );

	m_pCamera = CCamera::GetInstance();
}

CTile::~CTile( void )
{

}

void CTile::Update( float fElapsed )
{
	CBaseObject::Update( fElapsed );
}

void CTile::Render( )
{
	// Should not be used.
}

void CTile::SetTile( int nTileX, int nTileY )
{
	m_nTileX = nTileX;
	m_nTileY = nTileY;

	SetX( (float)(m_nTileX * GetWidth() ) );
	SetY( (float)(m_nTileY * GetHeight() ) );
}

bool CTile::CheckCollision( CBaseObject * pBase )
{
	RECT r;
	if(pBase->GetType() == OBJ_FOX)
		int x = 0;
	
	BOOL isColliding =  IntersectRect( &r, &pBase->GetRect(), &GetRect());

	if(pBase->GetType() == OBJ_BULLET && GetX() == 0)
		int x = 0;

	if( isColliding && pBase->GetType() != OBJ_BALLOON )
	{
		if( GetRect().right == r.right )
		{
			if(pBase->GetType() == OBJ_FOX)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				float nDist = (float)(r.right - r.left);
				pBase->SetX( pBase->GetX() + nDist );
				pBase->SetLeftBool(true);
				pBase->tileCollisionResponse(RIGHT);
			}
		}

		if( GetRect().left == r.left )
		{
			if(pBase->GetType() == OBJ_CORINNE)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				float nDist = (float)(r.right - r.left);
				pBase->SetX( pBase->GetX() - nDist );
				pBase->SetRightBool(true);

				pBase->tileCollisionResponse(LEFT);
			}
		}

		if( GetRect().top == r.top )
		{
			if(pBase->GetType() == OBJ_FOX)
				int x = 0;
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				float nDist = (float)(r.bottom - r.top);
				if(pBase->GetType() == OBJ_FOX)
					pBase->SetY( pBase->GetY() - nDist - 2.0f);
				else
					pBase->SetY( pBase->GetY() - nDist );
				pBase->SetGravityBool(false);
				pBase->tileCollisionResponse(TOP);
				pBase->SetRightBool( false );
				pBase->SetLeftBool( false );
				pBase->SetJumpBool( false );
				pBase->SetGravityFloat( 1000.0f );
				return false;
			}
		}

		if( GetRect().bottom == r.bottom )
		{
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				float nDist = (float)(r.bottom - r.top);
				if(pBase->GetType() == OBJ_FOX)
					pBase->SetY( pBase->GetY() + nDist );
				pBase->SetVelX( pBase->GetVelX() + pBase->GetVelX()  / 2 );
				//pBase->tileCollisionResponse(BOTTOM);
				return false;
			}
		}
	}
	return TRUE == isColliding;
}

RECT CTile::GetRect( )
{
	RECT r;
	r.left		=	( LONG )( GetX() );
	r.top		=	( LONG )( GetY() );
	r.right		=	( LONG )( GetX() + GetWidth() );
	r.bottom	=	( LONG )( GetY() + GetHeight() );

	return r;
}