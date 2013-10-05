#include "HeldState.h"
#include "BaseObject.h"
#include "Fox.h"
#include "FileManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "Camera.h"
#include "Game.h"
#include "Enemy.h"
#include "Hoodlum.h"
#include "Corinne.h"

CHeldState::CHeldState(void)
{
	GetObjectsInvolved().pCarried = nullptr;
	GetObjectsInvolved().pCarrier = nullptr;
	m_nAIType = HELD;
	SetArchValue( 0.0f );
	SetScalarX( 1.0f );
	SetScalarY( 1.0f );
}


CHeldState::~CHeldState(void)
{
	//Exit();
	//if( GetAttachedObject()->GetType() == OBJECT_TYPES::OBJ_CORINNE )
	//{
	//	CCorinne*	pCO	= dynamic_cast< CCorinne* >( GetAttachedObject() );
	//	CSGD_EventSystem::GetInstance()->UnregisterClient( "putdown", pCO );
	//	CSGD_EventSystem::GetInstance()->UnregisterClient( "throw", pCO );
	//}
}

void CHeldState::Enter( IBaseObject* pB, CBaseObject* pHeldBy )
{
	//CFileManager*			pFM = CFileManager::GetInstance();
	//CSGD_TextureManager*	pTM	= CSGD_TextureManager::GetInstance();
	//pFM->AddImageID( "Game_Cursor", pTM->LoadTexture( _T("Resources/Images/MenuImages/MASQ_GameCursor.png") ) );

	GetObjectsInvolved().pCarried = (CBaseObject*)pB;
	GetObjectsInvolved().pCarrier = pHeldBy;
	m_nAIType = HELD;
	SetArchValue( 0.0f );
	dynamic_cast< CBaseObject* >( GetObjectsInvolved().pCarried )->SetGravityBool( false );

	if( GetObjectsInvolved().pCarried->GetType() == OBJ_CORINNE )
	{
		CCorinne*	pCO	= dynamic_cast< CCorinne* >( GetObjectsInvolved().pCarried );
		CSGD_EventSystem::GetInstance()->RegisterClient( "putdown", pCO );
		CSGD_EventSystem::GetInstance()->RegisterClient( "throw", pCO );
	}
	else
	{
		//CSGD_EventSystem::GetInstance()->RegisterClient( "putdown", pCO );
		//CSGD_EventSystem::GetInstance()->RegisterClient( "throw",	pCO );
	}
}

void CHeldState::Exit(void)		
{
	CBaseAI::Exit();
	SetArchValue( 0.0f );
}
void CHeldState::Update( float fElapsedTime )
{
	CBaseAI::Update( fElapsedTime );
	CBaseObject*	pAttached = dynamic_cast< CBaseObject* >( GetObjectsInvolved().pCarried );

	if( GetObjectsInvolved().pCarrier->GetType() == OBJ_FOX )
	{
		CFox*			pFox	  = dynamic_cast< CFox*>( GetObjectsInvolved().pCarrier );

		if( pFox->GetFrame().isFlipped() == false )
			pAttached->SetX( pFox->GetX() );
		else
			pAttached->SetX( pFox->GetX() - 10.0f);
		pAttached->SetY( ( pFox->GetY()  - 45.0f)
			/* GetObjectsInvolved().pCarrier->GetY()*/ /*- GetHolder()->GetHeight() */);


		CSGD_DirectInput*	pDI	=	CSGD_DirectInput::GetInstance();

#if _DEBUG
		//if( pDI->MouseMovementX() > 0 && pDI->MouseMovementY() > 0)
		//{
		//	printf( "Mouse X: %i", pDI->MouseMovementX() );
		//	printf( "Mouse Y: %i \n", pDI->MouseMovementY() );
		//}
		/*else if( pDI->MouseMovementY() > 0 )
		printf( "Mouse Y: %i \n", pDI->MouseMovementY() );
		else if( pDI->MouseMovementX() > 0 )
		printf( "Mouse X: %i \n", pDI->MouseMovementX() );

		printf( "Arch: %f", GetArchValue() );*/
#endif

		if( pDI->MouseMovementY() == 0)
		{}
		else if( pDI->MouseMovementY() < 0 && GetArchValue() > -1.5f)
			SetArchValue( GetArchValue() -  0.01f );
		else if( pDI->MouseMovementY() > 0 && GetArchValue()  < 1.5f)
			SetArchValue( GetArchValue() + 0.01f );	
	}
	else if( GetObjectsInvolved().pCarrier->GetType() == OBJ_ENEMY )
	{
		CEnemy*			pEnemy	  = dynamic_cast< CEnemy*>( GetObjectsInvolved().pCarrier );

		if( pEnemy->getEnemyType() == NME_HOODLUM )
		{
			CHoodlum*			pHood	  = dynamic_cast< CHoodlum*>( pEnemy );
			if( pHood->GetFrame().isFlipped() == false )
				pAttached->SetX( pHood->GetX() );
			else
				pAttached->SetX( pHood->GetX() - 10.0f);
			pAttached->SetY( ( pHood->GetY()  - 45.0f) );
		}
	}
	else
	{
		pAttached->SetX( GetObjectsInvolved().pCarrier->GetX() / 2.0f);
		pAttached->SetY( GetObjectsInvolved().pCarrier->GetY() / 2.0f );
	}
}

void CHeldState::Render(void)
{
	CBaseObject*			pAttached = dynamic_cast< CBaseObject* >( GetObjectsInvolved().pCarried );
	CFileManager*			pFM = CFileManager::GetInstance();
	CSGD_TextureManager*	pTM	= CSGD_TextureManager::GetInstance();
	CCamera*				pCM	=	CCamera::GetInstance();

	if( m_ObjectsInvolved.pCarrier->GetType() == OBJ_FOX )
	{
		CFox*	pFox = dynamic_cast< CFox* >( GetObjectsInvolved().pCarrier );

		float fX	= pTM->GetTextureWidth( pFM->GetImageIdMap()["Game_Cursor"].nId ) / 2.0f;
		float fY	= pTM->GetTextureHeight( pFM->GetImageIdMap()["Game_Cursor"].nId ) / 2.0f;

		//if( pFox->GetFrame().GetAnimationID() == FoxAnimations::
		if( pFox->GetFrame().isFlipped() == false )
			pTM->Draw( pFM->GetImageIdMap()["Game_Cursor"].nId, 
			(int)( pAttached->GetX() - pCM->OffsetX() ) + 20,
			((int)(pAttached->GetY() - pCM->OffsetY()) - 30 ), 1.0f, 1.0f, 
			nullptr, fX, fY, GetArchValue(), 
			D3DCOLOR_ARGB( 200, 200, 255, 255) );
		else
			pTM->Draw( pFM->GetImageIdMap()["Game_Cursor"].nId, 
			(int)( pAttached->GetX() - pCM->OffsetX() ) - 20  ,
			(int)( ( pAttached->GetY() - pCM->OffsetY() )  ), 
			-1.0f, -1.0f, nullptr, fX, fY, 
			-GetArchValue(), D3DCOLOR_ARGB( 225, 200, 255, 50));

		//		if( pFox->GetFrame().isFlipped() == false )
		//	pTM->Draw( pFM->GetImageIdMap()["Game_Cursor"].nId, 
		//	(int)( pAttached->GetX() - pCM->OffsetX() ) + 20,
		//	((int)(pAttached->GetY() - pCM->OffsetY()) - 30 ), 1.0f, 1.0f, 
		//	nullptr, fX, fY, GetArchValue(), 
		//	D3DCOLOR_ARGB( 200, 200, 255, 255) );
		//else
		//	pTM->Draw( pFM->GetImageIdMap()["Game_Cursor"].nId, 
		//	(int)( pAttached->GetX() - pCM->OffsetX() ) - 70  ,
		//	(int)( ( pAttached->GetY() - pCM->OffsetY() )  ), 
		//	-1.0f, -1.0f, nullptr, fX, fY, 
		//	-GetArchValue(), D3DCOLOR_ARGB( 225, 200, 255, 50));

	}
	switch( GetObjectsInvolved().pCarrier->GetType() )
	{
	case OBJ_FOX:
		{
			CFox*	pFox = dynamic_cast< CFox* >( GetObjectsInvolved().pCarrier );
			if( pFox->GetFrame().isFlipped() == true )
			{
				if( pAttached->GetType() == OBJ_CORINNE )
				{
					CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pAttached );
					pCorinne->GetFrame().SetFlipped( true );
				}
			}
			else if( pFox->GetFrame().isFlipped() == false )
			{
				if( pAttached->GetType() == OBJ_CORINNE )
				{
					CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pAttached );
					pCorinne->GetFrame().SetFlipped( false );
				}
			}
		
		}
		break;
	case OBJ_ENEMY:
		{
			CEnemy*			pEnemy	  = dynamic_cast< CEnemy*>( GetObjectsInvolved().pCarrier );

			if( pEnemy->getEnemyType() == NME_HOODLUM )
			{
				CHoodlum*			pHood	  = dynamic_cast< CHoodlum*>( pEnemy );

				if( pHood->GetFrame().isFlipped() == true )
				{
					if( pAttached->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pAttached );
						pCorinne->GetFrame().SetFlipped( true );
					}
				}
				else if( pHood->GetFrame().isFlipped() == false )
				{
					if( pAttached->GetType() == OBJ_CORINNE )
					{
						CCorinne*	pCorinne = dynamic_cast< CCorinne* >( pAttached );
						pCorinne->GetFrame().SetFlipped( false );
					}
				}
			}
		}
		break;
	default:
		break;
	};
}

void CHeldState::NodeCollision( void )
{
	CBaseAI::NodeCollision();
}

