#include <config/ConfigParser.h>
#include <core.h>
#include <iostream>
#include <fstream>
#include <string.h>

ConfigParser::ConfigParser() :
	mConfig(Json::objectValue)
    ,confFile("")
{
}

ConfigParser::~ConfigParser()
{

}

Json::Value ConfigParser::GetConfigFileContent(void)
{
	return mConfig;
}

Json::Value ConfigParser::GetConfigFileKey(const char * key)
{
	Json::Value jsonValue = Json::nullValue;
	jsonValue = mConfig.get(key, Json::nullValue);
    LOG_INFO(LOG_CONTEXT_MEDIA, "ConfigParser::GetConfigFileKey");

	return jsonValue;
}

Json::Value ConfigParser::parseConfigFile(const char* configFile)
{
	Json::Reader mJsonReader;
	bool status = false;
	Json::Value root(Json::objectValue);

	std::ifstream configIF(configFile, std::ifstream::in);
	if (configIF.is_open())
	{
		status = mJsonReader.parse(configIF, root);
		if (!status)
		{
            LOG_INFO(LOG_CONTEXT_MEDIA, "ConfigParser::parseConfigFile - ERROR: Failed to parse the configuration file:%s", mJsonReader.getFormatedErrorMessages().c_str());
			return (Json::nullValue);
		}
		else
		{
			return root;
		}
	}
	else
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "ConfigParser::parseConfigFile: ERROR: failed to open %s\n", configFile);
		return (Json::nullValue);
	}

}

void ConfigParser::setConfigFile(const std::string& str)
{
	confFile = str;
    LOG_INFO(LOG_CONTEXT_MEDIA, "ConfigParser::setConfigFile- confFile %s\n", confFile.c_str());
	mConfig = parseConfigFile(confFile.c_str());
}
