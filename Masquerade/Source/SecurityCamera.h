#pragma once
#include "trigger.h"
#include "../SGD Wrappers/SGD_Math.h"
class CSecurityCamera :
	public CTrigger
{
public:

	CSecurityCamera(void);
	~CSecurityCamera(void);

	void Use(void);
	void Update(float fElapsed);
	void Render();
	bool CheckCollision(IBaseObject* pBase);

	float	GetSightRange()	{ return m_fSightRange; }
	void	SetSightRange( float range ) { m_fSightRange = range; }
	float	GetOrientation() { return m_fOrientation; }
	void	SetOrientation( float orient ) { m_fOrientation = orient; }
	float	GetRotationSpeed() { return m_fRotationSpeed; }
	void	SetRotationSpeed( float speed ) { m_fRotationSpeed = speed; }
	float	GetDuration() { return m_fDuration; }
	void	SetDuration( float duration ) { m_fDuration = duration; }

private:
	float		m_fSightRange;
	int			m_nStationsryID;
	int			m_nRotatingID;
	int			nDirection;
	float		m_fOrientation;
	float		m_fRotationSpeed;
	float		m_fDuration;
	tVector2D	m_vLookAt;
};

