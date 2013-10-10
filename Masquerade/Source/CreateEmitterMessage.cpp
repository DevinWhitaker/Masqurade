/*
	File:		CreateEmitterMessage.cpp
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Message to create an emitter.
*/

#include "CreateEmitterMessage.h"
#include "BaseObject.h"

CCreateEmitterMessage::CCreateEmitterMessage( std::string szEmitterName, char* chMapName, float fxParam, 
	float fyParam, CBaseObject* pBase, bool useCamera  )
	: m_szEmitterName( szEmitterName ), 
					m_chMapName( chMapName ), m_fXPos( fxParam ), m_fYPos( fyParam ), m_pBase( pBase ),
					m_bUseCamera( useCamera )
{

}


CCreateEmitterMessage::~CCreateEmitterMessage(void)
{

}
