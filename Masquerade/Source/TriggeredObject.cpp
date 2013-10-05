/*
	File:		TriggeredObject.cpp
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Base class for all objects being handle by triggers.
*/

#include "TriggeredObject.h"
#include "Frame.h"
//#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "AnimationManager.h"

CTriggeredObject::CTriggeredObject(void)
{
	m_nTriggeredType = TRIGOBJ_BASE;
	SetActivatedBool( false );
	m_nType = OBJ_PUZZLE;
	m_bHasBeenRegistered  = false;
	RegisterDefaultEvents(  );
	//SetTriggeredObjectFrame( nullptr, "" );
	//CAnimationManager::GetInstance()->AddFrame( *GetTriggeredObjectFrame() );
	RegisterDefaultEvents();
}

CTriggeredObject::CTriggeredObject( CFrame*	pFrame, std::string szFilePath, unsigned int unAnimationToUse, 
	float fpositionX, float fpositionY, bool bIsGravityApplied, 
	int nLayerType, std::string szID, float fGravityApplied )
{
	m_nTriggeredType = TRIGOBJ_BASE;
	SetActivatedBool( false );
	m_nType = OBJ_PUZZLE;
	m_bHasBeenRegistered  = false;
	RegisterDefaultEvents(  );

	SetX( fpositionX );
	SetY( fpositionY );

	SetGravityBool( bIsGravityApplied );
	SetLayerType( nLayerType );
	SetID( szID );
	if( GetGravityBool() == true )
		SetGravityFloat( fGravityApplied );

	if( pFrame != nullptr )
		SetTriggeredObjectFrame( pFrame, szFilePath, unAnimationToUse );


}


CTriggeredObject::~CTriggeredObject(void)
{
	if( m_bHasBeenRegistered = true )
	{
		CSGD_EventSystem::GetInstance()->UnregisterClient( "activate", this );
		CSGD_EventSystem::GetInstance()->UnregisterClient( "deactivate", this );
		CSGD_EventSystem::GetInstance()->UnregisterClientAll( this );
	}
	
	if( m_pFrame != nullptr)
	{
		CAnimationManager::GetInstance()->RemoveFrame( m_pFrame );
	}
	/*if( GetTriggeredObjectFrame() != nullptr )
	{
		CAnimationManager::GetInstance()->RemoveFrame( GetTriggeredObjectFrame() );
		delete GetTriggeredObjectFrame();
		SetTriggeredObjectFrame( nullptr );
	}*/
}

void CTriggeredObject::Activate(void)
{
	CSGD_EventSystem::GetInstance()->SendUniqueEvent( "activate", this );
}

void CTriggeredObject::Deactivate(void)
{
	CSGD_EventSystem::GetInstance()->SendUniqueEvent( "deactivate", this );
}

void CTriggeredObject::SetTriggeredObjectFrame( CFrame* pFrame, std::string szFilePath, unsigned int unAnimationToUse )
{
	if( GetWidth() != 1 || GetHeight() != 1 )
	{
		SetWidth( 1 );
		SetHeight( 1 );
	}

	if( szFilePath.size() != 0)
	{
		std::vector< std::string > szAnimations;
		szAnimations = CAnimationManager::GetInstance()->CreateAnimationFromFile( szFilePath.c_str() );
		pFrame->Init( this );
		pFrame->SetAnimationID( szAnimations[unAnimationToUse] );
	}
	else
		return;

	if( pFrame == nullptr )
	{
		m_pFrame = pFrame ;
		CAnimationManager::GetInstance()->AddFrame( *m_pFrame );
	}
	else
	{
		CAnimationManager::GetInstance()->RemoveFrame( m_pFrame );
		m_pFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pFrame );
	}
}

void CTriggeredObject::AddEvent( std::string szEventName/*, CTriggeredObject* pTrigger */ )
{
	CSGD_EventSystem::GetInstance()->RegisterClient( szEventName, this );
}

void CTriggeredObject::SendEvent( std::string szEventName/*, CTriggeredObject* pTrigger */ )
{
	if( CSGD_EventSystem::GetInstance()->HasEventTriggered( szEventName ) == false )
		CSGD_EventSystem::GetInstance()->SendUniqueEvent( szEventName, this );
}

void CTriggeredObject::RegisterDefaultEvents( void )
{
	if( m_bHasBeenRegistered == false )
	{
		CSGD_EventSystem::GetInstance()->RegisterClient( "activate", this );
		CSGD_EventSystem::GetInstance()->RegisterClient( "deactivate", this );
		m_bHasBeenRegistered = true;
	}
}

void CTriggeredObject::PlayAnimation( TriggeredObjectType nTriggerType )
{
	//switch( nTriggerType )
	//{
	//case TRIGOBJ_BASE:
	//	{
	//	}
	//	break;
	//case TRIGOBJ_GATE:
	//	{
	//	}
	//	break;
	//case TRIGOBJ_SPRING:
	//	{
	//	}
	//	break;
	//case TRIGOBJ_BALLOON:
	//	{
	//	}
	//	break;
	//case TRIGOBJ_SPAWNPOINT:
	//	{
	//	}
	//	break;
	//default:
	//	{

	//	}
	//	break;
	//};
}

void CTriggeredObject::HandleEvent( CEvent* pEvent )
{
	if( pEvent->GetParam() != this )
		return;

	if( pEvent->GetEventID() == "activate" )
	{
		return;
	}
	else if( pEvent->GetEventID() == "deactivate" )
	{
		return;
	}
}