/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef REMOTECDPLAYBACKENGINE_H
#define REMOTECDPLAYBACKENGINE_H
#include <core.h>
#include <MediaPresCore.h>
#include <core/PresCtrl.h>

#define REMOTE_CD_DEVICE_ID 65537
#define REMOTE_CD_SLOT_ID 4

class RemoteCDPlaybackEngine
{
public:
    RemoteCDPlaybackEngine();
    ~RemoteCDPlaybackEngine();
    void setRepeatRemoteCD(int RepeatMode);
    void setShuffleRemoteCD(int ShuffleMode);
    void resumeRemoteCD();
    void pauseRemoteCD();
    void stopRemoteCD();
    void nextRemoteCD();
    void previousRemoteCD();
    void fastForwardRemoteCD();
    void fastReverseRemoteCD();
    void playDeviceRemoteCD();
    void setPlayItem(eCDPlayItemType mCDPlayItemType, uint32_t itemId);
    void setCDStatusEvent(std::function<void(::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus)>);
    void setCDErrorEvent(std::function<void(::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus)>);
    void setCDEjectChimesWarningEvent(std::function<void(bool)>);
    void setEOLStatusEvent ( std::function<void(::v3::com::harman::mmpres::MMPresTypes::eCDEOLStatus)> );
    
    std::function<void(::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus)>& getCDStatusEventCallback();
    std::function<void(::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus)>& getCDErrorEventCallback();
    std::function<void(bool)>& getCDEjectChimesWarningEventCallback();
    
    std::function < void (::v3::com::harman::mmpres::MMPresTypes::eCDEOLStatus)>& getEOLStatus () ;
    
    eCDStatus getCDSttaus();
     MMPresTypes::eMetadataType getCDType();
    CDError getCDError();
    MMPresTypes::tTrackState mTrackStateRemoteCD;
    void setActiveFolderNumber(const short activeFolderNumber) { mActiveFolderNumber = activeFolderNumber; }
    short getActiveFolderNumber() { return mActiveFolderNumber ; }
    int getActiveFolderTotalItem () { return { mFolderCount + mFileCount } ;  }
    short mFolderCount;
    short mFileCount ;
    void setParentFolderNumber ( short mParentFolderNum ) { this->mParentFolderNum = mParentFolderNum ; }
    short getParentFolderNumber () { return this->mParentFolderNum ; }
    void     setCurrentFolder ( short CurrentFolder ) { mCurrentFolder = CurrentFolder; } 
    short    getCurrentFolder (  ) {  return mCurrentFolder ; }
    IRemoteCDPlugin* getPluginInstance();
    void CDStatusEvent(eCDStatus mCDStatus);
    void CDErrorEvent(CDError mCDError);
    void CDTypeEvent (eCDType mCDType);
    void RemoteCDNumofTracksStatus(uint64_t  numofTracks);
    void RemoteCDActiveTrack( uint64_t  mTrackIndex );
    void RemoteCDEjectChimesWarning(bool warningstatus);
    void RemoteCDActiveFolderStatus ( CDActiveFolder mCDActiveFolder);
    void RemoteCDNowPlayingInfo(CDNowPlaying mCDNowPlaying);
    void initPluginEvent();
    short mActiveFolderNumber = 0 ;
    MMPresTypes::tNowPlayingInfo mNowPlayingInfo_RemoteCDPlayEngine ;

     void dummyNowPlaying (); 
     void publishNowPlayingInfoIfNeeded(uint64_t);
     uint16_t mTrackIndex = 0  ;
     
     void  insertFolderName ( int FolderNum , string Name )   { mFolderNumNameMap [ FolderNum ] = Name ;   }
     
     string getFolderName (  int FolderNum ) {  (( mFolderNumNameMap .find ( FolderNum ) != mFolderNumNameMap.end() ) ? mFolderNumNameMap [ FolderNum ] : "/"); }
     
     void    ClearAllNames ( )                   {     mFolderNumNameMap.clear();   }
     
     void    ClearAllFolderItem ()              {    mFolderItemCount.clear();                                }
     
     void    insertFolderItemCount ( int FolderNum,   int TotalCount ) {  mFolderItemCount [ FolderNum ] =   TotalCount;          }
     
     int     getFolderItemCount ( int FolderNum )     {   (( mFolderItemCount .find ( FolderNum ) != mFolderItemCount.end() ) ? mFolderItemCount [ FolderNum ] :  0 );  }

private:
    short mCurrentFolder = 0;
    //CDPlayItem getCDPlayItem(eCDPlayItemType mCDPlayItemType, int itemId, eCDPlayDirection mCDPlayDirection);
    short mParentFolderNum;
    uint32_t registerClient()
	{
        ++mNumofClient;
        
        return mNumofClient;
    }
    void deregisterClient(uint32_t ClientID)
	{
		--mNumofClient;
	}
	friend class FacadeHelper<RemoteCDPlaybackEngine>;
    FacadeHelper<MediaPresSettings> mediaSettings;
	CDPlayContext getCDPlayContext (eCDActionContext meCDActionContext, int mode);
	
	IRemoteCDPlugin* mRemoteCDPluginInstance;
   
    CDPlayContext mCDPlayContext;
    std::function<void(::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus)> mRemoteCDDiscStatusCallBack;
    std::function<void(::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus)>  mCDErrorStatusCallBack;
    std::function < void (::v3::com::harman::mmpres::MMPresTypes::eCDEOLStatus)>      mEolStatusCallBack; 
    std::function < void (bool)>      mCDEjectChimesWarningCallBack;
    uint32_t  mNumofClient;
    
    eCDStatus mCDStatus;
    MMPresTypes::eMetadataType mCDMetaDataType;
    
    CDError mCDError;
    bool pluginInstanceFlag;

    FacadeHelper<MediaPlaybackEngine> mPlayEngine;
    unordered_map <int , string >    mFolderNumNameMap  {} ;
    unordered_map <int , int >       mFolderItemCount   {} ;
    
    uint64_t mTotalTracks = 0 ;
    short mItemIndex;
    //CDPlayItem mCDPlayItem;

};

#endif // REMOTECDPLAYBACKENGINE_H
