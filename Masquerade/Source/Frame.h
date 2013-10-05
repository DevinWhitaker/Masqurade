#pragma once

#include <string>
#include <Windows.h>
#include <vector>
using namespace std;

class CBaseObject;


class CFrame 
{
	friend class CAnchorAnimation;
	friend class CCellAnimation;
	friend class CAnimationManager;
private:
	CBaseObject*	m_pOwner;
	int				m_nCurrFrame;
	float			m_fElapsed;
	float			m_fTPerFrame;
	bool			m_bIsPlaying;
	bool			m_bIsLooping;
	bool			m_bIsFlipped;
	bool			m_bReverse;
	float			m_fSpeed;
	float			m_fScaleX;
	float			m_fScaleY;
	float*			m_nX;
	float*			m_nY;
	unsigned int*	m_nWidth;
	unsigned int*	m_nHeight;
	int				m_nStartFrame;
	DWORD			dwColor;
	
	std::string		m_nAnimationID;
	
public:
	CFrame(void);
	~CFrame(void);

	////////////////////////////////////////////////////////////
	//Accessors
	float GetX(){return *m_nX;};
	float GetY(){return *m_nY;};
	int	  GetWidth(){return *m_nWidth;};
	int	  GetHeight(){return *m_nHeight;};
	bool  isPlaying(){return m_bIsPlaying;};
	bool  isReversed(){ return m_bReverse; }
	bool  isLooping(){return m_bIsLooping;};
	bool  isFlipped(){return m_bIsFlipped;};
	float GetSpeed(){return m_fSpeed;};
	float GetYScale(){return m_fScaleY;};
	float GetXScale(){return m_fScaleX;};
	int	GetCurFrame(){ return m_nCurrFrame; }
	string GetAnimationID(){return m_nAnimationID;};
	DWORD GetColor(){return dwColor;};
	RECT  GetRect();
	CBaseObject*	GetOwner()	{ return m_pOwner; }
	vector<RECT> GetCollisionBounds();
	////////////////////////////////////////////////////////////
	//Mutators
	void SetPlaying( bool playing ){ m_bIsPlaying = playing; };
	void Reverse( bool reverse = true  ){ m_bReverse = reverse; }
	void SetLooping( bool looping ){ m_bIsLooping = looping; };
	void SetFlipped( bool flipped ){ m_bIsFlipped = flipped; };
	void SetSpeed( float speed ){ m_fSpeed = speed; };
	void SetYScale( float scale ){ m_fScaleY = scale; };
	void SetXScale( float scale ){ m_fScaleX = scale; };
	void SetAnimationID( string id );
	void SetColor( DWORD color ){ dwColor = color; };
	void SetCurrFrame( int frame ) { m_nCurrFrame = frame; }
	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Init(CBaseObject* attach_to);

	void Init(float* X, float* Y, unsigned int* Width, unsigned int* Height );

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Update(float fElapsed);

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Render();

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Play(bool bLooping = false, float fSpeed = 1.0f);

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Stop();

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	void Reume();

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	bool CheckCollision(CBaseObject* pBase);

	////////////////////////////////////////////////////////////
	//Purpose:
	//
	//In:
	//
	//Return:
	////////////////////////////////////////////////////////////
	bool CheckCollision(CFrame* pBase);
};


