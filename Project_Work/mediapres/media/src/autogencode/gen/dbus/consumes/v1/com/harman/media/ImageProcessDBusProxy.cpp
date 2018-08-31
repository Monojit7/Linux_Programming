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
 * description: ImageProcess provide image scale interfaces.
 */
#include <v1/com/harman/media/ImageProcessDBusProxy.hpp>

namespace v1 {
namespace com {
namespace harman {
namespace media {

std::shared_ptr<CommonAPI::DBus::DBusProxy> createImageProcessDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection) {
	return std::make_shared<ImageProcessDBusProxy>(_address, _connection);
}

INITIALIZER(registerImageProcessDBusProxy) {
     CommonAPI::DBus::Factory::get()->registerProxyCreateMethod(
		ImageProcess::getInterface(),
		&createImageProcessDBusProxy);
}

ImageProcessDBusProxy::ImageProcessDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection)
	:	CommonAPI::DBus::DBusProxy(_address, _connection)
{
}


    
    /**
     * description: scale images to specified resolution(width x height),write output files into
     *   tmpfs. 	      If width x height not honoring original aspect ratio,outWidth x
     *   OutHeight is the real resolution 	      for output file.
     * author: daoyong,tang
     */
    void ImageProcessDBusProxy::scaleImage(const std::string &_inputfile, const uint32_t &_width, const uint32_t &_height, CommonAPI::CallStatus &_internalCallStatus, std::string &_outputfile, uint32_t &_outWidth, uint32_t &_OutHeight, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment> deploy_inputfile(_inputfile, static_cast<CommonAPI::DBus::StringDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_width(_width, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_height(_height, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment> deploy_outputfile(static_cast<CommonAPI::DBus::StringDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_outWidth(static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_OutHeight(static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment >,
            CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment >,
            CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<std::string,CommonAPI::DBus::StringDeployment>,
            CommonAPI::Deployable<uint32_t,CommonAPI::EmptyDeployment>,
            CommonAPI::Deployable<uint32_t,CommonAPI::EmptyDeployment>
            >
        	>::callMethodWithReply(
        *this,
        "scaleImage",
        "suu",
(_info ? _info : &CommonAPI::DBus::defaultCallInfo),
deploy_inputfile, deploy_width, deploy_height,
_internalCallStatus,
deploy_outputfile, deploy_outWidth, deploy_OutHeight);
_outputfile = deploy_outputfile.getValue();
_outWidth = deploy_outWidth.getValue();
_OutHeight = deploy_OutHeight.getValue();
}
    std::future<CommonAPI::CallStatus> ImageProcessDBusProxy::scaleImageAsync(const std::string &_inputfile, const uint32_t &_width, const uint32_t &_height, ScaleImageAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment> deploy_inputfile(_inputfile, static_cast<CommonAPI::DBus::StringDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_width(_width, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_height(_height, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment> deploy_outputfile(static_cast<CommonAPI::DBus::StringDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_outWidth(static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> deploy_OutHeight(static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        return CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment >,
            CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment >,
            CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<std::string,CommonAPI::DBus::StringDeployment>,
            CommonAPI::Deployable<uint32_t,CommonAPI::EmptyDeployment>,
            CommonAPI::Deployable<uint32_t,CommonAPI::EmptyDeployment>
            >
        	>::callMethodAsync(
        *this,
        "scaleImage",
        "suu",
        (_info ? _info : &CommonAPI::DBus::defaultCallInfo),
        deploy_inputfile, deploy_width, deploy_height,
        [_callback] (CommonAPI::CallStatus _internalCallStatus, CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment> _outputfile, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> _outWidth, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment> _OutHeight) {
        	if (_callback)
        		_callback(_internalCallStatus, _outputfile.getValue(), _outWidth.getValue(), _OutHeight.getValue());
        },
        std::make_tuple(deploy_outputfile, deploy_outWidth, deploy_OutHeight));
    }


void ImageProcessDBusProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
          ownVersionMajor = 1;
          ownVersionMinor = 0;
      }

      } // namespace media
      } // namespace harman
      } // namespace com
      } // namespace v1
