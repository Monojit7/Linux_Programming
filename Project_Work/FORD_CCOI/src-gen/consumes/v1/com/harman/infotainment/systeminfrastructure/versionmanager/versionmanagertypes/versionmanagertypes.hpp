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
 * description: Required types for Version Manager interface.
 * author: Dinesh D(Dinesh.Dhanaraja(at)harman.com)
 */
#ifndef V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGERTYPES_versionmanagertypes_HPP_
#define V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGERTYPES_versionmanagertypes_HPP_



#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Types.hpp>
#include <cstdint>
#include <string>
#include <vector>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace com {
namespace harman {
namespace infotainment {
namespace systeminfrastructure {
namespace versionmanager {
namespace versionmanagertypes {

struct versionmanagertypes {
    
    struct VersionStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            VERSION_STATUS_INFO_AVAILABLE = 0,
            VERSION_STATUS_INFO_NOT_AVAILABLE = 1,
            VERSION_STATUS_ERROR = 2
        };
        
        VersionStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::VERSION_STATUS_INFO_AVAILABLE)) {}
        VersionStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::VERSION_STATUS_INFO_AVAILABLE):
                case static_cast<int32_t>(Literal::VERSION_STATUS_INFO_NOT_AVAILABLE):
                case static_cast<int32_t>(Literal::VERSION_STATUS_ERROR):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const VersionStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const VersionStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const VersionStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const VersionStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const VersionStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const VersionStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    /**
     * description: Information about version of a single component/module
     */
    struct VersionInfo : CommonAPI::Struct<std::string, std::string, VersionStatus> {
    	
    	VersionInfo() {
    	}
    	VersionInfo(const std::string &_versionName, const std::string &_versionData, const VersionStatus &_versionStatus)
    	{
    		std::get<0>(values_) = _versionName;
    		std::get<1>(values_) = _versionData;
    		std::get<2>(values_) = _versionStatus;
    	}
    	/**
    	 * description: Name of the component/module for which VersionInfo is provided
    	 */
    	inline const std::string &getVersionName() const { return std::get<0>(values_); }
    	inline void setVersionName(const std::string &_value) { std::get<0>(values_) = _value; }
    	/**
    	 * description: The actual version information of the component/module in the string format
    	   
    	 *           Valid only when version status is "VERSION_STATUS_INFO_AVAILABLE"
    	 */
    	inline const std::string &getVersionData() const { return std::get<1>(values_); }
    	inline void setVersionData(const std::string &_value) { std::get<1>(values_) = _value; }
    	/**
    	 * description: Information about the status of the version information
    	 */
    	inline const VersionStatus &getVersionStatus() const { return std::get<2>(values_); }
    	inline void setVersionStatus(const VersionStatus &_value) { std::get<2>(values_) = _value; }
    	inline bool operator==(const VersionInfo& _other) const {
                return (getVersionName() == _other.getVersionName() && getVersionData() == _other.getVersionData() && getVersionStatus() == _other.getVersionStatus());
        }
    	inline bool operator!=(const VersionInfo &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    /**
     * description: list of all the known version informations
     */
    typedef std::vector<versionmanagertypes::VersionInfo> VersionInfoList;



static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "com.harman.infotainment.systeminfrastructure.versionmanager.versionmanagertypes.versionmanagertypes";
    return typeCollectionName;
}

inline CommonAPI::Version getTypeCollectionVersion() {
    return CommonAPI::Version(1, 0);
}

}; // struct versionmanagertypes

} // namespace versionmanagertypes
} // namespace versionmanager
} // namespace systeminfrastructure
} // namespace infotainment
} // namespace harman
} // namespace com
} // namespace v1

namespace CommonAPI {
}


namespace std {
    //Hash for VersionStatus
    template<>
    struct hash<::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionStatus> {
        inline size_t operator()(const ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionStatus& versionStatus) const {
            return static_cast<int32_t>(versionStatus);
        }
    };
}


// Compatibility
namespace v1_0 = v1;

#endif // V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGERTYPES_versionmanagertypes_HPP_
