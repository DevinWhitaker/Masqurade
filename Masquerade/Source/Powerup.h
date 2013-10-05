#pragma once
#include "baseobject.h"

class CPowerup :
	public CBaseObject
{
public:

	enum PowerupType { EMPTY = -1, NORMAL = 0, GREASED_PIG, CHEETAH, POGO, FLUBBER, FIZZY_LIFTING_DRINK, MAX_POWERUPS };

	

	CPowerup(void);
	virtual ~CPowerup(void);
	
	bool CheckCollision( IBaseObject* pOther );

	void	SetPowerupType(PowerupType pow)		{m_eCurrPowerupType = pow;}
	PowerupType	&GetPowerupType(void)			{return m_eCurrPowerupType;}
	void Render(void);

	void	SetIsPickedUp(bool isPickedUp)		{m_bIsPickedUp = isPickedUp;}
	bool	&GetIsPickedUp(void)				{return m_bIsPickedUp;}

private:
	PowerupType m_eCurrPowerupType;
	bool m_bIsPickedUp;


};

