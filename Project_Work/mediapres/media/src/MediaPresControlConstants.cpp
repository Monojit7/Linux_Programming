#include <core.h>


const std::unordered_map<uint64_t, Request> RequestList = {
    { e_REQUEST_DEVICEINFO_TASK , { "DeviceInfoTask", e_GROUP_DEVICEINFO_GROUP,0u,0u }},
#if 1
    {e_REQUEST_SESSION_QUERY_TASK , { "SessionQuery" , e_GROUP_MEDIA_BROWSE,e_REQUEST_CANCEL_INTERMEDIATE_REQUESTS,0u } },
#else
    {e_REQUEST_SESSION_QUERY_TASK , { "SessionQuery" , e_GROUP_MEDIA_BROWSE,e_REQUEST_CANCEL_OLD_REQ,0u } },
#endif
    {e_REQUEST_SETWINDOW_TASK      ,{ "SetWindow"	 , e_GROUP_MEDIA_BROWSE,e_REQUEST_CANCEL_INTERMEDIATE_REQUESTS,0u } },
    {e_REQUEST_PLAY      ,{ "RequestPlay"	 , e_GROUP_MEDIA_PLAYBACK,e_REQUEST_CANCEL_OLD_REQ,0u } },
    { e_REQUEST_SKIP     ,{ "RequestSkip"	 , e_GROUP_MEDIA_PLAYBACK,e_REQUEST_CANCEL_OLD_REQ,0u } },
    { e_REQUEST_REMOTE_CD     ,{ "RequestRemoteCD"	 , e_GROUP_MEDIA_PLAYBACK,e_REQUEST_CANCEL_OLD_REQ,0u } },
};
