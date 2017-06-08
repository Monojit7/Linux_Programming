#include "CCOILog.hpp"

namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {


// initialize all the contexts (class / modules) for which the logs needs to be captured, future class/ modules shall be added here.
// add the entries in ascending order of the key and match it with the enum eCCOILogContexts (in the header file) for the log contextId

CCOILog::CCOILog ()
{
    logContextIdsNamesMap_ =
    {
        { "BTON", 	"CCOIButton" 		         },	
        { "EXCP", 	"CCOIException" 	         },
        { "IMGR", 	"CCOIManager"                },	
        { "MAIN", 	"main"       		         },	
        { "MGTP", 	"CCOIMegaTP"                 },	
        { "MOCK", 	"CCOIMockTcu"                },	
        { "MSGU", 	"CCOIMessagesUtil"           },	
        { "PCST", 	"CCOIPresCtrlStubImpl"       },	
        { "PERS", 	"CCOIPersist"		         },	
        { "PRXY", 	"CCOIProxyManager"		     },	
        { "S256",	"Sha256" 		             },	
        { "UPRM", 	"CCOIUserPromptScreenData"   },
        { "ZLIB",	"zlibWrapper" 		         }	
    } ;
}

// registers the app, log level.
int CCOILog::init ( )
{
	// register our application/ component with the dlt daemon.
	dlt_register_app ( LOG_APP_COMPONENT_ID, LOG_APP_COMPONENT_NAME ) ;
    	
	int dltIndex = 0 ;	
	for ( auto const &appContext : logContextIdsNamesMap_ )
    {
		dlt_register_context ( &dltData_ [ dltIndex++ ].context, appContext.first.c_str(), appContext.second.c_str() ) ;
    }
	
    //set logger in verbose mode
    dlt_verbose_mode();
	
	return 0 ; // i.e., DLT_RETURN_OK.
}

int CCOILog::write ( eCCOILogContexts contextId, LogLevelType logLevel, const char *textFormat, ...)
{
	char buffer [LOG_MSG_MAXSIZE] ;
	
	// push variable arguments into buffer which is written into log.
	va_list vargs ;
	va_start ( vargs, textFormat ) ;
	vsnprintf (buffer, LOG_MSG_MAXSIZE, textFormat, vargs) ;
	va_end ( vargs ) ;

	if ( dlt_user_log_write_start ( &dltData_ [ contextId ].context, &dltData_ [ contextId ].contextdata, static_cast < DltLogLevelType > (logLevel) ) )
    {
        dlt_user_log_write_string(&dltData_ [ contextId ].contextdata, buffer ) ;
        dlt_user_log_write_finish(&dltData_ [ contextId ].contextdata ) ;
    }

	return 0 ; // i.e., DLT_RETURN_OK.
}

			} //namespace voice
		} //namespace harman
	} //namespace com
} //namespace v0
