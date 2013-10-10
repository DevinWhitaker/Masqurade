/*
	File:		IBaseObject.cpp
	Course:		SGD 1204
	Author:		Jam'D Team
	Purpose:	contain and manager all BaseObject information
*/ 
#include "BaseObject.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "BaseAI.h"
#include "IdleState.h"
#include "HeldState.h"
#include "ThrowState.h"
#include "FleeState.h"
#include "SeekState.h"
#include "FollowState.h"
#include "Corinne.h"
#include "GamePlayState.h"
#include "Fox.h"

RECT	CBaseObject::GetRect()
{
	RECT r;
	r.left = (LONG)(GetX()-GetWidth()/2.0f);
	r.top = (LONG)(GetY()-GetHeight()/2.0f);
	r.right = (LONG)(GetX()+GetWidth()/2.0f);
	r.bottom = (LONG)(GetY()+GetHeight()/2.0f);

	/*RECT hitbox = { (LONG) ( this->GetX() - (this->GetWidth() / 2) ), 
		(LONG)( this->GetY() - (this->GetHeight() / 2) ), 
		(LONG)( this->GetWidth() + this->GetX() ), 
		(LONG)( this->GetHeight() + this->GetY() ) };
	return hitbox;*/
	return r;
}

bool CBaseObject::CheckCollision(IBaseObject * pBase)
{
	RECT r;
	BOOL isColliding =  IntersectRect( &r, &pBase->GetRect(), &GetRect());
	if( isColliding == TRUE )
	{
		return true;
	}
	
	return false;
}


CBaseObject::CBaseObject(void)
{
	m_fX = 0;
	m_fY = 0;

	m_fVelX = 0;
	m_fVelY = 0;


	m_unWidth = 0;
	m_unHeight = 0;


	m_nImageID = -1;
	m_unColor = D3DCOLOR_XRGB(255, 255, 255);

	m_unRefCount = 1;

	m_nType = OBJ_BASE;

	SetJumpBool( false );
	SetRightBool( false );
	SetLeftBool( false );
	SetTopBool( false );

	SetMaxXVelocity( 0.0f );
	SetMaxYVelocity( 0.0f );
	SetGravityBool( true );
	SetMaxJumpHeight( 0.0f);
	SetGravityFloat( 9.8f );
	m_pCurrObjectState = nullptr;
	SetStopVelocityBool( false );
	SetMovingRightBool( true );
	SetLayerType( 1 );
	m_fTimeScalar = 1.0f;
	m_bJumpEnabled = true;
	m_bJustColided = false;
}


CBaseObject::~CBaseObject(void)
{
	if( GetCurrentState() != nullptr )
	{
		GetCurrentState()->Exit();
		delete GetCurrentState();
		SetCurrentState( nullptr );
	}
}

void CBaseObject::Update(float fElapsedTime)
{

	if( GetCurrentState() != nullptr )
		GetCurrentState()->Update( fElapsedTime );

	// modify the token's position by its velocity

	if( GetMovingRightBool() == true)
	m_fX += m_fVelX * fElapsedTime * m_fTimeScalar;
	else
		m_fX -= m_fVelX * fElapsedTime * m_fTimeScalar;
	m_fY += m_fVelY * fElapsedTime * m_fTimeScalar;

	if( GetGravityBool() == true )
		Gravity( fElapsedTime );
	
	m_fTimeScalar = 1.0f;
}

void CBaseObject::Render(void)
{
	if( GetCurrentState() != nullptr )
		GetCurrentState()->Render();
	// render the token's image at its position (top-left corner)
	if(m_nImageID != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw( m_nImageID, 
			int(m_fX), int(m_fY), 1.0f, 1.0f, nullptr, 0.0f, 0.0f, 
			0.0f, DWORD(m_unColor) );
	}
}

void CBaseObject::AddRef(void)
{
	++m_unRefCount;
}

void CBaseObject::Release(void)
{
	--m_unRefCount;

	// Auto deallocate this object when no more references
	if( m_unRefCount == 0 )
		delete this;
}

void CBaseObject::ChangeState( CBaseAI* newState)
{

//	CSGD_EventSystem*	m_pES = CSGD_EventSystem::GetInstance();
//
////	CBaseAI holddata = *newState;
//	if( GetCurrentState() != nullptr )
//	{
//		//GetCurrentState()->Exit();
//		delete GetCurrentState();
//		SetCurrentState( nullptr );
//	}
//	SetCurrentState( newState );
//
//	switch( GetCurrentState()->GetAIType() )
//	{
//	case IAIState::IDLE:
//		{
//			//m_pES->UnregisterClientAll( this );
//			CIdleState*	pIdleState = dynamic_cast< CIdleState* >( GetCurrentState() );
//			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, 
//				pIdleState->GetObjectsInvolved().pCarried );
//		}
//		break;
//	case IAIState::HELD:
//		{
//			//m_pES->UnregisterClientAll( this );
//			CHeldState*	pIdleState = dynamic_cast< CHeldState* >( GetCurrentState() );
//			//m_pES->UnregisterClientAll( this );
//			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, pIdleState->GetObjectsInvolved().pCarrier);
//			m_pES->RegisterClient( "putdown", this );
//			m_pES->RegisterClient( "throw", this);
//		}
//		break;
//	case IAIState::THROW:
//		{
//			//m_pES->UnregisterClientAll( this );
//			CThrowState*	pIdleState = dynamic_cast< CThrowState* >( GetCurrentState() );
//			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, pIdleState->GetObjectsInvolved().pCarrier );
//			m_pES->RegisterClient( "idle", this );
//		}
//		break;
//	case IAIState::FOLLOW:
//		{
//			//m_pES->UnregisterClientAll( this );
//			CFollowState*	pThrowState = dynamic_cast< CFollowState* >( GetCurrentState() );
//			GetCurrentState()->Enter( pThrowState->GetObjectsInvolved().pCarried, pThrowState->GetObjectsInvolved().pCarrier/*, 128*/ );
//			//m_pES->RegisterClient( "idle", this );
//			m_pES->RegisterClient( "pickup", this );
//		}
//		break;
//	case IAIState::SEEK:
//		{
//	//		CSeekState*	pSeekState = dynamic_cast< CSeekState* >( GetCurrentState() );
////			GetCurrentState()->Enter( pThrowState->GetObjectsInvolved().pCarried, pThrowState->GetObjectsInvolved().pCarrier );
//			//m_pES->RegisterClient( "
//		}
//		break;
//	default:
//		break;
//	}

	CSGD_EventSystem*	m_pES = CSGD_EventSystem::GetInstance();

//	CBaseAI holddata = *newState;
	//if( GetCurrentState() != nullptr )
	//{
	//	//GetCurrentState()->Exit();
	//	delete GetCurrentState();
	//	//delete m_pCurrObjectState;
	//	SetCurrentState( nullptr );
	//}
	SetCurrentState( newState );

	switch( GetCurrentState()->GetAIType() )
	{
	case IAIState::IDLE:
		{
			//m_pES->UnregisterClientAll( this );
			CIdleState*	pIdleState = dynamic_cast< CIdleState* >( GetCurrentState() );
			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, 
				pIdleState->GetObjectsInvolved().pCarried );
		}
		break;
	case IAIState::HELD:
		{
			//m_pES->UnregisterClientAll( this );
			CHeldState*	pIdleState = dynamic_cast< CHeldState* >( GetCurrentState() );
			//m_pES->UnregisterClientAll( this );
			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, pIdleState->GetObjectsInvolved().pCarrier);
			//m_pES->RegisterClient( "putdown", this );
			//m_pES->RegisterClient( "throw", this);
		}
		break;
	case IAIState::THROW:
		{
			//m_pES->UnregisterClientAll( this );
			CThrowState*	pIdleState = dynamic_cast< CThrowState* >( GetCurrentState() );
			GetCurrentState()->Enter( pIdleState->GetObjectsInvolved().pCarried, pIdleState->GetObjectsInvolved().pCarrier );
			m_pES->RegisterClient( "idle", this );
		}
		break;
	case IAIState::FOLLOW:
		{
			//m_pES->UnregisterClientAll( this );
			CFollowState*	pThrowState = dynamic_cast< CFollowState* >( m_pCurrObjectState/*GetCurrentState()*/ );
			GetCurrentState()->Enter( pThrowState->GetObjectsInvolved().pCarried, pThrowState->GetObjectsInvolved().pCarrier/*, 128*/ );
			//m_pES->RegisterClient( "idle", this );
			m_pES->RegisterClient( "pickup", this );
		}
		break;
	case IAIState::SEEK:
		{
			CSeekState*	pSeekState = dynamic_cast< CSeekState* >( GetCurrentState() );
			GetCurrentState()->Enter( pSeekState->GetObjectsInvolved().pCarried, pSeekState->GetObjectsInvolved().pCarrier );
			m_pES->RegisterClient( "flee", this );
			m_pES->RegisterClient( "idle", this );
			m_pES->RegisterClient( "pickup", this );
			m_pES->RegisterClient( "follow", this );
		}
		break;
	case IAIState::FLEE:
		{
			CFleeState*	pFleeState = dynamic_cast< CFleeState* >( GetCurrentState() );
			GetCurrentState()->Enter( pFleeState->GetObjectsInvolved().pCarried, pFleeState->GetObjectsInvolved().pCarrier );
			m_pES->RegisterClient( "idle", this );
			m_pES->RegisterClient( "pickup", this );
			m_pES->RegisterClient( "follow", this );
		}
		break;
	default:
		break;
	}

}

void CBaseObject::HandleEvent(const CEvent* pEvent)
{
	if( GetCurrentState() == nullptr )
		return;
}



void CBaseObject::IncreaseVelocity( float fAmountToIncrease, bool bXVelocity)
{
	if( bXVelocity == true && GetMaxXVelocity() > GetVelX() )
		SetVelX( GetVelX() + fAmountToIncrease );
	else if( bXVelocity == false && GetMaxXVelocity() < GetVelX() )
		SetVelY( GetVelY() + fAmountToIncrease );
}

void CBaseObject::DecreaseVelocity( float fAmountToDecrease, bool bXVelocity )
{
	if( bXVelocity == true && -GetMaxXVelocity() < GetVelX() )
	{
		SetVelX( GetVelX() - fAmountToDecrease );
		if( GetVelX() < 0.0f )
			SetVelX( 0.0f );
	}
	else/* if( bXVelocity == false && GetVelY() >= 0 )*/
	{
		//printf( "Base Object Fox maxYVel: %f\n", GetMaxYVelocity());

		SetVelY( GetVelY() - fAmountToDecrease );
	}
}

void CBaseObject::HaltVelocity( float fElapsed )
{
	SetVelX( 0.0f );
	SetVelY( 0.0f );
}

void CBaseObject::Gravity( float fElapsed )
{
	if( GetGravityBool() == true  )
	{
		SetVelY( GetVelY() + ( GetGravityFloat() * fElapsed ) );
		

	}
	if( GetVelX() > VELYCAP)
		SetVelX(VELYCAP);
	if( GetVelX() < -VELYCAP)
		SetVelX(-VELYCAP);
	if( GetVelY() > VELYCAP)
		SetVelY(VELYCAP);
	if( GetVelY() < -VELYCAP)
		SetVelY(-VELYCAP);

}

void CBaseObject::Jump( float fJumpVariable )
{
	while( GetMaxYVelocity() < GetVelY() )
 		DecreaseVelocity( fJumpVariable, false );
	//SetStopVelocityBool( true );
	//SetVelY( 0.0f );

}

void CBaseObject::SlowDown( float fElapsed )
{
	if( GetVelX() == 0.0f )
		return;
	if( GetVelX() > 0.0f )
	{
		DecreaseVelocity( fElapsed, true );
		if( GetVelX() < 0.0f )
			SetVelX( 0.0f );
	}
	else if( GetVelX() < 0.0f )
	{
		IncreaseVelocity( fElapsed, true );
		if( GetVelX() > 0.0f )
			SetVelX( 0.0f );
	}
}

void CBaseObject::CollisionResponse( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor )
{
	int x = 0;
}

void CBaseObject::tileCollisionResponse(int COLLISION_SIDE)
{
	switch( COLLISION_SIDE )
	{
	case LEFT:
		SetVelX(0);
		break;
	case RIGHT:
		SetVelX(0);
		break;
	case TOP:
		{
		SetVelY(0);
		}
		break;
	case BOTTOM:
		SetVelY(0);
		break;
	}
}
