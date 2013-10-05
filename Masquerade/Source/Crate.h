#pragma once
#include "baseobject.h"

struct tVector2D;

class CCrate :
	public CBaseObject
{
private:
	bool  m_bIsMoovable;
	bool  m_bHasBalloon;
	float m_fTimeBalloonAttached;
	float m_fMaxTimeBalloon;
	CBaseObject* m_pControlingObject;
	float m_fDistanceToCO;
	float m_fGrabDistance;
	int m_nBalloonImageID;
	float m_fRaiseDistance;
	float m_fStartPosY;

public:

	float &GetRaiseDistance(void)	{return m_fRaiseDistance;}
	void SetRaiseDistance(float d) {m_fRaiseDistance = d;}

	float &GetGrabDistance(void)	{return m_fGrabDistance;}
	void SetGrabDistance(float d) {m_fGrabDistance = d;}

	bool &HasBalloon(void)			{return m_bHasBalloon;}
	void setHasBalloon(bool b)		{m_bHasBalloon = b;}

	bool &IsMoovable(void)			{return m_bIsMoovable;}
	void setIsMoovable(bool b)		{m_bIsMoovable = b;}

	float &GetMaxTimeBalloonAttached(void)	{return m_fMaxTimeBalloon;}
	void SetMaxTimeBalloonAttached(float t) {m_fMaxTimeBalloon = t;}

	float Distance(tVector2D x, tVector2D y);

	CCrate(void);
	virtual ~CCrate(void);

	void Update(float fElapsed);
	void Render(void);
	bool CheckCollision( IBaseObject* pOther );
	RECT CCrate::GetRect();

	bool AttachBalloon(void);
	bool DetachBalloon(void);
};

