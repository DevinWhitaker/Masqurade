#pragma once
#include "ITool.h"
class CSquirtGun :
	public ITool
{
public:
	CSquirtGun(void);
	virtual ~CSquirtGun(void);

	bool Shoot(CBaseObject* firingEntity);
	void Update(float fElapsed);
	void Render();
};

