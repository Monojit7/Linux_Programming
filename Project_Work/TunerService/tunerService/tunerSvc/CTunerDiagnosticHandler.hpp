#ifndef TUNER_DIAGNOSTICHANDLER_HPP_
#define TUNER_DIAGNOSTICHANDLER_HPP_

/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2010
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CTunerDiagnosticHandler.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerCommand.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CTunerRequestResponseHandler.hpp"


class CTunerDiagnosticHandler: public CTunerCommand
{
public:
   CTunerDiagnosticHandler();
   virtual ~CTunerDiagnosticHandler();

   void execute(CSvcIpcRequest* reqData);

   //Get functions
   void getFieldStrength(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getSoftmuteSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHighCutSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getStereoHighBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getStereoBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getQualityThresholdEntry(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getQualityThresholdExit(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDAudioMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDFMBlendTimeAD(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDFMBlendTimeDA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDAMBlendTimeAD(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDAMBlendTimeDA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDBerMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDAudioSplitting(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void getHDBlendingMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);


   //Set functions
   void setSoftmuteSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setHighCutSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setStereoHighBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setStereoBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setQualityThresholdEntry(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setQualityThresholdExit(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setHDBerMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void setHDAudioSplitting(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);

};

#endif
