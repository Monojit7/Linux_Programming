/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceBluetooth.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from a bluetooth device.
*/

#ifndef _DEVICE_BLUETOOTH_H_
#define _DEVICE_BLUETOOTH_H_

#include <device/DeviceBase.h>

using namespace v4::com::harman::btpres;

class MediaDetectionEngine;
class DeviceBluetooth : public DeviceBase
{
public:
	DeviceBluetooth(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);
	void SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
    void SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	void setDisconnectReason(BTPresTypes::eServiceConnectDisconnectReason reason);
	//uint16_t getDisconnectReason();

protected:
	virtual uint32_t getDeviceCaps();
	virtual RequestBase* SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual void getDeviceHardWareInfoCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tHardwareInfo&, const ::v1::com::harman::media::BaseType::tBaseError&);
	virtual void getDeviceHardWareInfo();
	virtual void getRemotePlayerCapabilityCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability&, const ::v1::com::harman::media::BaseType::tBaseError&);
	virtual void getRemotePlayerCapability();
   virtual const void onRemotePlayerChanged();
   virtual void gotoTrack(const uint64_t trackPosition);

   void onCurrentTrackChanged(const uint64_t&);
   void onTotalTracksChanged(const uint64_t&);
   void onPlayPositionChanged(const uint64_t& pos);
   void onPlayDurationChanged(const uint64_t&);

	void btConnManagerProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
    void getServiceConnectionStatusCallback(const std::string&, const std::string&, const ::v4::com::harman::btpres::BTPresTypes::eSupportedProfile&, const ::v4::com::harman::btpres::BTPresTypes::eConnectionStatus&, const ::v4::com::harman::btpres::BTPresTypes::eServiceConnectDisconnectReason&);

private:
    CommonAPIClientAbstraction<v2::com::harman::btpres::ConnectionManagerProxy> mBTConnManager;
   void init();
   void subscribeToCurrentTrackUpdate(bool subscribe);
   void subscribeToTotalTrackUpdate(bool subscribe);
   void subscribeToPlayPositionUpdate(bool subscribe);
   void subscribeToPlayDurationUpdate(bool subscribe);
};

#endif
