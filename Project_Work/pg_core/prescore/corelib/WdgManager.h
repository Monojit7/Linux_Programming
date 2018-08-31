/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            WdgManager.h
* @ingroup         presctrlcore
* @author          Sandeep kumar(sandeep.kumar7@harman.com)
*/

#include"core/CTimerInterface.h"

class WdgManager:public ITimerEventListener
{
public:
	WdgManager(int timer);
	virtual ~WdgManager();
	void timerEvent(int timerId) ;
	void stopTimer();
private:
	CTimerInterface mWdgTimer;
	int mWdgTimerId;
	void startTimer(int timer);
};
