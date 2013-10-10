/*
	File:		../SGD Wrappers/IMessage.h
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Base Class for all Messages
*/

#pragma once

enum MessageType {  MSG_CREATE_ENEMY, MSG_CREATE_BULLET, 
					MSG_DESTROY_BULLET, MSG_DESTROY_ENEMY,
					MSG_CREATE_PEMITTER, MSG_DESTROY_PEMITTER,
					MSG_RESET };

class IMessage
{
public:


	explicit IMessage(MessageType msgID);
	virtual ~IMessage(void);
	
	// Accessor
	MessageType GetMessageID(void) const	{ return m_msgID; }

private:
	MessageType m_msgID;
};