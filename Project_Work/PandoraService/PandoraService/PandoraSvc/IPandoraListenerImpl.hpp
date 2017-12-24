/*----------------------------------------------------------------------------------------------------------------------
*
* 
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTimer.h"

#ifndef PANDORA_MESSAGE_LISTENER_HPP
#define PANDORA_MESSAGE_LISTENER_HPP
#include "api/mm/pandoralib/pf/src/CPandoraDataTypes.hpp"
//#include "api/mm/pandoralib/pf/src/CPandoraMessage.hpp"
#include "api/mm/pandoralib/pf/src/CPandoraProtocolMgr.hpp"
//#include "api/mm/pandoralib/pf/src/IPandoraProtocolListener.hpp"


class IPandoraListenerImpl : public IPandoraProtocolListener,public ITimerHandler
{
public:
	IPandoraListenerImpl();
	virtual void sendAcknowledgement (const uint8_t* data, const uint32_t &dataSize);
	/*The PresCtrl can send the next PandoraMessage over transport*/
	virtual void receivedAcknowledgement ();

	virtual void infoGetBrandingImage ();
	virtual void infoReturnGetStatus (pndr_return_status_params &);
	virtual void infoReturnTrackAlbumArtSegment(pndr_return_track_album_art_segment_params &);
	virtual void infoReturnTrackInfoExtended(pndr_return_track_info_extended_params &);
	virtual void infoReturnTrackInfo (pndr_return_track_info_params & );
	virtual void infoReturnTrackArtist (pndr_return_track_artist_params & );
	virtual void infoReturnTrackTitle (pndr_return_track_title_params & );
	virtual void infoReturnTrackAlbum (pndr_return_track_album_params & );
	virtual void infoReturnTrackExplain (pndr_return_track_explain_segment_params&);
	virtual void infoReturnStationActive (pndr_return_station_active_params & );
	virtual void infoReturnStationCount (pndr_return_station_count_params & );
	virtual void infoReturnStationTokens(pndr_return_station_tokens_params &);
	virtual void infoReturnStationInfo (pndr_return_station_info_params & );
	virtual void infoReturnStationsOrder (pndr_return_stations_order_params & );
	virtual void infoReturnStationArtSegment (pndr_return_station_art_segment_params & );
	virtual void infoReturnGenreCategoryCount(pndr_return_genre_category_count_params & );
	virtual void infoReturnGenreCategoryNames(pndr_return_genre_category_names_params & );
	virtual void infoReturnGenreCategoryStationCount(pndr_return_genre_category_station_count_params & );
	virtual void infoReturnGenreStationNames(pndr_return_genre_station_names_params & );
	virtual void infoReturnGenreStationArtSegment (pndr_return_genre_station_art_segment_params & );
	virtual void infoReturnSearchResultInfo (pndr_return_search_result_info_params &);
	virtual void infoReturnListener (pndr_return_listener_params & );

	virtual void infoUpdateTrackAlbumArt (pndr_update_track_album_art_params & );
	virtual void infoUpdateTrackElapsed (pndr_update_track_elapsed_params &);
	virtual void infoUpdateTrackRating (pndr_update_track_rating_params & );
	virtual void infoUpdateTrackExplain (pndr_update_track_explain_params&);
	virtual void infoUpdateTrackBookmarkArtist (pndr_update_track_bookmark_artist_params & );
	virtual void infoUpdateTrackBookmarkTrack (pndr_update_track_bookmark_track_params & );
	virtual void infoUpdateTrackCompleted (pndr_update_track_completed_params &);
	virtual void infoUpdateTrack (pndr_update_track_params & );
	virtual void infoUpdateStationsOrder (pndr_update_stations_order_params & );
	virtual void infoUpdateStationDeleted (pndr_update_station_deleted_params & );
	virtual void infoUpdateStationAdded (pndr_update_station_added_params & );
	virtual void infoUpdateStationActive(pndr_update_station_active_params & );
	virtual void infoUpdateSearch (pndr_update_search_params & );
	virtual void infoUpdateNotice(pndr_update_notice_params &);
	virtual void infoUpdateStatus (pndr_update_status_params &);
	void handleTimerEvent (CTimer* pTimer);
	void retryAlbumArt();
	void updateBrandImageSegments();
public:
	static std::ofstream myImagefile;
	CTimer	mAlbumArtRetryTimer;
};

#endif
