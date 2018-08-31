/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_ORG_HARMAN_FORD_key_input_types_HPP_
#define V0_ORG_HARMAN_FORD_key_input_types_HPP_



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

struct key_input_types {
    
    struct KeyMykeyTot_No_Cnt_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Thirteen = 13,
            En_Twelve = 12,
            En_Eleven = 11,
            En_Ten = 10,
            En_Nine = 9,
            En_Eight = 8,
            En_Seven = 7,
            En_Six = 6,
            En_Five = 5,
            En_Four = 4,
            En_Three = 3,
            En_Two = 2,
            En_One = 1,
            En_Zero = 0,
            En_Invalid = 15,
            En_Unlimited = 14
        };
        
        KeyMykeyTot_No_Cnt_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Thirteen)) {}
        KeyMykeyTot_No_Cnt_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Thirteen):
                case static_cast<int32_t>(Literal::En_Twelve):
                case static_cast<int32_t>(Literal::En_Eleven):
                case static_cast<int32_t>(Literal::En_Ten):
                case static_cast<int32_t>(Literal::En_Nine):
                case static_cast<int32_t>(Literal::En_Eight):
                case static_cast<int32_t>(Literal::En_Seven):
                case static_cast<int32_t>(Literal::En_Six):
                case static_cast<int32_t>(Literal::En_Five):
                case static_cast<int32_t>(Literal::En_Four):
                case static_cast<int32_t>(Literal::En_Three):
                case static_cast<int32_t>(Literal::En_Two):
                case static_cast<int32_t>(Literal::En_One):
                case static_cast<int32_t>(Literal::En_Zero):
                case static_cast<int32_t>(Literal::En_Invalid):
                case static_cast<int32_t>(Literal::En_Unlimited):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const KeyMykeyTot_No_Cnt_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct AssocConfirm_D_Actl_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_None = 0,
            En_Associate = 7,
            En_Keycode_Rejected = 6,
            En_Keycode_Accepted = 5,
            En_In_Progress = 4,
            En_Erase = 3,
            En_Duplicate = 2,
            En_Disassociate = 1
        };
        
        AssocConfirm_D_Actl_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_None)) {}
        AssocConfirm_D_Actl_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_None):
                case static_cast<int32_t>(Literal::En_Associate):
                case static_cast<int32_t>(Literal::En_Keycode_Rejected):
                case static_cast<int32_t>(Literal::En_Keycode_Accepted):
                case static_cast<int32_t>(Literal::En_In_Progress):
                case static_cast<int32_t>(Literal::En_Erase):
                case static_cast<int32_t>(Literal::En_Duplicate):
                case static_cast<int32_t>(Literal::En_Disassociate):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const AssocConfirm_D_Actl_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const AssocConfirm_D_Actl_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const AssocConfirm_D_Actl_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const AssocConfirm_D_Actl_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const AssocConfirm_D_Actl_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const AssocConfirm_D_Actl_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct Personality_BCM2_Data_HS3_struct : CommonAPI::Struct<AssocConfirm_D_Actl_enum> {
    	
    	Personality_BCM2_Data_HS3_struct() {
    	}
    	Personality_BCM2_Data_HS3_struct(const AssocConfirm_D_Actl_enum &_assocConfirm_D_Actl_enum)
    	{
    		std::get<0>(values_) = _assocConfirm_D_Actl_enum;
    	}
    	inline const AssocConfirm_D_Actl_enum &getAssocConfirm_D_Actl_enum() const { return std::get<0>(values_); }
    	inline void setAssocConfirm_D_Actl_enum(const AssocConfirm_D_Actl_enum &_value) { std::get<0>(values_) = _value; }
    	inline bool operator==(const Personality_BCM2_Data_HS3_struct& _other) const {
                return (getAssocConfirm_D_Actl_enum() == _other.getAssocConfirm_D_Actl_enum());
        }
    	inline bool operator!=(const Personality_BCM2_Data_HS3_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct Coding_BtnID_A_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Pressed = 1,
            En_Not_Pressed = 0
        };
        
        Coding_BtnID_A_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Pressed)) {}
        Coding_BtnID_A_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Pressed):
                case static_cast<int32_t>(Literal::En_Not_Pressed):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const Coding_BtnID_A_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const Coding_BtnID_A_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const Coding_BtnID_A_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const Coding_BtnID_A_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const Coding_BtnID_A_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const Coding_BtnID_A_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct EFP_Button_Press_struct : CommonAPI::Struct<uint8_t, Coding_BtnID_A_enum> {
    	
    	EFP_Button_Press_struct() {
    	}
    	EFP_Button_Press_struct(const uint8_t &_BtnID_A, const Coding_BtnID_A_enum &_coding_BtnID_A_enum)
    	{
    		std::get<0>(values_) = _BtnID_A;
    		std::get<1>(values_) = _coding_BtnID_A_enum;
    	}
    	inline const uint8_t &getBtnID_A() const { return std::get<0>(values_); }
    	inline void setBtnID_A(const uint8_t &_value) { std::get<0>(values_) = _value; }
    	inline const Coding_BtnID_A_enum &getCoding_BtnID_A_enum() const { return std::get<1>(values_); }
    	inline void setCoding_BtnID_A_enum(const Coding_BtnID_A_enum &_value) { std::get<1>(values_) = _value; }
    	inline bool operator==(const EFP_Button_Press_struct& _other) const {
                return (getBtnID_A() == _other.getBtnID_A() && getCoding_BtnID_A_enum() == _other.getCoding_BtnID_A_enum());
        }
    	inline bool operator!=(const EFP_Button_Press_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct ICI_Coding_BtnID_B_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Idle = 15,
            En_Stuck = 5,
            En_LongEvent = 4,
            En_ShortElapsed = 3,
            En_ShortEvent = 2,
            En_Active = 1,
            En_Inactive = 0
        };
        
        ICI_Coding_BtnID_B_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Idle)) {}
        ICI_Coding_BtnID_B_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Idle):
                case static_cast<int32_t>(Literal::En_Stuck):
                case static_cast<int32_t>(Literal::En_LongEvent):
                case static_cast<int32_t>(Literal::En_ShortElapsed):
                case static_cast<int32_t>(Literal::En_ShortEvent):
                case static_cast<int32_t>(Literal::En_Active):
                case static_cast<int32_t>(Literal::En_Inactive):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ICI_Coding_BtnID_B_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct ICI_Coding_BtnID_A_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Idle = 15,
            En_Stuck = 5,
            En_LongEvent = 4,
            En_ShortElapsed = 3,
            En_ShortEvent = 2,
            En_Active = 1,
            En_Inactive = 0
        };
        
        ICI_Coding_BtnID_A_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Idle)) {}
        ICI_Coding_BtnID_A_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Idle):
                case static_cast<int32_t>(Literal::En_Stuck):
                case static_cast<int32_t>(Literal::En_LongEvent):
                case static_cast<int32_t>(Literal::En_ShortElapsed):
                case static_cast<int32_t>(Literal::En_ShortEvent):
                case static_cast<int32_t>(Literal::En_Active):
                case static_cast<int32_t>(Literal::En_Inactive):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ICI_Coding_BtnID_A_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct ICI_Coding_BtnID_D_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Idle = 15,
            En_Stuck = 5,
            En_LongEvent = 4,
            En_ShortElapsed = 3,
            En_ShortEvent = 2,
            En_Active = 1,
            En_Inactive = 0
        };
        
        ICI_Coding_BtnID_D_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Idle)) {}
        ICI_Coding_BtnID_D_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Idle):
                case static_cast<int32_t>(Literal::En_Stuck):
                case static_cast<int32_t>(Literal::En_LongEvent):
                case static_cast<int32_t>(Literal::En_ShortElapsed):
                case static_cast<int32_t>(Literal::En_ShortEvent):
                case static_cast<int32_t>(Literal::En_Active):
                case static_cast<int32_t>(Literal::En_Inactive):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ICI_Coding_BtnID_D_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct ICI_Coding_BtnID_C_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Idle = 15,
            En_Stuck = 5,
            En_LongEvent = 4,
            En_ShortElapsed = 3,
            En_ShortEvent = 2,
            En_Active = 1,
            En_Inactive = 0
        };
        
        ICI_Coding_BtnID_C_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Idle)) {}
        ICI_Coding_BtnID_C_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Idle):
                case static_cast<int32_t>(Literal::En_Stuck):
                case static_cast<int32_t>(Literal::En_LongEvent):
                case static_cast<int32_t>(Literal::En_ShortElapsed):
                case static_cast<int32_t>(Literal::En_ShortEvent):
                case static_cast<int32_t>(Literal::En_Active):
                case static_cast<int32_t>(Literal::En_Inactive):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ICI_Coding_BtnID_C_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct MC_Send_Signals_1_struct : CommonAPI::Struct<uint8_t, uint8_t, uint8_t, uint8_t, ICI_Coding_BtnID_B_enum, ICI_Coding_BtnID_A_enum, ICI_Coding_BtnID_D_enum, ICI_Coding_BtnID_C_enum> {
    	
    	MC_Send_Signals_1_struct() {
    	}
    	MC_Send_Signals_1_struct(const uint8_t &_ICI_BtnID_A, const uint8_t &_ICI_BtnID_B, const uint8_t &_ICI_BtnID_C, const uint8_t &_ICI_BtnID_D, const ICI_Coding_BtnID_B_enum &_iCI_Coding_BtnID_B_enum, const ICI_Coding_BtnID_A_enum &_iCI_Coding_BtnID_A_enum, const ICI_Coding_BtnID_D_enum &_iCI_Coding_BtnID_D_enum, const ICI_Coding_BtnID_C_enum &_iCI_Coding_BtnID_C_enum)
    	{
    		std::get<0>(values_) = _ICI_BtnID_A;
    		std::get<1>(values_) = _ICI_BtnID_B;
    		std::get<2>(values_) = _ICI_BtnID_C;
    		std::get<3>(values_) = _ICI_BtnID_D;
    		std::get<4>(values_) = _iCI_Coding_BtnID_B_enum;
    		std::get<5>(values_) = _iCI_Coding_BtnID_A_enum;
    		std::get<6>(values_) = _iCI_Coding_BtnID_D_enum;
    		std::get<7>(values_) = _iCI_Coding_BtnID_C_enum;
    	}
    	inline const uint8_t &getICI_BtnID_A() const { return std::get<0>(values_); }
    	inline void setICI_BtnID_A(const uint8_t &_value) { std::get<0>(values_) = _value; }
    	inline const uint8_t &getICI_BtnID_B() const { return std::get<1>(values_); }
    	inline void setICI_BtnID_B(const uint8_t &_value) { std::get<1>(values_) = _value; }
    	inline const uint8_t &getICI_BtnID_C() const { return std::get<2>(values_); }
    	inline void setICI_BtnID_C(const uint8_t &_value) { std::get<2>(values_) = _value; }
    	inline const uint8_t &getICI_BtnID_D() const { return std::get<3>(values_); }
    	inline void setICI_BtnID_D(const uint8_t &_value) { std::get<3>(values_) = _value; }
    	inline const ICI_Coding_BtnID_B_enum &getICI_Coding_BtnID_B_enum() const { return std::get<4>(values_); }
    	inline void setICI_Coding_BtnID_B_enum(const ICI_Coding_BtnID_B_enum &_value) { std::get<4>(values_) = _value; }
    	inline const ICI_Coding_BtnID_A_enum &getICI_Coding_BtnID_A_enum() const { return std::get<5>(values_); }
    	inline void setICI_Coding_BtnID_A_enum(const ICI_Coding_BtnID_A_enum &_value) { std::get<5>(values_) = _value; }
    	inline const ICI_Coding_BtnID_D_enum &getICI_Coding_BtnID_D_enum() const { return std::get<6>(values_); }
    	inline void setICI_Coding_BtnID_D_enum(const ICI_Coding_BtnID_D_enum &_value) { std::get<6>(values_) = _value; }
    	inline const ICI_Coding_BtnID_C_enum &getICI_Coding_BtnID_C_enum() const { return std::get<7>(values_); }
    	inline void setICI_Coding_BtnID_C_enum(const ICI_Coding_BtnID_C_enum &_value) { std::get<7>(values_) = _value; }
    	inline bool operator==(const MC_Send_Signals_1_struct& _other) const {
                return (getICI_BtnID_A() == _other.getICI_BtnID_A() && getICI_BtnID_B() == _other.getICI_BtnID_B() && getICI_BtnID_C() == _other.getICI_BtnID_C() && getICI_BtnID_D() == _other.getICI_BtnID_D() && getICI_Coding_BtnID_B_enum() == _other.getICI_Coding_BtnID_B_enum() && getICI_Coding_BtnID_A_enum() == _other.getICI_Coding_BtnID_A_enum() && getICI_Coding_BtnID_D_enum() == _other.getICI_Coding_BtnID_D_enum() && getICI_Coding_BtnID_C_enum() == _other.getICI_Coding_BtnID_C_enum());
        }
    	inline bool operator!=(const MC_Send_Signals_1_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct Coding_BtnID_A_EFP2_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_Pressed = 1,
            En_Not_Pressed = 0
        };
        
        Coding_BtnID_A_EFP2_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_Pressed)) {}
        Coding_BtnID_A_EFP2_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_Pressed):
                case static_cast<int32_t>(Literal::En_Not_Pressed):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const Coding_BtnID_A_EFP2_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct EFP2_Button_Press_struct : CommonAPI::Struct<uint8_t, Coding_BtnID_A_EFP2_enum> {
    	
    	EFP2_Button_Press_struct() {
    	}
    	EFP2_Button_Press_struct(const uint8_t &_BtnID_A_EFP2, const Coding_BtnID_A_EFP2_enum &_coding_BtnID_A_EFP2_enum)
    	{
    		std::get<0>(values_) = _BtnID_A_EFP2;
    		std::get<1>(values_) = _coding_BtnID_A_EFP2_enum;
    	}
    	inline const uint8_t &getBtnID_A_EFP2() const { return std::get<0>(values_); }
    	inline void setBtnID_A_EFP2(const uint8_t &_value) { std::get<0>(values_) = _value; }
    	inline const Coding_BtnID_A_EFP2_enum &getCoding_BtnID_A_EFP2_enum() const { return std::get<1>(values_); }
    	inline void setCoding_BtnID_A_EFP2_enum(const Coding_BtnID_A_EFP2_enum &_value) { std::get<1>(values_) = _value; }
    	inline bool operator==(const EFP2_Button_Press_struct& _other) const {
                return (getBtnID_A_EFP2() == _other.getBtnID_A_EFP2() && getCoding_BtnID_A_EFP2_enum() == _other.getCoding_BtnID_A_EFP2_enum());
        }
    	inline bool operator!=(const EFP2_Button_Press_struct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct KeyOffMde_D_Actl_enum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            En_NotUsed_11 = 15,
            En_NotUsed_10 = 14,
            En_NotUsed_9 = 13,
            En_NotUsed_8 = 12,
            En_NotUsed_7 = 11,
            En_NotUsed_6 = 10,
            En_NotUsed_5 = 9,
            En_NotUsed_4 = 8,
            En_NotUsed_3 = 7,
            En_NotUsed_2 = 6,
            En_NotUsed_1 = 5,
            En_CriticalBattery = 4,
            En_Hibernate = 3,
            En_TransportPower = 2,
            En_FactoryPower = 1,
            En_NormalPower = 0
        };
        
        KeyOffMde_D_Actl_enum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::En_NotUsed_11)) {}
        KeyOffMde_D_Actl_enum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::En_NotUsed_11):
                case static_cast<int32_t>(Literal::En_NotUsed_10):
                case static_cast<int32_t>(Literal::En_NotUsed_9):
                case static_cast<int32_t>(Literal::En_NotUsed_8):
                case static_cast<int32_t>(Literal::En_NotUsed_7):
                case static_cast<int32_t>(Literal::En_NotUsed_6):
                case static_cast<int32_t>(Literal::En_NotUsed_5):
                case static_cast<int32_t>(Literal::En_NotUsed_4):
                case static_cast<int32_t>(Literal::En_NotUsed_3):
                case static_cast<int32_t>(Literal::En_NotUsed_2):
                case static_cast<int32_t>(Literal::En_NotUsed_1):
                case static_cast<int32_t>(Literal::En_CriticalBattery):
                case static_cast<int32_t>(Literal::En_Hibernate):
                case static_cast<int32_t>(Literal::En_TransportPower):
                case static_cast<int32_t>(Literal::En_FactoryPower):
                case static_cast<int32_t>(Literal::En_NormalPower):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const KeyOffMde_D_Actl_enum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const KeyOffMde_D_Actl_enum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const KeyOffMde_D_Actl_enum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const KeyOffMde_D_Actl_enum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const KeyOffMde_D_Actl_enum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const KeyOffMde_D_Actl_enum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };



static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "org.harman.ford.key_input_types";
    return typeCollectionName;
}

inline CommonAPI::Version getTypeCollectionVersion() {
    return CommonAPI::Version(0, 1);
}

}; // struct key_input_types

} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

namespace CommonAPI {
}


namespace std {
    //Hash for KeyMykeyTot_No_Cnt_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::KeyMykeyTot_No_Cnt_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::KeyMykeyTot_No_Cnt_enum& keyMykeyTot_No_Cnt_enum) const {
            return static_cast<int32_t>(keyMykeyTot_No_Cnt_enum);
        }
    };
    //Hash for AssocConfirm_D_Actl_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::AssocConfirm_D_Actl_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::AssocConfirm_D_Actl_enum& assocConfirm_D_Actl_enum) const {
            return static_cast<int32_t>(assocConfirm_D_Actl_enum);
        }
    };
    //Hash for Coding_BtnID_A_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::Coding_BtnID_A_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::Coding_BtnID_A_enum& coding_BtnID_A_enum) const {
            return static_cast<int32_t>(coding_BtnID_A_enum);
        }
    };
    //Hash for ICI_Coding_BtnID_B_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_B_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_B_enum& iCI_Coding_BtnID_B_enum) const {
            return static_cast<int32_t>(iCI_Coding_BtnID_B_enum);
        }
    };
    //Hash for ICI_Coding_BtnID_A_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_A_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_A_enum& iCI_Coding_BtnID_A_enum) const {
            return static_cast<int32_t>(iCI_Coding_BtnID_A_enum);
        }
    };
    //Hash for ICI_Coding_BtnID_D_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_D_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_D_enum& iCI_Coding_BtnID_D_enum) const {
            return static_cast<int32_t>(iCI_Coding_BtnID_D_enum);
        }
    };
    //Hash for ICI_Coding_BtnID_C_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_C_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::ICI_Coding_BtnID_C_enum& iCI_Coding_BtnID_C_enum) const {
            return static_cast<int32_t>(iCI_Coding_BtnID_C_enum);
        }
    };
    //Hash for Coding_BtnID_A_EFP2_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::Coding_BtnID_A_EFP2_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::Coding_BtnID_A_EFP2_enum& coding_BtnID_A_EFP2_enum) const {
            return static_cast<int32_t>(coding_BtnID_A_EFP2_enum);
        }
    };
    //Hash for KeyOffMde_D_Actl_enum
    template<>
    struct hash<::v0::org::harman::ford::key_input_types::KeyOffMde_D_Actl_enum> {
        inline size_t operator()(const ::v0::org::harman::ford::key_input_types::KeyOffMde_D_Actl_enum& keyOffMde_D_Actl_enum) const {
            return static_cast<int32_t>(keyOffMde_D_Actl_enum);
        }
    };
}


// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_HARMAN_FORD_key_input_types_HPP_
