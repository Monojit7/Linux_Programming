/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V2_COM_HARMAN_VOICE_CCOI_Pres_Ctrl_DBUS_STUB_ADAPTER_HPP_
#define V2_COM_HARMAN_VOICE_CCOI_Pres_Ctrl_DBUS_STUB_ADAPTER_HPP_

#include <v2/com/harman/voice/CCOIPresCtrlStub.hpp>
#include "v2/com/harman/voice/CCOIPresCtrlDBusDeployment.hpp"        
#include <v2/com/harman/voice/CCOIPresCtrlDBusDeployment.hpp>        

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusStubAdapterHelper.hpp>
#include <CommonAPI/DBus/DBusStubAdapter.hpp>
#include <CommonAPI/DBus/DBusDeployment.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v2 {
namespace com {
namespace harman {
namespace voice {

typedef CommonAPI::DBus::DBusStubAdapterHelper<CCOIPresCtrlStub> CCOIPresCtrlDBusStubAdapterHelper;

class CCOIPresCtrlDBusStubAdapterInternal
    : public virtual CCOIPresCtrlStubAdapter,
      public CCOIPresCtrlDBusStubAdapterHelper
{
public:
    CCOIPresCtrlDBusStubAdapterInternal(
            const CommonAPI::DBus::DBusAddress &_address,
            const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
            const std::shared_ptr<CommonAPI::StubBase> &_stub);

    ~CCOIPresCtrlDBusStubAdapterInternal();

    virtual bool hasFreedesktopProperties();

    inline static const char* getInterface() {
        return CCOIPresCtrl::getInterface();
    }

    /**
     * description: Refer the description in the enumerator LocDataShareIconValues 
    		  This is for
     *   showing the location-data-icons as per policy files
     */
    void fireLoc_data_icon_valAttributeChanged(const ::v2::com::harman::voice::CCOIPresCtrl::LocDataShareIconValues& value);
    /**
     * description: HMI can query the below method to get complete set of entries that need to be
     *   
    		    populated in the settings screen
     */
    void fireResponseAttributeChanged(const ::v2::com::harman::voice::CCOIPresCtrl::entity& value);
    /**
     * description: attribute holding the info text pop-up message for Connectivity Settings
     */
    void fireInfoMessageAttributeChanged(const std::string& value);
    /**
     * description: refer struct Location.
    	    	Applink has to act based on this status
     */
    void fireLocationstsAttributeChanged(const ::v2::com::harman::voice::CCOIPresCtrl::Location& value);
    /**
     * description: TCU serial number will be mapped in the below parameter
     */
    void fireTcu_snAttributeChanged(const std::vector<uint8_t>& value);
    /**
     * description: refer struct AppBrandNames.
    	    	WifiPresCtrl has to retrieve this value and
     *   display their variable text screens accordingly based on the Names for the App
     *   & Brand
     */
    void fireAppBrandNamesAttributeChanged(const ::v2::com::harman::voice::CCOIPresCtrl::AppBrandNames& value);
    /**
     * description: refer struct syncPopupMessage
    		                  sync popups along with Ids,
     *   as per latest PTE & UFM received the message displayed in the pop-up is
     *   dynamic and shall retrieved from PTE, UFM
     */
    void fireSyncMessageAttributeChanged(const ::v2::com::harman::voice::CCOIPresCtrl::syncPopupMessage& value);



    const CCOIPresCtrlDBusStubAdapterHelper::StubDispatcherTable& getStubDispatcherTable();
    const CommonAPI::DBus::StubAttributeTable& getStubAttributeTable();

    void deactivateManagedInstances();
    

static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        CommonAPI::Version
        > getCCOIPresCtrlInterfaceVersionStubDispatcher;

/**
 * description: Refer the description in the enumerator LocDataShareIconValues 
		  This is for
 *   showing the location-data-icons as per policy files
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        ::v2::com::harman::voice::CCOIPresCtrl::LocDataShareIconValues                
        > getLoc_data_icon_valAttributeStubDispatcher;
/**
 * description: HMI can query the below method to get complete set of entries that need to be
 *   
		    populated in the settings screen
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        ::v2::com::harman::voice::CCOIPresCtrl::entity,
        CCOIPresCtrl_::entityDeployment_t                
        > getResponseAttributeStubDispatcher;
/**
 * description: attribute holding the info text pop-up message for Connectivity Settings
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        std::string,
        CommonAPI::DBus::StringDeployment                
        > getInfoMessageAttributeStubDispatcher;
/**
 * description: refer struct Location.
	    	Applink has to act based on this status
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        ::v2::com::harman::voice::CCOIPresCtrl::Location,
        CCOIPresCtrl_::LocationDeployment_t                
        > getLocationstsAttributeStubDispatcher;
/**
 * description: TCU serial number will be mapped in the below parameter
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        std::vector<uint8_t>,
        CommonAPI::DBus::ArrayDeployment<CommonAPI::EmptyDeployment>                
        > getTcu_snAttributeStubDispatcher;
/**
 * description: refer struct AppBrandNames.
	    	WifiPresCtrl has to retrieve this value and
 *   display their variable text screens accordingly based on the Names for the App
 *   & Brand
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        ::v2::com::harman::voice::CCOIPresCtrl::AppBrandNames,
        CCOIPresCtrl_::AppBrandNamesDeployment_t                
        > getAppBrandNamesAttributeStubDispatcher;
/**
 * description: refer struct syncPopupMessage
		                  sync popups along with Ids,
 *   as per latest PTE & UFM received the message displayed in the pop-up is
 *   dynamic and shall retrieved from PTE, UFM
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v2::com::harman::voice::CCOIPresCtrlStub,
        ::v2::com::harman::voice::CCOIPresCtrl::syncPopupMessage,
        CCOIPresCtrl_::syncPopupMessageDeployment_t                
        > getSyncMessageAttributeStubDispatcher;


/**
 * description: HMI can inform the CCOI module with the below method that user has changes the
 *   settings screen  
		    entity status
 * param: request (entity) : user changes setting entity's updated details
        
     
 *     	returns: response (entity) : true or false to indicate the success of
 *   updation of the status
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v2::com::harman::voice::CCOIPresCtrlStub,
    std::tuple<CCOIPresCtrl::entityDetails>,
    std::tuple<CCOIPresCtrl::entityDetails>,
    std::tuple<CCOIPresCtrl_::entityDetailsDeployment_t>,
    std::tuple<CCOIPresCtrl_::entityDetailsDeployment_t>
    
    > settingsEntriesUpdateStubDispatcher;




 protected:
    virtual const char* getMethodsDBusIntrospectionXmlData() const;

 private:
    CCOIPresCtrlDBusStubAdapterHelper::StubDispatcherTable stubDispatcherTable_;
    CommonAPI::DBus::StubAttributeTable stubAttributeTable_;
};

class CCOIPresCtrlDBusStubAdapter
    : public CCOIPresCtrlDBusStubAdapterInternal,
      public std::enable_shared_from_this<CCOIPresCtrlDBusStubAdapter> {
public:
    CCOIPresCtrlDBusStubAdapter(
    	const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
        const std::shared_ptr<CommonAPI::StubBase> &_stub)
    	: CommonAPI::DBus::DBusStubAdapter(
    		_address, 
    		_connection,
            false),
          CCOIPresCtrlDBusStubAdapterInternal(
          	_address, 
          	_connection, 
          	_stub) { 
    }
};

} // namespace voice
} // namespace harman
} // namespace com
} // namespace v2

#endif // V2_COM_HARMAN_VOICE_CCOI_Pres_Ctrl_DBUS_STUB_ADAPTER_HPP_
