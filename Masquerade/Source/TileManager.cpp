// Project:		Masquerade
// Filename:	TileManager.cpp
// Author:		Jam'D Team
// Purpose:		Tile Manager implementation.

#include "TileManager.h"
#include "FileManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "AnimationManager.h"
#include "Balloon.h"
#include "BaseAI.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../tinyxml/tinyxml.h"
#include "Checkpoint.h"
#include "Rat.h"
#include "Hoodlum.h"
#include "Clown.h"
#include "Bullet.h"
#include "ObjectFactory.h"
#include "Fox.h"
#include "Corinne.h"
#include "ITool.h"
#include "SquirtGun.h"
#include "CottonCandyLauncher.h"
#include "BalloonGun.h"
#include "IdleState.h"
#include "GamePlayState.h"
#include "Gate.h"
#include "PressurePlate.h"
#include "Spring.h"
#include "Lever.h"
#include "Balloon.h"
#include "Crate.h"
#include "SecurityCamera.h"
#include "SpawnPoint.h"
#include "Enemy.h"
#include "NPC.h"

CTileManager* CTileManager::GetInstance( )
{
	static CTileManager s_Instance;

	return &s_Instance;
}

CTileManager::CTileManager( )
{

}

CTileManager::~CTileManager( )
{

}

void CTileManager::RegisterObjects()
{
	CObjectFactory*	m_pOF			=	CObjectFactory::GetInstance();
	// Create and Register 

	// Checkpoint Object
	CCheckpoint*	p_tCP	=	new CCheckpoint();
	m_pOF->RegisterClass( *p_tCP, CHECKPOINT );
	p_tCP->Release();

	// Tools //////////////////////////////////////////
	ITool*	pSQUIRTGUN	=	new CSquirtGun();
	m_pOF->RegisterClass( *pSQUIRTGUN, SQUIRT_GUN );
	pSQUIRTGUN->Release();

	ITool*	pCOTTONCANDY	=	new CCottonCandyLauncher();
	m_pOF->RegisterClass( *pCOTTONCANDY, COTTON_LAUNCHER );
	pCOTTONCANDY->Release();

	ITool*	pBALLOONGUN	=	new CBalloonGun();
	m_pOF->RegisterClass( *pBALLOONGUN, BALLOON_GUN );
	pBALLOONGUN->Release();
	////////////////////////////////////////////////////

	// Powerups ////////////////////////////////////////
	// Greased Pig, Pogo, Cheetah, Flubber, and Fizzy Powerup Objects
	CPowerup* pPOWERUP = new CPowerup();
	m_pOF->RegisterClass( *pPOWERUP, POWERUP );
	pPOWERUP->Release();
	////////////////////////////////////////////////////

	// Enemies /////////////////////////////////////////
	CRat* pCRAT = new CRat();
	m_pOF->RegisterClass( *pCRAT, RAT );
	pCRAT->Release();

	CHoodlum* pCHOOD = new CHoodlum();
	//MOOSE YOU NEED TO PASS FOX AND CORINNE INTO THESE FUNCTIONS TO FIX SPAWNPOINTS
	//pCHOOD->SetFox();pCHOOD->SetCorinne();
	m_pOF->RegisterClass( *pCHOOD, HOODLUM );
	pCHOOD->Release();

	CClown* pCCLOWN = new CClown();
	//MOOSE YOU NEED TO PASS FOX AND CORINNE INTO THESE FUNCTIONS TO FIX SPAWNPOINTS
	//pCCLOWN->SetFox();pCCLOWN->SetCorinne();
	m_pOF->RegisterClass( *pCCLOWN, CLOWN );
	pCCLOWN->Release();
	/////////////////////////////////////////////////////

	// Puzzles //////////////////////////////////////////
	CGate* pCGATE = new CGate();
	m_pOF->RegisterClass( *pCGATE, GATE );
	pCGATE->Release();
	
	CPressurePlate* pCPLATE = new CPressurePlate();
	m_pOF->RegisterClass( *pCPLATE, PRESSURE_PLATE );
	pCPLATE->Release();

	CSpring* pCSPRING = new CSpring();
	m_pOF->RegisterClass( *pCSPRING, SPRING );
	pCSPRING->Release();

	CLever* pCLEVER = new CLever();
	m_pOF->RegisterClass( *pCLEVER, LEVER );
	pCLEVER->Release();

	CBalloon* pCBALLOON = new CBalloon();
	m_pOF->RegisterClass( *pCBALLOON, BALLOON );
	pCBALLOON->Release();

	CCrate* pCCRATE = new CCrate();
	m_pOF->RegisterClass( *pCCRATE, CRATE );
	pCCRATE->Release();

	CSecurityCamera* pCSECURITY = new CSecurityCamera();
	m_pOF->RegisterClass( *pCSECURITY, SECURITY_CAMERA );
	pCSECURITY->Release();
	/////////////////////////////////////////////////////

}

bool CTileManager::CheckTileCollisions( CBaseObject* pBase )
{
	RECT trect = pBase->GetRect();
	int top, left, right, bottom;
	top = ( trect.top / 32 ) - 1;
	left = ( trect.left / 32 ) - 1;
	right = ( trect.right / 32 ) + 1;
	bottom = ( trect.bottom / 32 ) + 1;

	for ( int x = left; x <= right; ++x )
	{
		for ( int y = top; y <= bottom; ++y ) 
		{
			if( (size_t)x < m_vTileList.size() && x >= 0 )
			{
				if( (size_t)y < m_vTileList[ x ].size() && y >= 0 )
				{
					if( m_vTileList[ x ][ y ] == nullptr ) 
						continue;

					// Collide Type to Check Collision
					if( m_vTileList[ x ][ y ]->GetTileType() == COLLIDE )
						m_vTileList[ x ][ y ]->CheckCollision( pBase );

					// Gravity Tile Type to flip Gravity back on
					if( m_vTileList[ x ][ y ]->GetTileType() == GRAVITYON && pBase->GetType() != OBJ_BULLET )
					{
						pBase->SetGravityBool(true);
						pBase->SetRightBool( false );
						pBase->SetLeftBool( false );
					}
						
					//if( pBase->GetType() == OBJ_BULLET )
					//{
					//	CBullet* pie = (CBullet*)pBase;
					//	
					//	if( pie->getBulletType() == PIE_BULLET /*&& !pie->HasPieLanded()*/ )
					//	{
					//		CSGD_EventSystem::GetInstance()->SendEvent( "Pie Landed", pBase );
					//		/*pie->PieLanded();*/
					//	}
					//}
					if( pBase->GetType() == OBJ_BULLET )
					{
						CBullet* pie = (CBullet*)pBase;
						
						if( pie->getBulletType() == PIE_BULLET /*&& !pie->HasPieLanded()*/ )
						{
							CSGD_EventSystem::GetInstance()->SendUniqueEvent( "Pie Landed", pBase );
							break;
							/*pie->PieLanded();*/
						}
					}
					

					if( pBase->GetType() == OBJ_CORINNE || pBase->GetType()  == OBJ_ENEMY )
					{
						bool bSafe = true;
						if(pBase->GetType() == OBJ_ENEMY)
						{
							CEnemy* e = dynamic_cast<CEnemy*>(pBase);
							if(e->getEnemyType() == NME_RAT)
								bSafe = false;
						}
						
						if(bSafe)
						{
							if( m_vTileList[ x ][ y ]->GetTileType() == JUMP_NODE && CSGD_EventSystem::GetInstance()->HasEventTriggered("JUMP") == false )
							{
								if( pBase->GetJumpBool() != true )
								{
									CSGD_EventSystem::GetInstance()->SendUniqueEvent( "JUMP", pBase->GetCurrentState() );
									pBase->SetJumpBool( true );								
									pBase->SetGravityBool(true);
								}
							}

							if( m_vTileList[ x ][ y ]->GetTileType() == STOP_NODE && CSGD_EventSystem::GetInstance()->HasEventTriggered("STOP") == false)
								CSGD_EventSystem::GetInstance()->SendUniqueEvent( "STOP", pBase->GetCurrentState() );
						}
					}
				}
			}
		}
	}

	return false;
}

void CTileManager::Render( )
{
	CSGD_Direct3D*	pD3D		= CSGD_Direct3D::GetInstance();
	CCamera*		pCamera		= CCamera::GetInstance();
	CSGD_TextureManager* m_pTM	= CSGD_TextureManager::GetInstance();

	nRenderCounter = 0;

	for ( unsigned int x = 0; x < m_vTileList.size(); ++x )
	{
		for ( unsigned int y = 0; y < m_vTileList[ x ].size(); ++y ) 
		{
			if( m_vTileList[ x ][ y ] == nullptr )
				continue;

			// Culling Tiles based off of screen space.
			if( m_vTileList[ x ][ y ]->GetX() < (pCamera->GetScreenSpaceX() + pCamera->OffsetX()) &&
				(m_vTileList[ x ][ y ]->GetX() + m_vTileList[ x ][ y ]->GetWidth()) > pCamera->OffsetX() )
			{
				if( m_vTileList[ x ][ y ]->m_nTileX != -1 && m_vTileList[ x ][ y ]->m_nTileY != -1 )
				{
					if( m_vTileList[ x ][ y ]->GetLayerType() == MID_LAYER )
						m_pTM->Draw( m_vTileList[ x ][ y ]->GetImageID(), 
						(int)(m_vTileList[ x ][ y ]->GetX() - pCamera->OffsetX()) /** 0.55f*/, (int)(m_vTileList[ x ][ y ]->GetY() - pCamera->OffsetY()), 
							1.0f, 1.0f, &m_vTileList[ x ][ y ]->GetSourceRect(), 0, 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ) );
					else if( m_vTileList[ x ][ y ]->GetLayerType() == PLAY_LAYER )
					{
						m_pTM->Draw( m_vTileList[ x ][ y ]->GetImageID(), (int)(m_vTileList[ x ][ y ]->GetX() - pCamera->OffsetX()), (int)(m_vTileList[ x ][ y ]->GetY() - pCamera->OffsetY()), 
							1.0f, 1.0f, &m_vTileList[ x ][ y ]->GetSourceRect(), 0, 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ) );
					}
					
					nRenderCounter++;
				}
			}
		}
	}
}

void CTileManager::RenderFront( )
{
	CCamera*		pCamera		= CCamera::GetInstance();
	CSGD_TextureManager*	m_pTM			= CSGD_TextureManager::GetInstance();
	// Tiles Being Render Reset
	nRenderCounter = 0;
	
	for ( unsigned int x = 0; x < m_vTileList.size(); ++x )
	{
		for ( unsigned int y = 0; y < m_vTileList[ x ].size(); ++y ) 
		{
			if( m_vTileList[ x ][ y ] == nullptr )
				continue;

			// Culling Tiles based off of screen space.
			if( ( m_vTileList[ x ][ y ]->GetX() ) < pCamera->GetScreenSpaceX() + pCamera->OffsetX() &&
				( m_vTileList[ x ][ y ]->GetX() ) > -( m_vTileList[ x ][ y ]->GetX()) + pCamera->OffsetX() )
			{
				if( m_vTileList[ x ][ y ]->m_nTileX != -1 && m_vTileList[ x ][ y ]->m_nTileY != -1 )
				{
					if( m_vTileList[ x ][ y ]->GetLayerType() == FRONT_LAYER )
					{
						m_pTM->Draw( m_vTileList[ x ][ y ]->GetImageID(), 
							(int)((m_vTileList[ x ][ y ]->GetX() )/** 2.0 ) - ( ( m_vTileList[ x ].capacity() * 32) / 2)*/ - pCamera->OffsetX()) /** 1.5f*/, 
							(int)((m_vTileList[ x ][ y ]->GetY())  - pCamera->OffsetY())/**2.0) - 496*/, 
							1.0f, 1.0f, &m_vTileList[ x ][ y ]->GetSourceRect(), 0, 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ) );
					}
				}
				nRenderCounter++;
			}
		}
	}
}

void CTileManager::RenderBack( )
{
	CCamera*				pCamera		= CCamera::GetInstance();
	CSGD_TextureManager*	pTM			= CSGD_TextureManager::GetInstance();
	pTM->Draw( m_mImageList[ "Background" ].GetImageID(), (int)(m_mImageList[ "Background" ].GetX() - pCamera->OffsetX() * 0.25f), (int)(m_mImageList[ "Background" ].GetY()- pCamera->OffsetY()), 
					1.0f, 1.0f, &m_mImageList[ "Background" ].GetSourceRect(), 0, 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ) );
}

IBaseObject* CTileManager::GetTile( const int& iX, const int& iY )
{
	if( ( iX < 0 || (unsigned int)iX > m_vTileList.size() ) && ( iY < 0 || (unsigned int)iY > m_vTileList[ 0 ].size() ) )
		return m_vTileList[ iX ][ iY ];
	return nullptr;
}

void CTileManager::AddTile( CTile& pTile )
{
	pTile.AddRef();
	if( ( unsigned int )pTile.GetTileX( ) >= m_vTileList.size( ) )
		m_vTileList.resize(pTile.GetTileX( ) + 1 );

	if( ( unsigned int )pTile.GetTileY( ) >= m_vTileList[ pTile.GetTileX( ) ].size( ) )
		m_vTileList[ pTile.GetTileX( ) ].resize( pTile.GetTileY( ) + 1 );

	m_vTileList[ pTile.GetTileX( ) ][ pTile.GetTileY( ) ] = &pTile;
}

bool CTileManager::LoadTileWorld( std::string filename, int nLevelNumber )
{
	TiXmlDocument doc;

	if( doc.LoadFile( filename.c_str() ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return false;

	//TiXmlElement* pLeveler = pRoot->FirstChildElement( "Level" );

	while( pRoot != nullptr )
	{
		CTile* t = new CTile;

		std::string buffer;
		std::wostringstream woss;

		buffer = pRoot->Attribute( "Name" );
		int nCurrLevel = atoi( buffer.c_str() );

	/*	if( nCurrLevel != nLevelNumber )
			continue;*/

		TiXmlElement* ut = pRoot->FirstChildElement( "Unique_Tiles" );

		// I might have to make Rect for the Image temporarily to make Image IDs for each tile.
		t->m_szFilename = ut->Attribute( "IMGFileName" );

		woss << "Resources/Images/Graphics/" << t->m_szFilename.c_str();
		

		buffer = ut->Attribute( "Tile_Widths" );
		int tileWidth = atoi(buffer.c_str() ); 
		t->SetWidth( atoi(buffer.c_str() ) );


		buffer = ut->Attribute( "Tile_Heights" );
		int tileHeight = atoi(buffer.c_str() ); 
		t->SetHeight( atoi(buffer.c_str() ) );

		
		TiXmlElement* u = ut->FirstChildElement( "Tile" );

		while( u != nullptr )
		{
			if( t == nullptr )
			{
				t = new CTile;
				t->SetWidth(tileWidth);
				t->SetHeight(tileHeight);
			}

#pragma region COLLISION_LAYER_LOADING
			buffer = u->Attribute( "Collide_Type" );
			if( atoi(buffer.c_str() ) == COLLIDE ) 
			{
				buffer = u->Attribute( "ImageID" );
				if( atoi(buffer.c_str() ) != -1 )
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( woss.str().c_str() ) );
				else
					// Setting a blank image if there's none present.
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "Resources/Images/Graphics/MASQ_Blank.png") ) );

				buffer = u->Attribute( "Collide_Type" );
				t->SetTileType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "X" );
				t->SetX( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "Y" );
				t->SetY( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "TileX" );
				t->m_nTileX = atoi( buffer.c_str() );

				buffer = u->Attribute( "TileY" );
				t->m_nTileY = atoi( buffer.c_str() );

				RECT trect = {};
				buffer = u->Attribute( "RectX" );
				trect.left = atol(buffer.c_str() );

				buffer = u->Attribute( "RectY" );
				trect.top = atol(buffer.c_str() );

				buffer = u->Attribute( "RectWidth" );
				trect.right = trect.left + atol( buffer.c_str() );

				buffer = u->Attribute( "RectHeight" );
				trect.bottom = trect.top + atol( buffer.c_str() );

				t->SetSourceRect(trect);

				AddTile( *t );
			}
#pragma endregion

#pragma region MID_LAYER_LOADING
			buffer = u->Attribute( "Layer_Type" );
			if( atoi(buffer.c_str() ) == MID_LAYER ) 
			{
				buffer = u->Attribute( "ImageID" );
				if( atoi(buffer.c_str() ) != -1 )
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( woss.str().c_str() ) );
				else
					// Setting a blank image if there's none present.
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "Resources/Images/Graphics/MASQ_Blank.png") ) );

				buffer = u->Attribute( "Layer_Type" );
				t->SetLayerType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "Collide_Type" );
				t->SetTileType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "X" );
				t->SetX( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "Y" );
				t->SetY( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "TileX" );
				t->m_nTileX = atoi( buffer.c_str() );

				buffer = u->Attribute( "TileY" );
				t->m_nTileY = atoi( buffer.c_str() );

				RECT trect = {};
				buffer = u->Attribute( "RectX" );
				trect.left = atol(buffer.c_str() );

				buffer = u->Attribute( "RectY" );
				trect.top = atol(buffer.c_str() );

				buffer = u->Attribute( "RectWidth" );
				trect.right = trect.left + atol( buffer.c_str() );

				buffer = u->Attribute( "RectHeight" );
				trect.bottom = trect.top + atol( buffer.c_str() );

				t->SetSourceRect(trect);

				AddTile( *t );
			}
#pragma endregion

#pragma region PLAY_LAYER_LOADING
			else if( atoi(buffer.c_str() ) == PLAY_LAYER ) 
			{
				buffer = u->Attribute( "ImageID" );
				if( atoi(buffer.c_str() ) != -1 )
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( woss.str().c_str() ) );
				else
					// Setting a blank image if there's none present.
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "Resources/Images/Graphics/MASQ_Blank.png") ) );

				buffer = u->Attribute( "Layer_Type" );
				t->SetLayerType( 2 );
				t->SetLayerType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "Collide_Type" );
				t->SetTileType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "X" );
				t->SetX( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "Y" );
				t->SetY( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "TileX" );
				t->m_nTileX = atoi( buffer.c_str() );

				buffer = u->Attribute( "TileY" );
				t->m_nTileY = atoi( buffer.c_str() );

				RECT trect = {};
				buffer = u->Attribute( "RectX" );
				trect.left = atol(buffer.c_str() );

				buffer = u->Attribute( "RectY" );
				trect.top = atol(buffer.c_str() );

				buffer = u->Attribute( "RectWidth" );
				trect.right = trect.left + atol( buffer.c_str() );

				buffer = u->Attribute( "RectHeight" );
				trect.bottom = trect.top + atol( buffer.c_str() );

				t->SetSourceRect(trect);

				AddTile( *t );
			}
#pragma endregion

#pragma region FRONT_LAYER_LOADING
			else if( atoi(buffer.c_str() ) == FRONT_LAYER ) 
			{
				buffer = u->Attribute( "ImageID" );
				if( atoi(buffer.c_str() ) != -1 )
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( woss.str().c_str() ) );
				else
					// Setting a blank image if there's none present.
					t->SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "Resources/Images/Graphics/MASQ_Blank.png") ) );

				buffer = u->Attribute( "Collide_Type" );
				t->SetTileType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "Layer_Type" );
				t->SetLayerType( atoi(buffer.c_str() ) );

				buffer = u->Attribute( "X" );
				t->SetX( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "Y" );
				t->SetY( (float)(atof(buffer.c_str() ) ));

				buffer = u->Attribute( "TileX" );
				t->m_nTileX = atoi( buffer.c_str() );

				buffer = u->Attribute( "TileY" );
				t->m_nTileY = atoi( buffer.c_str() );

				RECT trect = {};
				buffer = u->Attribute( "RectX" );
				trect.left = atol(buffer.c_str() );

				buffer = u->Attribute( "RectY" );
				trect.top = atol(buffer.c_str() );

				buffer = u->Attribute( "RectWidth" );
				trect.right = trect.left + atol( buffer.c_str() );

				buffer = u->Attribute( "RectHeight" );
				trect.bottom = trect.top + atol( buffer.c_str() );

				t->SetSourceRect(trect);

				AddTile( *t );
			}
#pragma endregion
			t = nullptr;
			u = u->NextSiblingElement( "Tile" );
		}

		pRoot = pRoot->NextSiblingElement( "Level" );
	}

	return true;
}

bool CTileManager::LoadObjects( std::string filename, int nLevelNumber,
										CFox*	pFoxData, std::string szCheck)
{
	TiXmlDocument doc;
	
	
	CFileManager*	m_pFM			=	CFileManager::GetInstance();
	CObjectFactory*	m_pOF			=	CObjectFactory::GetInstance();
	CSGD_TextureManager* m_pTM			= CSGD_TextureManager::GetInstance();
	CGamePlayState*	pGame		= CGamePlayState::GetInstance();
	CFox*	pCF = pGame->GetFox();
	CCorinne*	pCC = pGame->GetCorrine();
	

	if( doc.LoadFile( filename.c_str() ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return false;

	while( pRoot != nullptr )
	{
		std::string buffer;
		std::wostringstream woss;

		buffer = pRoot->Attribute( "Name" );
		int nCurrLevel = atoi( buffer.c_str() );

		//if( nCurrLevel != nLevelNumber )
		//	continue;

		buffer = pRoot->Attribute( "End_Point_X" );
		float tempX = (float)(atof( buffer.c_str() ));
		CGamePlayState::GetInstance()->SetStageEndMarker( tempX );

#pragma region ___BACKGROUND____
		CImageObject tempIMGObj;
		buffer = pRoot->Attribute("BG_Width");
		tempIMGObj.SetWidth( atoi(buffer.c_str()));
		buffer = pRoot->Attribute("BG_Height");
		tempIMGObj.SetHeight( atoi(buffer.c_str()));

		RECT tempRect = {};
		tempRect.left = 0;
		tempRect.top = 0;
		buffer = pRoot->Attribute("BG_RectWidth");
		tempRect.right = atoi(buffer.c_str());
		buffer = pRoot->Attribute("BG_RectHeight");
		tempRect.bottom =  atoi(buffer.c_str());
		tempIMGObj.SetSourceRect( tempRect );

		buffer = pRoot->Attribute( "BG_LayerType" );
		tempIMGObj.SetLayerType( atoi( buffer.c_str() ) );

		buffer = pRoot->Attribute("BG_Filename");
		woss << "Resources/Images/Graphics/" << buffer.c_str();
		buffer = pRoot->Attribute( "BG_ImageID" );
		if( atoi(buffer.c_str() ) != -1 )
			tempIMGObj.SetImageID( CSGD_TextureManager::GetInstance()->LoadTexture( woss.str().c_str() ) );
		
		m_mImageList[ "Background" ] = tempIMGObj;
#pragma endregion

		TiXmlElement* uo = pRoot->FirstChildElement( "Unique_Objects" );
		
		TiXmlElement* o = uo->FirstChildElement( "Object" );

		while( o != nullptr )
		{
			buffer = o->Attribute( "EventID" );

#pragma region ___CORINNE___
			string cbuffer = "Corinne_";
			if( buffer.compare( 0, cbuffer.length(), cbuffer ) == 0 )
			{
				// Create and Register Checkpoint Object
				//pCC	=	new CCorinne();

				// Change Image to Corinne
				//pCC->SetImageID( m_pTM->LoadTexture( _T("Resources/Images/Characters/Corinne.png") ) );

				// Set Parameters

				// Physics
				pCC->SetGravityBool( true );
				pCC->SetGravityFloat( 300.0f );
				pCC->SetVelX( 0.0f );
				pCC->SetVelY( 0.0f );
				pCC->SetMaxXVelocity( 150.0f );
				pCC->SetMaxYVelocity( -250.0f );
				pCC->SetNormalMaxXvel(pCC->GetMaxXVelocity());
				pCC->SetNormalMaxYvel(pCC->GetMaxYVelocity());
				pCC->SetStopVelocityBool( false );

				if( pFoxData != nullptr && szCheck.size() > 0 )
				{
					pCC->SetX( pFoxData->GetX() );
					pCC->SetY( pFoxData->GetY() );
				}
				else
				{
					// Position
					buffer = o->Attribute( "X" );
					pCC->SetX( (float)(atof(buffer.c_str())));
					buffer = o->Attribute( "Y" );
					pCC->SetY( (float)(atof(buffer.c_str()) ));
				}

				// Animation Assignment
				pCC->SetWidth( 1 );
				pCC->SetHeight( 1 );
				pCC->AddAnimation("Resources/Scripts/Corinne_animations.xml", D_IDLE );
				pCC->GetFrame().SetPlaying( true );
				pCC->GetFrame().SetLooping( true );

				
				//pCC->SetLayerType( PLAY_LAYER );

				// Add Corinne Object into world
				CObjectManager::GetInstance()->AddObject( pCC );
				pCC->Release();
			}

#pragma endregion

#pragma region ___FOX___
			string fbuffer = "Fox_";
			if( buffer.compare( 0, fbuffer.length(), fbuffer ) == 0 )
			{
				// Create and Register Checkpoint Object
				//pCF	=	new CFox();

				// Change Image to Fox
				//pCF->SetImageID( m_pTM->LoadTexture( _T("Resources/Images/Characters/Fox.png") ) );

				// Set Parameters
				pCF->SetGravityBool( true );
				pCF->SetGravityFloat( 600.0f );
				pCF->SetVelX( 0.0f );
				pCF->SetVelY( 0.0f );
				pCF->SetMaxXVelocity( 260.0f );
				pCF->SetMaxYVelocity( -500.0f );
				pCF->SetStopVelocityBool( false );
				pCF->SetStrength( 325.0f );

				// HUD
				pCF->SetHearts( 5 );

				// Jump Physics
				pCF->SetJumpBool( false );
				pCF->SetMaxJumpHeight( 50.0f );

				if( pFoxData != nullptr && szCheck.size() > 0 )
				{
					pCF->SetX( pFoxData->GetX() );
					pCF->SetY( pFoxData->GetY() );
				}
				else
				{
					// Position
					buffer = o->Attribute( "X" );
					pCF->SetX( (float)(atof(buffer.c_str())));
					buffer = o->Attribute( "Y" );
					pCF->SetY( (float)(atof(buffer.c_str())) );
					pCF->SetStartingXPos( pCF->GetX() );
					pCF->SetStartingYPos( pCF->GetY() );
				}

				// Animation Assignment
				pCF->SetWidth( 1 );
				pCF->SetHeight( 1 );
				pCF->AddAnimation("Resources/Scripts/Fox_animations.xml", F_IDLE );
				//pCF-
				pCF->GetFrame().SetPlaying( false );
				pCF->GetFrame().SetLooping( true );
				pCF->SetCurrentTool(0);
				pCF->GetFrame().SetFlipped(true);
				
				pCF->SetLayerType( PLAY_LAYER );
				pCF->SetDaughter( pGame->GetCorrine() );

				// Add Checkpoint Object into world
				CObjectManager::GetInstance()->AddObject( pCF );
				pCF->Release();
			}
#pragma endregion

#pragma region ___CHECKPOINT___
			string cpbuffer = "Checkpoint_";
			if( buffer.compare( 0, cpbuffer.length(), cpbuffer ) == 0 )
			{
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( CHECKPOINT );
				CCheckpoint*	pCheckpoint	= dynamic_cast< CCheckpoint* >( pAddBase );

				// Change Image to Checkpoint
				//pCheckpoint->SetImageID( m_pTM->LoadTexture( _T("Resources/Images/Characters/Enemy_Rat.png") ) );
				pCheckpoint->SetID( buffer.c_str() );
				// Set Parameters

				if( pCheckpoint->GetID().compare( szCheck ) == false )
					pCheckpoint->SetCheckpointBool( true );
				else
				pCheckpoint->SetCheckpointBool( false );

				pCheckpoint->SetGravityBool( false );
				pCheckpoint->SetVelX( 0.0f );
				pCheckpoint->SetVelY( 0.0f );

				buffer = o->Attribute( "X" );
				pCheckpoint->SetX( (float)(atof(buffer.c_str())));

				buffer = o->Attribute( "Y" );
				pCheckpoint->SetY( (float)(atof(buffer.c_str()) + 64 ));

				pCheckpoint->SetWidth( 1 );
				pCheckpoint->SetHeight( 1 );
				pCheckpoint->AddAnimation("Resources/Scripts/tempcheckpoint.xml", 0 );
				pCheckpoint->SetLayerType( PLAY_LAYER );

				// Add Checkpoint Object into world
				CObjectManager::GetInstance()->AddObject( pCheckpoint );
				pCheckpoint->Release();
			}
#pragma endregion

#pragma region ___TOOL_LOADING___
			string tbuffer = "Tool_";
			if( buffer.compare( 0, tbuffer.length(), tbuffer ) == 0 )
			{
				buffer = o->Attribute( "Tool_Type" );
				if( atoi(buffer.c_str() ) == 0 )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( SQUIRT_GUN );
					ITool*	t	= dynamic_cast< CSquirtGun* >( pAddBase );

					t->SetImageID(m_pFM->GetImageID("Squirt_Gun_HUD.png"));

					if( pFoxData != nullptr )
					{
						if( pFoxData->GetATool( ITool::TOOL_SQUIRTGUN ) == nullptr )
						{
							buffer = o->Attribute( "X" );
							t->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							t->SetY( (float)(atof(buffer.c_str() )));
							t->SetWidth( 128 );
							t->SetHeight( 64 );

							t->SetGravityBool(true);
							t->SetGravityFloat( 200 );
							t->SetLayerType(PLAY_LAYER);
						}
						else
							pCF->AddATool( t );
					}

					else
					{
						buffer = o->Attribute( "X" );
						t->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						t->SetY( (float)(atof(buffer.c_str())) );
						t->SetWidth( 128 );
						t->SetHeight( 64 );

						t->SetGravityBool(true);
						t->SetGravityFloat( 200 );
						t->SetLayerType(PLAY_LAYER);
					}
					CObjectManager::GetInstance()->AddObject( t );
					t->Release();

				}

				if( atoi(buffer.c_str() ) == 1 )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( BALLOON_GUN );
					ITool*	t	= dynamic_cast< CBalloonGun* >( pAddBase );

					t->SetImageID(m_pFM->GetImageID("Balloon_Gun_HUD.png"));


					if( pFoxData != nullptr )
					{
						if( pFoxData->GetATool( ITool::TOOL_BALLOONGUN) == nullptr )
						{
							buffer = o->Attribute( "X" );
							t->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							t->SetY( (float)(atof(buffer.c_str())));

							t->SetWidth( 128 );
							t->SetHeight( 64 );

							t->SetGravityBool(true);
							t->SetGravityFloat( 200 );
							t->SetLayerType(PLAY_LAYER);
						}
						else
							pCF->AddATool( t );
					}
					else
					{
						buffer = o->Attribute( "X" );
						t->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						t->SetY( (float)(atof(buffer.c_str())));

						t->SetWidth( 128 );
						t->SetHeight( 64 );

						t->SetGravityBool(true);
						t->SetGravityFloat( 200 );
						t->SetLayerType(PLAY_LAYER);
					}
					CObjectManager::GetInstance()->AddObject( t );
					t->Release();
				}

				if( atoi(buffer.c_str() ) == 2 )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( COTTON_LAUNCHER );
					ITool*	t	= dynamic_cast< CCottonCandyLauncher* >( pAddBase );

					t->SetImageID(m_pFM->GetImageID("CottonCandy_Gun_HUD.png"));

					if( pFoxData != nullptr )
					{
						if( pFoxData->GetATool( ITool::TOOL_COTTONCANDYLAUNCHER ) == nullptr )
						{
							buffer = o->Attribute( "X" );
							t->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							t->SetY( (float)(atof(buffer.c_str())));

							t->SetWidth( 128 );
							t->SetHeight( 64 );

							t->SetGravityBool(true);
							t->SetGravityFloat( 200 );
							t->SetLayerType(PLAY_LAYER);
						}
						else
							pCF->AddATool( t );
					}
					else
					{
						buffer = o->Attribute( "X" );
						t->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						t->SetY( (float)(atof(buffer.c_str())));

						t->SetWidth( 128 );
						t->SetHeight( 64 );

						t->SetGravityBool(true);
						t->SetGravityFloat( 200 );
						t->SetLayerType(PLAY_LAYER);
					}
					CObjectManager::GetInstance()->AddObject( t );
					t->Release();
				}
			}
#pragma endregion

#pragma region ___POWERUP_LOADING___
			string ppbuffer = "PU_";
			if( buffer.compare( 0, ppbuffer.length(), ppbuffer ) == 0 )
			{
				buffer = o->Attribute( "Powerup_Type" );
				if( atoi(buffer.c_str() ) == CPowerup::GREASED_PIG )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( POWERUP );
					CPowerup*	pow	= dynamic_cast< CPowerup* >( pAddBase );

					if( pFoxData == nullptr )
					{
						pow->SetPowerupType(CPowerup::GREASED_PIG);
						pow->SetImageID(m_pFM->GetImageID("greased_pig.png"));
						pow->SetWidth(32);
						pow->SetHeight(32);

						buffer = o->Attribute( "X" );
						pow->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						pow->SetY( (float)(atof(buffer.c_str())));

						pow->SetLayerType( PLAY_LAYER );
						CObjectManager::GetInstance()->AddObject( pow );
						pow->Release();
					}
					else
					{
						bool bInVector = false;
						std::vector< CPowerup::PowerupType > vPowerups = pFoxData->GetPowerups();
						for( unsigned i = 0; i < vPowerups.size(); ++i )
						{
							if( vPowerups[i] == CPowerup::GREASED_PIG )
							{
								bInVector = true;
								pow->SetPowerupType(CPowerup::GREASED_PIG);
								pow->SetImageID(m_pFM->GetImageID("greased_pig.png"));
								pow->SetLayerType( PLAY_LAYER );
								pCF->AddAPowerUp( CPowerup::GREASED_PIG );
								break;
							}
						}

						if( bInVector == false )
						{
							pow->SetPowerupType(CPowerup::GREASED_PIG);
							pow->SetImageID(m_pFM->GetImageID("greased_pig.png"));
							pow->SetWidth(32);
							pow->SetHeight(32);

							buffer = o->Attribute( "X" );
							pow->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							pow->SetY( (float)(atof(buffer.c_str())));

							pow->SetLayerType( PLAY_LAYER );
							CObjectManager::GetInstance()->AddObject( pow );
							pow->Release();
						}
					}


				}

				if( atoi(buffer.c_str() ) == CPowerup::CHEETAH )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( POWERUP );
					CPowerup*	pow	= dynamic_cast< CPowerup* >( pAddBase );

					if( pFoxData == nullptr )
					{
						pow->SetPowerupType(CPowerup::CHEETAH);
						pow->SetImageID(m_pFM->GetImageID("cheetah.png"));
						pow->SetWidth(32);
						pow->SetHeight(32);

						buffer = o->Attribute( "X" );
						pow->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						pow->SetY( (float)(atof(buffer.c_str())));

						pow->SetLayerType( PLAY_LAYER );
						CObjectManager::GetInstance()->AddObject( pow );
						pow->Release();
					}
					else
					{
						bool bInVector = false;
						std::vector< CPowerup::PowerupType > vPowerups = pFoxData->GetPowerups();
						for( unsigned i = 0; i < vPowerups.size(); ++i )
						{
							if( vPowerups[i] == CPowerup::CHEETAH )
							{
								bInVector = true;
								pow->SetPowerupType(CPowerup::CHEETAH);
								pow->SetImageID(m_pFM->GetImageID("cheetah.png"));
								pow->SetLayerType( PLAY_LAYER );
								pCF->AddAPowerUp( CPowerup::CHEETAH );
								break;
							}
						}

						if( bInVector == false )
						{
							pow->SetPowerupType(CPowerup::CHEETAH);
							pow->SetImageID(m_pFM->GetImageID("cheetah.png"));
							pow->SetWidth(32);
							pow->SetHeight(32);

							buffer = o->Attribute( "X" );
							pow->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							pow->SetY( (float)(atof(buffer.c_str())));

							pow->SetLayerType( PLAY_LAYER );
							CObjectManager::GetInstance()->AddObject( pow );
							pow->Release();
						}
					}
				}

				if( atoi(buffer.c_str() ) == CPowerup::POGO )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( POWERUP );
					CPowerup*	pow	= dynamic_cast< CPowerup* >( pAddBase );

					if( pFoxData == nullptr )
					{
						pow->SetPowerupType(CPowerup::POGO);
						pow->SetImageID(m_pFM->GetImageID("pogo.png"));
						pow->SetWidth(32);
						pow->SetHeight(32);

						buffer = o->Attribute( "X" );
						pow->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						pow->SetY( (float)(atof(buffer.c_str())));

						pow->SetLayerType( PLAY_LAYER );
						CObjectManager::GetInstance()->AddObject( pow );
						pow->Release();
					}
					else
					{
						bool bInVector = false;
						std::vector< CPowerup::PowerupType > vPowerups = pFoxData->GetPowerups();
						for( unsigned i = 0; i < vPowerups.size(); ++i )
						{
							if( vPowerups[i] == CPowerup::POGO )
							{
								bInVector = true;
								pow->SetPowerupType(CPowerup::POGO);
								pow->SetImageID(m_pFM->GetImageID("pogo.png"));
								pCF->AddAPowerUp( CPowerup::POGO);
								pow->SetLayerType( PLAY_LAYER );
								break;
							}
						}

						if( bInVector == false )
						{
							pow->SetPowerupType(CPowerup::POGO);
							pow->SetImageID(m_pFM->GetImageID("pogo.png"));
							pow->SetWidth(32);
							pow->SetHeight(32);

							buffer = o->Attribute( "X" );
							pow->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							pow->SetY( (float)(atof(buffer.c_str())));

							pow->SetLayerType( PLAY_LAYER );
							CObjectManager::GetInstance()->AddObject( pow );
							pow->Release();
						}
					}

				}

				if( atoi(buffer.c_str() ) == CPowerup::FLUBBER )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( POWERUP );
					CPowerup*	pow	= dynamic_cast< CPowerup* >( pAddBase );

					if( pFoxData == nullptr )
					{
						pow->SetPowerupType(CPowerup::FLUBBER);
						pow->SetImageID(m_pFM->GetImageID("flubber.png"));
						pow->SetWidth(32);
						pow->SetHeight(32);

						buffer = o->Attribute( "X" );
						pow->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						pow->SetY( (float)(atof(buffer.c_str())));

						pow->SetLayerType( PLAY_LAYER );
						CObjectManager::GetInstance()->AddObject( pow );
						pow->Release();
					}
					else
					{
						bool bInVector = false;
						std::vector< CPowerup::PowerupType > vPowerups = pFoxData->GetPowerups();
						for( unsigned i = 0; i < vPowerups.size(); ++i )
						{
							if( vPowerups[i] == CPowerup::POGO )
							{
								bInVector = true;
								pow->SetPowerupType(CPowerup::FLUBBER);
								pow->SetImageID(m_pFM->GetImageID("flubber.png"));
								pow->SetLayerType( PLAY_LAYER );
								pCF->AddAPowerUp( CPowerup::FLUBBER );
								break;
							}
						}

						if( bInVector == false )
						{
							pow->SetPowerupType(CPowerup::FLUBBER);
							pow->SetImageID(m_pFM->GetImageID("flubber.png"));
							pow->SetWidth(32);
							pow->SetHeight(32);

							buffer = o->Attribute( "X" );
							pow->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							pow->SetY( (float)(atof(buffer.c_str())));

							pow->SetLayerType( PLAY_LAYER );
							CObjectManager::GetInstance()->AddObject( pow );
							pow->Release();
						}
					}
				}

				if( atoi(buffer.c_str() ) == CPowerup::FIZZY_LIFTING_DRINK )
				{
					// Temporary Object Creation
					CBaseObject*	pAddBase = m_pOF->CreateObject( POWERUP );
					CPowerup*	pow	= dynamic_cast< CPowerup* >( pAddBase );

					if( pFoxData == nullptr )
					{
						pow->SetPowerupType(CPowerup::FIZZY_LIFTING_DRINK);
						pow->SetImageID(m_pFM->GetImageID("fizzy_lifting_drink.png"));
						pow->SetWidth(32);
						pow->SetHeight(32);

						buffer = o->Attribute( "X" );
						pow->SetX( (float)(atof(buffer.c_str())));
						buffer = o->Attribute( "Y" );
						pow->SetY( (float)(atof(buffer.c_str())));

						pow->SetLayerType( PLAY_LAYER );
						CObjectManager::GetInstance()->AddObject( pow );
						pow->Release();
					}
					else
					{
						bool bInVector = false;
						std::vector< CPowerup::PowerupType > vPowerups = pFoxData->GetPowerups();
						for( unsigned i = 0; i < vPowerups.size(); ++i )
						{
							if( vPowerups[i] == CPowerup::FIZZY_LIFTING_DRINK )
							{
								bInVector = true;
								pow->SetPowerupType(CPowerup::FIZZY_LIFTING_DRINK);
								pow->SetImageID(m_pFM->GetImageID("fizzy_lifting_drink.png"));
								pow->SetLayerType( PLAY_LAYER );
								pCF->AddAPowerUp( CPowerup::FIZZY_LIFTING_DRINK );
								break;
							}
						}

						if( bInVector == false )
						{
							pow->SetPowerupType(CPowerup::FIZZY_LIFTING_DRINK);
							pow->SetImageID(m_pFM->GetImageID("fizzy_lifting_drink.png"));
							pow->SetWidth(32);
							pow->SetHeight(32);

							buffer = o->Attribute( "X" );
							pow->SetX( (float)(atof(buffer.c_str())));
							buffer = o->Attribute( "Y" );
							pow->SetY( (float)(atof(buffer.c_str())));

							pow->SetLayerType( PLAY_LAYER );
							CObjectManager::GetInstance()->AddObject( pow );
							pow->Release();
						}
					}
				}
			}
#pragma endregion

#pragma region ____RAT____
			string rbuffer = "Rat_";
			if( buffer.compare( 0, rbuffer.length(), rbuffer ) == 0 )
			{
				// Create and Register Rat Object
				CBaseObject*	pAddBase = m_pOF->CreateObject( RAT );
				CRat*	pRat	= dynamic_cast< CRat* >( pAddBase );

				// Set Image
				//pRat->SetImageID(m_pTM->LoadTexture( _T("Resources/Images/Characters/Enemy_Rat.png") ) );
				//m_pFM->AddImageID( "Enemy_pRat.png", pRat->GetImageID() );
				//pRat->SetImageID(m_pTM->LoadTexture( _T("Resources/Images/Characters/Enemy_Rat.png") ) );

				// Set Parameters
				float fRandVel = (float)(rand() % 40 + 60);
				pRat->SetGravityBool( true );
				pRat->SetGravityFloat(200.0f);
				pRat->SetVelX( fRandVel );
				//pRat->SetVelY( 500.0f );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pRat->Init( tempX, tempY );
				pRat->SetLayerType( PLAY_LAYER );

				// Setting to 1,1 for Animations
				pRat->SetWidth( 1 );
				pRat->SetHeight( 1 );

				// Add to World

				/*if( pFoxData != nullptr )
				{
					if( pFoxData->GetX() > pRat->GetX() )
					{}
					else
					{
						CObjectManager::GetInstance()->AddObject( pRat );
						pRat->Release();
					}
				}
				else
				{*/
					CObjectManager::GetInstance()->AddObject( pRat );
					pRat->Release();
				//}
				
			}
#pragma endregion

#pragma region ___HOODLUM___
			string hbuffer = "Hoodlum_";
			if( buffer.compare( 0, hbuffer.length(), hbuffer ) == 0 )
			{
				// Create and Register Hoodlum Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( HOODLUM );
				CHoodlum*	pHoodlum	= dynamic_cast< CHoodlum* >( pAddBase );

				// Set Velocities
				pHoodlum->SetMaxXVelocity(100.0f );
				pHoodlum->SetMaxYVelocity(-150.0f);
				pHoodlum->SetStopVelocityBool( false );

				// Set Parameters
				//float fRandVel = rand() % 40 + 60;
				pHoodlum->SetGravityBool( true );
				pHoodlum->SetGravityFloat(200.0f);
				//pHoodlum->SetVelX( fRandVel );
				//pHoodlum->SetVelY( 500.0f );
				pHoodlum->SetFox( pCF );
				pHoodlum->SetCorinne( pCC );

				pHoodlum->SetLayerType( PLAY_LAYER );

				// Setting to 1,1 for Animations
				pHoodlum->SetWidth( 1 );
				pHoodlum->SetHeight( 1 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pHoodlum->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pHoodlum->SetY( tempY );

				
				//if( pFoxData != nullptr )
				//{
				//	if( pFoxData->GetX() > pHoodlum->GetX() )
				//	{}
				//	else
				//	{
				//		CObjectManager::GetInstance()->AddObject( pHoodlum );
				//		pHoodlum->Release();
				//	}
				//}
				//else
				//{
					CObjectManager::GetInstance()->AddObject( pHoodlum );
					pHoodlum->Release();
				//}

			}
#pragma endregion

#pragma region ___CLOWN___ 
			string clobuffer = "Clown_";
			if( buffer.compare( 0, clobuffer.length(), clobuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( CLOWN );
				CClown*	pClown	= dynamic_cast< CClown* >( pAddBase );

				// Set Velocities
				pClown->SetMaxXVelocity(100.0f );
				pClown->SetMaxYVelocity(-150.0f);
				pClown->SetStopVelocityBool( false );
				pClown->SetMovingRightBool(true);

				// Set Parameters
				pClown->SetGravityBool( true );
				pClown->SetGravityFloat(200.0f);
				pClown->SetFox( pCF );
				pClown->SetCorinne( pCC );
				pClown->SetLayerType( PLAY_LAYER );
				pClown->SetColor( D3DCOLOR_XRGB( 0, 255, 0 ) );

				// Setting to 1,1 for Animations
				pClown->SetHeight( 1 );
				pClown->SetWidth( 1 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pClown->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pClown->SetY( tempY );

	/*			if( pFoxData != nullptr )
				{
					if( pFoxData->GetX() > pClown->GetX() )
					{}
					else
					{*/
						CObjectManager::GetInstance()->AddObject( pClown );
						pClown->Release();
	/*				}
				}
				else
				{
					CObjectManager::GetInstance()->AddObject( pClown );
					pClown->Release();
				}*/
			}
#pragma endregion

#pragma region ___HORZGATE___ 
			string hgbuffer = "Horz_Gate_";
			if( buffer.compare( 0, hgbuffer.length(), hgbuffer ) == 0 )
			{
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( GATE );
				CGate*	pGate	= dynamic_cast< CGate* >( pAddBase );

				// Set Velocities
				/*pGate->SetMaxXVelocity(100.0f );
				pGate->SetMaxYVelocity(-150.0f);
				pGate->SetStopVelocityBool( false );*/

				// Set Parameters
				pGate->SetGravityBool( false );
				pGate->SetID( buffer );
				pGate->SetLayerType( PLAY_LAYER );

				// Setting to 1,1 for Animations
				pGate->SetWidth( 1 );
				pGate->SetHeight( 1 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pGate->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pGate->SetY( tempY + 32 );
				pGate->SetGateFrame( 1 );
				
				//CAnimationManager::GetInstance()->AddFrame( pGate->GetGateFrame() );

				//pGate->GetGateFrame().Init( pGate );

				// Add to World
				CObjectManager::GetInstance()->AddObject( pGate );
				pGate->Release();
			}
#pragma endregion

#pragma region ___VERTGATE___ 
			string vgbuffer = "Vert_Gate_";
			if( buffer.compare( 0, vgbuffer.length(), vgbuffer ) == 0 )
			{
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( GATE );
				CGate*	pGate	= dynamic_cast< CGate* >( pAddBase );

				// Set Velocities
				/*pGate->SetMaxXVelocity(100.0f );
				pGate->SetMaxYVelocity(-150.0f);
				pGate->SetStopVelocityBool( false );*/

				// Set Parameters
				pGate->SetGravityBool( false );
				pGate->SetID( buffer );
				pGate->SetLayerType( PLAY_LAYER );

				// Setting to 1,1 for Animations
				pGate->SetWidth( 1 );
				pGate->SetHeight( 1 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pGate->SetX( tempX + 32 );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pGate->SetY( tempY + 256 );

				// Add to World
				CObjectManager::GetInstance()->AddObject( pGate );
				pGate->Release();
			}
#pragma endregion

#pragma region ___PRESSUREPLATE___ 
			string pplbuffer = "Pressure_Plate_";
			if( buffer.compare( 0, pplbuffer.length(), pplbuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( PRESSURE_PLATE );
				CPressurePlate*	pPressurePlate	= dynamic_cast< CPressurePlate* >( pAddBase );

				// Set Parameters
				pPressurePlate->SetGravityBool( true );
				pPressurePlate->SetGravityFloat(200.0f);
				pPressurePlate->SetLayerType( PLAY_LAYER );

				pPressurePlate->SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Pressure_Plate.png") );

				// Setting to 1,1 for Animations
				pPressurePlate->SetHeight( 64 );
				pPressurePlate->SetWidth( 64 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pPressurePlate->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pPressurePlate->SetY( tempY );

				
				CObjectManager::GetInstance()->AddObject( pPressurePlate );
				pPressurePlate->Release();


			}
#pragma endregion

#pragma region ___SPRING___ 
			string sprbuffer = "Spring_";
			if( buffer.compare( 0, sprbuffer.length(), sprbuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( SPRING );
				CSpring*	pSpring	= dynamic_cast< CSpring* >( pAddBase );

				// Set Parameters
				pSpring->SetGravityBool( true );
				pSpring->SetGravityFloat(200.0f);
				pSpring->SetLayerType( PLAY_LAYER );

				pSpring->SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Spring.png") );

				// Setting to 1,1 for Animations
				pSpring->SetHeight( 64 );
				pSpring->SetWidth( 32 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pSpring->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pSpring->SetY( tempY );

				
				CObjectManager::GetInstance()->AddObject( pSpring );
				pSpring->Release();
				

			}
#pragma endregion

#pragma region ___CRATE___ 
			string cratebuffer = "Crate_";
			if( buffer.compare( 0, cratebuffer.length(), cratebuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( CRATE );
				CCrate*	pCrate	= dynamic_cast< CCrate* >( pAddBase );

				// Set Parameters
				pCrate->SetGravityBool( true );
				pCrate->SetGravityFloat(200.0f);
				pCrate->SetLayerType( PLAY_LAYER );

				pCrate->SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Crate.png") );

				// Setting to 1,1 for Animations
				pCrate->SetHeight( 32 );
				pCrate->SetWidth( 32 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pCrate->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pCrate->SetY( tempY );

				
				CObjectManager::GetInstance()->AddObject( pCrate );
				pCrate->Release();
				

			}
#pragma endregion

#pragma region ___BALLOON___ 
			string balloonbuffer = "Balloon_";
			if( buffer.compare( 0, balloonbuffer.length(), balloonbuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( BALLOON );
				CBalloon*	pBalloon	= dynamic_cast< CBalloon* >( pAddBase );

				// Set Parameters
				pBalloon->SetGravityBool( true );
				pBalloon->SetGravityFloat(200.0f);
				pBalloon->SetLayerType( PLAY_LAYER );
				pBalloon->SetVelY(-150.0f);

				pBalloon->SetImageID( CFileManager::GetInstance()->GetImageID("MASQ_Platform_Balloon.png") );

				// Setting to 1,1 for Animations
				pBalloon->SetHeight( 96 );
				pBalloon->SetWidth( 64 );
				pBalloon->SetBalloonColor(CBalloon::BALLOON_GREEN);

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pBalloon->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pBalloon->SetY( tempY );

				
				CObjectManager::GetInstance()->AddObject( pBalloon );
				pBalloon->Release();

			}
#pragma endregion

#pragma region ___LEVER___ 
			string leverbuffer = "Lever_";
			if( buffer.compare( 0, leverbuffer.length(), leverbuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( LEVER );
				CLever*	pLever	= dynamic_cast< CLever* >( pAddBase );

				// Set Parameters
				pLever->SetGravityBool( false );
				pLever->SetID( buffer );
				pLever->SetLayerType( PLAY_LAYER );
				pLever->SetVelY(0.0f);
				pLever->SetVelX(0.0f);
				pLever->SetMaxXVelocity( 0.0f );
				pLever->SetMaxYVelocity( -150.0f );
				pLever->SetStopVelocityBool( true );
				pLever->SetColor( D3DCOLOR_XRGB( 255, 255 , 255 ) );
				pLever->SetInRangeBool( false );
				pLever->SetIsActivatedBool( false );
				pLever->SetTriggerFrame( "Resources/Scripts/lever.xml", 0 );
				pLever->SetCurrentState( nullptr );

				// Setting to 1,1 for Animations
				pLever->SetHeight( 1 );
				pLever->SetWidth( 1 );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pLever->SetX( tempX );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pLever->SetY( tempY + 59 );

				
				CObjectManager::GetInstance()->AddObject( pLever );
				pLever->Release();

			}
#pragma endregion

			// WORK ON ///////////////
#pragma region ___SECURITYCAMERA___ 
			string securitycambuffer = "Security_Cam_";
			if( buffer.compare( 0, securitycambuffer.length(), securitycambuffer ) == 0 )
			{
				// Create and Register Clown Object 
				// Temporary Object Creation
				CBaseObject*	pAddBase = m_pOF->CreateObject( SECURITY_CAMERA );
				CSecurityCamera*	pSecurityCam	= dynamic_cast< CSecurityCamera* >( pAddBase );

				CSpawnPoint* pSpawn = new CSpawnPoint;
				pSpawn->SetEvent( buffer );//must be the same as the security camera's ID
				// Set Parameters
				pSecurityCam->SetID( buffer );

				const char* type = o->Attribute("Enemy");
				if( type != nullptr )
					pSpawn->SetEnemySpawType( atoi(type) );
				else
					pSpawn->SetEnemySpawType( NME_HOODLUM );

				// Position
				buffer = o->Attribute( "X" );
				float tempX = (float)(atof(buffer.c_str()));
				pSecurityCam->SetX( tempX );
				pSpawn->SetX( tempX + 100 );
				buffer = o->Attribute( "Y" );
				float tempY = (float)(atof(buffer.c_str()));
				pSecurityCam->SetY( tempY );
				pSpawn->SetY( tempY );


				//// Set Parameters
				//pSecurityCam->SetID( buffer );

				CObjectManager::GetInstance()->AddObject( pSecurityCam );
				CObjectManager::GetInstance()->AddObject( pSpawn );
				pSecurityCam->Release();
				pSpawn->Release();
			}
#pragma endregion
			o = o->NextSiblingElement( "Object" );

#pragma region _NPC_
			CObjectManager::GetInstance()->AddObject( CNPC::GetInstance() );
#pragma endregion
		}


		o = uo->FirstChildElement( "Object" );

		std::vector<IBaseObject*> vLevers = CObjectManager::GetInstance()->FindObjectsWithType( OBJ_TRIGGER, CTrigger::TRIG_LEVER );
		std::vector<IBaseObject*> vGates = CObjectManager::GetInstance()->FindObjectsWithType( OBJ_PUZZLE, CTriggeredObject::TRIGOBJ_GATE );

		while( o != nullptr )
		{
			buffer = o->Attribute( "EventID" );

			string vgbuffer = "Vert_Gate_";
			string hgbuffer = "Horz_Gate_";
			if( buffer.compare( 0, hgbuffer.length(), hgbuffer ) == 0 )
			{
				for( size_t i = 0; i < vLevers.size(); i++ )
				{
					buffer = o->Attribute( "Trigger_EventID" );
					CLever* tempLever = dynamic_cast<CLever*>(vLevers[ i ]);
					
					if( tempLever->GetID().compare( buffer ) == false )
					{
						for( size_t j = 0; j < vGates.size(); j++ )
						{
							buffer = o->Attribute( "EventID" );
							CGate* tempGate = dynamic_cast<CGate*>(vGates[ j ]);
							if( tempGate->GetID().compare( buffer ) == false )
							{
								tempLever->AddObjectToTrigger( tempGate );
							}
						}
					}
				}
			}

			
			if( buffer.compare( 0, vgbuffer.length(), vgbuffer ) == 0 )
			{
				for( size_t i = 0; i < vLevers.size(); i++ )
				{
					buffer = o->Attribute( "Trigger_EventID" );
					CLever* tempLever = dynamic_cast<CLever*>(vLevers[ i ]);
					
					if( tempLever->GetID().compare( buffer ) == false )
					{
						for( size_t j = 0; j < vGates.size(); j++ )
						{
							buffer = o->Attribute( "EventID" );
							CGate* tempGate = dynamic_cast<CGate*>(vGates[ j ]);
							if( tempGate->GetID().compare( buffer ) == false )
							{
								
								tempLever->AddObjectToTrigger( tempGate );
							}
						}
					}
				}
			}

			o = o->NextSiblingElement( "Object" );
		}
		
		pRoot = pRoot->NextSiblingElement( "Level" );

	}

	return true;

}

void CTileManager::RemoveAllTiles( )
{
	CFileManager*	pFM = CFileManager::GetInstance();

	for ( unsigned int x = 0; x < m_vTileList.size(); ++x )
	{
		for ( unsigned int y = 0; y < m_vTileList[ x ].size(); ++y ) 
		{
			if ( m_vTileList[ x ][ y ] == nullptr ) 
				continue;

			//pFM->ReleaseTexture( m_vTileList[ x ][ y ]->GetImageID() );

			CTile* temp = m_vTileList[ x ][ y ];
			delete temp;
			m_vTileList[ x ][ y ] = nullptr;
		}
	}
	m_vTileList.clear();
}

