/**********************************************************************
 *  Project       Harman Car Multimedia System
 *  (c) copyright 2015
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 **********************************************************************/

/*
 * Generated by DSI Generator version 2.0
 * Date: 20.5.2015
 */

struct _DMMAmFmTunerControlStub ;
#define TDSIServer struct _DMMAmFmTunerControlStub

#define DSICONST

#include "DMMAmFmTunerControlStub.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif

static const char* SERVER_NAME = "DMMAmFmTunerControl" ;
static const int SERVER_MAJOR_VERSION = 6 ;
static const int SERVER_MINOR_VERSION = 5 ;

static int WriteAttribute( DMMAmFmTunerControlStub* stub, DSIStream* stream, unsigned int id, DSIUpdateType type, short position, short count )
{
   int success = 0 ;
   if (stub == 0)
   {
      return success;
   }

   // avoid unused variables
   (void)type;(void)position;(void)count;

   switch( (DMMAmFmTunerControl_UpdateIdEnum)id )
   {
   case DMMAmFmTunerControl_UPD_ID_afSwitch:
      DSIWriteBool( stream, &stub->afSwitch );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_multipleStationsSwitch:
      DSIWriteBool( stream, &stub->multipleStationsSwitch );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_regMode:
      DSIWrite32( stream, &stub->regMode );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_stereoMode:
      DSIWrite32( stream, &stub->stereoMode );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_stereoState:
      DSIWriteBool( stream, &stub->stereoState );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_hdMode:
      DSIWrite32( stream, &stub->hdMode );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_deviceState:
      DSIWrite32( stream, &stub->deviceState );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_diagSwitch:
      DSIWriteBool( stream, &stub->diagSwitch );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_wavebandGrids:
      DSIWriteDMMAmFmTunerControl_WavebandGrids( stream, &stub->wavebandGrids );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_hwError:
      DSIWriteDMMAmFmTunerControl_HwError( stream, &stub->hwError );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_versions:
      DSIWriteDMMAmFmTunerControl_Strings( stream, &stub->versions );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_codingData:
      DSIWriteDMMAmFmTunerControl_CodingData( stream, &stub->codingData );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_hdBallGameMode:
      DSIWrite32( stream, &stub->hdBallGameMode );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_tunerStateAttr:
      DSIWriteDMMAmFmTunerControl_TunerStates( stream, &stub->tunerStateAttr );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_arbitrationInfo:
      DSIWriteDMMAmFmTunerControl_ArbitrationInfo( stream, &stub->arbitrationInfo );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_rdsClockTime:
      DSIWriteDMMAmFmTunerControl_RdsClockTime( stream, &stub->rdsClockTime );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_powerSync:
      DSIWriteDMMTunerTypes_UInt32s( stream, &stub->powerSync );
      success = 1 ;
      break;

   case DMMAmFmTunerControl_UPD_ID_implVersion:
      DSIWrite( stream, &stub->implVersion, sizeof(stub->implVersion), 1 );
      success = 1 ;
      break;

   default:
      assert( 0 ) ;  //lint !e506 Constant value Boolean  bad response id
      break;
   }
   return success ;
}




static void ProcessRequest( DMMAmFmTunerControlStub* stub, DSIStream* stream, unsigned int id )
{
   if (stub == 0)
   {
      return;
   }
   switch( (DMMAmFmTunerControl_UpdateIdEnum)id )
   {
   case DMMAmFmTunerControl_UPD_ID_requestSetAfSwitch:
      {
         DSIBool enable = 0 ;
         DSIReadBool( stream, &enable ) ;
         if( 0 != stub->fnRequestSetAfSwitch )
         {
            stub->fnRequestSetAfSwitch( stub, enable );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetMultipleStationsSwitch:
      {
         DSIBool enable = 0 ;
         DSIReadBool( stream, &enable ) ;
         if( 0 != stub->fnRequestSetMultipleStationsSwitch )
         {
            stub->fnRequestSetMultipleStationsSwitch( stub, enable );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetRegMode:
      {
         DMMAmFmTunerControl_RegMode regMode = 0 ;
         DSIRead32( stream, &regMode ) ;
         if( 0 != stub->fnRequestSetRegMode )
         {
            stub->fnRequestSetRegMode( stub, regMode );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetStereoMode:
      {
         DMMAmFmTunerControl_StereoMode stereoMode = 0 ;
         DSIRead32( stream, &stereoMode ) ;
         if( 0 != stub->fnRequestSetStereoMode )
         {
            stub->fnRequestSetStereoMode( stub, stereoMode );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetHdMode:
      {
         DMMAmFmTunerControl_HdMode hdMode = 0 ;
         DSIRead32( stream, &hdMode ) ;
         if( 0 != stub->fnRequestSetHdMode )
         {
            stub->fnRequestSetHdMode( stub, hdMode );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetDiagSwitch:
      {
         DSIBool enable = 0 ;
         DSIReadBool( stream, &enable ) ;
         if( 0 != stub->fnRequestSetDiagSwitch )
         {
            stub->fnRequestSetDiagSwitch( stub, enable );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetTestMode:
      {
         DMMAmFmTunerControl_TestMode testMode = 0 ;
         unsigned int index = 0 ;
         DSIRead32( stream, &testMode ) ;
         DSIRead32( stream, &index ) ;
         if( 0 != stub->fnRequestSetTestMode )
         {
            stub->fnRequestSetTestMode( stub, testMode, index );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSendPmBusCommand:
      {
         DSIBuffer commandStream = 0 ;
         DSIReadBuffer( stream, &commandStream ) ;
         if( 0 != stub->fnRequestSendPmBusCommand )
         {
            stub->fnRequestSendPmBusCommand( stub, commandStream );
         }
         if( 0 != stub->fnRequestSendPmBusCommandEx )
         {
            stub->fnRequestSendPmBusCommandEx( stub, DSIBufferGet( commandStream ), DSIBufferSize( commandStream ) );
         }
         DSIBufferFree( commandStream );
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetSeekSensitivity:
      {
         DMMAmFmTunerControl_StopLevels stopLevels = 0 ;
         DSIReadDMMAmFmTunerControl_StopLevels( stream, &stopLevels ) ;
         if( 0 != stub->fnRequestSetSeekSensitivity )
         {
            stub->fnRequestSetSeekSensitivity( stub, stopLevels );
         }
         if( 0 != stub->fnRequestSetSeekSensitivityEx )
         {
            stub->fnRequestSetSeekSensitivityEx( stub, DSIVectorData( stopLevels ), DSIVectorSize( stopLevels ) );
         }
         DMMAmFmTunerControl_StopLevels_Free( &stopLevels );
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetSeekSensitivity:
      {
         if( 0 != stub->fnRequestGetSeekSensitivity )
         {
            stub->fnRequestGetSeekSensitivity( stub );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetHdBlending:
      {
         DMMAmFmTunerControl_HdBlending hdBlending = 0 ;
         DSIRead32( stream, &hdBlending ) ;
         if( 0 != stub->fnRequestSetHdBlending )
         {
            stub->fnRequestSetHdBlending( stub, hdBlending );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetHdBlending:
      {
         if( 0 != stub->fnRequestGetHdBlending )
         {
            stub->fnRequestGetHdBlending( stub );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetHdCalibration:
      {
         DSIBool enable = 0 ;
         DSIReadBool( stream, &enable ) ;
         if( 0 != stub->fnRequestSetHdCalibration )
         {
            stub->fnRequestSetHdCalibration( stub, enable );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetHdCalibration:
      {
         if( 0 != stub->fnRequestGetHdCalibration )
         {
            stub->fnRequestGetHdCalibration( stub );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetRegionCode:
      {
         DMMAmFmTunerControl_RegionCode regionCode = 0 ;
         unsigned int flags = 0 ;
         DSIRead32( stream, &regionCode ) ;
         DSIRead32( stream, &flags ) ;
         if( 0 != stub->fnRequestSetRegionCode )
         {
            stub->fnRequestSetRegionCode( stub, regionCode, flags );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetHdBer:
      {
         if( 0 != stub->fnRequestGetHdBer )
         {
            stub->fnRequestGetHdBer( stub );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestControlHdBer:
      {
         DMMAmFmTunerControl_HdBerMode hdBerMode = 0 ;
         DSIRead32( stream, &hdBerMode ) ;
         if( 0 != stub->fnRequestControlHdBer )
         {
            stub->fnRequestControlHdBer( stub, hdBerMode );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter:
      {
         unsigned int tunerParameter = 0 ;
         DSIBuffer buffer = 0 ;
         DSIRead32( stream, &tunerParameter ) ;
         DSIReadBuffer( stream, &buffer ) ;
         if( 0 != stub->fnRequestSetTunerParameter )
         {
            stub->fnRequestSetTunerParameter( stub, tunerParameter, buffer );
         }
         if( 0 != stub->fnRequestSetTunerParameterEx )
         {
            stub->fnRequestSetTunerParameterEx( stub, tunerParameter, DSIBufferGet( buffer ), DSIBufferSize( buffer ) );
         }
         DSIBufferFree( buffer );
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter:
      {
         unsigned int tunerParameter = 0 ;
         DSIRead32( stream, &tunerParameter ) ;
         if( 0 != stub->fnRequestGetTunerParameter )
         {
            stub->fnRequestGetTunerParameter( stub, tunerParameter );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetTunerTiming:
      {
         unsigned int tunerTiming = 0 ;
         unsigned int value = 0 ;
         DSIRead32( stream, &tunerTiming ) ;
         DSIRead32( stream, &value ) ;
         if( 0 != stub->fnRequestSetTunerTiming )
         {
            stub->fnRequestSetTunerTiming( stub, tunerTiming, value );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestGetTunerTiming:
      {
         unsigned int tunerTiming = 0 ;
         DSIRead32( stream, &tunerTiming ) ;
         if( 0 != stub->fnRequestGetTunerTiming )
         {
            stub->fnRequestGetTunerTiming( stub, tunerTiming );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestDeletePersistentData:
      {
         DMMAmFmTunerControl_PersistenceScope scope = 0 ;
         DSIRead32( stream, &scope ) ;
         if( 0 != stub->fnRequestDeletePersistentData )
         {
            stub->fnRequestDeletePersistentData( stub, scope );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetWavebandGrid:
      {
         DMMAmFmTunerControl_Waveband waveband = 0 ;
         DMMAmFmTunerControl_WavebandGrid grid ;
         memset( &grid, 0, sizeof(grid) )  ;
         DSIRead32( stream, &waveband ) ;
         DSIReadDMMAmFmTunerControl_WavebandGrid( stream, &grid ) ;
         if( 0 != stub->fnRequestSetWavebandGrid )
         {
            stub->fnRequestSetWavebandGrid( stub, waveband,  &grid );
         }
         DMMAmFmTunerControl_WavebandGrid_Free( &grid );
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetHdBallGameMode:
      {
         DMMAmFmTunerControl_HdBallGameMode hdBallGameMode = 0 ;
         DSIRead32( stream, &hdBallGameMode ) ;
         if( 0 != stub->fnRequestSetHdBallGameMode )
         {
            stub->fnRequestSetHdBallGameMode( stub, hdBallGameMode );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestAssign:
      {
         DMMAmFmTunerControl_SharedResources resource = 0 ;
         DSIBool grant = 0 ;
         DSIRead32( stream, &resource ) ;
         DSIReadBool( stream, &grant ) ;
         if( 0 != stub->fnRequestAssign )
         {
            stub->fnRequestAssign( stub, resource, grant );
         }
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestSetSpecialCoding:
      {
         DMMAmFmTunerControl_TunerParameters parameters = 0 ;
         DMMAmFmTunerControl_TunerTimings timings = 0 ;
         DSIReadDMMAmFmTunerControl_TunerParameters( stream, &parameters ) ;
         DSIReadDMMAmFmTunerControl_TunerTimings( stream, &timings ) ;
         if( 0 != stub->fnRequestSetSpecialCoding )
         {
            stub->fnRequestSetSpecialCoding( stub, parameters, timings );
         }
         if( 0 != stub->fnRequestSetSpecialCodingEx )
         {
            stub->fnRequestSetSpecialCodingEx( stub, DSIVectorData( parameters ), DSIVectorSize( parameters ), DSIVectorData( timings ), DSIVectorSize( timings ) );
         }
         DMMAmFmTunerControl_TunerParameters_Free( &parameters );
         DMMAmFmTunerControl_TunerTimings_Free( &timings );
      }
      break;

   case DMMAmFmTunerControl_UPD_ID_requestStorePersistentData:
      {
         if( 0 != stub->fnRequestStorePersistentData )
         {
            stub->fnRequestStorePersistentData( stub );
         }
      }
      break;

   default:
      break;
   }
}




void DMMAmFmTunerControl_ResponsePmBusCommand( DMMAmFmTunerControlStub* stub, const DSIBuffer resultStream )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteBuffer( &stream, &resultStream ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responsePmBusCommand, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponsePmBusCommandEx( DMMAmFmTunerControlStub* stub, const void* resultStream, int resultStreamSize )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &resultStreamSize );
   if( 0 != resultStreamSize )
   {
      DSIWrite( &stream, resultStream, resultStreamSize, 1 );
   }
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responsePmBusCommand, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseTestData( DMMAmFmTunerControlStub* stub,  DMMAmFmTunerControl_TestMode testMode,  unsigned int index, const DSIBuffer data )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &testMode ) ;
   DSIWrite32( &stream, &index ) ;
   DSIWriteBuffer( &stream, &data ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseTestData, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseTestDataEx( DMMAmFmTunerControlStub* stub,  DMMAmFmTunerControl_TestMode testMode,  unsigned int index, const void* data, int dataSize )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &testMode ) ;
   DSIWrite32( &stream, &index ) ;
   DSIWrite32( &stream, &dataSize );
   if( 0 != dataSize )
   {
      DSIWrite( &stream, data, dataSize, 1 );
   }
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseTestData, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseSeekSensitivity( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_StopLevels stopLevels )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_StopLevels( &stream, &stopLevels ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseSeekSensitivity, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseSeekSensitivityEx( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_StopLevel* stopLevels, int stopLevelsSize )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &stopLevelsSize );
   if (( 0 != stopLevelsSize ) && ( 0 != stopLevels))
   {
      int idx=0;
      while( idx < stopLevelsSize )
      {
         DSIWriteDMMAmFmTunerControl_StopLevel( &stream, &stopLevels[idx] ) ;
         idx++;
      }
   }
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseSeekSensitivity, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseHdBlending( DMMAmFmTunerControlStub* stub,  DMMAmFmTunerControl_HdBlending hdBlending )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &hdBlending ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseHdBlending, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseHdCalibration( DMMAmFmTunerControlStub* stub,  DSIBool enabled )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteBool( &stream, &enabled ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseHdCalibration, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseHdBer( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdBers bers )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_HdBers( &stream, &bers ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseHdBer, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseHdBerEx( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdBer* bers, int bersSize )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &bersSize );
   if (( 0 != bersSize ) && ( 0 != bers))
   {
      int idx=0;
      while( idx < bersSize )
      {
         DSIWriteDMMAmFmTunerControl_HdBer( &stream, &bers[idx] ) ;
         idx++;
      }
   }
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseHdBer, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdStatus( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdStatus* status )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_HdStatus( &stream, status ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdStatus, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdPerformance( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdPerformance* performance )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_HdPerformance( &stream, performance ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdPerformance, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdCallToAction( DMMAmFmTunerControlStub* stub,  unsigned int frequency,  unsigned char serviceId,  unsigned int caServiceId,  unsigned int radioId, const char* message )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &frequency ) ;
   DSIWrite8( &stream, &serviceId ) ;
   DSIWrite32( &stream, &caServiceId ) ;
   DSIWrite32( &stream, &radioId ) ;
   DSIWriteString( &stream, &message ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdCallToAction, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdPsdChanged( DMMAmFmTunerControlStub* stub,  unsigned int frequency,  unsigned char service )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &frequency ) ;
   DSIWrite8( &stream, &service ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdPsdChanged, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdTagging( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdToken* token )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_HdToken( &stream, token ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdTagging, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseTunerParameter( DMMAmFmTunerControlStub* stub,  unsigned int tunerParameter, const DSIBuffer buffer,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &tunerParameter ) ;
   DSIWriteBuffer( &stream, &buffer ) ;
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseTunerParameter, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseTunerParameterEx( DMMAmFmTunerControlStub* stub,  unsigned int tunerParameter, const void* buffer, int bufferSize,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &tunerParameter ) ;
   DSIWrite32( &stream, &bufferSize );
   if( 0 != bufferSize )
   {
      DSIWrite( &stream, buffer, bufferSize, 1 );
   }
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseTunerParameter, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseTunerTiming( DMMAmFmTunerControlStub* stub,  unsigned int tunerTiming,  unsigned int value,  DMMTunerTypes_Feedback feedback )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWrite32( &stream, &tunerTiming ) ;
   DSIWrite32( &stream, &value ) ;
   DSIWrite32( &stream, &feedback ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseTunerTiming, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_InformationHdImage( DMMAmFmTunerControlStub* stub, const DMMAmFmTunerControl_HdImage* image )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIWriteDMMAmFmTunerControl_HdImage( &stream, image ) ;
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_informationHdImage, &stream );
   DSIClose( &stream );
}

void DMMAmFmTunerControl_ResponseStorePersistentData( DMMAmFmTunerControlStub* stub )
{
   DSIStream stream ;
   DSIOpen( &stream );
   DSIServerSendResponse( stub, RESULT_OK, DMMAmFmTunerControl_UPD_ID_responseStorePersistentData, &stream );
   DSIClose( &stream );
}


static void ProcessInternalEvent( DMMAmFmTunerControlStub* stub, int code, void* data )
{
   if( stub && stub->fnProcessInternalEvent )
   {
      stub->fnProcessInternalEvent( stub, code, data );
   }
}

void DMMAmFmTunerControlStub_SendNotification( DMMAmFmTunerControlStub* stub, DMMAmFmTunerControl_UpdateIdEnum id )
{
   DSIServerSendNotification( stub, id );
}

static void Cleanup( DMMAmFmTunerControlStub* stub )
{
   if( stub && stub->fnCleanup )
   {
      stub->fnCleanup( stub );
   }
}

static int GetResponse( DMMAmFmTunerControlStub* stub, int requestId )
{
   DMMAmFmTunerControl_UpdateIdEnum responseId = DMMAmFmTunerControl_UPD_ID_NOP ;


   return (responseId == DMMAmFmTunerControl_UPD_ID_NOP) ? DSI_INVALID_ID : (int)responseId ;
}

int DMMAmFmTunerControlStub_Init( DMMAmFmTunerControlStub* stub, const char* rolename )
{
   DSIInit();
   if (stub)
   {
      memset( stub, 0, sizeof(*stub));
   }
   DSIServerInit( stub, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   if (stub)
   {
      stub->server.fnWriteAttribute = WriteAttribute ;
      stub->server.fnProcessRequest = ProcessRequest ;
      stub->server.fnProcessInternalEvent = ProcessInternalEvent ;
      stub->server.fnCleanup = Cleanup ;
      stub->server.fnGetResponse = GetResponse ;
   }
   return 0;
}

void DMMAmFmTunerControlStub_Free( DMMAmFmTunerControlStub* stub  )
{
   if (stub == 0)
   {
      return;
   }
   DMMAmFmTunerControl_WavebandGrids_Free( &stub->wavebandGrids );
   DMMAmFmTunerControl_Strings_Free( &stub->versions );
   DMMTunerTypes_UInt32s_Free( &stub->powerSync );
   DSIServerFree( stub );
   memset( stub, 0, sizeof(*stub) );
}

int DMMAmFmTunerControlStub_Start( DMMAmFmTunerControlStub* stub, const char* rolename )
{
   DSIInit();
   if (stub == 0)
   {
      return 0;
   }
   // check if all request function pointers are valid
   assert( 0 != stub->fnRequestSetAfSwitch  );
   assert( 0 != stub->fnRequestSetMultipleStationsSwitch  );
   assert( 0 != stub->fnRequestSetRegMode  );
   assert( 0 != stub->fnRequestSetStereoMode  );
   assert( 0 != stub->fnRequestSetHdMode  );
   assert( 0 != stub->fnRequestSetDiagSwitch  );
   assert( 0 != stub->fnRequestSetTestMode  );
   assert( 0 != stub->fnRequestSendPmBusCommand || 0 != stub->fnRequestSendPmBusCommandEx );
   assert( 0 != stub->fnRequestSetSeekSensitivity || 0 != stub->fnRequestSetSeekSensitivityEx );
   assert( 0 != stub->fnRequestGetSeekSensitivity  );
   assert( 0 != stub->fnRequestSetHdBlending  );
   assert( 0 != stub->fnRequestGetHdBlending  );
   assert( 0 != stub->fnRequestSetHdCalibration  );
   assert( 0 != stub->fnRequestGetHdCalibration  );
   assert( 0 != stub->fnRequestSetRegionCode  );
   assert( 0 != stub->fnRequestGetHdBer  );
   assert( 0 != stub->fnRequestControlHdBer  );
   assert( 0 != stub->fnRequestSetTunerParameter || 0 != stub->fnRequestSetTunerParameterEx );
   assert( 0 != stub->fnRequestGetTunerParameter  );
   assert( 0 != stub->fnRequestSetTunerTiming  );
   assert( 0 != stub->fnRequestGetTunerTiming  );
   assert( 0 != stub->fnRequestDeletePersistentData  );
   assert( 0 != stub->fnRequestSetWavebandGrid  );
   assert( 0 != stub->fnRequestSetHdBallGameMode  );
   assert( 0 != stub->fnRequestAssign  );
   assert( 0 != stub->fnRequestSetSpecialCoding || 0 != stub->fnRequestSetSpecialCodingEx );
   assert( 0 != stub->fnRequestStorePersistentData  );

   DSIServerInit( stub, SERVER_NAME, rolename, SERVER_MAJOR_VERSION, SERVER_MINOR_VERSION );
   stub->server.fnWriteAttribute = WriteAttribute ;
   stub->server.fnProcessRequest = ProcessRequest ;
   stub->server.fnProcessInternalEvent = ProcessInternalEvent ;
   stub->server.fnCleanup = Cleanup ;
   stub->server.fnGetResponse = GetResponse ;
   return DSIServerRun( stub );
}


int DMMAmFmTunerControlStub_Stop( DMMAmFmTunerControlStub* stub, int exitcode )
{
	return DSIServerStop( stub, exitcode );
}


void DMMAmFmTunerControlSendError( DMMAmFmTunerControlStub* stub, DMMAmFmTunerControl_UpdateIdEnum id )
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


int DMMAmFmTunerControlStub_PostInternalEvent( DMMAmFmTunerControlStub* stub, int code, void* data )
{
   if (stub)
	   return DSIPostInternalEvent( stub->server.Channel.Master, stub->server.Id, 0, code, data );
   else
      return -1;
}


int DMMAmFmTunerControlStub_SendInternalEvent( DMMAmFmTunerControlStub* stub, int code, void* data )
{
   if (stub)
   {
      if (stub->server.SendChid)
      {
          return DSISendInternalEvent( stub->server.SendChid, stub->server.Id, 0, code, data );

      }
      else
      {

   	   return DSISendInternalEvent( stub->server.Channel.Master, stub->server.Id, 0, code, data );
      }
   }
   else
      return -1;
}

