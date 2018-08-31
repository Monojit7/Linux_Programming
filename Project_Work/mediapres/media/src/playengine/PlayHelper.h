/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PlayHelper.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements some helper methods for playback functionality. Basically does the mapping between data from coc media to media pres..
*/

#ifndef _PLAY_HELPER_H_
#define _PLAY_HELPER_H_

#include <core.h>
#include <unordered_map>

using namespace v3::com::harman::mmpres;
using namespace v1::com::harman::media;

class PlayHelper
{
public:
	static MMPresTypes::eMetadataType getNowPlayingItemType(const std::string&);
	static bool getMetadataType(const MMPresTypes::eMetadataType&,BaseType::tAudioObjectType&);
	static double getPlaybackSpeed(const MMPresTypes::ePlaybackSpeed);
	static bool isPlaybackSpeedSupported(const MMPresTypes::eMetadataType);
private:
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mNowPlayingItemTypeMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, BaseType::tAudioObjectType> mMetadataMapping;
	static std::unordered_map<MMPresTypes::ePlaybackSpeed, double> mPlaybackSpeedMapping;
};

#endif