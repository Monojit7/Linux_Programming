#include <core/logging.h>

#ifdef PLATFORM_WIN32
#include <iostream>
using namespace std;
#else
#include <dlt/dlt.h>
#endif

std::unordered_map <uint16_t, const char* > Log::mContextMap;

#ifndef PLATFORM_WIN32

struct t_DltData
{
	DltContext context;
	DltContextData contextdata;
};

t_DltData* DLT_DATA = nullptr;

void log_dlt_message(uint16_t type, const char* str,DltLogLevelType level)
{
	if (dlt_user_log_write_start(&DLT_DATA[type].context, &DLT_DATA[type].contextdata, level))
	{
		dlt_user_log_write_string(&DLT_DATA[type].contextdata, str);
		dlt_user_log_write_finish(&DLT_DATA[type].contextdata);
	}
}
#endif

void Log::initLogging(const char* AppName, const char* AppDesc, const std::unordered_map <uint16_t, const char* > & contextmap)
{
	mContextMap.insert(std::make_pair(0, "Core")); 
	uint16_t size = contextmap.size();
	auto it = contextmap.begin(); 

	while (it != contextmap.end())
	{
	 mContextMap.insert(std::make_pair(it->first, it->second));
	 it++;
	}

#ifndef PLATFORM_WIN32
        DLT_DATA = (t_DltData*)malloc(sizeof(t_DltData)* (size+1) ); 
	dlt_register_app(AppName,AppDesc);	
	for (uint16_t i = 0; i < (size+1); i++)
	{
		dlt_register_context(&DLT_DATA[i].context, Log::mContextMap[i], "Context");
	}
	dlt_verbose_mode();
#endif
}

void Log::log_verbose(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_VERBOSE);
#endif
}

void Log::log_info(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_INFO);
#endif
}

void Log::log_debug(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_DEBUG);
#endif
}

void Log::log_warning(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_WARN);
#endif
}

void Log::log_error(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_ERROR);
#endif
}

void Log::log_fatal(uint16_t type, const char* str)
{
#ifdef PLATFORM_WIN32
	std::cout << Log::mContextMap[type] << " " << str << std::endl;
#else
	log_dlt_message(type, str, DLT_LOG_FATAL);
#endif
}


