#pragma once
#include "message.h"

class CEnemy;

class CDestroyEnemyMessage :
	public CMessage
{
public:
	explicit CDestroyEnemyMessage( CEnemy* pEnemy );
	virtual ~CDestroyEnemyMessage(void);

	// Accessor
	CEnemy*	GetEnemy(void) { return m_pEnemy; }

private:
	CEnemy* m_pEnemy;
};