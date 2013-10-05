#pragma once
#include "BaseAI.h"
//#include <vector>

class CHeldState : public CBaseAI
{
private:
	float			m_fArch;
public:

	CHeldState(void);
	virtual ~CHeldState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy );
	virtual void Exit(void);
	virtual void Update( float fElapsedTime );
	virtual void Render(void);
	virtual void NodeCollision( void );

	float			&GetArchValue( void )			{ return m_fArch;	}
	void			SetArchValue( float fArch )		{ m_fArch = fArch;	}
};

