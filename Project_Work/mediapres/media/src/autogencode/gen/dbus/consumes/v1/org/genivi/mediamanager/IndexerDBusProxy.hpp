/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
/**
 * description: Indexer control interface. Allows administerial tasks for                   the
 *   indexer backend, such as starting, stopping and requesting                  
 *   status of the indexer.                   For querying the database, please see
 *                     org.genivi.mediamanager.Browser
 */
#ifndef V1_ORG_GENIVI_MEDIAMANAGER_Indexer_DBUS_PROXY_HPP_
#define V1_ORG_GENIVI_MEDIAMANAGER_Indexer_DBUS_PROXY_HPP_

#include <v1/org/genivi/mediamanager/IndexerProxyBase.hpp>
#include "v1/org/genivi/mediamanager/IndexerDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusAttribute.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v1 {
namespace org {
namespace genivi {
namespace mediamanager {

class IndexerDBusProxy
    : virtual public IndexerProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    IndexerDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~IndexerDBusProxy() { }

    virtual IndexerStatusAttribute& getIndexerStatusAttribute();

    virtual DatabaseErrorEvent& getDatabaseErrorEvent();

    /**
     * description: Get the path to the database file in the host file system.                     
     *    This is the database file used by LMS to store metadata
     * param: output: Path to database file in host file system
     * deprecated: This is only used for testing purposes and will be removed
     */
    virtual COMMONAPI_DEPRECATED void getDatabasePath(CommonAPI::CallStatus &_internalCallStatus, std::string &_output, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getDatabasePathAsync(GetDatabasePathAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: remove the database file of specified device.
     * param: id: the specified device id
     */
    virtual void removeDatabase(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> removeDatabaseAsync(const uint32_t &_id, RemoveDatabaseAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: If the indexer is currently IDLE or RUNNING                       (see
     *   IndexerStatus), stop indexing and set IndexerStatus                       to
     *   STOPPED.                       If indexer is currently STOPPED, no action is
     *   taken
     */
    virtual void stopIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> stopIndexingAsync(StopIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: If indexer is currently STOPPED (see IndexerStatus),                      
     *   status is changed to RUNNING and indexing is RUNNING. If                      
     *   the status is IDLE or RUNNING, no action is taken.                       Note
     *   that the status may go from RUNNING to IDLE directly                      
     *   after issuing this call, if no files are to be indexed
     */
    virtual void startIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> startIndexingAsync(StartIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: re-indexing a device,      				  all the device indices will be deleted and
     *   start indexing     			      from fresh.
     */
    virtual void reIndexing(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> reIndexingAsync(const uint32_t &_id, ReIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: set index options.                 this method MUST be invoked before
     *   startIndexing if project                 need special behavior for indexing.
     * param: option, the option want to set.                 optionValue, the value for the
     *   option, can be empty string for some options.
     */
    virtual void setIndexOption(const Indexer::IndexOption &_option, const std::string &_optionValue, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setIndexOptionAsync(const Indexer::IndexOption &_option, const std::string &_optionValue, SetIndexOptionAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   
    class DBusindexerStatus_Attribute : public CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<IndexerStatusAttribute>> {
    public:
    template <typename... _A>
    	DBusindexerStatus_Attribute(DBusProxy &_proxy,
    		_A ... arguments)
    		: CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<IndexerStatusAttribute>>(
    			_proxy, arguments...) {}
    };
    DBusindexerStatus_Attribute indexerStatus_;

    CommonAPI::DBus::DBusEvent<DatabaseErrorEvent, CommonAPI::Deployable<::v1::org::genivi::mediamanager::MediaTypes::DatabaseError, CommonAPI::EmptyDeployment>> databaseError_;

};

} // namespace mediamanager
} // namespace genivi
} // namespace org
} // namespace v1

#endif // V1_ORG_GENIVI_MEDIAMANAGER_Indexer_DBUS_PROXY_HPP_

