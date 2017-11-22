/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PluginLoader.h
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
* @brief           Support for plugin load
*/

#ifndef PLUGINLOADER
#define PLUGINLOADER

#include <string>
#include <unordered_map>
#include <core/IPluginBase.h>

#ifdef __linux__ 
#include <dlfcn.h>
#endif

#ifdef PLATFORM_WIN32
#define LOADPLUGIN(A) LoadLibrary(A)
#define UNLOADPLUGIN(A) FreeLibrary(static_cast<HMODULE>(A))
#else
#define __stdcall
#define LOADPLUGIN(A) dlopen(A, RTLD_LAZY)
#define UNLOADPLUGIN(A) dlclose(A)
#endif

template<typename... ARGS>
class DLL_EXPORT PluginLoader
{
public:
	/**
	* @brief  Provide a unique name to plugin
	* On invoking this interfaces , provided plugin has implemented handlers for 
	* createPluginFunc and destroyPluginFunc , following is callback sequence to plugin
	* - createPluginFuncargs
	* - IPluginBase::init
	* Refer respective handler description for more details
	* @return IPluginBase* - success
	* nullptr - failure
	*/
	IPluginBase* LoadPlugin(std::string&& pluginPath, ARGS... input) {
		LOG_DEBUG(5, "Load plugin - %s",pluginPath.c_str());

		PluginInfo<ARGS...> *newPlugin = new PluginInfo<ARGS...>(
			std::move(pluginPath), input...);
		
		if (!newPlugin->isValid) {
			delete newPlugin;
			return nullptr;
		}
		
		std::string pluginName;
		newPlugin->getName(pluginName);
		if (pluginList.find(pluginName) == pluginList.end())
		{
			pluginList[pluginName] = std::move(newPlugin);
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Plugin entry parse success");
			return newPlugin->plugin;
		}
		else
		{
			LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "!!Plugin %s load error reason-reloaded or name not unique!!", \
				pluginPath.c_str());
			delete newPlugin;
			return nullptr;
		}
	}
	
	/**
	* On class destroy following is callback sequence to plugin
	* - destroyPluginFunc
	* - IPluginBase::deInit
	* Refer respective handler description for more details
	*/
	virtual ~PluginLoader() {
		for (auto& it : pluginList) {
			if (it.second) {
				delete it.second;
				it.second = 0;
			}
		}
	}
private:
	
	template<typename... INARGS>
	class PluginInfo
	{
	public:
		bool isValid;
		IPluginBase* plugin;

		PluginInfo(std::string&& pluginPath, INARGS...input) :
			pluginHandle(0),
			isValid(false),
			plugin(nullptr)
		{
			initPlugin(std::move(pluginPath), input...);
		}

		~PluginInfo() {
			deInitPlugin();
		}

		void getName(std::string& pluginName) {
			pluginName = "NONE";
			if (plugin)
			{
				plugin->getName(pluginName);
			}
		}

	private:
		void initPlugin(std::string&& pluginPath, INARGS...input) {
			isValid = false;
			do {
				pluginHandle = LOADPLUGIN(pluginPath.c_str());
				if (!pluginHandle) {
					LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "Plugin load fail");
					break;
				}
				LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Plugin load success");

				create = std::function<IPluginBase*(INARGS...)>(
					reinterpret_cast<IPluginBase*(__stdcall *)(INARGS...)>(GETENTRYHANDLER(pluginHandle)));
				if (!create) {
					LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "plugin entry parse fail");
					break;
				}

				destroy = GETDESTROYHANDLER(pluginHandle);
				if (!destroy) {
					LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "plugin exit parse fail");
					break;
				}

				plugin = create(input...);
				if (plugin) {
					plugin->init();
					LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "plugin loaded %s",
						plugin->getName().c_str());
					isValid = true;
				}
			} while (0);
		}

		void deInitPlugin() {
			if (plugin) {
				plugin->deInit();
				LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "plugin unloaded %s",
					plugin->getName().c_str());
				destroy(plugin);
				plugin = 0;
			}

			if (pluginHandle) {
				UNLOADPLUGIN(pluginHandle);
				pluginHandle = 0;
			}
		}

		std::function<IPluginBase*(INARGS...)> create;
		void* pluginHandle;
		destroyPluginFunc destroy;

	};
	std::unordered_map<std::string, PluginInfo<ARGS...>*> pluginList;
};

#endif
