#ifndef _COMMONAPI_MAIN_LOOP_ENGINE_
#define _COMMONAPI_MAIN_LOOP_ENGINE_

#include <CommonAPI/CommonAPI.hpp>
#include <thread>
#include <unordered_map>
#include <mutex>

class PresCtrlCore; 
class  CommonAPIMainLoopEngine
{
public : 

	CommonAPIMainLoopEngine(PresCtrlCore& core);

	std::shared_ptr<CommonAPI::MainLoopContext> getMainLoop()
	{
		return mainloopContext;
	}

	static void watchAddedCallback(CommonAPI::Watch* watch, const CommonAPI::DispatchPriority dispatchPriority);

	static void watchRemovedCallback(CommonAPI::Watch* watch);

	static void handleEvent(uint32_t fd,uint32_t revents);

private : 

	static CommonAPIMainLoopEngine* mpMySelf; 
	static std::mutex mWatchMutex;

	/// Polls for the registered FD, on event receipt, will notify listeners. 
	static void pollLoop(CommonAPI::Watch* watch);
	static std::thread* mWatcherThread; 
	static std::unordered_map<uint32_t, CommonAPI::Watch*> mWatches;
	std::shared_ptr<CommonAPI::MainLoopContext> mainloopContext;
	PresCtrlCore& mCore;

};

#endif
