#ifndef PANDORA_DATATYPES_HPP_
#define PANDORA_DATATYPES_HPP_

/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2010
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CTunerDataTypes.hpp
* @ingroup Tunerservice
* @author Sachin Athanikar
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/

#include "iostream.h"
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <pthread.h>
#include <errno.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "svcIpcEventHandling/CSvcIpcEventTypes.hpp"
#include <list>

#define NO_TRACE_SUPPORT

//#ifdef NO_TRACE_SUPPORT

//#endif

typedef int          INT32;
typedef unsigned int UINT32;
typedef char         CHAR;
typedef char*        STRING;
typedef bool         BOOL;
#define TOKEN        1


typedef enum 
{
   Pandora_Start_Session = 0,
   Pandora_Terminate_Session,
   Pandora_TrackInfo_Extended,
   Pandora_Set_ElapsedPolling,
   Pandora_Get_Status,
   Pandora_Select_Station,
   Pandora_Play_Track,
   Pandora_Pause_Track,
   Pandora_Skip_Track,
   Pandora_TrackRate_Positive,
   Pandora_TrackRate_Negative,
   Pandora_Get_StationList,
   Pandora_AlBum_Art,
   Pandora_BookMark_Current_Track,
   Pandora_BookMark_Current_Artist,
   Pandora_Get_Station_Count,
   Pandora_Get_Station_Tokens,
   Pandora_Get_Station_Info,
   Pandora_Set_Audio,
   Pandora_Remove_Audio,
   Pandora_Req_Invalid

}pandoraReqTypes;

typedef enum
{
   Pandora_Response_Success = 0,
   Pandora_Response_Error,
   Pandora_Response_Invalid_Request,
   Pandora_Response_Invalid_Param,
   Pandora_Response_Invalid
}PandoraErrorTypes;


typedef enum
{
	Pandora_Ack_Received,
	Pandora_New_Message_Received,
	Pandora_Timeout,
	Pandora_Set_Audio_Graph,
	Pandora_Remove_Audio_Graph,
	Pandora_Start_Timer,
	Pandora_Stop_Timer,
	Pandora_Device_Check_Timer,
	Pandora_Audio_Graph_Timer,
	Pandora_None
}PandoraThreadCmd;





#endif
