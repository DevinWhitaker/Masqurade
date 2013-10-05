#pragma once
#include "ITool.h"
class CCottonCandyLauncher :
	public ITool
{
public:
	CCottonCandyLauncher(void);
	virtual ~CCottonCandyLauncher(void);

	bool Shoot(CBaseObject* firingEntity);
	void Update(float fElapsed);
	void Render();
};

