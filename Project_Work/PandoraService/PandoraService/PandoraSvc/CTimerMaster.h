/*
 * 
 */
#include "IThreadInterface.h"
#include "CTimer.h"
#include <unistd.h>
#include <stdint.h>
#include <list>
#include <map>

using namespace std;

#ifndef CTIMERMASTER_H_
#define CTIMERMASTER_H_


// Class waits to receive timer events. You can create a new timer object and
// Add to Timer Master
class CTimerMaster : public IThreadInterface
{
private:
	// Constructor
	CTimerMaster();

	// Destructor
	~CTimerMaster();

private:
	// Reference count to remember how many timer objects are being used
	static uint32_t 					m_RefCount;

	// Timer master object
	static CTimerMaster*				m_TimerMaster;

	int32_t 							m_ChannelId;

	struct sigevent 					m_Event;

	_Int8t 								m_CodeIdIndex;

	typedef std::map<short, CTimer*>	tTimerMap;

	tTimerMap							m_TimerMap;

	bool								m_Active;

private:
	// max upto
	_Int8t 					generateCodeId();

	void					addRefCount ();

	void					releaseRefCount();

	bool					initializeTimerChannel ();

	void 					resetTimerData ();

public:
	// runs in the thread context
	virtual void 			run();

	// get the timerMaster object
	static CTimerMaster* 	getTimerMaster ();

	// release Timer master object
	static void 			releaseTimerMaster ();

	bool					addTimer (CTimer* pTimer, bool repeat = true);

	bool					removeTimer (CTimer* pTimer);
};

#endif /* CTIMERMASTER_H_ */
