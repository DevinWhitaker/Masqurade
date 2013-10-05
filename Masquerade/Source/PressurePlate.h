// Project:		Masquerade
// Filename:	PressurePlate.h
// Author:		Jam'D Team
// Purpose:		Pressure Plate declaration.

#pragma once
#include "Trigger.h"

class CPressurePlate : public CTrigger
{
private:

public:
	CPressurePlate( void );
	CPressurePlate( CPressurePlate &cp );
	~CPressurePlate( void );

	void Activate( void );
	void Deactivate( void );
	void Update( float fElapsed );
	void Render( );
	bool CheckCollision( IBaseObject* pBase );
	RECT GetRect( );
};

