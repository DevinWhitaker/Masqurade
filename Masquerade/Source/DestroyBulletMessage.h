#pragma once
#include "../SGD Wrappers/IMessage.h"

class CBullet;

class CDestroyBulletMessage :
	public IMessage
{
public:
	explicit CDestroyBulletMessage( CBullet* pBullet );
	virtual ~CDestroyBulletMessage(void);

	// Accessor
	CBullet*			GetBullet(void)				{ return m_pBullet; }
	virtual MSGID		GetMessageID( void )		{ return MSG_DESTROY_BULLET; }

private:
	CBullet* m_pBullet;
};