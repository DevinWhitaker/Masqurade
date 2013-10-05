#include "Rat.h"
#include "FileManager.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "AnimationManager.h"
#include "Camera.h"
#include <vector>
#include <string>
#include "DestroyEnemyMessage.h"
#include "..\SGD Wrappers\CSGD_MessageSystem.h"
#include "GamePlayState.h"

CRat::CRat(void)
{
	m_nType = OBJ_ENEMY;
	setEnemyType( NME_RAT );
}

CRat::CRat( CRat& rat )
{
	m_pThunderbolt = nullptr;

	m_ratFrame.Init(this);
	std::vector<std::string> keys;
	keys = CAnimationManager::GetInstance()->CreateAnimationFromFile("Resources/Scripts/rat_animation_2.xml");
	

	
	m_nType = rat.m_nType;
	setEnemyType( rat.getEnemyType() );
	if( CGamePlayState::GetInstance()->GetCurStage() == 2 )
	{
		m_ratFrame.SetAnimationID(keys[1]);
		m_pThunderbolt = new CFrame;
		m_pThunderbolt->Init( this );
		m_pThunderbolt->SetAnimationID(keys[2]);
		CAnimationManager::GetInstance()->AddFrame(*m_pThunderbolt);
	}
	else
	{
		m_ratFrame.SetAnimationID(keys[0]);
	}
	CAnimationManager::GetInstance()->AddFrame(m_ratFrame);
	m_ratFrame.Play(true, 0.5f);

	m_nHealth = 30;
	m_nEnemyType = NME_RAT;
	
	SetVelX(100);
	SetVelX( (rand()%2 == 1) ? GetVelX() : -GetVelX() );

	m_nMoveDistance = 64;
}

void CRat::Init(float xStartPos, float yStartPos)
{
	SetX(xStartPos);
	SetY(yStartPos);

	m_nStartPosX = (int)(xStartPos);
}

CRat::~CRat(void)
{
	CAnimationManager::GetInstance()->RemoveFrame(&m_ratFrame);
	if(m_pThunderbolt != nullptr)
		CAnimationManager::GetInstance()->RemoveFrame(m_pThunderbolt);
}

void CRat::Update(float fElapsed)
{
	//SetGravityBool(true);
	if(m_nHealth <= 0)
	{
		m_bIsAlive = false;
		CDestroyEnemyMessage* pMsg = new CDestroyEnemyMessage(this);
		CSGD_MessageSystem::GetInstance()->SendMsg(pMsg);
		pMsg = nullptr;
		CAnimationManager::GetInstance()->RemoveFrame(&m_ratFrame);
		if ( CGamePlayState::GetInstance()->GetCurStage() == 2 )
			++CGamePlayState::GetInstance()->m_nPikachuKills;
	}
	float x = GetVelX();

	if(m_bIsAlive)
	{
		CEnemy::Update(fElapsed);

		m_ratFrame.SetFlipped( (GetVelX() > 0) ? true : false );
		
		if( fabs(m_nStartPosX-GetX()) >= m_nMoveDistance )
		{
			SetX( (float)((GetX() < m_nStartPosX) ? m_nStartPosX-m_nMoveDistance : m_nStartPosX+m_nMoveDistance ));
			SetVelX( -GetVelX() );
		}
	}
}

void CRat::Render()
{
	if(m_bIsAlive)
	{
		/*CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		CCamera* cam = CCamera::GetInstance();
		pTM->Draw(GetImageID(), GetX()-cam->OffsetX(), GetY()-cam->OffsetY() );*/
	}
	/*CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	pD3D->GetSprite()->Flush();
	CCamera* cam = CCamera::GetInstance();
	RECT r = GetRect();
	r.left -= (LONG)cam->OffsetX();
	r.top -= (LONG)cam->OffsetY();
	r.right -= (LONG)cam->OffsetX();
	r.bottom -= (LONG)cam->OffsetY();
	pD3D->DrawRect( r, 255, 0, 0);*/
}

bool CRat::CheckCollision(IBaseObject* pBase)
{
	//CEnemy::CheckCollision(pBase);
	
	//if( m_ratFrame.CheckCollision( (CBaseObject*)pBase ) )
	if( CBaseObject::CheckCollision( pBase ) == true )
	{
		if(pBase->GetType() == OBJ_FOX)
			CSGD_EventSystem::GetInstance()->SendEvent( "Fox_Hit", this);

		if (pBase->GetType() == OBJ_FOX || pBase->GetType() == OBJ_CORINNE)
		{
			if ( CGamePlayState::GetInstance()->GetCurStage() == 2 )
			{
				if(!m_pThunderbolt->isPlaying())
					m_pThunderbolt->Play();
			}
		}
	}
	
	return false;
}

void CRat::tileCollisionResponse(int COLLISION_SIDE)
{
	/*switch(COLLISION_SIDE)
	{
	case LEFT:
		SetX( GetX() - 8 );
		SetVelX(-GetVelX());
		break;
	case RIGHT:
		SetX( GetX() + 8 );
		SetVelX(-GetVelX());
		break;
	case TOP:
		SetVelY(0);
		break;
	case BOTTOM:
		SetVelY(0);
		break;
	}*/
}

void CRat::CollisionResponse( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor )
{
	if(pBase->GetType() == OBJ_FOX)
			CSGD_EventSystem::GetInstance()->SendEvent( "Fox_Hit", this);
}


void CRat::HandleEvent(CEvent* pEvent)
{
	CEnemy::HandleEvent(pEvent);
}


RECT CRat::GetRect()
{
	return m_ratFrame.GetRect();
}