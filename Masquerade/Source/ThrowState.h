#pragma once
#include "BaseAI.h"
#include <vector>

class CThrowState : public CBaseAI
{
private:
	std::vector<OBJECT_TYPES> FleeFromObjTypes;
public:
	CThrowState(void);
	virtual~CThrowState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr );
	virtual void Exit(void);
	virtual void Update( float fElapsedTime );
	virtual void Render(void);
	virtual void NodeCollision( void );
	virtual void HandleEvent( CEvent* pEvent );

};

