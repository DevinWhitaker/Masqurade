#include "Bullet.h"
#include "Game.h"

#include "DestroyBulletMessage.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "GameplayState.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "Camera.h"
// Constructor
CBullet::CBullet(void)
{
	CBaseObject::m_nType = OBJ_BULLET;
	m_nCurRotation = -1.0f;
	m_pOwner = nullptr;
	m_bIsFlipped = false;
	m_fElapsed = 0.0f;

	CSGD_EventSystem::GetInstance()->RegisterClient( "Pie Landed", this );
	m_fTimeAlive = 0.0f;
	m_fLifeDuration = 30.0f;
	m_bFuseLit = false;
	m_bPieLanded = false;
	m_fObjectMovementScalar = 0.25f;
}

// Destructor
CBullet::~CBullet(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClient( "Pie Landed", this );
	SetOwner(nullptr);
}

void CBullet::Update( float fElapsedTime )
{
	CBaseObject::Update( fElapsedTime );
	m_fTimeAlive += fElapsedTime;

	if(m_fTimeAlive >= m_fLifeDuration)
	{
		CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( this );
		CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
		pMsg = nullptr;
	}

	// Did this bullet go off the screen?
	RECT rSelf = GetRect();
	CGame* pGame = CGame::GetInstance();

	CCamera* cam = CCamera::GetInstance();

	switch(m_nBulletType)
	{
	case SQUIRT_BULLET:
	
		if( rSelf.bottom < 0+cam->OffsetY() || rSelf.top > pGame->GetHeight()+cam->OffsetY() 
			|| rSelf.right < 0+cam->OffsetX() || rSelf.left > pGame->GetWidth()+cam->OffsetX() )
		{
			// Self Destruct Sequence Activated
			CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( this );
			CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
			pMsg = nullptr;
		}

		break;
	case BALLOON_BULLET:

		break;
	case COTTON_BULLET:
		if( m_fTimeAlive >= m_fLifeDuration )
		{
			// Self Destruct Sequence Activated
			CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( this );
			CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
			pMsg = nullptr;

		}
		break;
	case PIE_BULLET:
		{
			m_fElapsed += fElapsedTime;

			if( m_fElapsed > PIE_DURATION )
			{
				// Self Destruct Sequence Activated
				CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( this );
				CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
				pMsg = nullptr;
			}
		}
		break;
	}

	//printf( "bullet X pos: %f\n", GetX());
	//printf( "bullet Y pos: %f\n", GetY());
}

void CBullet::Render(void)
{
	if(GetImageID() != -1)
	{
		float flip = 1.0f;
		if(m_bIsFlipped)
			flip = -flip;

		CSGD_TextureManager::GetInstance()->Draw( GetImageID(), 
			int(GetX()-GetWidth()/2.0f-CCamera::GetInstance()->OffsetX()),
			int(GetY()-GetHeight()/2.0f-CCamera::GetInstance()->OffsetY()), 
			flip*1.0f, 1.0f, 0, 
			GetWidth()/2.0f, GetHeight()/2.0f, 
			0.0f, DWORD(GetColor()) );
	}
}

RECT  CBullet::GetRect()
{
	RECT r;
	r.left = (LONG)(GetX()-GetWidth()/2.0f);
	r.top = (LONG)(GetY()-GetHeight()/2.0f);
	r.right = (LONG)(GetX()+GetWidth()/2.0f);
	r.bottom = (LONG)(GetY()+GetHeight()/2.0f);
	if(m_bIsFlipped)
	{
		r.left -= GetWidth();
		r.right -= GetWidth();
	}
	return r;
}

void CBullet::HandleEvent(CEvent* pEvent)
{
	
	if( pEvent->GetEventID() == "Pie Landed" &&
		pEvent->GetParam() == this )
	{
		SetVelX( 0.0f );
		SetVelY( 0.0f );
	}
	
}


bool CBullet::CheckCollision( IBaseObject* pOther )
{
	if( CBaseObject::CheckCollision( pOther ) == true)
	{
		switch(m_nBulletType)
		{
		case SQUIRT_BULLET:
			if(pOther->GetType() == OBJ_ENEMY)
			{
				CSGD_EventSystem::GetInstance()->QueueEvent( "Enemy_Hit", pOther);
				// Play Sound Here
				CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
				CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
				pMsg = nullptr;
			}
			break;
		case BALLOON_BULLET:

			break;
		case COTTON_BULLET:
			
				//if(pOther->GetType() != OBJ_FOX || pOther->GetType() != OBJ_CORINNE )
				if(pOther->GetType() == OBJ_ENEMY)
				{
					
					if(!m_bFuseLit)
					{
						m_fLifeDuration = 5.0f;
						m_bFuseLit = true;
			
						SetVelX(0);
						SetVelY(0);
						SetGravityBool(false);
					}
				}
				if(pOther->GetType() == OBJ_ENEMY || pOther->GetType() == OBJ_CRATE )				
				{
					pOther->SetScalar( m_fObjectMovementScalar );
				}
			break;
		case PIE_BULLET:
			if( pOther->GetType() != OBJ_ENEMY )
			{
				SetVelX( 0.0f );
				//SetVelY( 0.0f );
			}
			if(pOther->GetType() == OBJ_FOX)
			{
				//this->AddRef();
				CSGD_EventSystem::GetInstance()->QueueEvent( "Fox_Hit", this);
				// Play Sound Here
				/*CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
				CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
				pMsg = nullptr;*/
			}
			break;
		}
		
		
	}


	return false;
}

void CBullet::tileCollisionResponse(int COLLISION_SIDE)
{
	switch(m_nBulletType)
	{
	case SQUIRT_BULLET:
		{
			CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
			CSGD_MessageSystem::GetInstance()->QueueMessage( pMsg );
			pMsg = nullptr;
		}
		break;
	case BALLOON_BULLET:

		break;
	case COTTON_BULLET:
		{
			m_fLifeDuration = 5.0f;
			m_bFuseLit = true;

			SetVelX(0);
			SetVelY(0);
			SetGravityBool(false);
		}
		break;
	case PIE_BULLET:
		
		switch(COLLISION_SIDE)
		{
		case LEFT:
			SetVelX(0);
			break;
		case RIGHT:
			SetVelX(0);
			break;
		case TOP:
			CSGD_EventSystem::GetInstance()->QueueEvent( "Pie Landed", this );
			SetVelY(0);
			break;
		case BOTTOM:
			SetVelY(0);
			break;
		}
	}

	switch(COLLISION_SIDE)
	{
	case LEFT:
		SetVelX(0);
		break;
	case RIGHT:
		SetVelX(0);
		break;
	case TOP:
		SetVelY(0);
		break;
	case BOTTOM:
		SetVelY(0);
		break;
	}
}
