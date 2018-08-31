/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            SlotBase.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Abstraction for the slot concept. All communication with the HMI will be in terms of a slot id. 
*				   Slot represents a  generic abstraction of a media container. For example, USB is a type of slot which can contain any usb device (ipod/usb/mtp/app/carplay etc.. )
*				   The slot exposes generic API which can be overridden for slot specific behavior, for example to eject a CD drive. 
*				   When slots are firt created they initialize themselves with a dummy device. 
*/
#ifndef _SLOT_BASE_H_
#define _SLOT_BASE_H_
#include <core.h>


using namespace v1::com::harman::media;

class DeviceBase; 

class SlotBase
{
public : 

	enum SlotType
	{
		e_SLOT_USB=0,
		e_SLOT_A2DP,
		e_SLOT_OPTICAL_DRIVE
	};


	SlotBase(SlotType type);

	virtual ~SlotBase(); 

	/**
	*	Registers a new device in the slot. The old device will be deleted. 
	*/
	void setDevice(DeviceBase*	pDevice);

	/**
	*    Removes and cleansup a registered device. Also will assign the dummy device to the slot. 
	*/
	void removeDevice(); 



private : 
	SlotType mSlotType; 
	//uint8_t mSlotID;
};

#endif
