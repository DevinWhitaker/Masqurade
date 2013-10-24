#include "Corinne.h"
#include "../Source/AnimationManager.h"
#include "IAIState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CEvent.h"
#include "Fox.h"
#include "HeldState.h"
#include "IdleState.h"
#include "ThrowState.h"
#include "FollowState.h"
#include "SeekState.h"
#include "FileManager.h"
#include "FleeState.h"
#include "Hoodlum.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Lever.h"
#include "ResetMessage.h"
#include "BitmapFont.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "TriggeredObject.h"
#include "GamePlayState.h"

CCorinne::CCorinne(void)
{
	m_nType = OBJ_CORINNE;
	CSGD_EventSystem* pES = CSGD_EventSystem::GetInstance();

	pES->RegisterClient( "Pie Landed", this );
	m_frCorinne.Init( this );
	CAnimationManager::GetInstance()->AddFrame(m_frCorinne);

	m_eCurPowerup = CPowerup::NORMAL;
	m_fFoxMoveScalar = 0.65f;
	m_bIsGreased = false;
	m_fElapsed = -1.0f;
	m_bIsInflicted = false;
}


CCorinne::~CCorinne(void)
{
	CSGD_EventSystem* pES = CSGD_EventSystem::GetInstance();

	//if( this != nullptr 
		//pES->UnregisterClient( "Pie Landed", this );
}

void CCorinne::Update(float fElapsed)
{
	CGame*					pGame = CGame::GetInstance();

	if( m_fElapsed != -1.0f )
	{
		m_fElapsed += fElapsed;
		int red = (int)(50 * m_fElapsed);
		
		if( !m_bIsInflicted )
		{
			GetFrame().SetAnimationID( m_vIDs[D_INFLICTED] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			GetFrame().SetLooping(false);
			GetFrame().Play();
			m_bIsInflicted = true;
		}
		
		if( red >= PENALTY )
		{
			CGamePlayState::GetInstance()->RespawnFox();
			/*CResetMessage* msg = new CResetMessage;

			CSGD_MessageSystem::GetInstance()->QueueMessage( msg );*/

			m_fElapsed = -1.0f;
			GetFrame().SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			m_bIsInflicted = false;
		}
	}

	/*if( GetY() > CGame::GetInstance()->GetHeight() )
	{
		SetX( 200.0f );
		SetY( 400.0f );
	}*/

	if( GetX() < 0 )
		SetX( 0 );

	if( GetY() < 0 )
		SetY( 0 );

	if( GetX() > CTileManager::GetInstance()->GetWorldWidth() )
		SetX( (float)CTileManager::GetInstance()->GetWorldWidth() );

	if( GetCurrentState() != nullptr )
		GetCurrentState()->Update( fElapsed );

	CBaseObject::Update( fElapsed );
}

void CCorinne::Render()
{
	//CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	//GetFrame().Render();


	//else if( (me.right - cam->OffsetY()) < 0 )
	//{
	//	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	//	pTM->Draw( pFM->GetImageID( "MASQ_Corinne_Indicator.png" ), (int)GetX(), 50 );
	//}
	//else if( (me.right + cam->OffsetY()) > cam->GetScreenSpaceY() )
	//{
	//	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	//	pTM->Draw( pFM->GetImageID( "MASQ_Corinne_Indicator.png" ), (int)GetX(), (int)cam->GetScreenSpaceY() - 50 );
	//}
		//(me.bottom - cam->OffsetY()) < 0 || // check if im above the screen
		//(me.top - cam->OffsetY()) > (cam->GetScreenSpaceY() + cam->OffsetY()))
	CBaseObject::Render();

	//CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	//CCamera*	cam = CCamera::GetInstance();
	//pD3D->GetSprite()->Flush();
	//RECT r = GetRect();
	//r.left -= (LONG)cam->OffsetX();
	//r.top -= (LONG)cam->OffsetY();
	//r.right -= (LONG)cam->OffsetX();
	//r.bottom -= (LONG)cam->OffsetY();
	//pD3D->DrawRect( r, 255, 0, 0);
}

void CCorinne::setPowerup(CPowerup::PowerupType powerup)
{
	if( m_eCurPowerup == powerup )
		return;

	if( m_eCurPowerup != CPowerup::FIZZY_LIFTING_DRINK )
		m_fOrigGravity = this->GetGravityFloat();

	m_eCurPowerup = powerup;
	
	GetCurrentState()->SetScalarX(1.0f);
	GetCurrentState()->SetScalarY(1.0f);
	this->SetMaxXVelocity(m_fNormalMaxXvel);
	this->SetMaxYVelocity(m_fNormalMaxYvel);
	m_bIsGreased = false;
	SetGravityFloat( m_fOrigGravity );
	m_fFoxMoveScalar = 0.65f;
	

	switch(m_eCurPowerup)
	{
	case CPowerup::NORMAL:

		break;
	case CPowerup::GREASED_PIG:
		m_bIsGreased = true;
		break;
	case CPowerup::CHEETAH:
		GetCurrentState()->SetScalarX(2.0f);
		this->SetMaxXVelocity(GetMaxXVelocity() * 2.0f );
		break;
	case CPowerup::POGO:
		GetCurrentState()->SetScalarY(2.0f);
		this->SetMaxYVelocity(GetMaxYVelocity() * 2.0f );
		break;
	case CPowerup::FLUBBER:

		break;
	case CPowerup::FIZZY_LIFTING_DRINK:
		m_fFoxMoveScalar = 1.0f;
		SetGravityFloat( GetGravityFloat() * 0.1f );
		break;
	}
}


void CCorinne::AddAnimation( const char* szFileName, int nAnimationToUse )
{
	CAnimationManager*	pAM = CAnimationManager::GetInstance();

	//"Resources/Scripts/animation_anchors.xml"
	m_vIDs = pAM->CreateAnimationFromFile
		( szFileName );

	GetFrame().Init( this );
	GetFrame().SetAnimationID( m_vIDs[ nAnimationToUse ] );
}

void CCorinne::HandleEvent( const CEvent* pEvent )
{

	CBaseObject::sDataToPassIn* pBase = (CBaseObject::sDataToPassIn* )pEvent->GetParam();
	// Is this event for us?
	//if( pEvent->GetParam() != this )
	//	return;

	// Handle the unanchor_target event

		// Is this event for us?

	if( pEvent->GetEventID() == "Pie Landed" && GetCurrentState()->GetAIType() != IAIState::HELD)
	{
		CBullet*	pBullet = dynamic_cast< CBullet* >( (CBullet* )pEvent->GetParam() );
		
		if( pBullet->getBulletType() == PIE_BULLET )
		{
			if( GetCurrentState()->GetAIType() != IAIState::SEEK )
			{
				CSeekState*	pSeekState = new CSeekState();
				pSeekState->SetObjectsInvolved( GetCurrentState()->GetObjectsInvolved() );
				pSeekState->SetScalarX( 25.0f );
				pSeekState->SetXDistance( 256.0f );
				pSeekState->SetYDistance( 32.0f );
				pSeekState->AddSeekType( OBJ_BULLET,PIE_BULLET );
				ChangeState( pSeekState );
			}
			else if( GetCurrentState()->GetAIType() == IAIState::SEEK )
			{
				CSeekState*	pSeekState = dynamic_cast< CSeekState* >( GetCurrentState() );
				//pSeekState->AddSeekType( PIE_BULLET );

				if( pSeekState->IsObjectInBeingSeeked( OBJ_BULLET, PIE_BULLET ) == false )
					pSeekState->AddSeekType( OBJ_BULLET,PIE_BULLET );

				pSeekState->SetVectorUpdateBool( true );
			}
			if(GetFrame().GetAnimationID() != m_vIDs[D_SEEK])
				GetFrame().SetAnimationID( m_vIDs[D_SEEK] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			if(!GetFrame().isPlaying())
				GetFrame().Play(true);
		}
	}

	
	//if( GetCurrentState()->GetAIType() == IAIState::THROW )
	//{
	//	if( pEvent->GetParam() != this )
	//		return;
	//}
	else if( pBase->pCarried != this )
		return;

#if _DEBUG
	if( pEvent->GetEventID() == "blue_node" )
	{
		GetFrame().SetColor( D3DCOLOR_ARGB( 255, 0, 0, 255 ) );
	}
#endif
	if( pEvent->GetEventID() == "pickup" )
	{
		if( GetX() >= pBase->pCarrier->GetX() - 32 &&
			GetX() <= pBase->pCarrier->GetX() + 32 )
		{
			CHeldState* pHeldState = new CHeldState();
			//pHeldState->SetData( *pBase );

			pHeldState->SetObjectsInvolved( *pBase );
			pHeldState->GetObjectsInvolved().pCarried->SetVelX( 0.0f );
			pHeldState->GetObjectsInvolved().pCarried->SetVelY( 0.0f );
			/* POGO POWERUP HERE */
			pHeldState->SetScalarY( 1.0f );
			/*					*/
			ChangeState( pHeldState );
			//GetFrame().SetXScale( 4.0f );
			//GetFrame().SetYScale( 4.0f );
			m_fElapsed = -1.0f;
			GetFrame().SetColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			m_bIsInflicted = false;
			if(GetFrame().GetAnimationID() != m_vIDs[D_HELD])
				GetFrame().SetAnimationID( m_vIDs[D_HELD] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			if(!GetFrame().isPlaying())
				GetFrame().Play(true);
		}
	}
	else if( pEvent->GetEventID() == "putdown" )
	{
		CIdleState* pIdleState = new CIdleState();
		pIdleState->SetObjectsInvolved( *pBase );
		ChangeState( pIdleState );
		if(GetFrame().GetAnimationID() != m_vIDs[D_IDLE])
			GetFrame().SetAnimationID( m_vIDs[D_IDLE] );
		GetFrame().SetFlipped( !GetMovingRightBool() );
		if(!GetFrame().isPlaying())
			GetFrame().Play( );
	}
	else if( pEvent->GetEventID() == "throw" )
	{
		CHeldState* pHeld = dynamic_cast< CHeldState* >( GetCurrentState() );
		CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pBase->pCarried );
		CFox*	pFox = dynamic_cast< CFox* >( pBase->pCarrier );
		if( pFox->GetVelX() == 0.0f )
		{
			if( pCorinne->GetFrame().isFlipped() == false )
			{
				SetVelX( pFox->GetStrength() * pHeld->GetScalarX() );
				pCorinne->SetMovingRightBool( true );
			}
			else
			{
				SetVelX( -( pFox->GetStrength()* pHeld->GetScalarX() ) );
				pCorinne->SetMovingRightBool( true );
			}
		}
		else
		{

#if _DEBUG
			printf( "Fox Velocity X: %f \n", pFox->GetVelX() );
#endif
			if( pCorinne->GetFrame().isFlipped() == false )
			{
				pCorinne->SetMovingRightBool( pFox->GetMovingRightBool() );
				SetVelX(/* pFox->GetStrength() +*/ ( ( pFox->GetVelX() / 225.0f ) * pFox->GetStrength() ) );
			}
			else
			{
				pCorinne->SetMovingRightBool( true );
				SetVelX(/* -(pFox->GetStrength() +*/ -( ( pFox->GetVelX() / 225.0f) * pFox->GetStrength() ) );
			}
		}

		//CSGD_EventSystem::GetInstance()->
		float fYPower = pHeld->GetArchValue();
		if( fYPower <= -0.5f  && fYPower >= -1.0f)
		{
			fYPower *= 1.7f;
			//SetVelX( pFox->GetStrength() );
		}
		else if( fYPower <= -1.1f && fYPower >= -1.5f )
			fYPower *= 2.3f;		

		SetVelY( ( fYPower * 175.0f ) * pHeld->GetScalarY() );

		CThrowState* pState = new CThrowState();
		SetGravityBool( true );
		pState->SetObjectsInvolved( *pBase );
		//RECT rTemp = { 416, 416, 448, 448 };
		//pState->AddAINode( "Test", "blue_node", 416, 416, rTemp, true );
		ChangeState( pState );
		if(GetFrame().GetAnimationID() != m_vIDs[D_HELD])
			GetFrame().SetAnimationID( m_vIDs[D_HELD] );
		GetFrame().SetFlipped( !GetMovingRightBool() );
		if(!GetFrame().isPlaying())
			GetFrame().Play(true);
	}
	else if( pEvent->GetEventID() == "idle" )
		{
			CIdleState* pIdleState = new CIdleState();
			pIdleState->SetObjectsInvolved( GetCurrentState()->GetObjectsInvolved() );
			pIdleState->GetObjectsInvolved().pCarried->SetVelX( 0.0f );
			pIdleState->GetObjectsInvolved().pCarried->SetVelY( 0.0f );
			ChangeState( pIdleState );
			if(GetFrame().GetAnimationID() != m_vIDs[D_IDLE])
				GetFrame().SetAnimationID( m_vIDs[D_IDLE] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			if(!GetFrame().isPlaying())
				GetFrame().Play( );
		}
	else if( pEvent->GetEventID() == "follow" )
		{
			CFollowState* pIdleState = new CFollowState();
			pIdleState->SetObjectsInvolved( *pBase );
			pIdleState->SetDistance( 128.0f );

			pIdleState->SetScalarX( 25.0f );
			ChangeState( pIdleState );
			if(GetFrame().GetAnimationID() != m_vIDs[D_WALK])
				GetFrame().SetAnimationID( m_vIDs[D_WALK] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			if(!GetFrame().isPlaying())
			{
				if( getPowerup() == CPowerup::CHEETAH )
					GetFrame().Play(true,2.0f);
				else
					GetFrame().Play(true);
			}
			
		}
	else if( pEvent->GetEventID() == "seek" )
	{
		//enum EnemyType { NME_CLOWN, NME_RAT, NME_HOODLUM };
		CSeekState* pSeekState = new CSeekState();
		pSeekState->SetObjectsInvolved( *pBase );
		//pSeekState->AddFleeType( NME_HOODLUM );
		pSeekState->SetScalarX( 25.0f );

		pSeekState->AddSeekType( OBJ_TRIGGER, CTrigger::TRIG_LEVER );
		//pSeekState->AddSeekType( OBJ_FOX );
		pSeekState->SetXDistance( 256.0f );
		pSeekState->SetYDistance( 64.0f );
		//pSeekState->AddSeekType( OBJ_FOX );
		ChangeState( pSeekState );
		if(GetFrame().GetAnimationID() != m_vIDs[D_SEEK])
			GetFrame().SetAnimationID( m_vIDs[D_SEEK] );
		GetFrame().SetFlipped( !GetMovingRightBool() );
		if(!GetFrame().isPlaying())
			GetFrame().Play(true);
	}
	else if( pEvent->GetEventID() == "flee" )
		{
			//enum EnemyType { NME_CLOWN, NME_RAT, NME_HOODLUM };




			CFleeState* pFleeState = new CFleeState();
			pFleeState->SetObjectsInvolved( *pBase );
			//pFleeState->AddFleeType( NME_HOODLUM );
			pFleeState->AddFleeType( NME_HOODLUM );
			ChangeState( pFleeState );
			if (GetFrame().GetAnimationID() != m_vIDs[D_SEEK])
				GetFrame().SetAnimationID( m_vIDs[D_SEEK] );
			GetFrame().SetFlipped( !GetMovingRightBool() );
			if(!GetFrame().isPlaying())
				GetFrame().Play(true);
		}

}

bool CCorinne::CheckCollision(IBaseObject * pBase)
{
	//if( GetCurrentState()->GetAIType() == IAIState::HELD )
	//{
	//	if( pBase->GetType() == OBJ_PUZZLE )
	//	{
	//		CTriggeredObject*	pTriggered = dynamic_cast< CTriggeredObject* >( pBase );
	//		{			if( pTriggered->GetTriggeredType() == CTriggeredObject::TRIGOBJ_GATE )

	//			if( GetCurrentState()->GetObjectsInvolved().pCarrier->GetType() == OBJ_FOX )
	//				return pTriggered->CheckCollision( GetCurrentState()->GetObjectsInvolved().pCarrier );
	//		}
	//	}
	//	return false;
	//}
	return GetFrame().CheckCollision( (CBaseObject*)pBase );
}

RECT CCorinne::GetRect()
{
	//if( GetCurrentState()->GetAIType() != IAIState::HELD )
	//{
		return m_frCorinne.GetRect();
	//}
	//else
	//{
	//	RECT rEmpty = { };
	//	return rEmpty;
	//}
}

void CCorinne::tileCollisionResponse(int COLLISION_SIDE)
{
	if( GetCurrentState()->GetAIType() != IAIState::HELD )
	{
		SetGravityBool( true );
		switch(COLLISION_SIDE)
		{
		case LEFT:
			if( GetCurrentState()->GetAIType() == IAIState::SEEK )
			{
				CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "idle",  &GetCurrentState()->GetObjectsInvolved() );
				SetGravityBool( true );
				break;
			}
			(m_eCurPowerup == CPowerup::FLUBBER) ? SetVelX(-GetVelX()) : SetVelX(0);
			SetGravityBool(true);
			break;
		case RIGHT:
			if( GetCurrentState()->GetAIType() == IAIState::SEEK )
			{
				CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "idle",  &GetCurrentState()->GetObjectsInvolved() );
				SetGravityBool( true );
				break;
			}
			(m_eCurPowerup == CPowerup::FLUBBER) ? SetVelX(-GetVelX()) : SetVelX(0);		
			SetGravityBool(true);
			break;
		case TOP:
		SetGravityBool( false );
			SetVelY(0);
			if( GetCurrentState()->GetAIType() == IAIState::THROW &&
				CSGD_EventSystem::GetInstance()->HasEventTriggered( "seek" ) == false )
				CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "seek", &GetCurrentState()->GetObjectsInvolved() );
			break;
		case BOTTOM:
			SetVelY(0);
			SetGravityBool(true);
			break;
		default:
			break;
		}
		SetGravityBool(true);
	}
}

void CCorinne::CollisionResponse(  CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor  )
{

	if( pBase->GetType() == OBJ_ENEMY && m_fElapsed == -1 )
	{
		m_fElapsed = 0.0f;
		CIdleState* pIdle = new CIdleState;
		pIdle->Enter( this );
		ChangeState( pIdle );
	}

}

void CCorinne::SetIdleAnimation()
{
	GetFrame().SetAnimationID( m_vIDs[D_IDLE] );
}