/*
	File:		Checkpoint.cpp
	Course:		SGP1
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Allows auto-saving when Fox collides with it.
*/

#include "Checkpoint.h"
#include "FileManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "AnimationManager.h"
#include "Fox.h"
#include "ObjectManager.h"
#include "GamePlayState.h"

CCheckpoint::CCheckpoint(void)
{
	SetWidth( -1 );
	SetHeight( -1 );
	SetX( -1.0f );
	SetY( -1.0f );
	SetVelX( 0.0f );
	SetVelY( 0.0f );
	SetImageID( -1 );		
	SetColor( D3DCOLOR_ARGB( 0, 0, 0, 0 ) );	 
	SetMaxXVelocity( 0.0f )	;
	SetMaxYVelocity( 0.0f )	;
	SetJumpBool( false ); 
	SetStopVelocityBool( true );
	SetCheckpointBool( false );
	m_nType = OBJ_CHECKPOINT;
	/*CAnimationManager::GetInstance()->AddFrame(m_pCheck);*/
}


CCheckpoint::~CCheckpoint(void)
{
	CAnimationManager::GetInstance()->RemoveFrame( &m_pCheck );
}

bool CCheckpoint::CheckCollision( IBaseObject * pBase )
{
		if( pBase->GetType() == OBJ_FOX )
		{
			if( CBaseObject::CheckCollision( (CFox*)pBase ) == true &&
					GetCheckpointBool() == false)
			{

				if( /*CFileManager::GetInstance()->GetSaves().size() < 3 && */GetCheckpointBool() == false )
				{
					CFileManager*	pFile = CFileManager::GetInstance();
					CFileManager::playerProgress saveGame;
					std::ostringstream tempss;
					int i = CFileManager::GetInstance()->GetSaveToLoad();
					if( CFileManager::GetInstance()->GetSaveToLoad() < 0 ||
						CFileManager::GetInstance()->GetSaveToLoad() > 3 )
					{
						CFileManager::GetInstance()->SetCurrentSaveToLoad( 1 );
						tempss << CFileManager::GetInstance()->GetCurrentSaveToLoad();
					}
					else
						tempss << CFileManager::GetInstance()->GetCurrentSaveToLoad();

					saveGame.nLevelNum = CGamePlayState::GetInstance()->GetCurStage();
					saveGame.szLevelPath = CGamePlayState::GetInstance()->GetLevelFile();
					saveGame.pFox = (CFox*)pBase;
					saveGame.szCheckPoint = GetID();
					saveGame.szFileName = "Resources/Scripts/Save_";
					saveGame.szFileName += tempss.str();
					saveGame.szFileName += ".xml";
					CFileManager::GetInstance()->SaveGame( saveGame.szFileName.c_str(), saveGame );
					CFileManager::GetInstance()->GetSaves().push_back( saveGame );
				}

				std::vector< IBaseObject* > vCheckpoints = CObjectManager::GetInstance()->FindObjectsWithType( OBJ_CHECKPOINT );

				for( unsigned i = 0; i < vCheckpoints.size(); ++i)
				{
					CCheckpoint*	pCheck = dynamic_cast< CCheckpoint* >( vCheckpoints[i] );
					if( pCheck->GetCheckpointBool() == true )
						pCheck->SetCheckpointBool( false );
				}
				SetCheckpointBool( true );
				return true;
			}
			else
				return false;
		}
		else
			return GetFrame().CheckCollision( (CBaseObject*)pBase );
}

void CCheckpoint::Update( float fElapsed )
{
	//GetFrame().Update( fElapsed );
	CBaseObject::Update( fElapsed );
}

void CCheckpoint::Render( void )
{
	//CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	//pD3D->GetSprite()->Flush();
	//CCamera* cam = CCamera::GetInstance();
	//RECT r = GetRect();
	//r.left -= (LONG)cam->OffsetX();
	//r.top -= (LONG)cam->OffsetY();
	//r.right -= (LONG)cam->OffsetX();
	//r.bottom -= (LONG)cam->OffsetY();
	//pD3D->DrawRect( r, 255, 0, 0);
	GetFrame().Render();
	CBaseObject::Render();
}

void CCheckpoint::AddAnimation( const char* szFileName, int nAnimationToUse)
{
	CAnimationManager*	pAM = CAnimationManager::GetInstance();

	//"Resources/Scripts/animation_anchors.xml"
	vector< std::string > vTemp = pAM->CreateAnimationFromFile
		( szFileName );

	GetFrame().Init( this );
	GetFrame().SetAnimationID( vTemp[ nAnimationToUse ] );

	CAnimationManager::GetInstance()->AddFrame(m_pCheck);
}

RECT CCheckpoint::GetRect()
{
	return m_pCheck.GetRect();
}