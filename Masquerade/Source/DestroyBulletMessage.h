#pragma once
#include "message.h"

class CBullet;

class CDestroyBulletMessage :
	public CMessage
{
public:
	explicit CDestroyBulletMessage( CBullet* pBullet );
	virtual ~CDestroyBulletMessage(void);

	// Accessor
	CBullet*	GetBullet(void) { return m_pBullet; }

private:
	CBullet* m_pBullet;
};