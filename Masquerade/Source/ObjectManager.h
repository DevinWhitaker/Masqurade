/*
	File:		ObjectManager.h
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Contain & Manager all game objects
*/ 

#pragma once

#include "IBaseObject.h"
//#include <vector>
#include "TileManager.h"

class CObjectManager
{
public:
	static CObjectManager* GetInstance(void);
	static void DeleteInstance(void);

	// Object Interaction Methods:
	void UpdateAllObjects(float fElapsedTime);
	void RenderAllObjects(void);
	
	void CheckCollisions( void );

	void AddObject( IBaseObject* pObject);
	void RemoveObject( IBaseObject* pObject);

	void RemoveAllObjects(void);
	void SetGlobalTimeScalar(float m_fScalar);

	std::vector<IBaseObject*> FindObjectsWithType( int nBaseObjType, int eObjType = -100 );
	IBaseObject* ReturnClosestObjectType( int nBaseObjType, int nObjType = -1, 
		float fBaseObjectsXPos = 32.0f, float fBaseObjectsYPos = 64.0f, 
		int nDistanceToCheck = 0, int nYDistanceToCheck = 0 );

	struct sClosestObject
	{
		int		nPosInVec;
		float	fXRange;
		float	fYRange;
		float	fDistance;
	};

private:
	// SINGLETON!!!!!!!!
	static CObjectManager* s_pInstance;

	CObjectManager(void);
	CObjectManager( const CObjectManager& );
	CObjectManager& operator=( const CObjectManager& );
	~CObjectManager(void);

	// List of objects
	std::vector<IBaseObject*>	m_vObjectList;

	CTileManager*	m_pTileManager;
};

