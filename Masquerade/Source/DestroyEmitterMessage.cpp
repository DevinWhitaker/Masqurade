/*
	File:		DestroyEmitterMessage.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Message to destroy an emitter.
*/

#include "DestroyEmitterMessage.h"
#include "ParticleEmitter.h"

CDestroyEmitterMessage::CDestroyEmitterMessage( std::string szMapName ) 
	: CMessage( MSG_DESTROY_PEMITTER ), m_szMapName( szMapName ) 
{
	//this->GetEmitter()->AddRef();
}


CDestroyEmitterMessage::~CDestroyEmitterMessage(void)
{
	//this->GetEmitter()->Release();
}
