/*
	File:		CreateEmitterMessage.h
	Course:		SGP1
	Author:		Ja'Michael Garcia
	Purpose:	Message to create an emitter.
*/

#ifndef CREATEEMITTERMESSAGE_H
#define CREATEEMITTERMESSAGE_H

#include "../SGD Wrappers/IMessage.h"
#include <string>
using namespace std;

class CBaseObject;
class CCreateEmitterMessage : public IMessage
{
public:
	CCreateEmitterMessage( std::string szEmitterName, char* chMapName, float fxParam = 0, float fyParam = 0,
						 CBaseObject* pBase = nullptr, bool useCamera = true );
	virtual ~CCreateEmitterMessage(void);

	//	Accessors
	CBaseObject*		&GetBaseObject( void )		{ return m_pBase;		}
	std::string			&GetEmitterName( void )		{ return m_szEmitterName;}
	char*				&GetMapName( void )			{ return m_chMapName;}
	float				&GetXPos( void )			{ return m_fXPos;		}
	float				&GetYPos( void )			{ return m_fYPos;		}
	bool				&GetUseCamera( void )		{ return m_bUseCamera;  }
	virtual MSGID		GetMessageID( void )		{ return MSG_CREATE_EMITTER; }

private:
	CBaseObject*		m_pBase;
	char*				m_chMapName;
	std::string			m_szEmitterName;
	float				m_fXPos;
	float				m_fYPos;
	bool				m_bUseCamera;
};

#endif