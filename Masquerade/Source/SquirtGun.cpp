#include "SquirtGun.h"
#include "CreateBulletMessage.h"
#include "..\SGD Wrappers\CSGD_MessageSystem.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "Camera.h"

CSquirtGun::CSquirtGun(void)
{
	m_eToolType = TOOL_SQUIRTGUN;
}


CSquirtGun::~CSquirtGun(void)
{
}

bool CSquirtGun::Shoot(CBaseObject* firingEntity)
{
	ITool::Shoot(firingEntity);

	return true;
}

void CSquirtGun::Update(float fElapsed)
{
	//ITool::Update(fElapsed);
	CBaseObject::Update(fElapsed);

	if( m_bFuseLit )
	{
		CCreateBulletMessage* pBulletMsg = new CCreateBulletMessage(SQUIRT_BULLET, m_pFiringEntity );
		CSGD_MessageSystem* pMS = CSGD_MessageSystem::GetInstance();
		pMS->QueueMessage(pBulletMsg);
		m_bFuseLit = false;
		m_fChargeTime = 0.0f;
		m_isSelected = false;
	}
}

void CSquirtGun::Render()
{
	ITool::Render();

}