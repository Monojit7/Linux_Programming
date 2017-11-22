/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PluginsBase.h
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
* @brief           Support class for plugins
*/

#ifndef _IPluginBase_
#define _IPluginBase_

#include <string>
#include <core/PresCtrlTypes.h>

class IPluginBase;

/**
* CreatePlugin should be implemented by a plugin for prescore to have an entry handler
* Client should do bare minimal initializations within
* Functionality start can be done within IPluginBase::init
*/

/**
* DestroyPlugin should be implemented by a plugin for prescore to have an exit handler
* Client should do cleanup.Plugin should be deleted if a new operator was used with createPluginFunc handler
* Functionality stop can be done within IPluginBase::deInit
*/
typedef void(*destroyPluginFunc)(IPluginBase* plugin);

#ifdef PLATFORM_WIN32
#define GETENTRYHANDLER(A) GetProcAddress(static_cast<HMODULE>(A), "CreatePlugin")
#define GETDESTROYHANDLER(A) (destroyPluginFunc)GetProcAddress(static_cast<HMODULE>(A), "DestroyPlugin")
#else
#define GETENTRYHANDLER(A) dlsym(A, "CreatePlugin")
#define GETDESTROYHANDLER(A) (destroyPluginFunc)dlsym(A, "DestroyPlugin")
#endif

/* 
* This class should be inherited by a plugin to implement int / deinit
*/
class DLL_EXPORT IPluginBase
{
public:
	/**
	* @brief  Provide a unique name to plugin
	*/
	IPluginBase(std::string&& name) :
		name(std::move(name)) {};

	virtual ~IPluginBase() {};

	/**
	* @brief  Plugin to initialize  
	*/
	virtual bool init()   = 0;

	/**
	* @brief  Callback for any cleanup to be done in plugin
	*/
	virtual bool deInit() = 0;

	void getName(std::string& pluginName) {
		pluginName = name;
	}

	std::string getName() {
		return name;
	}
private:
	std::string name;
};
#endif
