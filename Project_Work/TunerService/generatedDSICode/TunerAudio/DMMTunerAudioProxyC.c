/**********************************************************************
 *  Project       Harman Car Multimedia System
 *  (c) copyright 2015
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 **********************************************************************/

/*
 * Generated by DSI Generator version 2.0
 * Date: 15.1.2015
 */


struct _DMMTunerAudioProxy ;
#define TDSIClient struct _DMMTunerAudioProxy

/*
 * Remove the const from the attributes for this module.
 */
#define DSICONST


#include "DMMTunerAudioProxy.h"

static const char* SERVER_NAME = "DMMTunerAudio" ;
static const int SERVER_MAJOR_VERSION = 1 ;
static const int SERVER_MINOR_VERSION = 1 ;


static void DMMTunerAudioReadErrorEnumerator( DMMTunerAudioProxy* proxy, DSIStream* stream )
{
   (void)proxy;
}


static void ProcessResponse( DMMTunerAudioProxy* proxy, DSIStream* stream, const EventInfo* event )
{
    if ((event == 0) || (proxy == 0))
      return;
   if( event->responseType == RESULT_INVALID )
   {
      DMMTunerAudioReadErrorEnumerator( proxy, stream );

      if( proxy->fnResponseInvalid )
      {
         proxy->fnResponseInvalid( proxy, (DMMTunerAudio_UpdateIdEnum)event->requestID );
      }
   }
   else if( event->responseType == RESULT_REQUEST_ERROR
           || event->responseType == RESULT_REQUEST_BUSY
           || event->responseType == RESULT_NOP )   // FIXME remove when MoCCA is fixed
   {
      DMMTunerAudioReadErrorEnumerator( proxy, stream );

      switch( event->requestID )
      {
      case DMMTunerAudio_UPD_ID_requestSetSourceActivity:
         if( proxy->fnRequestSetSourceActivityFailed )
         {
            proxy->fnRequestSetSourceActivityFailed( proxy, (DSIResultType)event->responseType );
         }
         break;

      case DMMTunerAudio_UPD_ID_requestEnableSourceMute:
         if( proxy->fnRequestEnableSourceMuteFailed )
         {
            proxy->fnRequestEnableSourceMuteFailed( proxy, (DSIResultType)event->responseType );
         }
         break;

      case DMMTunerAudio_UPD_ID_requestSetCurrentSinks:
         if( proxy->fnRequestSetCurrentSinksFailed )
         {
            proxy->fnRequestSetCurrentSinksFailed( proxy, (DSIResultType)event->responseType );
         }
         break;

      default:
         assert( 0 ); //lint !e506 Constant value Boolean
         break;
      }
   }
   else
   {
      int bSendEvent = 0 ;
      switch( event->requestID )
      {
      case DMMTunerAudio_UPD_ID_audioAvailability:
         {

            // set attribute state
            DSI_SETSTATE( proxy->audioAvailability, event->responseType == RESULT_DATA_OK ? DATA_OK : DATA_INVALID );

            // read the attribute data
            if( event->responseType == RESULT_DATA_OK )
            {
               DSIRead32( stream, &proxy->audioAvailability ) ;
            }
            bSendEvent = 1 ;
         }
         break;

      case DMMTunerAudio_UPD_ID_responseSetSourceActivity:
         {
            unsigned int handle = 0 ;
            DMMTunerTypes_Feedback feedback = 0 ;
            DSIRead32( stream, &handle ) ;
            DSIRead32( stream, &feedback ) ;
            if( proxy->fnResponseSetSourceActivity )
            {
               proxy->fnResponseSetSourceActivity( proxy, handle, feedback );
            }
         }
         break;

      case DMMTunerAudio_UPD_ID_responseEnableSourceMute:
         {
            unsigned int handle = 0 ;
            DMMTunerTypes_Feedback feedback = 0 ;
            DSIRead32( stream, &handle ) ;
            DSIRead32( stream, &feedback ) ;
            if( proxy->fnResponseEnableSourceMute )
            {
               proxy->fnResponseEnableSourceMute( proxy, handle, feedback );
            }
         }
         break;

      case DMMTunerAudio_UPD_ID_responseSetCurrentSinks:
         {
            unsigned int handle = 0 ;
            DMMTunerTypes_Feedback feedback = 0 ;
            DSIRead32( stream, &handle ) ;
            DSIRead32( stream, &feedback ) ;
            if( proxy->fnResponseSetCurrentSinks )
            {
               proxy->fnResponseSetCurrentSinks( proxy, handle, feedback );
            }
         }
         break;

      default:
         assert( 0 ); //lint !e506 Constant value Boolean unknown update id
         break;
      }

      if( bSendEvent && proxy->fnProcessUpdateEvent )
      {
         // notify the user about the update
         proxy->fnProcessUpdateEvent( proxy, (DMMTunerAudio_UpdateIdEnum)event->requestID );
      }
   }
}

int DMMTunerAudio_RequestSetSourceActivity( DMMTunerAudioProxy* proxy,  DMMTunerAudio_SourceActivity activity,  unsigned char sourceNumber,  unsigned int handle )
{
   int seqNr = DSICreateId();
   DSIStream stream ;
   DSIOpen( &stream );   
   DSIWrite32( &stream, &activity ) ;   
   DSIWrite8( &stream, &sourceNumber ) ;   
   DSIWrite32( &stream, &handle ) ;
   DSIClientSendRequest( proxy, DMMTunerAudio_UPD_ID_requestSetSourceActivity, seqNr, &stream );
   DSIClose( &stream );
   return seqNr ;
}



int DMMTunerAudio_RequestEnableSourceMute( DMMTunerAudioProxy* proxy,  DSIBool mute,  unsigned int handle )
{
   int seqNr = DSICreateId();
   DSIStream stream ;
   DSIOpen( &stream );   
   DSIWriteBool( &stream, &mute ) ;   
   DSIWrite32( &stream, &handle ) ;
   DSIClientSendRequest( proxy, DMMTunerAudio_UPD_ID_requestEnableSourceMute, seqNr, &stream );
   DSIClose( &stream );
   return seqNr ;
}



int DMMTunerAudio_RequestSetCurrentSinks( DMMTunerAudioProxy* proxy,  DMMTunerAudio_SourceType sourceType, const DMMTunerAudio_SinkTypes sinkTypes,  unsigned int handle )
{
   int seqNr = DSICreateId();
   DSIStream stream ;
   DSIOpen( &stream );   
   DSIWrite32( &stream, &sourceType ) ;   
   DSIWriteDMMTunerAudio_SinkTypes( &stream, &sinkTypes ) ;   
   DSIWrite32( &stream, &handle ) ;
   DSIClientSendRequest( proxy, DMMTunerAudio_UPD_ID_requestSetCurrentSinks, seqNr, &stream );
   DSIClose( &stream );
   return seqNr ;
}

int DMMTunerAudio_RequestSetCurrentSinksEx( DMMTunerAudioProxy* proxy,  DMMTunerAudio_SourceType sourceType, const DMMTunerAudio_SinkType* sinkTypes, int sinkTypesSize,  unsigned int handle )
{
   int seqNr = DSICreateId();
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &sourceType ) ;
   DSIWrite32( &stream, &sinkTypesSize );
   if( (0 != sinkTypesSize ) && (0 != sinkTypes ))
   {
      int idx=0;
      while( idx < sinkTypesSize )
      {
         DSIWrite32( &stream, &sinkTypes[idx] ) ;
         idx++;
      }
   }
   DSIWrite32( &stream, &handle ) ;
   DSIClientSendRequest( proxy, DMMTunerAudio_UPD_ID_requestSetCurrentSinks, seqNr, &stream );
   DSIClose( &stream );
   return seqNr ;
}





/*******************************************************************************************/

static void ComponentConnected( DMMTunerAudioProxy* proxy )
{
   if( proxy && proxy->fnComponentConnected )
   {
      proxy->fnComponentConnected( proxy );
   }
}

static void ComponentDisconnected( DMMTunerAudioProxy* proxy )
{
   if( proxy && proxy->fnComponentDisconnected )
   {
      proxy->fnComponentDisconnected( proxy );
   }
}

static void ProcessInternalEvent( DMMTunerAudioProxy* proxy, int code, void* data )
{
   if( proxy && proxy->fnProcessInternalEvent )
   {
      proxy->fnProcessInternalEvent( proxy, code, data );
   }
}

static void Cleanup( DMMTunerAudioProxy* proxy )
{
   if( proxy && proxy->fnCleanup )
   {
      proxy->fnCleanup( proxy );
   }
}

/*** notifications *************************************************************************/

void DMMTunerAudioProxy_SetNotification( DMMTunerAudioProxy* proxy, DMMTunerAudio_UpdateIdEnum updateId )
{
   DSIClientSetNotification( proxy, (int)updateId );
}


void DMMTunerAudioProxy_ClearNotification( DMMTunerAudioProxy* proxy, DMMTunerAudio_UpdateIdEnum updateId )
{
   DSIClientClearNotification( proxy, (int)updateId );
}


void DMMTunerAudioProxy_ClearAllNotifications( DMMTunerAudioProxy* proxy )
{
   DSIClientClearAllNotifications( proxy );
}

/*** (de)initialization ********************************************************************/

int DMMTunerAudioProxy_Init( DMMTunerAudioProxy* proxy, const char* rolename )
{
   DSIInit();
   if (proxy)
   {
      memset( proxy, 0, sizeof(*proxy) );
   }
   DSIClientInit( proxy, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   if (proxy)
   {
	   proxy->client.fnComponentConnected = ComponentConnected ;
	   proxy->client.fnComponentDisconnected = ComponentDisconnected ;
	   proxy->client.fnProcessResponse = ProcessResponse ;
	   proxy->client.fnProcessInternalEvent = ProcessInternalEvent ;
	   proxy->client.fnCleanup = Cleanup ;
   }
   return 0 ;
}

void DMMTunerAudioProxy_Free( DMMTunerAudioProxy* proxy  )
{
   if (proxy == 0)
      return;

   DSIClientFree( proxy );

   memset( proxy, 0, sizeof(*proxy) );
}

int DMMTunerAudioProxy_Start( DMMTunerAudioProxy* proxy, const char* rolename )
{
   // do not call the _Init function here since the proxy will be NULL'ed which makes problems with
   // the CPP implementation callbacks already registered...

   DSIInit();
   DSIClientInit( proxy, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   if (proxy)
   {
	   proxy->client.fnComponentConnected = ComponentConnected ;
	   proxy->client.fnComponentDisconnected = ComponentDisconnected ;
	   proxy->client.fnProcessResponse = ProcessResponse ;
	   proxy->client.fnProcessInternalEvent = ProcessInternalEvent ;
	   proxy->client.fnCleanup = Cleanup ;
   }
   return DSIClientRun( proxy );
}

int DMMTunerAudioProxy_Stop( DMMTunerAudioProxy* proxy, int exitcode )
{
   return DSIClientStop( proxy, exitcode );
}

int DMMTunerAudioProxy_PostInternalEvent( DMMTunerAudioProxy* proxy, int code, void* data )
{
   if (0 == proxy)
      return 0;
   return DSIPostInternalEvent( proxy->client.Channel.Master, proxy->client.Id, 0, code, data );
}

int DMMTunerAudioProxy_SendInternalEvent( DMMTunerAudioProxy* proxy, int code, void* data )
{
   if (0 != proxy)
   {
      if (proxy->client.SendChid)
      {
          return DSISendInternalEvent( proxy->client.SendChid, proxy->client.Id, 0, code, data );

      }
      else
      {

         return DSISendInternalEvent( proxy->client.Channel.Master, proxy->client.Id, 0, code, data );
      }
   }
   else
   {
      return 0;
   }
}

