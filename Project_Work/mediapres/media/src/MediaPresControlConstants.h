#ifndef _MEDIA_PRES_CONTROL_CONSTANTS_
#define _MEDIA_PRES_CONTROL_CONSTANTS_

enum e_MediaRequestIds
{
	e_REQUEST_DEVICEINFO_TASK = 1,
	e_REQUEST_SESSION_QUERY_TASK = 2,
	e_REQUEST_SETWINDOW_TASK = 4,
	e_REQUEST_PLAY = 8,
	e_REQUEST_SKIP = 16,
    e_REQUEST_REMOTE_CD = 32

};


enum e_MediaRequestGroups
{
	e_GROUP_DEVICEINFO_GROUP=0u,
	e_GROUP_MEDIA_PLAYBACK,
	e_GROUP_MEDIA_BROWSE
};

extern const std::unordered_map<uint64_t, Request> RequestList;

#endif
