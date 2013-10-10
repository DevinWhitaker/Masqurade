#pragma once
#include "../SGD Wrappers/IMessage.h"
class CResetMessage :
	public IMessage
{
public:
	CResetMessage(void);
	~CResetMessage(void);
	virtual MSGID		GetMessageID( void )		{ return MSG_RESET; }
};

