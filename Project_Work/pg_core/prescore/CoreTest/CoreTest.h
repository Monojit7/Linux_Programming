#include <csignal>
#include <core/PresCtrl.h>
#include <core/RequestBase.h>
#include <core/CTimerInterface.h>
#include <core/CThreadInterface.h>
#include <core/Persistence.h>
#include <core/CommonAPIClientAbstraction.h>
#include <core/PluginLoader.h>
#ifdef PLATFORM_WIN32
#else
#include<unistd.h>
#endif
#include<core/PresEventAbstraction.h>
#include "CoreTestPlugin.h"
#include "autogencode/src-gen/consumes/v1/com/harman/pres/keypresProxy.hpp"

using namespace std;
using namespace v1::com::harman::pres;

enum e_MediaRequestIds
{
	e_REQUEST_DEVICEINFO_TASK = 1,
	e_REQUEST_PLAY_ENGINE_TASK = 2,
	e_REQUEST_PLAY = 4,
	e_REQUEST_PAUSE = 8,
	e_REQUEST_FFWD = 16,
	e_REQUEST_FBWD,
	e_REQUEST_PLAY_SELECTION,
	e_REQUEST_SHUFFLE_CHANGE,
	e_REQUEST_NEW_SESSION_QUERY,
	e_REQUEST_CANCEL_SESSION,
	e_REQUEST_DUMMY_PUBLISHER,
	e_REQUEST_MAX
};


enum e_MediaRequestGroups
{
	e_GROUP_DEVICEINFO_GROUP = 0u,
	e_GROUP_MEDIA_PLAYBACK,
	e_GROUP_MEDIA_BROWSE
};

//Publisher supports this type of reply
typedef std::function<void(string message)>EventTestReplyType;
//Event type corresponds to the above reply
#define DUMMY_PUBLISHER_EVENT 0x200

class DeviceInfoTask : public RequestBase, public IThreadDataHandler
{
public:

	DeviceInfoTask()
		:RequestBase(e_REQUEST_DEVICEINFO_TASK, "DeviceInfo")
		, mThread(*this)
	{
		LOG_INFO(1, "Created Request Play");
	}

	virtual ~DeviceInfoTask()
	{
		LOG_INFO(1, "Destroyed Request Play");
	}

	virtual void onRun()
	{

		mThread.template startThread<int>(mMyThreadID, &ThreadFunc, 55);
		setIdle();
	}

	static void ThreadFunc(IThreadDataHandler::SendDataFunction & func, int MyData)
	{
		LOG_INFO(1, "Running Thread");
		func(new BaseData());
	}

	virtual void onData(int threadId, BaseData* data = nullptr)
	{
		LOG_INFO(1, "Thread ID : %d Data Received", threadId);
		setCompleted();
	}

	virtual void onCancelled()
	{

	}

private:
	static int mMyThreadID;
	CThreadInterface mThread;
};

int DeviceInfoTask::mMyThreadID = 0;

class RequestPlay : public RequestBase, public ITimerEventListener
{
public:

	RequestPlay()
		:RequestBase(e_REQUEST_PLAY, "Play")
		, mTimerManager(*this)
	{
		LOG_INFO(1, "Created Request Play");
	}

	virtual ~RequestPlay()
	{
		LOG_INFO(1, "Destroyed Request Play");
	}

	virtual void onRun()
	{
		LOG_INFO(1, "Running Request Play");
		mTimerManager.setTimer(CTimerInterface::TIMER_ONESHOT, 5000);
		setIdle();
	}

	virtual void timerEvent(int timerid)
	{
		//LOG_INFO(1, "Timer expired");
		setCompleted();
	}

	virtual void onCancelled()
	{

	}

private:

	CTimerInterface mTimerManager;
};

class RequestPause : public RequestBase, public ITimerEventListener
{
public:

	RequestPause()
		: RequestBase(e_REQUEST_PAUSE, "PauseRequest")
		, mTimerManager(*this)
	{

	}

	virtual void onRun()
	{
		LOG_INFO(1, "Running Request Pause");
		mTimerManager.setTimer(CTimerInterface::TIMER_ONESHOT, 5000);
	}

	virtual void onCancelled()
	{
		LOG_INFO(1, "Cancelled Request Pause");
	}

	virtual void timerEvent(int timerid)
	{
		//	LOG_INFO(1, "Timer expired");
		setCompleted();
	}

private:
	CTimerInterface mTimerManager;
};

class RequestFfwd : public RequestBase, public ITimerEventListener
{
public:

	RequestFfwd()
		: RequestBase(e_REQUEST_FFWD, "FFWDRequest")
		, mTimerManager(*this)
	{

	}

	virtual void onRun()
	{
		LOG_INFO(1, "Running Request Pause");
		mTimerManager.setTimer(CTimerInterface::TIMER_ONESHOT, 5000);
	}

	virtual void onCancelled()
	{
		LOG_INFO(1, "Cancelled Request FFWD");
	}

	virtual void timerEvent(int timerid)
	{
		//	LOG_INFO(1, "Timer expired");
		setCompleted();
	}

private:
	CTimerInterface mTimerManager;
};


/*
Example Dummy publisher which publishes the event onRun()

*/
class DummyPublisher : public RequestBase
{
public:

	DummyPublisher()
		: RequestBase(e_REQUEST_DUMMY_PUBLISHER, "DUMMYPUBLISHER")
	{
	}

	void test(string message)
	{
		LOG_INFO(1, "Received test event! Message : %s\n", message.c_str());
	}

	void onRun()
	{
		LOG_INFO(1, "Running dummy publisher ");
		string message = "Message from publisher!";
		PRESEVENT_PUBLISH(EventTestReplyType, DUMMY_PUBLISHER_EVENT, message);
		//PresEventAbstraction::getInstance()->publish<EventTestReplyType>(DUMMY_PUBLISHER_EVENT, message);
		setCompleted();
	}
	void onCancelled() {}
};



class MyData :public BaseData
{
public:
	MyData(int a, int b) :m(a)
		, n(b)
	{

	}
	int m;
	int n;
};

class ExternalEventHandler
{
public:

	ExternalEventHandler()
	{
		cout << "ExternalEventHandler::ExternalEventHandler" << endl;
	}

	void eventHandler1(int a, int b)
	{
		cout << "eventHandler1-a:" << a << " b:" << b << endl;
#ifdef PLATFORM_WIN32
		cout << "eventHandler1 Thread id is " << GetCurrentThreadId() << endl;
#else
		cout << "eventHandler1 Thread id is " << std::this_thread::get_id() << endl;
#endif
	}

	void eventHandler2(int a)
	{
		cout << "eventHandler2-a:" << a << endl;
#ifdef PLATFORM_WIN32
		cout << "eventHandler2 Thread id is " << GetCurrentThreadId() << endl;
#else
		cout << "eventHandler2 Thread id is " << std::this_thread::get_id() << endl;
#endif
	}

	void eventHandler3()
	{
		cout << "eventHandler3" << endl;

#ifdef PLATFORM_WIN32
		cout << "eventHandler3 Thread id is " << GetCurrentThreadId() << endl;
#else
		cout << "eventHandler3 Thread id is " << std::this_thread::get_id() << endl;
#endif
	}
};

class keypresclient
{
	CommonAPIClientAbstraction<keypresProxy> keypresproxy;
public:
	keypresclient();
};
