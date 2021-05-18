
/*
	Copyright 2006 - 2008, All Rights Reserved.
							
	打开, 保存, 目录浏览对话框之调用

	作者	- 张鲁夺(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQ群	- 34064264

	为所有爱我的人和我爱的人努力!
*/

#include "stdafx.h"
#include "FileDlg.h"
#include <memory.h> // for memset

FileDlg::FileDlg()
{
	/** 判断操作系统, 
		并根据操作系统填充结构体大小
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

	// 初始目录

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

	// 初始化目录
	if(uMsg == BFFM_INITIALIZED && !m_InitBrowseDir.empty())
		::SendMessage(hWnd, BFFM_SETSELECTION, 1, (WPARAM)m_InitBrowseDir.c_str());

	// 确定按钮是否禁用
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
		m_OFNOpen.lpstrFilter	= "所有文件 (*.*)\0*.*\0\0";
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

	// 返回
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

	// 初始化文件名
	if(szInitName != "\0")
		strcpy(m_SaveNameBuf, szInitName);

	m_OFNSave.lpstrFile			= m_SaveNameBuf;
	m_OFNSave.nMaxFile			= _FILEDLG_BUFSIZE;

	// 初始化目录
	m_OFNSave.lpstrInitialDir	= m_InitSaveDir.c_str();

	if(szFilter == NULL)
		m_OFNSave.lpstrFilter	= "所有文件 (*.*)\0*.*\0\0";
	else
		m_OFNSave.lpstrFilter	= szFilter;	


	// 显示保存对话框
	if(!::GetSaveFileName(&m_OFNSave))
	{
		delete[] m_SaveNameBuf;

//		CString str;
//		str.Format("Error Code = 0x%X, defined in file \"Cderr.h\"", CommDlgExtendedError());
//		AfxMessageBox(str);
		
		return "";
	}

	// 取文件名框中的输入
	string strFileName = m_SaveNameBuf; 

	// 保存当前打开的目录名
	m_InitSaveDir = m_OFNSave.lpstrFile;
	m_InitSaveDir = m_InitSaveDir.substr(0,m_InitSaveDir.find_last_of("\\")+1);

	// 是否为保存的文件名自动添加后缀名
	if(!AutoAddSuffixName)
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	// 判断用户填写的文件名是否有后缀
	bool bSuffix = strFileName.find_last_of(".") == -1 ? false : true;
	if(bSuffix)
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	//将"过滤器"中的串以'\0'分隔成数组存放
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

	// 如果过滤类型为"*.*",则不进行处理,直接返回
	string strFilter = FilterArray[m_OFNSave.nFilterIndex - 1];
	strFilter.erase(remove_if(strFilter.begin(), strFilter.end(), ptr_fun(::isspace)), strFilter.end());
	if(strFilter == "*.*")
	{
		delete[] m_SaveNameBuf;
		return strFileName;
	}

	// 取过滤器中的后缀名
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
	m_BIFBrowse.lpszTitle		= "请选择文件夹:";
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
