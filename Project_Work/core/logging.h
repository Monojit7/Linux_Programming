/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            logging.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Logging abstraction. Ensures DLT is used on Linux, stdout on Windows. 
*/

#ifndef LOGGING_H
#define LOGGING_H

#include <cstdio>
#include <iostream>
#include <core/PresCtrlTypes.h>
#include <stdint.h>
#include <unordered_map>

#define LOG_VERBOSE(A,B,...)         { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_verbose(A,LogBuffer); }
#define LOG_INFO(A,B,...)            { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_info(A,LogBuffer); }
#define LOG_DEBUG(A,B,...)           { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_debug(A,LogBuffer); }
#define LOG_WARNING(A,B,...)         { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_warning(A,LogBuffer); }
#define LOG_ERROR(A,B,...)           { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_error(A,LogBuffer); }
#define LOG_FATAL(A,B,...)           { char LogBuffer[1024];snprintf(LogBuffer,1024, B, ## __VA_ARGS__) ; Log::log_fatal(A,LogBuffer); }

class DLL_EXPORT Log
{
public:

	/**
	* Call this method once to initialize the logging related data structures. 
	* 0 is reserved for Core in the Context map passed. User must use 1 as the starting point for Context ID. 
	*/
	static void initLogging(const char* AppName,const char* AppDesc,const std::unordered_map <uint16_t,const char* > & );

	/**
	* Can be called directly by the user with the log message and the ID. But, use of logging macros is highly recommended. 
	*/
	static void log_verbose(uint16_t type, const char* str);

	static void log_info(uint16_t type,const char* str);

	static void log_debug(uint16_t type, const char* str);

	static void log_warning(uint16_t type, const char* str);

	static void log_error(uint16_t type, const char* str);

	static void log_fatal(uint16_t type, const char* str);

	static const uint16_t e_LOG_CONTEXT_CORE = 0u; 

private : 
	static std::unordered_map <uint16_t, const char* > mContextMap;
};


#endif // LOGGING_H
