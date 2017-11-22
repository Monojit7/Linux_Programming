/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            Persistence.h
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
* @brief           Persistence abstraction 
*/

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include <core/PresCtrl.h>


class DLL_EXPORT Persistence
{
public:
	/*
	* This interface is used to initialize the handlers
	The client should invoke this only once if it wants to use the persistence capability
	@param appname application name, the name must be a unique name in the system
	@return positive value: success
		negative value - failure
	TBD : To update error codes
	*/
	static int Init(const char* appname);

	/*
	* This interface is used to close the handlers and release resources
	* This will also ensure that if the persistence used is in-cache mode , push the data
	* to the persistent memory
	* For further use re invoke Init
	* TBD : Look at invoking this within prescore once the power modes are integrated
	*/
	static void DeInit(void);

	/*
	* This interface is used to read the size of a value stored against a key
	@param key key name
	@return positive value: size in number of bytes
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int ReadKeyValueSize(const char* key);

	/*
	* This interface is used to read a value stored against a key
	@param key key name
	@param value container to which value of the key will be populated , 
	Pass a container with sufficient buffer	,for ex: std::string value(10,'\0')
	Without a default buffer , no data can be returned
	@return positive value: success , number of bytes read
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int ReadKeyValue(const char* key,std::string& value);

	/*
	* This interface is used to write/update a value against a key
	@param key key name
	@param value container holding value of the key
	@return positive value: success , number of bytes written
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int WriteKeyValue(const char* key, std::string& value);

	/*
	* This interface is used to delete a key and its value
	@param key key name
	@return positive value: success
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int DeleteKeyValue(const char* key);

	/*
	* This interface is used to read the EOL values set
	@param value container to which value of the key will be populated , 
	Pass a container with sufficient buffer ,for ex: std::string value(10,'\0')
	Without a default buffer , no data can be returned
	@return positive value: success , number of bytes read
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int ReadEOLParams(std::string& value);

	/*
	* This interface is used to read boot command parameters
	@param value container to which value of the key will be populated ,
	Pass a container with sufficient buffer ,for ex: std::string value(10,'\0')
	Without a default buffer , no data can be returned
	@return positive value: success , number of bytes read
	negative value - failure
	Refer persistence_client_library_error_def.h for the exact error details
	*/
	static int ReadBootCmdParams(std::string& value);
};

#endif // PERSISTENCE_H
