/*
	File:		CBaseAI.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Base class for all ai states to inherit from.
*/

#pragma once
#include "IAIState.h"
#include "ObjectManager.h"
#include <map>
//#include <string>
using namespace std;

class CBaseAI : public IAIState
{
public:
	CBaseAI(void);
	CBaseAI( int nAIType, CBaseObject* pCarrier, CBaseObject* pCarried );
	virtual~CBaseAI(void);
	virtual void Enter( IBaseObject* pB, CBaseObject* pHeldBy = nullptr );
	virtual void Exit(void) ;
	virtual void Update( float fElapsedTime ) ;
	virtual void Render(void) ;
	virtual int GetAIType(void) ;
	virtual void NodeCollision( void );
	virtual void HandleEvent( CEvent* pEvent );

	struct sObjectStruct
	{
		int	nBaseObject;
		int	nTypeObject;
	};

	struct sNode
	{
		std::string		szNodeName;
		std::string		szNodeEvent;
		float			fNodePosX;
		float			fNodePosY;
		unsigned int	unNodeWidth;
		unsigned int	unNodeHeight;
		RECT			rNode;
		bool			bRegisterCarrierToEvent;
	};

	void SetAIType( int nAIType ) { m_nAIType = nAIType; }
	

	CBaseObject::sDataToPassIn &GetObjectsInvolved( void ) { return m_ObjectsInvolved; }
	void	SetObjectsInvolved( CBaseObject::sDataToPassIn &transfer ) { m_ObjectsInvolved = transfer; }

	float	&GetScalarX( void ) { return m_fScalarX; }
	float	&GetScalarY( void ) { return m_fScalarY; }

	void	SetScalarX( float fScalarX ) { m_fScalarX = fScalarX; }
	void	SetScalarY( float fScalarY ) { m_fScalarY = fScalarY; } 


protected:
	CBaseObject::sDataToPassIn	m_ObjectsInvolved;
	int m_nAIType;
private:
	std::map< std::string, sNode > m_mAINodeMap;
	float	m_fScalarX;
	float	m_fScalarY;
};