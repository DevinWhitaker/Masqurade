// Project:		Masquerade
// Filename:	Spring.h
// Author:		Jam'D Team
// Purpose:		Spring Object declaration.

#pragma once
#include "BaseObject.h"

class CSpring : public CBaseObject
{
private:

public:
	CSpring( void );
	CSpring( CSpring &cs );
	~CSpring( void );

	RECT GetRect();

	void Activate( void );
	void Deactivate( void );
	void Update(  float fElapsed );
	void Render();
	bool CheckCollision( IBaseObject* pBase );
};

