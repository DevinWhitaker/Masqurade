// Project:		Masquerade
// Filename:	TileManager.h
// Author:		Jam'D Team
// Purpose:		Tile Engine. Manages Tiles by loading, updating, and checking collision.

#pragma once

#include <vector>
#include <sstream>
#include "Tile.h"
//#include "ObjectManager.h"
//#include "FileManager.h"


#include "Camera.h"
#include "../SGD Wrappers/SGD_String.h"
#include "ImageObject.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"



class CCheckpoint;
class CObjectFactory;
class CObjectManager;

class CTileManager
{
public:
	static				CTileManager* GetInstance( );
	

	bool				CheckTileCollisions( CBaseObject* pBase );

	void				RegisterObjects();

	void				Render( );
	void				RenderFront( );
	void				RenderBack( );
	void				AddTile( CTile& pTile );
	IBaseObject*		GetTile( const int& iX, const int& iY );
	bool				LoadTileWorld( std::string filename, int nLevelNumber = 0 );
	bool				LoadObjects( std::string filename, int nLevelNumber,
										CFox*	pFoxData = nullptr, std::string szCheck = "");
	void				RemoveAllTiles( );

	void				AddImage( std::string szHandle, CImageObject pImageObject );

	int					&GetRenderCount( void )			{ return nRenderCounter; }
	void				SetRenderCount( int nCount )	{nRenderCounter = nCount; }
	int					GetWorldWidth() { return m_vTileList.size() * 32; };
	int					GetWorldHeight(){ return m_vTileList[0].size() * 32; }
private:
	CTileManager( );
	CTileManager( CTileManager& );
	CTileManager& operator=( CTileManager& );
	~CTileManager( );

	// Tile Render Counter Logic
	int nRenderCounter;

protected:
	// Tracking all Tiles and Image Objects
	std::vector< std::vector< CTile* > >	m_vTileList;
	std::map< std::string, CImageObject >	m_mImageList;

};