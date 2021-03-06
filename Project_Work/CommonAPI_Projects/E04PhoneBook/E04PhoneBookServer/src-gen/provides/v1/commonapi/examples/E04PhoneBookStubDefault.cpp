/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.5.
* Used org.franca.core 0.9.1.201412191134.
*
* generated by Voice CodeGen Version: R1_v1.3.0
* generated on: Fri Jun 23 13:08:16 IST 2017
*/
#include <v1/commonapi/examples/E04PhoneBookStubDefault.hpp>
#include <assert.h>

namespace v1 {
namespace commonapi {
namespace examples {

E04PhoneBookStubDefault::E04PhoneBookStubDefault():
        remoteEventHandler_(this),
        interfaceVersion_(E04PhoneBook::getInterfaceVersion()) {
}

const CommonAPI::Version& E04PhoneBookStubDefault::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client) {
    (void)_client;
    return interfaceVersion_;
}

E04PhoneBookStubRemoteEvent* E04PhoneBookStubDefault::initStubAdapter(const std::shared_ptr<E04PhoneBookStubAdapter> &_adapter) {
    CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_ = _adapter;
    return &remoteEventHandler_;
}

const std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookStruct>& E04PhoneBookStubDefault::getPhoneBookAttribute() {
    return phoneBookAttributeValue_;
}

const std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookStruct>& E04PhoneBookStubDefault::getPhoneBookAttribute(const std::shared_ptr<CommonAPI::ClientId> _client) {
    (void)_client;
    return getPhoneBookAttribute();
}

void E04PhoneBookStubDefault::setPhoneBookAttribute(std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookStruct> _value) {
    const bool valueChanged = trySetPhoneBookAttribute(std::move(_value));
    if (valueChanged) {
        auto stubAdapter = CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_.lock();
        if (stubAdapter)
            stubAdapter->firePhoneBookAttributeChanged(phoneBookAttributeValue_);
    }
}

bool E04PhoneBookStubDefault::trySetPhoneBookAttribute(std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookStruct> _value) {
    if (!validatePhoneBookAttributeRequestedValue(_value))
        return false;

    const bool valueChanged = (phoneBookAttributeValue_ != _value);
    phoneBookAttributeValue_ = std::move(_value);
    return valueChanged;
}

bool E04PhoneBookStubDefault::validatePhoneBookAttributeRequestedValue(const std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookStruct> &_value) {
    (void)_value;
	return true;
}



/**
 * description: filter operations
 */
void E04PhoneBookStubDefault::setPhoneBookDataFilter(const std::shared_ptr<CommonAPI::ClientId> _client, E04PhoneBook::elementFilterStruct _elementFilter, std::vector<E04PhoneBook::contentFilterStruct> _contentFilter, setPhoneBookDataFilterReply_t _reply) {
    (void)_client;
    (void) _elementFilter;
    (void) _contentFilter;
    _reply();
}


/**
 * description: filter result
 */
void E04PhoneBookStubDefault::firePhoneBookDataSetSelective(const std::vector<::v1::commonapi::examples::E04PhoneBook::phoneBookDataElementMap> &_phoneBookDataSet, const std::shared_ptr<CommonAPI::ClientIdList> _receivers) {
    assert((CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_.lock()) !=NULL);
    auto stubAdapter = CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_.lock();
    if (stubAdapter)
        stubAdapter->sendPhoneBookDataSetSelective(_phoneBookDataSet, _receivers);
}
void E04PhoneBookStubDefault::onPhoneBookDataSetSelectiveSubscriptionChanged(const std::shared_ptr<CommonAPI::ClientId> _client, const CommonAPI::SelectiveBroadcastSubscriptionEvent _event) {
    (void)_client;
    (void)_event;
    // No operation in default
}
bool E04PhoneBookStubDefault::onPhoneBookDataSetSelectiveSubscriptionRequested(const std::shared_ptr<CommonAPI::ClientId> _client) {
    (void)_client;
    // Accept in default
    return true;
}
std::shared_ptr<CommonAPI::ClientIdList> const E04PhoneBookStubDefault::getSubscribersForPhoneBookDataSetSelective() {
    assert((CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_.lock()) !=NULL);
    auto stubAdapter = CommonAPI::Stub<E04PhoneBookStubAdapter, E04PhoneBookStubRemoteEvent>::stubAdapter_.lock();
   	if (stubAdapter)
        return(stubAdapter->getSubscribersForPhoneBookDataSetSelective());
    else
        return NULL;
}



E04PhoneBookStubDefault::RemoteEventHandler::RemoteEventHandler(E04PhoneBookStubDefault *_defaultStub)
    : 
      defaultStub_(_defaultStub) {
}

} // namespace examples
} // namespace commonapi
} // namespace v1
