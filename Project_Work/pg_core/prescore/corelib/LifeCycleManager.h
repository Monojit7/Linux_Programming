/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            LifeCycleManager.h
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
* @brief           Life cycle manager
*
*/

#include <core/PresCtrl.h>
#include <core/CommonAPIClientAbstraction.h>

/**
* \brief  LifeCycleManager manages the on off states as per system states
*/
class LifeCycleManager 
{
public:
	
	LifeCycleManager(ShutdownCallback&&);

private:
	
	uint32_t									mShutdownSignalHandle;
	CommonAPIClientAbstraction<ConsumerProxy>	mNsmproxy;
	const ShutdownCallback						mSCallback;

	void statusListener(CommonAPI::AvailabilityStatus status);
	void shutdownevent(const uint32_t &shutdownType);
};
