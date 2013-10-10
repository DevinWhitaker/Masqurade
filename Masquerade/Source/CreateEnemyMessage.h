#pragma once

#include "../SGD Wrappers/IMessage.h"
#include "Enemy.h"

class CSpawnPoint;
class CBaseObject;
class CCreateEnemyMessage : public IMessage
{
public:
	CCreateEnemyMessage( EnemyType bType, CSpawnPoint* spawn = nullptr );
	virtual ~CCreateEnemyMessage(void);

	//	Accessors
	int					&GetEnemyType( void )		{ return m_nEnemyType; }
	CSpawnPoint*		GetSpawnPoint()				{ return m_pSpawnPoint; }
	virtual MSGID		GetMessageID( void )		{ return MSG_CREATE_ENEMY; }

private:
	int					m_nEnemyType;
	CSpawnPoint*		m_pSpawnPoint;
};
