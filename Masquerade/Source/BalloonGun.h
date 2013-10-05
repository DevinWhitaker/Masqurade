#pragma once
#include "ITool.h"

class CCrate;

class CBalloonGun :
	public ITool
{
private:
	int m_nBalloonsAttached;
	float m_fMaxAttachDistance;

	//CCrate* m_pCratesWithBaloons[3];	
	//float m_fTimeBeforeDetach;


public:
	CBalloonGun(void);
	virtual ~CBalloonGun(void);

	bool Shoot(CBaseObject* firingEntity);

	void Update(float fElapsed);
	void Render();

	

};

