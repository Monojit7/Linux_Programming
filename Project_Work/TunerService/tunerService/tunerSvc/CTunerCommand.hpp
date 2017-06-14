#ifndef TUNER_COMMAND_HPP_
#define TUNER_COMMAND_HPP_

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
* @file CTunerCommand.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
//#include "CTunerStationProxy.hpp"
#include "CTunerReqQueue.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"

class CTunerCommand
{
public:   
	CTunerCommand();
    virtual ~CTunerCommand();


	bool Initialize(void);
	virtual void execute(CSvcIpcRequest* reqData) = 0;
	//virtual void seekUp(CTunerStationProxy* stationproxy) = 0;
	//virtual void seekDown(CTunerStationProxy* stationproxy) = 0;
	//virtual void manualSeekUp(CTunerStationProxy* stationproxy,int freq) = 0;
	//virtual void manualSeekDown(CTunerStationProxy* stationproxy,int freq) = 0;
	//virtual void scan(CTunerStationProxy* stationproxy) = 0;


};

#endif
