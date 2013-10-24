#pragma once
#include "Enemy.h"
#include "Frame.h"
#include <vector>

class CFox;
class CCorinne;

#define FLEE_FROM_FOX_RANGE 128.0f
#define GRAB_CORINNE_RANGE  32.0f
#define FLEE_FROM_HOOD_RANGE 64.0f
#define GRAB_CHANCE 50

enum HoodlumAnimations { H_IDLE, H_RUN, H_PICKUP, H_FAINT, H_NUMAIMATIONS };


class CHoodlum :
	public CEnemy
{
public:
	CHoodlum(void);
	CHoodlum( CHoodlum& obj);
	virtual ~CHoodlum(void);

	void Update(float fElapsed);
	void Render();
	bool CheckCollision(IBaseObject* pBase);
	void HandleEvent( const CEvent* pEvent );

	RECT GetRect();
	void SetFox( CFox* fox ) { m_pFox = fox; }
	void SetCorinne( CCorinne* corinne ) { m_pCorinne = corinne; }
	CFox* GetFox(  ) { return m_pFox; }
	CCorinne* GetCorinne( ) { return m_pCorinne; }
	CFrame	GetFrame( void ) const { return m_frAnime; }

private:
	bool			m_bHasCorinne;
	CFrame			m_frAnime;
	CFox*			m_pFox;
	CCorinne*		m_pCorinne;
	sDataToPassIn	m_sHoodAndCorinne;
	vector<string>  m_vIDs;
};

