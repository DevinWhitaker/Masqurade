#pragma once
#include "BaseAI.h"
//#include "ObjectManager.h"
//#include <vector>

class CSeekState : public CBaseAI
{
private:
	std::vector< IBaseObject* > m_vObjectsToCheck;
	std::vector< CBaseAI::sObjectStruct > m_vObjectsToSeek;
	float	m_fxDistance;
	float	m_fyDistance;
	bool	m_bUpdateVector;

public:

	CSeekState(void);
	virtual ~CSeekState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr );
	virtual void Exit(void) ;
	virtual void Update( float fElapsedTime ) ;
	virtual void Render(void) ;
	virtual void NodeCollision( void );
	virtual void HandleEvent( CEvent* pEvent );

	void	AddSeekType( int nBaseType, int nType = -100 );
	CObjectManager::sClosestObject	ReturnClosetObject( void );

	void	SetXDistance( float fDistance ) { m_fxDistance = fDistance;	}
	float	GetXDistance( void )	const	   { return m_fxDistance;	}

	void	SetYDistance( float fDistance ) { m_fyDistance = fDistance;	}
	float	GetYDistance( void )	const	   { return m_fyDistance;	}

	void	SetVectorUpdateBool( bool bUpdateVec ) { m_bUpdateVector = bUpdateVec; }
	bool	GetVectorUpdateBool( void ) const { return m_bUpdateVector; }

	bool	IsObjectInBeingSeeked( int nBaseObject, int nObjToCheckFor = -1 );
};

