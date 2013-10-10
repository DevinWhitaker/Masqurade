/*
	File:		DestroyEmitterMessage.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Message to destroy an emitter.
*/
#ifndef DESTROYEMITTERMESSAGE_H
#define DESTROYEMITTERMESSAGE_H

#include "../SGD Wrappers/IMessage.h"
#include <string>
using namespace std;

class CDestroyEmitterMessage : public IMessage
{
public:
	CDestroyEmitterMessage( std::string szMapName );
	virtual ~CDestroyEmitterMessage(void);
	std::string			&GetMapName( void )			{ return m_szMapName; }
	virtual MSGID		GetMessageID( void )		{ return MSG_DESTROY_EMITTER; }

private:
	std::string			m_szMapName;
};

#endif