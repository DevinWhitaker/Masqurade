//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MessageSystem.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles sending the messages to the specified proc function.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include <queue>
using std::queue;

#include "IMessage.h"

// Forward Declaration of the message class
class CMessage;

// Typedef for the MessageProc function type
typedef void (*MESSAGEPROC)(CMessage*);



class CSGD_MessageSystem
{
private:
	queue<CMessage*>			m_MsgQueue;				//	Stores my messages.
	MESSAGEPROC					m_pfnMsgProc;			//	Points to user defined function.

	CSGD_MessageSystem() { m_pfnMsgProc = NULL;	}
	CSGD_MessageSystem(const CSGD_MessageSystem&);
	CSGD_MessageSystem& operator=(const CSGD_MessageSystem&);

	~CSGD_MessageSystem() {}

public:
	
	static CSGD_MessageSystem* GetInstance(void);

	//	How many messages waiting to be processed.
	inline int GetNumMessages(void) { return (int)m_MsgQueue.size(); }

	//	Setup the function pointer for our messages.
	void InitMessageSystem(MESSAGEPROC pfnMsgProc);

	//	Sends the message into the queue and awaits processing later on through
	//	The ProcessMessages() function.
	void SendMsg(CMessage* pMsg);

	//	Processes all the messages that are waiting inside of the queue.
	//	Normally you only call this function once per frame.
	void ProcessMessages(void);

	//	Clears any messages that may be remaining
	void ClearMessages(void);

	//	Clears any messages that may be remaining and sets the function pointer to NULL.
	void ShutdownMessageSystem(void);
};