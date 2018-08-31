/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            Lifecycle.h
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
* @brief           Lifecyle component abstraction for shutdown
				   NOTE : For session / application mode / boot mode details and more , client is expected
				   to consume the interfaces provided in ConsumerProxy.hpp directly
*/

#ifndef LIFECYCLE_H
#define LIFECYCLE_H

#include <lifecycle/v2/org/genivi/NodeStateManager/ConsumerProxy.hpp>

using namespace v2::org::genivi::NodeStateManager;

/*
Registered shutdown handler SHOULD exit only after all the shutdown activities are executed
Client has the option to register no handler in which case core assumes that client requires 
no shutdown intimation / activities to be performed
*/
typedef std::function<NodeStateTypes::NsmErrorStatus(const NodeStateTypes::NsmShutdownType &)> ShutdownCallback;

#endif // LIFECYCLE_H
