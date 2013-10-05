#pragma once
#include "BaseObject.h"

#include "ObjectManager.h"

class CBalloon :
	public CBaseObject
{
public:
	enum BalloonColors { BALLOON_BLUE= 0, BALLOON_RED, BALLOON_GREEN };
private:
	int m_nBalloonColor;
	
public:
	CBalloon(void);
	~CBalloon(void);

	void SetBalloonColor(int color)		{ m_nBalloonColor = color; }
	int &GetBalloonColor(void)			{ return m_nBalloonColor; }

	void Activate(void);
	void Deactivate(void);
	void Update(float fElapsed);
	void Render(void);
	RECT GetRect();
	virtual bool CheckCollision( IBaseObject *pBase );
	virtual void HandleEvent( CEvent* pEvent );
	void tileCollisionResponse(int COLLISION_SIDE);


};

