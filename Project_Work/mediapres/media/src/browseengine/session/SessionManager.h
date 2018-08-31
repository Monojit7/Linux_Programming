/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionManager.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This provides the interfaces which can be used by clients for managing browse sessions.
*/

#ifndef _SESSIONMANAGER_H
#define _SESSIONMANAGER_H

#include <v3/com/harman/mmpres/MediaBrowserStub.hpp>
#include <browseengine/session/Session.h>
#include <detectionengine/MediaDetectionEngine.h>

class SessionQuery;
class MediaDetectionEngine;
class SessionManager
{

public:
	uint32_t openSession();
	uint32_t registerSessionQuery(uint32_t,SessionQuery*); 
	bool closeSession(uint32_t sessionId);
	bool cancelSession(uint32_t sessionId);
	Session* getSession(uint32_t sessionId);
	std::unordered_map <uint32_t, Session*>& getSessionList();
private:

	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{
		
	}
	friend class FacadeHelper<SessionManager>;

	uint32_t mLastSessionId;
	SessionManager();
	~SessionManager();
	std::unordered_map <uint32_t ,Session*> mSessionList;
	FacadeHelper<MediaDetectionEngine> mDetect; 
};

#endif
