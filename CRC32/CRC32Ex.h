
/*
	Copyright 2006 - 2008, All Rights Reserved.

	CRC32��֮��չ

	����	- ��³��(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQȺ	- 34064264

	Ϊ���а��ҵ��˺��Ұ�����Ŭ��!
*/

#ifndef _CRC32EX_H
#define _CRC32EX_H

#include "CRC32.h"
#include "MemberFxn.h"

#pragma warning(disable : 4786)
#include <string>
using namespace std;

#define BUFFER_SIZE 1024

class CRC32Ex : public CRC32
{
public:

	CRC32Ex();
	virtual ~CRC32Ex();

private:

	string	m_strErr;		// ��������
	bool	m_bTerminate;	// ��ֹ�ļ������־

	void OnFileProcessing(int nProgress, MemberFxn addr);
	string ResultToHex(unsigned long Result, bool bUpper = true);

public:

	// ��ȡ�ļ���CRC32ֵ
	string GetCRC32FromFile (char* FileName, bool bUpper = true, MemberFxn addr = 0);

	// ��ȡ�ڴ����ݵ�CRC32ֵ
	string GetCRC32FromBuffer(unsigned char* pData, unsigned long DataLen, bool bUpper = true);

	// �˺�������ǰ����GetCRC32FromFile������ִ��
	void	StopCalcFile();

	// ��ȡ��������
	string GetError();
};

#endif