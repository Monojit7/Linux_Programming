/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceManager.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements some helper methods for browse functionality. Basically does the mapping between data from coc media to media pres..
*/

#ifndef _DEVICEMANAGER_HELPER_H_
#define _DEVICEMANAGER_HELPER_H_

#include <core.h>
#include <unordered_map>

using namespace v1::com::harman::mmpres; 

class DeviceManagerHelper
{
public : 
	static const char* IAP1_PROTOCOL;
	static const char* IAP2_PROTOCOL;
	static const char* AVRCP_PROFILE;
	static const char* A2DP_PROFILE;
	static std::string getIAPVersion(std::string protocol);
private : 
};

#endif