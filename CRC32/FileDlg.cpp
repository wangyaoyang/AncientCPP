
/*
	Copyright 2006 - 2008, All Rights Reserved.
							
	��, ����, Ŀ¼����Ի���֮����

	����	- ��³��(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQȺ	- 34064264

	Ϊ���а��ҵ��˺��Ұ�����Ŭ��!
*/

#include "stdafx.h"
#include "FileDlg.h"
#include <memory.h> // for memset

FileDlg::FileDlg()
{
	/** �жϲ���ϵͳ, 
		�����ݲ���ϵͳ���ṹ���С
	*/

	memset(&m_OFNOpen, 0, sizeof(OPENFILENAMEEX));
	memset(&m_OFNSave, 0, sizeof(OPENFILENAMEEX));
	memset(&m_BIFBrowse, 0, sizeof(BROWSEINFO));

	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO *)&osvi))
		return ;
	switch(osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
		if(	(osvi.dwMajorVersion==4 && osvi.dwMinorVersion ==10) || //win 98
			(osvi.dwMajorVersion==4 && osvi.dwMinorVersion ==90) || //win Me
			(osvi.dwMajorVersion==3 && osvi.dwMinorVersion ==51) || //win NT 3.51
			(osvi.dwMajorVersion==4 && osvi.dwMinorVersion ==0))	//win NT 4.0
		{
			m_OFNOpen.lStructSize = sizeof(OPENFILENAME);
			m_OFNSave.lStructSize = sizeof(OPENFILENAME);
		}
		break;
	case VER_PLATFORM_WIN32_NT:
		if(	(osvi.dwMajorVersion==5 && osvi.dwMinorVersion ==0) ||	//win 2000
			(osvi.dwMajorVersion==5 && osvi.dwMinorVersion ==1) ||	//win xp
			(osvi.dwMajorVersion==5 && osvi.dwMinorVersion ==2))	//win 2003
		{
			m_OFNOpen.lStructSize = sizeof(OPENFILENAMEEX);
			m_OFNSave.lStructSize = sizeof(OPENFILENAMEEX);
		}
		break;
	default:
		return ;
	}

	// ��ʼĿ¼

	char szAppName[256]	= {0};
	GetModuleFileName(NULL, szAppName, 255);
	string strAppName	= szAppName;
	m_InitOpenDir = m_InitSaveDir = strAppName.substr(0,strAppName.find_last_of("\\")+1);
}

FileDlg::~FileDlg()
{

}

UINT_PTR FileDlg::OpenProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_INITDIALOG)
	{
		RECT rtWnd, rcSys;
		::GetWindowRect(GetParent(hWnd), &rtWnd);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcSys, 0);
		::MoveWindow(GetParent(hWnd),
					(rcSys.right	- (rtWnd.right	- rtWnd.left))	/ 2,
					(rcSys.bottom	- (rtWnd.bottom	- rtWnd.top))	/ 2,
					rtWnd.right		- rtWnd.left,
					rtWnd.bottom	- rtWnd.top,
					true);
	}
	return 0;
}

UINT_PTR FileDlg::SaveProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_INITDIALOG)
	{
		RECT rtWnd, rcSys;
		::GetWindowRect(GetParent(hWnd), &rtWnd);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcSys, 0);
		::MoveWindow(GetParent(hWnd),
					(rcSys.right	- (rtWnd.right	- rtWnd.left))	/ 2,
					(rcSys.bottom	- (rtWnd.bottom	- rtWnd.top))	/ 2,
					rtWnd.right		- rtWnd.left,
					rtWnd.bottom	- rtWnd.top,
					true);
	}
	return 0;
}

int FileDlg::BrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if(uMsg == WM_INITDIALOG)
	{
		RECT rtWnd, rcSys;
		::GetWindowRect(GetParent(hWnd), &rtWnd);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcSys, 0);
		::MoveWindow(GetParent(hWnd),
					(rcSys.right	- (rtWnd.right	- rtWnd.left))	/ 2,
					(rcSys.bottom	- (rtWnd.bottom	- rtWnd.top))	/ 2,
					rtWnd.right		- rtWnd.left,
					rtWnd.bottom	- rtWnd.top,
					true);
	}

	// ��ʼ��Ŀ¼
	if(uMsg == BFFM_INITIALIZED && !m_InitBrowseDir.empty())
		::SendMessage(hWnd, BFFM_SETSELECTION, 1, (WPARAM)m_InitBrowseDir.c_str());

	// ȷ����ť�Ƿ����
	if(uMsg == BFFM_SELCHANGED )
	{
		char buf[MAX_PATH] = {0};
		if(SHGetPathFromIDList((LPCITEMIDLIST)lParam, buf) && buf != 0)
		{
			::SendMessage(hWnd, BFFM_ENABLEOK, 0, 1);
			::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)buf);
		}
		else
			::SendMessage(hWnd, BFFM_ENABLEOK, 0, 0);
	}
	return 0;
}

vector<string> FileDlg::GetOpenFileName(HWND hWnd, bool bMulti, char* szFilter, char* szInitName)
{
	vector<string> Files;

	m_OpenNameBuf				= new char[_FILEDLG_BUFSIZE + 1];
	if(m_OpenNameBuf == 0)
		AfxMessageBox("sdf");
	memset(m_OpenNameBuf, 0, _FILEDLG_BUFSIZE + 1);
	
	if(szInitName != "\0")
		strcpy(m_OpenNameBuf, szInitName);

	m_OFNOpen.lpstrFile			= m_OpenNameBuf;
	m_OFNOpen.nMaxFile			= _FILEDLG_BUFSIZE;


	m_OFNOpen.Flags				|= OFN_EXPLORER | OFN_ENABLEHOOK | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	m_OFNOpen.hwndOwner			= hWnd;
	m_OFNOpen.lpstrInitialDir	= m_InitOpenDir.c_str();
	m_OFNOpen.lpfnHook			= (LPOFNHOOKPROC)m_ThunkOpen.Thiscall(this, Thunk::GetMemberFxnAddr(&FileDlg::OpenProc));

	if(szFilter == NULL)
		m_OFNOpen.lpstrFilter	= "�����ļ� (*.*)\0*.*\0\0";
	else
		m_OFNOpen.lpstrFilter	= szFilter;		

	if(bMulti)
		m_OFNOpen.Flags			|= OFN_ALLOWMULTISELECT;	

	string strTmp				= "";
	char* ptr					= m_OFNOpen.lpstrFile;
	int nFiles					= 0;

	if(!::GetOpenFileName(&m_OFNOpen))
	{
		delete[] m_OpenNameBuf;
		return Files;
	}

	while(*ptr++)
	{
		nFiles++;
		if(nFiles == 1)
		{
			strTmp = ptr - 1;
			if(strTmp[strTmp.size() - 1] != '\\')
			{
				strTmp += "\\";
				m_InitOpenDir = strTmp;
			}
		}
		else if(nFiles >= 2)
		{
			Files.push_back(strTmp+(ptr - 1));
		}
		while(*ptr++);
	}
	if(Files.size() ==0 )
		Files.push_back(m_OFNOpen.lpstrFile);

	// ����
	delete[] m_OpenNameBuf;
	return Files;
}

string FileDlg::GetSaveFileName(HWND hWnd, bool AutoAddSuffixName, char* szFilter, char* szInitName)
{
	m_OFNSave.Flags				|= OFN_EXPLORER | OFN_ENABLEHOOK | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;
	m_OFNSave.hwndOwner			= hWnd;
	m_OFNSave.lpfnHook			= (LPOFNHOOKPROC)m_ThunkSave.Thiscall(this, Thunk::GetMemberFxnAddr(&FileDlg::SaveProc));

	m_SaveNameBuf				= new char[_FILEDLG_BUFSIZE + 1];
	memset(m_SaveNameBuf, 0, _FILEDLG_BUFSIZE + 1);

	// ��ʼ���ļ���
	if(szInitName != "\0")
		strcpy(m_SaveNameBuf, szInitName);

	m_OFNSave.lpstrFile			= m_SaveNameBuf;
	m_OFNSave.nMaxFile			= _FILEDLG_BUFSIZE;

	// ��ʼ��Ŀ¼
	m_OFNSave.lpstrInitialDir	= m_InitSaveDir.c_str();

	if(szFilter == NULL)
		m_OFNSave.lpstrFilter	= "�����ļ� (*.*)\0*.*\0\0";
	else
		m_OFNSave.lpstrFilter	= szFilter;	


	// ��ʾ����Ի���
	if(!::GetSaveFileName(&m_OFNSave))
	{
		delete[] m_SaveNameBuf;

//		CString str;
//		str.Format("Error Code = 0x%X, defined in file \"Cderr.h\"", CommDlgExtendedError());
//		AfxMessageBox(str);
		
		return "";
	}

	// ȡ�ļ������е�����
	string strFileName = m_SaveNameBuf; 

	// ���浱ǰ�򿪵�Ŀ¼��
	m_InitSaveDir = m_OFNSave.lpstrFile;
	m_InitSaveDir = m_InitSaveDir.substr(0,m_InitSaveDir.find_last_of("\\")+1);

	// �Ƿ�Ϊ������ļ����Զ���Ӻ�׺��
	if(!AutoAddSuffixName)
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	// �ж��û���д���ļ����Ƿ��к�׺
	bool bSuffix = strFileName.find_last_of(".") == -1 ? false : true;
	if(bSuffix)
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	//��"������"�еĴ���'\0'�ָ���������
	vector<string> FilterArray;
	char* p = (char*)m_OFNSave.lpstrFilter;
	int odd = 0;
	while(*p++)
	{
		odd++;
		if(odd == 2)
		{
			FilterArray.push_back(p - 1);
			odd = 0;
		}
		while(*p++);
	}

	// �����������Ϊ"*.*",�򲻽��д���,ֱ�ӷ���
	string strFilter = FilterArray[m_OFNSave.nFilterIndex - 1];
	strFilter.erase(remove_if(strFilter.begin(), strFilter.end(), ptr_fun(::isspace)), strFilter.end());
	if(strFilter == "*.*")
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	// ȡ�������еĺ�׺��
	string	strSuffixName = strFilter.substr(strFilter.find_last_of(".") + 1);
	if(strSuffixName == "*")
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	if(!bSuffix)
	{
		delete[] m_SaveNameBuf;
		return strFileName + "." + strSuffixName;
	}

	delete[] m_SaveNameBuf;
	return strFileName;
}

string FileDlg::GetBrowseForFolder(HWND hWnd, char* szInitPath)
{
	m_BrowseNameBuf = new char[MAX_PATH + 1];
	memset(m_BrowseNameBuf, 0, MAX_PATH + 1);
	m_BIFBrowse.pszDisplayName	= m_BrowseNameBuf;
	m_BIFBrowse.hwndOwner		= hWnd;
	if(szInitPath != "\0")
	m_InitBrowseDir				= szInitPath;
	m_BIFBrowse.lpszTitle		= "��ѡ���ļ���:";
	m_BIFBrowse.pidlRoot		= NULL;
	m_BIFBrowse.ulFlags			|= BIF_STATUSTEXT;
	m_BIFBrowse.lpfn			= (BFFCALLBACK)m_ThunkBrowse.Thiscall(this, Thunk::GetMemberFxnAddr(&FileDlg::BrowseProc));
	LPCITEMIDLIST lpcitemidlist = SHBrowseForFolder(&m_BIFBrowse);

	if(lpcitemidlist)
	{
		if(SHGetPathFromIDList(lpcitemidlist, m_BrowseNameBuf))
		{
			if((int)m_BrowseNameBuf[strlen(m_BrowseNameBuf) - 1] == (int)'\\')
			{
				string strRetVal = m_BrowseNameBuf;
				delete[] m_BrowseNameBuf;
				return strRetVal;
			}
			else
			{
				string strRetVal = string(m_BrowseNameBuf) + "\\";
				delete[] m_BrowseNameBuf;
				return strRetVal;
			}
		}
	}

	delete[] m_BrowseNameBuf;
	return "";
}
