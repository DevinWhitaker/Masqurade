#pragma once

#include "../SGD Wrappers/IMessage.h"
#include "Bullet.h"

class CBaseObject;
class CCreateBulletMessage : public IMessage
{
public:
	CCreateBulletMessage( BulletType bType, CBaseObject* firingEntity, float yVel = 0.0f );
	virtual ~CCreateBulletMessage(void);

	//	Accessors
	int					&GetBulletType( void )		{ return m_nBulletType; }
	CBaseObject*		getFiringEntity(void)		{ return m_pFiringEntity; }
	float				&GetVelY(void)				{ return m_fVelY; }
	virtual MSGID		GetMessageID( void )		{ return MSG_CREATE_BULLET; }

private:
	int					m_nBulletType;
	CBaseObject*		m_pFiringEntity;
	float				m_fVelY;
};
