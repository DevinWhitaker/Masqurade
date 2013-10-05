#pragma once
#include "baseobject.h"

enum EnemyType { NME_CLOWN, NME_RAT, NME_HOODLUM };

class CEnemy :
	public CBaseObject
{	

public:

	CEnemy(void);
	virtual ~CEnemy(void);
	virtual bool CheckCollision(IBaseObject* pBase);
	virtual void CollisionResponse(CBaseObject* pBase, RECT hitBox, RECT hit, POINT anchor);


	void setEnemyType(int eType)	{m_nEnemyType = eType;}
	int getEnemyType(void)			{return m_nEnemyType;}
	virtual void Update(float fElapsed);

	virtual void HandleEvent(CEvent* pEvent);

protected:
	int m_nEnemyType;
	bool m_bIsAlive;
	int m_nHealth;
};

