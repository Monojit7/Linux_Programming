#include <core/IPluginBase.h>
#include <iostream>

class ITestplugin : public IPluginBase {
public:
	ITestplugin(std::string&& name) : IPluginBase(std::move(name)) {
	}

	virtual ~ITestplugin() {};

	virtual bool init() {
		std::cout << "init" << std::endl;
		return true;
	}
	virtual bool deInit() {
		std::cout << "deInit" << std::endl;
		return true;
	}
	virtual void testApi() {
		std::cout << "testapi default impl" << std::endl;
	};
};