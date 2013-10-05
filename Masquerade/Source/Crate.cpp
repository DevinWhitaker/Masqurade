#include "Crate.h"
#include "..\SGD Wrappers\CSGD_DirectInput.h"
#include "FileManager.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "..\SGD Wrappers\SGD_Math.h"

CCrate::CCrate(void)
{
	m_bIsMoovable = false;
	m_bHasBalloon = false;
	m_fTimeBalloonAttached = 0.0f;
	m_fMaxTimeBalloon = 5.0f;
	m_pControlingObject = nullptr;
	//SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Crate.png") );
	CFileManager::GetInstance()->AddImageID( "MASQ_Balloon_Bullet.png", 
	CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resources/Images/Graphics/MASQ_Balloon_Bullet.png")) );
	m_nBalloonImageID = CFileManager::GetInstance()->GetImageID("MASQ_Balloon_Bullet.png");
	m_fDistanceToCO = 0.0f;
	m_fGrabDistance = 128.0f;
	m_nType = OBJ_CRATE;
	m_fRaiseDistance = 64.0f;
	m_fStartPosY = 0.0f;
}


CCrate::~CCrate(void)
{
}

float CCrate::Distance(tVector2D x, tVector2D y)
{
	return sqrt( pow((x.fX-y.fX), 2) + pow((x.fY-y.fY), 2) );
}

void CCrate::Update(float fElapsed)
{
	CObjectManager* pOM = CObjectManager::GetInstance();
	std::vector<IBaseObject*> crates = pOM->FindObjectsWithType( OBJ_CRATE );

	tVector2D myPos;
	myPos.fX = GetX();
	myPos.fY = GetY();

	bool stuck = false;

	for(decltype(crates.size()) i = 0; i < crates.size(); ++i)
	{
		CBaseObject* crate = dynamic_cast<CBaseObject*>(crates[i]);
		if( crate->GetRect().top > GetRect().bottom || crate->GetRect().bottom < GetRect().top )
			continue;

		if( crate == this )
			continue;

		tVector2D cratePos;
		cratePos.fX = crate->GetX();
		cratePos.fY = crate->GetY();

		float dist = Distance( myPos, cratePos);
		
		if( dist <= GetWidth() + 1 )
		{
			//stuck = true;
			break;
		}
	}

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

#if !ARCADE
	if( ! pDI->KeyDown( DIK_E) )
	{
		m_bIsMoovable = false;
		SetVelX(0.0f);
		m_fDistanceToCO = 0.0f;
	}
#else
	if( ! pDI->JoystickButtonDown( 1 ) )
	{
		m_bIsMoovable = false;
		SetVelX(0.0f);
		m_fDistanceToCO = 0.0f;
	}
#endif

	if( stuck == false )
	{
		if( m_bIsMoovable && ! m_bHasBalloon && m_pControlingObject != nullptr )
		{
				float distToObject = GetX() - m_pControlingObject->GetX();
			if(m_fDistanceToCO > 0 && ! m_pControlingObject->GetMovingRightBool())
			{
				SetX( m_pControlingObject->GetX() + m_fDistanceToCO );
			}
			else if(m_fDistanceToCO < 0 && m_pControlingObject->GetMovingRightBool() )
			{
				SetX( m_pControlingObject->GetX() + m_fDistanceToCO );
			}
			else
			{
				SetMovingRightBool( m_pControlingObject->GetMovingRightBool() );
				SetVelX(m_pControlingObject->GetVelX());
			}
		}
	}
#if !ARCADE
	if(	m_pControlingObject != nullptr && pDI->KeyDown( DIK_E ) )
	{
		m_pControlingObject->SetJumpEnabled(false);
	}
	else if ( m_pControlingObject != nullptr )
	{
		m_pControlingObject->SetJumpEnabled(true);
	}
#else
	if(	m_pControlingObject != nullptr && pDI->JoystickButtonPressed( 1 ) )
	{
		m_pControlingObject->SetJumpEnabled(false);
	}
	else if ( m_pControlingObject != nullptr )
	{
		m_pControlingObject->SetJumpEnabled(true);
	}
#endif
		//if( m_pControlingObject!= nullptr)
		//printf("Fox's Vel X: %f\n", m_pControlingObject->GetVelX());

	if( m_bHasBalloon )
	{
		m_fTimeBalloonAttached += fElapsed;
		if( m_fStartPosY-GetY() >= m_fRaiseDistance)
			SetVelY( 0.0f );
	}
	else
	{
		SetGravityBool(true);
		SetGravityFloat(200.0f);
	}
	//printf("Time with balloon: %f\n", m_fTimeBalloonAttached);

	if( m_fTimeBalloonAttached >= m_fMaxTimeBalloon )
	{
		m_fTimeBalloonAttached = 0.0f;
		m_fStartPosY = 0.0f;
		m_bHasBalloon = false;
	}
	
	

	CBaseObject::Update(fElapsed);
}

void CCrate::Render(void)
{
	if(GetImageID() != -1)
	{
		CCamera* cam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		pTM->Draw( GetImageID(),
			(int)(GetX()-(GetWidth()/2.0f)-cam->OffsetX()), 
			(int)(GetY()-(GetHeight()/2.0f)-cam->OffsetY()));
	}

	if( m_nBalloonImageID != -1 )
	{
		if( m_bHasBalloon )
		{
			CCamera* cam = CCamera::GetInstance();
			CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
			int balloonHeight = pTM->GetTextureHeight(m_nBalloonImageID);

			/*pTM->Draw( m_nBalloonImageID, 
				(int)(GetX()-(GetWidth()/2.0f)-cam->OffsetX()),
				(int)(GetY()-(GetHeight()/2.0f)-cam->OffsetY()-balloonHeight/2.0f ));*/
			pTM->Draw( m_nBalloonImageID, 
				(int)(GetX()-(GetWidth()+16)-cam->OffsetX()),
				(int)(GetY()-balloonHeight/2-cam->OffsetY()-balloonHeight/2.0f ));
		}
	}
	/*CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	CCamera* cam = CCamera::GetInstance();
	RECT r = GetRect();
	r.left -= cam->OffsetX();
	r.right -= cam->OffsetX();
	r.top -= cam->OffsetY();
	r.bottom -= cam->OffsetY();
	CSGD_Direct3D::GetInstance()->DrawRect(r, 255, 255, 255 );*/
}


bool CCrate::CheckCollision( IBaseObject* pOther )
{
	CBaseObject* pBase = dynamic_cast<CBaseObject*>(pOther);
	RECT r;
	BOOL isColliding =  IntersectRect( &r, &pBase->GetRect(), &GetRect());

	if(pOther->GetType() == OBJ_CRATE)
			int x = 0;
	bool bCanMove = true;
	int side = -1;

	if( pBase->m_bJustColided )
	{
		if( pBase->GetRect().right < GetRect().left || pBase->GetRect().left > GetRect().right )
		{
			pBase->m_bJustColided = false;
			pBase->SetGravityBool(true);
		}
		else if( pBase->GetJumpBool() )
		{
			pBase->m_bJustColided = false;
			pBase->SetGravityBool(true);
		}
	}

	if( CBaseObject::CheckCollision( pOther ) == true && 
		( pOther->GetType() != OBJ_ENEMY && pOther->GetType() != OBJ_CHECKPOINT && 
		  pOther->GetType() != OBJ_SPRING && pOther->GetType() != OBJ_BALLOON )  )
	{

		/*if(pOther->GetType() == OBJ_FOX && 
			pOther->GetRect().bottom <= GetRect().bottom &&
			pOther->GetRect().bottom >= GetRect().top)
		{
			CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
			if( pDI->KeyDown(DIK_E) )
			{
				m_bIsMoovable = true;
				m_pControlingObject = pBase;
				m_fDistanceToCO = GetX() - m_pControlingObject->GetX();
			}
		}*/


		if( GetRect().top == r.top )
		{
			//if(pBase->GetType() == OBJ_CORINNE)
			//	int x = 0;
			//if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pBase->SetY( pBase->GetY() - nDist );

				if(pBase->GetType() != OBJ_BALLOON)
					pBase->tileCollisionResponse(TOP);
				pBase->SetJumpBool( false );
				pBase->m_bJustColided = true;
				//return true;
			}
			
		}
		if( GetRect().bottom == r.bottom )
		{
			if( (r.right - r.left) >= (r.bottom - r.top) && abs(pBase->GetVelX()) <= abs(pBase->GetVelY()) )
			{
				int nDist = r.bottom - r.top;
				pBase->SetY( pBase->GetY() + nDist );
				if(pBase->GetType() != OBJ_BALLOON)
					pBase->tileCollisionResponse(BOTTOM);
				//return true;
			}
			
		}
		if( GetRect().right == r.right )
		{
			if(pBase->GetType() == OBJ_CORINNE)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				int nDist = r.right - r.left;
				
				//if( ! m_bIsMoovable || pBase->GetType() != OBJ_FOX)
				/*f(pBase->GetType() == OBJ_CRATE)
					pBase->SetX( pBase->GetX() - nDist );
				else*/

				if(pBase->GetType() == OBJ_CRATE)
					m_bIsMoovable = false;

					pBase->SetX( pBase->GetX() + nDist );
					bCanMove = false;

					this->m_fDistanceToCO = 0.0f;

				if( ! m_bIsMoovable || pBase->GetType() != OBJ_FOX || pBase->GetType() != OBJ_BALLOON )
					pBase->tileCollisionResponse(RIGHT);

				//return true;
			}
		}
		if( GetRect().left == r.left )
		{
			if(pBase->GetType() == OBJ_CRATE)
			int x = 0;
			if( (r.right - r.left) <= (r.bottom - r.top) && abs(pBase->GetVelX()) >= abs(pBase->GetVelY()) )
			{
				int nDist = r.right - r.left;

				//if( ! m_bIsMoovable || pBase->GetType() != OBJ_FOX)
					pBase->SetX( pBase->GetX() - nDist );
					bCanMove = false;

				if(pBase->GetType() == OBJ_CRATE)
					m_bIsMoovable = false;

				if( ! m_bIsMoovable || pBase->GetType() != OBJ_FOX || pBase->GetType() != OBJ_BALLOON )
					pBase->tileCollisionResponse(LEFT);

				//return true;
			}
		}
		
	}

	if( bCanMove )
	{
		if(pOther->GetType() == OBJ_FOX && 
				pOther->GetRect().bottom <= GetRect().bottom &&
				pOther->GetRect().bottom >= GetRect().top)
		{
			CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
			if( pDI->KeyDown(DIK_E) || pDI->JoystickButtonDown( 1 ) )
			{
				float distance = fabs(GetX() - pBase->GetX());
				if( distance <= m_fGrabDistance )
				{
					m_bIsMoovable = true;
					m_pControlingObject = pBase;
					m_fDistanceToCO = GetX() - m_pControlingObject->GetX();
				}
			}
		}
	}

	if(isColliding && pOther->GetType() == OBJ_CRATE)
	{
		m_bIsMoovable = false;
	}

	return (isColliding) ? true : false;
}


bool CCrate::AttachBalloon(void)
{
	if( m_bHasBalloon == false)
	{
		m_bHasBalloon = true;
		m_bIsMoovable = false;
		SetVelY(-(GetGravityFloat()+50.0f));
		m_fStartPosY = GetY();
		return true;
	}
	return false;
}

bool CCrate::DetachBalloon(void)
{
	return true;
}

RECT CCrate::GetRect()
{
	return CBaseObject::GetRect();

	/*RECT r;
	r.left = (LONG)(GetX()-GetWidth()/2.0f);
	r.top = (LONG)(GetY()-GetHeight()/2.0f);
	r.right = (LONG)(GetX()+GetWidth()/2.0f)+300;
	r.bottom = (LONG)(GetY()+GetHeight()/2.0f);
	
	return r;*/
}