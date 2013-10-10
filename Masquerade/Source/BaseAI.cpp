/*
File:		CBaseAI.cpp
Course:		SGP2
Author:		JAM'D Studios ( Ja'Michael Garcia )
Purpose:	Base class for all ai states to inherit from.
*/

#include "BaseAI.h"
#include "IdleState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"

CBaseAI::CBaseAI(void)
{
	SetAIType( NONE );
	m_ObjectsInvolved.pCarried	= nullptr;
	m_ObjectsInvolved.pCarrier	= nullptr;
	SetScalarX( 1.0f );
	SetScalarY( 1.0f );

	CSGD_EventSystem::GetInstance()->RegisterClient( "JUMP", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "STOP", this );
}

CBaseAI::CBaseAI( int nAIType, CBaseObject* pCarrier, CBaseObject* pCarried )
{
	SetAIType( nAIType );
	m_ObjectsInvolved.pCarrier =	pCarrier;
	m_ObjectsInvolved.pCarried =	pCarried;
}


CBaseAI::~CBaseAI(void)
{

	//Exit();
}

///////////////////////////////////////////////////////////////////
//	Function:			"Enter"
//
//	Last Modified:		5/28/2012
//
//	Input:				IBaseObject* pB, CBaseObject* pHeldBy
//
//	Return:				void
//
//	Purpose:			Exits out the current state and handles
//						anything else that needs to be taken cared of
//						before switching to a new state.
///////////////////////////////////////////////////////////////////
void CBaseAI::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	m_ObjectsInvolved.pCarried	= (CBaseObject*)pB;
	m_ObjectsInvolved.pCarrier	= pHeldBy;
}

///////////////////////////////////////////////////////////////////
//	Function:			"Exit"
//
//	Last Modified:		5/28/2012
//
//	Input:				void
//
//	Return:				void
//
//	Purpose:			Exits out the current state and handles
//						anything else that needs to be taken cared of
//						before switching to a new state.
///////////////////////////////////////////////////////////////////
void CBaseAI::Exit(void)
{

	for( std::map< std::string, sNode >::iterator iters = m_mAINodeMap.begin();
		iters != m_mAINodeMap.end(); ++iters )
	{
		if( (*iters).second.bRegisterCarrierToEvent == true )
			CSGD_EventSystem::GetInstance()->UnregisterClient( (*iters).second.szNodeEvent, m_ObjectsInvolved.pCarrier );
		else
			CSGD_EventSystem::GetInstance()->UnregisterClient( (*iters).second.szNodeEvent, m_ObjectsInvolved.pCarried );
	}

	if( m_ObjectsInvolved.pCarried != nullptr )
	{
		//delete m_ObjectsInvolved.pCarried;
		m_ObjectsInvolved.pCarried	= nullptr;
	}

	if( m_ObjectsInvolved.pCarrier != nullptr )
	{
		//delete m_ObjectsInvolved.pCarrier;
		m_ObjectsInvolved.pCarrier	= nullptr;
	}

	m_mAINodeMap.clear();
}

///////////////////////////////////////////////////////////////////
//	Function:			"Update"
//
//	Last Modified:		5/28/2012
//
//	Input:				float fElapsedTime
//
//	Return:				void
//
//	Purpose:			Handles any special updating that needs to
//						happen within the current state.
///////////////////////////////////////////////////////////////////
void CBaseAI::Update( float fElapsedTime ) 
{
	NodeCollision(  );
}

///////////////////////////////////////////////////////////////////
//	Function:			"Render"
//
//	Last Modified:		5/28/2012
//
//	Input:				void
//
//	Return:				void
//
//	Purpose:			Handles any special rendering that needs to
//						happen within the current state.
///////////////////////////////////////////////////////////////////
void CBaseAI::Render(void)
{
}

///////////////////////////////////////////////////////////////////
//	Function:			"GetAIType"
//
//	Last Modified:		5/28/2012
//
//	Input:				void
//
//	Return:				int
//
//	Purpose:			Accessor to return the ai type of the ai state.
///////////////////////////////////////////////////////////////////
int	 CBaseAI::GetAIType(void) 
{
	return m_nAIType;
}

///////////////////////////////////////////////////////////////////
//	Function:			"AddAINode"
//
//	Last Modified:		5/28/2012
//
//	Input:				std::string NodeName, std::string NodeEvent, float fNodeWorldPositionX,
//							float fNodeWorldPositionY, unsigned int NodeWidth, unsigned int NodeHeight,
//								RECT nodeRect, bool bIsCarrierBeingRegistered 
//
//	Return:				void
//
//	Purpose:			Adds an node to m_mAINodeMap
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//	Function:			"RemoveAINode"
//
//	Last Modified:		5/28/2012
//
//	Input:				std::string szNodeName
//
//	Return:				bool
//
//	Purpose:			Removes a node from the m_mAINodeMap map.
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//	Function:			"GetAINodePosition"
//
//	Last Modified:		5/29/2012
//
//	Input:				std::string szNodeName
//
//	Return:				RECT
//
//	Purpose:			Returns an AI node from the m_mAINodeMap
//						Returns a -1 node if the node was not
//						found within the map.
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//	Function:			"GetAINodeEvent"
//
//	Last Modified:		5/29/2012
//
//	Input:				std::string szNodeName
//
//	Return:				std::string
//
//	Purpose:			Returns an AI node event from the m_mAINodeMap
//						Returns a -1 node if the node was not
//						found within the map.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//	Function:			"NodeCollision"
//
//	Last Modified:		5/28/2012
//
//	Input:				void
//
//	Return:				void
//
//	Purpose:			MUST BE CALLEDuh IN EVERY STATE!!!
//						Checks to see if the carrier in the struct
//						collided with any nodes and handles 
//						the aftermath of it by sending out events.
///////////////////////////////////////////////////////////////////
void CBaseAI::NodeCollision( void )
{
	for( std::map< std::string, sNode >::iterator iters = m_mAINodeMap.begin();
		iters != m_mAINodeMap.end(); ++iters )
	{
		RECT rIntersect;
		if( IntersectRect( &rIntersect, &(*iters).second.rNode, 
			&m_ObjectsInvolved.pCarrier->GetRect() ) == TRUE )
		{
			if( (*iters).second.bRegisterCarrierToEvent == true )
				CSGD_EventSystem::GetInstance()->QueueEvent( (*iters).second.szNodeEvent, m_ObjectsInvolved.pCarrier );
			else
				CSGD_EventSystem::GetInstance()->QueueEvent( (*iters).second.szNodeEvent, m_ObjectsInvolved.pCarried );
		}
	}
}

void CBaseAI::HandleEvent( const CEvent* pEvent )
{
	CBaseAI* tObj = dynamic_cast< CBaseAI* >((CBaseAI*)pEvent->GetParam());


	bool isUsingCarrier = true;

	if( tObj->GetObjectsInvolved().pCarried == GetObjectsInvolved().pCarried )
		isUsingCarrier = false;
	else if( tObj->GetObjectsInvolved().pCarrier == GetObjectsInvolved().pCarrier ) { }
	else
		return;

	if( isUsingCarrier )
	{
		if( tObj->GetObjectsInvolved().pCarrier == nullptr )
			return;
		if( tObj->GetAIType() != GetObjectsInvolved().pCarrier->GetCurrentState()->GetAIType() )
			return;
	}
	else
	{		
		if( tObj->GetObjectsInvolved().pCarried == nullptr )
			return;
		if( tObj->GetAIType() != GetObjectsInvolved().pCarried->GetCurrentState()->GetAIType() )
			return;
	}



	if( pEvent->GetEventID() == "JUMP" )
	{
		if( isUsingCarrier == true && GetObjectsInvolved().pCarrier != nullptr )
		{
			if( GetObjectsInvolved().pCarrier->GetJumpBool() == false )
			{
				GetObjectsInvolved().pCarrier->Jump( 150.0f );
				GetObjectsInvolved().pCarrier->SetVelX( 75.0f );
				GetObjectsInvolved().pCarrier->SetGravityBool( true );
			}
		}
		else if( isUsingCarrier == false && GetObjectsInvolved().pCarried != nullptr )
		{
			if( GetObjectsInvolved().pCarried->GetJumpBool() == false )
			{
				GetObjectsInvolved().pCarried->Jump( 150.0f );
				GetObjectsInvolved().pCarried->SetVelX( 75.0f );
				GetObjectsInvolved().pCarried->SetGravityBool( true );
			}
		}
		return;
	}

	if( pEvent->GetEventID() == "STOP" )
	{

		if( isUsingCarrier == true )
		{
			GetObjectsInvolved().pCarrier->SetVelX( -(GetObjectsInvolved().pCarrier->GetVelX()) );//GetObjectsInvolved().pCarrier->ChangeState( &cTempIdle );

		}
		else
		{
			GetObjectsInvolved().pCarried->SetVelX( -(GetObjectsInvolved().pCarried->GetVelX()) );
		}

		return;
	}
}