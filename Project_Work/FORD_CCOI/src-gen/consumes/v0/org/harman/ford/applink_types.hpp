/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_ORG_HARMAN_FORD_applink_types_HPP_
#define V0_ORG_HARMAN_FORD_applink_types_HPP_



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

struct applink_types {
    
    struct WiprFront_D_Stat_UB_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Fresh_data = 1,
            En_Unchanged_data = 0
        };
        
        WiprFront_D_Stat_UB_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Fresh_data)) {}
        WiprFront_D_Stat_UB_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Fresh_data):
                case static_cast<int32_t>(Literal::En_Unchanged_data):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const WiprFront_D_Stat_UB_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct WiprFront_D_Stat_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_NO_DATA_EXISTS = 15,
            En_STALLED = 14,
            En_RESERVED = 13,
            En_AUTO_ADJUST = 12,
            En_COURTESYWIPE = 11,
            En_AUTO_HIGH = 10,
            En_AUTO_LOW = 9,
            En_WASH = 8,
            En_MIST_FLICK = 7,
            En_MAN_HIGH = 6,
            En_MAN_LOW = 5,
            En_MAN_INT_ON = 4,
            En_MAN_INT_OFF = 3,
            En_OFF_MOVING = 2,
            En_AUTO_OFF = 1,
            En_OFF = 0
        };
        
        WiprFront_D_Stat_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_NO_DATA_EXISTS)) {}
        WiprFront_D_Stat_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_NO_DATA_EXISTS):
                case static_cast<int32_t>(Literal::En_STALLED):
                case static_cast<int32_t>(Literal::En_RESERVED):
                case static_cast<int32_t>(Literal::En_AUTO_ADJUST):
                case static_cast<int32_t>(Literal::En_COURTESYWIPE):
                case static_cast<int32_t>(Literal::En_AUTO_HIGH):
                case static_cast<int32_t>(Literal::En_AUTO_LOW):
                case static_cast<int32_t>(Literal::En_WASH):
                case static_cast<int32_t>(Literal::En_MIST_FLICK):
                case static_cast<int32_t>(Literal::En_MAN_HIGH):
                case static_cast<int32_t>(Literal::En_MAN_LOW):
                case static_cast<int32_t>(Literal::En_MAN_INT_ON):
                case static_cast<int32_t>(Literal::En_MAN_INT_OFF):
                case static_cast<int32_t>(Literal::En_OFF_MOVING):
                case static_cast<int32_t>(Literal::En_AUTO_OFF):
                case static_cast<int32_t>(Literal::En_OFF):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const WiprFront_D_Stat_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const WiprFront_D_Stat_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const WiprFront_D_Stat_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const WiprFront_D_Stat_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const WiprFront_D_Stat_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const WiprFront_D_Stat_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct GPS_Compass_direction_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_North_West = 7,
            En_West = 6,
            En_South_West = 5,
            En_South = 4,
            En_South_East = 3,
            En_East = 2,
            En_North_East = 1,
            En_North = 0
        };
        
        GPS_Compass_direction_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_North_West)) {}
        GPS_Compass_direction_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_North_West):
                case static_cast<int32_t>(Literal::En_West):
                case static_cast<int32_t>(Literal::En_South_West):
                case static_cast<int32_t>(Literal::En_South):
                case static_cast<int32_t>(Literal::En_South_East):
                case static_cast<int32_t>(Literal::En_East):
                case static_cast<int32_t>(Literal::En_North_East):
                case static_cast<int32_t>(Literal::En_North):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const GPS_Compass_direction_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const GPS_Compass_direction_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const GPS_Compass_direction_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const GPS_Compass_direction_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const GPS_Compass_direction_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const GPS_Compass_direction_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct HeadLghtHiOn_B_Stat_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_On = 1,
            En_Off = 0
        };
        
        HeadLghtHiOn_B_Stat_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_On)) {}
        HeadLghtHiOn_B_Stat_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_On):
                case static_cast<int32_t>(Literal::En_Off):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const HeadLghtHiOn_B_Stat_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct HeadLampLoActv_B_Stat_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_On = 1,
            En_Off = 0
        };
        
        HeadLampLoActv_B_Stat_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_On)) {}
        HeadLampLoActv_B_Stat_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_On):
                case static_cast<int32_t>(Literal::En_Off):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const HeadLampLoActv_B_Stat_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct DrStatPsngr_B_Actl_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Ajar = 1,
            En_Closed = 0
        };
        
        DrStatPsngr_B_Actl_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Ajar)) {}
        DrStatPsngr_B_Actl_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Ajar):
                case static_cast<int32_t>(Literal::En_Closed):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const DrStatPsngr_B_Actl_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct BrakeSnData_2_sig_6_struct : CommonAPI::Struct<WiprFront_D_Stat_enum, WiprFront_D_Stat_UB_enum> {
    	
    	BrakeSnData_2_sig_6_struct() {
    	}
    	BrakeSnData_2_sig_6_struct(const WiprFront_D_Stat_enum &_wiprFront_D_Stat_enum, const WiprFront_D_Stat_UB_enum &_wiprFront_D_Stat_UB_enum)
    	{
    		std::get<0>(values_) = _wiprFront_D_Stat_enum;
    		std::get<1>(values_) = _wiprFront_D_Stat_UB_enum;
    	}
    	inline const WiprFront_D_Stat_enum &getWiprFront_D_Stat_enum() const { return std::get<0>(values_); }
    	inline void setWiprFront_D_Stat_enum(const WiprFront_D_Stat_enum &_value) { std::get<0>(values_) = _value; }
    	inline const WiprFront_D_Stat_UB_enum &getWiprFront_D_Stat_UB_enum() const { return std::get<1>(values_); }
    	inline void setWiprFront_D_Stat_UB_enum(const WiprFront_D_Stat_UB_enum &_value) { std::get<1>(values_) = _value; }
    	inline bool operator==(const BrakeSnData_2_sig_6_struct& _other) const {
                return (getWiprFront_D_Stat_enum() == _other.getWiprFront_D_Stat_enum() && getWiprFront_D_Stat_UB_enum() == _other.getWiprFront_D_Stat_UB_enum());
        }
    	inline bool operator!=(const BrakeSnData_2_sig_6_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };



static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "org.harman.ford.applink_types";
    return typeCollectionName;
}

inline CommonAPI::Version getTypeCollectionVersion() {
    return CommonAPI::Version(0, 1);
}

}; // struct applink_types

} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

namespace CommonAPI {
}


namespace std {
    //Hash for WiprFront_D_Stat_UB_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::WiprFront_D_Stat_UB_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::WiprFront_D_Stat_UB_enum& wiprFront_D_Stat_UB_enum) const {
            return static_cast<int32_t>(wiprFront_D_Stat_UB_enum);
        }
    };
    //Hash for WiprFront_D_Stat_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::WiprFront_D_Stat_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::WiprFront_D_Stat_enum& wiprFront_D_Stat_enum) const {
            return static_cast<int32_t>(wiprFront_D_Stat_enum);
        }
    };
    //Hash for GPS_Compass_direction_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::GPS_Compass_direction_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::GPS_Compass_direction_enum& gPS_Compass_direction_enum) const {
            return static_cast<int32_t>(gPS_Compass_direction_enum);
        }
    };
    //Hash for HeadLghtHiOn_B_Stat_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::HeadLghtHiOn_B_Stat_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::HeadLghtHiOn_B_Stat_enum& headLghtHiOn_B_Stat_enum) const {
            return static_cast<int32_t>(headLghtHiOn_B_Stat_enum);
        }
    };
    //Hash for HeadLampLoActv_B_Stat_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::HeadLampLoActv_B_Stat_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::HeadLampLoActv_B_Stat_enum& headLampLoActv_B_Stat_enum) const {
            return static_cast<int32_t>(headLampLoActv_B_Stat_enum);
        }
    };
    //Hash for DrStatPsngr_B_Actl_enum
    template<>
    struct hash<::v0::org::harman::ford::applink_types::DrStatPsngr_B_Actl_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::applink_types::DrStatPsngr_B_Actl_enum& drStatPsngr_B_Actl_enum) const {
            return static_cast<int32_t>(drStatPsngr_B_Actl_enum);
        }
    };
}


// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_HARMAN_FORD_applink_types_HPP_