/*
	File:		Trigger.cpp
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	To trigger objects in the game world.
*/

#include "Trigger.h"
#include "TriggeredObject.h"
#include "Frame.h"
#include "AnimationManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "FileManager.h"
#include <sstream>
//#include "../SGD Wrappers/CSGD_EventSystem.h"

CTrigger::CTrigger(void)
{
	//SetWidth( 1 );
	//SetHeight( 1 );
	m_pTriggerFrame = nullptr;
	m_nTriggerType = TRIG_BASE;
	this->m_nType = OBJ_TRIGGER;
	SetInRangeBool( false );
	SetIsActivatedBool( false );
}


CTrigger::~CTrigger(void)
{

	if( GetObjectMap().size() > 0 )
	{
		//for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
		//	iter != GetObjectMap().end(); ++iter )
		//{
		//	if( (*iter).second != nullptr )
		//	{
		//		GetObjectMap().erase( iter );
		//		//delete &(*iter);
		//		//iter = GetObjectMap().begin();
		//	}
		//}

		GetObjectMap().clear();
	}

	if( m_pTriggerFrame != nullptr )
	{
		CAnimationManager::GetInstance()->RemoveFrame( m_pTriggerFrame );
		delete m_pTriggerFrame;
		m_pTriggerFrame = nullptr;
	}
}

void CTrigger::Use( void )
{
	CSGD_XAudio2*	pXA = CSGD_XAudio2::GetInstance();

	for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
		iter != GetObjectMap().end(); ++iter )
	{
		if( (*iter).second->GetTriggeredType() != CTriggeredObject::TRIGOBJ_BASE )
		{
			if( (*iter).second->GetActivatedBool() == false && 
				GetInRangeBool() == true /*&&
				CSGD_EventSystem::GetInstance()->HasEventTriggered( "activate") == false */ )
			{
				CSGD_EventSystem::GetInstance()->SendEvent( "activate", (*iter).second );
				(*iter).second->SetActivatedBool( true );
				if( pXA->SFXIsSoundPlaying( CFileManager::GetInstance()->GetSoundID("MASQ_Gate") ) == false )
					pXA->SFXPlaySound( CFileManager::GetInstance()->GetSoundID("MASQ_Gate") );
				//bHasAnythingBeenChanged = true;
			}
			else if( (*iter).second->GetActivatedBool() == true && 
				GetInRangeBool() == true /*&&
				CSGD_EventSystem::GetInstance()->HasEventTriggered(  "deactivate" ) == false  */)
			{
				CSGD_EventSystem::GetInstance()->SendEvent( "deactivate", (*iter).second );
				(*iter).second->SetActivatedBool( false );
				if( pXA->SFXIsSoundPlaying( CFileManager::GetInstance()->GetSoundID("MASQ_Gate") ) == false )
					pXA->SFXPlaySound( CFileManager::GetInstance()->GetSoundID("MASQ_Gate") );
				//bHasAnythingBeenChanged = true;
			}
		}
	}

	if( GetIsActivatedBool() == false )
		SetIsActivatedBool( true );
	else
		SetIsActivatedBool( false );
}


void CTrigger::AddObjectToTrigger( CTriggeredObject* pTriggeredObject )
{
	if(  pTriggeredObject == nullptr )
		return;

	unsigned int nCounter = 0;
	switch( pTriggeredObject->GetTriggeredType() )
	{
	case CTriggeredObject::TRIGOBJ_BASE:
		return;
		break;
	case CTriggeredObject::TRIGOBJ_BALLOON:
		{
			for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
				iter != GetObjectMap().end(); ++iter )
			{
				if( (*iter).second->GetTriggeredType() == CTriggeredObject::TRIGOBJ_BALLOON )
					nCounter++;
			}
			std::string szNameToUse = "balloon_" + nCounter;
			GetObjectMap()[ szNameToUse ] = pTriggeredObject;
		}
		break;
	case CTriggeredObject::TRIGOBJ_GATE:
		{
			for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
				iter != GetObjectMap().end(); ++iter )
			{
				if( (*iter).second->GetTriggeredType() == CTriggeredObject::TRIGOBJ_GATE )
					nCounter++;
			}
			std::ostringstream oss;
			oss << "gate_" << nCounter;
			GetObjectMap()[ oss.str().c_str() ] = pTriggeredObject;
		}
		break;
	case CTriggeredObject::TRIGOBJ_SPAWNPOINT:
		{
			for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
				iter != GetObjectMap().end(); ++iter )
			{
				if( (*iter).second->GetTriggeredType() == CTriggeredObject::TRIGOBJ_SPAWNPOINT )
					nCounter++;
			}
			std::string szNameToUse = "spawnpoint_" + nCounter;
			GetObjectMap()[ szNameToUse ] = pTriggeredObject;
		}
		break;
	case CTriggeredObject::TRIGOBJ_SPRING:
		{
			for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
				iter != GetObjectMap().end(); ++iter )
			{
				if( (*iter).second->GetTriggeredType() == CTriggeredObject::TRIGOBJ_SPRING )
					nCounter++;
			}
			std::string szNameToUse = "spring_" + nCounter;
			GetObjectMap()[ szNameToUse ] = pTriggeredObject;
		}
		break;
	default:
		break;
	};
}


void CTrigger::AddObjectToTrigger( CTriggeredObject* pTriggeredObject, std::string szNameOfTriggeredObject )
{
	if( pTriggeredObject != nullptr )
		GetObjectMap()[ szNameOfTriggeredObject ] = pTriggeredObject;
}

void CTrigger::RemoveObjectToTrigger( std::string szTriggeredObjectName )
{
	for( std::map< std::string, CTriggeredObject* >::iterator iter = GetObjectMap().begin();
		iter != GetObjectMap().end(); ++iter )
	{
		if( (*iter).first.compare( szTriggeredObjectName ) == false )
		{
			GetObjectMap().erase( iter );
			return;
		}
	}
}

void CTrigger::SetTriggerFrame( CFrame*	pFrame, std::string szFilePath, unsigned int unAnimationToUse )
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
		m_pTriggerFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame);
	}
	else
	{
		CAnimationManager::GetInstance()->RemoveFrame( m_pTriggerFrame );
		m_pTriggerFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame );
	}
}

void CTrigger::SetTriggerFrame( const char* szFilePath, unsigned int unAnimationToUse  )
{
	CFrame*	pFrame = new CFrame();
	CAnimationManager*	pAM = CAnimationManager::GetInstance();

	//"Resources/Scripts/animation_anchors.xml"
	vector< std::string > vTemp = pAM->CreateAnimationFromFile
		( szFilePath );

	pFrame->Init( this );
	pFrame->SetAnimationID( vTemp[ unAnimationToUse ] );

	//m_pTriggerFrame = pFrame;

	if( m_pTriggerFrame == nullptr )
	{
		m_pTriggerFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame);
	}
	else
	{
		//CAnimationManager::GetInstance()->RemoveFrame( m_pTriggerFrame );
		m_pTriggerFrame->SetAnimationID( pFrame->GetAnimationID() );
		//if( GetIsActivatedBool() == true )
			//m_pTriggerFrame->Play
		//CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame );
	}
}



void	CTrigger::SetTriggerFrame( std::string szFilePath, 
		DWORD dwColor, int nCurrentFrame, bool bFacingRight, 
		bool bIsItPlaying, float fAnimationSpeed, 
		float fXScale, float fYScale, 
		unsigned int unAnimationToUse )
{

	CFrame*	pFrame = new CFrame();
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
		pFrame->SetColor( dwColor );
		pFrame->SetCurrFrame( nCurrentFrame );
		pFrame->SetFlipped( bFacingRight );
		pFrame->SetPlaying( bIsItPlaying );
		pFrame->SetSpeed( fAnimationSpeed );
		pFrame->SetXScale( fXScale );
		pFrame->SetYScale( fYScale );
	}
	else
		return;

	if( pFrame == nullptr )
	{
		m_pTriggerFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame);
	}
	else
	{
		CAnimationManager::GetInstance()->RemoveFrame( m_pTriggerFrame );
		m_pTriggerFrame = pFrame;
		CAnimationManager::GetInstance()->AddFrame( *m_pTriggerFrame );
	}
}

void CTrigger::HandleEvent( CEvent* pEvent )
{
	CBaseObject::HandleEvent( pEvent );
}

bool CTrigger::CheckCollision(IBaseObject* pBase)
{
	return CBaseObject::CheckCollision( pBase );
}