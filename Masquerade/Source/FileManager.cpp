#include "FileManager.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../tinyxml/tinyxml.h"
#include "BaseObject.h"
#include "Fox.h"
#include "Checkpoint.h"
#include "ITool.h"
#include "Powerup.h"
#include "BalloonGun.h"
#include "SquirtGun.h"
#include "CottonCandyLauncher.h"
#include "TileManager.h"
#include "GamePlayState.h"
CFileManager* CFileManager::GetInstance(void)
{
	static CFileManager s_Instance;

	return &s_Instance;
}


CFileManager::CFileManager(void)
{
	SetSaveToLoad( -1 );
	SetCurrentSaveToLoad( -1 );
	SetFadeBool( true );
	ResetTimer( );
	ResetAlpha();
}

CFileManager::~CFileManager(void)
{
}

	///////////////////////////////////////////////////////////////////
	//	Function:			"AddImageID"
	//
	//	Last Modified:		5/11/2012
	//
	//	Input:				std::string nameOfImage
	//
	//	Return:				void
	//
	//	Purpose:			Adds an id to the m_mImageMap map data member.
	///////////////////////////////////////////////////////////////////

void CFileManager::AddImageID( std::string nameOfImage, int id,  CBaseObject* pBase, bool UsingABaseObj )
{

	for( std::map< std::string, ImageStruct >::iterator i = m_mImageMap.begin();
		i != m_mImageMap.end(); ++i )
	{
		if( (*i).first.compare( nameOfImage ) == false )
			return;
	}

	if( UsingABaseObj == true )
	{
		ImageStruct tempStruct = { id, pBase, true };
		GetImageIdMap()[ nameOfImage ] = tempStruct;
	}
	else
	{
		ImageStruct tempStruct = { id, pBase, false };
		GetImageIdMap()[ nameOfImage ] = tempStruct;
	}

}

///////////////////////////////////////////////////////////////////
//	Function:			"AddSoundID"
//
//	Last Modified:		5/11/2012
//
//	Input:				std::string nameOfSoundFile
//						int id
//
//	Return:				void
//
//	Purpose:			Adds an id to the m_mSoundMap map data member.
///////////////////////////////////////////////////////////////////
void CFileManager::AddSoundID( std::string nameOfSoundFile, int id, bool MusicFile )
{

	for( std::map< std::string, SoundStruct >::iterator i = m_mSoundMap.begin();
		i != m_mSoundMap.end(); ++i )
	{
		if( (*i).first.compare( nameOfSoundFile ) == false )
			return;
	}

	if( MusicFile == true )
	{
		SoundStruct tempStruct = { id, true };
		GetSoundIdMap()[ nameOfSoundFile ] = tempStruct;
	}
	else
	{
		SoundStruct tempStruct = { id, false };
		GetSoundIdMap()[ nameOfSoundFile ] = tempStruct;
	}
}

bool CFileManager::LoadSettings( const char* szFileName, volumeStruct &vVolumes )
{
	TiXmlDocument doc;

	if( doc.LoadFile( szFileName ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return false;

	TiXmlElement* pScore = pRoot->FirstChildElement( "volume_info" );

	while( pScore != nullptr )
	{
		volumeStruct info = { };
		

		// Read the name (which is a Text node between the XML tags)
		const char* pText = pScore->GetText();

		// Read the attributes (within the XML tag)
		if( pScore->Attribute( "music_volume", &info.m_nMusicVolume ) == nullptr )
			info.m_nMusicVolume = 0;

		// Read the attributes (within the XML tag)
		if( pScore->Attribute( "sound_volume", &info.m_nSFXVolume ) == nullptr )
			info.m_nSFXVolume = 0;

		if( pScore->Attribute( "fullscreen", &info.b_FullScreen ) == nullptr )
			info.b_FullScreen = 0;

		//vVolumes.push_back( info );


		vVolumes = info;

		// Move to the next node in the volume_list
		pScore = pScore->NextSiblingElement( "volume_info" );
	}

	return true;
}

void CFileManager::SaveSettings( const char* szFileName, volumeStruct &vVolumes )
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Create a TinyXML Declaration ( version info )
	TiXmlDeclaration* pDec = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Add the declaration to the document
	doc.LinkEndChild( pDec );

	// Create a root node for the document ( player_list )
	TiXmlElement* pRoot = new TiXmlElement( "volume_list" );

	// Add the root node to the document
	doc.LinkEndChild( pRoot );

		TiXmlElement* pVolume = new TiXmlElement( "volume_info" );

		// Fill the node with player info
		pVolume->SetAttribute( "music_volume", vVolumes.m_nMusicVolume); // integer
		pVolume->SetAttribute( "sound_volume", vVolumes.m_nSFXVolume ); // integer
		pVolume->SetAttribute( "fullscreen", vVolumes.b_FullScreen );


		// Add the player node to the "player_list"
		pRoot->LinkEndChild( pVolume );
	

	// Save the XML document to the file
	doc.SaveFile( szFileName );
}

bool CFileManager::LoadGame(const char* szFileName, playerProgress &load, 
	const char* szWorldXML, bool bLoadWorld )
{
	TiXmlDocument doc;

	if( doc.LoadFile( szFileName ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == nullptr )
		return false;

	TiXmlElement* pFoxData = pRoot->FirstChildElement( "Player_Info" );

	while( pFoxData != nullptr )
	{
		playerProgress info = { };
		info.pFox = new CFox();
		

		// Read the name (which is a Text node between the XML tags)
		const char* pText = pFoxData->GetText();
		if( pFoxData->Attribute("FileName") == nullptr )
			info.szFileName = "";
		else
			info.szFileName = pFoxData->Attribute("FileName");

		if( pFoxData->Attribute( "XMLLevel" ) == nullptr )
			info.szLevelPath = "";
		else
			info.szLevelPath = pFoxData->Attribute( "XMLLevel" );
			// Read the attributes (within the XML tag)
		double xValue = 0;
		if( pFoxData->Attribute( "Position_X", ( &xValue ) ) == nullptr )
			info.pFox->SetX( -1.0f );
		else
			info.pFox->SetX( (float)xValue );

		xValue = 0;
		if( pFoxData->Attribute( "Position_Y", &xValue ) == nullptr )
			info.pFox->SetY( -1.0f );
		else
			info.pFox->SetY( (float)xValue );

		// Read the attributes (within the XML tag)
		if( pFoxData->Attribute( "Level", &info.nLevelNum ) == nullptr )
			info.nLevelNum = -1;

		std::string check = "";
		if( pFoxData->Attribute("Checkpoint" ) == nullptr )
			info.szCheckPoint = "";
		else
			info.szCheckPoint = pFoxData->Attribute("Checkpoint");

		TiXmlElement*	pToolElement = pFoxData->FirstChildElement( "ToolType" );

		while( pToolElement != nullptr )
		{

			int nTool = 0;
			pToolElement->Attribute( "ToolType", &nTool );

			switch( nTool )
			{
			case ITool::TOOL_BALLOONGUN:
				{
					CBalloonGun*	pBG = new CBalloonGun();
					info.pFox->AddATool( pBG );
				}
				break;
			case ITool::TOOL_COTTONCANDYLAUNCHER:
				{
					CCottonCandyLauncher*	pCCL = new CCottonCandyLauncher();
					info.pFox->AddATool( pCCL );
				}
				break;
			case ITool::TOOL_SQUIRTGUN:
				{
					CSquirtGun*	pSG		= new CSquirtGun();
					info.pFox->AddATool( pSG );
				}
				break;
			default:
				break;
			};

			pToolElement = pToolElement->NextSiblingElement( "ToolType" );

		}


		TiXmlElement*	pPowerElement = pFoxData->FirstChildElement("Powerup");

		while( pPowerElement != nullptr )
		{
			int nPU = -2;

			pPowerElement->Attribute(  "Powerup" ,  &nPU);
			switch( nPU )
			{
			case CPowerup::NORMAL:
				info.pFox->AddAPowerUp( CPowerup::NORMAL );
				break;
			case CPowerup::EMPTY:
				info.pFox->AddAPowerUp( CPowerup::EMPTY );
				break;
			case CPowerup::GREASED_PIG:
				info.pFox->AddAPowerUp( CPowerup::GREASED_PIG );
				break;
			case CPowerup::CHEETAH:
				info.pFox->AddAPowerUp( CPowerup::CHEETAH );
				break;
			case CPowerup::POGO:
				info.pFox->AddAPowerUp( CPowerup::POGO );
				break;
			case CPowerup::FLUBBER:
				info.pFox->AddAPowerUp( CPowerup::FLUBBER );
				break;
			case CPowerup::FIZZY_LIFTING_DRINK:
				info.pFox->AddAPowerUp( CPowerup::FIZZY_LIFTING_DRINK );
				break;
			default:
				break;
			};
			pPowerElement = pPowerElement->NextSiblingElement( "Powerup" );
		}


		load = info;

		pFoxData = pFoxData->NextSiblingElement( "Player_Info" );

	}

	std::vector< CPowerup::PowerupType > vPowerups = load.pFox->GetPowerups();


	for( unsigned i = 0; i <  vPowerups.size(); ++i )
	{
		if( vPowerups[0] == CPowerup::NORMAL )
			break;
		else if( vPowerups[i] == CPowerup::NORMAL )
		{
			CPowerup::PowerupType temp = vPowerups[0];
			vPowerups[0] = vPowerups[i];
			vPowerups[i] = temp;
			break;
		}
	}
		
		
	if( bLoadWorld == true )
	{
		if( CTileManager::GetInstance()->LoadTileWorld( load.szLevelPath, load.nLevelNum ) == false )
			return false;
		if( CTileManager::GetInstance()->LoadObjects(  load.szLevelPath, load.nLevelNum, load.pFox, load.szCheckPoint ) == false )
			return false;
	}

	CGamePlayState::GetInstance()->SetCurStage( load.nLevelNum );
		// Move to the next node in the volume_list

	return true;
}
void CFileManager::SaveGame(const char* szFileName, playerProgress &save)
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Create a TinyXML Declaration ( version info )
	TiXmlDeclaration* pDec = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Add the declaration to the document
	doc.LinkEndChild( pDec );

	// Create a root node for the document ( player_list )
	TiXmlElement* pRoot = new TiXmlElement( "Player_Data" );

	// Add the root node to the document
	doc.LinkEndChild( pRoot );

		TiXmlElement* pFoxInfo = new TiXmlElement( "Player_Info" );

		std::ostringstream oss;
		oss << szFileName;
		save.szFileName = oss.str().c_str();
		pFoxInfo->SetAttribute( "FileName", save.szFileName.c_str() );
		pFoxInfo->SetAttribute( "XMLLevel", save.szLevelPath.c_str() );
		pFoxInfo->SetAttribute( "Level", save.nLevelNum ); // user data
		pFoxInfo->SetAttribute( "Checkpoint",  save.szCheckPoint.c_str() ); // integer

		pFoxInfo->SetDoubleAttribute( "Position_X", save.pFox->GetX() );
		pFoxInfo->SetDoubleAttribute( "Position_Y", save.pFox->GetY() );
		//	Tools for Fox

		for( unsigned int i = 0; i < ITool::MAX_TOOLS; ++i )
		{
			if( save.pFox->GetATool( i ) == nullptr )
				break;

			TiXmlElement*	pTool = new TiXmlElement( "ToolType" );
			//doc.LinkEndChild( pTool );
			pTool->SetAttribute( "ToolType",  save.pFox->GetATool(i)->getToolType() );

			pFoxInfo->InsertEndChild( *pTool );
		}
		
		
		//	Powerups for Corinne
		std::vector< CPowerup::PowerupType > vPowerups = save.pFox->GetPowerups();

		//pFoxInfo->SetAttribute( "NumberOfPowerups",  vPowerups.size() ); // integer

		for( unsigned int i = 0; i < vPowerups.size(); ++i )
		{
			TiXmlElement*	pPower = new TiXmlElement( "Powerup" );

			pPower->SetAttribute( "Powerup",  vPowerups[i] ); // integer
			//doc.LinkEndChild( pPower );
			pFoxInfo->InsertEndChild( *pPower );
		}

		// Add the player node to the "player_list"
		pRoot->LinkEndChild( pFoxInfo);
	

	// Save the XML document to the file
	doc.SaveFile( szFileName );
	save.szFileName = szFileName;


	//if( GetSaves().size() < 3 )
	//	m_vSaves.push_back( save );
	//else
	//	m_
}

void CFileManager::GenerateVolume( volumeStruct vVolumes )
{
	// Generate default info
	volumeStruct volume1 = { 50, 50 }; // Aggregate member initializer list

	vVolumes = volume1;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CleanUpIds"
//
//	Last Modified:		5/11/2012
//
//	Input:		void
//
//	Return:		void
//
//	Purpose:		Cleans up all id's within the m_mImageMap data member to
//					prevent memory leaks and crashs.
///////////////////////////////////////////////////////////////////
void CFileManager::CleanUpIDs( void )
{
	for( std::map< std::string, ImageStruct >::iterator i = m_mImageMap.begin();
		i != m_mImageMap.end(); ++i )
	{
		if( (*i).second.bAmIUsingABaseObject == true )
			ReleaseTexture( (*i).second.nId, (*i).second.m_pBaseObject, true);
		else if( (*i).second.bAmIUsingABaseObject == false )
			ReleaseTexture( (*i).second.nId );
	}

	for( std::map< std::string, SoundStruct >::iterator i = m_mSoundMap.begin();
		i != m_mSoundMap.end(); ++i )
	{
		if( (*i).second.bMusicID == true )
			ReleaseMusic( (*i).second.nId );
		else
			ReleaseSFX( (*i).second.nId );
	}

	GetImageIdMap().clear();
	GetSoundIdMap().clear();
}

void CFileManager::ReleaseTexture(  int TextureID, CBaseObject* object, bool releaseObj  )
{
	if( TextureID != -1 )
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture( TextureID );
		TextureID = -1;
	}

	if( releaseObj == true )
		object->Release();

	object = nullptr;
}

void CFileManager::ReleaseMusic( int MusicID )
{
	if( MusicID  != -1 )
	{
		CSGD_XAudio2::GetInstance()->MusicStopSong( MusicID );
		CSGD_XAudio2::GetInstance()->MusicUnloadSong( MusicID );
		MusicID  = -1;
	}
}

void CFileManager::ReleaseSFX( int SFXID )
{
	if( SFXID  != -1 )
	{
		CSGD_XAudio2::GetInstance()->SFXStopSound( SFXID  );
		CSGD_XAudio2::GetInstance()->SFXUnloadSound( SFXID );
		SFXID  = -1;
	}
}

int CFileManager::GetImageID( std::string szFilename )
{
	for( std::map< std::string, ImageStruct >::iterator i = m_mImageMap.begin();
		i != m_mImageMap.end(); ++i )
	{
		if( (*i).first.compare( szFilename ) == false )
			return (*i).second.nId;
	}
	return -1;

	//return m_mImageMap[ szFilename ].nId;
}