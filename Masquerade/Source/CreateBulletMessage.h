#pragma once

#include "Message.h"
#include "Bullet.h"

class CBaseObject;
class CCreateBulletMessage : public CMessage
{
public:
	CCreateBulletMessage( BulletType bType, CBaseObject* firingEntity, float yVel = 0.0f );
	virtual ~CCreateBulletMessage(void);

	//	Accessors
	int					&GetBulletType( void )		{ return m_nBulletType; }
	CBaseObject*		getFiringEntity(void)		{ return m_pFiringEntity; }
	float				&GetVelY(void)				{ return m_fVelY; }

private:
	int					m_nBulletType;
	CBaseObject*		m_pFiringEntity;
	float				m_fVelY;
};
