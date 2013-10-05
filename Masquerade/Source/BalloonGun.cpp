#include "BalloonGun.h"
#include "ObjectManager.h"
#include "..\SGD Wrappers\SGD_Math.h"
#include "Crate.h"
#include "FileManager.h"

CBalloonGun::CBalloonGun(void)
{
	m_eToolType = TOOL_BALLOONGUN;
	m_bFuseLit = false;
	m_fMaxAttachDistance = 128.0f;
}


CBalloonGun::~CBalloonGun(void)
{
}

bool CBalloonGun::Shoot(CBaseObject* firingEntity)
{
	ITool::Shoot(firingEntity);

	return true;
}

float Distance(tVector2D x, tVector2D y)
{
	return sqrt( pow((x.fX-y.fX), 2) + pow((x.fY-y.fY), 2) );
}

void CBalloonGun::Update(float fElapsed)
{
	//ITool::Update(fElapsed);
	CBaseObject::Update(fElapsed);

	if( m_bFuseLit && m_pFiringEntity != nullptr )
	{
		std::vector<IBaseObject*> crates = CObjectManager::GetInstance()->FindObjectsWithType( OBJ_CRATE );
		CBaseObject* closestCrate = nullptr;
		tVector2D nearest;
		nearest.fX = 0;
		nearest.fY = 0;	

		tVector2D fox;
		fox.fX = m_pFiringEntity->GetX();
		fox.fY = m_pFiringEntity->GetY();

		for(decltype(crates.size()) i = 0; i < crates.size(); ++i)
		{
			CBaseObject* cur = dynamic_cast<CBaseObject*>(crates[i]);
			if( cur != nullptr )
			{
				if( i == 0 )
				{
					closestCrate = cur;
					nearest.fX = cur->GetX();
					nearest.fY = cur->GetY();
				}
				else
				{
					tVector2D next;
					next.fX = cur->GetX();
					next.fY = cur->GetY();
					
					float newdist = Distance(fox, next);


					float nearestdist = Distance(fox, nearest);

					if( newdist < nearestdist )
					{
						closestCrate = cur;
						nearest.fX = cur->GetX();
						nearest.fY = cur->GetY();
					}
				}
			}
		}
		float dist = Distance(fox, nearest);

		if(closestCrate != nullptr)
		{
			if( dist <= m_fMaxAttachDistance)
			{
				CSGD_XAudio2*	pXA = CSGD_XAudio2::GetInstance();
				CCrate* c = dynamic_cast<CCrate*>(closestCrate);
				c->AttachBalloon();
				pXA->SFXPlaySound( CFileManager::GetInstance()->GetSoundID( "MASQ_BalloonGun_Shoot" ) );
			}
		}
		// Create bullet message here
		m_bFuseLit = false;
		m_fChargeTime = 0.0f;	
		m_isSelected = false;
	}
}

void CBalloonGun::Render()
{
	ITool::Render();
}

