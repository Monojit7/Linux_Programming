#ifndef TUNER_PRESETMGR_HPP_
#define TUNER_PRESETMGR_HPP_

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
#include "svcIpcEventHandling/CSvcImplBase.hpp"

class CTunerPresetMgr
{
public:   
	CTunerPresetMgr();
    virtual ~CTunerPresetMgr();


	virtual void storePresets(CSvcIpcRequest* reqData) = 0;
	virtual void selectPresets(CSvcIpcRequest* reqData) = 0;
	virtual void getPresets(CSvcIpcRequest* reqData) = 0;
	virtual void nextPreset(CSvcIpcRequest* reqData) = 0;
	virtual void previousPreset(CSvcIpcRequest* reqData) = 0;

};

#endif
