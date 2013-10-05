
#include "DestroyBulletMessage.h"

#include "Bullet.h"
#include "BaseObject.h"

CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet)
	: CMessage( MSG_DESTROY_BULLET ),
	  m_pBullet( pBullet )
{
	if( m_pBullet != nullptr )
		m_pBullet->AddRef();
}


CDestroyBulletMessage::~CDestroyBulletMessage(void)
{
	if( m_pBullet != nullptr )
		m_pBullet->Release();

	m_pBullet = nullptr;
}