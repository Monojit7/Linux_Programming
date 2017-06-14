#ifndef TUNER_PRESETFM_HPP_
#define TUNER_PRESETFM_HPP_

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
* @file CTunerPresetFM1.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerPresetMgr.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CTunerRespHandler.hpp"
#include "CTunerDataTypes.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include <iostream.h>

//FM Presets
class CTunerPresetFM: public CTunerPresetMgr
{
public:
   CTunerPresetFM();
   virtual ~CTunerPresetFM();

   void storePresets(CSvcIpcRequest* reqData);
   void selectPresets(CSvcIpcRequest* reqData);
   void getPresets(CSvcIpcRequest* reqData);
   void nextPreset(CSvcIpcRequest* reqData);
   void previousPreset(CSvcIpcRequest* reqData);
   Json::Value retrievePresetFM();
   static INT32 readPreset();
   static void writePreset();
   static void updatePresetNoFM(unsigned int frequency,INT32 spsNo,UINT32 piCode);
   //bool comp(const CTunerPresetFM & num1, const CTunerPresetFM & num2);

   unsigned int frequency;
   //int pi;
   std::string psName;
   int presetNo;
   //INT32 spsNo;
   INT32 hdDispStatus;
   UINT32 piCode;
   static std::list<CTunerPresetFM*> presetListFM;
   static unsigned int currentFrequency;
   static int currentPresetNo;
   static bool reqStatus;
   static int nextPresetNo;

};

//AM Presets
class CTunerPresetAM: public CTunerPresetMgr
{
public:
   CTunerPresetAM();
   virtual ~CTunerPresetAM();


   void storePresets(CSvcIpcRequest* reqData);
   void selectPresets(CSvcIpcRequest* reqData);
   void getPresets(CSvcIpcRequest* reqData);
   void nextPreset(CSvcIpcRequest* reqData);
   void previousPreset(CSvcIpcRequest* reqData);
   void AutoStorePresetListUpdateAM(struct stationInformation value, int PresetNo);
   Json::Value retrievePresetAM();
   static void updatePresetNoAM(unsigned int frequency,INT32 spsNo);

   unsigned int frequency;
   //int pi;
   std::string psName;
   int presetNo;
   INT32 spsNo;
   INT32 hdDispStatus;
   UINT32 piCode;
   UINT32 pty;
   static std::list<CTunerPresetAM> presetListAM;
   static stationInformation value;
   static unsigned int currentFrequencyAM;
   static int currentPresetNoAM;
   static bool reqStatusAM;
   static int nextPresetNoAM;
};


//WB Presets

//AM Presets
class CTunerPresetWB: public CTunerPresetMgr
{
public:
   CTunerPresetWB();
   virtual ~CTunerPresetWB();


   void storePresets(CSvcIpcRequest* reqData);
   void selectPresets(CSvcIpcRequest* reqData);
   void getPresets(CSvcIpcRequest* reqData);
   void nextPreset(CSvcIpcRequest* reqData);
   void previousPreset(CSvcIpcRequest* reqData);
   Json::Value retrievePresetWB();
   static void updatePresetNoWB(unsigned int frequency,INT32 spsNo);

   unsigned int frequency;
   //int pi;
   std::string psName;
   int presetNo;
   INT32 spsNo;
   UINT32 pty;
   INT32 hdDispStatus;
   UINT32 piCode;
   static std::list<CTunerPresetWB> presetListWB;
   static unsigned int currentFrequencyWB;
   static int currentPresetNoWB;
   static bool reqStatusWB;
   static int nextPresetNoWB;
};

#endif
