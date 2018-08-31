/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCoreMainLoop.h
* @ingroup         presctrlcore
* @author          amit.yatheesh@harman.com
* @brief           Mainloop class that handles CAPI/timer/thread events
*				   
*/

#pragma once

#ifndef  PLATFORM_WIN32

#include <sys/epoll.h>
#include <unordered_map>
#include <core/IDataHandler.h>
#include <mutex>

enum eFdType
{
	E_UNKNOWN,
	E_CAPI_FD,
	E_TIMER_FD,
	E_THREAD_FD,
	E_EVENT_FD,
	E_CONTEXT_SWITCH_FD,
	E_EXIT_FD
};

class  CMainLoop
{
public:

	CMainLoop();

	~CMainLoop();

	static CMainLoop* Self();

	void addWatchFd(eFdType, uint32_t fd, uint32_t events = EPOLLIN);

	void removeWatchFd(uint32_t);
	
	void addWorkerThreadEvent(int, BaseData*);
	
	void addUserEventData(uint32_t, BaseData*);
	
	void addExternalEvent();
	
	void shutdown();

	void mainloop();

private:
	static CMainLoop* mSelf;
	int mEpollFd;
	int mThreadCtrlFds[2];
	int mEventCtrlFds[2];
	int mCtxSwitchCtrlFds[2];
	int mCtrlFds[2];
	std::unordered_map<uint32_t,eFdType> mFdMap;
	std::mutex mFdMapMutex;
};
#endif // ! PLATFORM_WIN32