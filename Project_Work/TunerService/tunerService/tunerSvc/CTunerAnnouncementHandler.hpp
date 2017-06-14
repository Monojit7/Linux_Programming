#ifndef TUNER_ANNOUNCEMENTHANDLER_HPP_
#define TUNER_ANNOUNCEMENTHANDLER_HPP_

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
* @file CTunerAnnouncementHandler.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerCommand.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CDMMTunerAnnouncementProxy.hpp"


class CTunerAnnouncementHandler : public CTunerCommand
{
public:
   CTunerAnnouncementHandler();
   ~CTunerAnnouncementHandler();

public:
   static CTunerAnnouncementHandler* getAnnouncementHandler();
   static bool mAnnouncementState;
   static UINT32 TAfreq;
   void execute(CSvcIpcRequest* reqData);
   bool getTAMode(void);
   void setTAMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   void cancelTA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData);
   static bool TAmode;
private:
   static CTunerAnnouncementHandler* mAnnouncementHandler;
};
#endif
