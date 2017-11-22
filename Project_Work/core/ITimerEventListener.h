/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            ITimerEventListener.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

* @brief           Override this method to get time expiry notifications. 
*/

#ifndef _ITIMER_EVENT_LISTENER_H
#define _ITIMER_EVENT_LISTENER_H

class ITimerEventListener
{
public : 
	virtual void timerEvent(int timerid) = 0; 
};

#endif
