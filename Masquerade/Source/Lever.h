/*
	File:		Lever.h
	Course:		SGP2
	Author:		JAM'D Studios ( Ja'Michael Garcia )
	Purpose:	Allow the player to activate or deactivate certain objects
				in the game world.
*/

#pragma once
#include "trigger.h"
class CFrame;

class CLever : public CTrigger
{
public:
	CLever(void);
	CLever( CLever &obj );
	virtual~CLever(void);
	virtual void Use( void );
	virtual bool CheckCollision(IBaseObject* pBase);
	virtual void HandleEvent( const CEvent* pEvent );
};

