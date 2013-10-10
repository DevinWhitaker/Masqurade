/*
	File:		Lever.cpp
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Allow the player to activate or deactivate certain objects
				in the game world.
*/

#include "Lever.h"
#include "Frame.h"
#include "AnimationManager.h"

CLever::CLever(void)
{
	CSGD_EventSystem::GetInstance()->RegisterClient( "pull_lever_left", this );
	CSGD_EventSystem::GetInstance()->RegisterClient( "pull_lever_right", this );
	//SetTriggerFrame( nullptr, "" );
	SetInRangeBool( false );
	SetIsActivatedBool( false );
	m_nType = OBJ_TRIGGER;
	m_nTriggerType = TRIG_LEVER;
}


CLever::~CLever(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClient( "pull_lever_left", this );
	CSGD_EventSystem::GetInstance()->UnregisterClient( "pull_lever_right", this );
	//if( GetLeverAnimation() != nullptr )
	//{
	//	delete m_pLeverAnimation;
	//	m_pLeverAnimation = nullptr;
	//}
}

CLever::CLever( CLever &obj )
{
	SetWidth( obj.GetWidth() );
	SetHeight( obj.GetHeight() );
	SetX( obj.GetX() );
	SetY( obj.GetY() );
	SetVelX( obj.GetVelX() );
	SetVelY( obj.GetVelY() );
	SetMaxXVelocity( obj.GetMaxXVelocity());
	SetMaxYVelocity( obj.GetMaxYVelocity() );
	SetStopVelocityBool( obj.GetStopVelocityBool() );
	SetGravityBool( obj.GetGravityBool() );
	SetGravityFloat( obj.GetGravityFloat() );
	SetColor( obj.GetColor() );
	SetLayerType( obj.GetLayerType() );
	this->m_nType = obj.m_nType;
	this->m_nTriggerType = obj.m_nTriggerType;
	SetTriggerFrame( obj.GetTriggerFrame() );
	SetCurrentState( obj.GetCurrentState() );
	SetIsActivatedBool( obj.GetIsActivatedBool() );
	SetMap( obj.GetObjectMap() );
	SetInRangeBool( obj.GetInRangeBool());
	//GetTriggerFrame()->Init( this );
}


//void CLever::SetTriggerFrame( const char* szFileName, unsigned int nAnimationToUse)
//{
//
//	CFrame*	pFrame = new CFrame();
//	CAnimationManager*	pAM = CAnimationManager::GetInstance();
//
//	//"Resources/Scripts/animation_anchors.xml"
//	vector< std::string > vTemp = pAM->CreateAnimationFromFile
//		( szFileName );
//
//	pFrame->Init( this );
//	pFrame->SetAnimationID( vTemp[ nAnimationToUse ] );
//
//	this->TransferFrame( pFrame );
//}

void CLever::Use( /* CLever*	pLever*/  )
{
	//if( GetIsActivatedBool() == true /*&&
	//	CSGD_EventSystem::GetInstance()->HasEventTriggered( "pull_lever_right") == false*/ )
	//	CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "pull_lever_left", this );
	//else if( GetIsActivatedBool() == false /*&&
	//	CSGD_EventSystem::GetInstance()->HasEventTriggered( "pull_lever_left") == false*/ )
	//	CSGD_EventSystem::GetInstance()->QueueUniqueEvent( "pull_lever_right", this );

	if( GetIsActivatedBool() == true )
	{
		CTrigger::Use();
		SetTriggerFrame( "Resources/Scripts/lever.xml", 0 );
		GetTriggerFrame()->SetPlaying( true );
		GetTriggerFrame()->Play( true);
		return;
	}
	else
	{
		CTrigger::Use();
		SetTriggerFrame( "Resources/Scripts/lever.xml", 1 );
		GetTriggerFrame()->SetPlaying( true );
		GetTriggerFrame()->Play( true);
		return;
	}
}

bool CLever::CheckCollision(IBaseObject* pBase)
{
	RECT rIntersect = {};

	if( IntersectRect( &rIntersect, &pBase->GetRect(), &GetTriggerFrame()->GetRect() ) == TRUE )
	{
		CBaseObject*	pBaseObject = dynamic_cast< CBaseObject* >( pBase );

		if( pBaseObject->GetType() == OBJ_FOX || 
			pBaseObject->GetType() == OBJ_CORINNE )
			SetInRangeBool( true );
		else
			SetInRangeBool( false );
		return true;
	}
	return false;
}

void CLever::HandleEvent( CEvent* pEvent )
{
	CLever*	pLever = dynamic_cast< CLever* >( (CLever*)pEvent->GetParam() );
	//if( pLever->GetTriggerType() != this->GetTriggerType() )
	//	return;

	if( pEvent->GetParam() != this )
		return;

	if( pEvent->GetEventID( ) == "pull_lever_left" )
	{
		if( GetTriggerFrame() != nullptr )
		{
			CTrigger::Use();
			SetTriggerFrame( "Resources/Scripts/lever.xml", 0 );
			GetTriggerFrame()->SetPlaying( true );
			GetTriggerFrame()->Play( true);
		}
		return;
	}

	if( pEvent->GetEventID( ) == "pull_lever_right" )
	{
		if( GetTriggerFrame() != nullptr )
		{
			CTrigger::Use();
			SetTriggerFrame( "Resources/Scripts/lever.xml", 1 );
			GetTriggerFrame()->SetPlaying( true );
			GetTriggerFrame()->Play( true );
		}
		return;
	}
}

//void	CLever::SetLeverAnimation( std::string szFilePath, 
//	unsigned int unAnimationToUse )
//{
//	CFrame*	pFrame  = new CFrame();
//	if( GetWidth() != 1 || GetHeight() != 1 )
//	{
//		SetWidth( 1 );
//		SetHeight( 1 );
//	}
//
//	if( szFilePath.size() != 0)
//	{
//		std::vector< std::string > szAnimations;
//		szAnimations = CAnimationManager::GetInstance()->CreateAnimationFromFile( szFilePath.c_str() );
//		pFrame->Init( this );
//		pFrame->SetAnimationID( szAnimations[unAnimationToUse] );
//	}
//	else
//		return;
//
//	if( pFrame == nullptr )
//	{
//		m_pLeverAnimation = pFrame;
//		CAnimationManager::GetInstance()->AddFrame( *m_pLeverAnimation );
//	}
//	else
//	{
//		CAnimationManager::GetInstance()->RemoveFrame( m_pLeverAnimation );
//		m_pLeverAnimation = pFrame;
//		CAnimationManager::GetInstance()->AddFrame( *m_pLeverAnimation );
//	}
//}