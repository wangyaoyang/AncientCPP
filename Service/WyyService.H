#ifndef		__WYY_SERVICE_H__
#define		__WYY_SERVICE_H__

//////////////////////////////////////////////////////////////////////////
//	You can modify the service name and service display name here		//
//////////////////////////////////////////////////////////////////////////
#include "windows.h"

#define		SZSERVICENAME			"Tm90_nts_service"	//Fill your own service name here
#define		SZSERVICEDISPLAYNAME	"TM90 NTS Service"	//Fill your own service display name here
#define		MAX_NUM_OF_THREADS		2					//Number of threads in your service

//////////////////////////////////////////////////////////////////////////
//	Do not modify the following codes									//
//////////////////////////////////////////////////////////////////////////
extern	HANDLE		hThreadEvt[MAX_NUM_OF_THREADS];
void		main_entry(void);
HANDLE		Start_new_thread(DWORD No,LPTHREAD_START_ROUTINE pNewThread);
VOID		Stop_all_threads();
BOOL		Test_single_state(HANDLE& hEvent);
//////////////////////////////////////////////////////////////////////////

BOOL		StartServiceThreads();	//Developer muse be over write this function.
VOID		StopServiceThreads();	//Developer could be use this function when debug.
//////////////////////////////////////////////////////////////////////////

#endif		//__WYY_SERVICE_H__

//////////////////////////////////////////////////////////////////////////
//	You can add your own defination here								//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

