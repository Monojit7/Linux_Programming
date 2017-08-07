/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.5.
* Used org.franca.core 0.9.1.201412191134.
*
* generated by Voice CodeGen Version: R1_v1.3.0
* generated on: Fri Jun 23 13:08:16 IST 2017
*/
#ifndef V1_COMMONAPI_EXAMPLES_E04_PHONE_BOOK_HPP_
#define V1_COMMONAPI_EXAMPLES_E04_PHONE_BOOK_HPP_




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
#include <unordered_map>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace commonapi {
namespace examples {

class E04PhoneBook {
public:
    virtual ~E04PhoneBook() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
    
    struct phoneNumberEnum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            WORK = 0,
            HOME = 1,
            MOBILE1 = 2,
            MOBILE2 = 3
        };
        
        phoneNumberEnum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::WORK)) {}
        phoneNumberEnum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::WORK):
                case static_cast<int32_t>(Literal::HOME):
                case static_cast<int32_t>(Literal::MOBILE1):
                case static_cast<int32_t>(Literal::MOBILE2):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const phoneNumberEnum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const phoneNumberEnum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const phoneNumberEnum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const phoneNumberEnum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const phoneNumberEnum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const phoneNumberEnum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    typedef std::unordered_map<::v1::commonapi::examples::E04PhoneBook::phoneNumberEnum, std::string, CommonAPI::EnumHasher<::v1::commonapi::examples::E04PhoneBook::phoneNumberEnum>> phoneNumberMap;
    struct phoneBookStruct : CommonAPI::Struct<std::string, std::string, std::string, std::string, std::string, uint32_t, phoneNumberMap> {
    	
    	phoneBookStruct() {
    	}
    	phoneBookStruct(const std::string &_name, const std::string &_forename, const std::string &_organisation, const std::string &_address, const std::string &_email, const uint32_t &_data, const phoneNumberMap &_phoneNumber)
    	{
    		std::get<0>(values_) = _name;
    		std::get<1>(values_) = _forename;
    		std::get<2>(values_) = _organisation;
    		std::get<3>(values_) = _address;
    		std::get<4>(values_) = _email;
    		std::get<5>(values_) = _data;
    		std::get<6>(values_) = _phoneNumber;
    	}
    	inline const std::string &getName() const { return std::get<0>(values_); }
    	inline void setName(const std::string &_value) { std::get<0>(values_) = _value; }
    	inline const std::string &getForename() const { return std::get<1>(values_); }
    	inline void setForename(const std::string &_value) { std::get<1>(values_) = _value; }
    	inline const std::string &getOrganisation() const { return std::get<2>(values_); }
    	inline void setOrganisation(const std::string &_value) { std::get<2>(values_) = _value; }
    	inline const std::string &getAddress() const { return std::get<3>(values_); }
    	inline void setAddress(const std::string &_value) { std::get<3>(values_) = _value; }
    	inline const std::string &getEmail() const { return std::get<4>(values_); }
    	inline void setEmail(const std::string &_value) { std::get<4>(values_) = _value; }
    	inline const uint32_t &getData() const { return std::get<5>(values_); }
    	inline void setData(const uint32_t &_value) { std::get<5>(values_) = _value; }
    	inline const phoneNumberMap &getPhoneNumber() const { return std::get<6>(values_); }
    	inline void setPhoneNumber(const phoneNumberMap &_value) { std::get<6>(values_) = _value; }
    	inline bool operator==(const phoneBookStruct& _other) const {
                return (getName() == _other.getName() && getForename() == _other.getForename() && getOrganisation() == _other.getOrganisation() && getAddress() == _other.getAddress() && getEmail() == _other.getEmail() && getData() == _other.getData() && getPhoneNumber() == _other.getPhoneNumber());
        }
    	inline bool operator!=(const phoneBookStruct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    /**
     * description: Data types for the filter operations
     */
    struct elementFilterStruct : CommonAPI::Struct<bool, bool, bool, bool, bool, bool> {
    	
    	elementFilterStruct() {
    		std::get<0>(values_) = false;
    		std::get<1>(values_) = false;
    		std::get<2>(values_) = false;
    		std::get<3>(values_) = false;
    		std::get<4>(values_) = false;
    		std::get<5>(values_) = false;
    	}
    	elementFilterStruct(const bool &_addName, const bool &_addForename, const bool &_addOrganisation, const bool &_addAddress, const bool &_addEmail, const bool &_addPhoneNumber)
    	{
    		std::get<0>(values_) = _addName;
    		std::get<1>(values_) = _addForename;
    		std::get<2>(values_) = _addOrganisation;
    		std::get<3>(values_) = _addAddress;
    		std::get<4>(values_) = _addEmail;
    		std::get<5>(values_) = _addPhoneNumber;
    	}
    	inline const bool &getAddName() const { return std::get<0>(values_); }
    	inline void setAddName(const bool _value) { std::get<0>(values_) = _value; }
    	inline const bool &getAddForename() const { return std::get<1>(values_); }
    	inline void setAddForename(const bool _value) { std::get<1>(values_) = _value; }
    	inline const bool &getAddOrganisation() const { return std::get<2>(values_); }
    	inline void setAddOrganisation(const bool _value) { std::get<2>(values_) = _value; }
    	inline const bool &getAddAddress() const { return std::get<3>(values_); }
    	inline void setAddAddress(const bool _value) { std::get<3>(values_) = _value; }
    	inline const bool &getAddEmail() const { return std::get<4>(values_); }
    	inline void setAddEmail(const bool _value) { std::get<4>(values_) = _value; }
    	inline const bool &getAddPhoneNumber() const { return std::get<5>(values_); }
    	inline void setAddPhoneNumber(const bool _value) { std::get<5>(values_) = _value; }
    	inline bool operator==(const elementFilterStruct& _other) const {
                return (getAddName() == _other.getAddName() && getAddForename() == _other.getAddForename() && getAddOrganisation() == _other.getAddOrganisation() && getAddAddress() == _other.getAddAddress() && getAddEmail() == _other.getAddEmail() && getAddPhoneNumber() == _other.getAddPhoneNumber());
        }
    	inline bool operator!=(const elementFilterStruct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct phoneBookDataElementEnum : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            NAME = 0,
            FORENAME = 1,
            ORGANISATION = 2,
            ADDRESS = 3,
            EMAIL = 4,
            PHONENUMBER = 5
        };
        
        phoneBookDataElementEnum() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::NAME)) {}
        phoneBookDataElementEnum(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::NAME):
                case static_cast<int32_t>(Literal::FORENAME):
                case static_cast<int32_t>(Literal::ORGANISATION):
                case static_cast<int32_t>(Literal::ADDRESS):
                case static_cast<int32_t>(Literal::EMAIL):
                case static_cast<int32_t>(Literal::PHONENUMBER):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const phoneBookDataElementEnum &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const phoneBookDataElementEnum &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const phoneBookDataElementEnum &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const phoneBookDataElementEnum &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const phoneBookDataElementEnum &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const phoneBookDataElementEnum &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    struct contentFilterStruct : CommonAPI::Struct<phoneBookDataElementEnum, std::string> {
    	
    	contentFilterStruct() {
    	}
    	contentFilterStruct(const phoneBookDataElementEnum &_element, const std::string &_expression)
    	{
    		std::get<0>(values_) = _element;
    		std::get<1>(values_) = _expression;
    	}
    	inline const phoneBookDataElementEnum &getElement() const { return std::get<0>(values_); }
    	inline void setElement(const phoneBookDataElementEnum &_value) { std::get<0>(values_) = _value; }
    	inline const std::string &getExpression() const { return std::get<1>(values_); }
    	inline void setExpression(const std::string &_value) { std::get<1>(values_) = _value; }
    	inline bool operator==(const contentFilterStruct& _other) const {
                return (getElement() == _other.getElement() && getExpression() == _other.getExpression());
        }
    	inline bool operator!=(const contentFilterStruct &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    static const CommonAPI::Serial PHONEBOOKDATAELEMENT_SERIAL = 0x1D47FC57;
    static const CommonAPI::Serial PHONEBOOKDATAELEMENTSTRING_SERIAL = 0xAF191104;
    static const CommonAPI::Serial PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL = 0x711BF94C;
    
    struct phoneBookDataElement : CommonAPI::PolymorphicStruct {
    	static std::shared_ptr<phoneBookDataElement> create(CommonAPI::Serial _serial);
    	CommonAPI::Serial getSerial() const { return PHONEBOOKDATAELEMENT_SERIAL; }
    	
    	phoneBookDataElement() {
    	}
    	template<class _Input>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const CommonAPI::EmptyDeployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template readValue<_Input>(_input, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template readValue<_Input>(_input, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    
    	template<class _Input, class _Deployment>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const _Deployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template readValue<>(_input, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template readValue<>(_input, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    	template<class _Output>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template writeType<_Output>(_output, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template writeType<_Output>(_output, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    	template<class _Output, class _Deployment>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const _Deployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template writeType<_Output, _Deployment>(_output, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template writeType<_Output, _Deployment>(_output, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    
    	template<class _Output>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template writeValue<_Output>(_output, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template writeValue<_Output>(_output, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    
    	template<class _Output, class _Deployment>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const _Deployment *_depl) {
    		switch (getSerial()) {
    		case PHONEBOOKDATAELEMENTSTRING_SERIAL:
    			static_cast<phoneBookDataElementString *>(this)->template writeValue<>(_output, _depl);
    			break;
    		case PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL:
    			static_cast<phoneBookDataElementPhoneNumber *>(this)->template writeValue<>(_output, _depl);
    			break;
    		default:
    			break;
    		}
    	}
    	
    	inline bool operator==(const phoneBookDataElement& _other) const {
                (void) _other;
                return true;
                    }
    	inline bool operator!=(const phoneBookDataElement &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    struct phoneBookDataElementString : phoneBookDataElement {
    	CommonAPI::Serial getSerial() const { return PHONEBOOKDATAELEMENTSTRING_SERIAL; }
    	
    	phoneBookDataElementString() {
    	}
    	phoneBookDataElementString(const std::string &_content)
    	: phoneBookDataElement()
    	{
    		std::get<0>(values_) = _content;
    	}
    	template<class _Input>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const CommonAPI::EmptyDeployment *_depl) {
    	    (void) _depl;
    		_input.template readValue<CommonAPI::EmptyDeployment>(std::get<0>(values_));
    	}
    
    	template<class _Input, class _Deployment>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const _Deployment *_depl) {
    		_input.template readValue<>(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    	template<class _Output>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    		_output.writeType(std::get<0>(values_), _depl);
    	}
    	template<class _Output, class _Deployment>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const _Deployment *_depl) {
    		_output.writeType(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    
    	template<class _Output>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    	    (void) _depl;
    		_output.template writeValue<CommonAPI::EmptyDeployment>(std::get<0>(values_));
    	}
    
    	template<class _Output, class _Deployment>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const _Deployment *_depl) {
    		_output.template writeValue<>(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    	inline const std::string &getContent() const { return std::get<0>(values_); }
    	inline void setContent(const std::string &_value) { std::get<0>(values_) = _value; }
    	
    	std::tuple<std::string> values_;
    	inline bool operator==(const phoneBookDataElementString& _other) const {
                return (getContent() == _other.getContent());
        }
    	inline bool operator!=(const phoneBookDataElementString &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    struct phoneBookDataElementPhoneNumber : phoneBookDataElement {
    	CommonAPI::Serial getSerial() const { return PHONEBOOKDATAELEMENTPHONENUMBER_SERIAL; }
    	
    	phoneBookDataElementPhoneNumber() {
    	}
    	phoneBookDataElementPhoneNumber(const phoneNumberMap &_content)
    	: phoneBookDataElement()
    	{
    		std::get<0>(values_) = _content;
    	}
    	template<class _Input>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const CommonAPI::EmptyDeployment *_depl) {
    	    (void) _depl;
    		_input.template readValue<CommonAPI::EmptyDeployment>(std::get<0>(values_));
    	}
    
    	template<class _Input, class _Deployment>
    	void readValue(CommonAPI::InputStream<_Input> &_input, const _Deployment *_depl) {
    		_input.template readValue<>(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    	template<class _Output>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    		_output.writeType(std::get<0>(values_), _depl);
    	}
    	template<class _Output, class _Deployment>
    	void writeType(CommonAPI::TypeOutputStream<_Output> &_output, const _Deployment *_depl) {
    		_output.writeType(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    
    	template<class _Output>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const CommonAPI::EmptyDeployment *_depl) {
    	    (void) _depl;
    		_output.template writeValue<CommonAPI::EmptyDeployment>(std::get<0>(values_));
    	}
    
    	template<class _Output, class _Deployment>
    	void writeValue(CommonAPI::OutputStream<_Output> &_output, const _Deployment *_depl) {
    		_output.template writeValue<>(std::get<0>(values_), std::get<0>(_depl->values_));
    	}
    	inline const phoneNumberMap &getContent() const { return std::get<0>(values_); }
    	inline void setContent(const phoneNumberMap &_value) { std::get<0>(values_) = _value; }
    	
    	std::tuple<phoneNumberMap> values_;
    	inline bool operator==(const phoneBookDataElementPhoneNumber& _other) const {
                return (getContent() == _other.getContent());
        }
    	inline bool operator!=(const phoneBookDataElementPhoneNumber &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    typedef std::unordered_map<::v1::commonapi::examples::E04PhoneBook::phoneBookDataElementEnum, std::shared_ptr<phoneBookDataElement>, CommonAPI::EnumHasher<::v1::commonapi::examples::E04PhoneBook::phoneBookDataElementEnum>> phoneBookDataElementMap;
};

const char* E04PhoneBook::getInterface() {
    return ("commonapi.examples.E04PhoneBook");
}

CommonAPI::Version E04PhoneBook::getInterfaceVersion() {
    return CommonAPI::Version(1, 0);
}

/**
 * description: Data types of the phone book itself
 */
/**
 * description: Data types for the filter operations
 */
/**
 * description: Data types for the result of the phone book filter
 */

} // namespace examples
} // namespace commonapi
} // namespace v1

namespace CommonAPI {
}


// Compatibility
namespace v1_0 = v1;

#endif // V1_COMMONAPI_EXAMPLES_E04_PHONE_BOOK_HPP_