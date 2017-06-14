#ifndef TUNER_SEEKHANDLER_HPP_
#define TUNER_SEEKHANDLER_HPP_

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
* @file CTunerSeekHandler.hpp
* @ingroup Tunerservice
* @author 
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerCommand.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CDMMTunerStationProxy.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "DMMTunerStation.h"
#include "CTunerDataTypes.hpp"


class CTunerSeekHandler: public CTunerCommand
{
public:
   static CDMMTunerStationProxy* mProxy;
   static tunerBandTypes mCurrentTunerBand;
public:   
    CTunerSeekHandler();
    virtual ~CTunerSeekHandler();

   void execute(CSvcIpcRequest* reqData);
   void updateFrequency(INT32 region);
   void seekUp(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void seekDown(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void manualSeekUp(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void manualSeekDown(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void abortSeek(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setFrequency(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getFrequency(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setAF(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setWaveband(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setHDMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setBallgameMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setRegMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setStationSortingFlag(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getStationList(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getInfoCurrentStation(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void abortScan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void scan(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void tuneLastFMStation(void);
   void tuneLastAMStation(void);

   void stnListSortingbyFreq();
   void stnListSortingbyAlpb();
   Json::Value sendStationList();

  // static void setLastTunerAMFreq(INT32 freq);
   //static void setLastTunerFMFreq(INT32 freq);

   static bool getHDStatus(void);



   static inline void setCurrentstationQuality(INT32 quality)
    {
       mStationQuality = quality;
    }

   static inline INT32 getCurrentstationQuality()
    {
       return mStationQuality;
    }

   static inline tunerBandTypes getCurrBand()
    {
       return mCurrentTunerBand;
    }

   static inline UINT32 getFMLastTunedFreq()
    {
       return mLastTunedFMFrequency;
    }

   static inline UINT32 getAMLastTunedFreq()
   {
      return mLastTunedAMFrequency;
   }
   static inline UINT32 getWBLastTunedFreq()
   {
      return mLastTunedWBFrequency;
   }
   static inline INT32 getLastTunedHDService()
   {
      return mLastHDService;
   }
   /*UINT32 getFMLastTunedFreq()
   {
	  return mLastTunedFMFrequency;
   }

   UINT32 getAMLastTunedFreq()
   {
	 return mLastTunedAMFrequency;
   }*/
   void setFMLastTunedFreq(UINT32 freq);
   //{
	// mLastTunedFMFrequency = freq;
   //}

   void setAMLastTunedFreq(UINT32 freq);

   //{
	 //mLastTunedAMFrequency = freq;
   //}

   void setWBLastTunedFreq(UINT32 freq);
   void setLastTunedHDService(INT32 service);

   static UINT32 mFMMinFrequency;
   static UINT32 mFMMaxFrequency;
   static UINT32 mAMMinFrequency;
   static UINT32 mAMMaxFrequency;
   static UINT32 mWBMinFrequency;
   static UINT32 mWBMaxFrequency;
   static bool mStationSortingFlag;
   static int stnSize;
   static struct stationInformation stndata[48];
   static struct stationInformation stndataAutoStore[18];
   static struct currentStationInformation stnInfo;
   static UINT32 regionCode;
   static UINT32 mLastTunedAMFrequency;
   static UINT32 mLastTunedFMFrequency;
   static UINT32 mLastTunedWBFrequency;
   static UINT32 mPIcode;
   static INT32 mCurrentHDService;
   static INT32 mLastHDService;
   static UINT32 mTotalHDService;
   static bool hdStatus;
   static bool hdMode;
   static std::list<int> spsList;
private:
   //tunerReqTypes getMethod(char* method);	
   static bool scanStatus;

   static UINT32 mStationQuality;

};

#endif
