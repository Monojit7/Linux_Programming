/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            SlotBase.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Abstraction for the slot concept. All communication with the HMI will be in terms of a slot id.
*				   Slot represents a  generic abstraction of a media container. For example, USB is a type of slot which can contain any usb device (ipod/usb/mtp/app/carplay etc.. )
*				   The slot exposes generic API which can be overridden for slot specific behavior, for example to eject a CD drive.
*/

#include "SlotBase.h"
#include "device/DeviceDummy.h"

SlotBase::SlotBase(SlotType type)
	:mSlotType(type)
{

}


SlotBase::~SlotBase()
{

}

/**
*	Registers a new device in the slot. The old device will be deleted.
*/
void SlotBase::setDevice(DeviceBase* pDevice)
{
 
}

/**
*    Removes and cleansup a registered device. Also will assign the dummy device to the slot.
*/
void SlotBase::removeDevice()
{
}
