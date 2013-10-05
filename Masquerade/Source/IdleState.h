#pragma once
#include "BaseAI.h"
#include "..\SGD Wrappers\IListener.h"
#include <vector>

//class CFrame;

class CIdleState : public CBaseAI
{
private:
	std::vector<OBJECT_TYPES>	m_vFleeFromObjTypes;
public:
	CIdleState(void);
	virtual~CIdleState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr );
	virtual void Exit(void);
	virtual void Update( float fElapsedTime );
	virtual void Render(void);
	virtual void HandleEvent( CEvent* pEvent );
	//virtual void NodeCollision( void );
};

