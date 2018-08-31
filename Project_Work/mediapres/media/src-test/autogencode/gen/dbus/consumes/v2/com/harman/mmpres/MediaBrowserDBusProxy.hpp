/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V2_COM_HARMAN_MMPRES_Media_Browser_DBUS_PROXY_HPP_
#define V2_COM_HARMAN_MMPRES_Media_Browser_DBUS_PROXY_HPP_

#include <v2/com/harman/mmpres/MediaBrowserProxyBase.hpp>
#include "v2/com/harman/mmpres/MediaBrowserDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusAttribute.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>
#include <CommonAPI/Types.hpp>
#include <CommonAPI/DBus/DBusSelectiveEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v2 {
namespace com {
namespace harman {
namespace mmpres {

class MediaBrowserDBusProxy
    : virtual public MediaBrowserProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    MediaBrowserDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~MediaBrowserDBusProxy() { }

    virtual SearchCandidatesAttribute& getSearchCandidatesAttribute();

    virtual SessionQueryUpdateSelectiveEvent& getSessionQueryUpdateSelectiveEvent();
    virtual SessionQueryWindowUpdateSelectiveEvent& getSessionQueryWindowUpdateSelectiveEvent();
    virtual ThumbnailListUpdateSelectiveEvent& getThumbnailListUpdateSelectiveEvent();
    virtual SessionQueryAlphaJumpUpdateSelectiveEvent& getSessionQueryAlphaJumpUpdateSelectiveEvent();

    /**
     * description: Opens a session for the client communication. This session will be used
     *   internally to uniquely identify a client, and to maintain client specific
     *   caches. 					  The client need not create a new session for every request,
     *   this is rather used to identify different instances of clients.               
     *          returns:     sessionID: Session identifier allocated to the client.
     */
    virtual void openSession(CommonAPI::CallStatus &_internalCallStatus, uint32_t &_sessionID, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> openSessionAsync(OpenSessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Close a client session and release all associated resources. Once HMI calls
     *   closeSession,they cannot use the same sessionId 	for furthur browsing. For
     *   this, they have to again invoke a openSession to get a new sessionId for
     *   furthur browsing.
     * param: sessionID: The Session ID to close.
     */
    virtual void closeSession(const uint32_t &_sessionID, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> closeSessionAsync(const uint32_t &_sessionID, CloseSessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Essentially Stops all background tasks in the specified session. But unlike
     *   closeSession, the sessionID 	 is valid and can be used for furthur browsing.
     * param: sessionID: The Session ID alloted.
     */
    virtual void cancelSession(const uint32_t &_sessionID, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> cancelSessionAsync(const uint32_t &_sessionID, CancelSessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets a session query. This api handles all requests to fetch metadata browse
     *   results, database searches, file system browse etc.  	 	  	 	 Note: iF HMI
     *   wants to show the categories like ARTISTS,ALBUMS,FOLDERS,GENRES etc on
     *   clicking browse icon,they need not 	 	 use the setSessionQuery. They can
     *   instead use the tSlotUpdate broadcast which contains the tDevice structure.In 
     *   	 	 this, browse capabilities will be provided as a bitmask of different
     *   eMetadataTypes.If HMI is interested in 	 	 the count of these categories,then
     *   HMI can use the MetadataCountMap which is a map of different
     *   eMetadataCountType. 	 	  	 	  	 	 1st Example:  	 	  	 	 Get a list of all
     *   available artists. The filterID's' and filter's' will be empty, and the result
     *   type would be set to artist id.  	 	 Presentation control would return a list
     *   of all available artists / artist id for the medium. 	 	 sessionQueryType will
     *   be METADATA_BROWSE  	 	  	 	 2nd Example: 	 	  	 	 Get a list of all available
     *   artists. Also for each artist,get the count of songs.For this, input
     *   parameters will be same as first example 	 	 but now HMI should pass
     *   metadataCountTypes=eMetadataType::METADATATYPE_SONG. 	 	  	 	 If HMI wants
     *   album count also along with songs count, then metadataCountTypes= bitmask
     *   combination of  	 	 eMetadataType::METADATATYPE_SONG +
     *   eMetadataType::METADATATYPE_ALBUM. 	 	  	 	 sessionQueryType will be
     *   METADATA_BROWSE  	 	  	 	 3rd Example :  	 	  	 	 Get a list of all available
     *   albums by the artist "Pink Floyd". HMI would know the artist id for "Pink
     *   Floyd" because of the previous returned result.  	 	  	 	 In the session
     *   query, HMI sets the filterIDMap as: 	 	  	 	
     *   key=eMetadataType::METADATATYPE_ARTIST,value="artist id" for Pink Floyd  	 	 
     *   	 	 resultType=eMetadataType::METADATATYPE_ALBUM.  	 	  	 	 Presentation
     *   Controller understands what needs to be done here. This can be further
     *   extended by setting 	 	 artist id and album id returned in the previous steps,
     *   and a query for songs can be done.  	 	  	 	 sessionQueryType will be
     *   METADATA_BROWSE. 	 	  	 	 Suppose we need the list of songs inside the artist
     *   "Pink Floyd" but for eah song song, we also need to pass the artist,album  	 	
     *   and genre values. Then this can be done by providing the "metadataAttributes"
     *   input parameter as a bit mask combination of
     *   eMetadataType::METADATATYPE_ARTIST + 	 	 eMetadataType::METADATATYPE_ALBUM &
     *   eMetadataType::METADATATYPE_GENRE. 	 	  	 	 Note that metadataAttributes will
     *   be considered only if the resultType="song". 	 	  	 	 4th Example :  	 	  	 	
     *   Get a list of all available albums by "Pink Floyd", which begin with the
     *   letters "The".  	 	 This is same as the 3rd example, but in addition, the
     *   filter "album" must be set to "The" 	 	  	 	 sessionQueryType will be
     *   METADATA_BROWSE  	 	  	 	 5th example: 	 	  	 	 usage of resetPrevious
     *   parameter. 	 	  	 	 Assume resetPrevious=false: 	 	  	 	 In the first
     *   example,HMI obtained the list of albums for artist "Pink Floyed". Now if HMI
     *   wants list of songs for 	 	 artist "Pink Floyed", then in the filters,HMi need
     *   not mention anything again.  	 	 They can just set
     *   resultype=eMetadataType::METADATATYPE_SONG . 	 	  	 	 Now pres will remember
     *   that artist "Pink Floyd" was already selected. So pres will supply the song
     *   list for artist pink floyd. 	 	  	 	 If resetPrevious=true,pres would not
     *   remeber that Artist Pink Floyd was already selected. In that case, 	 	 HMI has
     *   to set the filterIDMap again by providing the artist id for pink Floyd. 	 	  	
     *   	 Now in the use cases, where the resultType is
     *   "eMetadataType::METADATATYPE_SONG", if the requirement is to show only AUDIO 	
     *   	 or VIDEO or both AUDIO + VIDEO, this can be done by setting "ContentMask"
     *   param as a bit mask of "eTitleContentType". 	 	  	 	
     *   eTitleContentType::TITLETYPE_IMAGE will be applicable for filesystem browse. 
     *   	 	  	 	 6th Example: 	 	  	 	 Do a fileSystem or FOLDER browse.This is
     *   applicable for mass storage devices like USB.At any level, we are  	 	 allowed
     *   to show both folders, files or playlists depending on project requirements. 	
     *   	  	 	 sessionQueryType will be SESSION_QUERY_FS_BROWSE. 	 	  	 	 First go to
     *   root level and show only folders and files but not playlists. For this, the
     *   filterID's and filters are empty, but the resultType will be  	 	 combination
     *   of eMetadataType::METADATATYPE_FOLDER + eMetadataType::METADATATYPE_FILE.With
     *   this query,HMI can get 	 	 the folders and file at a given level in
     *   filesystem. When HMI gets the individual items, they can check the
     *   eMetadataType 	 	 of that item to see if it is a file or folder. 	 	  	 	 Now
     *   HMI wants to go inside one more level by selecting a folder obtained in the
     *   previous step.For this,HMI can set the filterID as  	 	
     *   key=METADATAID_FOLDER_ID and value=(folder id or item id) of folder
     *   communicated in previous step. 	 	  	 	 If at every level, there is a
     *   requirement to list folders first and only then followed by audio files,image
     *   files etc, then 	 	 these will be maintained by pres as project specific
     *   config parameters.  	 	  	 	 Now HMI wants to go inside one of the folders
     *   named "Folder1" listed at the root level.For this,HMI can set the FilterIDMap
     *   as: 	 	  	 	 key: "eMetadataType::METADATATYPE_FOLDER" and value: folder id of
     *   "folder 1" from the result of the first query to  	 	       show the root
     *   level contents. 	 	  	 	 Also if HMI wants the level ID (depth of folder) in
     *   file system browse, such as 4th folder depth or fifth folder depth etc, 	 	
     *   then they can use the sessionQueryID as an indicator of level ID. Each
     *   sessionQueryID will correspond to a 	 	 particular folder depth in file system
     *   browse. 	 	  	 	 7th example: 	 	  	 	 If HMI wants the list of songs from the
     *   recently played list. 	 	  	 	 The can set the sessionQueryType will be
     *   SESSION_QUERY_RECENTLYPLAYED and resultype=eMetadataType::METADATATYPE_SONG 	 
     *      other parameters can be empty.
     * param: filterIDs: The previously communicated numeric identifiers of the different
     *   filter categories.
     * param: filters:	 The string filters which should be applied to the result.
     * param: resultType: The data which is expected from the query.
     * param: sessionID: The Session ID alloted.
     * param: slotId:	 The identifier for the slot.
     * param: type:		 The type of session query.
     * param: metadataAttributes: [TODO]:Pres ctrl can return the required attributes for
     *   each metadata item based on the                                           
     *   client requirements.This is a bitmask combination of eMetadataType values.
     *   This will be                                            supported only for the
     *   song category.
     * param: metadataCountTypes: Pres Ctrl can count the number of items present in a
     *   subcategory. For example, while retreving a list of all artists,          
     *   								  Pres Ctrl can also count how many albums/songs are available for
     *   each artist. (Might be a costly operation depending on Media Type.          
     *   								  	Select this option only if really required )
     * param: resetPrevious: Whether to use old history or not. Pres Ctrl has internal
     *   history mechanism. For example          							 if in the first session query,
     *   the list of all available artists was obtained.In the 2nd browse level, HMI
     *   requested for          							 all albums by a specific artist. If now HMI
     *   wants a list of all songs by the artist, it need not tell the selected artist 
     *            							 again. However if this flag is reset, it will override this
     *   behavior.
     * param: AutoUpdateEnabled: 	This is applicable for result sets which might update
     *   dynamically, for example the current playing list for external         
     *   								devices might update on its own.
     * param: ContentMask      //Bitmask combination of eTitleContentType. Bitmask
     *   combination for what content is expected - Audio/Video/Image.                 
     *    returns:	  numResults: The number of records matching the search criteria.
     *   (The number of results might not be accurate for ipod iAP1 like devices)      
     *                returns      sessionQueryID:The sessionQueryID allotted.This
     *   uniquely identifies a particular session query/filter for retrieving data in a
     *   session and it has to be passed                                        for
     *   furthur setting the sessionQuery window and retrieving the contents of that
     *   window.
     */
    virtual void setSessionQuery(const uint32_t &_sessionID, const uint8_t &_slotId, const ::v2::com::harman::mmpres::MMPresTypes::FilterIDMap &_filterIDs, const ::v2::com::harman::mmpres::MMPresTypes::FilterMap &_filters, const ::v2::com::harman::mmpres::MMPresTypes::eMetadataType &_resultType, const uint32_t &_metadataAttributes, const uint32_t &_metadataCountTypes, const ::v2::com::harman::mmpres::MMPresTypes::eSessionQueryTypes &_type, const bool &_resetPrevious, const bool &_AutoUpdateEnabled, const uint32_t &_ContentMask, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, uint64_t &_numResults, uint32_t &_sessionQueryID, std::string &_breadcrumbpath, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setSessionQueryAsync(const uint32_t &_sessionID, const uint8_t &_slotId, const ::v2::com::harman::mmpres::MMPresTypes::FilterIDMap &_filterIDs, const ::v2::com::harman::mmpres::MMPresTypes::FilterMap &_filters, const ::v2::com::harman::mmpres::MMPresTypes::eMetadataType &_resultType, const uint32_t &_metadataAttributes, const uint32_t &_metadataCountTypes, const ::v2::com::harman::mmpres::MMPresTypes::eSessionQueryTypes &_type, const bool &_resetPrevious, const bool &_AutoUpdateEnabled, const uint32_t &_ContentMask, SetSessionQueryAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Ask presentation control to fetch the desired set of entries for a particular
     *   session query. This call does not block for the result to complete, and will
     *   override 					  the last request given to Pres Ctrl.
     * param: sessionID: 	The Session ID alloted.
     * param: sessionQueryID: The sessionQueryID allotted.This uniquely identifies a
     *   particular session query/filter for retrieving data in a session and it has to
     *   be passed                                        for furthur setting the
     *   sessionQuery window and retrieving the contents of that window.
     * param: startIndex:	The index to start fetching.
     * param: numItems:		The number of items to fetch.
     */
    virtual void setSessionQueryWindow(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setSessionQueryWindowAsync(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, SetSessionQueryWindowAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Reselects the previous selected session query criteria. The previously shared
     *   sessionQueryID  will be returned.
     */
    virtual void gotoPreviousSessionQuery(const uint32_t &_sessionID, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, uint64_t &_numResults, uint32_t &_sessionQueryID, ::v2::com::harman::mmpres::MMPresTypes::ePrevBrowseLevel &_browselevel, std::string &_breadcrumbpath, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> gotoPreviousSessionQueryAsync(const uint32_t &_sessionID, GotoPreviousSessionQueryAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: HMI Can use this to do a direct jump to any previous query ids
     */
    virtual void jumpToSessionQuery(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, uint64_t &_numResults, std::string &_breadcrumbpath, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> jumpToSessionQueryAsync(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, JumpToSessionQueryAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Play session category. This api handles all requests to play metadata browse
     *   results, database searches, etc .                                             
     *      For this API, HMI should have initially done a openSession used the
     *   setSessionQuery to browse something. Then playSession                will play
     *   the currently selected browse context.                                1st
     *   Example:                               If the HMI wanted to play a list of all
     *   available songs.                                Initially HMI would have got
     *   the list of songs using setSessionQuery.                                Now
     *   playSession will just play the songs of that session.                         
     *      If HMI wants to play a particular song from the list of songs, then HMI can
     *   set the filterID map by:                                passing the id of the
     *   praticular song.                                But if there is a rquirement
     *   that on selecting the partiuclar song, the entire collection has to be played
     *   with the                 selected song maintaining its position/offset, then
     *   this type of playback will be maintained in pres ctrl project               
     *   configuration.                                         2nd Example :          
     *                        We are in the root level of file system and HMI screen
     *   has the list of folders and files.Now HMI selects a file.               We
     *   want to play only that file.                               Then HMI can pass
     *   the fileID in filterIDMap.                               But if the project
     *   requirement is to play that file + other files in the same level together, or
     *   even incude the files from subfolders,then this will               be
     *   maintained as a config parameter in pres.                               or if
     *   there is a button in HMI called "playAll" at each level of file system browse
     *   and on clicking that, we               need to play all the files in that
     *   level. Now for these cases, HMI can just pass the sessionID as an input       
     *          based on project requirements,we can play all the files in the current
     *   folder browse level or include the                 subfolders.
     * param: sessionID: The Session ID alloted.
     * param: filterIDs: The previously communicated numeric identifiers of the different
     *   filter categories.                                     Should be zero for
     *   playAll for the Folderlevel else the file index for playing the particular file
     */
    virtual void playSession(const uint32_t &_sessionID, const ::v2::com::harman::mmpres::MMPresTypes::FilterIDMap &_filterIDs, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> playSessionAsync(const uint32_t &_sessionID, const ::v2::com::harman::mmpres::MMPresTypes::FilterIDMap &_filterIDs, PlaySessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This requests the Media pres to play the track of a given position from the
     *   NowPlaying list 	                     trackPosition: Postion of the track from
     *   the NowPlaying list to jump to.
     */
    virtual void gotoTrack(const uint64_t &_trackPosition, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> gotoTrackAsync(const uint64_t &_trackPosition, GotoTrackAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This requests the pres to directly jump to the file system level or metadata
     *   browse path level which                     contains the currently playing
     *   track and returns the number of items in that level matching the filter       
     *                 criteria.  	                     For example, after connecting
     *   the USB and selecting it for playback and it starts playing some file         
     *               (which is at the 4th nested level from root).  	                  
     *     Now clicking on browse icon, based on project requirement , we can directly
     *   jump to the 4th nested level                     (assuming it is the
     *   nowplaying level) by calling this API.                     [TODO]:The
     *   interface input & output parameters are yet to be finalized. To check if the
     *   interface can be                     combined with setSessionQuery.
     */
    virtual void gotoNowplayingLevel(const uint32_t &_sessionID, const uint8_t &_slotId, CommonAPI::CallStatus &_internalCallStatus, ::v2::com::harman::mmpres::MMPresTypes::eCallResult &_result, uint64_t &_numItems, uint32_t &_sessionQueryID, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> gotoNowplayingLevelAsync(const uint32_t &_sessionID, const uint8_t &_slotId, GotoNowplayingLevelAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<SearchCandidatesAttribute, CommonAPI::DBus::ArrayDeployment<CommonAPI::DBus::StringDeployment>>> searchCandidates_;

    CommonAPI::DBus::DBusSelectiveEvent<SessionQueryUpdateSelectiveEvent, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>> sessionQueryUpdateSelective_;
    CommonAPI::DBus::DBusSelectiveEvent<SessionQueryWindowUpdateSelectiveEvent, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v2::com::harman::mmpres::MMPresTypes::eQueryStatus, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v2::com::harman::mmpres::MMPresTypes::MetadataItemList, ::v2::com::harman::mmpres::MMPresTypes_::MetadataItemListDeployment_t>, CommonAPI::Deployable<bool, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>> sessionQueryWindowUpdateSelective_;
    CommonAPI::DBus::DBusSelectiveEvent<ThumbnailListUpdateSelectiveEvent, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint64_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<std::vector<std::string>, CommonAPI::DBus::ArrayDeployment<CommonAPI::DBus::StringDeployment>>> thumbnailListUpdateSelective_;
    CommonAPI::DBus::DBusSelectiveEvent<SessionQueryAlphaJumpUpdateSelectiveEvent, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v2::com::harman::mmpres::MMPresTypes::arAlphaIndex, ::v2::com::harman::mmpres::MMPresTypes_::arAlphaIndexDeployment_t>> sessionQueryAlphaJumpUpdateSelective_;

};

} // namespace mmpres
} // namespace harman
} // namespace com
} // namespace v2

#endif // V2_COM_HARMAN_MMPRES_Media_Browser_DBUS_PROXY_HPP_

