#pragma once
#include "TriggeredObject.h"
#include "Frame.h"
#include "../SGD Wrappers/IListener.h"
class CAnimationManager;
class CGate :
	public CTriggeredObject
{
private:
	string				m_szEvent;
	CFrame				m_frAnime;
	bool				m_bIsToggleable;
	bool				m_bIsActivated;
	CAnimationManager*	m_pAM;
	vector<string>		m_vGateIDs;
	vector< string  > &GetGateIDs( void ) { return m_vGateIDs; }
public:
	CGate				(void);
	CGate				(CGate& obj);
	~CGate				(void);
	bool	GetToggleable		()					{ return m_bIsToggleable; }
	bool	GetActivated		()					{ return m_bIsActivated;  }
	void	SetToggleable		( bool set )		{ m_bIsToggleable = set;  }
	void	SetEvent			( string e );
	void	Activate			(void);
	void	Deactivate			(void);
	void	Update				(void);
	void	Render				(float fElapsed);
	virtual void	CollisionResponse	( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor );
	virtual void	HandleEvent			( const CEvent* pEvent );
	CFrame GetGateFrame( void ) { return m_frAnime; }

	//	0 for Vertical | 1 for Horizontal
	void	SetGateFrame( unsigned int nID );

	inline void	ClosedGate( void ) 
	{ 
		m_frAnime.SetCurrFrame( 0 ); 
		SetActivatedBool( false );
	}
	inline void OpenedGate( void ) 
	{ 
		m_frAnime.SetCurrFrame( 5 ); 
		SetActivatedBool( true );
	}
};

