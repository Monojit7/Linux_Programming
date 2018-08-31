#include <spot/harman_kpimarker.h>
#include "MediaBrowserStubImpl.h"

#define REMOTE_CD_DEVICE_ID 65537

namespace v3 {
namespace com {
namespace harman {
namespace mmpres {

MediaBrowserStubImpl::MediaBrowserStubImpl()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::MediaBrowserStubImpl");

   mItemIndexTobeHighlightedAttribute.clear();

   mBrowser->setWindowUpdateHandler(std::bind(&MediaBrowserStubImpl::fireSessionQueryWindowUpdateSelective,this,_1,_2,_3,_4,_5,_6,_7,_8));
   mBrowser->setWindowUpdateForThumbnailHandler(std::bind(&MediaBrowserStubImpl::fireThumbnailListUpdateSelective,this,_1,_2,_3,_4,_5));
   mBrowser->setSearchCandidatesStubCallBack(std::bind(&MediaBrowserStubImpl::setSearchCandidatesAttribute,this,_1));
   mBrowser->setAlphajumpUpdateHandler(std::bind(&MediaBrowserStubImpl::fireSessionQueryAlphaJumpUpdateSelective,this,_1,_2,_3));
   mBrowser->setItemIndexTobeHighlightedStubCallBack(std::bind(&MediaBrowserStubImpl::setItemIndexTobeHighlightedAttribute, this, _1));
}

MediaBrowserStubImpl::~MediaBrowserStubImpl()
{

}

void MediaBrowserStubImpl::openSession(const std::shared_ptr<CommonAPI::ClientId> _client, openSessionReply_t reply)
{
   uint32_t id = mManager->openSession();
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::opensession-session id=%lu", id);
   reply(id, MMPresTypes::eCallResult::CALLRESULT_OK);
}

void MediaBrowserStubImpl::closeSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, closeSessionReply_t reply)
{
   bool ret = mManager->closeSession(sessionID);
   if (ret)
      reply(MMPresTypes::eCallResult::CALLRESULT_OK);
   else
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::closeSession-session id=%lu ret %d", sessionID, ret);

}

void MediaBrowserStubImpl::cancelSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, cancelSessionReply_t reply)
{
   bool ret = mManager->cancelSession(sessionID);
   if (ret)
      reply(MMPresTypes::eCallResult::CALLRESULT_OK);
   else
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::cancelSession-session id=%lu ret %d", sessionID, ret);
}

void MediaBrowserStubImpl::setSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint8_t slotId, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap filterIDs, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filters, ::v3::com::harman::mmpres::MMPresTypes::eMetadataType resultType, uint32_t metadataAttributes, uint32_t metadataCountTypes, ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes type, bool resetPrevious, bool AutoUpdateEnabled, uint32_t ContentMask, setSessionQueryReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQuery-resultType=%lu,slotId=%d,query type=%d,resetPrevious = %d",resultType,slotId,(int)type,resetPrevious);
   Session* session = mManager->getSession(sessionID);
   bool bPauseVideoPlaybackDuringBrowse = false;
   bool bVideoPlayStatus = false;

      uint64_t mediumID = mDetect->getDevice(slotId);
      DeviceBase* pDevice = mDetect->getDevice(mediumID);
      if (nullptr != pDevice)
      {
         if (session != nullptr)
         {
        	 if(resetPrevious)
        	  {
        		 pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
        	  }
            //FacadeHelper<MediaPresSettings> mSettings;

            bPauseVideoPlaybackDuringBrowse = mSettings->getPauseVideoPlaybackDuringBrowse();
            bVideoPlayStatus = pDevice->getVideoPlayStatus();

            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaBrowserStubImpl::setSessionQuery - bVideoPlayStatus = %d, bPauseVideoPlaybackDuringBrowse = %d", bVideoPlayStatus, bPauseVideoPlaybackDuringBrowse);

            /* pause video playback if browsing is initiated */
            if(bPauseVideoPlaybackDuringBrowse && bVideoPlayStatus)
            {
                pDevice->pause();
            }

            checkforclearingSessionCache(sessionID,mediumID, filterIDs, type, filters);
            pDevice->SessionQueryCreateNew(sessionID, _client, std::move(reply), std::move(nullptr), std::move(nullptr), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask);
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQuery-sessionID=%lu not found", sessionID);
            reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER, 0, 0, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQuery-mediumID=%llu not found", mediumID);
         reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED, 0, 0, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
      }
}

void MediaBrowserStubImpl::setSessionQueryWindow(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint32_t sessionQueryID, uint64_t startIndex, uint64_t numItems, setSessionQueryWindowReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-sessionID=%lu,sessionQueryID=%lu,startIndex=%llu,numItems=%llu", sessionID, sessionQueryID, startIndex, numItems);
   uint32_t currQueryId = 0;

   if (startIndex > 0 && numItems>0)
   {
      Session* session = mManager->getSession(sessionID);

      if (session != nullptr)
      {
         currQueryId = session->getCurrentQueryId();
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-currQueryId=%lu", currQueryId);
         if (currQueryId != sessionQueryID)
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-currQueryId is not same as the input queryId");
            reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
         }
         else
         {
            Session::SessionQueryData* data = session->getSessionQueryData(sessionQueryID);
            if (data != nullptr)
            {
               DeviceBase* pDevice = mDetect->getDevice(data->mMediumId);
               if (nullptr != pDevice)
               {
                  pDevice->SessionQueryCreateExisting(sessionID, sessionQueryID, _client, std::move(nullptr), std::move(nullptr), std::move(nullptr), (*data), startIndex, numItems);
               }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-mediumID=%llu not found", data->mMediumId);
                  reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED);
                  session->removeQueryFromCache(sessionQueryID);
               }
            }
            else
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-sessionQueryID=%lu not found", sessionQueryID);
               reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
            }
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::setSessionQueryWindow-sessionID=%lu not found", sessionID);
         reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
      }
   }
   else
   {
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
   }
   
}

void MediaBrowserStubImpl::gotoPreviousSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, gotoPreviousSessionQueryReply_t reply)
{
   //FacadeHelper<MediaPresSettings> mSettings;
   if (mSettings->getBrowseHistoryEnumeration())
   {
      gotoPreviousSessionQueryWithBrowseEnumeration(_client, sessionID, std::move(reply));
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-sessionID=%lu", sessionID);
      Session* session = mManager->getSession(sessionID);

      if (session != nullptr)
      {
         uint32_t currqueryId = session->getCurrentQueryId();
         uint32_t prevqueryId = 0;

         Session::SessionQueryData* data = session->getSessionQueryData(currqueryId);
         if (data != nullptr)
         {
            DeviceBase* pDevice = mDetect->getDevice(data->mMediumId);
            if (nullptr != pDevice)
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-no browse history enumeration");
               prevqueryId = session->getPreviousQueryId(currqueryId);
               Session::SessionQueryData* prevdata = session->getSessionQueryData(prevqueryId);

               if ((prevqueryId > 0)&&(nullptr != prevdata))
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-sessionID=%lu,prevqueryId=%lu", sessionID, prevqueryId);
                  pDevice->SessionQueryCreateExisting(sessionID, prevqueryId, _client, std::move(nullptr), std::move(reply), std::move(nullptr), (*prevdata), 0, 0);
               }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-return enum for root5");
                  reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
               }
            }
            else
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-mediumID=%llu not found", data->mMediumId);
               reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
               session->removeQueryFromCache(currqueryId);
            }
         }
         else
         {

         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQuery-sessionID=%lu not found", sessionID);
         reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
      }
   }
}

void MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, gotoPreviousSessionQueryReply_t&& reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-sessionID=%lu", sessionID);
   Session* session = mManager->getSession(sessionID);

   if (session != nullptr)
   {
      uint32_t currqueryId = session->getCurrentQueryId();
      uint32_t prevqueryId = 0;

       DeviceBase* pDevice1 = mDetect->getActiveDevice();
       if(pDevice1 != nullptr)
       {
       //if((!(pDevice1->getTrackListSupported())&&(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == pDevice1->getDeviceType()))&&(currqueryId == 0))
    	  if(!(pDevice1->getTrackListSupported())&&(currqueryId == 0))
           {
              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumerationFor BT when Track not support");
              reply(MMPresTypes::eCallResult::CALLRESULT_OK, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
              pDevice1->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
           }
        }

      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-currqueryId:%lu", currqueryId);
      Session::SessionQueryData* data = session->getSessionQueryData(currqueryId);
      if (data != nullptr)
      {
         DeviceBase* pDevice = mDetect->getDevice(data->mMediumId);
         if (nullptr != pDevice)
         {
            if (pDevice->getBrowseHistoryEnumeration() == MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT)
            {
              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-PREVBROWSELEVEL_ROOT");
              reply(MMPresTypes::eCallResult::CALLRESULT_OK, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-setting enum back to default1");
              pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
            }
            else if (pDevice->getBrowseHistoryEnumeration() == MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_NOWPLAYINGDATA)
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-return enum for last play plan level");
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-PREVBROWSELEVEL_NOWPLAYINGDATA");
                if(pDevice->getPlayPlanQueryId()>0)
                 {
                   uint32_t currqId = pDevice->getPlayPlanQueryId();
                   uint32_t prevqId = 0;
                   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-currqId:%lu", currqId);
                    if(currqId > 0)
                    {
                     prevqId =  pDevice->getPreviousPlayPlanQueryId(currqId);
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-prevqId:%lu", prevqId);
                    }
                   session->switchToPlayPlanQuery(pDevice->getPlayPlanQueryList());
                   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-pDevice->getPlayPlanQueryId()=%lu", pDevice->getPlayPlanQueryId());
                   if(prevqId > 0)
                    {
                     Session::SessionQueryData* data1 = session->getSessionQueryData(prevqId);
                     
                    // Below code added to track the folder id for Remote CD 
                     
                     uint64_t currentFolderNumber   = 0;
                     
                     auto it = ( data1 ->mFilterIDMap.find ( MMPresTypes::eMetadataType::METADATATYPE_FOLDER ) );
                     
                     if ( it != ( data1 ->mFilterIDMap ).end() )
                     {
                         currentFolderNumber = it->second;
                         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d <foldernumber : %d>", __FUNCTION__, __LINE__, currentFolderNumber  );
                     }
                     
                     ////////////////////////////////////////////////////
                     
                     if (data1 != nullptr)
                     {
                        
                        // If the device is not RemoteCD then then process sessionQuery onRun else processing would be done only previous data is not root data
                         
                       if ( data1->mMediumId != REMOTE_CD_DEVICE_ID )
                       {
                           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration Local- data1->mBreadCrumbPath:%s ", data1->mBreadCrumbPath.c_str());
                          pDevice->SessionQueryCreateExisting(sessionID, prevqId, _client, std::move(nullptr), std::move(reply), std::move(nullptr), (*data1), 0, 0);
                           data1->itemList.clear();
                          pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
                       }
                       else if (  currentFolderNumber  == 0  )
                       {
                           LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d", __FUNCTION__, __LINE__ );
                           reply(MMPresTypes::eCallResult::CALLRESULT_OK, 9, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "",MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                       }
                       
                       if  ( ( data1->mMediumId == REMOTE_CD_DEVICE_ID ) && ( currentFolderNumber != 0 ) )
                       {
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d", __FUNCTION__, __LINE__ );
                        
                            pDevice->SessionQueryCreateExisting(sessionID, prevqId, _client, std::move(nullptr), std::move(reply), std::move(nullptr), (*data1), 0, 0);
                       
                            pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
                       }
                       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                       
                     }
                     else
                     { 
                       LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-return error during history computation data1 is null");
                       reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_NOWPLAYINGDATA, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                       pDevice->setPlayPlanQueryId(0);
                     }
                   }
                  else
                   {
                      reply(MMPresTypes::eCallResult::CALLRESULT_OK, 9, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "",MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-setting enum back to default123");
                      pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
                   }

                }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-return enum for root2");
                  reply(MMPresTypes::eCallResult::CALLRESULT_OK, 9, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-setting enum back to default1234");
                  pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
               }

            }
            else
            {
            	Session::SessionQueryData* prevdata = nullptr;
            	prevqueryId = session->getPreviousQueryId(currqueryId);
                if(prevqueryId > 0)
                {
                   prevdata = session->getSessionQueryData(prevqueryId);
                }

                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-sessionID=%lu,prevqueryId=%lu", sessionID, prevqueryId);
                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-PREVBROWSELEVEL_DEFAULT");
                /* Below code is for smart search
                * During back button operation on multi-level smart search, get previous Query Result and check against current Query result.
                * Let us take below example for multi-search. Artist ---> Albums of selected artist ---> Songs of selected album of artist
                * 1) User at 3rd level of multi-search i.e. at Songs level, where till Songs level is reached by normal browsing from Album level.
                * 2) But artist is selected through smart search and then normal browsing is done.
                * 3) User presses back button from Songs level, next screen should be album list of selected artist. In this case, normal scenario and below
                * condition will not hold true.
                * 4) Now user at album list of selected artist i.e. at 2nd level. And now user presses back from this screen.
                * 5) Expectation is next screen should be complete list of Artist from where actual smart search is started i.e. 1st level.
                * For this, current screen is artist and hence current Query result is Artist. But previous Query is the one when user presses smart search button
                * and enter into Smart search screen. This is dummy Query for mpres but valid from HMI point of view whose result is Artist only.
                * Hence, we should go back to 1 more query and so if result type are same, go back to previous query and hence condition.
                * Through this condition, previous Query data i.e. complete list of artist is obtained and provided to HMI.
                *
                * Note:- For all smart search specific queries, QueryID is not added to QueryList.
                *
                */
                if (data->mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE)
                {
                   if (prevdata != nullptr)
                   {
                      if (data->mResultType == prevdata->mResultType)
                      {
                         prevqueryId = session->getPreviousQueryId(prevqueryId);
                         prevdata = session->getSessionQueryData(prevqueryId);
                      }
                   }
                }

                if(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == pDevice->getDevType())
                {
                  uint32_t prevqueryidBtsa = session->getPreviousQueryId(currqueryId);
                  prevqueryidBtsa = session->getPreviousQueryId(prevqueryidBtsa);
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-BTSA prevqueryidBtsa=%lu", prevqueryidBtsa);
                   if(prevqueryidBtsa  == 0)
                   {
                     prevqueryId = 0;
                   }
                 }

               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-sessionID=%lu,Final prevqueryId=%lu", sessionID, prevqueryId);
               if ((prevqueryId > 0) && (nullptr != prevdata))
               {
                  
                   // This is for updating the previous Folder number 
                    uint64_t currentFolderNumber   = 0;
                     
                     auto it = ( prevdata ->mFilterIDMap.find ( MMPresTypes::eMetadataType::METADATATYPE_FOLDER ) );
                     
                     if ( it != ( prevdata ->mFilterIDMap ).end() )
                     {
                         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d <foldernumber : %d>", __FUNCTION__, __LINE__, currentFolderNumber  );
                         currentFolderNumber = ( it ) -> second;
                         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d <foldernumber : %d>", __FUNCTION__, __LINE__, currentFolderNumber  );
                     }
                     
                     ///////////////////////////////////////////////////////////////////////
                     
                     // Process the onRun if device is not Remote CD else processing could be done RemoteCD based 
                    if ( data-> mMediumId != REMOTE_CD_DEVICE_ID ) 
                    {

                       pDevice->SessionQueryCreateExisting(sessionID, prevqueryId, _client, std::move(nullptr), std::move(reply), std::move(nullptr), (*prevdata), 0, 0);
                    }


                    else if (  currentFolderNumber  == 0  )
                    {
                        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d", __FUNCTION__, __LINE__ );
                        reply(MMPresTypes::eCallResult::CALLRESULT_OK, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "",MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                    }

                    if  ( ( data ->mMediumId == REMOTE_CD_DEVICE_ID ) && ( currentFolderNumber != 0 ) )
                    {
                        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d", __FUNCTION__, __LINE__ );

                        pDevice->SessionQueryCreateExisting(sessionID, prevqueryId, _client, std::move(nullptr), std::move(reply), std::move(nullptr), (*prevdata), 0, 0);

                    }
               }
               else if (prevqueryId == 0)
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-return enum for root5currqueryId:%d",currqueryId);
                  reply(MMPresTypes::eCallResult::CALLRESULT_OK, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
               }
               else
               {
                 //do nothing
            	  reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
               }
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-setting enum back to default12");
               pDevice->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT);
            }
         }//pDevice != nullptr
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-mediumID=%llu not found", data->mMediumId);
            reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
            session->removeQueryFromCache(currqueryId);
         }
      }//data != nullptr
      else
      {
          //do nothing
    	  reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
      }
   }//session != nullptr
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoPreviousSessionQueryWithBrowseEnumeration-sessionID=%lu not found", sessionID);
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER, 0, 0, MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, "", MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
   }
}

void MediaBrowserStubImpl::jumpToSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint32_t sessionQueryID, jumpToSessionQueryReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::jumpToSessionQuery-sessionID=%lu,queryId=%lu", sessionID, sessionQueryID);
   Session* session = mManager->getSession(sessionID);

   if (session != nullptr)
   {
      Session::SessionQueryData* data = session->getSessionQueryData(sessionQueryID);
      if (data != nullptr)
      {
         DeviceBase* pDevice = mDetect->getDevice(data->mMediumId);
         if (nullptr != pDevice)
         {
            if (!session->isJumpValid(sessionQueryID))
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::jumpToSessionQuery- jump is not possible");
               reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER, 0, "");
            }
            pDevice->SessionQueryCreateExisting(sessionID, sessionQueryID, _client, std::move(nullptr), std::move(nullptr), std::move(reply), (*data), 0, 0);
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::jumpToSessionQuery-mediumID=%llu not found", data->mMediumId);
            reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED,0,"");
         }
      }
      else
      {
         reply(MMPresTypes::eCallResult::CALLRESULT_CANCELLED_USER_REQUEST,0,"");
      }
   }
   else
   {
      reply(MMPresTypes::eCallResult::CALLRESULT_CANCELLED_USER_REQUEST,0,"");
   }
}

void MediaBrowserStubImpl::playSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap filterIDs, playSessionReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-sessionID=%lu", sessionID);
   Session* session = mManager->getSession(sessionID);
   uint64_t fileId = 0;
   uint32_t currQueryId = 0;
   
   std::string path = "";
   bool fileIdAvailable = false;
   bool bCurrentPlayingFileselected = false;
   auto fileSelectionitr = filterIDs.find(MMPresTypes::eMetadataType::METADATATYPE_FILE);
   auto playallSelectionitr = filterIDs.find(MMPresTypes::eMetadataType::METADATATYPE_FOLDER);

   if (session != nullptr)
   {
      currQueryId = session->getCurrentQueryId();
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-currQueryId=%lu", currQueryId);
      if (fileSelectionitr != filterIDs.end())
      {
         fileId = fileSelectionitr->second;
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-fileId=%llu",fileId);
         std::unordered_map<uint64_t, Session::ItemData>::const_iterator ctr;
         Session::SessionQueryData* data = session->getSessionQueryData(currQueryId);
         if (data!=nullptr)
         {
            DeviceBase* pDevice = mDetect->getDevice(data->mMediumId);
            if (nullptr != pDevice)
            {
                if (data->mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE)
                {
                  path.append(data->mBreadCrumbPath);
                  std::string key ("/");
                  std::size_t found = path.rfind(key);

                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl:SESSION_QUERY_METADATA_BROWSE::playSession- for found=%d",found);
                  if ((found!=std::string::npos)&& (found!=0))
                  {
                     path.replace (found,key.length(),":");
                  }
                  pDevice->setBreadCrumbForSetSession(path);
                }
                else if (data->mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE)
                {
                   path.append(data->mBreadCrumbPath);
                   std::size_t found = path.find_last_of("/\\");
                   std::string file = path.substr(found+1);
                   path = file;
                   pDevice->setBreadCrumbForSetSession(path);
                   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl:SESSION_QUERY_FS_BROWSE::playSession- for path=%s",path.c_str());
                }
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-query data available for query id=%lu, item list size=%d", currQueryId, data->itemList.size());
               Session::ItemData itemData;
               if (data->itemList.size() > 0)
               {
                  for (ctr = data->itemList.begin(); ctr != (data->itemList.end()); ctr++)
                  {
                     itemData = ctr->second;
                     if (itemData.item.getID() == fileId)
                     {
                        path = itemData.mItemPlayUri;
                        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-playing file id=%llu, file path:%s", fileId, path.c_str());
                        mSourceSwitch->SetretainPauseStateForDevice(false);
                        if(!mAudioInterface->isCallOnGoing())
                        {
                           if( data->mMediumId != REMOTE_CD_DEVICE_ID )
                           {
                              KPIMARKER(306, "AV_Request_Play");
                              mPlayer->getPlayerProxy()->openUriAsync(path, std::bind(&MediaBrowserStubImpl::openuriCallback, this, data->mMediumId,sessionID,currQueryId, _1, _2));
                              LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::playSession - Selected ItemID %llu CurrentItemID %llu", itemData.mItemId, pDevice->getNowPlayingInfo().getFid());
                              if (MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == pDevice->getDeviceType())
                              {
                                 LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::playSession -currenttrack %llu ", pDevice->getTrackState().getCurrentTrack());
                                 if (pDevice->getTrackState().getCurrentTrack() > 0)
                                 {
                                    if (itemData.mItemId == pDevice->getTrackState().getCurrentTrack())
                                    {
                                       bCurrentPlayingFileselected = true;
                                       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::playSession track comparison");
                                    }
                                 }
                                 else if (pDevice->getNowPlayingInfo().getTitle().compare(itemData.item.getValue())==0)
                                 {
                                    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::playSession Title comparison");
                                    bCurrentPlayingFileselected = true;
                                 }
                              }
                              else if (itemData.mItemId == pDevice->getNowPlayingInfo().getFid())
                              {
                                 bCurrentPlayingFileselected = true;
                              }
                              else
                              {
                                 //Do Nothing
                              }
                           }
                           else
                           {
                                /*Sometimes RemoteCD module gives delayed active folder Info. Therefore we are extracting the folder number 
                              from the requested track ID*/
                              short lActiveFolderNumber = ((fileId >> 16) & 0xFFFF);
                              
                              LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d playSession for Remote CD <trackIndex : %d , trackIndex from fileid : %d , ctr-> first : %d >",__FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mTrackIndex,    ( fileId  & 0xFFFF ),  ctr-> first  );
                              
                              LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d <ActiveFolderNumber : %d , TotalItem : %d>", __FUNCTION__, __LINE__, lActiveFolderNumber , data -> mNumResults );
                           
                              if ( ( lActiveFolderNumber == mRemoteCDPlayBackEngine -> getActiveFolderNumber()  ) )
                              {
                                    if (mRemoteCDPlayBackEngine->mTrackIndex != (fileId & 0xFFFF))
                                    {
                                        LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Calling setPlayItem  %lu \n", __FUNCTION__, __LINE__, fileId  );
                                          mRemoteCDPlayBackEngine->setPlayItem(eTrack, fileId);
                                    }
                                    else
                                    {
                                        reply(MMPresTypes::eCallResult::CALLRESULT_OK);
                                        FacadeHelper<MediaPlaybackEngine> mPlayEngine;
                                         std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
                                         
                                        if (nowPlayingCallback != nullptr)
                                        {
                                            LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Sending NowPlaying Data for fileName  %d \n", __FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine.getFid () );
                                            nowPlayingCallback( mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine );
                                        }
                                    }
                              }
                              else
                              {
                                  mRemoteCDPlayBackEngine->setPlayItem(eTrack, fileId);
                                  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d playSession for Remote CD <trackIndex : %d , trackIndex from fileid : %d , ctr-> first : %d >",__FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mTrackIndex,    ( fileId  & 0xFFFF ),  ctr-> first  );
                              }
                              mRemoteCDPlayBackEngine->setActiveFolderNumber(lActiveFolderNumber);
                              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession- setPlayItem playing file id=%llu ActiveFolderNumber %d", fileId, lActiveFolderNumber);
                              pDevice->setPlayPlanQueryId( currQueryId );
                              session->insertToPlayPlanDeviceList( data->mMediumId );
                              pDevice->contructPlayPlan(session->getQueryList());
                           }
                        }
                        else
                        {
                           mPlayer->getPlayerProxy()->enqueueUriAsync(path,std::bind(&MediaBrowserStubImpl::enqueueUriCallback, this,_1, _2));
                        }
                        fileIdAvailable = true;
                        break;
                     }
                  }
               }
               else
               {
                  fileIdAvailable = false;
               }

               if (fileIdAvailable)
               {
                  reply(MMPresTypes::eCallResult::CALLRESULT_OK);
                  //Check if same track has been selected. ELVIS #2407088
                  if (REMOTE_CD_DEVICE_ID == data->mMediumId)
                  {
                     mRemoteCDPlayBackEngine->publishNowPlayingInfoIfNeeded(itemData.mItemId);
                  }
                  else if (true == bCurrentPlayingFileselected)
                  {
                     MMPresTypes::tNowPlayingInfo nowPlayingInfo = pDevice->getNowPlayingInfo();
                     //Publish the current existing metadata 
                     std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayer->getNowPlayingDataStubCallback();
                     if (nowPlayingCallback != nullptr)
                     {
                        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::playSession - sending nowPlaying event to HMI audio/video playback");
                        nowPlayingCallback(nowPlayingInfo);
                     }
                  }
                  else
                  {
                     //Do Nothing
                  }
               }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession,fileId=%llu is not available", fileId);
                  reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR);
               }
            }
            else
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-mediumID=%llu not found", data->mMediumId);
               reply(MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED);
               session->removeQueryFromCache(currQueryId);
            }
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession,current query data not available");
            reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
         }
      } // End of "if (fileSelectionitr != filterIDs.end())"
      else if (playallSelectionitr != filterIDs.end())
      {
         if(fileId == FOLDERID)
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-currQueryId=%lu ,SessionID = %lu fileID = %lu", currQueryId,sessionID,fileId);
            bool isreplySent = false;
            bool isRequestValid = getPlayAllInfoForFS(sessionID, reply, isreplySent);
            if (!isreplySent)
            {
            	if (isRequestValid)
            	{
            		reply(MMPresTypes::eCallResult::CALLRESULT_OK);
            	}
            	else
            	{
            		reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
            	}
            }
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession,filterIDMap is empty");
         reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
      }
   } // End of "if (session != nullptr)"
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::playSession-sessionID=%lu not found", sessionID);
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
   }
}

bool MediaBrowserStubImpl::getPlayAllInfoForFS(const uint32_t sessionID, playSessionReply_t reply, bool& isreplySent)
{
   bool ret = false;
   Session* session = mManager->getSession(sessionID);
   uint32_t currQueryId = 0;
   if (session != nullptr)
   {
      currQueryId = session->getCurrentQueryId();
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::getPlayAllInfoForFS-currQueryId=%lu", currQueryId);
      Session::SessionQueryData* data = session->getSessionQueryData(currQueryId);
      if (data != NULL)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::getPlayAllInfoForFS-URI =%s", data->mPlayURI.c_str());
         DeviceBase* pDevice = mDetect->getActiveDevice();
         std::string path = "";
         if (pDevice != NULL)
         {
            if (data->mPlayURI.empty())
            {
               //Audio Root Folder browsing
             if ( data->mMediumId != REMOTE_CD_DEVICE_ID )
               {
                   if (0 == data->mContentMask)
                   {
                       pDevice->playDevice(MMPresTypes::eMetadataType::METADATATYPE_TITLE);
                   }
                   else //Video Root Folder browsing
                   {
                      pDevice->playDevice(MMPresTypes::eMetadataType::METADATATYPE_VIDEO);
                   }
                }
            else
                {
                  LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d playFolder for remoteCD %d \n", __FUNCTION__, __LINE__,  mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                  LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d playFolder for remoteCD %d != 1 && %d != %d\n", __FUNCTION__, __LINE__,  mRemoteCDPlayBackEngine-> getCurrentFolder (  ), mRemoteCDPlayBackEngine-> getCurrentFolder (  ) , mRemoteCDPlayBackEngine-> getActiveFolderNumber () );
                     if ( ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getCurrentTrack() == 1 ) && ( mRemoteCDPlayBackEngine-> getCurrentFolder (  ) == mRemoteCDPlayBackEngine-> getActiveFolderNumber () ) )
                     {
                         FacadeHelper<MediaPlaybackEngine> mPlayEngine;
                         std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();

                         if (nowPlayingCallback != nullptr)
                         {

                             LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Sending NowPlaying Data for fileName  %d \n", __FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine.getFid () );
                             reply(MMPresTypes::eCallResult::CALLRESULT_OK);
                             isreplySent =  true;
                             nowPlayingCallback( mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine );
                         }
                     }
                     LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Calling setPlayItem for Folder  %d \n", __FUNCTION__, __LINE__,  mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                     mRemoteCDPlayBackEngine->setPlayItem(ePlayFolder, mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                }
            }
            else
            {
                if ( data->mMediumId != REMOTE_CD_DEVICE_ID )
                 {
                     pDevice->playDevice(MMPresTypes::eMetadataType::METADATATYPE_FOLDER, data->mPlayURI.c_str());
                     path.append(data->mBreadCrumbPath);
                     std::size_t found = path.find_last_of("/\\");
                     std::string file = path.substr(found + 1);
                     path = file;
                     pDevice->setBreadCrumbForSetSession(path);
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl:getPlayAllInfoForFS- for path=%s", path.c_str());
                 }
                 else
                 {
                     LOG_INFO(LOG_CONTEXT_MEDIA, "%s %dplayFolder for remoteCD %d \n", __FUNCTION__, __LINE__,  mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                     if (  ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getCurrentTrack() == 1 ) && ( mRemoteCDPlayBackEngine-> getCurrentFolder (  ) == mRemoteCDPlayBackEngine-> getActiveFolderNumber () ) )
                     {
                         FacadeHelper<MediaPlaybackEngine> mPlayEngine;
                         std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();

                         if (nowPlayingCallback != nullptr)
                         {

                             LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Sending NowPlaying Data for fileName  %d \n", __FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine.getFid () );
                             reply(MMPresTypes::eCallResult::CALLRESULT_OK);
                             isreplySent =  true;
                             nowPlayingCallback( mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine );
                         }

                     }
                     LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Calling setPlayItem for Folder  %d \n", __FUNCTION__, __LINE__,  mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                     mRemoteCDPlayBackEngine->setPlayItem(ePlayFolder, mRemoteCDPlayBackEngine-> getCurrentFolder (  ) );
                 }
            }
            ret = true;
         }
      }
   }
   return ret;
}

void MediaBrowserStubImpl::enqueueUriCallback(const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError& error)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::enqueueUriCallback-Error=%lu", (int)error);
}


void MediaBrowserStubImpl::gotoTrack(const std::shared_ptr<CommonAPI::ClientId> _client, uint64_t trackPosition, gotoTrackReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoTrack-trackPosition=%llu ", trackPosition);
   DeviceBase* pActDevice = mDetect->getActiveDevice();

   if (nullptr != pActDevice)
   {
      pActDevice->gotoTrack(trackPosition);
      reply(MMPresTypes::eCallResult::CALLRESULT_OK);
   }
   else
   {
      reply(MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER);
   }
}

void MediaBrowserStubImpl::gotoNowplayingLevel(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint8_t slotId, gotoNowplayingLevelReply_t reply)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::gotoNowplayingLevel-sessionID=%lu", sessionID);
}

void MediaBrowserStubImpl::fireSessionQueryWindowUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)
{
   MediaBrowserStubDefault::fireSessionQueryWindowUpdateSelective(_sessionID, _sessionQueryID, _status, _startIndex, _numItems, _itemList, _windowContainsCurrentlyPlayingTrack, _itemIndexTobeHighlighted);
}

void MediaBrowserStubImpl::fireThumbnailListUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)
{
    MediaBrowserStubDefault::fireThumbnailListUpdateSelective(_sessionID, _sessionQueryID,  _startIndex, _numItems,_thumbnaiList);
}

void MediaBrowserStubImpl::fireSessionQueryAlphaJumpUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::fireSessionQueryAlphaJumpUpdateSelective-sessionID=%lu, sessionQueryID=%lu",_sessionID, _sessionQueryID);
   MediaBrowserStubDefault::fireSessionQueryAlphaJumpUpdateSelective(_sessionID, _sessionQueryID, _ajList);
}


void MediaBrowserStubImpl::openuriCallback(const uint64_t mediumID,const uint32_t sessionID,const uint32_t queryId,const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::openuriCallback-mediumID:%llu,sessionID=%lu,queryId=%lu,callstatus:%d error:%d", mediumID, sessionID, queryId,(int)status, (int)error);
   if (status == CommonAPI::CallStatus::SUCCESS && error == PlayerTypes::PlayerError::NO_ERROR)
   {
      //FacadeHelper<MediaPresSettings> mSettings;
      if (mSettings->getBrowseHistoryEnumeration())
      {
         DeviceBase* pDevice = mDetect->getDevice(mediumID);
         if (nullptr != pDevice)
         {
            Session* session = mManager->getSession(sessionID);
            Session::SessionQueryData* data = nullptr;
            std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator mapitr;
            if (session != nullptr)
            {
               data = session->getSessionQueryData(queryId);
               if (data != nullptr)
               {
                  switch (data->mQueryType)
                  {
                    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
                    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::openuriCallback-setting play plan query id to %lu", queryId);
                     pDevice->setPlayPlanQueryId(queryId);
                     session->insertToPlayPlanDeviceList(mediumID);
                     pDevice->contructPlayPlan(session->getQueryList());
                     break;
                    default:
                     break;
                  }
               }
            }
         }
      }
   }
}

bool MediaBrowserStubImpl::checkForPersistedPlayPlan(uint32_t sessionID)
{
 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkForPersistedPlayPlan-sessionID=%lu",sessionID);
 Session* session = mManager->getSession(sessionID);
 bool ret = false;
 if (session != nullptr)
 {
    std::list<uint64_t> playPlanDevList = session->getPlayPlanDeviceList();
    if (playPlanDevList.size() > 0)
    {
       ret = true;
    }
 }
 return ret;
}

void MediaBrowserStubImpl::checkforclearingSessionCache(uint32_t sessionID, uint64_t mediumID,::v3::com::harman::mmpres::MMPresTypes::FilterIDMap map,MMPresTypes::eSessionQueryTypes type, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filterMap)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCache");
   //FacadeHelper<MediaPresSettings> mSettings;
   bool clearCache = false;
   if (mSettings->getBrowseHistoryEnumeration())
   {
      checkforclearingSessionCacheWithBrowseEnumeration(sessionID, mediumID, map, type, filterMap);
   }
   else
   {
      Session* session = mManager->getSession(sessionID);
      if (session != nullptr)
      {
         if (!map.empty())
         {
            std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;
            itr = map.begin();
            while (itr != map.end())
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCache-filter key=%lu,filter value=%llu", (itr->first), (itr->second));
               if ((itr->second) == 0)  // if any of the input is 0, it means consider the same as root level
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCache-one of the filter input is 0...so root level");
                  clearCache = true;
                  break;
               }
               itr++;
            }
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCache:mFilterIDMap is empty...so root level");
            clearCache = true;
         }

         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCache-clearCache=%d", clearCache);
         if (clearCache)
         {
            session->clearSessionCache();
         }
      }
   }
}

void MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration(uint32_t sessionID, uint64_t mediumID, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap map, MMPresTypes::eSessionQueryTypes type, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filterMap)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration");
   bool clearCache = false;
   Session* session = mManager->getSession(sessionID);

   if (session != nullptr)
   {
         bool status = checkForPersistedPlayPlan(sessionID);
         if (status)
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-persisted play plans present...so don't clear cache");
            bool clearList = false;
            if (!map.empty())
            {
               std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;
               itr = map.begin();
               while (itr != map.end())
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-filter key=%lu,filter value=%llu", (itr->first), (itr->second));
                  if ((itr->second) == 0)  // if any of the input is 0, it means consider the same as root level
                  {
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-one of the filter input is 0...so root level");
                     clearList = true;
                     break;
                  }
                  itr++;
               }
            }
            else
            {
               clearList = true;

               /* Below condition is for smart search. During smart search do not clear previous session cache i.e. without smart search specific.
                * For smart search specific QueryId, we are not adding such queryId into QueryList for all metadata categories
                * For smart search, below are various conditions
                * 1. After selection of smart search button, HMI call setSessionQuery() with filterIDmap & filterMap as empty. For all leveles of smart search
                * 2. Once available characters are displayed on keyboard, user presses any valid character, HMI calls setSessionQuery() with filterIDMap as empty, but
                * filterMap will not be empty.
                */
               Session::SessionQueryData* prevData = nullptr;
               uint32_t currQueryId = session->getCurrentQueryId();
               prevData = session->getSessionQueryData(currQueryId);

            if(prevData != nullptr)
            {
         	   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration currQueryId=%lu, prevData->mQueryType = %d ",currQueryId, prevData->mQueryType);
               DeviceBase* pDevice = mDetect->getActiveDevice();
               if(pDevice!=NULL)
               {
         	      if((MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH != pDevice->getDeviceType())&&
                     (map.empty()) && ((prevData->mQueryType != MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE) || !filterMap.empty()))
         		   clearList = false;
               }
            }
         }
         if (clearList)
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-clear query list");
            session->clearQueryList();
            session->setCurrentQueryId(0);
         }
      }
      else
      {
         if (!map.empty())
         {
            std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;
            itr = map.begin();
            while (itr != map.end())
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-filter key=%lu,filter value=%llu", (itr->first), (itr->second));
               if ((itr->second) == 0)  // if any of the input is 0, it means consider the same as root level
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-one of the filter input is 0...so root level");
                  clearCache = true;
                  break;
               }
               itr++;
            }
         }
         else
         {
         	clearCache = true;

            	/* Below condition is for smart search. During smart search do not clear previous session cache i.e. without smart search specific.
            	 * For smart search specific QueryId, we are not adding such queryId into QueryList for all metadata categories
            	 * For smart search, below are various conditions
            	 * 1. After selection of smart search button, HMI call setSessionQuery() with filterIDmap & filterMap as empty. For all leveles of smart search
            	 * 2. Once available characters are displayed on keyboard, user presses any valid character, HMI calls setSessionQuery() with filterIDMap as empty, but
            	 * filterMap will not be empty.
            	 */
            	Session::SessionQueryData* prevData = nullptr;
            	uint32_t currQueryId = session->getCurrentQueryId();
            	prevData = session->getSessionQueryData(currQueryId);

         	if(prevData != nullptr)
         	{
         	   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration currQueryId=%lu, prevData->mQueryType = %d ",currQueryId, prevData->mQueryType);
               DeviceBase* pDevice = mDetect->getActiveDevice();
               if(pDevice!=NULL)
               {
         		   if((MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH != pDevice->getDeviceType())&&(map.empty()) && 
                      ((prevData->mQueryType != MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE) || !filterMap.empty()))
         		   {
         			   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration:mFilterIDMap is empty...so root level");
         			   clearCache = false;
         		   }
               }
         	}
         }
      }

         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::checkforclearingSessionCacheWithBrowseEnumeration-clearCache=%d", clearCache);
      if (clearCache)
      {
       session->clearSessionCache();
      }
   }
}

void MediaBrowserStubImpl::setSearchCandidatesAttribute(std::vector<std::string> _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::setSearchCandidatesAttribute - firing SearchCandidatesAttribute");

   auto stubAdapter = CommonAPI::Stub<MediaBrowserStubAdapter, MediaBrowserStubRemoteEvent>::stubAdapter_.lock();
   if (stubAdapter)
         stubAdapter->fireSearchCandidatesAttributeChanged(_value);

    mSearchCandidatesAttributeValue = std::move(_value);
}

const std::vector<std::string>& MediaBrowserStubImpl::getSearchCandidatesAttribute()
{
   return mSearchCandidatesAttributeValue;
}
void MediaBrowserStubImpl::getElevatorBarHelperLetterTimeout(const std::shared_ptr<CommonAPI::ClientId> _client, getElevatorBarHelperLetterTimeoutReply_t reply)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::getElevatorBarHelperLetterTimeout");

	//FacadeHelper<MediaPresSettings> mSettings;
	float timeout = mSettings->getElevatorBarHelperLetterTimeout();

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::getElevatorBarHelperLetterTimeout - timeout = %f", timeout);

	reply(timeout);
}

void MediaBrowserStubImpl::setItemIndexTobeHighlightedAttribute(MMPresTypes::HighlightItemIndexMap _value)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowserStubImpl::setItemIndexTobeHighlightedAttribute - firing ItemIndexTobeHighlightedAttribute");

	auto stubAdapter = CommonAPI::Stub<MediaBrowserStubAdapter, MediaBrowserStubRemoteEvent>::stubAdapter_.lock();
	if (stubAdapter)
		stubAdapter->fireItemIndexTobeHighlightedAttributeChanged(_value);

	mItemIndexTobeHighlightedAttribute = std::move(_value);
}

const MMPresTypes::HighlightItemIndexMap& MediaBrowserStubImpl::getItemIndexTobeHighlightedAttribute() {
    return mItemIndexTobeHighlightedAttribute;
}

} // namespace mmpres
} // namespace harman
} // namespace com
} // namespace v3
