#pragma once

#include "BaseObject.h"
#include <map>
#include <vector>
using namespace std;

enum CREATEABLE { BALLOON, BALLOON_GUN, CHECKPOINT, CLOWN, COTTON_LAUNCHER, CRATE, GATE, HOODLUM, LEVER, POWERUP, PRESSURE_PLATE, RAT, SECURITY_CAMERA, SPAWN_POINT, SPRING, SQUIRT_GUN, TILE, MAX_CREATEABLES };

class CObjectFactory
{

public:
							///////////////////////////////////////////////////////////
							//	Purpose:	Allows access to this singleton
							//
							//	In:			None
							//
							//	Return:		An instance of this object
							///////////////////////////////////////////////////////////
	static CObjectFactory*	GetInstance();

							///////////////////////////////////////////////////////////
							//	Purpose:	Setup an object to be copied from
							//
							//	In:			
							//				MasterCopy: A dynamically allocated instance of the class
							//				Tag: The enumeration indicating what type of class to register
							//
							//	Return:		True if the class is successfully registered
							///////////////////////////////////////////////////////////
	bool					RegisterClass( CBaseObject& MasterCopy, CREATEABLE TAG );

							///////////////////////////////////////////////////////////
							//	Purpose:	Releases the memory of the selected class
							//
							//	In:			The enumeration indicating which class to unregister
							//
							//	Return:		True if the class is successfully unregistered
							///////////////////////////////////////////////////////////
	bool					UnregisterClass( CREATEABLE Tag );

							///////////////////////////////////////////////////////////
							//	Purpose:	Dynamically instantiate an object of the intended class
							//
							//	In:			The enumeration indicating the object to create
							//
							//	Return:		A pointer to the newly created object
							///////////////////////////////////////////////////////////
	CBaseObject*			CreateObject( CREATEABLE Tag );

							///////////////////////////////////////////////////////////
							//	Purpose:	Release all the memory within the class 
							//
							//	In:			None
							//
							//	Return:		None
							///////////////////////////////////////////////////////////
	void					ShutdownObjectFactory();

private:

							///////////////////////////////////////////////////////////
							//	Purpose:	private constructor
							//
							//	In:			None
							//
							//	Return:		None
							///////////////////////////////////////////////////////////
							CObjectFactory(void);

							///////////////////////////////////////////////////////////
							//	Purpose:	private copy constructor
							//
							//	In:			None
							//
							//	Return:		None
							///////////////////////////////////////////////////////////
							CObjectFactory(const CObjectFactory&);

							///////////////////////////////////////////////////////////
							//	Purpose:	private assignment operator
							//
							//	In:			const CObjectFactory reference
							//
							//	Return:		CObjectFactory reference
							///////////////////////////////////////////////////////////
							CObjectFactory& operator=(const CObjectFactory&);

							///////////////////////////////////////////////////////////
							//	Purpose:	private destructor
							//
							//	In:			None
							//
							//	Return:		None
							///////////////////////////////////////////////////////////
							~CObjectFactory(void);

							//vector<IBaseObject*> m_vCloneFrom;

							CBaseObject* m_pCloneFrom[MAX_CREATEABLES];	//	Array holding all registerd classes

protected:
	
};

