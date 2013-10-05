#include "SeekState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
//#include "BaseObject.h"
#include <algorithm>
#include "Enemy.h"
#include "Bullet.h"
#include "Trigger.h"
#include "Lever.h"
#include "IdleState.h"
#include "Corinne.h"

CSeekState::CSeekState(void)
{
	m_nAIType = SEEK;
	GetObjectsInvolved().pCarried = nullptr;
	GetObjectsInvolved().pCarrier = nullptr;
	SetXDistance( 32.0f );
	SetYDistance( 64.0f );
	SetScalarX( 1.0f );
	SetScalarY( 1.0f );
	SetVectorUpdateBool( false );
}


CSeekState::~CSeekState(void)
{
	//Exit();
}

void CSeekState::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	GetObjectsInvolved().pCarried =		(CBaseObject*)pB;
	GetObjectsInvolved().pCarrier =		pHeldBy;

	if( GetObjectsInvolved().pCarried->GetCurrentState() == nullptr &&
		GetObjectsInvolved().pCarried->GetType() != OBJ_BASE )
		GetObjectsInvolved().pCarried->SetCurrentState( this );

	m_nAIType = SEEK;

	for( unsigned i = 0; i < m_vObjectsToSeek.size(); ++i )
	{
		std::vector< IBaseObject* > vCatchData;

		vCatchData = CObjectManager::GetInstance()->FindObjectsWithType( m_vObjectsToSeek[i].nBaseObject, m_vObjectsToSeek[i].nTypeObject ) ;
		for( unsigned j = 0; j < vCatchData.size(); ++j )
			m_vObjectsToCheck.push_back( vCatchData[j] );

	}

	//for( unsigned i = 0; i < m_vFleeFromObjTypes.size(); ++i )
	//{
	//	std::vector< IBaseObject* > vCatchData;
	//	vCatchData = CObjectManager::GetInstance()->FindObjectsWithType(  m_vFleeFromObjTypes[i], true ) ;
	//	for( unsigned j = 0; j < vCatchData.size(); ++j )
	//		 m_vFleeCheck.push_back( vCatchData[j] );

	//}
}
void CSeekState::Exit(void)
{
	//for( unsigned m = 0; m < m_vObjectsToCheck.size(); ++m )
	//{
	//	delete m_vObjectsToCheck[m];
	//	//m_vObjectsToCheck[m] = nullptr;
	//}

	/***************************************/
	//			NEEDS TO BE FIXED
	/***************************************/

	//while( !m_vObjectsToCheck.empty() )
	//{
	//	delete m_vObjectsToCheck.back();
	//	m_vObjectsToCheck.pop_back();
	//}
	m_vObjectsToCheck.clear();

	//CBaseAI::Exit();
}

void CSeekState::AddSeekType( int nBaseType, int nType )
{
	sObjectStruct sData = { nBaseType, nType };
	m_vObjectsToSeek.push_back( sData );
}


void CSeekState::Update( float fElapsedTime )
{
	if( CSGD_EventSystem::GetInstance()->HasEventTriggered( "idle" ) == true &&
		GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
		return;
	if( m_vObjectsToCheck.size() == 0 )
	{
#if _DEBUG
			printf( "Switching State to Idle because the vector was empty. \n" );
#endif
		CSGD_EventSystem::GetInstance()->SendUniqueEvent( "follow", &GetObjectsInvolved() );
		return;
	}

	if( IsObjectInBeingSeeked( OBJ_BULLET, PIE_BULLET ) == false &&
		IsObjectInBeingSeeked( OBJ_TRIGGER, CTrigger::TRIG_LEVER ) == false &&
		GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
	{
		CSGD_EventSystem::GetInstance()->SendUniqueEvent("idle", &GetObjectsInvolved() );
		return;
	}

	if( GetVectorUpdateBool() == true )
	{
		//for( unsigned m = 0; m < m_vObjectsToCheck.size(); ++m )
		//{
		//	delete m_vObjectsToCheck[m];
		//	m_vObjectsToCheck[m] = nullptr;
		//}

		m_vObjectsToCheck.clear();
		for( unsigned i = 0; i < m_vObjectsToSeek.size(); ++i )
		{
			std::vector< IBaseObject* > vCatchData;
			vCatchData = CObjectManager::GetInstance()->FindObjectsWithType( m_vObjectsToSeek[i].nBaseObject, m_vObjectsToSeek[i].nTypeObject ) ;
			for( unsigned j = 0; j < vCatchData.size(); ++j )
				m_vObjectsToCheck.push_back( vCatchData[j] );
		}

		SetVectorUpdateBool( false );
	}

	CObjectManager::sClosestObject sClosetObject = ReturnClosetObject();

	if( sClosetObject.nPosInVec == -1 )
	{
			CSGD_EventSystem::GetInstance()->SendUniqueEvent("idle", &GetObjectsInvolved() );
			return;
	}
//#if _DEBUG
//		printf( "Corinne Y: %f, Distance: %f",  GetObjectsInvolved().pCarried->GetY(), sClosetObject.fYRange + GetYDistance() );  
//#endif
	if( GetObjectsInvolved().pCarried->GetX()  >= sClosetObject.fXRange - 32 &&
		GetObjectsInvolved().pCarried->GetX()  <= sClosetObject.fXRange + 32 &&
		GetObjectsInvolved().pCarried->GetY()  >= sClosetObject.fYRange - GetYDistance() &&
		GetObjectsInvolved().pCarried->GetY()  <= sClosetObject.fYRange  + GetYDistance() )
	{
		if( m_vObjectsToCheck[ sClosetObject.nPosInVec ]->GetType() == OBJ_TRIGGER &&
			GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
		{
#if _DEBUG
			printf( "Switching State to Idle because Corinne found a lever! \n" );
#endif
			CLever*	pLever = dynamic_cast< CLever* >(  m_vObjectsToCheck[ sClosetObject.nPosInVec ] );
			pLever->Use();
			CSGD_EventSystem::GetInstance()->SendUniqueEvent("idle", &GetObjectsInvolved() );
			return;
		}
		else
		{
		//	Handle Events
		}
	}
	else if( GetObjectsInvolved().pCarried->GetX()  > sClosetObject.fXRange )
	{
		GetObjectsInvolved().pCarried->SetMovingRightBool( false );
		GetObjectsInvolved().pCarried->IncreaseVelocity( 2.0f * GetScalarX(), true );
	}
	else if( GetObjectsInvolved().pCarried->GetX()  < sClosetObject.fXRange )
	{
		GetObjectsInvolved().pCarried->SetMovingRightBool( true );
		GetObjectsInvolved().pCarried->IncreaseVelocity( 2.0f * GetScalarX(), true );
	}


	CBaseAI::Update( fElapsedTime );
}

void CSeekState::Render(void)
{
	CBaseAI::Render();
}


void CSeekState::NodeCollision( void )
{
	CBaseAI::NodeCollision();
}

bool CSeekState::IsObjectInBeingSeeked( int nBaseObject, int nObjToCheckFor )
{
	for( unsigned i = 0; i < m_vObjectsToCheck.size(); ++i )
	{
		if( m_vObjectsToCheck[i]->GetType() == OBJ_ENEMY )
		{
			CEnemy*	pEnemy = dynamic_cast< CEnemy* >( m_vObjectsToCheck[i] );
			if( pEnemy->getEnemyType() == nObjToCheckFor &&
				pEnemy->GetType() == nBaseObject)
				return true;
		}
		else if( m_vObjectsToCheck[i]->GetType() == OBJ_BULLET )
		{
			CBullet* pBullet = dynamic_cast< CBullet* >( m_vObjectsToCheck[i] );
			if( pBullet->getBulletType() == nObjToCheckFor &&
				pBullet->GetType() == nBaseObject)
				return true;
		}
		else if( m_vObjectsToCheck[i]->GetType() == OBJ_TRIGGER )
		{
			CTrigger* pTrigger = dynamic_cast< CTrigger* >( m_vObjectsToCheck[i] );
			if( pTrigger->GetTriggerType() == nObjToCheckFor &&
				pTrigger->GetType() == nBaseObject)
				return true;
		}
		else
		{
			if( m_vObjectsToCheck[i]->GetType() == nObjToCheckFor &&
				nObjToCheckFor == -1 )
				return true;
		}
	}

	return false;
}

void CSeekState::HandleEvent( CEvent* pEvent )
{
	CBaseAI::HandleEvent( pEvent );
}

CObjectManager::sClosestObject CSeekState::ReturnClosetObject( void )
{
	std::vector< CObjectManager::sClosestObject > vDistanceSorting;
	std::vector< float > vFloatSort;

	for( unsigned  i = 0; i < m_vObjectsToCheck.size(); ++i )
	{
		CBaseObject*	pBaseObject = dynamic_cast< CBaseObject* >( m_vObjectsToCheck[i] );
		CObjectManager::sClosestObject sData = {};

		if(		pBaseObject->GetX()  >= GetObjectsInvolved().pCarried->GetX() - GetXDistance() &&
			pBaseObject->GetX()  <= GetObjectsInvolved().pCarried->GetX() + GetXDistance() &&
			pBaseObject->GetY()  >= GetObjectsInvolved().pCarried->GetY() - GetYDistance() &&
			pBaseObject->GetY()  <= GetObjectsInvolved().pCarried->GetY()  + GetYDistance() )
		{
			float fxDistance = pow( GetXDistance(  ) - pBaseObject->GetX(), 2.0f );
			float fyDistance = pow( GetYDistance(  ) - pBaseObject->GetY(), 2.0f );
			float fTotal = fxDistance + fyDistance;
			float fDistanceFormula = sqrt( fTotal );
			sData.fDistance = fDistanceFormula;
			sData.fXRange = pBaseObject->GetX();
			sData.fYRange = pBaseObject->GetY();
			sData.nPosInVec = i;
			vFloatSort.push_back( sData.fDistance );
			vDistanceSorting.push_back( sData );
		}
	}

	sort( vFloatSort.begin(), vFloatSort.end() );

	if( vDistanceSorting.size() != 0 )
	{
		for( unsigned i = 0; i < vDistanceSorting.size(); ++i )
		{
			if( vFloatSort[0] == vDistanceSorting[i].fDistance )
				return vDistanceSorting[i];
		}
	}

	CObjectManager::sClosestObject sEmptyData = {  };
	sEmptyData.nPosInVec = -1;
	return sEmptyData;
}