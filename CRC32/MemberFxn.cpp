
/*
	Copyright 2006 - 2008, All Rights Reserved.
							
	�������������һ��C++���Ա������ַ

	����	- ��³��(zhangluduo)
	MSN		- zhangluduo@msn.com
	QQȺ	- 34064264

	Ϊ���а��ҵ��˺��Ұ�����Ŭ��!
*/

#include "StdAfx.h"
#include "MemberFxn.h"

MemberFxn::MemberFxn( ) : m_This(0), m_MemberFxnAddr(0)
{
}

MemberFxn::~MemberFxn( )
{
}

MemberFxn::MemberFxn(int n /* n must be zero */)
{
	if(n != 0)
		return ;
	m_This			= 0;
	m_MemberFxnAddr	= 0;
}

MemberFxn& MemberFxn::operator = (MemberFxn addr)
{
	m_This			= addr.m_This;
	m_MemberFxnAddr	= addr.m_MemberFxnAddr;
	return *this;
}

bool MemberFxn::operator == (MemberFxn addr) 
{
	return m_This == addr.GetThis() && m_MemberFxnAddr	== addr.GetAddr();
}

bool MemberFxn::IsNull()
{
	return m_This == 0 && m_MemberFxnAddr == 0 ? true : false;
}

void* MemberFxn::GetThis()
{
	return m_This;
}

unsigned long MemberFxn::GetAddr()
{
	return m_MemberFxnAddr;
}