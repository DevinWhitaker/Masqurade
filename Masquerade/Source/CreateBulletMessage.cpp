
#include "CreateBulletMessage.h"
#include "BaseObject.h"

CCreateBulletMessage::CCreateBulletMessage( BulletType bType, CBaseObject* firingEntity, float yVel)
	: CMessage( MSG_CREATE_BULLET ), m_nBulletType(bType),
					m_pFiringEntity( firingEntity ), m_fVelY( yVel )
{
	
}


CCreateBulletMessage::~CCreateBulletMessage(void)
{

}
