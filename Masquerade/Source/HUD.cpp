#include "HUD.h"
#include "..\SGD Wrappers\CEvent.h"
#include "GamePlayState.h"
#include "FileManager.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "Fox.h"
#include "Corinne.h"
#include "ITool.h"
#include "FileManager.h"
#include "ParticleManager.h"
#include "CreateEmitterMessage.h"
#include "..\SGD Wrappers\CSGD_MessageSystem.h"
#include "CottonCandyLauncher.h"

CHUD::CHUD(void)
{
	m_fHeartScale	= 0.5f;
	m_fToolScale	= 1.0f;
	m_fPowerupScale	= 1.0f;
	m_fBorderScale	= 1.0f;

	
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CFileManager::GetInstance();

	m_bIsDarkened = false;

	m_nHeartStartPos.fX = 50;
	m_nHeartStartPos.fY = 100;

	m_pHeartPop = nullptr;
}


CHUD::~CHUD(void)
{
}

CHUD* CHUD::GetInstance()
{
	static CHUD hud;

	return &hud;
}

void CHUD::Init()
{
	m_nHeartID = m_pTM->LoadTexture( _T("Resources/Images/Graphics/MASQ_HUD_Heart.png"));
	m_pFM->AddImageID( "SGP_HUD_Heart.png", m_nHeartID );

	// HUD tool images
	m_nSquirtID = m_pFM->GetImageID("Squirt_Gun_HUD.png");
	m_nBalloonID = m_pFM->GetImageID("Balloon_Gun_HUD.png");
	m_nCottonID = m_pFM->GetImageID("CottonCandy_Gun_HUD.png");

	m_nPowerupIDs[CPowerup::NORMAL] = m_pFM->GetImageID( "normal.png" );
	m_nPowerupIDs[CPowerup::GREASED_PIG] = m_pFM->GetImageID( "greased_pig.png" );
	m_nPowerupIDs[CPowerup::CHEETAH] = m_pFM->GetImageID( "cheetah.png" );
	m_nPowerupIDs[CPowerup::POGO] = m_pFM->GetImageID( "pogo.png" );
	m_nPowerupIDs[CPowerup::FLUBBER] = m_pFM->GetImageID( "flubber.png" );
	m_nPowerupIDs[CPowerup::FIZZY_LIFTING_DRINK] = m_pFM->GetImageID( "fizzy_lifting_drink.png" );
	
	CFileManager* pFM = CFileManager::GetInstance();

	m_nBGimageID = m_pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_HUD_Background.png") );
	pFM->AddImageID( "MASQ_HUD_Background.png", m_nBGimageID );
	
	m_pFox = CGamePlayState::GetInstance()->GetFox();
	m_pCorinne = CGamePlayState::GetInstance()->GetCorrine();

	m_nFoxCurHearts = m_pFox->GetHearts();

	m_nCurrToolID = -1;
}

void CHUD::Render()
{
	//m_pD3D->DrawLine( m_nHeartStartPos.fX, m_nHeartStartPos.fY, m_nHeartStartPos.fX+
	m_pTM->Draw( m_nBGimageID, 0, 25, 1.25f, 1.25f);

	switch(m_pFox->GetCurrentToolType())
	{
	case ITool::TOOL_SQUIRTGUN:
		m_nCurrToolID = m_nSquirtID;
		break;
	case ITool::TOOL_BALLOONGUN:
		m_nCurrToolID = m_nBalloonID;
		break;
	case ITool::TOOL_COTTONCANDYLAUNCHER:
		m_nCurrToolID = m_nCottonID;
		break;
	}

	if( m_pFox->GetCurrentToolType() != -1 )
	{
		m_pTM->Draw( m_nCurrToolID, 120, 0, 0.8f, 0.8f );
		
		
		if(m_pFox->GetATool(m_pFox->GetCurrentTool())->getToolType() == ITool::TOOL_COTTONCANDYLAUNCHER )
		{
			CCottonCandyLauncher* ccl = dynamic_cast<CCottonCandyLauncher*>(m_pFox->GetATool(m_pFox->GetCurrentTool()) );
			CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
			pD3D->GetSprite()->Flush();

			float max = ccl->getMaxCharge();
			float cur = ccl->getChargeTime();

			RECT r;
			r.left = 100;
			r.top = (max > 0) ? (LONG)(60 - ( cur/max * 50 )) : 60;
				 
			r.right = 110;
			r.bottom = 60;

			pD3D->DrawRect(r,255, 100, 200);
			
			pD3D->DrawLine( 100, 10, 110, 10, 0,0,0);
			pD3D->DrawLine( 110, 10, 110, 60, 0,0,0);
			pD3D->DrawLine( 110, 60, 100, 60, 0,0,0);
			pD3D->DrawLine( 100, 60, 100, 10, 0,0,0);
		}
	}

	

	bool bHeartPop = false;
	if( m_pFox->GetHearts() < m_nFoxCurHearts || m_pFox->GetHearts() == 0 || m_pFox->GetHearts() == 5 )
	{
		bHeartPop = true;
		m_nFoxCurHearts = m_pFox->GetHearts();
	}

	

	float HeartScale = m_fHeartScale * 0.5f;
	int width = m_pTM->GetTextureWidth(m_nHeartID);
	int height = m_pTM->GetTextureHeight(m_nHeartID);

	float offsetX = 0;
	float offsetY = 0;
	float prevSize = 0;
	float spacer = 0;
	float raiseYpos = 0;

	for( int i = 0; i < 5; ++i)
	{

		float curHeartSize = (width*HeartScale/2.0f);
		offsetX = (width*m_fHeartScale/2)*i;
		spacer +=  prevSize*0.5f;

		if( bHeartPop )
		{
			if( i == m_nFoxCurHearts && i != 0 )
			{
				float xPos = (m_nHeartStartPos.fX-curHeartSize+offsetX+spacer) + curHeartSize/2.0f;
				float yPos = ((m_nHeartStartPos.fY-(height*HeartScale)/2.0f+offsetY)+raiseYpos) + curHeartSize/2.0f;
				CCreateEmitterMessage*	pPE = new CCreateEmitterMessage( "Heart Pop", "Heart Pop", xPos, yPos, nullptr, false );
				CSGD_MessageSystem::GetInstance()->SendMsg( pPE );
				pPE = nullptr;
				/*m_pHeartPop = CParticleManager::GetInstance()->CloneEmitterFromTemplate("Heart Pop");
				m_pHeartPop->SetPos( (m_nHeartStartPos.fX-curHeartSize+offsetX+spacer),
								((m_nHeartStartPos.fY-(height*HeartScale)/2.0f+offsetY)+raiseYpos) );
				CParticleManager::GetInstance()->AddEmitter(m_pHeartPop);*/
			}

		}

		if( i < m_pFox->GetHearts() )
		m_pTM->Draw( m_nHeartID, (int)(m_nHeartStartPos.fX-curHeartSize+offsetX+spacer), 
			(int)((m_nHeartStartPos.fY-(height*HeartScale)/2.0f+offsetY)+raiseYpos), HeartScale, HeartScale);
		

		prevSize = HeartScale*width;
		/*m_pTM->Draw( m_nHeartID, (m_nHeartStartPos.fX-(width*HeartScale)/2.0f)+(width*m_fHeartScale/2)*i, 
			(m_nHeartStartPos.fY-(height*HeartScale)/2.0f+offsetY), HeartScale, HeartScale);*/
		offsetY = -offsetY;
		if( i%2 == 1 )
			offsetY -= i*10*HeartScale;
		else
			offsetY += i*10*HeartScale;
		
		if( i == 1 || i == 3 )
			HeartScale += HeartScale * 0.25f;

		
		if( i == 3 )
			raiseYpos += prevSize * 0.3f;
		else
			raiseYpos -= prevSize * 0.4f;

		
			
	}

	if(m_bIsDarkened)
	m_pTM->Draw( m_pFM->GetImageIdMap()["Pause_Screen"].nId, 0, 0, 1.0f,
			1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 155, 255, 255, 255 ) );

	m_pTM->Draw( m_nPowerupIDs[m_pCorinne->getPowerup()], 15, 5, 1.0f, 1.0f );

}

void CHUD::HandleEvent(CEvent* pEvent)
{
}