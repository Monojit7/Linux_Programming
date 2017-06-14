 
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


#include "CDMMTunerAnnouncementStub.hpp"
#include <string.h>

static void _requestSetFilter( DMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Filter* filter,  unsigned int handle )
{
   ((CDMMTunerAnnouncementStub*)stub)->requestSetFilter(  *filter, handle ) ;
}

static void _requestAbort( DMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_StationSelector* source,  DMMTunerAnnouncement_AnnouncementType type,  unsigned int handle )
{
   ((CDMMTunerAnnouncementStub*)stub)->requestAbort(  *source, type, handle ) ;
}

static void _requestStartTune( DMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_StationSelector* station,  DSIBool blockStation,  unsigned int handle )
{
   ((CDMMTunerAnnouncementStub*)stub)->requestStartTune(  *station, blockStation, handle ) ;
}

static void _requestUnblockStation( DMMTunerAnnouncementStub* stub )
{
   ((CDMMTunerAnnouncementStub*)stub)->requestUnblockStation( ) ;
}

static void _requestSetFilters( DMMTunerAnnouncementStub* stub, const DMMTunerAnnouncement_Filters filters,  unsigned int handle )
{
   ((CDMMTunerAnnouncementStub*)stub)->requestSetFilters( filters, handle ) ;
}

static void _processInternalEvent( DMMTunerAnnouncementStub* stub, int id, void* data )
{
   ((CDMMTunerAnnouncementStub*)stub)->processInternalEvent( id,  data ) ;
}

static void _cleanup( DMMTunerAnnouncementStub* stub )
{
   ((CDMMTunerAnnouncementStub*)stub)->cleanup( ) ;
}

CDMMTunerAnnouncementStub::CDMMTunerAnnouncementStub( )
{
   memset( (DMMTunerAnnouncementStub*)this, 0, sizeof(DMMTunerAnnouncementStub) );
   fnRequestSetFilter = _requestSetFilter ;
   fnRequestAbort = _requestAbort ;
   fnRequestStartTune = _requestStartTune ;
   fnRequestUnblockStation = _requestUnblockStation ;
   fnRequestSetFilters = _requestSetFilters ;
   fnProcessInternalEvent = _processInternalEvent ;
   fnCleanup = _cleanup ;
}

CDMMTunerAnnouncementStub::~CDMMTunerAnnouncementStub()
{
   // DMMTunerAnnouncementStop( this ); not implemented
}

int CDMMTunerAnnouncementStub::start( const char* rolename )
{
   return DMMTunerAnnouncementStub_Start( this, rolename );
}

void CDMMTunerAnnouncementStub::stop( int exitcode )
{
   DMMTunerAnnouncementStub_Stop( this, exitcode );
}

void CDMMTunerAnnouncementStub::sendError( DMMTunerAnnouncement_UpdateIdEnum id )
{
   DMMTunerAnnouncementSendError( this, id );
}

int CDMMTunerAnnouncementStub::postInternalEvent( int code, void* data )
{
   return DSIPostInternalEvent( server.Channel.Master, server.Id, 0, code, data );
}

int CDMMTunerAnnouncementStub::sendInternalEvent( int code, void* data )
{
   return DSISendInternalEvent( server.Channel.Master, server.Id, 0, code, data );
}

void CDMMTunerAnnouncementStub::cleanup()
{
}
