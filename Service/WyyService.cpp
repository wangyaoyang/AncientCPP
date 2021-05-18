#include <stdio.h>
#include <time.h>
#include "WyyService.H"

HANDLE					hThreadEvt[MAX_NUM_OF_THREADS];
HANDLE					hServerStopEvent = NULL;
SERVICE_STATUS          ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwErr = 0;
DWORD					dwThreadNo = 0;


////////////////////////////////////////////////////////////////////////////////////
VOID WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI service_ctrl(DWORD dwCtrlCode);
BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
VOID ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv);
VOID ServiceStop();
VOID Stop_all_threads(VOID);

void main_entry(void)
{
    SERVICE_TABLE_ENTRY dispatchTable[] =
    {
        { TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)service_main },
        { NULL, NULL }
    };

	for( int i=0;i<MAX_NUM_OF_THREADS;i++ ) hThreadEvt[i] = NULL;
    if(!StartServiceCtrlDispatcher(dispatchTable))
		printf("StartServiceCtrlDispatcher failed.\n");
}

////////////////////////////////////////////////////////////////////////////////////

void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{	//ServiceMain()
    sshStatusHandle = RegisterServiceCtrlHandler( TEXT(SZSERVICENAME), service_ctrl);

    if (!sshStatusHandle) goto cleanup;

    ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ssStatus.dwServiceSpecificExitCode = 0;

    if (!ReportStatusToSCMgr( SERVICE_START_PENDING, NO_ERROR, 3000)) goto cleanup;

	ServiceStart( dwArgc, lpszArgv );

cleanup:

    if (sshStatusHandle) ReportStatusToSCMgr( SERVICE_STOPPED, dwErr, 0 );
    return;
}

VOID WINAPI service_ctrl(DWORD dwCtrlCode)
{	//Handler()
    switch(dwCtrlCode)
    {
        case SERVICE_CONTROL_STOP:
			ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 3000);
			ServiceStop();
			ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
			break;
        case SERVICE_CONTROL_INTERROGATE:
			break;
        default:
            break;
    }

    ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
}

BOOL ReportStatusToSCMgr(DWORD dwCurrentState,DWORD dwWin32ExitCode,DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;
    BOOL fResult = TRUE;

    if (dwCurrentState == SERVICE_START_PENDING)
			ssStatus.dwControlsAccepted = 0;
	else	ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    ssStatus.dwCurrentState = dwCurrentState;
    ssStatus.dwWin32ExitCode = dwWin32ExitCode;
    ssStatus.dwWaitHint = dwWaitHint;

    if ( ( dwCurrentState == SERVICE_RUNNING ) ||
         ( dwCurrentState == SERVICE_STOPPED ) )
	        ssStatus.dwCheckPoint = 0;
    else	ssStatus.dwCheckPoint = dwCheckPoint++;

    if (!(fResult = SetServiceStatus( sshStatusHandle, &ssStatus)))
		printf("SetServiceStatus\n");
    return fResult;
}
////////////////////////////////////////////////////////////////////////////////////

VOID ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
	DWORD	dwWait;

    if( !ReportStatusToSCMgr( SERVICE_START_PENDING, NO_ERROR, 3000 ) ) goto cleanup;
	if( (hServerStopEvent=CreateEvent( NULL, TRUE, FALSE, NULL))==NULL ) goto cleanup;
    if( !ReportStatusToSCMgr( SERVICE_START_PENDING, NO_ERROR, 3000 ) ) goto cleanup;
	if( !StartServiceThreads() ) goto cleanup;
    if( !ReportStatusToSCMgr( SERVICE_RUNNING, NO_ERROR, 0 ) ) goto cleanup;

	dwWait = WaitForSingleObject( hServerStopEvent,INFINITE );
cleanup:
	Stop_all_threads();
	if( hServerStopEvent ) { CloseHandle( hServerStopEvent ); hServerStopEvent = NULL; }
	ReportStatusToSCMgr( SERVICE_STOP_PENDING, NO_ERROR, 0 );
}

VOID ServiceStop()
{
    if ( hServerStopEvent ) SetEvent(hServerStopEvent);
}

////////////////////////////////////////////////////////////////////////////////////
HANDLE Start_new_thread(DWORD dwTNo,LPTHREAD_START_ROUTINE pNewThread)
{
	DWORD			dwTId=0;
	HANDLE			hThread=NULL;

	dwThreadNo = dwTNo;
	if( (hThreadEvt[dwTNo]=CreateEvent(NULL,TRUE,FALSE,NULL))==NULL ) return NULL;
	hThread = CreateThread( NULL, 0, pNewThread, &dwThreadNo, 0, &dwTId );
	return hThread;
}

VOID Stop_all_threads()
{
	for( int i=0;i<MAX_NUM_OF_THREADS;i++ )
		if( hThreadEvt[i] ) SetEvent(hThreadEvt[i]);
}

BOOL Test_single_state(HANDLE& hEvent)
{
	DWORD	dwWait = 0;

	if( hEvent == NULL ) return FALSE;
	dwWait = WaitForSingleObject( hEvent,0 );
	switch( dwWait )
	{
	case WAIT_ABANDONED:
		// The specified object is a mutex object that was not released by the
		// thread that owned the mutex object before the owning thread terminated.
		// Ownership of the mutex object is granted to the calling thread, and the
		// mutex is set to nonsignaled.
		return FALSE;
	case WAIT_OBJECT_0:
		// The state of the specified object is signaled.
		break;
	case WAIT_TIMEOUT:
		// The time-out interval elapsed, and the object's state is nonsignaled.
		return FALSE;
	default:;
	}
	CloseHandle( hEvent ); hEvent = NULL;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////

