
#ifndef MEDIASWITCH7_H_
#define MEDIASWITCH7_H_

#include "sc_types.h"
#include "StatemachineInterface.h"
#include <deque>
#include <functional>

/*! \file Header of the state machine 'MediaSwitch7'.
*/

class MediaSwitch7 : public StatemachineInterface
{
	
	public:
		
		MediaSwitch7();
		
		~MediaSwitch7();
		
		/*! Enumeration of all states */ 
		typedef enum
		{
			main_region_unknown,
			main_region_ON_REQUESTED,
			main_region_SS_ON,
			main_region_SS_PAUSED,
			main_region_PAUSE_REQUESTED,
			main_region_SS_OFF,
			main_region_OFF_REQUESTED,
			MediaSwitch7_last_state
		} MediaSwitch7States;
		
		//! Inner class for default interface scope.
		class DefaultSCI
		{
			
			public:
				
			private:
				friend class MediaSwitch7;
		};
				
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		//! Inner class for Media interface scope.
		class SCI_Media
		{
			
			public:
				/*! Raises the in event 'onPlaying' that is defined in the interface scope 'Media'. */ 
				void raise_onPlaying();
				
				/*! Raises the in event 'onPaused' that is defined in the interface scope 'Media'. */ 
				void raise_onPaused();
				
				/*! Raises the in event 'onStopped' that is defined in the interface scope 'Media'. */ 
				void raise_onStopped();
				
				/*! Raises the in event 'reqPlay' that is defined in the interface scope 'Media'. */ 
				void raise_reqPlay(sc_integer value);
				
				/*! Raises the in event 'reqPause' that is defined in the interface scope 'Media'. */ 
				void raise_reqPause(sc_integer value);
				
				/*! Raises the in event 'reqOff' that is defined in the interface scope 'Media'. */ 
				void raise_reqOff(sc_integer value);
				
				/*! Raises the in event 'reqTimeout' that is defined in the interface scope 'Media'. */ 
				void raise_reqTimeout();
				
				/*! Raises the in event 'devErr' that is defined in the interface scope 'Media'. */ 
				void raise_devErr(sc_integer value);
				
				/*! Raises the in event 'onRequest' that is defined in the interface scope 'Media'. */ 
				void raise_onRequest(sc_integer value);
				
				/*! Raises the in event 'pauseRequest' that is defined in the interface scope 'Media'. */ 
				void raise_pauseRequest(sc_integer value);
				
				/*! Raises the in event 'offRequest' that is defined in the interface scope 'Media'. */ 
				void raise_offRequest(sc_integer value);
				
				/*! Raises the in event 'onUnknown' that is defined in the interface scope 'Media'. */ 
				void raise_onUnknown();
				
				
			private:
				friend class MediaSwitch7;
				sc_boolean onPlaying_raised;
				sc_boolean onPaused_raised;
				sc_boolean onStopped_raised;
				sc_boolean reqPlay_raised;
				sc_integer reqPlay_value;
				sc_boolean reqPause_raised;
				sc_integer reqPause_value;
				sc_boolean reqOff_raised;
				sc_integer reqOff_value;
				sc_boolean reqTimeout_raised;
				sc_boolean devErr_raised;
				sc_integer devErr_value;
				sc_boolean onRequest_raised;
				sc_integer onRequest_value;
				sc_boolean pauseRequest_raised;
				sc_integer pauseRequest_value;
				sc_boolean offRequest_raised;
				sc_integer offRequest_value;
				sc_boolean onUnknown_raised;
		};
				
				//! Inner class for Media interface scope operation callbacks.
				class SCI_Media_OCB
				{
					public:
						virtual ~SCI_Media_OCB() = 0;
						
						virtual sc_boolean doPlay(sc_integer sourceId) = 0;
						
						virtual sc_boolean doPause(sc_integer sourceId) = 0;
						
						virtual sc_boolean doOff(sc_integer sourceId) = 0;
						
						virtual void resPlay(sc_integer sourceId, sc_boolean status) = 0;
						
						virtual void resPause(sc_integer sourceId, sc_boolean status) = 0;
						
						virtual void resOff(sc_integer sourceId, sc_boolean status) = 0;
						
						virtual void print(sc_string name) = 0;
				};
				
				/*! Set the working instance of the operation callback interface 'SCI_Media_OCB'. */
				void setSCI_Media_OCB(SCI_Media_OCB* operationCallback);
		
		/*! Returns an instance of the interface class 'SCI_Media'. */
		SCI_Media* getSCI_Media();
		
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		/*!
		* Checks if the state machine is active (until 2.4.1 this method was used for states).
		* A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
		*/
		sc_boolean isActive();
		
		
		/*!
		* Checks if all active states are final. 
		* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
		*/
		sc_boolean isFinal();
		
		
		/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
		sc_boolean isStateActive(MediaSwitch7States state);
	
	private:
	
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 1;
		
		
		MediaSwitch7States stateConfVector[maxOrthogonalStates];
		
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		SCI_Media ifaceMedia;
		SCI_Media_OCB* ifaceMedia_OCB;
		
		// prototypes of all internal functions
		
		sc_boolean check_main_region_unknown_tr0_tr0();
		sc_boolean check_main_region_unknown_tr1_tr1();
		sc_boolean check_main_region_unknown_tr2_tr2();
		sc_boolean check_main_region_unknown_tr3_tr3();
		sc_boolean check_main_region_unknown_tr4_tr4();
		sc_boolean check_main_region_unknown_tr5_tr5();
		sc_boolean check_main_region_unknown_lr1_lr1();
		sc_boolean check_main_region_unknown_lr2_lr2();
		sc_boolean check_main_region_unknown_lr3_lr3();
		sc_boolean check_main_region_ON_REQUESTED_tr0_tr0();
		sc_boolean check_main_region_ON_REQUESTED_tr1_tr1();
		sc_boolean check_main_region_ON_REQUESTED_tr2_tr2();
		sc_boolean check_main_region_ON_REQUESTED_tr3_tr3();
		sc_boolean check_main_region_ON_REQUESTED_tr4_tr4();
		sc_boolean check_main_region_ON_REQUESTED_lr1_lr1();
		sc_boolean check_main_region_ON_REQUESTED_lr2_lr2();
		sc_boolean check_main_region_ON_REQUESTED_lr3_lr3();
		sc_boolean check_main_region_ON_REQUESTED_lr4_lr4();
		sc_boolean check_main_region_SS_ON_tr0_tr0();
		sc_boolean check_main_region_SS_ON_tr1_tr1();
		sc_boolean check_main_region_SS_ON_tr2_tr2();
		sc_boolean check_main_region_SS_ON_tr3_tr3();
		sc_boolean check_main_region_SS_ON_tr4_tr4();
		sc_boolean check_main_region_SS_ON_tr5_tr5();
		sc_boolean check_main_region_SS_ON_lr1_lr1();
		sc_boolean check_main_region_SS_ON_lr2_lr2();
		sc_boolean check_main_region_SS_ON_lr3_lr3();
		sc_boolean check_main_region_SS_ON_lr4_lr4();
		sc_boolean check_main_region_SS_PAUSED_tr0_tr0();
		sc_boolean check_main_region_SS_PAUSED_tr1_tr1();
		sc_boolean check_main_region_SS_PAUSED_tr2_tr2();
		sc_boolean check_main_region_SS_PAUSED_tr3_tr3();
		sc_boolean check_main_region_SS_PAUSED_tr4_tr4();
		sc_boolean check_main_region_SS_PAUSED_lr1_lr1();
		sc_boolean check_main_region_SS_PAUSED_lr2_lr2();
		sc_boolean check_main_region_SS_PAUSED_lr3_lr3();
		sc_boolean check_main_region_SS_PAUSED_lr4_lr4();
		sc_boolean check_main_region_PAUSE_REQUESTED_tr0_tr0();
		sc_boolean check_main_region_PAUSE_REQUESTED_tr1_tr1();
		sc_boolean check_main_region_PAUSE_REQUESTED_tr2_tr2();
		sc_boolean check_main_region_PAUSE_REQUESTED_tr3_tr3();
		sc_boolean check_main_region_PAUSE_REQUESTED_tr4_tr4();
		sc_boolean check_main_region_PAUSE_REQUESTED_lr1_lr1();
		sc_boolean check_main_region_PAUSE_REQUESTED_lr2_lr2();
		sc_boolean check_main_region_PAUSE_REQUESTED_lr3_lr3();
		sc_boolean check_main_region_PAUSE_REQUESTED_lr4_lr4();
		sc_boolean check_main_region_SS_OFF_tr0_tr0();
		sc_boolean check_main_region_SS_OFF_tr1_tr1();
		sc_boolean check_main_region_SS_OFF_tr2_tr2();
		sc_boolean check_main_region_SS_OFF_tr3_tr3();
		sc_boolean check_main_region_SS_OFF_lr1_lr1();
		sc_boolean check_main_region_SS_OFF_lr2_lr2();
		sc_boolean check_main_region_SS_OFF_lr3_lr3();
		sc_boolean check_main_region_SS_OFF_lr4_lr4();
		sc_boolean check_main_region_OFF_REQUESTED_tr0_tr0();
		sc_boolean check_main_region_OFF_REQUESTED_tr1_tr1();
		sc_boolean check_main_region_OFF_REQUESTED_tr2_tr2();
		sc_boolean check_main_region_OFF_REQUESTED_lr1_lr1();
		sc_boolean check_main_region_OFF_REQUESTED_lr2_lr2();
		sc_boolean check_main_region_OFF_REQUESTED_lr3_lr3();
		sc_boolean check_main_region_OFF_REQUESTED_lr4_lr4();
		void effect_main_region_unknown_tr0();
		void effect_main_region_unknown_tr1();
		void effect_main_region_unknown_tr2();
		void effect_main_region_unknown_tr3();
		void effect_main_region_unknown_tr4();
		void effect_main_region_unknown_tr5();
		void effect_main_region_unknown_lr1_lr1();
		void effect_main_region_unknown_lr2_lr2();
		void effect_main_region_unknown_lr3_lr3();
		void effect_main_region_ON_REQUESTED_tr0();
		void effect_main_region_ON_REQUESTED_tr1();
		void effect_main_region_ON_REQUESTED_tr2();
		void effect_main_region_ON_REQUESTED_tr3();
		void effect_main_region_ON_REQUESTED_tr4();
		void effect_main_region_ON_REQUESTED_lr1_lr1();
		void effect_main_region_ON_REQUESTED_lr2_lr2();
		void effect_main_region_ON_REQUESTED_lr3_lr3();
		void effect_main_region_ON_REQUESTED_lr4_lr4();
		void effect_main_region_SS_ON_tr0();
		void effect_main_region_SS_ON_tr1();
		void effect_main_region_SS_ON_tr2();
		void effect_main_region_SS_ON_tr3();
		void effect_main_region_SS_ON_tr4();
		void effect_main_region_SS_ON_tr5();
		void effect_main_region_SS_ON_lr1_lr1();
		void effect_main_region_SS_ON_lr2_lr2();
		void effect_main_region_SS_ON_lr3_lr3();
		void effect_main_region_SS_ON_lr4_lr4();
		void effect_main_region_SS_PAUSED_tr0();
		void effect_main_region_SS_PAUSED_tr1();
		void effect_main_region_SS_PAUSED_tr2();
		void effect_main_region_SS_PAUSED_tr3();
		void effect_main_region_SS_PAUSED_tr4();
		void effect_main_region_SS_PAUSED_lr1_lr1();
		void effect_main_region_SS_PAUSED_lr2_lr2();
		void effect_main_region_SS_PAUSED_lr3_lr3();
		void effect_main_region_SS_PAUSED_lr4_lr4();
		void effect_main_region_PAUSE_REQUESTED_tr0();
		void effect_main_region_PAUSE_REQUESTED_tr1();
		void effect_main_region_PAUSE_REQUESTED_tr2();
		void effect_main_region_PAUSE_REQUESTED_tr3();
		void effect_main_region_PAUSE_REQUESTED_tr4();
		void effect_main_region_PAUSE_REQUESTED_lr1_lr1();
		void effect_main_region_PAUSE_REQUESTED_lr2_lr2();
		void effect_main_region_PAUSE_REQUESTED_lr3_lr3();
		void effect_main_region_PAUSE_REQUESTED_lr4_lr4();
		void effect_main_region_SS_OFF_tr0();
		void effect_main_region_SS_OFF_tr1();
		void effect_main_region_SS_OFF_tr2();
		void effect_main_region_SS_OFF_tr3();
		void effect_main_region_SS_OFF_lr1_lr1();
		void effect_main_region_SS_OFF_lr2_lr2();
		void effect_main_region_SS_OFF_lr3_lr3();
		void effect_main_region_SS_OFF_lr4_lr4();
		void effect_main_region_OFF_REQUESTED_tr0();
		void effect_main_region_OFF_REQUESTED_tr1();
		void effect_main_region_OFF_REQUESTED_tr2();
		void effect_main_region_OFF_REQUESTED_lr1_lr1();
		void effect_main_region_OFF_REQUESTED_lr2_lr2();
		void effect_main_region_OFF_REQUESTED_lr3_lr3();
		void effect_main_region_OFF_REQUESTED_lr4_lr4();
		void enact_main_region_unknown();
		void enact_main_region_ON_REQUESTED();
		void enact_main_region_SS_ON();
		void enact_main_region_SS_PAUSED();
		void enact_main_region_PAUSE_REQUESTED();
		void enact_main_region_SS_OFF();
		void enact_main_region_OFF_REQUESTED();
		void enseq_main_region_unknown_default();
		void enseq_main_region_ON_REQUESTED_default();
		void enseq_main_region_SS_ON_default();
		void enseq_main_region_SS_PAUSED_default();
		void enseq_main_region_PAUSE_REQUESTED_default();
		void enseq_main_region_SS_OFF_default();
		void enseq_main_region_OFF_REQUESTED_default();
		void enseq_main_region_default();
		void exseq_main_region_unknown();
		void exseq_main_region_ON_REQUESTED();
		void exseq_main_region_SS_ON();
		void exseq_main_region_SS_PAUSED();
		void exseq_main_region_PAUSE_REQUESTED();
		void exseq_main_region_SS_OFF();
		void exseq_main_region_OFF_REQUESTED();
		void exseq_main_region();
		void react_main_region_unknown();
		void react_main_region_ON_REQUESTED();
		void react_main_region_SS_ON();
		void react_main_region_SS_PAUSED();
		void react_main_region_PAUSE_REQUESTED();
		void react_main_region_SS_OFF();
		void react_main_region_OFF_REQUESTED();
		void react_main_region__entry_Default();
		void clearInEvents();
		void clearOutEvents();
		
	private:
	void runCycleIntern();
	std::deque<std::function<void()>> InternalEventQueue; 	
};
inline MediaSwitch7::SCI_Media_OCB::~SCI_Media_OCB() {}
#endif /* MEDIASWITCH7_H_ */
