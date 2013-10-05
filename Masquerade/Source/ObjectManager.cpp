/*
	File:		ObjectManager.cpp
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Contain & Manager all game objects
*/ 

#include "ObjectManager.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Trigger.h"
#include "TriggeredObject.h"
#include "Powerup.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Camera.h"

typedef std::vector<IBaseObject*>::iterator iterator;

// SINGLETON:

// Intantiate the static pointer data member
CObjectManager* CObjectManager::s_pInstance = nullptr;

//Return the singleton instance (allocate if needed)
CObjectManager* CObjectManager::GetInstance(void)
{
	if(s_pInstance == nullptr)
		s_pInstance = new CObjectManager;

	return s_pInstance;
}
// Deallocate the singleton instance
void CObjectManager::DeleteInstance(void)
{
	if(s_pInstance != nullptr)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}

// Constructor
CObjectManager::CObjectManager(void)
{
	m_pTileManager = CTileManager::GetInstance();
}

// Destructor
CObjectManager::~CObjectManager(void)
{
}

// Object Interaction Methods:
void CObjectManager::UpdateAllObjects(float fElapsedTime)
{
	// Call the Update method of all objects in the list'
	iterator iter;
	CCamera* cam = CCamera::GetInstance();

#if _DEVIN
	int i = 0;
#endif

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		if ( (*iter)->GetType() == OBJ_BALLOON )
			int foo = 0;
		//Dont cull fox or corinne's update
		if( (*iter)->GetType() != OBJ_FOX && (*iter)->GetType() != OBJ_CORINNE && (*iter)->GetType() != OBJ_TOOL && (*iter)->GetType() != OBJ_BALLOON
			&& (*iter)->GetType() != OBJ_NPC )
		{
		//Culling
		if( (((CBaseObject*)(*iter))->GetX() + ((CBaseObject*)(*iter))->GetWidth()) <= -50.0f || ((CBaseObject*)(*iter))->GetX() > (cam->OffsetX() + cam->GetScreenSpaceX() + 50.0f)
			|| (((CBaseObject*)(*iter))->GetY() + ((CBaseObject*)(*iter))->GetHeight()) <= -50.0f || ((CBaseObject*)(*iter))->GetY() > (cam->OffsetY() + cam->GetScreenSpaceY() + 50.0f) )
			continue;
		}

		(*iter)->Update(fElapsedTime);
		
#if _DEVIN
			i++;
#endif
		// WARNING!!!
		// Do NOT remove an object while iterating
	}

#if _DEVIN
	printf("%i objects updated\n",i);
#endif
}



void CObjectManager::SetGlobalTimeScalar(float m_fScalar)
{
	iterator iter;

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		(*iter)->SetScalar(m_fScalar);
	}
}

void CObjectManager::CheckCollisions( void )
{
	iterator iter;
	iterator iterj;
	CCamera* cam = CCamera::GetInstance();

#if _DEVIN
	int i = 0;
#endif

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		for( iterj = m_vObjectList.begin(); iterj != m_vObjectList.end(); ++iterj)
		{
			//Culling
			if( abs( ( (CBaseObject*)*iter )->GetX()  - ( (CBaseObject*)*iterj )->GetX() ) > cam->GetScreenSpaceX() / 2 ) 
				continue;

			if( (*iter)->GetType() != (*iterj)->GetType() || 
				((*iter)->GetType() == OBJ_CRATE && 
				(*iterj)->GetType() == OBJ_CRATE && 
				(*iter) != (*iterj) ))
			{
				if( (*iter)->CheckCollision((*iterj)) ) {}
					//break;
			}

			// Tile Manager Collisions
			m_pTileManager->CheckTileCollisions( (CBaseObject*)(*iter) );

#if _DEVIN
			i++;
#endif
		}
	}

#if _DEVIN
	printf("%i Collisions Checked\n",i);
#endif
}

void CObjectManager::RenderAllObjects(void)
{
	// Call the Update method of all objects in the list'
	iterator iter;
	CCamera* cam = CCamera::GetInstance();

#if _DEVIN
	int i = 0;
#endif

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		if( (*iter)->GetType() != OBJ_CORINNE )
		{
			if( (((CBaseObject*)(*iter))->GetX() + ((CBaseObject*)(*iter))->GetWidth()) <= 0.0f || ((CBaseObject*)(*iter))->GetX() > (cam->OffsetX() + cam->GetScreenSpaceX())
				|| (((CBaseObject*)(*iter))->GetY() + ((CBaseObject*)(*iter))->GetHeight()) <= 0.0f || ((CBaseObject*)(*iter))->GetY() > (cam->OffsetY() + cam->GetScreenSpaceY()))
				continue;
		}
		(*iter)->Render();

#if _DEVIN
		i++;
#endif

		// WARNING!!!
		// Do NOT remove an object while iterating
	}

#if _DEVIN
	printf("rendered %i objects\n",i);
#endif
}

void CObjectManager::AddObject( IBaseObject* pObject)
{
	// Sanity-check
	if( pObject == nullptr )
		return;

	// Add a reference to the object
	pObject->AddRef();

	// Add object to the list
	m_vObjectList.push_back( pObject );

}

void CObjectManager::RemoveObject( IBaseObject* pObject)
{
	// Sanity-check
	if( pObject == nullptr )
		return;

	iterator iter;

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		if( (*iter) == pObject )
		{
			// Release our reference to the object
			(*iter)->Release();

			// Remove the object from the list
			iter = m_vObjectList.erase( iter );
			break;
		}
	}

}

void CObjectManager::RemoveAllObjects(void)
{
	// Rlease ever object in the list
	iterator iter;

	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
			// Release our reference to the object
			(*iter)->Release();
	}

	// Collapse the vector
	m_vObjectList.clear();
}

std::vector<IBaseObject*>  CObjectManager::FindObjectsWithType( int nBaseObjType, int eObjType  )
{
	iterator iter;
	std::vector<IBaseObject*> vReturnVector;
	for( iter = m_vObjectList.begin(); iter != m_vObjectList.end(); ++iter)
	{
		if( (*iter) != nullptr && (*iter)->GetType() == nBaseObjType )
		{
			switch( (*iter)->GetType() )
			{
			case OBJ_PUZZLE:
				{
					CTriggeredObject*	pTriggered = dynamic_cast< CTriggeredObject* >( (*iter) );
					if( pTriggered->GetTriggeredType() == eObjType )
						vReturnVector.push_back( (*iter) );
				};
				break;
			case OBJ_ENEMY:
				{
					CEnemy*	pEnemy = dynamic_cast< CEnemy* >( (*iter) );
						if( pEnemy->getEnemyType() == eObjType )
							vReturnVector.push_back( (*iter) );
				}
				break;
			case OBJ_POWERUP:
				{
					CPowerup*	pPowerup = dynamic_cast< CPowerup* >( (*iter) );
					if( pPowerup->GetPowerupType() == eObjType )
						vReturnVector.push_back( (*iter) );
				}
				break;
			case OBJ_TRIGGER:
				{
					CTrigger*	pTrigger = dynamic_cast< CTrigger* >( (*iter) );
					if( pTrigger->GetTriggerType() == eObjType )
						vReturnVector.push_back( (*iter) );
				}
				break;
			case OBJ_BULLET:
				{
					CBullet*	pBullet = dynamic_cast< CBullet* >( (*iter) );

					if( pBullet->getBulletType() == eObjType)
						vReturnVector.push_back( (*iter) );
				}
				break;
			default:
				{
					CBaseObject*	pObject = dynamic_cast< CBaseObject* >( (*iter) );
					if( pObject->GetType() == nBaseObjType )
						vReturnVector.push_back( (*iter) );
				};
				break;
			};
		}
	}

	return vReturnVector;
}

IBaseObject* CObjectManager::ReturnClosestObjectType(int nBaseObjType, 
	int nObjType, float fBaseObjectsXPos, float fBaseObjectsYPos, 
	int nXDistanceToCheck, int nYDistanceToCheck  )
{
	std::vector< sClosestObject > vDistanceSorting;
	std::vector< float > vFloatSort;

	for( unsigned i = 0; i < m_vObjectList.size(); ++i)
	{
		bool bAllowIntoSorting = true;


		switch( m_vObjectList[i]->GetType() )
		{
		case OBJ_ENEMY:
			{
				CEnemy*	pEnemy = dynamic_cast< CEnemy* >( m_vObjectList[i] );
				if( pEnemy->getEnemyType() != nObjType ||
					pEnemy->GetType() != nBaseObjType)
					bAllowIntoSorting = false;
			}
			break;
		case OBJ_BULLET:
			{
				CBullet*	pBullet = dynamic_cast< CBullet* >( m_vObjectList[i] );
				if( pBullet->getBulletType() != nObjType ||
					pBullet->GetType() != nBaseObjType )
					bAllowIntoSorting = false;
			}
			break;
		case OBJ_TRIGGER:
			{
				CTrigger*	pTrigger = dynamic_cast< CTrigger* >( m_vObjectList[i] );
				if( pTrigger->GetTriggerType() != nObjType ||
					pTrigger->GetType() != nBaseObjType)
					bAllowIntoSorting = false;
			}
			break;
		case OBJ_PUZZLE:
			{
				CTriggeredObject*	pTriggeredObject = dynamic_cast< CTriggeredObject* >( m_vObjectList[i] );
				if( pTriggeredObject->GetTriggeredType() != nObjType ||
					pTriggeredObject->GetType() != nBaseObjType)
					bAllowIntoSorting = false;
			}
			break;
		case OBJ_POWERUP:
			{
				CPowerup*	pPowerup = dynamic_cast< CPowerup* >( m_vObjectList[i]  );
					if( pPowerup->GetPowerupType() != nObjType ||
						pPowerup->GetType() != nBaseObjType )
						bAllowIntoSorting = false;
			}
			break;
		default:
			{
				if( m_vObjectList[i]->GetType() != nBaseObjType )
					bAllowIntoSorting = false;
			}
			break;
		};

		if( bAllowIntoSorting == true )
		{
			CBaseObject*	pBaseObject = dynamic_cast< CBaseObject* >( m_vObjectList[i] );
			if( fBaseObjectsXPos >= pBaseObject->GetX() - nXDistanceToCheck &&
				fBaseObjectsXPos <= pBaseObject->GetX() + nXDistanceToCheck &&
				fBaseObjectsYPos >= pBaseObject->GetY() - nYDistanceToCheck &&
				fBaseObjectsYPos <= pBaseObject->GetY() + nYDistanceToCheck  )
			{

				sClosestObject sData = {};

				float fxDistance = pow( nXDistanceToCheck - pBaseObject->GetX(), 2.0f );
				float fyDistance = pow( nYDistanceToCheck - pBaseObject->GetY(), 2.0f );
				float fTotal = fxDistance + fyDistance;
				float fDistanceFormula = sqrt( fTotal );

				sData.fDistance = fDistanceFormula;
				//sData.fXRange = nXDistanceToCheck;
				//sData.fYRange = nYDistanceToCheck;
				sData.nPosInVec = i;
				vFloatSort.push_back( sData.fDistance );
				vDistanceSorting.push_back( sData );
				//if( pBaseObject->GetX() > fBaseObjectsXPos )
				//{
				//	sClosestObject sData = {};
				//	sData.fRange = pBaseObject->GetX() - fBaseObjectsXPos ;
				//	sData.nPosInVec = i;
				//	vFloatSort.push_back( sData.fRange );
				//	vDistanceSorting.push_back( sData );
				//}
				//else if( pBaseObject->GetX() < fBaseObjectsXPos )
				//{
				//	sClosestObject sData;
				//	sData.fRange = fBaseObjectsXPos - pBaseObject->GetX();
				//	sData.nPosInVec = i;
				//	vFloatSort.push_back( sData.fRange );
				//	vDistanceSorting.push_back( sData );
				//}
			}
		}
	}

		sort( vFloatSort.begin(), vFloatSort.end() );

		if( vDistanceSorting.size() != 0 )
		{
			for( unsigned i = 0; i < vDistanceSorting.size(); ++i )
			{
				if( vFloatSort[0] == vDistanceSorting[i].fDistance )
					return m_vObjectList[ vDistanceSorting[i].nPosInVec ];
			}
		}

		return nullptr;
	}
