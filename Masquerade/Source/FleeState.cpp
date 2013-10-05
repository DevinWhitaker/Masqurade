#include "FleeState.h"
#include "ObjectManager.h"

CFleeState::CFleeState(void)
{
	m_nAIType = FLEE;
	GetObjectsInvolved().pCarried = nullptr;
	GetObjectsInvolved().pCarrier = nullptr;
	SetScalarX( 1.0f );
	SetScalarY( 1.0f );
}


CFleeState::~CFleeState(void)
{
	//Exit();
}

void CFleeState::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	GetObjectsInvolved().pCarried = (CBaseObject*)pB;
	GetObjectsInvolved().pCarrier = pHeldBy;

	if( GetObjectsInvolved().pCarried->GetCurrentState() == nullptr &&
		GetObjectsInvolved().pCarried->GetType() != OBJ_BASE )
		GetObjectsInvolved().pCarried->SetCurrentState( this );
	m_nAIType = FLEE;

	for( unsigned i = 0; i < m_vFleeFromObjTypes.size(); ++i )
	{
		std::vector< IBaseObject* > vCatchData;
		vCatchData = CObjectManager::GetInstance()->FindObjectsWithType(  m_vFleeFromObjTypes[i].nBaseObject, m_vFleeFromObjTypes[i].nTypeObject );
		for( unsigned j = 0; j < vCatchData.size(); ++j )
			 m_vFleeCheck.push_back( vCatchData[j] );

	}
}

void CFleeState::Exit(void)
{
	CBaseAI::Exit();
}

void CFleeState::Update( float fElapsedTime )
{
	for( unsigned i = 0; i < m_vFleeCheck.size(); ++i )
	{
		CBaseObject* pDistance = dynamic_cast< CBaseObject* >( m_vFleeCheck[i] );
		if( GetObjectsInvolved().pCarried->GetX() > pDistance->GetX() &&
			GetObjectsInvolved().pCarried->GetY() <= pDistance->GetY()  + 64 )
		{
			GetObjectsInvolved().pCarried->SetMovingRightBool( true );
			GetObjectsInvolved().pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
		}
		else if( GetObjectsInvolved().pCarried->GetX() < pDistance->GetX()  )
		{
			GetObjectsInvolved().pCarried->SetMovingRightBool( false );
			GetObjectsInvolved().pCarried->IncreaseVelocity( 5.0f * GetScalarX(), true );
		}
	}
	CBaseAI::Update( fElapsedTime );
}

void CFleeState::Render(void)
{
	CBaseAI::Render();
}

void CFleeState::NodeCollision( void )
{
	CBaseAI::NodeCollision();
}

void CFleeState::AddFleeType( int nBaseType, int nType )
{
	CBaseAI::sObjectStruct  sData = { nBaseType, nType };
	m_vFleeFromObjTypes.push_back( sData );
}

void CFleeState::HandleEvent( CEvent* pEvent )
{
	CBaseAI::HandleEvent( pEvent );
}