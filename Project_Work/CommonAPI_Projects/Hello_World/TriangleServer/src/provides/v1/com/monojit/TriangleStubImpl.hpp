/**
 * generated by Voice CodeGen Version: R1_v1.3.0
 * generated on: Mon Nov 14 16:54:17 IST 2016
 */

#ifndef V1_COM_MONOJIT_TRIANGLESTUBIMPL_HPP
#define V1_COM_MONOJIT_TRIANGLESTUBIMPL_HPP

#include <CommonAPI/CommonAPI.hpp>
#include "v1/com/monojit/TriangleStubDefault.hpp"

namespace v1 {
namespace com {
namespace monojit {

class TriangleStubImpl : public v1::com::monojit::TriangleStubDefault
{

public:
	TriangleStubImpl();
    virtual ~TriangleStubImpl();

	virtual void Area(const std::shared_ptr<CommonAPI::ClientId> _client, AreaReply_t);

	virtual void onRemoteWidthAttributeChanged();
	virtual bool onRemoteSetWidthAttribute(uint32_t _value);
	virtual bool onRemoteSetWidthAttribute(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _value);
};

} // namespace monojit
} // namespace com
} // namespace v1

#endif //V1_COM_MONOJIT_TRIANGLESTUBIMPL_HPP