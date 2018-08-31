#include <detectionengine/DeviceManagerHelper.h>

using namespace std;

const char* DeviceManagerHelper::IAP1_PROTOCOL = "IAP1";
const char* DeviceManagerHelper::IAP2_PROTOCOL = "IAP2";
const char* DeviceManagerHelper::AVRCP_PROFILE = "AVRCP";
const char* DeviceManagerHelper::A2DP_PROFILE = "A2DP";

std::string DeviceManagerHelper::getIAPVersion(std::string protocol)
{
	if (!protocol.compare(DeviceManagerHelper::IAP1_PROTOCOL))
	{
		return protocol;
	}
	else if (!protocol.compare(DeviceManagerHelper::IAP2_PROTOCOL))
	{
		return protocol;
	}
	else
	{
		return "unknown";
	}
}