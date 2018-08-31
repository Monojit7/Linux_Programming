#pragma once        // to have just one inclusion of this file in the compilation phase

// using the logging context of the media component as otherwise we would overwrite media app with RCD in the DLT and below logger is not used at present will be made active when this is done as plugin
#include <core/logging.h>
#define LOG_CONTEXT_RCD_PLUGIN  ( 6 )   // value same as what is defined in core.h, this weird hack is to reduce the compilation time for remotecd modules in media

// commented out as diverted to core logging in context of mpres process

#if 0
#include <dlt/dlt.h>
#include <string>
#include <cstdarg>	// for variable list arguments functions
#include <map>

#define LOG_MSG_MAXSIZE			( 1024 )   // 1kb
#define APP_COMPONENT_ID		( "RCD" )  // Id & name of the app used to register in the init of the logger - dlt.
#define APP_COMPONENT_NAME		( "FORDRemoteCDComponent" )

// namespace declaratives
using namespace std ;

// This class integrates dlt logger utility for more functionality check the headers of dlt in ..toolchain/usr/include/dlt

// populated & used only by dlt lib to get the context of the log message we ask it to write..
typedef struct t_DltData {

	DltContext 	    context ;
    DltContextData  contextdata ;
} t_DltData;

class RemoteCDLog
{
	public:
        // constructor
        RemoteCDLog () ;

		enum eRemoteCDLogContexts {
			eLogBrowser,
			eLogCacheBuild,
			eLogCache,
			eLogTP,
			eLogManager,
			eLogPlayControls,
		    eCDLogMaxContext
		};
		
		// different log levels supported, we directly used the dlt enums, so as to avoid internal mapping again.
		enum LogLevelType {
			
			LOG_DEFAULT = DLT_LOG_DEFAULT,   // Default log level i.e., log level of the application is used
			LOG_ERROR   = DLT_LOG_ERROR,     // error with impact to correct functionality
			LOG_WARN    = DLT_LOG_WARN,      // warning, correct behaviour could not be ensured 
			LOG_INFO    = DLT_LOG_INFO,      // informational
			LOG_DEBUG   = DLT_LOG_DEBUG,   	 // debug
		} ;

		// dlt context handles only to be used by dlt lib.
		t_DltData 	dltData_ [ eCDLogMaxContext ] ;

		// map to hold the contextId & contextName which would be used by the logger - dlt, while logging the message buffer we send to it.
		map < string, string >   logContextIdsNamesMap_ ;
		
		// sets up the logger for the component form which this is called - registers the app, log level.
		int init ( ) ; 
		
		// log message to capture
		int write ( eRemoteCDLogContexts contextId, LogLevelType level, const char *textFormat, ...) ;

};

// extern declaration of log instance which all modules in CCOI can use
extern RemoteCDLog cdLog ;

#endif
