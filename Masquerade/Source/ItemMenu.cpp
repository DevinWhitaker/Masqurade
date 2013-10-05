#include "ItemMenu.h"
#include "FileManager.h"
#include "Fox.h"
#include "Corinne.h"
#include "GamePlayState.h"
#include "Game.h"
#include "..\SGD Wrappers\CSGD_Direct3D.h"
#include "..\SGD Wrappers\CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"

#include <vector>


CItemMenu::CItemMenu(void)
{
	m_fPowImageScale = 1.0f;
}


CItemMenu::~CItemMenu(void)
{
}

void CItemMenu::Init(int width, int height)
{
	CFileManager* pFM = CFileManager::GetInstance();
	m_nPowIDs[CPowerup::NORMAL] = pFM->GetImageID("normal.png");
	m_nPowIDs[CPowerup::GREASED_PIG] = pFM->GetImageID("greased_pig.png");
	m_nPowIDs[CPowerup::CHEETAH] = pFM->GetImageID("cheetah.png");
	m_nPowIDs[CPowerup::POGO] = pFM->GetImageID("pogo.png");
	m_nPowIDs[CPowerup::FLUBBER] = pFM->GetImageID("flubber.png");
	m_nPowIDs[CPowerup::FIZZY_LIFTING_DRINK] = pFM->GetImageID("fizzy_lifting_drink.png");

	m_nCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Menu_Selection.png") );
	pFM->AddImageID( "cursor.png", m_nCursorID );
	
	m_nBGimageID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resources/Images/MenuImages/MASQ_Item_Background.png") );
	pFM->AddImageID( "Item_Background+9.png", m_nBGimageID );

	m_vCenterPos.fX = (float)CGame::GetInstance()->GetWidth()/2.0f;
	m_vCenterPos.fY = (float)CGame::GetInstance()->GetHeight()/2.0f;

	tVector2D center = m_vCenterPos;

	m_rBoundingRect.left = (LONG)(center.fX-width/2);
	m_rBoundingRect.top = (LONG)(center.fY-height/2);
	m_rBoundingRect.right = (LONG)(center.fX+width/2);
	m_rBoundingRect.bottom = (LONG)(center.fY+height/2);

	int box4th = (m_rBoundingRect.right-m_rBoundingRect.left)/4;
	int box12th = (m_rBoundingRect.right-m_rBoundingRect.left)/12;

	

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	m_fPowWidth = ((float)pTM->GetTextureWidth(m_nPowIDs[CPowerup::NORMAL])*m_fPowImageScale);
	m_fPowHeight = ((float)pTM->GetTextureWidth(m_nPowIDs[CPowerup::NORMAL])*m_fPowImageScale);
	

	m_rPowerupPos[0].left = (LONG)(center.fX);
	m_rPowerupPos[0].top = (LONG)(m_rBoundingRect.top);

	m_rPowerupPos[1].left = (LONG)(m_rBoundingRect.right-box12th);
	m_rPowerupPos[1].top = m_rBoundingRect.top+box4th;

	m_rPowerupPos[2].left = m_rBoundingRect.right-box12th;
	m_rPowerupPos[2].top = m_rBoundingRect.bottom-box4th;

	m_rPowerupPos[3].left = (LONG)(center.fX);
	m_rPowerupPos[3].top = m_rBoundingRect.bottom;

	m_rPowerupPos[4].left = m_rBoundingRect.left+box12th;
	m_rPowerupPos[4].top = m_rBoundingRect.bottom-box4th;

	m_rPowerupPos[5].left = m_rBoundingRect.left+box12th;
	m_rPowerupPos[5].top = m_rBoundingRect.top+box4th;

	for(int i = 0; i < CPowerup::MAX_POWERUPS; ++i)
	{
		m_rPowerupPos[i].left -= (LONG)(m_fPowWidth/2.0f);
		m_rPowerupPos[i].top -= (LONG)(m_fPowHeight/2.0f);
		m_rPowerupPos[i].right = (LONG)(m_rPowerupPos[i].left+m_fPowWidth);
		m_rPowerupPos[i].bottom = (LONG)(m_rPowerupPos[i].top+m_fPowHeight);
	}

	m_pFox = CGamePlayState::GetInstance()->GetFox();
	m_pCorinne = CGamePlayState::GetInstance()->GetCorrine();

	m_nCursorPos = 0;
	m_bIsMainPause = false;

	m_BMP.Init(_T("Resources/Images/MenuImages/MASQ_Font.png"), 32, 32, 8, 8, 32 );
}

void CItemMenu::Shutdown(void)
{
	m_BMP.Shutdown();
}
	
bool CItemMenu::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	
	POINT mouse;

	GetCursorPos(&mouse);
	ScreenToClient(CGame::GetInstance()->GetWindowHandle(), &mouse);


#if !ARCADE
	if(pDI->KeyPressed(DIK_D) || pDI->KeyPressed(DIK_RIGHT) )
	{
		if( m_nCursorPos == CPowerup::MAX_POWERUPS-1 )
			m_nCursorPos = 0;
		else
			++m_nCursorPos;
	}
	if(pDI->KeyPressed(DIK_A) || pDI->KeyPressed(DIK_LEFT) )
	{
		if( m_nCursorPos == 0 )
			m_nCursorPos = CPowerup::MAX_POWERUPS-1;
		else
			--m_nCursorPos;
	}

	if(pDI->MouseMovementX() != 0 || pDI->MouseMovementY() != 0 || pDI->MouseButtonPressed(0) )
	for(int i = 0; i < CPowerup::MAX_POWERUPS; ++i)
	{
		if(mouse.x > m_rPowerupPos[i].left && mouse.x < m_rPowerupPos[i].right &&
			mouse.y > m_rPowerupPos[i].top && mouse.y < m_rPowerupPos[i].bottom )
		{
			m_nCursorPos = i;
			break;
		}
	}

	//if(pDI->KeyPressed(DIK_E) || pDI->MouseButtonPressed(0) )
		//printf( "Mouse	X: %i , Y: %i\n", (int)mouse.x, (int)mouse.y );

	if(pDI->MouseButtonPressed(0) || pDI->KeyPressed(DIK_E) || pDI->KeyPressed(DIK_RETURN) )
	{
		std::vector<CPowerup::PowerupType> pows = m_pFox->GetPowerups();
		if(pows[m_nCursorPos] != -1)
			m_pCorinne->setPowerup(pows[m_nCursorPos]);
	}

	return true;
#else
	if(pDI->JoystickGetLStickDirPressed( DIR_RIGHT ) )
	{
		if( m_nCursorPos == CPowerup::MAX_POWERUPS-1 )
			m_nCursorPos = 0;
		else
			++m_nCursorPos;
	}
	if(pDI->JoystickGetLStickDirPressed( DIR_LEFT ) )
	{
		if( m_nCursorPos == 0 )
			m_nCursorPos = CPowerup::MAX_POWERUPS-1;
		else
			--m_nCursorPos;
	}

	if(pDI->MouseMovementX() != 0 || pDI->MouseMovementY() != 0 || pDI->MouseButtonPressed(0) )
	for(int i = 0; i < CPowerup::MAX_POWERUPS; ++i)
	{
		if(mouse.x > m_rPowerupPos[i].left && mouse.x < m_rPowerupPos[i].right &&
			mouse.y > m_rPowerupPos[i].top && mouse.y < m_rPowerupPos[i].bottom )
		{
			m_nCursorPos = i;
			break;
		}
	}


	if( pDI->JoystickButtonPressed( 0 ) )
	{
		std::vector<CPowerup::PowerupType> pows = m_pFox->GetPowerups();
		if(pows[m_nCursorPos] != -1)
			m_pCorinne->setPowerup(pows[m_nCursorPos]);
	}

	return true;
#endif
}

void CItemMenu::Render(void)
{

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	pD3D->GetSprite()->Flush();
	//pD3D->DrawRect(m_rBoundingRect, 255, 255, 255 );
	
	std::vector<CPowerup::PowerupType> pows = m_pFox->GetPowerups();

	pTM->Draw(m_nBGimageID, m_rBoundingRect.left, m_rBoundingRect.top );

	for(int i = 0; i < CPowerup::MAX_POWERUPS; ++i)
	{
		if( pows[i] != CPowerup::EMPTY)
		{
			pTM->Draw(m_nPowIDs[pows[i]], 
			m_rPowerupPos[i].left, 
			m_rPowerupPos[i].top,
			m_fPowImageScale, m_fPowImageScale );
		}

		//CSGD_Direct3D::GetInstance()->DrawRect( m_rPowerupPos[i], 255, 0, 0);
	}

	pTM->Draw(m_nCursorID, 
		m_rPowerupPos[m_nCursorPos].left, m_rPowerupPos[m_nCursorPos].top,
		m_fPowImageScale, m_fPowImageScale, 0, 0, 0, 0, D3DCOLOR_ARGB(100, 255, 255, 255) );
	
	if( ! m_bIsMainPause )
	{
		m_BMP.Print( "A OR D TO MOVE LEFT OR RIGHT", 275, 280, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
		m_BMP.Print( "E OR MOUSE CLICK TO SELECT", 285, 310, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);

		switch(pows[m_nCursorPos])
		{
		case CPowerup::NORMAL:
			m_BMP.Print( "NORMAL:", 375, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "REGULAR CORINNE, NOTHING SPECIAL", 260, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		case CPowerup::GREASED_PIG:
			m_BMP.Print( "GREASED PIG:", 345, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "CORINNE CAN ESCAPE FROM HOODLUM'S GRASP", 225, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		case CPowerup::CHEETAH:
			m_BMP.Print( "CHEETAH:", 365, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "CORINNE CAN RUN FASTER AND BE THROWN FURTHER", 210, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		case CPowerup::POGO:
			m_BMP.Print( "POGO:", 380, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "CORINNE CAN BOTH JUMP AND BE THROWN HIGHER", 215, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		case CPowerup::FLUBBER:
			m_BMP.Print( "FLUBBER:", 370, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "CORINNE WILL BOUNCE OFF OF WALLS", 255, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		case CPowerup::FIZZY_LIFTING_DRINK:
			m_BMP.Print( "FIZZY LIFTING DRINK:", 310, 540, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			m_BMP.Print( "CORINNE DOES NOT SLOW FOX DOWN WHEN CARRIED", 210, 565, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 23);
			break;
		}
	}
	//CSGD_Direct3D::GetInstance()->DrawRect( m_rBoundingRect, 255, 0, 0);
	
}