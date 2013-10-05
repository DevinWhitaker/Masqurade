// Project:		Masquerade
// Filename:	Tile.h
// Author:		Jam'D Team
// Purpose:		Tile Object declaration.

#pragma once
#include "BaseObject.h"
#include "../tinyxml/tinyxml.h"
#include <string>
#include "../SGD Wrappers/CSGD_TextureManager.h"

class CCamera;

enum TILE_TYPES { NONCOLLIDE = -1, COLLIDE = 1, GLASS_HAZARD, JUMP_NODE, STOP_NODE, GRAVITYON };
enum LAYER_TYPES { FRONT_LAYER = 1, PLAY_LAYER, MID_LAYER, BACK_LAYER  };

class CTile : public CBaseObject
{
private:
	CSGD_TextureManager*	m_pTM;
	CCamera*				m_pCamera;

	// Tile Logic
	int						m_nTileX;
	int						m_nTileY;
	int						m_nTileImageID;
	std::string				m_szFilename;
	std::string				m_szEvent;
	int						m_nTileLayerType;

public:
	friend class CTileManager;

	CTile( void );
	~CTile( void );

	void Update( float fElapsed );
	void Render( );
	bool CheckCollision( CBaseObject * pBase );

	// Accessors
	int GetTileX()			{ return m_nTileX; }
	int GetTileY()			{ return m_nTileY; }
	int GetTileType()		{ return m_nTileCollideType; }
	RECT GetRect( void );

	// Mutators
	void SetTile( int iTileX, int iTileY );
	void SetTileType( int nTileType ) { m_nTileCollideType = nTileType; }

protected:
	int						m_nTileCollideType;
	
};

