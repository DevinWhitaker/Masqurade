// Project:		Masquerade
// Filename:	Spring.cpp
// Author:		Jam'D Team
// Purpose:		Spring Object implementation.

#include "Spring.h"
#include "FileManager.h"
#include "Camera.h"
#include "Fox.h"

CSpring::CSpring( void )
{
	//SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Spring.png") );
	m_nType = OBJ_SPRING;
}

CSpring::CSpring( CSpring &cs )
{
	SetX( cs.GetX() );
	SetY( cs.GetY() );
	SetVelX( cs.GetVelX() );
	SetVelY( cs.GetVelY() );
	SetGravityBool(cs.GetGravityBool());
	SetGravityFloat(cs.GetGravityFloat());
	SetWidth(cs.GetWidth());
	SetHeight(cs.GetHeight());
	SetLayerType( cs.GetLayerType() );
	//SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Spring.png") );

}

CSpring::~CSpring( void )
{

}

void CSpring::Render()
{
	if(GetImageID() != -1)
	{
		CCamera* pCam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		pTM->Draw( GetImageID(),
			(int)(GetX( )  - pCam->OffsetX( )), 
			(int)(GetY( )  - pCam->OffsetY( )), 0.5f, 0.5f );
	}
}

void CSpring::Update( float fElapsed )
{
	CBaseObject::Update(fElapsed);
}

RECT CSpring::GetRect()
{
	RECT r;
	r.left = (LONG)(GetX());
	r.top = (LONG)(GetY() + 32);
	r.right = (LONG)(GetX()+GetWidth());
	r.bottom = (LONG)(GetY()+GetHeight());

	return r;
}

bool CSpring::CheckCollision( IBaseObject* pBase )
{
	if( CBaseObject::CheckCollision( pBase ) == true )
	{
		CBaseObject* pOther = dynamic_cast<CBaseObject*>(pBase);

		RECT r;
		BOOL isColliding =  IntersectRect( &r, &pBase->GetRect(), &GetRect());

		if( GetRect().top == r.top )
		{
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pOther->GetVelX()) <= abs(pOther->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pOther->SetVelY( -650.0f );
				pOther->SetGravityFloat( 700.0f );
				if( pOther->GetType() == OBJ_FOX )
				{
					CFox* fox = dynamic_cast<CFox*>(pOther);
					fox->setJumpAnimation();
				}
				pOther->SetJumpBool( true );
			}

		}

		if( GetRect().bottom == r.bottom )
		{
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pOther->GetVelX()) <= abs(pOther->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pOther->SetY( pOther->GetY() + nDist );
				pOther->tileCollisionResponse(BOTTOM);
			}
		}

		if( GetRect().left == r.left )
		{
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pOther->GetVelX()) >= abs(pOther->GetVelY()) )
			{
				int nDist = r.right - r.left;
				pOther->SetX( pOther->GetX() - nDist );
				pOther->tileCollisionResponse(LEFT);
			}
		}

		if( GetRect().right == r.right )
		{
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pOther->GetVelX()) >= abs(pOther->GetVelY()) )
			{
				int nDist = r.right - r.left;
				pOther->SetX( pOther->GetX() + nDist );
				pOther->tileCollisionResponse(RIGHT);
			}


		}

		return (isColliding) ? TRUE : FALSE;
	}
	
	return false;
}