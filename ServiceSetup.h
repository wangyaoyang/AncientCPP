#ifndef		__SERVICE_SETUP_H__
#define		__SERVICE_SETUP_H__

#include "WINSVC.H"

class CServiceSetup
{
public:
	SERVICE_STATUS	m_ssStatus;       // current status of the service
	CString			m_szPath;
	CString			m_szServiceName;
	CString			m_szServiceDisplayName;
private:
	BOOL	OpenSCM(SC_HANDLE&);
	BOOL	OpenSVC(SC_HANDLE&,SC_HANDLE&,DWORD=SERVICE_ALL_ACCESS);
	VOID	Close(SC_HANDLE&);
public:
	CServiceSetup();
	~CServiceSetup();
	VOID	InstallService();
	VOID	RemoveService();
	VOID	StartupService();
	VOID	ManageService(DWORD fdwControl);
};

#endif		//__SERVICE_SETUP_H__


