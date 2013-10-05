/*
	File:		Message.cpp
	Course:		SGD 1204
	Author:		afoy
	Purpose:	Base Class for all Messages
*/

#include "Message.h"


CMessage::CMessage(MessageType msgID)
{
	m_msgID = msgID;
}


CMessage::~CMessage(void)
{

}
