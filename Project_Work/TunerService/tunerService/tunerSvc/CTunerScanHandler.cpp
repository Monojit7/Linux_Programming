/*----------------------------------------------------------------------------------------------------------------------
 *
 * Project Harman Mid Systems - Tuner Service
 * (c) copyright 2013
 * Company Harman Internation
 * All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 *
 *-----------------------------------------------------------------------------------------------------------------------
 *
 * @file CTunerScanHandler.cpp
 * @ingroup TunerService
 * @author
 * Public API for the Tuner core service library.
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 ----------------------------------------------------------------------------------------------------------------------*/

#include "CTunerScanHandler.hpp"
#include "CTunerRespHandler.hpp"
#include "jsoncpp/json.h"
#include "CTunerDataTypes.hpp"
#include "DMMTunerTypes.h"
#include <iostream>
#include "stdio.h"

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
 ----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/

/*......................................................................................................................
 * @brief CTunerScanHandler()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
//pthread_t        gProxy;
//static void* runProxy(void *arg);

bool CTunerScanHandler::scanStatus = false;
bool CTunerScanHandler::timerLapse = true;
bool CTunerScanHandler::seekStatus = false;

CTunerScanHandler* CTunerScanHandler::mScanHandler = NULL;

CTunerScanHandler::CTunerScanHandler()
{
	currentFrequency = 87500;
	tuningFrequency = 0;
}

/*......................................................................................................................
 * @brief CTunerScanHandler()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerScanHandler::~CTunerScanHandler()
{
	;
}

CTunerScanHandler* CTunerScanHandler::getScanHandler()
{
	return mScanHandler;
}
void CTunerScanHandler::setCurrentFreq(unsigned int freq)
{
	if(false == scanStatus)
	{
		currentFrequency = freq;
	}
	else
	{
		;
	}
}
unsigned int CTunerScanHandler::getCurrentFreq(void)
{
	return currentFrequency;
}
void CTunerScanHandler::setTuningFreq(unsigned int freq)
{
	tuningFrequency = freq;
}
unsigned int CTunerScanHandler::getTuningFreq(void)
{
	return tuningFrequency;
}
bool CTunerScanHandler::getScanStatus(void)
{
	return scanStatus;
}
/*void CTunerScanHandler::setSeekStatus(bool status)
{
	seekStatus = status;
}*/
/*
void CTunerScanHandler::handleTimerEvent (CTimer* pTimer)
{
	//std::cout << "[CTunerScanHandler]: Timer lapsed !!\n" << std::endl;
	proxy tunerData;
	//tunerProxyData.token = (unsigned int)token;
	tunerData.token = TOKEN;
	tunerData.mode = DMMTunerStation_SEEK_AUTO_UP;
	std::cout<<"Current frequency is ------> "<<CTunerScanHandler::currentFrequency<<std::endl;
	if ((CTunerScanHandler::currentFrequency) == (CTunerScanHandler::tuningFrequency))
		{
			std::cout << "[CTunerScanHandler]: Frequency matched !!\n" << std::endl;
			CTunerScanHandler::scanStatus = false;
			CTunerScanHandler::tuningFrequency = 0;
			//break;
		}
		else
		{
			//Call normal DSI seek function
			(CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
			std::cout << "seekUp: Seek called for scan ...\n" << std::endl;
		}

}*/
void CTunerScanHandler::startTimer(void)
{
	//Time interval for which station should be tuned to
	//mTimerInst.startTimer(5000,false);
}
void CTunerScanHandler::execute(CSvcIpcRequest* reqData) {
	//std::cout << "[CTunerScanHandler]: In execute !!! /n"<< std::endl;
	SVCIPC_tConstStr method = reqData->getMethod();
	SVCIPC_tUserToken token = reqData->getUserToken();

	//std::cout << "[CTunerScanHandler]:execute and method is: " << method << std::endl;

	if (strcmp(method, "reqStartScan") == 0)
	{
		scan(token, reqData);
	} else if (strcmp(method, "reqStopScan") == 0) {
		abortScan(token, reqData);
	}else
	{
		std::cout << "[CTunerScanHandler]: Request Error"<<std::endl;
		CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Request);
	}
}

/*......................................................................................................................
 * @brief scanUp()- Function to initiate the scan functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerScanHandler::scan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData) {
	//std::cout << "[CTunerScanHandler]:scan" << std::endl;
	CTunerScanHandler::scanStatus = true;
	//Copy data to be send into a structure
	proxy tunerData;
	//tunerData.token = (unsigned int)token;
	tunerData.token = TOKEN;
	tunerData.mode = DMMTunerStation_SEEK_SCAN_UP;
	tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;

	CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
	//Call DSI Interface

	//printf("******CTunerScanHandler::In Scan while*****\n");

		//Call normal DSI seek function
		(CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
		 //std::cout << "seekUp: postInternalEvent Called ...\n" << std::endl;


}
/*......................................................................................................................
 * @brief abortScan()- Function to abort scan functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerScanHandler::abortScan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData) {

	//std::cout << "[CTunerScanHandler]:abortScan" << std::endl;
	//Copy data to be send into a structure
	proxy tunerData;
	//tunerData.token = (unsigned int)token;
	tunerData.token = TOKEN;
	tunerData.mode = DMMTunerStation_SEEK_OFF;
	//Call DSI Interface
	//DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_OFF;

	(CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

	CTunerScanHandler::scanStatus = false;
	//Send response to HMI
	CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}

//----------------------------------------------------- END OF FILE ---------------------------------------------------
