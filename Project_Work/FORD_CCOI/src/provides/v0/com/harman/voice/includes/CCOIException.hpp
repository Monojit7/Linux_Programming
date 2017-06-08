#ifndef CCOIEXCEPTION_HPP
#define CCOIEXCEPTION_HPP

#include <exception>
#include <string>

using namespace std;

namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

class CCOIException : public exception {

	string msg_;
	public:
		// constructor
  		CCOIException (string const& msg) : msg_ (msg) { }
		
		// destructor
  		~CCOIException() noexcept override { 
		
		}
  		
		// returns the char string which has the explanation about the description of the exception occured in the execution flow
		char const* what() const noexcept override {
			
			return msg_.c_str();
		}
};

			} // namespace voice
		} // namespace harman
	} // namespace com
} // namespace v0

#endif	// CCOIEXCEPTION_HPP
