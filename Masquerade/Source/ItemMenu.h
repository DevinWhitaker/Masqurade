#pragma once
#include "Powerup.h"
#include "..\SGD Wrappers\SGD_Math.h"
#include "BitmapFont.h"

class CCorinne;
class CFox;

class CItemMenu
{
private:
	int m_nPowIDs[CPowerup::MAX_POWERUPS];
	
	int m_nCursorID;
	int m_nCursorPos;
	int m_nBGimageID;
	float m_fMouseMove;

	tVector2D m_vCenterPos;
	RECT m_rBoundingRect;

	CCorinne* m_pCorinne;
	CFox* m_pFox;
	
	bool m_bInMenu;
	RECT m_rPowerupPos[CPowerup::MAX_POWERUPS];

	float m_fPowWidth;
	float m_fPowHeight;

	float m_fPowImageScale;
	bool m_bIsMainPause;

	CBitmapFont m_BMP;

public:
	CItemMenu(void);
	~CItemMenu(void);

	// Call after the HUD has been Initialized
	// because the images for power ups are 
	// loaded in from the HUD
	void Init(int width, int height);
	void Shutdown(void);

	bool Input(void);
	void Render(void);

	void setInMenu(bool isInMenu)		{m_bInMenu = isInMenu;}
	bool getInMenu(void)				{return m_bInMenu;}

	void setIsMainPause(bool isMainPause)	{m_bIsMainPause = isMainPause;}
	bool &getIsMainPause(void)				{return m_bIsMainPause;}

	void toggleInMenu(void)				
	{
		m_bInMenu = !m_bInMenu;
		if(m_bInMenu)
			m_nCursorPos = 0;
	}

};

