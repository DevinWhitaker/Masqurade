#pragma once
#include "baseobject.h"


class ITool :
	public CBaseObject
{
public:
	
	enum ToolType { TOOL_SQUIRTGUN, TOOL_BALLOONGUN, TOOL_COTTONCANDYLAUNCHER, MAX_TOOLS };
	

	ITool(void);
	virtual ~ITool(void);

	
	virtual bool Shoot(CBaseObject* firingEntity) = 0;
	virtual void Update(float fElapsed);
	virtual void Render(void);
	bool CheckCollision( IBaseObject* pOther );
	RECT GetRect(void);

	int getFacingDirection(void) {return m_nFacingDir;}
	// 1 == right
	// -1 == left
	void setFacingDirection(int dir) { m_nFacingDir = dir; }

	float getFireRate(void) {return m_fFireRate;}
	void setFireRate(float rate) {m_fFireRate = rate;}

	int getToolType(void) {return m_eToolType;}
	void setToolType(ToolType toolType) {m_eToolType = toolType;}

	void	SetIsPickedUp(bool isPickedUp)		{m_bIsPickedUp = isPickedUp;}
	bool	&GetIsPickedUp(void)				{return m_bIsPickedUp;} 

	void	SetIsChargable(bool b)				{m_bIsChargable = b;}
	bool	&GetIsChargable(void)				{return m_bIsChargable;} 

	float &getMinCharge(void) {return m_fMinCharge;}
	void setMinCharge(float m) {m_fMinCharge = m;}

	float &getMaxCharge(void) {return m_fMaxCharge;}
	void setMaxCharge(float m) {m_fMaxCharge = m;}

	float &getChargeTime(void) {return m_fChargeTime;}
	void setChargeTime(float c) {m_fChargeTime= c;}

	void	SetIsSelected(bool b)				{m_isSelected = b;}
	bool	&GetIsSelected(void)				{return m_isSelected;}



protected:
	float m_fFireRate;
	int m_eToolType;
	int m_nFacingDir;
	bool m_bIsPickedUp;

	bool  m_bIsChargable;
	float m_fChargeTime;
	float m_fMinCharge;
	float m_fMaxCharge;
	bool m_bIsCharging;
	bool m_bFuseLit;
	bool m_isSelected;
	CBaseObject* m_pFiringEntity;
};

