/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
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
#ifndef V1_ORG_GENIVI_MEDIAMANAGER_Indexer_PROXY_HPP_
#define V1_ORG_GENIVI_MEDIAMANAGER_Indexer_PROXY_HPP_

#include <v1/org/genivi/mediamanager/IndexerProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/AttributeExtension.hpp>
#include <CommonAPI/Factory.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace org {
namespace genivi {
namespace mediamanager {

template <typename ... _AttributeExtensions>
class IndexerProxy
    : virtual public Indexer, 
      virtual public IndexerProxyBase,
      virtual public _AttributeExtensions... {
public:
    IndexerProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~IndexerProxy();

    typedef Indexer InterfaceType;


    /**
     * description: Get the current status of the indexer.
     */
    /**
     * Returns the wrapper class that provides access to the attribute indexerStatus.
     */
    virtual IndexerStatusAttribute& getIndexerStatusAttribute() {
        return delegate_->getIndexerStatusAttribute();
    }

    /**
     * description: Emit an event to show the database error.                     e: error code of
     *   database.
     */
    /**
     * Returns the wrapper class that provides access to the broadcast databaseError.
     */
    virtual DatabaseErrorEvent& getDatabaseErrorEvent() {
        return delegate_->getDatabaseErrorEvent();
    }

    /**
     * description: Get the path to the database file in the host file system.                     
     *    This is the database file used by LMS to store metadata
     * param: output: Path to database file in host file system
     * deprecated: This is only used for testing purposes and will be removed
     * Calls getDatabasePath with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual COMMONAPI_DEPRECATED void getDatabasePath(CommonAPI::CallStatus &_internalCallStatus, std::string &_output, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls getDatabasePath with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> getDatabasePathAsync(GetDatabasePathAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: remove the database file of specified device.
     * param: id: the specified device id
     * Calls removeDatabase with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void removeDatabase(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls removeDatabase with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> removeDatabaseAsync(const uint32_t &_id, RemoveDatabaseAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: If the indexer is currently IDLE or RUNNING                       (see
     *   IndexerStatus), stop indexing and set IndexerStatus                       to
     *   STOPPED.                       If indexer is currently STOPPED, no action is
     *   taken
     * Calls stopIndexing with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void stopIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls stopIndexing with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> stopIndexingAsync(StopIndexingAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: If indexer is currently STOPPED (see IndexerStatus),                      
     *   status is changed to RUNNING and indexing is RUNNING. If                      
     *   the status is IDLE or RUNNING, no action is taken.                       Note
     *   that the status may go from RUNNING to IDLE directly                      
     *   after issuing this call, if no files are to be indexed
     * Calls startIndexing with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void startIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls startIndexing with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> startIndexingAsync(StartIndexingAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: re-indexing a device,      				  all the device indices will be deleted and
     *   start indexing     			      from fresh.
     * Calls reIndexing with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void reIndexing(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls reIndexing with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> reIndexingAsync(const uint32_t &_id, ReIndexingAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: set index options.                 this method MUST be invoked before
     *   startIndexing if project                 need special behavior for indexing.
     * param: option, the option want to set.                 optionValue, the value for the
     *   option, can be empty string for some options.
     * Calls setIndexOption with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void setIndexOption(const Indexer::IndexOption &_option, const std::string &_optionValue, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls setIndexOption with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> setIndexOptionAsync(const Indexer::IndexOption &_option, const std::string &_optionValue, SetIndexOptionAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


    /**
     * Returns the CommonAPI address of the remote partner this proxy communicates with.
     */
    virtual const CommonAPI::Address &getAddress() const;

    /**
     * Returns true if the remote partner for this proxy is currently known to be available.
     */
    virtual bool isAvailable() const;

    /**
     * Returns true if the remote partner for this proxy is available.
     */
    virtual bool isAvailableBlocking() const;

    /**
     * Returns the wrapper class that is used to (de-)register for notifications about
     * the availability of the remote partner of this proxy.
     */
    virtual CommonAPI::ProxyStatusEvent& getProxyStatusEvent();

    /**
     * Returns the wrapper class that is used to access version information of the remote
     * partner of this proxy.
     */
    virtual CommonAPI::InterfaceVersionAttribute& getInterfaceVersionAttribute();

 private:
    std::shared_ptr<IndexerProxyBase> delegate_;
};

typedef IndexerProxy<> IndexerProxyDefault;

namespace IndexerExtensions {
    /**
     * description: Get the current status of the indexer.
     */
    template <template <typename > class _ExtensionType>
    class IndexerStatusAttributeExtension {
     public:
        typedef _ExtensionType<IndexerProxyBase::IndexerStatusAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<IndexerProxyBase::IndexerStatusAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        IndexerStatusAttributeExtension() {}
    #endif
    
        IndexerStatusAttributeExtension(IndexerProxyBase& proxy): attributeExtension_(proxy.getIndexerStatusAttribute()) {
        }
    
        inline extension_type& getIndexerStatusAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

} // namespace IndexerExtensions

//
// IndexerProxy Implementation
//
template <typename ... _AttributeExtensions>
IndexerProxy<_AttributeExtensions...>::IndexerProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<IndexerProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<IndexerProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
IndexerProxy<_AttributeExtensions...>::~IndexerProxy() {
}

/**
 * description: Get the path to the database file in the host file system.                     
 *    This is the database file used by LMS to store metadata
 * param: output: Path to database file in host file system
 * deprecated: This is only used for testing purposes and will be removed
 */
template <typename ... _AttributeExtensions>
COMMONAPI_DEPRECATED void IndexerProxy<_AttributeExtensions...>::getDatabasePath(CommonAPI::CallStatus &_internalCallStatus, std::string &_output, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    delegate_->getDatabasePath(_internalCallStatus, _output, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::getDatabasePathAsync(GetDatabasePathAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->getDatabasePathAsync(_callback, _info);
}
/**
 * description: remove the database file of specified device.
 * param: id: the specified device id
 */
template <typename ... _AttributeExtensions>
void IndexerProxy<_AttributeExtensions...>::removeDatabase(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    delegate_->removeDatabase(_id, _internalCallStatus, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::removeDatabaseAsync(const uint32_t &_id, RemoveDatabaseAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->removeDatabaseAsync(_id, _callback, _info);
}
/**
 * description: If the indexer is currently IDLE or RUNNING                       (see
 *   IndexerStatus), stop indexing and set IndexerStatus                       to
 *   STOPPED.                       If indexer is currently STOPPED, no action is
 *   taken
 */
template <typename ... _AttributeExtensions>
void IndexerProxy<_AttributeExtensions...>::stopIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    delegate_->stopIndexing(_internalCallStatus, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::stopIndexingAsync(StopIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->stopIndexingAsync(_callback, _info);
}
/**
 * description: If indexer is currently STOPPED (see IndexerStatus),                      
 *   status is changed to RUNNING and indexing is RUNNING. If                      
 *   the status is IDLE or RUNNING, no action is taken.                       Note
 *   that the status may go from RUNNING to IDLE directly                      
 *   after issuing this call, if no files are to be indexed
 */
template <typename ... _AttributeExtensions>
void IndexerProxy<_AttributeExtensions...>::startIndexing(CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    delegate_->startIndexing(_internalCallStatus, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::startIndexingAsync(StartIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->startIndexingAsync(_callback, _info);
}
/**
 * description: re-indexing a device,      				  all the device indices will be deleted and
 *   start indexing     			      from fresh.
 */
template <typename ... _AttributeExtensions>
void IndexerProxy<_AttributeExtensions...>::reIndexing(const uint32_t &_id, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    delegate_->reIndexing(_id, _internalCallStatus, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::reIndexingAsync(const uint32_t &_id, ReIndexingAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->reIndexingAsync(_id, _callback, _info);
}
/**
 * description: set index options.                 this method MUST be invoked before
 *   startIndexing if project                 need special behavior for indexing.
 * param: option, the option want to set.                 optionValue, the value for the
 *   option, can be empty string for some options.
 */
template <typename ... _AttributeExtensions>
void IndexerProxy<_AttributeExtensions...>::setIndexOption(const Indexer::IndexOption &_option, const std::string &_optionValue, CommonAPI::CallStatus &_internalCallStatus, Indexer::IndexerError &_e, const CommonAPI::CallInfo *_info) {
    if (!_option.validate()) {
        _internalCallStatus = CommonAPI::CallStatus::INVALID_VALUE;
        return;
    }
    delegate_->setIndexOption(_option, _optionValue, _internalCallStatus, _e, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> IndexerProxy<_AttributeExtensions...>::setIndexOptionAsync(const Indexer::IndexOption &_option, const std::string &_optionValue, SetIndexOptionAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    if (!_option.validate()) {
        Indexer::IndexerError e = Indexer::IndexerError::BACKEND_UNREACHABLE;
        _callback(CommonAPI::CallStatus::INVALID_VALUE, e);
        std::promise<CommonAPI::CallStatus> promise;
        promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
        return promise.get_future();
    }
    return delegate_->setIndexOptionAsync(_option, _optionValue, _callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &IndexerProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool IndexerProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool IndexerProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& IndexerProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& IndexerProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace mediamanager
} // namespace genivi
} // namespace org
} // namespace v1

namespace CommonAPI {
template<template<typename > class _AttributeExtension>
struct DefaultAttributeProxyHelper<::v1::org::genivi::mediamanager::IndexerProxy,
    _AttributeExtension> {
    typedef typename ::v1::org::genivi::mediamanager::IndexerProxy<
            ::v1::org::genivi::mediamanager::IndexerExtensions::IndexerStatusAttributeExtension<_AttributeExtension>
    > class_t;
};
}


// Compatibility
namespace v1_0 = v1;

#endif // V1_ORG_GENIVI_MEDIAMANAGER_Indexer_PROXY_HPP_
