#include "Enemy.h"


CEnemy::CEnemy(void)
{
	CBaseObject::m_nType = OBJ_ENEMY;
	m_bIsAlive = true;
	CSGD_EventSystem::GetInstance()->RegisterClient( "Enemy_Hit", this );
}


CEnemy::~CEnemy(void)
{
	CSGD_EventSystem::GetInstance()->UnregisterClient( "Enemy_Hit", this );
}

void CEnemy::HandleEvent(CEvent* pEvent)
{
	if(m_bIsAlive)
	{
		if(pEvent->GetEventID() == "Enemy_Hit")
		{
			if(pEvent->GetParam() == this)
				m_nHealth -= 10;
		}
	}
}

void CEnemy::Update(float fElapsed)
{
	if(m_bIsAlive)
	{
		CBaseObject::Update(fElapsed);
	}
}


bool CEnemy::CheckCollision(IBaseObject* pBase)
{
	if(m_bIsAlive)
	{
		if( CBaseObject::CheckCollision( pBase ) == true)
		{
			if( pBase->GetType() == OBJ_FOX )
			{
				CSGD_EventSystem::GetInstance()->QueueEvent( "Fox_Hit", this);
			}
			if( pBase->GetType() == OBJ_CORINNE )
				CSGD_EventSystem::GetInstance()->QueueEvent( "Corinne_Hit", pBase);
			if( pBase->GetType() == OBJ_TILE )
				int x = 0;
		}
	}
	return false;
}

void CEnemy::CollisionResponse(CBaseObject* pBase, RECT hitBox, RECT hit, POINT anchor)
{
	/*if(m_bIsAlive)
	{
		if( pBase->GetType() == OBJ_FOX )
		{
			CSGD_EventSystem::GetInstance()->QueueEvent( "Fox_Hit", this);
		}
	}*/
}