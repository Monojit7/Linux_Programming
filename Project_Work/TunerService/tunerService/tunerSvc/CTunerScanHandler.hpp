#ifndef TUNER_SCANHANDLER_HPP_
#define TUNER_SCANHANDLER_HPP_

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
* @file CTunerScanHandler.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerCommand.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CDMMTunerStationProxy.hpp"
//#include <CTimer.h>


class CTunerScanHandler : public CTunerCommand
{
public:
	unsigned int currentFrequency;
    unsigned int tuningFrequency;
    static CTunerScanHandler* mScanHandler;
    static bool scanStatus;
    static bool seekStatus;
    static bool timerLapse;

public:   
	CTunerScanHandler();
   ~CTunerScanHandler();

public:

    static CTunerScanHandler* getScanHandler();

	// Virtual override from ITimerHandler
	//void handleTimerEvent (CTimer* pTimer) ;
	void startTimer(void);
	void setCurrentFreq(unsigned int freq);
	unsigned int getCurrentFreq(void);

	void setTuningFreq(unsigned int freq);
	unsigned int getTuningFreq(void);

	bool getScanStatus(void);
	//void setSeekStatus(bool status);
	//bool getSeekStatus(void);

   	void execute(CSvcIpcRequest* reqData);
    void abortScan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
	void scan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);

private:
	//CTimer	mTimerInst;

};


#endif
