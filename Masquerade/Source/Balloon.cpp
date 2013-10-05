#include "Balloon.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "FileManager.h"
#include "Fox.h"

CBalloon::CBalloon( void )
{
	CBaseObject::m_nType = OBJ_BALLOON;
	m_nBalloonColor = BALLOON_BLUE;
	//SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Platform_Balloon.png") );
}

CBalloon::~CBalloon( void )
{

}

bool CBalloon::CheckCollision( IBaseObject* pOther )
{
	CBaseObject* pBase = dynamic_cast<CBaseObject*>(pOther);
	RECT r;
	BOOL isColliding =  IntersectRect( &r, &pBase->GetRect(), &GetRect());

	if( pBase->m_bJustColided )
	{
		if( pBase->GetRect().right < GetRect().left 
			|| pBase->GetRect().left > GetRect().right )
		{
			pBase->m_bJustColided = false;
			pBase->SetGravityBool(true);
		}
		else if( pBase->GetJumpBool() )
		{
			pBase->m_bJustColided = false;
			pBase->SetGravityBool(true);
		}
		else if( GetRect().top + 96 < 0 )
		{
			pBase->SetGravityBool( true );
		}
	}
	

	if( isColliding )
	{
		if( GetRect().top == r.top )
		{
			//if(pBase->GetType() == OBJ_CORINNE)
			//	int x = 0;
			//if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pBase->SetY( pBase->GetY() - nDist );
				//pBase->tileCollisionResponse(TOP);
				pBase->SetVelY(0.0f);
				pBase->SetJumpBool( false );
				if(pBase->GetType() == OBJ_FOX && GetVelY() < 0)
				{
					CFox* fox = dynamic_cast<CFox*>(pBase);
					fox->SetMaxYVelocityForReal(fox->GetNormalJumpVel() + GetVelY() );
				}
				//printf( "TOP\n" );
				pBase->m_bJustColided = true;
				//pBase->SetGravityBool(false);
				return true;
			}
			
		}
		if( GetRect().bottom == r.bottom )
		{
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pBase->SetY( pBase->GetY() + nDist );
				pBase->tileCollisionResponse(BOTTOM);
				//printf( "BOTTOM\n" );
				return true;
			}
			
		}
		if( GetRect().left == r.left )
		{
			if(pBase->GetType() == OBJ_CORINNE)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				int nDist = r.right - r.left;

				pBase->SetX( pBase->GetX() - nDist );

				pBase->tileCollisionResponse(LEFT);
				//printf( "LEFT\n" );
				return true;
			}
		}
		if( GetRect().right == r.right )
		{
			if(pBase->GetType() == OBJ_CORINNE)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				int nDist = r.right - r.left;
				pBase->SetX( pBase->GetX() + nDist );

				pBase->tileCollisionResponse(RIGHT);
				//printf( "RIGHT\n" );
				return true;
			}
		}
	}
	else
	{
		if(pBase->GetType() == OBJ_FOX)
		{
			CFox* fox = dynamic_cast<CFox*>(pBase);
			fox->SetMaxYVelocity(fox->GetNormalJumpVel() );
			//printf( "NONE\n" );
		}
		
	}

	return (isColliding) ? true : false;
}

void CBalloon::Update(float fElapsed)
{
	//CBaseObject::Update(fElapsed);
	//SetGravityBool(false);
	SetX( GetX() + GetVelX() * fElapsed );
	SetY( GetY() + GetVelY() * fElapsed );

	CGame* pGame = CGame::GetInstance();
	if( GetY() + 171 < 0 && GetVelY() < 0 )
	{
		SetY((float)(pGame->GetHeight() + 5));
	}
	else if( GetY() > pGame->GetHeight() && GetVelY() > 0 )
	{
		SetY(0 - 5);
	}
}

void CBalloon::Render(void)
{
	if(GetImageID() != -1)
	{
		CCamera* cam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		RECT sourceRect;
		sourceRect.left = m_nBalloonColor * 64;
		sourceRect.right = (m_nBalloonColor + 1) * 64;
		sourceRect.top = 0;
		sourceRect.bottom = 171;

		pTM->Draw( CFileManager::GetInstance()->GetImageIdMap()["MASQ_Platform_Balloon.png"].nId,
			(int)(GetX()/*-(GetWidth()/2.0f)*/-cam->OffsetX()), 
			(int)(GetY()/*-(GetHeight()/2.0f)*/-cam->OffsetY()),
			1.0f, 1.0f, &sourceRect );
	}
	//CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	//pD3D->GetSprite()->Flush();
	//CCamera* cam = CCamera::GetInstance();
	//RECT r = GetRect();
	//r.left -= (LONG)cam->OffsetX();
	//r.top -= (LONG)cam->OffsetY();
	//r.right -= (LONG)cam->OffsetX();
	//r.bottom -= (LONG)cam->OffsetY();
	//pD3D->DrawRect( r, 0, 255, 0);
}

void CBalloon::HandleEvent( CEvent* pEvent )
{
	// Is this event for us?
	if( pEvent->GetParam() != this )
		return;

	// Handle the unanchor_target event
	if( pEvent->GetEventID() == "balloon_reset" )
	{
		SetGravityFloat( 10.0f );
		SetGravityBool( false );
		SetY( 5.0f );
		SetX( 10.0f );
		//this->SetGravityFloat( 1.0f );
	}
}

RECT CBalloon::GetRect()
{
	RECT r;
	r.left = (LONG)GetX();
	r.top = (LONG)(GetY()+6);
	r.right = (LONG)(GetX()+GetWidth());
	r.bottom = (LONG)(GetY()+70);
	return r;
}

void CBalloon::tileCollisionResponse(int COLLISION_SIDE)
{

}
