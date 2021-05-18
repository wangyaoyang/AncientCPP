//#include "windows.h"
#include "stdafx.h"
#include "resource.h"
#include "ServiceSetup.h"

CServiceSetup::CServiceSetup()
{
}

CServiceSetup::~CServiceSetup()
{
}

BOOL CServiceSetup::OpenSCM(SC_HANDLE& schSCManager)
{
	schSCManager = OpenSCManager(
                        NULL,					// machine (NULL == local)
                        NULL,					// database (NULL == default)
                        SC_MANAGER_ALL_ACCESS);	// access required
	if( schSCManager ) return TRUE; else return FALSE;
}

BOOL CServiceSetup::OpenSVC(SC_HANDLE& schSCManager,SC_HANDLE& schService,DWORD fdwAccess)
{
	schService = OpenService(
			schSCManager,			//SCManager database
			TEXT(m_szServiceName),	//name of service
			fdwAccess);
	if( schService ) return TRUE; else return FALSE;
}
                        
VOID CServiceSetup::Close(SC_HANDLE& scHandle)
{
	if( scHandle ) { CloseServiceHandle( scHandle ); scHandle = NULL; }
}
                        
VOID CServiceSetup::InstallService()
{
	SC_HANDLE		schSCManager = NULL;
	SC_HANDLE		schService = NULL;

	if( !OpenSCM(schSCManager) ) return;
    schService = CreateService(
            schSCManager,               // SCManager database
            TEXT(m_szServiceName),        // name of service
            TEXT(m_szServiceDisplayName), // name to display
            SERVICE_ALL_ACCESS,         // desired access
            SERVICE_WIN32_OWN_PROCESS,  // service type
            SERVICE_AUTO_START,       // start type
            SERVICE_ERROR_NORMAL,       // error control type
            m_szPath,                     // service's binary
            NULL,                       // no load ordering group
            NULL,                       // no tag identifier
            NULL,						// dependencies
            NULL,                       // LocalSystem account
            NULL);                      // no password

    Close(schService);
	Close(schSCManager);
}

VOID CServiceSetup::RemoveService()
{
	SC_HANDLE		schSCManager = NULL;
	SC_HANDLE		schService = NULL;

    if( !OpenSCM(schSCManager) ) return;
	if( !OpenSVC(schSCManager,schService) ) return;

    if( ControlService( schService, SERVICE_CONTROL_STOP, &m_ssStatus ) )
    {
        Sleep( 1000 );
        while( QueryServiceStatus( schService, &m_ssStatus ) )
        {
            if ( m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
					Sleep( 1000 );
			else	break;
        }
        if ( m_ssStatus.dwCurrentState == SERVICE_STOPPED )
				MessageBox( NULL,"stopped.",m_szServiceDisplayName,MB_ICONINFORMATION );
		else	MessageBox( NULL,"failed to stop.",m_szServiceDisplayName,MB_ICONINFORMATION );
    }
    if( DeleteService(schService) )
			MessageBox( NULL,"removed.",m_szServiceDisplayName,MB_ICONINFORMATION );
    else	MessageBox( NULL,"DeleteService failed",m_szServiceDisplayName,MB_ICONINFORMATION );

	Close(schService);
	Close(schSCManager);
}

VOID CServiceSetup::StartupService()
{
	DWORD		dwOldCheckPoint;
	SC_HANDLE	schSCManager = NULL;
	SC_HANDLE	schService = NULL;

	if( !OpenSCM(schSCManager) ) return;
	if( !OpenSVC(schSCManager,schService) ) return;

	if( schService == NULL ) MessageBox( NULL,"OpenService failed.",
									m_szServiceDisplayName,MB_ICONINFORMATION );
	if( !StartService(schService,0,NULL) ) MessageBox( NULL,"StartService failed.",
									m_szServiceDisplayName,MB_ICONINFORMATION );
	if( !QueryServiceStatus(schService,&m_ssStatus) ) MessageBox( NULL,"QueryServiceStatus failed.",
									m_szServiceDisplayName,MB_ICONINFORMATION );
	while( m_ssStatus.dwCurrentState != SERVICE_RUNNING )
	{
		dwOldCheckPoint = m_ssStatus.dwCheckPoint;
		Sleep( m_ssStatus.dwWaitHint );
		if( !QueryServiceStatus(schService,&m_ssStatus) ) break;
		if( dwOldCheckPoint >= m_ssStatus.dwCheckPoint ) break;
	}
	if( m_ssStatus.dwCurrentState == SERVICE_RUNNING )
		MessageBox( NULL,"OK","Service started",MB_OK );

	Close(schService);
	Close(schSCManager);
}

VOID CServiceSetup::ManageService(DWORD fdwControl)
{
	DWORD			fdwAccess;
	SC_HANDLE		schSCManager = NULL;
	SC_HANDLE		schService = NULL;

	switch( fdwControl )
	{
	case SERVICE_CONTROL_STOP:			fdwAccess = SERVICE_STOP;				break;
	case SERVICE_CONTROL_PAUSE:
	case SERVICE_CONTROL_CONTINUE:		fdwAccess = SERVICE_PAUSE_CONTINUE;		break;
	case SERVICE_CONTROL_INTERROGATE:	fdwAccess = SERVICE_INTERROGATE;		break;
	default:							fdwAccess = SERVICE_INTERROGATE;
	}

	if( !OpenSCM(schSCManager) ) return;
	if( !OpenSVC(schSCManager,schService,fdwAccess) ) return;

	if( !ControlService(
			schService,		//handle of service
			fdwControl,		//control code to send
			&m_ssStatus) )	//address of status info
			MessageBox( NULL,"ControlService","Error",MB_ICONSTOP );

	Close(schService);
	Close(schSCManager);
}
