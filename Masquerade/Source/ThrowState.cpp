#include "ThrowState.h"
#include "BaseObject.h"
#include "Corinne.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"


CThrowState::CThrowState(void)
{
	GetObjectsInvolved().pCarried	= nullptr;
	GetObjectsInvolved().pCarrier	= nullptr;
	m_nAIType = THROW;
}


CThrowState::~CThrowState(void)
{
	//	Exit(); ???
	//if( GetAttachedObject()->GetType() == OBJECT_TYPES::OBJ_CORINNE )
	//{
	//	CCorinne*	pCO	= dynamic_cast< CCorinne* >( GetAttachedObject() );
	//	CSGD_EventSystem::GetInstance()->UnregisterClient( "idle", pCO );
	//}
}

void CThrowState::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	GetObjectsInvolved().pCarried	= (CBaseObject*)pB;
	GetObjectsInvolved().pCarrier	=	pHeldBy;
	m_nAIType	= THROW;

	//if( GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
	//{
	//	CCorinne*	pCO	= dynamic_cast< CCorinne* >( GetObjectsInvolved().pCarried );
	//	CSGD_EventSystem::GetInstance()->RegisterClient( "idle", pCO );
	//}
}

void CThrowState::Exit(void)		
{
	CBaseAI::Exit();
}

void CThrowState::Update( float fElapsedTime )
{
	if( GetObjectsInvolved().pCarried != nullptr )
		GetObjectsInvolved().pCarried->SlowDown( fElapsedTime );
	CBaseAI::Update( fElapsedTime );
	//CBaseObject*	pBase = dynamic_cast< CBaseObject* >( GetAttachedObject() );
	//if( pBase->GetType() == OBJECT_TYPES::OBJ_CORINNE )
	//{
	//	CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pBase );
	//}
}

void CThrowState::Render(void)
{

}

void CThrowState::NodeCollision( void )
{
	CBaseAI::NodeCollision();
}

void CThrowState::HandleEvent( const CEvent* pEvent )
{
	CBaseAI::HandleEvent( pEvent );
}