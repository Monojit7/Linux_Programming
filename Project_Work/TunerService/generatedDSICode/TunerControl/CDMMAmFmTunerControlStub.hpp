#ifndef CDMMAMFMTUNERCONTROLSTUB_HPP
#define CDMMAMFMTUNERCONTROLSTUB_HPP
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


#include "DMMAmFmTunerControlStub.h"

/**
 * DSI Stub: DMMAmFmTunerControl
 *
 * Project independent but tuner specific interface.
 * This interface contains AM/FM/HD specific functionality which is not covered by any generic tuner interface.
 */
class CDMMAmFmTunerControlStub : public DMMAmFmTunerControlStub
{
public:
   /**
    * Constructor.
    */
   CDMMAmFmTunerControlStub( );

   /**
    * Destructor.
    */
   virtual ~CDMMAmFmTunerControlStub();

   /**
    * Start the server
    */
   int start( const char* rolename );

   /**
    * Stop the server
    */
   void stop( int exitcode );

   /**
    * Called right before the stub thread ends
    */
   virtual void cleanup();

   /**
    * Set switch to enable/disable alternative frequency following. The current state is reflected by afSwitch attribute.
    */
   virtual void requestSetAfSwitch(  DSIBool enable ) = 0 ;

   /**
    * Set switch whether all alternative frequencies of one station shall be inserted into FM station list or not. The current state is reflected by multipleStationsSwitch attribute.
    */
   virtual void requestSetMultipleStationsSwitch(  DSIBool enable ) = 0 ;

   /**
    * Set desired regionalization mode. The current mode is reflected by regMode attribute.
    */
   virtual void requestSetRegMode(  DMMAmFmTunerControl_RegMode regMode ) = 0 ;

   /**
    * Set new desired stereo mode. The current mode is reflected by stereoMode attribute.
    */
   virtual void requestSetStereoMode(  DMMAmFmTunerControl_StereoMode stereoMode ) = 0 ;

   /**
    * Set new desired HD mode. The current mode is reflected by hdMode attribute.
    */
   virtual void requestSetHdMode(  DMMAmFmTunerControl_HdMode hdMode ) = 0 ;

   /**
    * Set switch to enable/disable diagnosis mode. The current state is reflected by diagSwitch attribute. Different behavior for Tuner32 and OOTuner implemenation, refert to "enable" parameter.
    */
   virtual void requestSetDiagSwitch(  DSIBool enable ) = 0 ;

   /**
    * Select a specific test mode, resulting test data is sent via testData response (don't forget to set notification).
    */
   virtual void requestSetTestMode(  DMMAmFmTunerControl_TestMode testMode,  unsigned int index ) = 0 ;

   /**
    * Send a PMBus command to the tuner device. Don't forget to set notifcation for pmBusCommand response in order to get the resulting message.
    */
   virtual void requestSendPmBusCommand( const DSIBuffer commandStream ) = 0 ;

   /**
    * Configure the seek/scan stop thresholds for all wavebands. The current settings can be queried  by getSeekSensitivity.
    */
   virtual void requestSetSeekSensitivity( const DMMAmFmTunerControl_StopLevels stopLevels ) = 0 ;

   /**
    * Get the current seek/scan thresholds via seekSensitivity response (don't forget to set notification).
    */
   virtual void requestGetSeekSensitivity( ) = 0 ;

   /**
    * Set blending mode.
    */
   virtual void requestSetHdBlending(  DMMAmFmTunerControl_HdBlending hdBlending ) = 0 ;

   /**
    * Get the current blending mode via hdSwVersion response (don't forget to set notification).
    */
   virtual void requestGetHdBlending( ) = 0 ;

   /**
    * Enable/disable HD calibration functionality.
    */
   virtual void requestSetHdCalibration(  DSIBool enable ) = 0 ;

   /**
    * Get the current HD calibration mode via hdCalibration response (don't forget to set notification).
    */
   virtual void requestGetHdCalibration( ) = 0 ;

   /**
    * Set RegionCode.
    */
   virtual void requestSetRegionCode(  DMMAmFmTunerControl_RegionCode regionCode,  unsigned int flags ) = 0 ;

   /**
    * Get HD bit error rates
    */
   virtual void requestGetHdBer( ) = 0 ;

   /**
    * Control (start / stop / reset) HD bit error measurement
    */
   virtual void requestControlHdBer(  DMMAmFmTunerControl_HdBerMode hdBerMode ) = 0 ;

   /**
    * OOTuner: Manipulate tuner engineering parameter (for experts only)
    */
   virtual void requestSetTunerParameter(  unsigned int tunerParameter, const DSIBuffer buffer ) = 0 ;

   /**
    * OOTuner: Get tuner engineering parameter
    */
   virtual void requestGetTunerParameter(  unsigned int tunerParameter ) = 0 ;

   /**
    * OOTuner: Manipulate tuner timing parameter (for experts only)
    */
   virtual void requestSetTunerTiming(  unsigned int tunerTiming,  unsigned int value ) = 0 ;

   /**
    * OOTuner: Get tuner timing parameter
    */
   virtual void requestGetTunerTiming(  unsigned int tunerTiming ) = 0 ;

   /**
    * Delete persistent data
    */
   virtual void requestDeletePersistentData(  DMMAmFmTunerControl_PersistenceScope scope ) = 0 ;

   /**
    * Configure waveband settings for specific waveband, becomes active with next setRegionCode (see MSCs).
    */
   virtual void requestSetWavebandGrid(  DMMAmFmTunerControl_Waveband waveband, const DMMAmFmTunerControl_WavebandGrid& grid ) = 0 ;

   /**
    * Set new desired HD ball game mode. The current mode is reflected by hdBallGameMode attribute.
    */
   virtual void requestSetHdBallGameMode(  DMMAmFmTunerControl_HdBallGameMode hdBallGameMode ) = 0 ;

   /**
    * Internal: Assign/withdraw resource 
    */
   virtual void requestAssign(  DMMAmFmTunerControl_SharedResources resource,  DSIBool grant ) = 0 ;

   /**
    * Configure special coding set via TunerParameter and TunerTiming, becomes active with next setRegionCode (see MSCs).
    */
   virtual void requestSetSpecialCoding( const DMMAmFmTunerControl_TunerParameters parameters, const DMMAmFmTunerControl_TunerTimings timings ) = 0 ;

   /**
    * ONLY for projects without dedicated persistency component (using simple files system persistency instead): Store persistent data, caller has to wait for response before shutting down system.
    */
   virtual void requestStorePersistentData( ) = 0 ;

   /**
    * Called when an internal event was sent to the server
    */
   virtual void processInternalEvent( int code, void* data ) {}

   /**
    * The response of the tuner device for a PMBus command.
    */
   void responsePmBusCommand( const DSIBuffer resultStream );

   /**
    * Raw test data sent by device if enabled via setTestMode.
    */
   void responseTestData(  DMMAmFmTunerControl_TestMode testMode,  unsigned int index, const DSIBuffer data );

   /**
    * The current seek sensitivity settings.
    */
   void responseSeekSensitivity( const DMMAmFmTunerControl_StopLevels stopLevels );

   /**
    * The current blending mode.
    */
   void responseHdBlending(  DMMAmFmTunerControl_HdBlending hdBlending );

   /**
    * The HD calibration state.
    */
   void responseHdCalibration(  DSIBool enabled );

   /**
    * The HD bit error rates
    */
   void responseHdBer( const DMMAmFmTunerControl_HdBers bers );

   /**
    * HD status data
    */
   void informationHdStatus( const DMMAmFmTunerControl_HdStatus& status );

   /**
    * HD performance data
    */
   void informationHdPerformance( const DMMAmFmTunerControl_HdPerformance& performance );

   /**
    * HD call to action information for decrypted services
    */
   void informationHdCallToAction(  unsigned int frequency,  unsigned char serviceId,  unsigned int caServiceId,  unsigned int radioId, const char* message );

   /**
    * HD program service data changed (trigger)
    */
   void informationHdPsdChanged(  unsigned int frequency,  unsigned char service );

   /**
    * Token for HD iTunes tagging
    */
   void informationHdTagging( const DMMAmFmTunerControl_HdToken& token );

   /**
    * Response of set/getTunerParameter
    */
   void responseTunerParameter(  unsigned int tunerParameter, const DSIBuffer buffer,  DMMTunerTypes_Feedback feedback );

   /**
    * Response of set/getTunerTiming
    */
   void responseTunerTiming(  unsigned int tunerTiming,  unsigned int value,  DMMTunerTypes_Feedback feedback );

   /**
    * An HD image (station logo or cover art) is available.
    * !!! Deprecated: Use DMMTunerStationImage instead !!!
    * 
    * DEPRECATED: Use DMMTunerStationImage instead
    */
   void informationHdImage( const DMMAmFmTunerControl_HdImage& image );

   /**
    * Data storing complete.
    */
   void responseStorePersistentData( );


   void sendNotification( DMMAmFmTunerControl_UpdateIdEnum updateId );

   /**
    * Respond with an error message.
    */
   void sendError( DMMAmFmTunerControl_UpdateIdEnum id );

   int postInternalEvent( int code, void* data );
   int sendInternalEvent( int code, void* data );

private:

   /*
    * Duplicating not allowed
    */
   CDMMAmFmTunerControlStub( const CDMMAmFmTunerControlStub& ) {}
   CDMMAmFmTunerControlStub& operator= (const CDMMAmFmTunerControlStub& ) { return *this ; }
};

inline void CDMMAmFmTunerControlStub::sendNotification( DMMAmFmTunerControl_UpdateIdEnum updateId )
{
   DMMAmFmTunerControlStub_SendNotification( this, updateId );
}


inline void CDMMAmFmTunerControlStub::responsePmBusCommand( const DSIBuffer resultStream )
{
   DMMAmFmTunerControl_ResponsePmBusCommand( (DMMAmFmTunerControlStub*)this, resultStream );
}

inline void CDMMAmFmTunerControlStub::responseTestData(  DMMAmFmTunerControl_TestMode testMode,  unsigned int index, const DSIBuffer data )
{
   DMMAmFmTunerControl_ResponseTestData( (DMMAmFmTunerControlStub*)this, testMode, index, data );
}

inline void CDMMAmFmTunerControlStub::responseSeekSensitivity( const DMMAmFmTunerControl_StopLevels stopLevels )
{
   DMMAmFmTunerControl_ResponseSeekSensitivity( (DMMAmFmTunerControlStub*)this, stopLevels );
}

inline void CDMMAmFmTunerControlStub::responseHdBlending(  DMMAmFmTunerControl_HdBlending hdBlending )
{
   DMMAmFmTunerControl_ResponseHdBlending( (DMMAmFmTunerControlStub*)this, hdBlending );
}

inline void CDMMAmFmTunerControlStub::responseHdCalibration(  DSIBool enabled )
{
   DMMAmFmTunerControl_ResponseHdCalibration( (DMMAmFmTunerControlStub*)this, enabled );
}

inline void CDMMAmFmTunerControlStub::responseHdBer( const DMMAmFmTunerControl_HdBers bers )
{
   DMMAmFmTunerControl_ResponseHdBer( (DMMAmFmTunerControlStub*)this, bers );
}

inline void CDMMAmFmTunerControlStub::informationHdStatus( const DMMAmFmTunerControl_HdStatus& status )
{
   DMMAmFmTunerControl_InformationHdStatus( (DMMAmFmTunerControlStub*)this,  &status );
}

inline void CDMMAmFmTunerControlStub::informationHdPerformance( const DMMAmFmTunerControl_HdPerformance& performance )
{
   DMMAmFmTunerControl_InformationHdPerformance( (DMMAmFmTunerControlStub*)this,  &performance );
}

inline void CDMMAmFmTunerControlStub::informationHdCallToAction(  unsigned int frequency,  unsigned char serviceId,  unsigned int caServiceId,  unsigned int radioId, const char* message )
{
   DMMAmFmTunerControl_InformationHdCallToAction( (DMMAmFmTunerControlStub*)this, frequency, serviceId, caServiceId, radioId, message );
}

inline void CDMMAmFmTunerControlStub::informationHdPsdChanged(  unsigned int frequency,  unsigned char service )
{
   DMMAmFmTunerControl_InformationHdPsdChanged( (DMMAmFmTunerControlStub*)this, frequency, service );
}

inline void CDMMAmFmTunerControlStub::informationHdTagging( const DMMAmFmTunerControl_HdToken& token )
{
   DMMAmFmTunerControl_InformationHdTagging( (DMMAmFmTunerControlStub*)this,  &token );
}

inline void CDMMAmFmTunerControlStub::responseTunerParameter(  unsigned int tunerParameter, const DSIBuffer buffer,  DMMTunerTypes_Feedback feedback )
{
   DMMAmFmTunerControl_ResponseTunerParameter( (DMMAmFmTunerControlStub*)this, tunerParameter, buffer, feedback );
}

inline void CDMMAmFmTunerControlStub::responseTunerTiming(  unsigned int tunerTiming,  unsigned int value,  DMMTunerTypes_Feedback feedback )
{
   DMMAmFmTunerControl_ResponseTunerTiming( (DMMAmFmTunerControlStub*)this, tunerTiming, value, feedback );
}

inline void CDMMAmFmTunerControlStub::informationHdImage( const DMMAmFmTunerControl_HdImage& image )
{
   DMMAmFmTunerControl_InformationHdImage( (DMMAmFmTunerControlStub*)this,  &image );
}

inline void CDMMAmFmTunerControlStub::responseStorePersistentData( )
{
   DMMAmFmTunerControl_ResponseStorePersistentData( (DMMAmFmTunerControlStub*)this );
}


#endif // CDMMAMFMTUNERCONTROLSTUB_HPP


