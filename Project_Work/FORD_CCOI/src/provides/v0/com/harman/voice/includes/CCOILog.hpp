#ifndef _CCOILOG_H
#define _CCOILOG_H

#include <dlt/dlt.h>
#include <iostream>
#include <string>
#include <cstdarg>	// for variable list arguments functions
#include <map>

#define LOG_MSG_MAXSIZE			( 1024 )   // 1kb
#define LOG_APP_COMPONENT_ID		( "CCOI" ) //Id & name of the app used to register in the init of the logger - dlt.
#define LOG_APP_COMPONENT_NAME		( "FORDCustomerOptInComponent" )

// namespace declaratives
using namespace std ;

// This class integrates dlt logger utility for more functionality check the headers of dlt in ..toolchain/usr/include/dlt
namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

// populated & used only by dlt lib to get the context of the log message we ask it to write..
typedef struct t_DltData {

	DltContext 	context ;
        DltContextData  contextdata ;
} t_DltData;

class CCOILog {

	public:
        // constructor
        CCOILog () ;

		enum eCCOILogContexts {
			eLogCCOIButton,
			eLogCCOIException,
			eLogCCOIManager,
			eLogCCOIMain,
			eLogCCOIMegaTP,
			eLogCCOIMockTcu,
			eLogCCOIMsgUtil,
            eLogCCOIPresCtrlStubImpl,
			eLogCCOIPersist,
			eLogCCOIProxyMgr,
			eLogSha256,
			eLogCCOIUserPromptScreenData,
			eLogzlibWrapper,
			eLogCCOIMaxLogContext
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
		t_DltData 	dltData_ [eLogCCOIMaxLogContext] ;

		// map to hold the contextId & contextName which would be used by the logger - dlt, while logging the message buffer we send to it.
		map < string, string >   logContextIdsNamesMap_ ;
		
		// sets up the logger for the component form which this is called - registers the app, log level.
		int init ( ) ; 
		
		// log message to capture
		int write ( eCCOILogContexts contextId, LogLevelType level, const char *textFormat, ...) ;

};

// extern declaration of log instance which all modules in CCOI can use
extern CCOILog CcoiLog ;

			} // namepsace voice
		} // namepsace harman
	} // namepsace com
} // namepsace v0

#endif //_CCOILOG_H
