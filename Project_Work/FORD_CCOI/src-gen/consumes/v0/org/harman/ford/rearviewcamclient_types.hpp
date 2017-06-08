/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_ORG_HARMAN_FORD_rearviewcamclient_types_HPP_
#define V0_ORG_HARMAN_FORD_rearviewcamclient_types_HPP_



#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Types.hpp>
#include <cstdint>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace harman {
namespace ford {

struct rearviewcamclient_types {
    
    struct GearRvrse_D_Actl_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Fault = 7,
            En_NotUsed_3 = 6,
            En_NotUsed_2 = 5,
            En_NotUsed_1 = 4,
            En_Active_confirmed = 3,
            En_Active_not_confirmed = 2,
            En_Inactive_confirmed = 1,
            En_Inactive_not_confirmed = 0
        };
        
        GearRvrse_D_Actl_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Fault)) {}
        GearRvrse_D_Actl_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Fault):
                case static_cast<int32_t>(Literal::En_NotUsed_3):
                case static_cast<int32_t>(Literal::En_NotUsed_2):
                case static_cast<int32_t>(Literal::En_NotUsed_1):
                case static_cast<int32_t>(Literal::En_Active_confirmed):
                case static_cast<int32_t>(Literal::En_Active_not_confirmed):
                case static_cast<int32_t>(Literal::En_Inactive_confirmed):
                case static_cast<int32_t>(Literal::En_Inactive_not_confirmed):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const GearRvrse_D_Actl_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const GearRvrse_D_Actl_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const GearRvrse_D_Actl_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const GearRvrse_D_Actl_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const GearRvrse_D_Actl_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const GearRvrse_D_Actl_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct GearRvrse_D_Actl_struct : CommonAPI::Struct<GearRvrse_D_Actl_enum, uint8_t> {
    	
    	GearRvrse_D_Actl_struct() {
    	}
    	GearRvrse_D_Actl_struct(const GearRvrse_D_Actl_enum &_gearRvrse_D_Actl_enum, const uint8_t &_status)
    	{
    		std::get<0>(values_) = _gearRvrse_D_Actl_enum;
    		std::get<1>(values_) = _status;
    	}
    	inline const GearRvrse_D_Actl_enum &getGearRvrse_D_Actl_enum() const { return std::get<0>(values_); }
    	inline void setGearRvrse_D_Actl_enum(const GearRvrse_D_Actl_enum &_value) { std::get<0>(values_) = _value; }
    	inline const uint8_t &getStatus() const { return std::get<1>(values_); }
    	inline void setStatus(const uint8_t &_value) { std::get<1>(values_) = _value; }
    	inline bool operator==(const GearRvrse_D_Actl_struct& _other) const {
                return (getGearRvrse_D_Actl_enum() == _other.getGearRvrse_D_Actl_enum() && getStatus() == _other.getStatus());
        }
    	inline bool operator!=(const GearRvrse_D_Actl_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };



static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "org.harman.ford.rearviewcamclient_types";
    return typeCollectionName;
}

inline CommonAPI::Version getTypeCollectionVersion() {
    return CommonAPI::Version(0, 1);
}

}; // struct rearviewcamclient_types

} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

namespace CommonAPI {
}


namespace std {
    //Hash for GearRvrse_D_Actl_enum
    template<>
    struct hash<::v0::org::harman::ford::rearviewcamclient_types::GearRvrse_D_Actl_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::rearviewcamclient_types::GearRvrse_D_Actl_enum& gearRvrse_D_Actl_enum) const {
            return static_cast<int32_t>(gearRvrse_D_Actl_enum);
        }
    };
}


// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_HARMAN_FORD_rearviewcamclient_types_HPP_
