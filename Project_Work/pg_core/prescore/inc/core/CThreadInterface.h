/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CThreadInterface.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

* @brief           CThreadInterface can be used by clients for creating and terminating threads.Client can 
                   create an object of CThreadInterface for doing various thread operations. Also client can 
				   use the same CThreadInterface object for creating multiple independent threads. The client will 
				   be provided a unique thread ID for each thread which is created.

                   CThreadInterface abstracts the underying OS (windows/linux) for which the thread 
				   is created.			

				   Also when the CThreadInterface object goes out of scope, it is better if the client invokes
				   joinThread and exit the thread gracefully. In the core, there is a clean up mechanism provided
				   to automatically join the associated thread when CThreadInterface object goes out of scope .
				   It has to be noted that since thread join has an infinite  timeout,client should not indefinitely block
				   in their associated thread function.

				   For starting a thread,client has to invoke startThread() and pass the following arguments:

 	               :1) Reference input(threadId) - Prescore will initialise this with the ID of the newly created thread only 
                                                if startThread returns successfully. The provided threadId can be used by the 
				                                client later for joining or terminating the thread.			            

                   :2) threadFunc -             The client thread function which has to be executed in the newly created thread context.This is equivalent to 
				                                the run() function which u generally encounter with OS threads.This function has to be 
											    implemented by the client.The core will invoke this function after the client invokes startThread.

				                               Note that threadFunc takes 2 arguments:

											   -a) IThreadDataHandler::SendDataFunction&- This is a function that is needed
											    to dispatch the thread data to the mainloop context.Note that this is a reference parameter.
												The client need not initialize this parameter.It is the responsibility of the core to initialize 
												SendDataFunction when the client invokes startThread.

												For example, assume that client creates a new thread for reading from PPS.Now client might have 
												to send the PPS data 
												periodically to the mainloop for processing. To do this, client can pack all the thread related
												data into a class object derived from BaseData (Declared in IDataHandler.h) and just invoke 
												sendDataFunction(BaseData*).Please make sure that BaseData* is valid.It is better to allocate 
												memory in the heap and the core shall destroy it later.
											
											   Now it is the repsonbility of the core to dispatch this data and invoke onData (see details below)
											   function from the mainloop context.

											   onData function is the function that has to be implemented by the client to handle/process the
											   thread data sent from the threadFunc using sendDataFunction().This is a pure virtual function 
											   declared in IDataHandler.h and it has to be implemented by the client.

											   Note:Also since BaseData class has a pure virtual destructor, the core shall destroy the BaseData*
											   after invoking onData function.

											   For the signature of sendDataFunction() and onData function and BaseData class,please refer IDataHandler.h file. 

											   b) Args- This is the multiple argument list for the thread. Generaly with OS threads,we pack
											   all thread arguments into (void*), but since c++ 11 supports variadic templates,we use this feature.


				   : 3) args-                      Argument list for the threadFunc.

				   Note:For more example on how to use CthreadInterface for starting/joining thread,please refer the sample code in 
				   CoreTest.cpp file.

*/

#ifndef _CTHREADINTERFACE_HPP_
#define _CTHREADINTERFACE_HPP_

#include<list>
#include<map>
#include<thread>
#include"logging.h"
#include"CThreadManager.h"

class DLL_EXPORT CThreadInterface
{
public:

     CThreadInterface(IThreadDataHandler& dataHandler);

	~CThreadInterface();

	/* creates & starts a thread and returns true if thread creation is successfull otherwise false.

	Input:

	threadId:This is a reference input.This will be initialized with the id of the created.Clients can use this id to identify a thread uniquely.
	threadFunc:This is the thread function which has to be executed. 
	
	Note:ThreadFunc must be a static class Function !!

	ts...: This is the variable argument list to the thread function threadFunc
    
	Output:
	True if thread creation is successful.
	False if thread creation fails.
	*/
   template<typename...ts>
   bool startThread(int& threadId,void (*threadFunc)(IThreadDataHandler::SendDataFunction &,ts...),ts... arg )
   {
	   int id = -1;
	   bool status = false;
	   id = CThreadManager::self()->startThread(threadId, mDataHandler, threadFunc, arg...);
	   if (id > 0)
	   {
		   mThreadIdList.push_back(id);
		   status = true;
	   }
	   else
	   {
		   LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CThreadInterface::createThread - thread creation failed");
	   }

	   threadId = id;

	   return status;
   }
	
	//joins a thread identified by the threadId.
	void joinThread(int threadId);

private:
	std::list<int> mThreadIdList;
	IThreadDataHandler& mDataHandler;
};

#endif