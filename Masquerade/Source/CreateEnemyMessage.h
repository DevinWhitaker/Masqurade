#pragma once

#include "Message.h"
#include "Enemy.h"

class CSpawnPoint;
class CBaseObject;
class CCreateEnemyMessage : public CMessage
{
public:
	CCreateEnemyMessage( EnemyType bType, CSpawnPoint* spawn = nullptr );
	virtual ~CCreateEnemyMessage(void);

	//	Accessors
	int					&GetEnemyType( void )		{ return m_nEnemyType; }
	CSpawnPoint*		GetSpawnPoint()				{ return m_pSpawnPoint; }

private:
	int					m_nEnemyType;
	CSpawnPoint*		m_pSpawnPoint;
};
