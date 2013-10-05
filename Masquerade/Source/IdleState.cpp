#include "IdleState.h"
#include "BaseObject.h"
#include "Corinne.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"

CIdleState::CIdleState(void)
{
	this->m_nAIType = IDLE;
	this->GetObjectsInvolved().pCarried = nullptr;
	this->GetObjectsInvolved().pCarrier = nullptr;
}


CIdleState::~CIdleState(void)
{
//	Exit();
}

void CIdleState::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	GetObjectsInvolved().pCarried = (CBaseObject*)pB;
	GetObjectsInvolved().pCarrier = pHeldBy;

	if( GetObjectsInvolved().pCarried->GetCurrentState() == nullptr &&
		GetObjectsInvolved().pCarried->GetType() != OBJ_BASE )
		GetObjectsInvolved().pCarried->SetCurrentState( this );
	m_nAIType = IDLE;

			CSGD_EventSystem::GetInstance()->RegisterClient( "follow", GetObjectsInvolved().pCarried  );
			CSGD_EventSystem::GetInstance()->RegisterClient( "pickup", GetObjectsInvolved().pCarried  );
			CSGD_EventSystem::GetInstance()->RegisterClient( "seek", GetObjectsInvolved().pCarried  );
			CSGD_EventSystem::GetInstance()->RegisterClient( "flee", GetObjectsInvolved().pCarried  );

	//AttachObject( pB );
	//CBaseObject*	pBO = dynamic_cast< CBaseObject* >( GetAttachedObject() );

	//if( pBO->GetCurrentState() == nullptr )
	//	pBO->SetCurrentState( this );
	//m_nType = IDLE;

	//if( GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
	//{
	//	CCorinne*	pCO	= dynamic_cast< CCorinne* >( GetObjectsInvolved().pCarried );
	//	CSGD_EventSystem::GetInstance()->RegisterClient( "pickup", pCO );
	//	CSGD_EventSystem::GetInstance()->RegisterClient( "follow", pCO );
	//}
}

void CIdleState::Exit(void)		
{
	//for( std::map< std::string, sNode >::iterator iter = m_mAINodeMap.begin();
	//	iter != m_mAINodeMap.end(); ++iter )
	//{
	//	if( (*iter).second.bRegisterCarrierToEvent == true )
	//		CSGD_EventSystem::GetInstance()->UnregisterClient( (*iter).second.szNodeEvent, m_ObjectsInvolved.pCarrier );
	//	else
	//		CSGD_EventSystem::GetInstance()->UnregisterClient( (*iter).second.szNodeEvent, m_ObjectsInvolved.pCarried );
	//}

	if( GetObjectsInvolved().pCarried != nullptr )
	{
		//delete GetObjectsInvolved().pCarried;
		GetObjectsInvolved().pCarried	= nullptr;
	}

	if( GetObjectsInvolved().pCarrier != nullptr )
	{
		//delete GetObjectsInvolved().pCarrier;
		GetObjectsInvolved().pCarrier	= nullptr;
	}

	//m_mAINodeMap.clear();
}

void CIdleState::Update( float fElapsedTime )
{
	CBaseObject*	pB = dynamic_cast< CBaseObject* >( GetObjectsInvolved().pCarried );

	switch( pB->GetType() )
	{
	case OBJ_CORINNE:
		{
			CCorinne*	pCO	= dynamic_cast< CCorinne* >( pB );
			//pCO->SetX( rand() % 600 + 100 );
			if( !pCO->isInflicted() )
			{
				if( pCO->GetFrame().isPlaying() == false )
					pCO->GetFrame().Play( true );
			}
			

			pCO->SetGravityBool( true );
			pCO->SetGravityFloat( 200.0f );
			//pCO->SetVelY( 1.0f );
			//pCO->Update( fElapsedTime );
		}
		break;
	default:
		break;
	};
}

void CIdleState::Render(void)
{
	CBaseObject*	pB = dynamic_cast< CBaseObject* >( GetObjectsInvolved().pCarried );

	switch( pB->GetType() )
	{
	case OBJ_CORINNE:
		{
			CCorinne*	pCO	= dynamic_cast< CCorinne* >( pB );
			//pCO->GetFrame().SetColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		}
		break;
	default:
		break;
	};
}

void CIdleState::HandleEvent( CEvent* pEvent )
{
	CBaseAI::HandleEvent( pEvent );
}


//void CIdleState::NodeCollision( void )
//{
//	CBaseAI::NodeCollision();
//}
