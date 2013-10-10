#include "CottonCandyLauncher.h"
#include "CreateBulletMessage.h"
#include "..\SGD Wrappers\CSGD_MessageSystem.h"

CCottonCandyLauncher::CCottonCandyLauncher(void)
{
	m_eToolType = TOOL_COTTONCANDYLAUNCHER;
	m_bIsChargable = true;
}


CCottonCandyLauncher::~CCottonCandyLauncher(void)
{
}

bool CCottonCandyLauncher::Shoot(CBaseObject* firingEntity)
{
	ITool::Shoot(firingEntity);

	return true;
}

void CCottonCandyLauncher::Update(float fElapsed)
{
	ITool::Update(fElapsed);
	CBaseObject::Update(fElapsed);

	if( m_bFuseLit )
	{
		CCreateBulletMessage* pBulletMsg = new CCreateBulletMessage(COTTON_BULLET, m_pFiringEntity, -m_fChargeTime );
		CSGD_MessageSystem* pMS = CSGD_MessageSystem::GetInstance();
		pMS->QueueMessage(pBulletMsg);
		m_fChargeTime = 0.0f;
		m_bFuseLit = false;
		m_isSelected = false;
	}
}

void CCottonCandyLauncher::Render()
{
	ITool::Render();
}