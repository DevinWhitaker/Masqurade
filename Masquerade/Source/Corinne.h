#pragma once
#include "baseobject.h"
#include "Powerup.h"
#include "Frame.h"
#include <vector>

class IAIState;
#define PENALTY 500
enum CorinneAnimations { D_WALK, D_IDLE, D_SEEK, D_JUMP, D_INFLICTED, D_HELD, D_NUMAIMATIONS };


class CCorinne :
	public CBaseObject
{
private:
	CPowerup::PowerupType	m_eCurPowerup;
	CFrame					m_frCorinne;
	void					PickUpCheck( void );
	float					m_fOrigVel;
	float					m_fFoxMoveScalar;
	float					m_fNormalMaxXvel;
	float					m_fNormalMaxYvel;
	bool					m_bIsGreased;
	float					m_fOrigGravity;
	float					m_fElapsed;
	vector<string>			m_vIDs;
	bool					m_bIsInflicted;
	//float					m_fThrowDistance;
public:
	CCorinne(void);
	virtual ~CCorinne(void);
	virtual void HandleEvent( CEvent* pEvent );
	void Update(float fElapsed);
	void Render();
	bool CheckCollision(IBaseObject * pBase);
	void tileCollisionResponse(int COLLISION_SIDE);
	void CollisionResponse(  CBaseObject* pBase, RECT hitbox, RECT hit, POINT anchor  );
	void SetIdleAnimation();

	bool	&GetGreased(void)				{return m_bIsGreased;}
	void	SetGreased(bool b)				{m_bIsGreased = b;}

	float	&GetNormalMaxXvel(void)			{return m_fNormalMaxXvel;}
	void	SetNormalMaxXvel(float y)		{m_fNormalMaxXvel = y;}

	float	&GetNormalMaxYvel(void)			{return m_fNormalMaxYvel;}
	void	SetNormalMaxYvel(float y)		{m_fNormalMaxYvel = y;}

	float	&GetFoxMoveScalar(void)			{return m_fFoxMoveScalar;}
	void	SetFoxMoveScalar(float s)		{m_fFoxMoveScalar = s;}

	void setPowerup(CPowerup::PowerupType powerup);// { m_eCurPowerup = powerup; }
	CPowerup::PowerupType getPowerup(void) {return m_eCurPowerup;}

	CFrame		&GetFrame( void )		{ return m_frCorinne;		}
	void AddAnimation( const char* szFileName, int nAnimationToUse = 0 );
	void ResetHurtTimer(){ m_fElapsed = -1.0f; }
	RECT GetRect();
	bool isInflicted(){ return m_bIsInflicted; }
	float PenaltyTimeRemaining(){ return (float)PENALTY - m_fElapsed; }
};

