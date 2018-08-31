#include "CoreTestPlugin.h"
#include <iostream>

#ifdef PLATFORM_WIN32
extern "C" __declspec(dllexport) IPluginBase* CreatePlugin(std::string,int);
extern "C" __declspec(dllexport) void DestroyPlugin(IPluginBase* base);
#else
extern "C" IPluginBase* CreatePlugin(std::string, int);
extern "C" void DestroyPlugin(IPluginBase* base);
#endif

class testplugin : public ITestplugin {
public:
	testplugin() :ITestplugin("test") {
	}

	virtual ~testplugin() {};

	bool init() {
		std::cout << "init" << std::endl;
		return true;
	}
	bool deInit() {
		std::cout << "deInit" << std::endl;
		return true;
	}
	void testApi()
	{
		std::cout << "testApi - plugin impl" << std::endl;
	}
};

IPluginBase* CreatePlugin(std::string in,int input) {
	std::cout << "in -" << in.c_str() << input << std::endl;
	return new testplugin;

}

void DestroyPlugin(IPluginBase *base) {
	delete base;
}
