/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            ConfigParser.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the interfaces for parsing project specific json configuration file(mediapres.cfg)
*/

#ifndef _CONFIGPARSER_H
#define _CONFIGPARSER_H

#include<core.h>
#include <memory>
#include <string>
#include <jsoncpp/value.h>
#include <jsoncpp/reader.h>
#include <jsoncpp/writer.h>
#include <jsoncpp/json.h>
#include <jsoncpp/config.h>
#include <jsoncpp/forwards.h>
#include <jsoncpp/autolink.h>
#include <jsoncpp/features.h>

using namespace std;
using namespace Json;

class ConfigParser
{
public:

	virtual Json::Value GetConfigFileContent(void);
	virtual Json::Value GetConfigFileKey(const char * key);
	void setConfigFile(const std::string&);

	// Declare all gets for configs here

private:
	
	ConfigParser();
	virtual ~ConfigParser();
	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{

	}
	friend class FacadeHelper<ConfigParser>;

	//member functions
	Json::Value parseConfigFile(const char* configFile);

	//member variables 
	Json::Value mConfig;
	std::string confFile;
};
#endif // !defined(CONFIGPARSER)
