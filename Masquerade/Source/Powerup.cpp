#include "Powerup.h"
#include "Fox.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "Camera.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "FileManager.h"
CPowerup::CPowerup(void)
{
	m_bIsPickedUp = false;
	this->m_nType = OBJ_POWERUP;
}


CPowerup::~CPowerup(void)
{
}

void CPowerup::Render(void)
{
	if(GetImageID() != -1 && m_bIsPickedUp == false)
	{
		CCamera* cam = CCamera::GetInstance();
		CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
		
		pTM->Draw( GetImageID(), 
			(int)((GetX()-GetWidth()/2)-cam->OffsetX()), 
			(int)((GetY()-GetHeight()/2)-cam->OffsetY()), 0.5f, 0.5f );
	}
}


bool CPowerup::CheckCollision( IBaseObject* pOther )
{
	if( m_bIsPickedUp == false)
	{
		if( CBaseObject::CheckCollision( pOther ) == true )
		{
			if(pOther->GetType() == OBJ_FOX )
			{
				CFox* fox = dynamic_cast<CFox*>(pOther);
				fox->AddAPowerUp(m_eCurrPowerupType);
				CSGD_XAudio2::GetInstance()->SFXSetSoundVolume( 	CFileManager::GetInstance()->GetSoundID("MASQ_Powerup"), 1.0f );
				CSGD_XAudio2::GetInstance()->SFXPlaySound( 
					CFileManager::GetInstance()->GetSoundID("MASQ_Powerup") );
				m_bIsPickedUp = true;
			}
		}
	}

	return false;
}