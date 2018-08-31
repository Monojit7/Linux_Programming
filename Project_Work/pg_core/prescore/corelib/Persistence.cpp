#include <core/Persistence.h>

#ifndef PLATFORM_WIN32
#include <persistence_client_library.h>
#include <persistence_client_library_key.h>
#include <persistence_client_library_error_def.h>
#endif

//These values are default ones as they are not yet implemented by pcl
unsigned int ldbid	 = 1U;
unsigned int usernum = 1U;
unsigned int seatnum = 1U;

#define DEFAULT_BUFFER 5

int Persistence::Init(const char* appname)
{
#ifndef	PLATFORM_WIN32
	return pclInitLibrary(appname, PCL_SHUTDOWN_TYPE_NORMAL);
#else
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "Init TBD");
	return -1;
#endif
}

void Persistence::DeInit(void)
{
#ifndef	PLATFORM_WIN32
	pclDeinitLibrary();
#endif
}

int Persistence::ReadKeyValueSize(const char* key)
{
#ifndef	PLATFORM_WIN32
	return pclKeyGetSize(ldbid, key, usernum, seatnum);
#else
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "ReadKeyValueSize TBD");
	return -1;
#endif	
}

int Persistence::ReadKeyValue(const char* key, std::string& value)
{
#ifndef	PLATFORM_WIN32
	if (value.size() == 0)
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "!!Null buffer passed!!");
		return false;
	}
	else
	{
		return pclKeyReadData(ldbid, key, usernum, seatnum, (unsigned char*)value.c_str(), value.size());
	}
#else
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "ReadKeyValue TBD");
	return -1;
#endif
}

int Persistence::DeleteKeyValue(const char* key)
{
#ifndef	PLATFORM_WIN32
	return pclKeyDelete(ldbid, key, usernum, seatnum);
#else
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "DeleteKeyValue TBD");
	return -1;
#endif
}

int Persistence::WriteKeyValue(const char* key, std::string& value)
{
#ifndef	PLATFORM_WIN32
	return pclKeyWriteData(ldbid, key, usernum, seatnum, (unsigned char*)value.c_str(), value.size());
#else
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "WriteKeyValue TBD");
	return -1;
#endif
}

int Persistence::ReadEOLParams(std::string& value)
{
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "ReadEOLParams TBD");
	return false;
}

int Persistence::ReadBootCmdParams(std::string& value)
{
	LOG_WARNING(Log::e_LOG_CONTEXT_CORE, "ReadEOLParams TBD");
	return false;
}

