#pragma once
#include "Enemy.h"
#include "Frame.h"
#include <vector>

#define PIE_RANGE	255.0f
#define PUNCH_RANGE 32.0f
#define MAX_PIE_INTERVAL 6
#define PIE_RATE 1.5f
#define TOO_LOW 30
#define FLEE_FROM_CLOWN_RANGE 64.0f

class CFox;
class CCorinne;

enum ClownAnimations { C_WALK, C_IDLE, C_THROW, C_FAINT, C_NUMAIMATIONS };


class CClown :
	public CEnemy
{
public:
	CClown(void);
	CClown( CClown& obj);
	virtual ~CClown(void);

	void Update(float fElapsed);
	void Render();
	bool CheckCollision(IBaseObject* pBase);
	void HandleEvent( CEvent* pEvent );
	RECT GetRect();

	void SetFox( CFox* fox ) { m_pFox = fox; }
	void SetCorinne( CCorinne* corinne ) { m_pCorinne = corinne; }
	CFox* GetFox(  ) { return m_pFox; }
	CCorinne* GetCorinne( ) { return m_pCorinne; }
	CFrame&	GetFrame(){ return m_frAnime; }

private:

	void ThrowPie();

	CFrame			m_frAnime;
	CFox*			m_pFox;
	CCorinne*		m_pCorinne;
	sDataToPassIn	m_sClownAndCorinne;
	float			m_fRandPie;
	float			m_fElapsed;
	vector<string>  m_vIDs;
};

