#include "SpawnPoint.h"
#include "CreateEnemyMessage.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"


CSpawnPoint::CSpawnPoint(void)
{
	m_nEnemyType = NME_RAT;
	m_nSpawnRate = 5;
	m_fElapsed   = 0.0f;
}


CSpawnPoint::~CSpawnPoint(void)
{
}

void CSpawnPoint::Activate(void)
{
	if( m_fElapsed > m_nSpawnRate )
	{
		CCreateEnemyMessage* msg = new CCreateEnemyMessage( (EnemyType)m_nEnemyType, this );		
		CSGD_MessageSystem::GetInstance()->SendMsg( msg );
		m_fElapsed = 0.0f;
	}
}

void CSpawnPoint::Deactivate(void)
{
}

void CSpawnPoint::Update(float fElapsed)
{
	m_fElapsed += fElapsed;
}

void CSpawnPoint::Render()
{
}

void CSpawnPoint::SetEvent( string e )
{
	CSGD_EventSystem::GetInstance()->UnregisterClient( m_szEvent, this );
	m_szEvent = e; 
	CSGD_EventSystem::GetInstance()->RegisterClient( m_szEvent, this );
}

void CSpawnPoint::HandleEvent( CEvent* pEvent )
{
	
	if( pEvent->GetEventID() == m_szEvent )
	{
		Activate();
#if _DEVIN
		//printf("handleing event: ");
		//printf(pEvent->GetEventID().c_str());
		//printf("\n");
#endif
	}
	
}