
#include "CreateEnemyMessage.h"
#include "SpawnPoint.h"
CCreateEnemyMessage::CCreateEnemyMessage( EnemyType bType, CSpawnPoint* spawn )
	: m_nEnemyType(bType)
{
	m_pSpawnPoint = spawn;
}


CCreateEnemyMessage::~CCreateEnemyMessage(void)
{

}
