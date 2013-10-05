/*
	File:		FollowState.cpp
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Implements the following behavior for ai.
*/

#include "FollowState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
//#include "ba
#include "Enemy.h"
#include "Corinne.h"

CFollowState::CFollowState(void)
{
	m_nAIType = FOLLOW;
	GetObjectsInvolved().pCarried = nullptr;
	GetObjectsInvolved().pCarrier = nullptr;
	SetRangeBool( false );
	SetDistance( 1.0f );
	SetScalarX( 1.0f );
	SetScalarY( 1.0f );
}


CFollowState::~CFollowState(void)
{
}

void CFollowState::Enter( IBaseObject* pB, CBaseObject* pHeldBy, bool bCarriedMoving )
{
	GetObjectsInvolved().pCarried = (CBaseObject*)pB;
	GetObjectsInvolved().pCarrier = pHeldBy;
	SetRangeBool( false );
	m_bCarriedFollowing = bCarriedMoving;
	//SetDistance( fDistanceToKeep );

	if( GetObjectsInvolved().pCarried->GetCurrentState() == nullptr &&
		GetObjectsInvolved().pCarried->GetType() != OBJ_BASE )
		GetObjectsInvolved().pCarried->SetCurrentState( this );
	m_nAIType = FOLLOW;
}

void CFollowState::Exit(void)
{
	CBaseAI::Exit();
}

void CFollowState::Update( float fElapsedTime )
{
			/*	if( GetX() >= pBase->pCarrier->GetX() - 32 &&
				GetX() <= pBase->pCarrier->GetX() + 32 )*/

	//GetObjectsInvolved().pCarried->SetY( 50.0f );

	if( m_bCarriedFollowing != true )
	{
		//SetRangeBool( false );
		if( GetObjectsInvolved().pCarried->GetX() >= GetObjectsInvolved().pCarrier->GetX() - 32  &&
			GetObjectsInvolved().pCarried->GetX() <= GetObjectsInvolved().pCarrier->GetX() + 32 )
		{
				SetRangeBool( true );
				GetObjectsInvolved().pCarried->SetVelX( 0.0f );
				//GetObjectsInvolved().pCarried->SetVelY( 0.0f );
				CBaseAI::Update( fElapsedTime );
				return;
			//if( GetRangeBool() == false )
			//{
			//	CSGD_EventSystem::GetInstance()->SendEvent( "idle", &GetObjectsInvolved() );
			//	SetRangeBool( true );
			//}
			//return;
		}


		if( GetObjectsInvolved().pCarried->GetX() > GetObjectsInvolved().pCarrier->GetX()  )
		{
					m_ObjectsInvolved.pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
					m_ObjectsInvolved.pCarried->SetMovingRightBool( false );
					if( m_ObjectsInvolved.pCarried->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( m_ObjectsInvolved.pCarried );
						pCorinne->GetFrame().SetFlipped( true );
					}

					if( GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( m_ObjectsInvolved.pCarried );
						RECT rCorinne = pCorinne->GetFrame().GetRect();
						if(  rCorinne.bottom < GetObjectsInvolved().pCarrier->GetY() )
						{
							pCorinne->IncreaseVelocity( 10.0f, false );
							pCorinne->SetGravityBool( true );
						}
						else if( rCorinne.top > GetObjectsInvolved().pCarrier->GetY() )
							pCorinne->SetGravityBool( true );
					}
		}
		else if( GetObjectsInvolved().pCarried->GetX() < GetObjectsInvolved().pCarrier->GetX()  )
		{
					m_ObjectsInvolved.pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
					m_ObjectsInvolved.pCarried->SetMovingRightBool( true );
					if( m_ObjectsInvolved.pCarried->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( m_ObjectsInvolved.pCarried);
						pCorinne->GetFrame().SetFlipped( false );
					}

					if( GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( m_ObjectsInvolved.pCarried );
						RECT rCorinne = pCorinne->GetFrame().GetRect();
						if(  rCorinne.bottom < GetObjectsInvolved().pCarrier->GetY() )
						{
							//pCorinne->SetY( GetObjectsInvolved().pCarrier->GetY() ); 
							pCorinne->IncreaseVelocity( 10.0f, false );
							pCorinne->SetGravityBool( true );
						}
						else if( rCorinne.top > GetObjectsInvolved().pCarrier->GetY() )
							pCorinne->SetGravityBool( true );

					}
		}


		//pCorinne->
	}
	else
	{
		//SetRangeBool( false );
		if( GetObjectsInvolved().pCarrier->GetX() >= GetObjectsInvolved().pCarried->GetX() - 32  &&
			GetObjectsInvolved().pCarrier->GetX() <= GetObjectsInvolved().pCarried->GetX() + 32 )
		{
	
			if( GetRangeBool() == false && m_ObjectsInvolved.pCarrier->GetType() == OBJ_ENEMY)
			{
				CEnemy*	pEnemy = dynamic_cast< CEnemy* >( m_ObjectsInvolved.pCarrier );

				if( pEnemy->getEnemyType() ==  NME_HOODLUM )
				{
					GetObjectsInvolved().pCarried->SetVelX( 0.0f );
					GetObjectsInvolved().pCarried->SetVelY( 0.0f );
					CSGD_EventSystem::GetInstance()->SendEvent( "pickup", &GetObjectsInvolved() );
					SetRangeBool( true );
				}
			}
			return;
		}

		if( GetObjectsInvolved().pCarrier->GetX() > GetObjectsInvolved().pCarried->GetX()  )
		{
			m_ObjectsInvolved.pCarried->SetMovingRightBool( false );
			m_ObjectsInvolved.pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
		}
		else if( GetObjectsInvolved().pCarrier->GetX() < GetObjectsInvolved().pCarried->GetX()  )
		{
			m_ObjectsInvolved.pCarried->SetMovingRightBool( true );
			m_ObjectsInvolved.pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
		}
	}

	CBaseAI::Update( fElapsedTime );

}

void CFollowState::Render(void)
{
}

void CFollowState::NodeCollision( void )
{
	CBaseAI::NodeCollision();
}

void  CFollowState::HandleEvent( CEvent* pEvent )
{
	CBaseAI::HandleEvent( pEvent );
}