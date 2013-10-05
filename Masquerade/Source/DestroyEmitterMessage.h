/*
	File:		DestroyEmitterMessage.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Message to destroy an emitter.
*/
#ifndef DESTROYEMITTERMESSAGE_H
#define DESTROYEMITTERMESSAGE_H

#include "Message.h"
#include <string>
using namespace std;

class CDestroyEmitterMessage : public CMessage
{
public:
	CDestroyEmitterMessage( std::string szMapName );
	virtual ~CDestroyEmitterMessage(void);
	std::string			&GetMapName( void )			{ return m_szMapName; }

private:
	std::string			m_szMapName;
};

#endif