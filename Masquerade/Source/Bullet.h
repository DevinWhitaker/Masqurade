#pragma once
#include "BaseObject.h"

enum BulletType { SQUIRT_BULLET, BALLOON_BULLET, COTTON_BULLET, PIE_BULLET, MAX_BULLET_TYPES };

#define PIE_DURATION 3

class CBullet : public CBaseObject
{
public:
	CBullet( void );
	virtual ~CBullet( void );

	virtual void Update( float fElapsedTime );
	virtual void Render(void);
	virtual RECT GetRect(void);

	virtual bool CheckCollision( IBaseObject* pOther );
	void tileCollisionResponse(int COLLISION_SIDE);

	virtual void HandleEvent(CEvent* pEvent);


	void SetRotation( float rotation ) { m_nCurRotation = rotation; }
	float GetRotation(void) { return m_nCurRotation; }

	void SetOwner( CBaseObject* pOwner ) 
	{ 
		if(m_pOwner != nullptr)
		{
			m_pOwner->Release();
		}

		m_pOwner = pOwner; 
		if(m_pOwner != nullptr) m_pOwner->AddRef();
	}

	CBaseObject* GetOwner(void) { return m_pOwner; }

	void SetSoundHit(int h) {m_nHitSoundID = h;}

	int getBulletType(void) {return m_nBulletType;}
	void setBulletType(int bType) { m_nBulletType = bType; }

	bool getIsFlipped(void)				{return m_bIsFlipped;}
	void setIsFlipped(bool isFlipped)	{m_bIsFlipped = isFlipped;}

	float &GetTimeAlive(void)			{return m_fTimeAlive;}
	void  SetTimeAlive(float t)			{m_fTimeAlive = t;}

	float &GetLifeDuration(void)			{return m_fLifeDuration;}
	void  SetLifeDuration(float t)			{m_fLifeDuration = t;}

	float &GetObjectMovementScalar(void)			{return m_fObjectMovementScalar;}
	void  SetObjectMovementScalar(float t)			{m_fObjectMovementScalar = t;}
	void  PieLanded() { m_bPieLanded = true; }
	bool  HasPieLanded() { return m_bPieLanded; };
private:
	float m_nCurRotation;
	CBaseObject * m_pOwner;

	int m_nHitSoundID;

	int m_nDamage;
	int m_nBulletType;
	float m_fElapsed;
	bool m_bIsFlipped;
	float m_fTimeAlive;
	float m_fLifeDuration;
	bool m_bFuseLit;
	bool m_bPieLanded;
	float m_fObjectMovementScalar;
};