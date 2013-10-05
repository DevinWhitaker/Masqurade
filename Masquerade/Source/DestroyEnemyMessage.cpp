
#include "DestroyEnemyMessage.h"

#include "Enemy.h"
#include "BaseObject.h"

CDestroyEnemyMessage::CDestroyEnemyMessage(CEnemy* pEnemy)
	: CMessage( MSG_DESTROY_ENEMY ),
	  m_pEnemy( pEnemy )
{
	if( m_pEnemy != nullptr )
		m_pEnemy->AddRef();
}


CDestroyEnemyMessage::~CDestroyEnemyMessage(void)
{
	if( m_pEnemy != nullptr )
		m_pEnemy->Release();

	m_pEnemy = nullptr;
}