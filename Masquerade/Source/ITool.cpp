#include "ITool.h"
#include "Fox.h"
#include "Camera.h"
#include "GamePlayState.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "FileManager.h"
ITool::ITool(void)
{
	m_bIsPickedUp = false;

	m_bIsChargable = false;
	m_fChargeTime = 0.0f;
	m_fMinCharge = 0.0f;
	m_fMaxCharge = 2.0f;
	m_bIsCharging = false;
	m_bFuseLit = false;
	m_pFiringEntity = nullptr;
	m_isSelected = false;
	m_nType = OBJ_TOOL;
}


ITool::~ITool(void)
{
}

bool ITool::Shoot(CBaseObject* firingEntity)
{
	m_pFiringEntity = firingEntity;
	
	if( ! m_bIsChargable )
		m_bFuseLit = true;
	
	return true;
	
}

void ITool::Update(float fElapsed)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( m_bIsPickedUp )
	{
		SetX(CGamePlayState::GetInstance()->GetFox()->GetX());
		SetY(CGamePlayState::GetInstance()->GetFox()->GetY());
	}

#if !ARCADE
	if( m_isSelected && m_bIsChargable)
	{
		if( pDI->MouseButtonReleased(0) )
		{
			m_bIsCharging = false;
			
			if(m_fChargeTime >= m_fMinCharge)
			{
				m_bFuseLit = true;
			}
			else if( ! m_bFuseLit )
			{
				m_bFuseLit = false;
				m_fChargeTime = 0.0f;
			}
		}

		if( pDI->MouseButtonDown(0) )
			m_bIsCharging = true;

		if(m_bIsCharging)
		{
			m_fChargeTime += fElapsed;
			if(m_fChargeTime > m_fMaxCharge) 
				m_fChargeTime = m_fMaxCharge;
		}
	}
#else
	if( m_isSelected && m_bIsChargable)
	{
		if( !pDI->JoystickButtonDown( 0 ) && m_bIsCharging )
		{
			m_bIsCharging = false;
			
			if(m_fChargeTime >= m_fMinCharge)
			{
				m_bFuseLit = true;
			}
			else if( ! m_bFuseLit )
			{
				m_bFuseLit = false;
				m_fChargeTime = 0.0f;
			}
		}

		if( pDI->JoystickButtonPressed( 0 ) )
			m_bIsCharging = true;

		if(m_bIsCharging)
		{
			m_fChargeTime += fElapsed;
			if(m_fChargeTime > m_fMaxCharge) 
				m_fChargeTime = m_fMaxCharge;
		}
	}
#endif
	
}

void ITool::Render(void)
{
	if(GetImageID() != -1 && m_bIsPickedUp == false)
	{
		CCamera* cam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		pTM->Draw( GetImageID(), 
			(int)(GetX()-cam->OffsetX()), (int)(GetY()-cam->OffsetY()), 0.5f, 0.5f );

			//GetX()-GetWidth()/2.0f-cam->OffsetX(), GetY()-GetHeight()/2.0f-cam->OffsetY(), 0.25f, 0.25f ) ) );
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

bool ITool::CheckCollision( IBaseObject* pOther )
{
	if( m_bIsPickedUp == false )
	{
		if( CBaseObject::CheckCollision( pOther ) == true )
		{
			if(pOther->GetType() == OBJ_FOX)
			{
				CFox* fox = dynamic_cast<CFox*>(pOther);
				fox->AddATool(this);
				CSGD_XAudio2::GetInstance()->SFXSetSoundVolume( 	CFileManager::GetInstance()->GetSoundID("MASQ_Powerup"), 1.0f );
				if( CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying( CFileManager::GetInstance()->GetSoundID("MASQ_Powerup") ) == false )
					CSGD_XAudio2::GetInstance()->SFXPlaySound( CFileManager::GetInstance()->GetSoundID("MASQ_Powerup")  ) ;
				m_bIsPickedUp = true;
			}
		}
	}

	return false;
}

RECT ITool::GetRect( )
{
	RECT r;
	r.left = (LONG)(GetX()-GetWidth()/2.0f);
	r.top = (LONG)(GetY()-GetHeight()/2.0f);
	r.right = (LONG)(GetX()+GetWidth()/2.0f);
	r.bottom = (LONG)(GetY()+GetHeight()/2.0f);
	return r;
}
