#pragma once
#include "Enemy.h"
#include "Frame.h"

class CRat :
	public CEnemy
{
public:
	CRat(void);
	virtual ~CRat(void);

	void Update(float fElapsed);
	void Render();
	bool CheckCollision(IBaseObject* pBase);

	void HandleEvent(CEvent* pEvent);
	void CollisionResponse( CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor );
	void tileCollisionResponse(int COLLISION_SIDE);

	CRat( CRat& rat );

	RECT GetRect();
	
	void Init(float xStartPos, float yStartPos);

	void setMoveDistance(int d)		{m_nMoveDistance = d;}
	int getMoveDistance(void)		{return m_nMoveDistance;}
	CFrame* m_pThunderbolt;
private:
	CFrame	m_ratFrame;

	int m_nMoveDistance;

	int m_nStartPosX;
	
};

