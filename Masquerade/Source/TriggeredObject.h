/*
	File:		TriggeredObject.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Base class for all objects being handle by triggers.
*/

#pragma once
#include "baseobject.h"

class CFrame;

class CTriggeredObject : public CBaseObject
{
private:
	bool	m_bHasBeenRegistered;
	bool	m_bisActivated;
	CFrame*	m_pFrame;
	void	SetRegisteredBool( bool bisRegistered ) { m_bHasBeenRegistered = bisRegistered; }
protected:
	int	m_nTriggeredType;
public:

	enum TriggeredObjectType { TRIGOBJ_BASE, TRIGOBJ_GATE, TRIGOBJ_SPRING, TRIGOBJ_BALLOON, TRIGOBJ_SPAWNPOINT, TRIGOBJ_MAX };

	CTriggeredObject(void);
	CTriggeredObject( CFrame*	pFrame, std::string szFilePath, 
		unsigned int unAnimationToUse,float fpositionX, float fpositionY, 
		bool bIsGravityApplied, int nLayerType, std::string szID, 
		float fGravityApplied = 0.0f );

	virtual ~CTriggeredObject(void);

	 void Activate(void);
	 void Deactivate(void);
	 void RegisterDefaultEvents( void );
	 void AddEvent( std::string szEventName/*, CTriggeredObject* pTrigger */  );
	 void QueueEvent( std::string szEventName/*, CTriggeredObject* pTrigger */  );
	 void PlayAnimation( TriggeredObjectType nTriggerType );
	 int GetTriggeredType( void ) { return m_nTriggeredType; }
	 virtual void HandleEvent( CEvent* pEvent );

	CFrame*	GetTriggeredObjectFrame( void )	const		{ return m_pFrame;		}
	void	SetTriggeredObjectFrame( CFrame* pFrame, std::string szFilePath, unsigned int unAnimationToUse = 0 );
	void	SetTriggeredObjectFrame( CFrame* pFrame )
	{ m_pFrame = pFrame; }


	bool	GetRegisteredBool( void ) { return m_bHasBeenRegistered; }

	void	SetActivatedBool( bool binUse ) { m_bisActivated = binUse; }
	bool	GetActivatedBool( void ) { return m_bisActivated; }
};

