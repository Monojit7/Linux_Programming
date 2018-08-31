/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceIpod.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from an IAP1 device.
*/


#ifndef _DEVICE_IPOD_H_
#define _DEVICE_IPOD_H_

#include <device/DeviceBase.h>

class DeviceIpod : public DeviceBase
{
public:
	enum protocol
	{
		IAP1,
		IAP2,
		UNKNOWN
	};

   struct iTunesStationInfo
   {
      bool promoteStatus;
      bool demoteStatus;
      bool addToWishListStatus;
      iTunesStationInfo()
      {
         promoteStatus = false;
         demoteStatus = false;
         addToWishListStatus = false;
      }
   };

	DeviceIpod(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);


protected:
	virtual uint32_t getDeviceCaps();
	virtual void getDeviceFirmWareInfo();
	virtual void getDeviceFirmWareInfoCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tFirmwareInfo&, const ::v1::com::harman::media::BaseType::tBaseError&);
   virtual void sendIAP2KeyEvent(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType, bool pressed);
   virtual void sendIAP2KeyEventCallback(const CommonAPI::CallStatus& status);
   //iTunes related functions
	virtual void promote(uint64_t fileid);
   virtual void demote(uint64_t fileid);
   virtual void addToWishList(uint64_t fileid);
	virtual void promoteModeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
   virtual void demoteCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
   virtual void addToWishListCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
   bool getPromoteStatus();
   void setPromoteStatus(bool value);
   bool getDemoteStatus();
   void setDemoteStatus(bool value);
   bool getAddToWishListStatus();
   void setAddToWishListStatus(bool value);

private:
   DeviceIpod::protocol mProtocolVersion;
   static std::unordered_map<std::string, DeviceIpod::protocol> mProtocolMap;
   std::unordered_map<std::string, DeviceIpod::iTunesStationInfo> miTunesStationInfoMap;
   const void updatePromoteStatus();
   const void updateDemoteStatus();
   const void updateAddToWishListStatus();
   ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType getIAP2ButtonType(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType);
};

#endif

