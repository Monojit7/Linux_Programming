// CoreTest.cpp : Defines the entry point for the console application.
//
#include "CoreTest.h"
#if 1

uint32_t presEventTestToken = 0;

void signalHandler(int signum)
{
	LOG_INFO(1, "signalHandler");

	PresCtrl::shutDown();
}

void testSubscribeEventFunction(string messaage)
{
	LOG_INFO(1, "Received test event! Message : %s\n", messaage.c_str());
	if (presEventTestToken) {
		//PresEventAbstraction::getInstance()->unSubscribe(DUMMY_PUBLISHER_EVENT, presEventTestToken);
		PRESEVENT_UNSUBSCRIBE(DUMMY_PUBLISHER_EVENT, presEventTestToken);
		LOG_INFO(1, "UPDATED Un subscribe : %s\n", messaage.c_str());
	}
}

RequestBase* createReqPlay()
{
	return new RequestPlay();
}

RequestBase* createReqPause()
{
	return new RequestPause();
}

RequestBase* createReqFFWd()
{
	return new RequestFfwd();
}

RequestBase* createDevInfo()
{
	return new DeviceInfoTask();
}

RequestBase* createDummyPublisher()
{
	return new DummyPublisher();
}

NodeStateTypes::NsmErrorStatus shutdownCallback(const NodeStateTypes::NsmShutdownType &type)
{
	LOG_INFO(1, "shutdown type - %d ", type);
	return NodeStateTypes::NsmErrorStatus::NsmErrorStatus_Ok;
}

keypresclient::keypresclient() :
	keypresproxy("local", "keypres.keysInst") {

	LOG_INFO(1, "keypresclient constructor");
	keypresproxy->getProxyStatusEvent().subscribe(
	[this](CommonAPI::AvailabilityStatus status) 
	{ 
		LOG_INFO(1, "proxyAvailability %d", status);

		if (status == CommonAPI::AvailabilityStatus::AVAILABLE) {
			keypresproxy->getKeyEventSelectiveEvent().subscribe([&](
				const std::shared_ptr<::v1::com::harman::pres::keyprestypes::tKey>& key) {

				cout << "key id - " << int(key->getId());
				cout << ", key state - " << key->getKeyState().c_str() << std::endl;

				if (std::shared_ptr<::v1::com::harman::pres::keyprestypes::tRotaryKnobKey> rkkey = 
					std::dynamic_pointer_cast<::v1::com::harman::pres::keyprestypes::tRotaryKnobKey>(key)) {
					
					cout << "rkkey step - " << int(rkkey->getSteps()) << std::endl;
				}
			});
		std::vector<keyprestypes::tKeyStates> allkeys;
		CommonAPI::CallStatus status;
		keypresproxy->subscribeKeys(std::move(allkeys), status);
		}
	});
}

int main()
{
	signal(SIGINT, signalHandler);
	//signal(SIGABRT, signalHandler);

	std::unordered_map<uint16_t, const char*> loggingContexts = {
		{ 1,"Media" }
		,{ 2, "Detection" }
		,{ 3, "Playback" }
		,{ 4, "Browse" }
	};

	std::unordered_map <uint64_t, Request> RequestTable = {
		{ e_REQUEST_DEVICEINFO_TASK, {"DeviceInfo",e_GROUP_DEVICEINFO_GROUP,0,0u}},
		{ e_REQUEST_PLAY,{ "RequestPlay",e_GROUP_MEDIA_PLAYBACK,e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES,0u } },
		{ e_REQUEST_PAUSE,{ "RequestPause",e_GROUP_MEDIA_PLAYBACK,e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES,e_REQUEST_PLAY } },
	    { e_REQUEST_FFWD,{ "RequestFFwd",e_GROUP_MEDIA_PLAYBACK,e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES,e_REQUEST_PLAY } },
	    { e_REQUEST_DUMMY_PUBLISHER,{ "DummyRequest",e_GROUP_MEDIA_PLAYBACK,e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES,0u } },
	};


	Log::initLogging("PresCoreTest", "Pres core test utility", loggingContexts); 
	CommonAPI::Runtime::setProperty("LibraryBase", "keyprestest");

	//Test persistence , this is currently handled only for target build
	LOG_INFO(1, "Persistence::Init - status %d", Persistence::Init("PresCoreTest"));
	std::string  value;
	
	//Test read a key passing null buffer
	LOG_INFO(1, "ReadKeyValue::Test read a key passing null buffer - status %d", Persistence::ReadKeyValue("Test", value));

	std::string  value1(10,'\0');	
	
	//Test read a key which is not present
	LOG_INFO(1, "ReadKeyValue::Test read a key which is not present- status %d", Persistence::ReadKeyValue("Test1", value1));

	//Read a value which was written earlier
	LOG_INFO(1, "ReadKeyValue::Test Read - status %d , value - %s ", Persistence::ReadKeyValue("Test", value1), value1.c_str());
	
	//Write a key and value
	value1 = "Testvalue";
	LOG_INFO(1, "WriteKeyValue::Test Write a key and value - %d bytes written", Persistence::WriteKeyValue("Test", value1));
	
	//Read after write
	std::string newvalue(20,'\0');
	LOG_INFO(1, "ReadKeyValue::Test Read - %d bytes read , value %s", Persistence::ReadKeyValue("Test", newvalue), newvalue.c_str());

	//Read the size of a value
	LOG_INFO(1, "ReadKeyValueSize::Test Read size - %d", Persistence::ReadKeyValueSize("Test"));

	//Write a key and value
	LOG_INFO(1, "WriteKeyValue::Test Write a key and value - %d bytes written", Persistence::WriteKeyValue("Test1", value1));

	//Delete a written key
	LOG_INFO(1, "DeleteKeyValue::Test delete a key and value - %d status", Persistence::DeleteKeyValue("Test1"));

	//Delete a key that is not available
	LOG_INFO(1, "DeleteKeyValue::Test delete a key and value which are not available - %d status", Persistence::DeleteKeyValue("Test2"));

	std::string newvalue1(20, '\0');
	LOG_INFO(1, "ReadKeyValue::Test Read a deleted key   %d bytes read , value %s", Persistence::ReadKeyValue("Test1", newvalue1), newvalue1.c_str());

	Persistence::DeInit();
	
	//Read after deinit	
	LOG_INFO(1, "ReadKeyValue::Test Read after deinit - %d", Persistence::ReadKeyValue("Test", newvalue));

	//PresCtrl::initPresControl(RequestTable,std::bind(shutdownCallback,std::placeholders::_1));
	PresCtrl::initPresControl(RequestTable);

	PresCtrl::addRequest(e_REQUEST_PLAY, std::bind(&createReqPlay), 0);
	PresCtrl::addRequest(e_REQUEST_PLAY, std::bind(&createReqPlay), 0);
	PresCtrl::addRequest(e_REQUEST_PAUSE, std::bind(&createReqPause), 0);
	PresCtrl::addRequest(e_REQUEST_PAUSE, std::bind(&createReqPause), 0);
	PresCtrl::addRequest(e_REQUEST_FFWD, std::bind(&createReqFFWd), 0);
	PresCtrl::addRequest(e_REQUEST_FFWD, std::bind(&createReqFFWd), 0);
	PresCtrl::addRequest(e_REQUEST_DEVICEINFO_TASK, std::bind(&createDevInfo), 0);
	PresCtrl::addRequest(e_REQUEST_PLAY, std::bind(&createReqPlay), 0);
	PresCtrl::addRequest(e_REQUEST_PLAY, std::bind(&createReqPlay), 0);
	PresCtrl::addRequest(e_REQUEST_PLAY, std::bind(&createReqPlay), 0);
	

	// Ttesting the externalEvent changes..

	ExternalEventHandler e;

#ifdef PLATFORM_WIN32
	cout << "Mainloop Thread id is " << GetCurrentThreadId();
#else
	cout << "Mainloop Thread id is " << std::this_thread::get_id();
#endif

	std::thread first([&e]() {cout << "running Thread1" << endl;
#ifdef PLATFORM_WIN32
	cout << "Thread1 id is " << GetCurrentThreadId() << endl;
#else
	cout << "Thread1 id is " << std::this_thread::get_id() << endl;
#endif
	MyData* mydata = new MyData(1, 55);
	PresCtrl::addExternalEvent(std::bind(&ExternalEventHandler::eventHandler1,&e,1,2));
#ifdef PLATFORM_WIN32
	Sleep(10000);
#else
	sleep(10);
#endif
	});

	std::thread second([&e]() {cout << "running Thread2" << endl;
#ifdef PLATFORM_WIN32
	cout << "Thread2 id is " << GetCurrentThreadId() << endl;
#else
	cout << "Thread2 id is " << std::this_thread::get_id() << endl;
#endif
	MyData* mydata = new MyData(2, 56);
	PresCtrl::addExternalEvent(std::bind(&ExternalEventHandler::eventHandler2, &e,1));
#ifdef PLATFORM_WIN32
	Sleep(10000);
#else
	sleep(10);
#endif
	});

	std::thread third([&e]() {cout << "running Thread3" << endl;
#ifdef PLATFORM_WIN32
	cout << "Thread3 id is " << GetCurrentThreadId() << endl;
#else
	cout << "Thread3 id is " << std::this_thread::get_id() << endl;
#endif
	MyData* mydata = new MyData(3, 57);
	PresCtrl::addExternalEvent(std::bind(&ExternalEventHandler::eventHandler3,&e));
#ifdef PLATFORM_WIN32
	Sleep(10000);
#else
	sleep(10);
#endif
	});


#ifdef PLATFORM_WIN32
	std::string validPlugin("testplugin.dll");
	std::string invalidPlugin("testplugin.dll1");
#else
	std::string validPlugin("libtestplugin.so");
	std::string invalidPlugin("libtestplugin.so1");;
#endif // PLATFORM_WIN32

	//Test valid plugin load
	PluginLoader<std::string, int> loader;
	ITestplugin* plugin = dynamic_cast<ITestplugin*>(loader.LoadPlugin(std::move(validPlugin), "inputstring", 10));
	if (plugin){
		plugin->testApi();
	}

	//Reload the same plugin , check for error
	loader.LoadPlugin(std::move(validPlugin), "inputstring", 10);

	//Test invalid plugin load
	loader.LoadPlugin(std::move(invalidPlugin), "inputstring", 10);

	//EventAbstraction Test
	//Subscribe for event
//	PresEventSubscriber<EventTestReplyType>::subscribe(new PresSubscribeObject<EventTestReplyType>
//		(DUMMY_PUBLISHER_EVENT,std::bind(&testSubscribeEventFunction, _1)));

	//PRESEVENT_SUBSCRIBE(EventTestReplyType,DUMMY_PUBLISHER_EVENT, testSubscribeEventFunction);//called from class function only
	presEventTestToken = PresEventAbstraction::getInstance()->subscribe<EventTestReplyType>(DUMMY_PUBLISHER_EVENT, std::bind(&testSubscribeEventFunction,  _1));
	//Dummy publisher
	//To test subscriber & publisher  test case : Create dummy publisher & run in main loop put a sample request
	PresCtrl::addRequest(e_REQUEST_DUMMY_PUBLISHER, std::bind(&createDummyPublisher), 0);

	//keypresclient capiclient;

	PresCtrl::mainLoop();

    return 0;
}

#else

int value = 0;

enum e_RequestIds
{
	e_REQUEST_1 = 1,
	e_REQUEST_2 = 2,
	e_REQUEST_3 = 3,
};

enum e_RequestGroups
{
	e_group1 = 0u,
	e_group2,
	e_group3
};

RequestBase* createReq1();
RequestBase* createReq2();
RequestBase* createReq3();

class Request1 : public RequestBase, public ITimerEventListener
{
public:

	Request1()
		: RequestBase(e_REQUEST_1, "e_REQUEST_1")
		, mTimerManager(*this)
		, cycles(10)
		, update(0)
	{

	}

	virtual void onRun()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
		timerid = mTimerManager.setTimer(CTimerInterface::TIMER_PERIODIC, 1000);
	}

	virtual void onCancelled()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}

	virtual void timerEvent(int timerid)
	{
		LOG_INFO(1, "%s update %d", __FUNCTION__,update);
		if (++update == cycles)
			mTimerManager.cancelTimer(timerid);
		//setCompleted();
	}

private:
	CTimerInterface mTimerManager;
	int cycles,update;
	int timerid;
};

class Request2 : public RequestBase
{
public:

	Request2()
		:RequestBase(e_REQUEST_2, "e_REQUEST_2")
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}

	virtual ~Request2()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}

	virtual void onRun()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
		setCompleted();
		value = 1;
		//PresCtrl::addRequest(e_REQUEST_3, std::bind(&createReq3), 0);
	}

	virtual void onCancelled()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}
};

class Request3 : public RequestBase
{
public:

	Request3()
		:RequestBase(e_REQUEST_3, "e_REQUEST_3")
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}

	virtual ~Request3()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}

	virtual void onRun()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
		setCompleted();
	}

	virtual void onCancelled()
	{
		LOG_INFO(1, "%s", __FUNCTION__);
	}
};

RequestBase* createReq1()
{
	return new Request1();
}

RequestBase* createReq2()
{
	return new Request2();
}

RequestBase* createReq3()
{
	return new Request3();
}

int main()
{
	std::unordered_map<uint16_t, const char*> loggingContexts = {
		{ 1,"coretest" }
	};

	Log::initLogging("PresCoreTest", "Pres core test utility", loggingContexts);

	std::unordered_map <uint64_t, Request> RequestTable = {
		{ e_REQUEST_1,{ "e_REQUEST_1",0,0,0u } }
		,{ e_REQUEST_2,{ "e_REQUEST_2",0,e_REQUEST_IGNORE_THIS_IF_OLD_ACTIVE,0u } }
		,{ e_REQUEST_3,{ "e_REQUEST_3",1,0,0u } }
	};

	LOG_INFO(1, "init pres core");
	PresCtrl::initPresControl(RequestTable);
	LOG_INFO(1, "init pres core done");
	
	PresCtrl::addRequest(e_REQUEST_1, std::bind(&createReq1), 0);
	PresCtrl::addRequest(e_REQUEST_1, std::bind(&createReq1), 0);
	//PresCtrl::addRequest(e_REQUEST_1, std::bind(&createReq1), 0);

	PresCtrl::mainLoop();

	return 0;
}
#endif