#pragma once
#include "../SGD Wrappers/IMessage.h"

class CEnemy;

class CDestroyEnemyMessage :
	public IMessage
{
public:
	explicit CDestroyEnemyMessage( CEnemy* pEnemy );
	virtual ~CDestroyEnemyMessage(void);

	// Accessor
	CEnemy*				GetEnemy(void)				{ return m_pEnemy; }
	virtual MSGID		GetMessageID( void )		{ return MSG_DESTROY_ENEMY; }

private:
	CEnemy* m_pEnemy;
};