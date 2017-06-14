#ifndef DMMTUNERANNOUNCEMENTSTUB_H
#define DMMTUNERANNOUNCEMENTSTUB_H
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


#include "DMMTunerAnnouncement.h"

#ifndef TDMMTunerAnnouncementStub
#define TDMMTunerAnnouncementStub struct _DMMTunerAnnouncementStub
#endif

DSI_EXTERN_C_BEGIN

/**
 * DSI Stub: DMMTunerAnnouncement
 *
 * Tuner generic and project independent announcement interface. Via this interface announcements are indicated and can be controlled.
 */
typedef struct _DMMTunerAnnouncementStub
{
   DSIServer server ;

   /**
    * Implementation version of this server
    */
   DSIVersionInfo implVersion ;

   /**
    * List of current announcement filters
    */
   DMMTunerAnnouncement_Filters filters ;

   /**
    * List of currently available announcements (and announcements where additional information is available even when not active, respectively).
    */
   DMMTunerAnnouncement_Announcements announcements ;

   /**
    * This attribute is only needed for handler components (announcement master) which control the announcements of/between different tuner devices: Current station, incl. if currently active  or not
    * 
    * DEPRECATED: Use tunerState instead
    */
   DMMTunerAnnouncement_Station currentStation ;

   /**
    * This attribute is only needed for handler components (announcement master) which control the announcements of/between different tuner devices: Current station and bitmask which represents the current tuner state
    */
   DMMTunerAnnouncement_TunerState tunerState ;


   /**
    * Set the filter for one announcement of one tuner device (enable/disable; priority).
    */
   void (*fnRequestSetFilter)( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Filter* filter,  unsigned int handle );

   /**
    * Abort an active announcement
    */
   void (*fnRequestAbort)( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_StationSelector* source,  DMMTunerAnnouncement_AnnouncementType type,  unsigned int handle );

   /**
    * Starts tuning a frequency or selecting a station. 
    * NOTE: Use this method ONLY if YOUR component is in charge to tune to an active announcement, usually this is done by the tuner itself or by the announcement master component.  
    */
   void (*fnRequestStartTune)( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_StationSelector* station,  DSIBool blockStation,  unsigned int handle );

   /**
    * Device shall unblock new tune requests via DMMTunerStation interface and shall also restore latest requested station.
    * NOTE: Use this method ONLY if YOUR component is in charge to tune to an active announcement, usually this is done by the tuner itself or by the announcement master component.
    */
   void (*fnRequestUnblockStation)( TDMMTunerAnnouncementStub* stub );

   /**
    * Like setFilter but with the capability to set several filters at once
    */
   void (*fnRequestSetFilters)( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Filters filters,  unsigned int handle );

   void (*fnRequestSetFiltersEx)( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Filter* filters, int filtersSize,  unsigned int handle );


   /**
    * Called when the stub received an internal event
    */
   void (*fnProcessInternalEvent) ( TDMMTunerAnnouncementStub* stub, int id, void* data ) ;

   /**
    * Called right before the stub thread ends
    */
   void (*fnCleanup) ( TDMMTunerAnnouncementStub* stub ) ;

} DMMTunerAnnouncementStub ;

/**
 * Result of filter setting request. This response is only sent if handle of setFilter is not equal to zero (notifcation has to be set explicitly).
 */
void DMMTunerAnnouncement_ResponseSetFilter( TDMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback );

/**
 * Result of abort request. This response is only sent if handle of abort is not equal to zero (notifcation has to be set explicitly).
 */
void DMMTunerAnnouncement_ResponseAbort( TDMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback );

/**
 * Result of startTune() request
 */
void DMMTunerAnnouncement_ResponseStartTune( TDMMTunerAnnouncementStub* stub,  unsigned int handle,  DMMTunerTypes_Feedback feedback );

/**
 * Current announcement information
 */
void DMMTunerAnnouncement_InformationCurrentAnnouncement( TDMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Announcement* announcement );

/**
 * Result of filters setting request. This response is only sent if handle of setFilters is not equal to zero (notifcation has to be set explicitly).
 */
void DMMTunerAnnouncement_ResponseSetFilters( TDMMTunerAnnouncementStub* stub,  unsigned int handle, const DMMTunerTypes_Feedbacks feedbacks );

void DMMTunerAnnouncement_ResponseSetFiltersEx( TDMMTunerAnnouncementStub* stub,  unsigned int handle, const DMMTunerTypes_Feedback* feedbacks, int feedbacksSize );


/**
 * Sends a notification to all clients that are interested.
 */
void DMMTunerAnnouncementStub_SendNotification( TDMMTunerAnnouncementStub* stub, DMMTunerAnnouncement_UpdateIdEnum id );

/**
 * Initailzes the stub
 */
int DMMTunerAnnouncementStub_Init( TDMMTunerAnnouncementStub* stub, const char* rolename );

/**
 * Frees the stub
 */
void DMMTunerAnnouncementStub_Free( TDMMTunerAnnouncementStub* stub );

/**
 * Starts the DMMTunerAnnouncement stub and enters the message loop. This functions
 * does not return until void DMMTunerAnnouncementStubStop() is called or an error
 * during startup occures.
 */
int DMMTunerAnnouncementStub_Start( TDMMTunerAnnouncementStub* stub, const char* rolename );

/**
 * Stops the DMMTunerAnnouncement stub. This function ends the stubs message loop.
 * DMMTunerAnnouncementStub_Start() returns the exitcode.
 */
int DMMTunerAnnouncementStub_Stop( TDMMTunerAnnouncementStub* stub, int exitcode );

/**
 * Respond with an error message.
 */
void DMMTunerAnnouncementSendError( TDMMTunerAnnouncementStub* stub, DMMTunerAnnouncement_UpdateIdEnum id );

/**
 * Posts an internal event in the stubs message queue and returns immediately.
 */
int DMMTunerAnnouncementStub_PostInternalEvent( TDMMTunerAnnouncementStub* stub, int code, void* data );

/**
 * Send an internal event to the stub and waits until the stub handled it. Do not call this
 * function from within the stubs task. This will lead into a dead lock.
 */
int DMMTunerAnnouncementStub_SendInternalEvent( TDMMTunerAnnouncementStub* stub, int code, void* data );

#define DMMTunerAnnouncementStub_RegisterCurrentSession( stub ) DSIServerRegisterCurrentSession( &stub->server )
#define DMMTunerAnnouncementStub_ClearActiveSessions( stub ) DSIServerClearActiveSessions( &stub->server )
#define DMMTunerAnnouncementStub_AddActiveSession( stub, sessionId ) DSIServerAddActiveSession( &stub->server, sessionId )
#define DMMTunerAnnouncementStub_IsSessionActive( stub, sessionId ) DSIServerIsSessionActive( &stub->server, sessionId )

#define DMMTunerAnnouncementStub_UnblockRequest( stub ) DSIServerUnblockRequest( &stub->server )
#define DMMTunerAnnouncementStub_PrepareResponse( stub, sessionHandle ) DSIServerPrepareResponse( &stub->server, sessionHandle )

DSI_EXTERN_C_END

#endif // DMMTUNERANNOUNCEMENTSTUB_H
