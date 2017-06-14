#ifndef DMMAMFMTUNERDIRANACONTROLSTUB_H
#define DMMAMFMTUNERDIRANACONTROLSTUB_H
/**********************************************************************
 *  Project       Harman Car Multimedia System
 *  (c) copyright 2014
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 **********************************************************************/

/*
 * Generated by DSI Generator version 2.0
 * Date: 21.11.2014
 */


#include "DMMAmFmTunerDiranaControl.h"

#ifndef TDMMAmFmTunerDiranaControlStub
#define TDMMAmFmTunerDiranaControlStub struct _DMMAmFmTunerDiranaControlStub
#endif

DSI_EXTERN_C_BEGIN

/**
 * DSI Stub: DMMAmFmTunerDiranaControl
 *
 * Interface for control of Dirana audio routing, for experts only.
 */
typedef struct _DMMAmFmTunerDiranaControlStub
{
   DSIServer server ;

   /**
    * Implementation version of this server
    */
   DSIVersionInfo implVersion ;


   /**
    * Changes Dirana audio routing: Connect input with output.
    */
   void (*fnRequestSetAudioRouting)( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_AudioInputs input,  DMMAmFmTunerDiranaControl_AudioOutputs output );

   /**
    * Get current audio input connected with given output.
    */
   void (*fnRequestGetAudioRouting)( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_AudioOutputs output );

   /**
    * Writes a single memory location with a 24-bit value in the dirana and returns the result of the operation in the corresponding response.
    */
   void (*fnRequestWriteXMem)( TDMMAmFmTunerDiranaControlStub* stub,  unsigned int memAddress,  unsigned int memValueX );

   /**
    * Reads a single memory address from the dirana and returns the value in the corresponding response
    */
   void (*fnRequestReadXMem)( TDMMAmFmTunerDiranaControlStub* stub,  unsigned int memAddress );

   /**
    * Writes a single memory location with a 16-bit value in the dirana and returns the result of the operation in the corresponding response.
    */
   void (*fnRequestWriteYMem)( TDMMAmFmTunerDiranaControlStub* stub,  unsigned int memAddress,  unsigned short memValueY );

   /**
    * Reads a single memory address from the dirana and returns the value in the corresponding response
    */
   void (*fnRequestReadYMem)( TDMMAmFmTunerDiranaControlStub* stub,  unsigned int memAddress );


   /**
    * Called when the stub received an internal event
    */
   void (*fnProcessInternalEvent) ( TDMMAmFmTunerDiranaControlStub* stub, int id, void* data ) ;

   /**
    * Called right before the stub thread ends
    */
   void (*fnCleanup) ( TDMMAmFmTunerDiranaControlStub* stub ) ;

} DMMAmFmTunerDiranaControlStub ;

/**
 * Returns success state of request, notifcation has to be set manually.
 */
void DMMAmFmTunerDiranaControl_ResponseSetAudioRouting( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_Result result );

/**
 * Returns requested audio input
 */
void DMMAmFmTunerDiranaControl_ResponseGetAudioRouting( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_AudioInputs input );

/**
 * The response.
 */
void DMMAmFmTunerDiranaControl_ResponseReadXMem( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_Result result,  unsigned int memValueX );

/**
 * The response.
 */
void DMMAmFmTunerDiranaControl_ResponseWriteXMem( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_Result result );

/**
 * The response.
 */
void DMMAmFmTunerDiranaControl_ResponseReadYMem( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_Result result,  unsigned short memValueY );

/**
 * The response.
 */
void DMMAmFmTunerDiranaControl_ResponseWriteYMem( TDMMAmFmTunerDiranaControlStub* stub,  DMMAmFmTunerDiranaControl_Result result );


/**
 * Sends a notification to all clients that are interested.
 */
void DMMAmFmTunerDiranaControlStub_SendNotification( TDMMAmFmTunerDiranaControlStub* stub, DMMAmFmTunerDiranaControl_UpdateIdEnum id );

/**
 * Initailzes the stub
 */
int DMMAmFmTunerDiranaControlStub_Init( TDMMAmFmTunerDiranaControlStub* stub, const char* rolename );

/**
 * Frees the stub
 */
void DMMAmFmTunerDiranaControlStub_Free( TDMMAmFmTunerDiranaControlStub* stub );

/**
 * Starts the DMMAmFmTunerDiranaControl stub and enters the message loop. This functions
 * does not return until void DMMAmFmTunerDiranaControlStubStop() is called or an error
 * during startup occures.
 */
int DMMAmFmTunerDiranaControlStub_Start( TDMMAmFmTunerDiranaControlStub* stub, const char* rolename );

/**
 * Stops the DMMAmFmTunerDiranaControl stub. This function ends the stubs message loop.
 * DMMAmFmTunerDiranaControlStub_Start() returns the exitcode.
 */
int DMMAmFmTunerDiranaControlStub_Stop( TDMMAmFmTunerDiranaControlStub* stub, int exitcode );

/**
 * Respond with an error message.
 */
void DMMAmFmTunerDiranaControlSendError( TDMMAmFmTunerDiranaControlStub* stub, DMMAmFmTunerDiranaControl_UpdateIdEnum id );

/**
 * Posts an internal event in the stubs message queue and returns immediately.
 */
int DMMAmFmTunerDiranaControlStub_PostInternalEvent( TDMMAmFmTunerDiranaControlStub* stub, int code, void* data );

/**
 * Send an internal event to the stub and waits until the stub handled it. Do not call this
 * function from within the stubs task. This will lead into a dead lock.
 */
int DMMAmFmTunerDiranaControlStub_SendInternalEvent( TDMMAmFmTunerDiranaControlStub* stub, int code, void* data );

#define DMMAmFmTunerDiranaControlStub_RegisterCurrentSession( stub ) DSIServerRegisterCurrentSession( &stub->server )
#define DMMAmFmTunerDiranaControlStub_ClearActiveSessions( stub ) DSIServerClearActiveSessions( &stub->server )
#define DMMAmFmTunerDiranaControlStub_AddActiveSession( stub, sessionId ) DSIServerAddActiveSession( &stub->server, sessionId )
#define DMMAmFmTunerDiranaControlStub_IsSessionActive( stub, sessionId ) DSIServerIsSessionActive( &stub->server, sessionId )

#define DMMAmFmTunerDiranaControlStub_UnblockRequest( stub ) DSIServerUnblockRequest( &stub->server )
#define DMMAmFmTunerDiranaControlStub_PrepareResponse( stub, sessionHandle ) DSIServerPrepareResponse( &stub->server, sessionHandle )

DSI_EXTERN_C_END

#endif // DMMAMFMTUNERDIRANACONTROLSTUB_H
