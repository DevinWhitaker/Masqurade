/*
	File:		Message.h
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Base Class for all Messages
*/

#pragma once

enum MessageType {  MSG_CREATE_ENEMY, MSG_CREATE_BULLET, 
					MSG_DESTROY_BULLET, MSG_DESTROY_ENEMY,
					MSG_CREATE_PEMITTER, MSG_DESTROY_PEMITTER,
					MSG_RESET };

class CMessage
{
public:


	explicit CMessage(MessageType msgID);
	virtual ~CMessage(void);
	
	// Accessor
	MessageType GetMessageID(void) const	{ return m_msgID; }

private:
	MessageType m_msgID;
};