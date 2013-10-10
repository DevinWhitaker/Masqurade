/*
	File:		Message.cpp
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Base Class for all Messages
*/

#include "../SGD Wrappers/IMessage.h"


IMessage::IMessage(MessageType msgID)
{
	m_msgID = msgID;
}


IMessage::~IMessage(void)
{

}
