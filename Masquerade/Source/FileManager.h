#pragma once


#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

class	CBaseObject;
class	CFox;
class	CCheckpoint;

class CFileManager 
{
public:
	struct playerProgress
	{
		CFox*			pFox;
		int				nLevelNum;
		std::string		szCheckPoint;
		std::string		szFileName;
		std::string		szLevelPath;
	};
private:
	CFileManager(const CFileManager&);
	CFileManager& operator=(const CFileManager&);
	CFileManager(void);
	virtual ~CFileManager(void);

	int m_nSaveToLoad;
	struct ImageStruct
	{
		int		nId;
		CBaseObject*	m_pBaseObject;
		bool	bAmIUsingABaseObject;
	};

	std::map<std::string, ImageStruct > m_mImageMap;

	struct SoundStruct
	{
		int nId;
		bool bMusicID;
	};

	std::map<std::string, SoundStruct > m_mSoundMap;

	std::vector< playerProgress > m_vSaves;

	float	m_fTimer;
	bool	m_bFadeEffect;
	long	m_lAlpha;
	int	m_unCurrentSave;

public:
	
	static CFileManager* GetInstance(void);

	std::vector< playerProgress >	&GetSaves( void ) { return m_vSaves; }
	void ReleaseTexture( int TextureID, CBaseObject* object = nullptr, bool releaseObj = false );
	void ReleaseMusic( int MusicID );
	void ReleaseSFX( int SFXID );
	
	int GetSaveToLoad( void ) { return m_nSaveToLoad; }
	void	SetSaveToLoad( int nLoad ) { m_nSaveToLoad = nLoad; }

	
	struct volumeStruct
	{
		int m_nMusicVolume;
		int m_nSFXVolume;
		int b_FullScreen;

		int		GetSFXVolume( void ) const			{ return m_nSFXVolume;		}
		int		GetMusicVolume( void ) const		{ return  m_nMusicVolume;	}

		void	SetSFXVolume( int vol )		{ m_nSFXVolume = vol;	}
		void	SetMusicVolume( int vol )	{ m_nMusicVolume = vol;	}
	};

	bool LoadSettings( const char* szFileName, volumeStruct &vVolumes );
	void SaveSettings( const char* szFileName, volumeStruct &vVolumes );
	void GenerateVolume( volumeStruct vVolumes );

	void LoadLevel(std::string szWorldFilename, int LevelNum);



	bool LoadGame(const char* szFileName, playerProgress &load, 
		const char* szWorldXML = "", bool bLoadWorld = true);
	void SaveGame(const char* szFileName, playerProgress &save);
	void Shutdown();

	void LoadAssets(std::vector<int> ImageIDVec, std::vector<int> SoundIDVec);
	void UnloadAssets(std::vector<int> ImageIDVec, std::vector<int> SoundIDVec);

	///////////////////////////////////////////////////////////////////
	//	Function:			"AddImageID"
	//
	//	Last Modified:		5/11/2012
	//
	//	Input:				std::string nameOfImage
	//						int id
	//
	//	Return:				void
	//
	//	Purpose:			Adds an id to the m_mImageMap map data member.
	///////////////////////////////////////////////////////////////////
	void AddImageID( std::string nameOfImage, int id, CBaseObject* pBase = nullptr, bool UsingABaseObj = false );

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
	void AddSoundID( std::string nameOfSoundFile, int id, bool MusicFile = false );

	///////////////////////////////////////////////////////////////////
	//	Function:	"CleanUpIds"
	//
	//	Last Modified:		5/11/2012
	//
	//	Input:		void
	//
	//	Return:		void
	//
	//	Purpose:		Cleans up all id's within the m_mImageMap and
	//					m_nSoundMap data member to
	//					prevent memory leaks and crashs.
	///////////////////////////////////////////////////////////////////
	void CleanUpIDs( void );

	//	Accessor
	std::map< string, ImageStruct > &GetImageIdMap( void )
	{ return m_mImageMap; }

	std::map< string, SoundStruct > &GetSoundIdMap( void )
	{ return m_mSoundMap; }

	int GetImageID(std::string szFilename);
	//{ return m_mImageMap[szFilename].nId; }

	int GetSoundID(std::string szFilename)
	{ return m_mSoundMap[szFilename].nId; }
	
	inline	float	GetTimer( void ) const { return m_fTimer; }
	inline	void	IncrementTimer( float fElapsed ) { m_fTimer += fElapsed; }
	inline	void	ResetTimer( void ) { m_fTimer = 0.0f; }

	inline	bool	GetFadeBool( void ) const { return m_bFadeEffect; }
	inline	void	SetFadeBool( bool bFade ) { m_bFadeEffect = bFade; }

	inline	long	GetAlpha( void ) const { return m_lAlpha; }
	inline	void	IncrementAlpha( void ) { m_lAlpha += 1; }
	inline	void	ResetAlpha( void ) { m_lAlpha = 0; }

	inline	int GetCurrentSaveToLoad( void ) const { return m_unCurrentSave; }
	inline	void		 SetCurrentSaveToLoad( int unSave ) { m_unCurrentSave = unSave; }

};

