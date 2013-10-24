#pragma once
#include "TriggeredObject.h"
class CSpawnPoint :
	public CTriggeredObject
{

public:

			CSpawnPoint(void);
			~CSpawnPoint(void);

	void	Activate(void);
	void	Deactivate(void);
	void	Update(float fElapsed);
	void	Render();
	string	GetEvent() { return m_szEvent; }
	void	SetEvent( string e );
	void	HandleEvent( const CEvent* pEvent );
	int		GetEnemySpawnType()	{ return m_nEnemyType; }
	void	SetEnemySpawType( int type )	{ m_nEnemyType = type; }

private:

	string	m_szEvent;
	float	m_fElapsed;
	int		m_nSpawnRate;
	int		m_nEnemyType;

};

