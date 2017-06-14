/*----------------------------------------------------------------------------------------------------------------------
 *
 * Project Harman Mid Systems - Pandora Service
 * (c) copyright 2015
 * Company Harman Internation
 * All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 *
 *-----------------------------------------------------------------------------------------------------------------------
 *
 * @file IPandoraListenerImpl.cpp
 * @ingroup PandoraService
 * @author Monojit Chatterjee
 *
 *
 *---------------------------------------------------------------------------------------------------------------------*/

#include "IPandoraListenerImpl.hpp"
#include "CPandoraMessageSenderImpl.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "IIpodNotificationImpl.hpp"
#include <fstream>
#include <CPandoraCmdInvoker.hpp>
#include "jsoncpp/json.h"
#include "TraceMacros.hpp"
extern CIap1MgrController* vCIap1MgrController;
pthread_mutex_t vWritemutex = PTHREAD_MUTEX_INITIALIZER;
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, sendAcknowledgement);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnGetStatus);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackInfoExtended);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnStationActive);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnStationInfo);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrackElapsed);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrackRating);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrackCompleted);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrack);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateStationActive);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateNotice);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, receivedAcknowledgement);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateStatus);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackAlbumArtSegment);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateAlbumArt);
//TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrack);
//TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoUpdateTrackCompleted);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnStationCount);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, handleTimerEvent);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, retryAlbumArt);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoGetBrandingImage);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, updateBrandImageSegments);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackInfo);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackArtist);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackTitle);
TRC_SCOPE_DEF(PandoraService, IPandoraListenerImpl, infoReturnTrackAlbum);
extern CPandoraProtocolMgr* vPandoraProtocolMgr;
extern CTimer *mTimerInstance;
extern pthread_mutex_t vSendDataWritemutex;
std::ofstream IPandoraListenerImpl::myImagefile;
int mCurrentSegmentIndex ;
int mCurrentTrackDuration ;
unsigned int mCurrentTrackToken ;
#define PANDORA_DEVICE_ANDROID  1
#define	PANDORA_SESSION_ANDROID	3
IPandoraListenerImpl::IPandoraListenerImpl():mAlbumArtRetryTimer(this)
{

}
/* --------------------------------------------------------------------------------------------------
 * timer trigger handler.
 * --------------------------------------------------------------------------------------------------*/
void IPandoraListenerImpl::handleTimerEvent (CTimer* pTimer)
{
	static char count = 0;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, handleTimerEvent);
	if (pTimer != 0)
	{
		if (pTimer == &mAlbumArtRetryTimer)
		{
			if(count <= 1)
			{
				retryAlbumArt();
				//DBG_MSG(("\n[IPandoraListenerImpl] mAlbumArtRetryTimer count=%d\n",count));
				count++;
			}
			else
			{
				mAlbumArtRetryTimer.stopTimer();
				//DBG_MSG(("\n[IPandoraListenerImpl] mAlbumArtRetryTimer.stopTimer count=%d\n",count));
				count = 0;
			}
		}

		else {} // Do nothing
	}
}
void IPandoraListenerImpl::sendAcknowledgement (const uint8_t* data, const uint32_t &dataSize)
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, sendAcknowledgement);

	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received sendAcknowledgement!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	//DBG_MSG(("[IPandoraListenerImpl]: Received sendAcknowledgement"));

	//CPandoraMessageSenderImpl::mSendInProgress = false;
	/// set sendprogress to false
	// pop the message from queue
	//CPandoraMessageSenderImpl::mPandoraRequestQueue.pop();
	//CPandoraMessage *msg = new CPandoraMessage;
	//CPandoraMessageSenderImpl::sendData();
	// no need to check sendprogress flag
	//put mutex while writing
	pthread_mutex_lock(&vWritemutex);
	if((IIpodNotificationImpl::mSessionId != -1))
	{

		//DBG_MSG(("[IPandoraListenerImpl]: Writing the Acknowledgement to ipod"));
		int size = static_cast<int>(dataSize);
		unsigned int ProtocolID = PROTOCOL_ID;
		//CTunerCmdInvoker::GetInstance()->printData((uint8_t*)data, (uint32_t)size, eWrite);
		if ( Iap1_Types::OK == vCIap1MgrController->requestIpodSendData(ProtocolID, (char*)data, size))
		{
			//DBG_MSG(("[IPandoraListenerImpl]: requestIpodSendData: Iap1_Types::OK"));
			//cout << "Sending successful to ios" << endl;
		}
	}
	else if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
	{
		//DBG_MSG(("[IPandoraListenerImpl]: Writing the Acknowledgement to Android"));
		write (CTunerCmdInvoker::mPandoraDeviceFd, data, dataSize);
	}

	pthread_mutex_unlock(&vWritemutex);
}
/*The PresCtrl can send the next PandoraMessage over transport*/
void IPandoraListenerImpl::receivedAcknowledgement ()
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, receivedAcknowledgement);
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received receivedAcknowledgement!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

	//DBG_MSG(("[ThreadContext]::Calling Stop Timer"));
	mTimerInstance->stopTimer();
	//DBG_MSG(("[IPandoraListenerImpl]: Received receivedAcknowledgement"));
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Ack_Received, 0);

}

void IPandoraListenerImpl::infoReturnGetStatus (pndr_return_status_params &val)
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnGetStatus!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnGetStatus);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnGetStatus!!!!!!!!!!!!!!!!!!!!!!!!!"));
	Json::Value data;
	std::string signalName = "InfoUpdateStatus";
	data["Pndr_Status_code"] = val.status;
	cout << "infoUpdateStatus:: Status is" << (int)val.status << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);
}

/*void IPandoraListenerImpl::infoGetBrandingImage()
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoGetBrandingImage);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoGetBrandingImage!!!!!!!!!!!!!!!!!!!!!!!!!"));
	ifstream file;
	file.open ("fs/etfs/usr/bin/brandYamahaCo2.png", ios::in | ios::binary | ios::ate);
	int fileSize;
	char* fileContents;
	if(file.is_open())
	{
	    fileSize = file.tellg();
	    fileContents = new char[fileSize];
	    file.seekg(0, ios::beg);
	    if(!file.read(fileContents, fileSize))
	    {
	    	//DBG_MSG(("[IPandoraListenerImpl]:fail to read"));
	    }
	    file.close();
	}
	//// calculate size of the structure pndr_return_branding_image_segment_params which is equal to sizeof (segment_index) + sizeof (total_segments) + imager bufLegth
	UInt32 bufLegth = 2*sizeof(UInt8) + fileSize;
    UInt32 i;
	const uint32_t length = bufLegth;
    const UInt8 *data = (const UInt8 *)fileContents;

	UInt8 buffer_1[bufLegth];
    pndr_return_branding_image_segment_params *pBrandingImageSegments_1 = (pndr_return_branding_image_segment_params *)buffer_1;
    pBrandingImageSegments_1->segment_index = 0;
    pBrandingImageSegments_1->total_segments = 1;

    for (i = 0; i < fileSize; i++) {
    	pBrandingImageSegments_1->bytes[i] = data[i];
    }
    //DBG_MSG(("[IPandoraListenerImpl]:fileSize:%d,bufLegth:%d,pBrandingImageSegments_1:%d,segment_index:%d,total_segments:%d,i:%d",fileSize,bufLegth,pBrandingImageSegments_1,pBrandingImageSegments_1->segment_index,pBrandingImageSegments_1->total_segments,i));
    const pndr_return_branding_image_segment_params *pBrandingImageSegmentsTemp_1 = pBrandingImageSegments_1;
	CPandoraMessage *msg_1 = new CPandoraMessage;
	msg_1->returnBrandingImage( pBrandingImageSegmentsTemp_1, length);
	CPandoraMessageSenderImpl::addSendQueue(msg_1);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
}*/

void IPandoraListenerImpl::infoGetBrandingImage()
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoGetBrandingImage);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoGetBrandingImage!!!!!!!!!!!!!!!!!!!!!!!!!"));
	ifstream file;
	file.open ("fs/etfs/usr/bin/brandYamahaCo2.png", ios::in | ios::binary | ios::ate);
	int fileSize;
	char* fileContents;
	if(file.is_open())
	{
	    fileSize = file.tellg();
	    fileContents = new char[fileSize];
	    file.seekg(0, ios::beg);
	    if(!file.read(fileContents, fileSize))
	    {
	    	//DBG_MSG(("[IPandoraListenerImpl]:fail to read"));
	    }
	    file.close();
	}
	//// calculate size of the structure pndr_return_branding_image_segment_params which is equal to sizeof (segment_index) + sizeof (total_segments) + imager bufLegth
	UInt32 bufLegth = 2*sizeof(UInt8) + fileSize;
    UInt32 i,j;
	const uint32_t length_1 = 1002;
    const UInt8 *data = (const UInt8 *)fileContents;

    //Total Brand Image Size = 1738Bytes. 1st Frame 1000Bytes+2Header Bytes = 1002Bytes//
	UInt8 buffer_1[length_1];
    pndr_return_branding_image_segment_params *pBrandingImageSegments_1 = (pndr_return_branding_image_segment_params *)buffer_1;
    pBrandingImageSegments_1->segment_index = 0;
    pBrandingImageSegments_1->total_segments = 2;
    for (i = 0; i < 1000; i++) {
    	pBrandingImageSegments_1->bytes[i] = data[i];
    }
    //DBG_MSG(("[IPandoraListenerImpl]:fileSize:%d,bufLegth:%d,pBrandingImageSegments_1:%d,segment_index:%d,total_segments:%d,i:%d",fileSize,bufLegth,pBrandingImageSegments_1,pBrandingImageSegments_1->segment_index,pBrandingImageSegments_1->total_segments,i));
    const pndr_return_branding_image_segment_params *pBrandingImageSegmentsTemp_1 = pBrandingImageSegments_1;
	CPandoraMessage *msg_1 = new CPandoraMessage;
	msg_1->returnBrandingImage( pBrandingImageSegmentsTemp_1, length_1);
	CPandoraMessageSenderImpl::addSendQueue(msg_1);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);

	//2nd Frame 738Bytes+2Header Bytes = 740Bytes//
	const uint32_t length_2 = 740;
	UInt8 buffer_2[length_2];
    pndr_return_branding_image_segment_params *pBrandingImageSegments_2 = (pndr_return_branding_image_segment_params *)buffer_2;
    pBrandingImageSegments_2->segment_index = 1;
    pBrandingImageSegments_2->total_segments = 2;
    for (i = 1000, j = 0; i < 1738; i++,j++) {
    		pBrandingImageSegments_2->bytes[j] = data[i];
    }
    //DBG_MSG(("[IPandoraListenerImpl]:fileSize:%d,bufLegth:%d,pBrandingImageSegments_2:%d,segment_index:%d,total_segments:%d,i:%d",fileSize,bufLegth,pBrandingImageSegments_2,pBrandingImageSegments_2->segment_index,pBrandingImageSegments_2->total_segments,i));
    const pndr_return_branding_image_segment_params *pBrandingImageSegmentsTemp_2 = pBrandingImageSegments_2;
	CPandoraMessage *msg_2 = new CPandoraMessage;
	msg_2->returnBrandingImage( pBrandingImageSegmentsTemp_2, length_2);
	CPandoraMessageSenderImpl::addSendQueue(msg_2);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
}

void IPandoraListenerImpl::retryAlbumArt()
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, retryAlbumArt);
	CPandoraMessage *msg = new CPandoraMessage;
	int AlbumArtLength;
	//int AlbumArtLength = 1024; //2048;
	//if(CTunerRequestResponseHandler::pandoraConnectedDeviceName == PANDORA_DEVICE_ANDROID)
	if(CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_ANDROID)
	{
		AlbumArtLength = 1024;
	}
	else
	{
		AlbumArtLength = 2048;
	}
	msg->getTrackAlbumArt(AlbumArtLength);
	//DBG_MSG(("[IPandoraListenerImpl]:Retrying Album Art command AlbumArtLength:%d",AlbumArtLength));
	CPandoraMessageSenderImpl::addSendQueue(msg);
	// Inform Pandora Worker thread to process the New Request
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
}
void IPandoraListenerImpl::infoReturnTrackAlbumArtSegment(pndr_return_track_album_art_segment_params &val)
{
	//static char segmentsCount = 0;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackAlbumArtSegment);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackAlbumArtSegment!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//DBG_MSG(("[CTunerCmdInvoker]:total_segments : %d, segment_index: %d, bytes_length: %d, track_token :%d, segmentsCount:%d", val.total_segments, val.segment_index, val.bytes_length, val.track_token,CTunerRequestResponseHandler::segmentsCount));
	//static char retryAlbumArt = 0;
	if( (val.segment_index == 0) && (IPandoraListenerImpl::myImagefile.is_open()) )
	{
		//DBG_MSG(("[infoReturnTrackAlbumArtSegment]: myImagefile.is_open so closing it"));
		IPandoraListenerImpl::myImagefile.close();
	}
	if(IPandoraListenerImpl::myImagefile.is_open())
	{

		//if( val.segment_index == mCurrentSegmentIndex )
		//{	retryAlbumArt = 1;
			//mAlbumArtRetryTimer.stopTimer();
			//DBG_MSG(("[CTunerCmdInvoker]:val.segment_index :%d byes length: %d", val.segment_index, val.bytes_length));
			mCurrentSegmentIndex++;
			CTunerRequestResponseHandler::segmentsCount++;
			for(int index = 0; index < val.bytes_length; index++)
			{
				IPandoraListenerImpl::myImagefile<<val.bytes[index];
			}
			//if ( mCurrentSegmentIndex == val.total_segments)
			if ( val.segment_index == val.total_segments-1)
			{
				IPandoraListenerImpl::myImagefile.close();
				if(CTunerRequestResponseHandler::segmentsCount < val.total_segments)
				{
					//DBG_MSG(("[infoReturnTrackAlbumArtSegment]:Retrying Album Art command segmentsCount:%d",CTunerRequestResponseHandler::segmentsCount));
					retryAlbumArt();
				}
				else if(CTunerRequestResponseHandler::segmentsCount > val.total_segments)
				{
					//DBG_MSG(("[infoReturnTrackAlbumArtSegment]:Retrying Album Art command segmentsCount:%d more than total_segments",CTunerRequestResponseHandler::segmentsCount));
					retryAlbumArt();
				}
				Json::Value data;
				std::string signalName = "infoReturnTrackAlbumArtSegment";
				data["ImagePath"] = "/tmp/example.jpeg";
				data["TrackToken"] = val.track_token;

				//DBG_MSG(("[CTunerCmdInvoker]:Sending Art Work signal to HMI segmentsCount:%d",CTunerRequestResponseHandler::segmentsCount));
				CTunerRequestResponseHandler::handleNotif(signalName, data);
				mCurrentSegmentIndex = 0;
				CTunerRequestResponseHandler::segmentsCount = 0;
			}
		//}
		//else
		//{
			////DBG_MSG(("[CTunerCmdInvoker] Album Art segments are missed: %d",mCurrentSegmentIndex));
			////////////
			/*if(retryAlbumArt == 1)
			{
				retryAlbumArt = 0;
				CPandoraMessage *msg = new CPandoraMessage;
				int AlbumArtLength = 2048;
				msg->getTrackAlbumArt(AlbumArtLength);
				//DBG_MSG(("[IPandoraListenerImpl]:Retrying Album Art command"));
				CPandoraMessageSenderImpl::addSendQueue(msg);
				// Inform Pandora Worker thread to process the New Request
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
			}*/
			////////////
			//mAlbumArtRetryTimer.startTimer (3000, true);
		//}
	}
	else
	{       //retryAlbumArt = 1;
		//mAlbumArtRetryTimer.stopTimer();
		//DBG_MSG(("[CTunerCmdInvoker]:File is not opened"));
		mCurrentSegmentIndex = 0;
		CTunerRequestResponseHandler::segmentsCount = 0;
		 //IPandoraListenerImpl::myImagefile.open("/tmp/example.jpeg", ios::trunc);
		 //IPandoraListenerImpl::myImagefile.close();
         //IPandoraListenerImpl::myImagefile.open("/tmp/example.jpeg", ios::in | ios::out | ios::app);
		IPandoraListenerImpl::myImagefile.open("/tmp/example.jpeg", ios::in | ios::out | ios::trunc);

		for(int index = 0; index < val.bytes_length; index++)
		{
			IPandoraListenerImpl::myImagefile<<val.bytes[index];
		}
		++mCurrentSegmentIndex;
		CTunerRequestResponseHandler::segmentsCount++;
	}


}
void IPandoraListenerImpl::infoReturnTrackInfoExtended(pndr_return_track_info_extended_params &val)
{
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackInfoExtended!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackInfoExtended);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackInfoExtended!!!!!!!!!!!!!!!!!!!!!!!!!"));
	mCurrentTrackDuration = 0;
	mCurrentTrackToken = 0;
	Json::Value data;
	std::string signalName = "InformationTrackInfoExtended";
	data["TrackToken"] = val.track_token;
	data["AlbumArtLength"] = val.album_art_length;
	data["duration"] = val.duration;
	data["Elapsed time"] = val.elapsed;
	data["RatingType"] = val.rating;
	data["PermissionFlag"] = val.permission_flags;
	data["IdentityFlag"] = val.identity_flags;
	data["TrackTitle"] = val.track_title;
	data["ArtistName"] = val.artist_name;
	data["AlbumName"] = val.album_name;
	mCurrentTrackDuration = val.duration;
	mCurrentTrackToken = val.track_token;
	if(((val.permission_flags) & 0x01))
	{
		data["allow_rating_flag"] = true;
	}
	else
	{
		data["allow_rating_flag"] = false;
	}
	if(((val.permission_flags) & 0x02))
	{
		data["allow_skip_flag"] = true;
	}
	else
	{
		data["allow_skip_flag"] = false;
	}
	if(((val.permission_flags) & 0x04))
	{
		data["allow_bookmark_flag"] = true;
	}
	else
	{
		data["allow_bookmark_flag"] = false;
	}
	if(((val.identity_flags) & 0x01))
	{
		data["advertisement_flag"] = true;
	}
	else
	{
		data["advertisement_flag"] = false;
	}
	if(((val.identity_flags) & 0x02))
	{
		data["track_bookmark_flag"] = true;
	}
	else
	{
		data["track_bookmark_flag"] = false;
	}
	if(((val.identity_flags) & 0x04))
	{
		data["artist_bookmark_flag"] = true;
	}
	else
	{
		data["artist_bookmark_flag"] = false;
	}
	if(val.album_art_length > 0)
	{
		CPandoraMessage *msg = new CPandoraMessage;
		int AlbumArtLength = 2048;
		msg->getTrackAlbumArt(AlbumArtLength);
		//DBG_MSG(("[IPandoraListenerImpl]:Sending Album Art command"));
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	}
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [TrackToken]: %d",val.track_token));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [AlbumArtLength]: %d",val.album_art_length));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [duration]: %d",val.duration));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [elapsed]: %d",val.elapsed));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [rating]: %d",val.rating));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [permission_flags]: %d",val.permission_flags));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [identity_flags]: %d",val.identity_flags));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [track_title]: %s",val.track_title));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [artist_name]: %s",val.artist_name));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfoExtended: [AlbumName]: %s",val.album_name));

	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoReturnTrackInfo (pndr_return_track_info_params &val )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackInfo);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackInfo!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackInfo!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [TrackToken]: %d",val.track_token));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [AlbumArtLength]: %d",val.album_art_length));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [duration]: %d",val.duration));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [elapsed]: %d",val.elapsed));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [rating]: %d",val.rating));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [permission_flags]: %d",val.permission_flags));
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnTrackInfo: [identity_flags]: %d",val.identity_flags));
}
void IPandoraListenerImpl::infoReturnTrackArtist (pndr_return_track_artist_params & )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackArtist);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackArtist!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackArtist!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoReturnTrackTitle (pndr_return_track_title_params & )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackTitle);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackTitle!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackTitle!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoReturnTrackAlbum (pndr_return_track_album_params & )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnTrackAlbum);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackAlbum!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnTrackAlbum!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoReturnTrackExplain (pndr_return_track_explain_segment_params&)
{

}
void IPandoraListenerImpl::infoReturnStationActive (pndr_return_station_active_params &val )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnStationActive);
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationActive!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationActive!!!!!!!!!!!!!!!!!!!!!!!!!"));
	Json::Value data;
	std::string signalName = "InfoUpdateStationActive";
	data["StationToken"] = val.station_token;
	data["value"] = true;
//	CTunerCmdInvoker::mStationActive = true;
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnStationActive:StationToken: %d", val.station_token));
	cout << "infoReturnStationActive:[station_token]" << val.station_token << endl;

	CTunerRequestResponseHandler::handleNotif(signalName, data);
	if(!val.station_token)
	{
        CTunerCmdInvoker::currentStationIsPlaying = false;
 		//DBG_MSG(("[IPandoraListenerImpl]:Pandora_Get_AllStationTokens"));
		CPandoraMessage *msg = new CPandoraMessage;
		msg->getAllStationTokens();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	}
	else
	{
		//DBG_MSG(("[IPandoraListenerImpl]:Pandora_eventStationSelect station_token:%d",val.station_token));
		/*CPandoraMessage *msgStationSelect = new CPandoraMessage;
		msgStationSelect->eventStationSelect(val.station_token);
		CPandoraMessageSenderImpl::addSendQueue(msgStationSelect);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);*/
	}
	/*else
	{
		//DBG_MSG(("[IPandoraListenerImpl]:Pandora_getTrackInfo"));
		CPandoraMessage *msg = new CPandoraMessage;
		msg->getTrackInfo();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	}*/
}

void IPandoraListenerImpl::infoReturnStationCount (pndr_return_station_count_params &val )
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationCount!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnStationCount);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationCount!!!!!!!!!!!!!!!!!!!!!!!!!"));
	Json::Value data;
	std::string signalName = "InfoUpdateStationCount";
	data["StationCount"] = val.count;
	//DBG_MSG(("[IPandoraListenerImpl]:infoReturnStationCount:StationCount: %d", val.count));
	//cout << "infoUpdateStatus:: StationCount" << val.count << endl;

	CTunerRequestResponseHandler::handleNotif(signalName, data);

}
void IPandoraListenerImpl::infoReturnStationTokens(pndr_return_station_tokens_params &val)
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationCount!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnStationCount);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationTokens!!!!!!!!!!!!!!!!!!!!!!!!!"));
	pndr_get_station_info_params vStationInfo;
	Json::Value vStationTokensArray(Json::arrayValue);
	Json::Value data(Json::objectValue);
	Json::Value vObjStationTokens(Json::objectValue);
	std::vector<pndr_token>::iterator tVectortokens;
	std::string signalName = "infoReturnStationTokens";
	int vStationTokenNo = 0;
	for(tVectortokens = val.station_tokens.begin(); tVectortokens != val.station_tokens.end(); tVectortokens++)
	{
		vStationInfo.station_tokens.push_back(*tVectortokens);
		data["station_tokens"] = *tVectortokens;
		//DBG_MSG(("IPandoraListenerImpl::infoReturnStationTokens: station_tokens %u",*tVectortokens));
		if(CTunerCmdInvoker::currentStationIsPlaying == false)
		{
			UINT32 station_token = *tVectortokens;
			//DBG_MSG(("[IPandoraListenerImpl]:Pandora Station Token:%u, currentStationIsPlaying:%d", station_token,CTunerCmdInvoker::currentStationIsPlaying));
			CTunerCmdInvoker::currentStationIsPlaying = true;
			if(CTunerRequestResponseHandler::mPandoraNoActiveStation == true)
			{
				if(station_token)
				{
					CPandoraMessage *msg = new CPandoraMessage;
					msg->eventStationSelect(station_token);
					CPandoraMessageSenderImpl::addSendQueue(msg);
					// Inform Pandora Worker thread to process the New Request
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				}
			}
		}
		vStationTokensArray[vStationTokenNo] = data;
		++vStationTokenNo;
	}
	//data["StationCount"] = vStationTokenNo;
	//DBG_MSG(("IPandoraListenerImpl::infoReturnStationTokens: total Stations %d",vStationTokenNo));
	if((vStationTokenNo > 0))
	{

		vObjStationTokens["StationToken"] = vStationTokensArray;
		vObjStationTokens["StationCount"] = vStationTokenNo;
		CTunerRequestResponseHandler::handleNotif(signalName,vObjStationTokens);
	}



	CPandoraMessage *msg = new CPandoraMessage;


	msg->getStationInfo(vStationInfo );
	if(msg == NULL)
	{
		// cout << "Message is NULL in StartSession" << endl;
	}
	//CTunerCmdInvoker::GetInstance()->printData(msg->getData(), msg->getDataSize(), eWrite);
	//vPandoraProtocolMgr->prepareMessageToSend(msg);
	//ssize_t vNumBytes = write(CTunerCmdInvoker::mPandoraDeviceFd, msg->getData(), msg->getDataSize());
	// cout << "Size of the Written bytes " << vNumBytes << endl;
	// CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
	CPandoraMessageSenderImpl::addSendQueue(msg);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	//CPandoraMessageSenderImpl::sendData();

	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoReturnStationInfo (pndr_return_station_info_params &val )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoReturnStationInfo);
	//DBG_MSG(("[IPandoraListenerImpl]: Received infoReturnStationInf"));
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoReturnStationInfo!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

	Json::Value vStationArray(Json::arrayValue);
	Json::Value data(Json::objectValue);
	Json::Value vObjStationInfo(Json::objectValue);
	std::string signalName = "InfoStationList";
	std::vector<pndr_station_info>::iterator tVectorStationList;
	//tVectorStationList vStationList;
	int vStationTokenNo = 0;
	for(tVectorStationList = val.station_list.begin(); tVectorStationList != val.station_list.end(); tVectorStationList++)
	{
		data["flags"] = tVectorStationList->flags;
		if( ((tVectorStationList->flags) & 0x2))
		{
			data["shared_flag"] = true;
		}
		else
		{
			data["shared_flag"] = false;
		}
		data["station_token"] = tVectorStationList->station_token;
		data["name"] = tVectorStationList->name;
		if(tVectorStationList->station_token ==  CTunerCmdInvoker::mStationActiveToken)
		{
			std::string mCurrentStationsignal = "InfoCurrentStation";
			Json::Value data;
			//DBG_MSG(("[IPandoraListenerImpl]: CurrentStationInformation : flags %d", tVectorStationList->flags));
			//DBG_MSG(("[IPandoraListenerImpl]: CurrentStationInformation : station_token %u", tVectorStationList->station_token));
			//DBG_MSG(("[IPandoraListenerImpl]: CurrentStationInformation : name %s", tVectorStationList->name));
			data["station_name"] = tVectorStationList->name;
			data["station_token"] = tVectorStationList->station_token;

			if( ((tVectorStationList->flags) & 0x2))
			{
				data["shared_flag"] = true;
			}
			else
			{
				data["shared_flag"] = false;
			}
			CTunerRequestResponseHandler::handleNotif(mCurrentStationsignal,data);
		}
		//DBG_MSG(("[IPandoraListenerImpl]: infoReturnStationInfo : flags %d", tVectorStationList->flags));
		//DBG_MSG(("[IPandoraListenerImpl]: infoReturnStationInfo : station_token %u", tVectorStationList->station_token));
		//DBG_MSG(("[IPandoraListenerImpl]: infoReturnStationInfo : name %s", tVectorStationList->name));
		vStationArray[vStationTokenNo] = data;
		++vStationTokenNo;
	}


	if((vStationTokenNo > 1))
	{

		vObjStationInfo["StationInfo"] = vStationArray;
		CTunerRequestResponseHandler::handleNotif(signalName,vObjStationInfo);
	}

}
void IPandoraListenerImpl::infoReturnStationsOrder (pndr_return_stations_order_params & )
{

}
void IPandoraListenerImpl::infoReturnStationArtSegment (pndr_return_station_art_segment_params & )
{

}
void IPandoraListenerImpl::infoReturnGenreCategoryCount(pndr_return_genre_category_count_params & )
{

}
void IPandoraListenerImpl::infoReturnGenreCategoryNames(pndr_return_genre_category_names_params & )
{

}
void IPandoraListenerImpl::infoReturnGenreCategoryStationCount(pndr_return_genre_category_station_count_params & )
{

}
void IPandoraListenerImpl::infoReturnGenreStationNames(pndr_return_genre_station_names_params & )
{

}
void IPandoraListenerImpl::infoReturnGenreStationArtSegment (pndr_return_genre_station_art_segment_params & )
{

}
void IPandoraListenerImpl::infoReturnSearchResultInfo (pndr_return_search_result_info_params &)
{

}
void IPandoraListenerImpl::infoReturnListener (pndr_return_listener_params & val)
{

}

void IPandoraListenerImpl::infoUpdateTrackAlbumArt (pndr_update_track_album_art_params & val)
{
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackAlbumArt!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateAlbumArt);
	//DBG_MSG(("[IPandoraListenerImpl]:Received infoUpdateTrackAlbumArt"));
	Json::Value data;
	std::string signalName = "infoUpdateTrackAlbumArt";
	data["TrackToken"] = val.track_token;
	data["image_length"] = val.image_length;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackElapsed: track_token: %d",val.track_token));
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackElapsed: image_length: %d",val.image_length));

	CPandoraMessage *msg = new CPandoraMessage;
	int AlbumArtLength;
	//int AlbumArtLength = 1024; //2048;
	//if(CTunerRequestResponseHandler::pandoraConnectedDeviceName == PANDORA_DEVICE_ANDROID)
	if(CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_ANDROID)
	{
		AlbumArtLength = 1024;
	}
	else
	{
		AlbumArtLength = 2048;
	}
	msg->getTrackAlbumArt(AlbumArtLength);
	//DBG_MSG(("[IPandoraListenerImpl]:Sending Album Art command length:%d",AlbumArtLength));
	CPandoraMessageSenderImpl::addSendQueue(msg);
	// Inform Pandora Worker thread to process the New Request
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);

	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoUpdateTrackElapsed (pndr_update_track_elapsed_params & val)
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackElapsed!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateTrackElapsed);
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackElapsed"));
	Json::Value data;
	std::string signalName = "InfoUpdateTrackElaped";
	data["TrackToken"] = val.track_token;
	data["Elapsed"] = val.elapsed;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackElapsed: Track token: %d",val.track_token));
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackElapsed: Elapsed: %d",val.elapsed));
	//cout << "infoUpdateStatus:: Track token  is" << val.track_token << endl;
	//cout << "infoUpdateStatus:: Rating Type  is" << val.elapsed << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoUpdateTrackRating (pndr_update_track_rating_params & val)
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateTrackRating);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackCompleted!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackRating!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	Json::Value data;
	std::string signalName = "InfoUpdateTrackRating";
	data["TrackToken"] = val.track_token;
	data["RatingType"] = val.rating;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackRating: Track token: %d",val.track_token));
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackRating: rating: %d",val.rating));
	//cout << "infoUpdateStatus:: Track token  is" << val.track_token << endl;
	//cout << "infoUpdateStatus:: Rating Type  is" << val.rating << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoUpdateTrackExplain (pndr_update_track_explain_params& val)
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackExplain!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}

void IPandoraListenerImpl::infoUpdateTrackBookmarkArtist (pndr_update_track_bookmark_artist_params &val )
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackBookmarkArtist!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoUpdateTrackBookmarkTrack (pndr_update_track_bookmark_track_params &val )
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackBookmarkTrack!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoUpdateTrackCompleted (pndr_update_track_completed_params &val)
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackCompleted!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateTrackCompleted);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrackCompleted!!!!!!!!!!!!!!!!!!!!!!!!!"));
    ///////Workaround/////////
	Json::Value data2;
	std::string signalName2 = "InfoUpdateTrackElaped";
	data2["TrackToken"] = mCurrentTrackToken;
	data2["Elapsed"] = mCurrentTrackDuration;
	CTunerRequestResponseHandler::handleNotif(signalName2, data2);
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackCompleted: Elapsed: %d, Track token: %d",mCurrentTrackDuration,mCurrentTrackToken));
    /////////////////////////
	Json::Value data;
	std::string signalName = "InfoUpdateTrackCompleted";
	data["TrackToken"] = val.track_token;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrackCompleted: Track token: %d",val.track_token));
	//cout << "infoUpdateStatus:: Status is" << val.track_token << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);

	Json::Value data1;
	std::string signalName1 = "InfoUpdateStatus";
	data1["Pndr_Status_code"] = 1;
	CTunerRequestResponseHandler::handleNotif(signalName1, data1);
}
void IPandoraListenerImpl::infoUpdateTrack (pndr_update_track_params &val )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateTrack);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrack!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateTrack!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	Json::Value data;
	std::string signalName = "InfoUpdateTrack";
	data["TrackToken"] = val.track_token;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateTrack: Track Token %d", val.track_token));
	//cout << "infoUpdateStatus:: Status is" << val.track_token << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);

	if(CTunerRequestResponseHandler::mPndrReqPlayTrack == true)
	{
		CTunerRequestResponseHandler::mPndrReqPlayTrack = false;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventPlayTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		//DBG_MSG(("[IPandoraListenerImpl]:eventPlayTrack()"));
	}
}
void IPandoraListenerImpl::infoUpdateStationsOrder (pndr_update_stations_order_params &val )
{

}
void IPandoraListenerImpl::infoUpdateStationDeleted (pndr_update_station_deleted_params &val )
{

}
void IPandoraListenerImpl::infoUpdateStationAdded (pndr_update_station_added_params &val )
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateStationAdded!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

}
void IPandoraListenerImpl::infoUpdateStationActive(pndr_update_station_active_params &val )
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateStationActive);
	//DBG_MSG(("[CTunerCmdInvoker]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateStationActive!!!!!!!!!!!!!!!!!!!!!!!!!"));
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateStationActive!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	Json::Value data;
	std::string signalName = "InfoUpdateStationActive";
	data["StationToken"] = val.station_token;
	data["value"] = true;
	//CTunerCmdInvoker::mStationActive = true;
	CTunerCmdInvoker::mStationActiveToken = val.station_token;
	//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateStationActive: Station Token : %d",val.station_token));
	pndr_get_station_info_params vStationInfo;
	vStationInfo.station_tokens.push_back(val.station_token);
	CPandoraMessage *msg = new CPandoraMessage;
	msg->getStationInfo(vStationInfo );
	CPandoraMessageSenderImpl::addSendQueue(msg);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	// CPandoraMessageSenderImpl::sendData();



	//cout << "[InfoUpdateStationActive]:: station_token " << val.station_token << endl;
	CTunerRequestResponseHandler::handleNotif(signalName, data);
}
void IPandoraListenerImpl::infoUpdateSearch (pndr_update_search_params &val )
{
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateSearch!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
}
void IPandoraListenerImpl::infoUpdateNotice(pndr_update_notice_params &val)
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateNotice);
	//DBG_MSG(("[IPandoraListenerImpl]:!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateNotice!!!!!!!!!!!!!!!!!!!!!!!!! "));

	if(val.code == PNDR_NOTICE_ERROR_SESSION_ALREADY_STARTED)
	{
		//DBG_MSG(("[IPandoraListenerImpl]:Session has already Started Notice code : %d",val.code));
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventPlayTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
	}
	else
	{
		Json::Value data;
		std::string signalName = "InfoUpdateNotice";
		data["pndr_notice_code"] = val.code;
		//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateNotice: Notice code : %d",val.code));
		CTunerRequestResponseHandler::handleNotif(signalName, data);
	}
}
void IPandoraListenerImpl::infoUpdateStatus (pndr_update_status_params &val)
{
	TRC_SCOPE(PandoraService, IPandoraListenerImpl, infoUpdateStatus);
	DBG_MSG(("[IPandoraListenerImpl]:!! Received infoUpdateStatus!! val.status:%d",val.status));
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Received infoUpdateStatus!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;
	static char insuficientConnectivityFlag = 0;
	static unsigned int InfoUpdateTrackToken = 0;
	if( (CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus == true) && (val.status == PNDR_STATUS_PAUSED) && (CTunerRequestResponseHandler::mIpodDeviceConnected == true) )
	{
		//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateNotice: Status code:%d, mPandoraAvoidIpodPauseStatus:%d",val.status,CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus));
		CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus = false;
	}
	else
	{
		Json::Value data1;
		std::string signalName1 = "InfoUpdateStatus";
		data1["Pndr_Status_code"] = val.status;
		CTunerRequestResponseHandler::handleNotif(signalName1, data1);
		//DBG_MSG(("[IPandoraListenerImpl]:infoUpdateStatus: Status code : %d",val.status));
	}
	if(insuficientConnectivityFlag)
	{
		insuficientConnectivityFlag = 0;
		if(InfoUpdateTrackToken == mCurrentTrackToken)
		{
			Json::Value data;
			std::string signalName = "InfoUpdateTrack";
			data["TrackToken"] = InfoUpdateTrackToken;
			DBG_MSG(("[IPandoraListenerImpl]:infoUpdateStatus: InfoUpdateTrackToken:%d,mCurrentTrackToken:%d", InfoUpdateTrackToken,mCurrentTrackToken));
			CTunerRequestResponseHandler::handleNotif(signalName, data);
		}
	}

	if(val.status == PNDR_STATUS_NO_STATIONS){
		CTunerRequestResponseHandler::mStationTokenLists = false;
	}
	else if(val.status == PNDR_STATUS_INVALID_LOGIN)
	{
		CTunerRequestResponseHandler::mPndrStatusInvalidLogin = true;
	}
	else if(val.status == PNDR_STATUS_INSUFFICIENT_CONNECTIVITY)
	{
		insuficientConnectivityFlag = 1;
		InfoUpdateTrackToken = mCurrentTrackToken;
		//DBG_MSG(("[IPandoraListenerImpl]:PNDR_STATUS_INSUFFICIENT_CONNECTIVITY "));
	}
	else
	{
		CTunerRequestResponseHandler::mPndrStatusInvalidLogin = false;
		CTunerRequestResponseHandler::mStationTokenLists = true;

		if(CTunerRequestResponseHandler::mPandoraSessionStartupFunctionCalls == true)
		{
			if(val.status == PNDR_STATUS_NO_STATION_ACTIVE){
				CTunerRequestResponseHandler::mPandoraNoActiveStation = true;
			}
			CTunerRequestResponseHandler::mPandoraSessionStartupFunctionCalls = false;
			CPandoraMessage *msgElp = new CPandoraMessage;
			msgElp->setTrackElapsedPolling(1);
			CPandoraMessageSenderImpl::addSendQueue(msgElp);
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);

			CPandoraMessage *msgAS = new CPandoraMessage;
			msgAS->getStationActive();
			CPandoraMessageSenderImpl::addSendQueue(msgAS);
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
			//DBG_MSG(("[IPandoraListenerImpl]:Method updateBrandingImage invoked "));
				//unsigned int BufLegth = 8192;//Should be multiple of 1K bytes, Max 8K can be sent in a msg
			unsigned int BufLegth = 1738;//For android Phone need to tell the exact image size :(
			CPandoraMessage *msgBrImg = new CPandoraMessage;
			msgBrImg->updateBrandingImage(BufLegth);
			CPandoraMessageSenderImpl::addSendQueue(msgBrImg);
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);

			/*if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
			{
				if(val.status == PNDR_STATUS_PAUSED)
				{
					CPandoraMessage *msg = new CPandoraMessage;
					msg->eventPlayTrack();
					CPandoraMessageSenderImpl::addSendQueue(msg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					//DBG_MSG(("[IPandoraListenerImpl]:eventPlayTrack Status code : %d",val.status));
				}
			}*/
		}
	}

	/*if(val.status == PNDR_STATUS_INVALID_LOGIN){
		CTunerRequestResponseHandler::mPndrStatusInvalidLogin = true;
	}
	else{
		CTunerRequestResponseHandler::mPndrStatusInvalidLogin = false;
	}*/
	//CTunerCmdInvoker::mStationActive = true;

	/*Json::Value data;
	std::string signalName = "InfoUpdateStationActive";
	data["StationToken"] = 4;
	data["value"] = true;
	CTunerRequestResponseHandler::handleNotif(signalName, data);*/

}
