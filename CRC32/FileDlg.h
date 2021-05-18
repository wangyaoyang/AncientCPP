
/*
	Copyright 2006 - 2008, All Rights Reserved.
							
	打开, 保存, 目录浏览对话框之调用

	作者	- 张鲁夺(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQ群	- 34064264

	为所有爱我的人和我爱的人努力!
*/

#ifndef _FILEDLG_H
#define _FILEDLG_H

#pragma warning(disable : 4786)
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

#include "Thunk.h"

#define _FILEDLG_BUFSIZE 524288 // 1024 * 1024 / 2 = 524288, 此值在有些情况下必须小于1M!

class FileDlg
{
public:

	FileDlg();
	virtual ~FileDlg();

public:

	// 此结构体为win2000以上操作系统定制
	typedef struct tagOFNEX : public OPENFILENAME
	{
		void*			pvReserved;
		DWORD			dwReserved;
		DWORD			FlagsEx;
	}	OPENFILENAMEEX,	*POPENFILENAMEEX;

	OPENFILENAMEEX	m_OFNOpen;
	OPENFILENAMEEX	m_OFNSave;
	BROWSEINFO		m_BIFBrowse;

private:

	Thunk	m_ThunkOpen;
	Thunk	m_ThunkSave;
	Thunk	m_ThunkBrowse;

	string	m_InitOpenDir;
	string	m_InitSaveDir;
	string	m_InitBrowseDir;

	char*	m_OpenNameBuf;
	char*	m_SaveNameBuf;
	char*	m_BrowseNameBuf;

protected:

	virtual UINT_PTR OpenProc	(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual UINT_PTR SaveProc	(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int		 BrowseProc	(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

public:

	vector<string>	GetOpenFileName(HWND hWnd, bool bMulti = false, char* szFilter = NULL, char* szInitName = "\0");
	string			GetSaveFileName(HWND hWnd, bool AutoAddSuffixName = true, char* szFilter = NULL, char* szInitName = "\0");
	string			GetBrowseForFolder(HWND hWnd, char* szInitPath = "\0");
};

#endif