/*
	File:		Trigger.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	To trigger objects in the game world.
*/

#pragma once
#include "Baseobject.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
//#include <map>
//using namespace std;

//#include "Frame.h"
class CFrame;
class CTriggeredObject;

class CTrigger : public CBaseObject
{
private:
	std::map< std::string, CTriggeredObject* > m_mObjectBeingTriggered;	
	CFrame*	m_pTriggerFrame;
	bool	m_bIsInRangeToActivate;
	//std::map< std::string, CTriggeredObject* > &GetObjectMap( void )
	//{ return m_mObjectBeingTriggered; }
	bool	m_bIsActivated;
public:

	enum TRIGGERS { TRIG_BASE, TRIG_LEVER, TRIG_PLATE, TRIG_CAM, TRIG_MAX };
	CTrigger(void);
	virtual ~CTrigger(void);


	virtual void HandleEvent( const CEvent* pEvent );
	virtual bool CheckCollision(IBaseObject* pBase);

	virtual void Use( void );

	void AddObjectToTrigger( CTriggeredObject*	pTriggeredObject );
	void AddObjectToTrigger( CTriggeredObject*	pTriggeredObject,
								std::string szNameOfTriggeredObject );

	void RemoveObjectToTrigger( std::string szTriggeredObjectName );

	unsigned int GetTriggerType( void ) { return m_nTriggerType; }

	CFrame*	GetTriggerFrame( void )		 { return m_pTriggerFrame; }
	void	SetTriggerFrame( CFrame* pFrame, std::string szFilePath, 
								unsigned int unAnimationToUse = 0 );
	void	SetTriggerFrame( const char*  szFilePath, unsigned int unAnimationToUse = 0 );
	void	SetTriggerFrame( CFrame*	pFrame ) { m_pTriggerFrame = pFrame; }
	void	SetTriggerFrame( std::string szFilePath, 
		DWORD dwColor = D3DCOLOR_ARGB( 255, 255, 255, 255 ), 
		int nCurrentFrame = 0, bool bFacingRight = true, 
		bool bIsItPlaying = false, float fAnimationSpeed = 1.0f, 
		float fXScale = 1.0f, float fYScale = 1.0f, 
		unsigned int unAnimationToUse = 0 );


	bool	GetInRangeBool( void ) const	{ return m_bIsInRangeToActivate; }
	void	SetInRangeBool( bool bRange )	{ m_bIsInRangeToActivate = bRange; }

	bool	GetIsActivatedBool( void ) const	{ return m_bIsActivated; }
	void	SetIsActivatedBool( bool bActive)	{ m_bIsActivated = bActive; }
	void	SetMap( std::map< std::string, CTriggeredObject* > mMap )
	{  m_mObjectBeingTriggered = mMap; }
		std::map< std::string, CTriggeredObject* > &GetObjectMap( void )
		{ return m_mObjectBeingTriggered; }

protected:
	unsigned int m_nTriggerType;
};

