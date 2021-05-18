
/*
	Copyright 2006 - 2008, All Rights Reserved.
							
	��, ����, Ŀ¼����Ի���֮����

	����	- ��³��(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQȺ	- 34064264

	Ϊ���а��ҵ��˺��Ұ�����Ŭ��!
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

#define _FILEDLG_BUFSIZE 524288 // 1024 * 1024 / 2 = 524288, ��ֵ����Щ����±���С��1M!

class FileDlg
{
public:

	FileDlg();
	virtual ~FileDlg();

public:

	// �˽ṹ��Ϊwin2000���ϲ���ϵͳ����
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