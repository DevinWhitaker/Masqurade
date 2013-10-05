#pragma once
#include "IBaseObject.h"
#include "BaseObject.h"
#include "../SGD Wrappers/IListener.h"

//#include <Windows.h>

class IAIState : public IListener
{
public:
	enum AISTATE { NONE, IDLE, FLEE, FOLLOW, HELD, SEEK, THROW };
	virtual~IAIState(void) = 0 {}
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr ) = 0;
	virtual void Exit(void) = 0;
	virtual void Update( float fElapsedTime ) = 0;
	virtual void Render(void) = 0;
	virtual int GetAIType(void) = 0;
	
};

