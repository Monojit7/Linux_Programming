#ifndef TUNER_DATATYPES_HPP_
#define TUNER_DATATYPES_HPP_

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
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "DMMTunerStation.h"
#include "DMMAmFmTunerControl.h"
#include "DMMTunerAudio.h"
#include "iostream.h"

#define NO_TRACE_SUPPORT

//#ifdef NO_TRACE_SUPPORT

//#endif

typedef int          INT32;
typedef unsigned int UINT32;
typedef char         CHAR;
typedef char*        STRING;
typedef bool         BOOL;
#define TOKEN        1
#define PRESET_MAX   18
#define PRESET_MAX_WB 6
#define TUNER_AM_BASE_FREQ_EU  531     // 531 KHz
#define TUNER_AM_END_FREQ_EU   1602    // 1602 KHz
#define TUNER_FM_BASE_FREQ_EU  87500   // 87.5 MHz
#define TUNER_FM_END_FREQ_EU   108000  // 108 MHz
#define TUNER_AM_BASE_FREQ_US  530     // 530 KHz
#define TUNER_AM_END_FREQ_US   1710    // 1710 KHz
#define TUNER_WB_BASE_FREQ_US  162400  // 142400 KHZ
#define TUNER_WB_END_FREQ_US   162550  // 162550 KHZ
#define TUNER_FM_BASE_FREQ_US  87700   // 87.7 MHz
#define TUNER_FM_END_FREQ_US   107900  // 107.9 MHz
#define TUNER_AM_BASE_FREQ_AUS  531     // 531 KHz
#define TUNER_AM_END_FREQ_AUS   1629    // 1629 KHz
#define TUNER_FM_BASE_FREQ_AUS  87500   // 87.5 MHz
#define TUNER_FM_END_FREQ_AUS   108000  // 108 MHz
#define TUNER_AM_BASE_FREQ_JPN  522     // 522 KHz
#define TUNER_AM_END_FREQ_JPN   1611    // 1611 KHz
#define TUNER_FM_BASE_FREQ_JPN  76000   // 76 MHz
#define TUNER_FM_END_FREQ_JPN   90000   // 90 MHz
#define TUNER_AM_BASE_FREQ_ASIA  531    // 531 KHz
#define TUNER_AM_END_FREQ_ASIA   1629   // 1629 KHz
#define TUNER_FM_BASE_FREQ_ASIA  87500  // 87.5 MHz
#define TUNER_FM_END_FREQ_ASIA   108000 // 108 MHz
#define TUNER_AM_BASE_FREQ_ROW  522     // 522 KHz
#define TUNER_AM_END_FREQ_ROW   1611    // 1611 KHz
#define TUNER_FM_BASE_FREQ_ROW  87500   // 87.5 MHz
#define TUNER_FM_END_FREQ_ROW   107900  // 107.9 MHz
#define MAX_TUNER_STATIONS_LIST 48
#define MAX_AUTO_STORE_AM_STATION_LIST 50
#define MAX_AUTO_STORE_AM_PRESET_LIST 6
#define MAX_AUTO_STORE_WB_STATION_LIST 6

typedef enum 
{
   Tuner_Start_Seek = 0,
   Tuner_Req_AutoSeekUp,
   Tuner_Req_AutoSeekDown,
   Tuner_Req_ManSeekUp,
   Tuner_Req_ManSeekDown,
   Tuner_Req_AbortSeek,
   Tuner_Req_Set_Frequency,
   Tuner_Req_Get_Frequency,
   Tuner_Req_Set_AFSwitch,
   Tuner_Req_Set_Waveband,
   Tuner_Req_Store_Preset,
   Tuner_Req_Auto_Store_Preset,
   Tuner_Req_Select_Preset,
   Tuner_Req_Get_Preset,
   Tuner_Req_Get_Properties,
   Tuner_Req_Set_TAMode,
   Tuner_Req_Cancel_TA,
   Tuner_Req_Scan,
   Tuner_Req_AbortScan,
   Tuner_Req_Get_SignalQuality,
   Tuner_Req_Set_HDMode,
   Tuner_Req_Set_BallgameMode,
   Tuner_Req_Set_RegMode,
   Tuner_Req_Set_StationSortingFlag,
   Tuner_Req_Get_StationList,
   Tuner_Req_Get_FieldStrength,
   Tuner_Req_Get_SoftMute_Switch,
   Tuner_Req_Get_High_Cut_Switch,
   Tuner_Req_Get_Stereo_High_Blend_Switch,
   Tuner_Req_Get_Stereo_Blend_Switch,
   Tuner_Req_Get_Quality_Threshold_Entry,
   Tuner_Req_Get_Quality_Threshold_Exit,
   Tuner_Req_Get_HD_Audio_Mode,
   Tuner_Req_Get_HD_FM_BlendTime_AD,
   Tuner_Req_Get_HD_FM_BlendTime_DA,
   Tuner_Req_Get_HD_AM_BlendTime_AD,
   Tuner_Req_Get_HD_AM_BlendTime_DA,
   Tuner_Req_Get_HD_BER_Mode,
   Tuner_Req_Get_HD_Audio_Splitting,
   Tuner_Req_Get_HD_Blending_Mode,
   Tuner_Req_Set_SoftmuteSwitch,
   Tuner_Req_Set_High_Cut_Switch,
   Tuner_Req_Set_Stereo_High_Blend_Switch,
   Tuner_Req_Set_Stereo_Blend_Switch,
   Tuner_Req_Set_Quality_Threshold_Entry,
   Tuner_Req_Set_Quality_Threshold_Exit,
   Tuner_Req_Set_HD_BER_Mode,
   Tuner_Req_Set_HD_Audio_Splitting,
   Tuner_Req_Skip_Preset_Up,
   Tuner_Req_Skip_Preset_Down,
   Tuner_Req_Get_D3_Signal,
   Tuner_Req_Get_Info_Current_Station,
   Tuner_Req_Clear_Presets,
   Tuner_Req_Invalid
}tunerReqTypes;

typedef enum
{
   Tuner_Response_Success = 0,
   Tuner_Response_Error,
   Tuner_Response_Invalid_Request,
   Tuner_Response_Invalid_Param,
   Tuner_Response_Invalid
}tunerErrorTypes;

typedef enum
{
   Tuner_Band_FM = 0,
   Tuner_Band_AM,
   Tuner_Band_WB,
   Tuner_Band_Invalid
}tunerBandTypes;

typedef enum
{
   Tuner_Persist_PresetsFM = 0,
   Tuner_Persist_PresetsAM,
   Tuner_Persist_PresetsWB,
   Tuner_Persist_LastTunedFreqFM,
   Tuner_Persist_LastTunedFreqAM,
   Tuner_Persist_LastTunedFreqWB,
   Tuner_Persist_LastHDService,
   Tuner_Persist_TA_Mode,
   Tuner_Persist_Invalid
}tunerPersistencyTypes;

typedef enum
{
   Tuner_Select_Direct_Tune = 0,
   Tuner_Select_Station_List_Tune,
   Tuner_Select_Preset_Tune,
}tunerSelectionTypes;

typedef enum
{
	   eNOT_STARTED,
	   ePERSISTENCY_COMPLETION,
	   eSTATION_INTERFACE_CONNECTED
}eInitProgressStatus;

struct proxy
{
   UINT32 token;
   UINT32 frequency;
   UINT32 spsNo;
   UINT32 piCode;
   UINT32 reqType;
   UINT32 selectionType;
   unsigned char antennaNo;
   INT32 value;
   DMMTunerStation_SeekMode mode;
   BOOL switchFlag;
   DMMAmFmTunerControl_HdMode hdMode;
   DMMAmFmTunerControl_HdBallGameMode bgMode;
   DMMTunerAudio_SourceType srcType;
   bool taStatus;
   bool snkSpeakerStatus;
   char lcf;
};

struct stationInformation
{
	char name[20];
	UINT32 freq;
	UINT32 picode;
	unsigned char pty;


};

struct currentStationInformation
{
  std::string psName;
  std::string radioText;
  std::string hdShortName;
  std::string hdSongName;
  std::string hdArtistName;
};

#endif
