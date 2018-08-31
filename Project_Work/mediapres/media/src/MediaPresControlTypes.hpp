/******************************************************************************
* Project         Media Pres Control
* (c) copyright   2017
* Company         Harman International
*                 All rights reserved
******************************************************************************/
/*
* @file            MediaPresControlTypes.hpp
* @ingroup         media-pres control
* @author          Nilesh Khairnar
* @brief           Contains required stuff (Events & Commands) for MPres plugin
*/

#ifndef _MEDIA_PRES_CONTROL_TYPES_H
#define _MEDIA_PRES_CONTROL_TYPES_H

#pragma once

#include <functional>

typedef enum
{
	REQ_RVCDISPLAY = 1,
	REQ_APA,
	REQ_PDC,
	REQ_DEVICEERROR,
	REQ_MASTERRESET,
	REQ_MASTERRESET_STATUS,
	REQ_DRIVERRESTRICTION_VIDEO = 26,
	REQ_DISPLAYLANGUAGE,
    REQ_CLEAR_QUEUE_CD
}tCommandName;

typedef enum
{
	eTOOMANYHUBS = 1,
   eTOOMANYHUBS_RESOLVED,
	eUNSUPPORTEDHUB,
	eUNSUPPORTEDDEVICE,
    eUNSUPPORTEDFILESYSTEM,
    eDEVICENORESPONSE
}tdeviceError;

typedef std::function<void(const bool&)> eventRVCDisplayStatus;
typedef std::function<void(const bool&)> eventAPAStatus;
typedef std::function<void(const bool&)> eventPDCStatus;
typedef std::function<void(const tdeviceError&)> eventDeviceError;
typedef std::function<void(const bool&)> eventMasterReset;
typedef std::function<void(const bool&)> eventMasterResetStatus;
typedef std::function<void(const bool&)> eventDriverRestrictionsVideo;
typedef std::function<void(const std::string&)> eventDisplayLanguageStatus;
typedef std::function<void(const bool&)> eventrequestQueueCD;


#endif /* _MEDIA_PRES_CONTROL_TYPES_h */
