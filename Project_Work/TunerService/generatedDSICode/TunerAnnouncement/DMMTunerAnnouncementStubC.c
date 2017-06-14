/**********************************************************************
 *  Project       Harman Car Multimedia System
 *  (c) copyright 2013
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 **********************************************************************/

/*
 * Generated by DSI Generator version 2.0
 * Date: 6.5.2013
 */

struct _DMMTunerAnnouncementStub ;
#define TDSIServer struct _DMMTunerAnnouncementStub

#define DSICONST

#include "DMMTunerAnnouncementStub.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif

static const char* SERVER_NAME = "DMMTunerAnnouncement" ;
static const int SERVER_MAJOR_VERSION = 1;
static const int SERVER_MINOR_VERSION = 2 ;

static int WriteAttribute( DMMTunerAnnouncementStub* stub, DSIStream* stream, unsigned int id, DSIUpdateType type, short position, short count )
{
   int success = 0 ;

   // avoid unused variables
   (void)type;(void)position;(void)count;

   switch( (DMMTunerAnnouncement_UpdateIdEnum)id )
   {
   case DMMTunerAnnouncement_UPD_ID_filters:
      DSIWriteDMMTunerAnnouncement_Filters( stream, &stub->filters );
      success = 1 ;
      break;

   case DMMTunerAnnouncement_UPD_ID_announcements:
      DSIWriteDMMTunerAnnouncement_Announcements( stream, &stub->announcements );
      success = 1 ;
      break;

   case DMMTunerAnnouncement_UPD_ID_currentStation:
      DSIWriteDMMTunerAnnouncement_Station( stream, &stub->currentStation );
      success = 1 ;
      break;

   case DMMTunerAnnouncement_UPD_ID_tunerState:
      DSIWriteDMMTunerAnnouncement_TunerState( stream, &stub->tunerState );
      success = 1 ;
      break;

   case DMMTunerAnnouncement_UPD_ID_implVersion:
      DSIWrite( stream, &stub->implVersion, sizeof(stub->implVersion), 1 );
      success = 1 ;
      break;
      
   default:
      assert( 0 ) ;  // bad response id
      break;
   }
   return success ;
}




static void ProcessRequest( DMMTunerAnnouncementStub* stub, DSIStream* stream, unsigned int id )
{
   switch( (DMMTunerAnnouncement_UpdateIdEnum)id )
   {
   case DMMTunerAnnouncement_UPD_ID_requestSetFilter:
      {
         DMMTunerAnnouncement_Filter filter ;
         unsigned int handle = 0 ;
         memset( &filter, 0, sizeof(filter) )  ;
         DSIReadDMMTunerAnnouncement_Filter( stream, &filter ) ;
         DSIRead32( stream, &handle ) ;
         if( 0 != stub->fnRequestSetFilter )
         {
            stub->fnRequestSetFilter( stub,  &filter, handle );
         }
         DMMTunerAnnouncement_Filter_Free( &filter );
      }
      break;

   case DMMTunerAnnouncement_UPD_ID_requestAbort:
      {
         DMMTunerAnnouncement_StationSelector source ;
         DMMTunerAnnouncement_AnnouncementType type = 0 ;
         unsigned int handle = 0 ;
         memset( &source, 0, sizeof(source) )  ;
         DSIReadDMMTunerAnnouncement_StationSelector( stream, &source ) ;
         DSIRead32( stream, &type ) ;
         DSIRead32( stream, &handle ) ;
         if( 0 != stub->fnRequestAbort )
         {
            stub->fnRequestAbort( stub,  &source, type, handle );
         }
         DMMTunerAnnouncement_StationSelector_Free( &source );
      }
      break;

   case DMMTunerAnnouncement_UPD_ID_requestStartTune:
      {
         DMMTunerAnnouncement_StationSelector station ;
         DSIBool blockStation = 0 ;
         unsigned int handle = 0 ;
         memset( &station, 0, sizeof(station) )  ;
         DSIReadDMMTunerAnnouncement_StationSelector( stream, &station ) ;
         DSIReadBool( stream, &blockStation ) ;
         DSIRead32( stream, &handle ) ;
         if( 0 != stub->fnRequestStartTune )
         {
            stub->fnRequestStartTune( stub,  &station, blockStation, handle );
         }
         DMMTunerAnnouncement_StationSelector_Free( &station );
      }
      break;

   case DMMTunerAnnouncement_UPD_ID_requestUnblockStation:
      {
         if( 0 != stub->fnRequestUnblockStation )
         {
            stub->fnRequestUnblockStation( stub );
         }
      }
      break;

   case DMMTunerAnnouncement_UPD_ID_requestSetFilters:
      {
         DMMTunerAnnouncement_Filters filters = 0 ;
         unsigned int handle = 0 ;
         DSIReadDMMTunerAnnouncement_Filters( stream, &filters ) ;
         DSIRead32( stream, &handle ) ;
         if( 0 != stub->fnRequestSetFilters )
         {
            stub->fnRequestSetFilters( stub, filters, handle );
         }
         if( 0 != stub->fnRequestSetFiltersEx )
         {
            stub->fnRequestSetFiltersEx( stub, DSIVectorData( filters ), DSIVectorSize( filters ), handle );
         }
         DMMTunerAnnouncement_Filters_Free( &filters );
      }
      break;

   default:
      break;
   }
}




void DMMTunerAnnouncement_ResponseSetFilter( DMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &handle ) ;
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_responseSetFilter, &stream );
   DSIClose( &stream );
}

void DMMTunerAnnouncement_ResponseAbort( DMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &handle ) ;
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_responseAbort, &stream );
   DSIClose( &stream );
}

void DMMTunerAnnouncement_ResponseStartTune( DMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &handle ) ;
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_responseStartTune, &stream );
   DSIClose( &stream );
}

void DMMTunerAnnouncement_InformationCurrentAnnouncement( DMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Announcement* announcement )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMTunerAnnouncement_Announcement( &stream, announcement ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_informationCurrentAnnouncement, &stream );
   DSIClose( &stream );
}

void DMMTunerAnnouncement_ResponseSetFilters( DMMTunerAnnouncementStub* stub,  unsigned int handle, const DMMTunerTypes_Feedbacks feedbacks )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &handle ) ;
   DSIWriteDMMTunerTypes_Feedbacks( &stream, &feedbacks ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_responseSetFilters, &stream );
   DSIClose( &stream );
}

void DMMTunerAnnouncement_ResponseSetFiltersEx( DMMTunerAnnouncementStub* stub,  unsigned int handle, const DMMTunerTypes_Feedback* feedbacks, int feedbacksSize )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &handle ) ;
   DSIWrite32( &stream, &feedbacksSize );
   if( 0 != feedbacksSize )
   {
      int idx=0;
      while( idx < feedbacksSize )
      {
         DSIWrite32( &stream, &feedbacks[idx] ) ;
         idx++;
      }
   }
   DSIServerSendResponse( stub, RESULT_OK, DMMTunerAnnouncement_UPD_ID_responseSetFilters, &stream );
   DSIClose( &stream );
}


static void ProcessInternalEvent( DMMTunerAnnouncementStub* stub, int code, void* data )
{
   if( stub->fnProcessInternalEvent )
   {
      stub->fnProcessInternalEvent( stub, code, data );
   }
}

void DMMTunerAnnouncementStub_SendNotification( DMMTunerAnnouncementStub* stub, DMMTunerAnnouncement_UpdateIdEnum id )
{
   DSIServerSendNotification( stub, id );
}

static void Cleanup( DMMTunerAnnouncementStub* stub )
{
   if( stub->fnCleanup )
   {
      stub->fnCleanup( stub );
   }
}

static int GetResponse( DMMTunerAnnouncementStub* stub, int requestId )
{
   DMMTunerAnnouncement_UpdateIdEnum responseId = DMMTunerAnnouncement_UPD_ID_NOP ;


   return (responseId == DMMTunerAnnouncement_UPD_ID_NOP) ? DSI_INVALID_ID : (int)responseId ;
}

int DMMTunerAnnouncementStub_Init( DMMTunerAnnouncementStub* stub, const char* rolename )
{
   DSIInit();
   memset( stub, 0, sizeof(*stub));
   DSIServerInit( stub, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   stub->server.fnWriteAttribute = WriteAttribute ;
   stub->server.fnProcessRequest = ProcessRequest ;
   stub->server.fnProcessInternalEvent = ProcessInternalEvent ;
   stub->server.fnCleanup = Cleanup ;
   stub->server.fnGetResponse = GetResponse ;
   return 0;
}

void DMMTunerAnnouncementStub_Free( DMMTunerAnnouncementStub* stub  )
{
   DMMTunerAnnouncement_Filters_Free( &stub->filters );
   DMMTunerAnnouncement_Announcements_Free( &stub->announcements );
   DMMTunerAnnouncement_Station_Free( &stub->currentStation );
   DMMTunerAnnouncement_TunerState_Free( &stub->tunerState );
   DSIServerFree( stub );
   memset( stub, 0, sizeof(*stub) );
}

int DMMTunerAnnouncementStub_Start( DMMTunerAnnouncementStub* stub, const char* rolename )
{
   DSIInit();
   // check if all request function pointers are valid
   assert( 0 != stub->fnRequestSetFilter  );
   assert( 0 != stub->fnRequestAbort  );
   assert( 0 != stub->fnRequestStartTune  );
   assert( 0 != stub->fnRequestUnblockStation  );
   assert( 0 != stub->fnRequestSetFilters || 0 != stub->fnRequestSetFiltersEx );

   DSIServerInit( stub, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   stub->server.fnWriteAttribute = WriteAttribute ;
   stub->server.fnProcessRequest = ProcessRequest ;
   stub->server.fnProcessInternalEvent = ProcessInternalEvent ;
   stub->server.fnCleanup = Cleanup ;
   stub->server.fnGetResponse = GetResponse ;
   return DSIServerRun( stub );
}


int DMMTunerAnnouncementStub_Stop( DMMTunerAnnouncementStub* stub, int exitcode )
{
	return DSIServerStop( stub, exitcode );
}


void DMMTunerAnnouncementSendError( DMMTunerAnnouncementStub* stub, DMMTunerAnnouncement_UpdateIdEnum id )
{
   DSIStream stream;
   DSIResultType typ = RESULT_REQUEST_ERROR;

   if ( IS_RESPONSE_ID(id) )
      typ = RESULT_INVALID;
   else if ( IS_ATTRIBUTE_ID(id) )
      typ = RESULT_DATA_INVALID;

   DSIOpen( &stream );
   DSIServerSendResponse( stub, typ, id, &stream );
   DSIClose( &stream );
}


int DMMTunerAnnouncementStub_PostInternalEvent( DMMTunerAnnouncementStub* stub, int code, void* data )
{
	return DSIPostInternalEvent( stub->server.Channel.Master, stub->server.Id, 0, code, data );
}


int DMMTunerAnnouncementStub_SendInternalEvent( DMMTunerAnnouncementStub* stub, int code, void* data )
{
	return DSISendInternalEvent( stub->server.Channel.Master, stub->server.Id, 0, code, data );
}


