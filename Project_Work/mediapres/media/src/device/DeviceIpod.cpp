#include <device/DeviceIpod.h>

std::unordered_map<std::string, DeviceIpod::protocol> DeviceIpod::mProtocolMap = {
   { "IAP1",DeviceIpod::protocol::IAP1 }
   ,{ "IAP2",DeviceIpod::protocol::IAP2 }
   ,{ "unknown",DeviceIpod::protocol::UNKNOWN }
};

DeviceIpod::DeviceIpod(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
   :DeviceBase(mediumInfo)
   , mProtocolVersion(DeviceIpod::protocol::UNKNOWN)
{
   miTunesStationInfoMap.clear();
}

uint32_t DeviceIpod::getDeviceCaps()
{
   return (MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}

void DeviceIpod::getDeviceFirmWareInfo()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getDeviceFirmWareInfo");
   mDetect->getDeviceFirmwareInfo(mDetect.mClientID, mMediumInfo.getMMediumId(), std::bind(&DeviceIpod::getDeviceFirmWareInfoCallback, this, _1, _2, _3));
}

void DeviceIpod::getDeviceFirmWareInfoCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tFirmwareInfo& firmWareInfo, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getDeviceFirmWareInfoCallback-status:%d, baseError:%d", (int32_t)status, (int32_t)(baseError));

   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getDeviceFirmWareInfoCallback-firmware:%s, protocol:%s,modelNumber:%s", firmWareInfo.getFirmware().c_str(), firmWareInfo.getProtocol().c_str(), firmWareInfo.getModelNumber().c_str());
      mFirmWareInfo = firmWareInfo;
      std::string version = DeviceManagerHelper::getIAPVersion(firmWareInfo.getProtocol());
      std::unordered_map<std::string, DeviceIpod::protocol>::const_iterator ctr = mProtocolMap.find(version);
      if (ctr != mProtocolMap.end())
      {
         mProtocolVersion = (ctr->second);
      }
      mAdditionalDeviceInfo.setFirmware(firmWareInfo.getFirmware());
      mAdditionalDeviceInfo.setProtocol(firmWareInfo.getProtocol());
      mAdditionalDeviceInfo.setModelNumber(firmWareInfo.getModelNumber());

      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getDeviceFirmWareInfoCallback-protocol version:%d", (int32_t)mProtocolVersion);
   }
}

void DeviceIpod::promote(uint64_t fileid)
{
   std::string nowPlayingTrack = "";

   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::promote - fileid=%d", fileid);
   mPlayEngine->promote(mPlayEngine.mClientID, nowPlayingTrack, std::bind(&DeviceIpod::promoteModeCallback, this, _1, _2));
}


void DeviceIpod::promoteModeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceIpod::promoteModeCallback :CallStatus : %d PlayerError %d", (int)status, (int)error);
   if (error == PlayerTypes::PlayerError::NO_ERROR)
   {
      updatePromoteStatus();
   }
}

const void DeviceIpod::updatePromoteStatus()
{
   const bool bPromoteStatus = getPromoteStatus();
   setPromoteStatus(!bPromoteStatus);
}
void DeviceIpod::demote(uint64_t fileid)
{
   std::string nowPlayingTrack = "";
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::demote - fileid=%d", fileid);
   mPlayEngine->demote(mPlayEngine.mClientID, nowPlayingTrack, std::bind(&DeviceIpod::demoteCallback, this, _1, _2));
}

void DeviceIpod::demoteCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceIpod::demoteCallback :CallStatus : %d PlayerError %d", (int)status, (int)error);
   if (error == PlayerTypes::PlayerError::NO_ERROR)
   {
      updateDemoteStatus();
   }
}

const void DeviceIpod::updateDemoteStatus()
{
   const bool bDemoteStatus = getDemoteStatus();
   setDemoteStatus(!bDemoteStatus);
}

void DeviceIpod::addToWishList(uint64_t fileid)
{
   std::string nowPlayingTrack = "";

   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::addToWishList - fileid=%d", fileid);
   mPlayEngine->addToWishList(mPlayEngine.mClientID, nowPlayingTrack, std::bind(&DeviceIpod::addToWishListCallback, this, _1, _2));
}

void DeviceIpod::addToWishListCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceIpod::addToWishListCallback :CallStatus : %d PlayerError %d", (int)status, (int)error);
   if (error == PlayerTypes::PlayerError::NO_ERROR)
   {
      updateAddToWishListStatus();
   }
}

const void DeviceIpod::updateAddToWishListStatus()
{
   const bool bAddToWishListStatus = getAddToWishListStatus();
   setAddToWishListStatus(!bAddToWishListStatus);
}


bool DeviceIpod::getPromoteStatus()
{
   bool lpromoteStatus = false;
   std::string lNowPlayingType = "";
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         lpromoteStatus = it->second.promoteStatus;
      }
      else
      {
         // Do Nothing
      }
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getPromoteStatus - %d", (int)lpromoteStatus);
   return lpromoteStatus;
}

void DeviceIpod::setPromoteStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::setPromoteStatus - %d", (int)_value);
   std::string lNowPlayingType = "";
   bool lDemoteStatus = false;
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::iTunesPlayback");
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         it->second.promoteStatus = _value;
         //Both Promote and Demote status cant be true at the same time
         if (it->second.promoteStatus == true)
         {
            it->second.demoteStatus = false;
         }
         lDemoteStatus = it->second.demoteStatus;
      }
      else
      {
         liTunesStationInfo.promoteStatus = _value;
         miTunesStationInfoMap.insert(std::pair<std::string, DeviceIpod::iTunesStationInfo>(lStationName, liTunesStationInfo));
      }
      std::function<void(const bool&)> promoteCallback = mPlayEngine->getPromoteStatusStubCallBack();
      if (promoteCallback != nullptr)
      {
         promoteCallback(_value);
      }
      std::function<void(const bool&)> demoteCallback = mPlayEngine->getDemoteStatusStubCallBack();
      if (demoteCallback != nullptr)
      {
         demoteCallback(lDemoteStatus);
      }
   }
}

bool DeviceIpod::getDemoteStatus()
{
   bool lDemoteStatus = false;
   std::string lNowPlayingType = "";
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         lDemoteStatus = it->second.demoteStatus;
      }
      else
      {
         // Do Nothing
      }
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getdemoteStatus - %d", (int)lDemoteStatus);
   return lDemoteStatus;
}

void DeviceIpod::setDemoteStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::setDemoteStatus - %d", (int)_value);
   std::string lNowPlayingType = "";
   bool lPromoteStatus = false;
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         it->second.demoteStatus = _value;
         //Both Promote and Demote status cant be true at the same time
         if (it->second.demoteStatus == true)
         {
            it->second.promoteStatus = false;
         }
         lPromoteStatus = it->second.promoteStatus;
      }
      else
      {
         liTunesStationInfo.demoteStatus = _value;
         miTunesStationInfoMap.insert(std::pair<std::string, DeviceIpod::iTunesStationInfo>(lStationName, liTunesStationInfo));
      }
      std::function<void(const bool&)> demoteCallback = mPlayEngine->getDemoteStatusStubCallBack();
      if (demoteCallback != nullptr)
      {
         demoteCallback(_value);
      }
      std::function<void(const bool&)> promoteCallback = mPlayEngine->getPromoteStatusStubCallBack();
      if (promoteCallback != nullptr)
      {
         promoteCallback(lPromoteStatus);
      }
   }
}

bool DeviceIpod::getAddToWishListStatus()
{
   bool laddToWishListStatus = false;
   std::string lNowPlayingType = "";
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         laddToWishListStatus = it->second.addToWishListStatus;
      }
      else
      {
         // Do Nothing
      }
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::getaddToWishListStatus - %d", (int)laddToWishListStatus);
   return laddToWishListStatus;
}

void DeviceIpod::setAddToWishListStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::setAddToWishListStatus - %d", (int)_value);
   std::string lNowPlayingType = "";
   getNowPlayingType(lNowPlayingType);
   if (lNowPlayingType.compare("iTunesRadio") == 0)
   {
      std::string lStationName = getNowPlayingInfo().getFilename();
      iTunesStationInfo liTunesStationInfo;
      auto it = miTunesStationInfoMap.find(lStationName);
      if (it != miTunesStationInfoMap.end())
      {
         it->second.addToWishListStatus = _value;
      }
      else
      {
         liTunesStationInfo.addToWishListStatus = _value;
         miTunesStationInfoMap.insert(std::pair<std::string, DeviceIpod::iTunesStationInfo>(lStationName, liTunesStationInfo));
      }
      std::function<void(const bool&)> addToWishListCallback = mPlayEngine->getAddToWishListStatusStubCallBack();
      if (addToWishListCallback != nullptr)
      {
         addToWishListCallback(_value);
      }
   }
}

::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType DeviceIpod::getIAP2ButtonType(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType)
{
   ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PLAY;
   switch (buttonType)
   {
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_PLAY:
      {
         //IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PLAY;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_PAUSE:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PAUSE;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_NEXTTRACK:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_NEXTTRACK;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_PREVTRACK:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PREVTRACK;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_RANDOM:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_RANDOM;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_REPEAT:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_REPEAT;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_NORMALSPEED:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_NORMALSPEED;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_INCREASESPEED:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_INCREASESPEED;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_DECREASESPEED:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_DECREASESPEED;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_PLAYPAUSE:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PLAYPAUSE;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_SIRI:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_SIRI;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_MUTE:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_MUTE;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_INCREASEVOLUME:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_INCREASEVOLUME;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_DECREASEVOLUME:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_DECREASEVOLUME;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_PROMOTE:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PROMOTE;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_DEMOTE:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_DEMOTE;
         break;
      }
      case ::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType::BUTTON_ADDTOWISHLIST:
      {
         IAP2Button = ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_ADDTOWISHLIST;
         break;
      }
      default:
      {
         break;
      }
   }
   return IAP2Button;
}

void DeviceIpod::sendIAP2KeyEvent(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType, bool pressed)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceIpod::sendIAP2KeyEvent - pressed %d buttonType %d", (int)pressed, (int)buttonType);
   ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType IAP2Button = getIAP2ButtonType(buttonType);
   mPlayEngine->sendIAP2KeyEvent(mPlayEngine.mClientID, IAP2Button, pressed, std::bind(&DeviceIpod::sendIAP2KeyEventCallback, this, _1));

   //Handling for iTunes button attributes update to HMI
   if (false == pressed) //Button has been released
   {
      switch (IAP2Button)
      {
         case ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_PROMOTE:
         {
            updatePromoteStatus();
            break;
         }
         case ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_DEMOTE:
         {
            updateDemoteStatus();
            break;
         }
         case ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType::BUTTON_ADDTOWISHLIST:
         {
            updateAddToWishListStatus();
            break;
         }
         default:
         {
            //Do Nothing
            break;
         }
      }
   }
}

void DeviceIpod::sendIAP2KeyEventCallback(const CommonAPI::CallStatus& status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceIpod::sendIAP2KeyEventCallback :CallStatus : %d ", (int)status);
}
