/*
	File:		FollowState.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Implements the following behavior for ai.
*/

#pragma once
#include "BaseAI.h"

class CFollowState : public CBaseAI
{
private:
	//OBJECT_TYPES FollowObjType;
	float	m_fDistance;
	bool	m_bInRange;
	bool	m_bCarriedFollowing;
public:
	CFollowState(void);
	virtual ~CFollowState(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy, bool bCarriedMoving = true );
	virtual void Exit(void);
	virtual void Update( float fElapsedTime );
	virtual void Render(void);
	virtual void NodeCollision( void );
	virtual void HandleEvent( const CEvent* pEvent );

	float	&GetDistance( void ) { return m_fDistance; }
	void	SetDistance( float fDistance ) { m_fDistance = fDistance; }
	
	bool	&GetRangeBool( void ) { return m_bInRange; }
	void	SetRangeBool( bool bRange ) { m_bInRange = bRange; }
};

