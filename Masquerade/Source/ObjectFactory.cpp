#include "ObjectFactory.h"
#include "BaseObject.h"
#include "Balloon.h"
#include "BalloonGun.h"
#include "BaseObject.h"
#include "Checkpoint.h"
#include "Clown.h"
#include "CottonCandyLauncher.h"
#include "Crate.h"
#include "Enemy.h"
#include "Gate.h"
#include "Hoodlum.h"
#include "IBaseObject.h"
#include "ITool.h"
#include "Lever.h"
#include "Powerup.h"
#include "PressurePlate.h"
#include "Rat.h"
#include "SecurityCamera.h"
#include "SpawnPoint.h"
#include "Spring.h"
#include "SquirtGun.h"
#include "Tile.h"

///////////////////////////////////////////////////////////
//	Purpose:	Allows access to this singleton
//
//	In:			None
//
//	Return:		An instance of this object
///////////////////////////////////////////////////////////
CObjectFactory*	CObjectFactory::GetInstance()
{
	static CObjectFactory s_Instance;

	return &s_Instance;
}

///////////////////////////////////////////////////////////
//	Purpose:	Setup an object to be copied from
//
//	In:			
//				MasterCopy: A dynamically allocated instance of the class
//				Tag: The enumeration indicating what type of class to register
//
//	Return:		True if the class is successfully registered
///////////////////////////////////////////////////////////
bool CObjectFactory::RegisterClass( CBaseObject& MasterCopy, CREATEABLE Tag )
{
	if( m_pCloneFrom[Tag] != nullptr )
		m_pCloneFrom[Tag]->Release();
	
	m_pCloneFrom[Tag] = &MasterCopy;

	m_pCloneFrom[Tag]->AddRef();

	return true;
}

///////////////////////////////////////////////////////////
//	Purpose:	Releases the memory of the selected class
//
//	In:			The enumeration indicating which class to unregister
//
//	Return:		True if the class is successfully unregistered
///////////////////////////////////////////////////////////
bool CObjectFactory::UnregisterClass( CREATEABLE Tag )
{
	if( m_pCloneFrom[Tag] != nullptr )
		m_pCloneFrom[Tag]->Release();

	m_pCloneFrom[Tag] = nullptr;

	return true;
}

///////////////////////////////////////////////////////////
//	Purpose:	Dynamically instantiate an object of the intended class
//
//	In:			The enumeration indicating the object to create
//
//	Return:		A pointer to the newly created object
///////////////////////////////////////////////////////////
CBaseObject* CObjectFactory::CreateObject( CREATEABLE Tag )
{
	CBaseObject* created = nullptr;

	switch(Tag)
	{
	case BALLOON: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CBalloon* convert = dynamic_cast<CBalloon*>(m_pCloneFrom[Tag]);
			created = new CBalloon( *convert );
		}
		break;
	case BALLOON_GUN: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CBalloonGun* convert = dynamic_cast<CBalloonGun*>(m_pCloneFrom[Tag]);
			created = new CBalloonGun( *convert );
		}
		break;
	case CHECKPOINT: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CCheckpoint* convert = dynamic_cast<CCheckpoint*>(m_pCloneFrom[Tag]);
			created = new CCheckpoint( *convert );
		}
		break;
	case CLOWN: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CClown* convert = dynamic_cast<CClown*>(m_pCloneFrom[Tag]);
			created = new CClown( *convert );
		}
		break;
	case COTTON_LAUNCHER: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CCottonCandyLauncher* convert = dynamic_cast<CCottonCandyLauncher*>(m_pCloneFrom[Tag]);
			created = new CCottonCandyLauncher( *convert );
		}
		break;
	case CRATE: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CCrate* convert = dynamic_cast<CCrate*>(m_pCloneFrom[Tag]);
			created = new CCrate( *convert );
		}
		break;
	case GATE: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CGate* convert = dynamic_cast<CGate*>(m_pCloneFrom[Tag]);
			created = new CGate( *convert );
		}
		break;
	case HOODLUM: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CHoodlum* convert = dynamic_cast<CHoodlum*>(m_pCloneFrom[Tag]);
			created = new CHoodlum( *convert );
		}
		break;
	case LEVER: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CLever* convert = dynamic_cast<CLever*>(m_pCloneFrom[Tag]);
			created = new CLever( *convert );
		}
		break;
	case POWERUP: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CPowerup* convert = dynamic_cast<CPowerup*>(m_pCloneFrom[Tag]);
			created = new CPowerup( *convert );
		}
		break;
	case PRESSURE_PLATE: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CPressurePlate* convert = dynamic_cast<CPressurePlate*>(m_pCloneFrom[Tag]);
			created = new CPressurePlate( *convert );
		}
		break;
	case RAT: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CRat* convert = dynamic_cast<CRat*>(m_pCloneFrom[Tag]);
			created = new CRat( *convert );
		}
		break;
	case SECURITY_CAMERA: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CSecurityCamera* convert = dynamic_cast<CSecurityCamera*>(m_pCloneFrom[Tag]);
			created = new CSecurityCamera( *convert );
		}
		break;
	case SPAWN_POINT: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CSpawnPoint* convert = dynamic_cast<CSpawnPoint*>(m_pCloneFrom[Tag]);
			created = new CSpawnPoint( *convert );
		}
		break;
	case SPRING: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CSpring* convert = dynamic_cast<CSpring*>(m_pCloneFrom[Tag]);
			created = new CSpring( *convert );
		}
		break;
	case SQUIRT_GUN: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CSquirtGun* convert = dynamic_cast<CSquirtGun*>(m_pCloneFrom[Tag]);
			created = new CSquirtGun( *convert );
		}
		break;
	case TILE: 
		if( m_pCloneFrom[Tag] != nullptr )
		{
			CTile* convert = dynamic_cast<CTile*>(m_pCloneFrom[Tag]);
			created = new CTile( *convert );
		}
		break;
	}

	return created;
}

///////////////////////////////////////////////////////////
//	Purpose:	Release all the memory within the class 
//
//	In:			None
//
//	Return:		None
///////////////////////////////////////////////////////////
void CObjectFactory::ShutdownObjectFactory()
{
	for( int i = 0; i < MAX_CREATEABLES; i++ )
	{
		if( m_pCloneFrom[i] != nullptr )
			m_pCloneFrom[i]->Release();

		m_pCloneFrom[i] = nullptr;
	}
}

///////////////////////////////////////////////////////////
//	Purpose:	private constructor
//
//	In:			None
//
//	Return:		None
///////////////////////////////////////////////////////////
CObjectFactory::CObjectFactory(void)
{
	for( int i = 0; i < MAX_CREATEABLES; i++ )
		m_pCloneFrom[i] = nullptr;
}

///////////////////////////////////////////////////////////
//	Purpose:	private destructor
//
//	In:			None
//
//	Return:		None
///////////////////////////////////////////////////////////
CObjectFactory::~CObjectFactory(void)
{
}
