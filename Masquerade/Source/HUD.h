#pragma once
#include "..\SGD Wrappers\IListener.h"
#include "..\SGD Wrappers\SGD_Math.h"
#include "Powerup.h"
class CFox;
class CEvent;
class	CSGD_TextureManager;
class	CFileManager;
class	CSGD_Direct3D;
class	CCorinne;
class	CParticleEmitter;

class CHUD :
	public IListener
{
private:
	int m_nCurrToolID;
	int m_nCurrPowerupID;
	int m_nSquirtID;
	int m_nBalloonID;
	int m_nCottonID;
	int m_nHeartID;
	int m_nNumHearts;
	int m_nPowerupIDs[CPowerup::MAX_POWERUPS];
	int m_nBGimageID;
	tVector2D m_nHeartStartPos;
	int m_nFoxCurHearts;

	float m_fHeartScale;
	float m_fToolScale;
	float m_fPowerupScale;
	float m_fBorderScale;

	CFox*		m_pFox;
	CCorinne*	m_pCorinne;

	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CFileManager*			m_pFM;
	CParticleEmitter*		m_pHeartPop;

	bool m_bIsDarkened;

	//CGamePlayState* m_pGPS;

	CHUD(void);
	CHUD(CHUD&);
	CHUD& operator=(CHUD&);
	~CHUD(void);
public:
	static CHUD* GetInstance();
	void Init(void);
	void Render(void);
	virtual void HandleEvent(const CEvent* pEvent);

	void SetDarkened(bool isDarkened)	{m_bIsDarkened = isDarkened;}
	bool GetDarkened(void)				{return m_bIsDarkened;}
};

