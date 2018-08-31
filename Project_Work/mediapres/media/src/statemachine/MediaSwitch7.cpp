
#include "MediaSwitch7.h"
#include <string.h>
/*! \file Implementation of the state machine 'MediaSwitch7'
*/

MediaSwitch7::MediaSwitch7()
{
	
	ifaceMedia_OCB = null;
	
	stateConfVectorPosition = 0;
	
}

MediaSwitch7::~MediaSwitch7()
{
}


void MediaSwitch7::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = MediaSwitch7_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
}

void MediaSwitch7::enter()
{
	/* Default enter sequence for statechart MediaSwitch7 */
	enseq_main_region_default();
}

void MediaSwitch7::exit()
{
	/* Default exit sequence for statechart MediaSwitch7 */
	exseq_main_region();
}

sc_boolean MediaSwitch7::isActive()
{
	return stateConfVector[0] != MediaSwitch7_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean MediaSwitch7::isFinal()
{
   return false;}

void MediaSwitch7::runCycleIntern()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_unknown :
		{
			react_main_region_unknown();
			break;
		}
		case main_region_ON_REQUESTED :
		{
			react_main_region_ON_REQUESTED();
			break;
		}
		case main_region_SS_ON :
		{
			react_main_region_SS_ON();
			break;
		}
		case main_region_SS_PAUSED :
		{
			react_main_region_SS_PAUSED();
			break;
		}
		case main_region_PAUSE_REQUESTED :
		{
			react_main_region_PAUSE_REQUESTED();
			break;
		}
		case main_region_SS_OFF :
		{
			react_main_region_SS_OFF();
			break;
		}
		case main_region_OFF_REQUESTED :
		{
			react_main_region_OFF_REQUESTED();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void MediaSwitch7::runCycle()
{
	runCycleIntern(); 
	while (!InternalEventQueue.empty())
	{
		InternalEventQueue.front()();
		InternalEventQueue.pop_front();
		runCycleIntern(); 
	}
}

void MediaSwitch7::clearInEvents()
{
	ifaceMedia.onPlaying_raised = false;
	ifaceMedia.onPaused_raised = false;
	ifaceMedia.onStopped_raised = false;
	ifaceMedia.reqPlay_raised = false;
	ifaceMedia.reqPause_raised = false;
	ifaceMedia.reqOff_raised = false;
	ifaceMedia.reqTimeout_raised = false;
	ifaceMedia.devErr_raised = false;
	ifaceMedia.onRequest_raised = false;
	ifaceMedia.pauseRequest_raised = false;
	ifaceMedia.offRequest_raised = false;
	ifaceMedia.onUnknown_raised = false;
}

void MediaSwitch7::clearOutEvents()
{
}


sc_boolean MediaSwitch7::isStateActive(MediaSwitch7States state)
{
	switch (state)
	{
		case main_region_unknown : 
			return (sc_boolean) (stateConfVector[0] == main_region_unknown
			);
		case main_region_ON_REQUESTED : 
			return (sc_boolean) (stateConfVector[0] == main_region_ON_REQUESTED
			);
		case main_region_SS_ON : 
			return (sc_boolean) (stateConfVector[0] == main_region_SS_ON
			);
		case main_region_SS_PAUSED : 
			return (sc_boolean) (stateConfVector[0] == main_region_SS_PAUSED
			);
		case main_region_PAUSE_REQUESTED : 
			return (sc_boolean) (stateConfVector[0] == main_region_PAUSE_REQUESTED
			);
		case main_region_SS_OFF : 
			return (sc_boolean) (stateConfVector[0] == main_region_SS_OFF
			);
		case main_region_OFF_REQUESTED : 
			return (sc_boolean) (stateConfVector[0] == main_region_OFF_REQUESTED
			);
		default: return false;
	}
}

MediaSwitch7::DefaultSCI* MediaSwitch7::getDefaultSCI()
{
	return &iface;
}


MediaSwitch7::SCI_Media* MediaSwitch7::getSCI_Media()
{
	return &ifaceMedia;
}

void MediaSwitch7::SCI_Media::raise_onPlaying()
{
	onPlaying_raised = true;
}

void MediaSwitch7::SCI_Media::raise_onPaused()
{
	onPaused_raised = true;
}

void MediaSwitch7::SCI_Media::raise_onStopped()
{
	onStopped_raised = true;
}

void MediaSwitch7::SCI_Media::raise_reqPlay(sc_integer value)
{
	reqPlay_value = value;
	reqPlay_raised = true;
}

void MediaSwitch7::SCI_Media::raise_reqPause(sc_integer value)
{
	reqPause_value = value;
	reqPause_raised = true;
}

void MediaSwitch7::SCI_Media::raise_reqOff(sc_integer value)
{
	reqOff_value = value;
	reqOff_raised = true;
}

void MediaSwitch7::SCI_Media::raise_reqTimeout()
{
	reqTimeout_raised = true;
}

void MediaSwitch7::SCI_Media::raise_devErr(sc_integer value)
{
	devErr_value = value;
	devErr_raised = true;
}

void MediaSwitch7::SCI_Media::raise_onRequest(sc_integer value)
{
	onRequest_value = value;
	onRequest_raised = true;
}

void MediaSwitch7::SCI_Media::raise_pauseRequest(sc_integer value)
{
	pauseRequest_value = value;
	pauseRequest_raised = true;
}

void MediaSwitch7::SCI_Media::raise_offRequest(sc_integer value)
{
	offRequest_value = value;
	offRequest_raised = true;
}

void MediaSwitch7::SCI_Media::raise_onUnknown()
{
	onUnknown_raised = true;
}


void MediaSwitch7::setSCI_Media_OCB(SCI_Media_OCB* operationCallback)
{
	ifaceMedia_OCB = operationCallback;
}

// implementations of all internal functions

sc_boolean MediaSwitch7::check_main_region_unknown_tr0_tr0()
{
	return ifaceMedia.onPlaying_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_tr1_tr1()
{
	return ifaceMedia.onRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_tr2_tr2()
{
	return ifaceMedia.onPaused_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_tr3_tr3()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_tr4_tr4()
{
	return ifaceMedia.pauseRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_tr5_tr5()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_lr1_lr1()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_lr2_lr2()
{
	return ifaceMedia.reqPause_raised;
}

sc_boolean MediaSwitch7::check_main_region_unknown_lr3_lr3()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_tr0_tr0()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_tr1_tr1()
{
	return ifaceMedia.onPlaying_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_tr2_tr2()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_tr3_tr3()
{
	return ifaceMedia.pauseRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_tr4_tr4()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_lr1_lr1()
{
	return ifaceMedia.reqTimeout_raised || ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_lr2_lr2()
{
	return (ifaceMedia.reqPlay_raised) && (! ifaceMedia_OCB->doPlay(ifaceMedia.reqPlay_value));
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_lr3_lr3()
{
	return ifaceMedia.reqPause_raised;
}

sc_boolean MediaSwitch7::check_main_region_ON_REQUESTED_lr4_lr4()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr0_tr0()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr1_tr1()
{
	return ifaceMedia.pauseRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr2_tr2()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr3_tr3()
{
	return ifaceMedia.onPaused_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr4_tr4()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_tr5_tr5()
{
	return ifaceMedia.pauseRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_lr1_lr1()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_lr2_lr2()
{
	return ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_lr3_lr3()
{
	return ifaceMedia.reqPause_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_ON_lr4_lr4()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_tr0_tr0()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_tr1_tr1()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_tr2_tr2()
{
	return ifaceMedia.onPlaying_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_tr3_tr3()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_tr4_tr4()
{
	return ifaceMedia.onRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_lr1_lr1()
{
	return ifaceMedia.reqPause_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_lr2_lr2()
{
	return ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_lr3_lr3()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_PAUSED_lr4_lr4()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_tr0_tr0()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_tr1_tr1()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_tr2_tr2()
{
	return ifaceMedia.onRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_tr3_tr3()
{
	return ifaceMedia.onPaused_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_tr4_tr4()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_lr1_lr1()
{
	return ifaceMedia.reqTimeout_raised || ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_lr2_lr2()
{
	return (ifaceMedia.reqPause_raised) && (! ifaceMedia_OCB->doPause(ifaceMedia.pauseRequest_value));
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_lr3_lr3()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_PAUSE_REQUESTED_lr4_lr4()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_tr0_tr0()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_tr1_tr1()
{
	return ifaceMedia.onRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_tr2_tr2()
{
	return ifaceMedia.onPlaying_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_tr3_tr3()
{
	return ifaceMedia.offRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_lr1_lr1()
{
	return ifaceMedia.reqOff_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_lr2_lr2()
{
	return ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_lr3_lr3()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_SS_OFF_lr4_lr4()
{
	return ifaceMedia.reqPause_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_tr0_tr0()
{
	return ifaceMedia.onRequest_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_tr1_tr1()
{
	return ifaceMedia.onStopped_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_tr2_tr2()
{
	return ifaceMedia.onUnknown_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_lr1_lr1()
{
	return ifaceMedia.reqTimeout_raised || ifaceMedia.devErr_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_lr2_lr2()
{
	return (ifaceMedia.reqOff_raised) && (! ifaceMedia_OCB->doOff(ifaceMedia.offRequest_value));
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_lr3_lr3()
{
	return ifaceMedia.reqPlay_raised;
}

sc_boolean MediaSwitch7::check_main_region_OFF_REQUESTED_lr4_lr4()
{
	return ifaceMedia.reqPause_raised;
}

void MediaSwitch7::effect_main_region_unknown_tr0()
{
	exseq_main_region_unknown();
	enseq_main_region_SS_ON_default();
}

void MediaSwitch7::effect_main_region_unknown_tr1()
{
	exseq_main_region_unknown();
	enseq_main_region_ON_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_unknown_tr2()
{
	exseq_main_region_unknown();
	enseq_main_region_SS_PAUSED_default();
}

void MediaSwitch7::effect_main_region_unknown_tr3()
{
	exseq_main_region_unknown();
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_unknown_tr4()
{
	exseq_main_region_unknown();
	enseq_main_region_PAUSE_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_unknown_tr5()
{
	exseq_main_region_unknown();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_unknown_lr1_lr1()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onRequest(ifaceMedia.reqPlay_value); });
}

void MediaSwitch7::effect_main_region_unknown_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_pauseRequest(ifaceMedia.reqPause_value); });
}

void MediaSwitch7::effect_main_region_unknown_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.reqOff_value); });
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_tr0()
{
	exseq_main_region_ON_REQUESTED();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_tr1()
{
	exseq_main_region_ON_REQUESTED();
	ifaceMedia_OCB->resPlay(ifaceMedia.onRequest_value, true);
	enseq_main_region_SS_ON_default();
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_tr2()
{
	exseq_main_region_ON_REQUESTED();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_tr3()
{
	exseq_main_region_ON_REQUESTED();
	enseq_main_region_PAUSE_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_tr4()
{
	exseq_main_region_ON_REQUESTED();
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_lr1_lr1()
{
	ifaceMedia_OCB->resPlay(ifaceMedia.onRequest_value, false);
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_pauseRequest(ifaceMedia.pauseRequest_value); });
}

void MediaSwitch7::effect_main_region_ON_REQUESTED_lr4_lr4()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.offRequest_value); });
}

void MediaSwitch7::effect_main_region_SS_ON_tr0()
{
	exseq_main_region_SS_ON();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_SS_ON_tr1()
{
	exseq_main_region_SS_ON();
	enseq_main_region_PAUSE_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_ON_tr2()
{
	exseq_main_region_SS_ON();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_ON_tr3()
{
	exseq_main_region_SS_ON();
	enseq_main_region_SS_PAUSED_default();
}

void MediaSwitch7::effect_main_region_SS_ON_tr4()
{
	exseq_main_region_SS_ON();
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_SS_ON_tr5()
{
	exseq_main_region_SS_ON();
	enseq_main_region_PAUSE_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_ON_lr1_lr1()
{
	ifaceMedia_OCB->resPlay(ifaceMedia.reqPlay_value, true);
}

void MediaSwitch7::effect_main_region_SS_ON_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_SS_ON_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_pauseRequest(ifaceMedia.reqPause_value); });
}

void MediaSwitch7::effect_main_region_SS_ON_lr4_lr4()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.reqOff_value); });
}

void MediaSwitch7::effect_main_region_SS_PAUSED_tr0()
{
	exseq_main_region_SS_PAUSED();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_SS_PAUSED_tr1()
{
	exseq_main_region_SS_PAUSED();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_PAUSED_tr2()
{
	exseq_main_region_SS_PAUSED();
	enseq_main_region_SS_ON_default();
}

void MediaSwitch7::effect_main_region_SS_PAUSED_tr3()
{
	exseq_main_region_SS_PAUSED();
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_SS_PAUSED_tr4()
{
	exseq_main_region_SS_PAUSED();
	enseq_main_region_ON_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_PAUSED_lr1_lr1()
{
	ifaceMedia_OCB->resPause(ifaceMedia.reqPause_value, true);
}

void MediaSwitch7::effect_main_region_SS_PAUSED_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_SS_PAUSED_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onRequest(ifaceMedia.reqPlay_value); });
}

void MediaSwitch7::effect_main_region_SS_PAUSED_lr4_lr4()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.reqOff_value); });
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_tr0()
{
	exseq_main_region_PAUSE_REQUESTED();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_tr1()
{
	exseq_main_region_PAUSE_REQUESTED();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_tr2()
{
	exseq_main_region_PAUSE_REQUESTED();
	enseq_main_region_ON_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_tr3()
{
	exseq_main_region_PAUSE_REQUESTED();
	ifaceMedia_OCB->resPause(ifaceMedia.pauseRequest_value, true);
	enseq_main_region_SS_PAUSED_default();
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_tr4()
{
	exseq_main_region_PAUSE_REQUESTED();
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_lr1_lr1()
{
	ifaceMedia_OCB->resPause(ifaceMedia.pauseRequest_value, false);
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onRequest(ifaceMedia.reqPlay_value); });
}

void MediaSwitch7::effect_main_region_PAUSE_REQUESTED_lr4_lr4()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.reqOff_value); });
}

void MediaSwitch7::effect_main_region_SS_OFF_tr0()
{
	exseq_main_region_SS_OFF();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_SS_OFF_tr1()
{
	exseq_main_region_SS_OFF();
	enseq_main_region_ON_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_OFF_tr2()
{
	exseq_main_region_SS_OFF();
	enseq_main_region_SS_ON_default();
}

void MediaSwitch7::effect_main_region_SS_OFF_tr3()
{
	exseq_main_region_SS_OFF();
	enseq_main_region_OFF_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_SS_OFF_lr1_lr1()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_offRequest(ifaceMedia.reqOff_value); });
}

void MediaSwitch7::effect_main_region_SS_OFF_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_SS_OFF_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onRequest(ifaceMedia.reqPlay_value); });
}

void MediaSwitch7::effect_main_region_SS_OFF_lr4_lr4()
{
	ifaceMedia_OCB->resPause(ifaceMedia.reqPause_value, false);
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_tr0()
{
	exseq_main_region_OFF_REQUESTED();
	enseq_main_region_ON_REQUESTED_default();
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_tr1()
{
	exseq_main_region_OFF_REQUESTED();
	ifaceMedia_OCB->resOff(ifaceMedia.offRequest_value, true);
	enseq_main_region_SS_OFF_default();
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_tr2()
{
	exseq_main_region_OFF_REQUESTED();
	enseq_main_region_unknown_default();
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_lr1_lr1()
{
	ifaceMedia_OCB->resOff(ifaceMedia.offRequest_value, false);
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_lr2_lr2()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_lr3_lr3()
{
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onRequest(ifaceMedia.reqPlay_value); });
}

void MediaSwitch7::effect_main_region_OFF_REQUESTED_lr4_lr4()
{
	ifaceMedia_OCB->resPause(ifaceMedia.offRequest_value, false);
	InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
}

/* Entry action for state 'unknown'. */
void MediaSwitch7::enact_main_region_unknown()
{
	/* Entry action for state 'unknown'. */
	ifaceMedia_OCB->print((char*)"unknown");
}

/* Entry action for state 'ON_REQUESTED'. */
void MediaSwitch7::enact_main_region_ON_REQUESTED()
{
	/* Entry action for state 'ON_REQUESTED'. */
	if (! ifaceMedia_OCB->doPlay(ifaceMedia.onRequest_value))
	{ 
		InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
	} 
}

/* Entry action for state 'SS_ON'. */
void MediaSwitch7::enact_main_region_SS_ON()
{
	/* Entry action for state 'SS_ON'. */
	ifaceMedia_OCB->print((char*)"SS_ON");
}

/* Entry action for state 'SS_PAUSED'. */
void MediaSwitch7::enact_main_region_SS_PAUSED()
{
	/* Entry action for state 'SS_PAUSED'. */
	ifaceMedia_OCB->print((char*)"SS_PAUSED");
}

/* Entry action for state 'PAUSE_REQUESTED'. */
void MediaSwitch7::enact_main_region_PAUSE_REQUESTED()
{
	/* Entry action for state 'PAUSE_REQUESTED'. */
	if (! ifaceMedia_OCB->doPause(ifaceMedia.pauseRequest_value))
	{ 
		InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
	} 
}

/* Entry action for state 'SS_OFF'. */
void MediaSwitch7::enact_main_region_SS_OFF()
{
	/* Entry action for state 'SS_OFF'. */
	ifaceMedia_OCB->print((char*)"SS_OFF");
}

/* Entry action for state 'OFF_REQUESTED'. */
void MediaSwitch7::enact_main_region_OFF_REQUESTED()
{
	/* Entry action for state 'OFF_REQUESTED'. */
	if (! ifaceMedia_OCB->doOff(ifaceMedia.offRequest_value))
	{ 
		InternalEventQueue.push_back( [&] { ifaceMedia.raise_onUnknown(); });
	} 
}

/* 'default' enter sequence for state unknown */
void MediaSwitch7::enseq_main_region_unknown_default()
{
	/* 'default' enter sequence for state unknown */
	enact_main_region_unknown();
	stateConfVector[0] = main_region_unknown;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state ON_REQUESTED */
void MediaSwitch7::enseq_main_region_ON_REQUESTED_default()
{
	/* 'default' enter sequence for state ON_REQUESTED */
	enact_main_region_ON_REQUESTED();
	stateConfVector[0] = main_region_ON_REQUESTED;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SS_ON */
void MediaSwitch7::enseq_main_region_SS_ON_default()
{
	/* 'default' enter sequence for state SS_ON */
	enact_main_region_SS_ON();
	stateConfVector[0] = main_region_SS_ON;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SS_PAUSED */
void MediaSwitch7::enseq_main_region_SS_PAUSED_default()
{
	/* 'default' enter sequence for state SS_PAUSED */
	enact_main_region_SS_PAUSED();
	stateConfVector[0] = main_region_SS_PAUSED;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state PAUSE_REQUESTED */
void MediaSwitch7::enseq_main_region_PAUSE_REQUESTED_default()
{
	/* 'default' enter sequence for state PAUSE_REQUESTED */
	enact_main_region_PAUSE_REQUESTED();
	stateConfVector[0] = main_region_PAUSE_REQUESTED;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state SS_OFF */
void MediaSwitch7::enseq_main_region_SS_OFF_default()
{
	/* 'default' enter sequence for state SS_OFF */
	enact_main_region_SS_OFF();
	stateConfVector[0] = main_region_SS_OFF;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state OFF_REQUESTED */
void MediaSwitch7::enseq_main_region_OFF_REQUESTED_default()
{
	/* 'default' enter sequence for state OFF_REQUESTED */
	enact_main_region_OFF_REQUESTED();
	stateConfVector[0] = main_region_OFF_REQUESTED;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void MediaSwitch7::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state unknown */
void MediaSwitch7::exseq_main_region_unknown()
{
	/* Default exit sequence for state unknown */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state ON_REQUESTED */
void MediaSwitch7::exseq_main_region_ON_REQUESTED()
{
	/* Default exit sequence for state ON_REQUESTED */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state SS_ON */
void MediaSwitch7::exseq_main_region_SS_ON()
{
	/* Default exit sequence for state SS_ON */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state SS_PAUSED */
void MediaSwitch7::exseq_main_region_SS_PAUSED()
{
	/* Default exit sequence for state SS_PAUSED */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state PAUSE_REQUESTED */
void MediaSwitch7::exseq_main_region_PAUSE_REQUESTED()
{
	/* Default exit sequence for state PAUSE_REQUESTED */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state SS_OFF */
void MediaSwitch7::exseq_main_region_SS_OFF()
{
	/* Default exit sequence for state SS_OFF */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state OFF_REQUESTED */
void MediaSwitch7::exseq_main_region_OFF_REQUESTED()
{
	/* Default exit sequence for state OFF_REQUESTED */
	stateConfVector[0] = MediaSwitch7_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
void MediaSwitch7::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of MediaSwitch7.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_unknown :
		{
			exseq_main_region_unknown();
			break;
		}
		case main_region_ON_REQUESTED :
		{
			exseq_main_region_ON_REQUESTED();
			break;
		}
		case main_region_SS_ON :
		{
			exseq_main_region_SS_ON();
			break;
		}
		case main_region_SS_PAUSED :
		{
			exseq_main_region_SS_PAUSED();
			break;
		}
		case main_region_PAUSE_REQUESTED :
		{
			exseq_main_region_PAUSE_REQUESTED();
			break;
		}
		case main_region_SS_OFF :
		{
			exseq_main_region_SS_OFF();
			break;
		}
		case main_region_OFF_REQUESTED :
		{
			exseq_main_region_OFF_REQUESTED();
			break;
		}
		default: break;
	}
}

/* The reactions of state unknown. */
void MediaSwitch7::react_main_region_unknown()
{
	/* The reactions of state unknown. */
	if (check_main_region_unknown_tr0_tr0())
	{ 
		effect_main_region_unknown_tr0();
	}  else
	{
		if (check_main_region_unknown_tr1_tr1())
		{ 
			effect_main_region_unknown_tr1();
		}  else
		{
			if (check_main_region_unknown_tr2_tr2())
			{ 
				effect_main_region_unknown_tr2();
			}  else
			{
				if (check_main_region_unknown_tr3_tr3())
				{ 
					effect_main_region_unknown_tr3();
				}  else
				{
					if (check_main_region_unknown_tr4_tr4())
					{ 
						effect_main_region_unknown_tr4();
					}  else
					{
						if (check_main_region_unknown_tr5_tr5())
						{ 
							effect_main_region_unknown_tr5();
						}  else
						{
							if (check_main_region_unknown_lr1_lr1())
							{ 
								effect_main_region_unknown_lr1_lr1();
							} 
							if (check_main_region_unknown_lr2_lr2())
							{ 
								effect_main_region_unknown_lr2_lr2();
							} 
							if (check_main_region_unknown_lr3_lr3())
							{ 
								effect_main_region_unknown_lr3_lr3();
							} 
						}
					}
				}
			}
		}
	}
}

/* The reactions of state ON_REQUESTED. */
void MediaSwitch7::react_main_region_ON_REQUESTED()
{
	/* The reactions of state ON_REQUESTED. */
	if (check_main_region_ON_REQUESTED_tr0_tr0())
	{ 
		effect_main_region_ON_REQUESTED_tr0();
	}  else
	{
		if (check_main_region_ON_REQUESTED_tr1_tr1())
		{ 
			effect_main_region_ON_REQUESTED_tr1();
		}  else
		{
			if (check_main_region_ON_REQUESTED_tr2_tr2())
			{ 
				effect_main_region_ON_REQUESTED_tr2();
			}  else
			{
				if (check_main_region_ON_REQUESTED_tr3_tr3())
				{ 
					effect_main_region_ON_REQUESTED_tr3();
				}  else
				{
					if (check_main_region_ON_REQUESTED_tr4_tr4())
					{ 
						effect_main_region_ON_REQUESTED_tr4();
					}  else
					{
						if (check_main_region_ON_REQUESTED_lr1_lr1())
						{ 
							effect_main_region_ON_REQUESTED_lr1_lr1();
						} 
						if (check_main_region_ON_REQUESTED_lr2_lr2())
						{ 
							effect_main_region_ON_REQUESTED_lr2_lr2();
						} 
						if (check_main_region_ON_REQUESTED_lr3_lr3())
						{ 
							effect_main_region_ON_REQUESTED_lr3_lr3();
						} 
						if (check_main_region_ON_REQUESTED_lr4_lr4())
						{ 
							effect_main_region_ON_REQUESTED_lr4_lr4();
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state SS_ON. */
void MediaSwitch7::react_main_region_SS_ON()
{
	/* The reactions of state SS_ON. */
	if (check_main_region_SS_ON_tr0_tr0())
	{ 
		effect_main_region_SS_ON_tr0();
	}  else
	{
		if (check_main_region_SS_ON_tr1_tr1())
		{ 
			effect_main_region_SS_ON_tr1();
		}  else
		{
			if (check_main_region_SS_ON_tr2_tr2())
			{ 
				effect_main_region_SS_ON_tr2();
			}  else
			{
				if (check_main_region_SS_ON_tr3_tr3())
				{ 
					effect_main_region_SS_ON_tr3();
				}  else
				{
					if (check_main_region_SS_ON_tr4_tr4())
					{ 
						effect_main_region_SS_ON_tr4();
					}  else
					{
						if (check_main_region_SS_ON_tr5_tr5())
						{ 
							effect_main_region_SS_ON_tr5();
						}  else
						{
							if (check_main_region_SS_ON_lr1_lr1())
							{ 
								effect_main_region_SS_ON_lr1_lr1();
							} 
							if (check_main_region_SS_ON_lr2_lr2())
							{ 
								effect_main_region_SS_ON_lr2_lr2();
							} 
							if (check_main_region_SS_ON_lr3_lr3())
							{ 
								effect_main_region_SS_ON_lr3_lr3();
							} 
							if (check_main_region_SS_ON_lr4_lr4())
							{ 
								effect_main_region_SS_ON_lr4_lr4();
							} 
						}
					}
				}
			}
		}
	}
}

/* The reactions of state SS_PAUSED. */
void MediaSwitch7::react_main_region_SS_PAUSED()
{
	/* The reactions of state SS_PAUSED. */
	if (check_main_region_SS_PAUSED_tr0_tr0())
	{ 
		effect_main_region_SS_PAUSED_tr0();
	}  else
	{
		if (check_main_region_SS_PAUSED_tr1_tr1())
		{ 
			effect_main_region_SS_PAUSED_tr1();
		}  else
		{
			if (check_main_region_SS_PAUSED_tr2_tr2())
			{ 
				effect_main_region_SS_PAUSED_tr2();
			}  else
			{
				if (check_main_region_SS_PAUSED_tr3_tr3())
				{ 
					effect_main_region_SS_PAUSED_tr3();
				}  else
				{
					if (check_main_region_SS_PAUSED_tr4_tr4())
					{ 
						effect_main_region_SS_PAUSED_tr4();
					}  else
					{
						if (check_main_region_SS_PAUSED_lr1_lr1())
						{ 
							effect_main_region_SS_PAUSED_lr1_lr1();
						} 
						if (check_main_region_SS_PAUSED_lr2_lr2())
						{ 
							effect_main_region_SS_PAUSED_lr2_lr2();
						} 
						if (check_main_region_SS_PAUSED_lr3_lr3())
						{ 
							effect_main_region_SS_PAUSED_lr3_lr3();
						} 
						if (check_main_region_SS_PAUSED_lr4_lr4())
						{ 
							effect_main_region_SS_PAUSED_lr4_lr4();
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state PAUSE_REQUESTED. */
void MediaSwitch7::react_main_region_PAUSE_REQUESTED()
{
	/* The reactions of state PAUSE_REQUESTED. */
	if (check_main_region_PAUSE_REQUESTED_tr0_tr0())
	{ 
		effect_main_region_PAUSE_REQUESTED_tr0();
	}  else
	{
		if (check_main_region_PAUSE_REQUESTED_tr1_tr1())
		{ 
			effect_main_region_PAUSE_REQUESTED_tr1();
		}  else
		{
			if (check_main_region_PAUSE_REQUESTED_tr2_tr2())
			{ 
				effect_main_region_PAUSE_REQUESTED_tr2();
			}  else
			{
				if (check_main_region_PAUSE_REQUESTED_tr3_tr3())
				{ 
					effect_main_region_PAUSE_REQUESTED_tr3();
				}  else
				{
					if (check_main_region_PAUSE_REQUESTED_tr4_tr4())
					{ 
						effect_main_region_PAUSE_REQUESTED_tr4();
					}  else
					{
						if (check_main_region_PAUSE_REQUESTED_lr1_lr1())
						{ 
							effect_main_region_PAUSE_REQUESTED_lr1_lr1();
						} 
						if (check_main_region_PAUSE_REQUESTED_lr2_lr2())
						{ 
							effect_main_region_PAUSE_REQUESTED_lr2_lr2();
						} 
						if (check_main_region_PAUSE_REQUESTED_lr3_lr3())
						{ 
							effect_main_region_PAUSE_REQUESTED_lr3_lr3();
						} 
						if (check_main_region_PAUSE_REQUESTED_lr4_lr4())
						{ 
							effect_main_region_PAUSE_REQUESTED_lr4_lr4();
						} 
					}
				}
			}
		}
	}
}

/* The reactions of state SS_OFF. */
void MediaSwitch7::react_main_region_SS_OFF()
{
	/* The reactions of state SS_OFF. */
	if (check_main_region_SS_OFF_tr0_tr0())
	{ 
		effect_main_region_SS_OFF_tr0();
	}  else
	{
		if (check_main_region_SS_OFF_tr1_tr1())
		{ 
			effect_main_region_SS_OFF_tr1();
		}  else
		{
			if (check_main_region_SS_OFF_tr2_tr2())
			{ 
				effect_main_region_SS_OFF_tr2();
			}  else
			{
				if (check_main_region_SS_OFF_tr3_tr3())
				{ 
					effect_main_region_SS_OFF_tr3();
				}  else
				{
					if (check_main_region_SS_OFF_lr1_lr1())
					{ 
						effect_main_region_SS_OFF_lr1_lr1();
					} 
					if (check_main_region_SS_OFF_lr2_lr2())
					{ 
						effect_main_region_SS_OFF_lr2_lr2();
					} 
					if (check_main_region_SS_OFF_lr3_lr3())
					{ 
						effect_main_region_SS_OFF_lr3_lr3();
					} 
					if (check_main_region_SS_OFF_lr4_lr4())
					{ 
						effect_main_region_SS_OFF_lr4_lr4();
					} 
				}
			}
		}
	}
}

/* The reactions of state OFF_REQUESTED. */
void MediaSwitch7::react_main_region_OFF_REQUESTED()
{
	/* The reactions of state OFF_REQUESTED. */
	if (check_main_region_OFF_REQUESTED_tr0_tr0())
	{ 
		effect_main_region_OFF_REQUESTED_tr0();
	}  else
	{
		if (check_main_region_OFF_REQUESTED_tr1_tr1())
		{ 
			effect_main_region_OFF_REQUESTED_tr1();
		}  else
		{
			if (check_main_region_OFF_REQUESTED_tr2_tr2())
			{ 
				effect_main_region_OFF_REQUESTED_tr2();
			}  else
			{
				if (check_main_region_OFF_REQUESTED_lr1_lr1())
				{ 
					effect_main_region_OFF_REQUESTED_lr1_lr1();
				} 
				if (check_main_region_OFF_REQUESTED_lr2_lr2())
				{ 
					effect_main_region_OFF_REQUESTED_lr2_lr2();
				} 
				if (check_main_region_OFF_REQUESTED_lr3_lr3())
				{ 
					effect_main_region_OFF_REQUESTED_lr3_lr3();
				} 
				if (check_main_region_OFF_REQUESTED_lr4_lr4())
				{ 
					effect_main_region_OFF_REQUESTED_lr4_lr4();
				} 
			}
		}
	}
}

/* Default react sequence for initial entry  */
void MediaSwitch7::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_unknown_default();
}


