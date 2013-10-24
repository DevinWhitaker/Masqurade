#pragma once
#include "BaseAI.h"
#include <vector>

class CFleeState : public CBaseAI
{
private:
	std::vector< IBaseObject* > m_vFleeCheck;
	std::vector< CBaseAI::sObjectStruct > m_vFleeFromObjTypes;
public:

	CFleeState(void);
	virtual ~CFleeState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr );
	virtual void Exit(void) ;
	virtual void Update( float fElapsedTime ) ;
	virtual void Render(void) ;
	virtual void NodeCollision( void );
	virtual void HandleEvent( const CEvent* pEvent );
	void	AddFleeType( int nBaseType, int otType = -100 );

};

