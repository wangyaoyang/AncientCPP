// MDTest.h: interface for the MDTest class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDTEST_H__A7B8BEFC_3506_4AF9_BA2A_EC727574708D__INCLUDED_)
#define AFX_MDTEST_H__A7B8BEFC_3506_4AF9_BA2A_EC727574708D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
#include <afx.h>
#include "..\ProcessComm\Vq3.h"
#include "..\ProcessComm\Vq3_db.h"
#include "..\ProcessComm\Packer.h"
#include "..\ProcessComm\Mailslot.h"
#include "DEVPARAM.H"

#define		MDTEST_INI_FILE_NAME	"MDTest.ini"

#define		MD_TEST_REQUEST		0x085
#define		MD_TEST_RESPONSE	0x086
#define		MD_TEST_TOMAILBOX	0x087

#define		TimeSignLen			2		//ʱ��곤�ȣ�    2���ֽ�
#define		MOID_LEN			12		//MOID���ȣ�     12���ֽ�
#define		TESTID_LEN			43		//����ID���ȣ�   43���ֽ�
#define		PARAMETER_LEN		200		//���Բ������ȣ�200���ֽ�
#define		RETCODE_LEN			8		//���ؽ���볤�ȣ� 8���ֽ�
#define		FILEPATH_LEN		64		//�����ļ�·������

#define	MAILSLOT_NAME_BASE			"PROCESS_MBX"
#define	MAILSLOT_NAME_NTS			"NTS_MBX"


//////////////////////////////////////////////////////////////////////////
////    This is come from wyy
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct RTU_CMD_STRUCT
{	//RTU�����ʽ
	char	m_device_name[8];				//	���磺RTU_01 
	char	m_ip_address[16];				//	168.116.100.121
	char	m_community[16];				//	public
	char	m_rtu_command[8];				//	��ʼ���ԣ�startTST
	char	m_reserved[16];					//	����16 BYTE����64 BYTE
}RTU_COMMAND;
////////////////////////////////////////////////////////////////////////////////
class CRtuParameter
{	//RTU���Լ�������������
public:
	OTDR_PARAMS			m_otdr_param;		//OTDR������ռ��131�ֽ�
	DWORD				m_device_descrip;	//�豸����(����)
	BYTE				m_device_num;		//����豸������������
	RTU_COMMAND			m_rtu_command;		//RTU����������ռ��64�ֽ�
public:
	//�����ṹ��ռ��200�ֽ�
	CRtuParameter ( void ) { memset( this,0x00,200 ); }
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class MDTest  : public CProtocol_VQ3 
{
private:
	char			m_MOID[12];				//���Բ���MOID
	char			m_TestID[43];			//���Բ���TestID
	char			m_FilePath[64];			//����,������õ�������·��
	char			m_RetCode[8];			//����,�����

private:
	void unpackResponse( char* vq3_string );


/////////////////////////////////////////////////////////////////////
////    �����������
/////////////////////////////////////////////////////////////////////
public:
	//��������ֵ(0����û���յ�MDӦ��
	//			 1�����յ�MDӦ��RTU��Ӧ��
	//			 2�������Գɹ�)
	__int8 OMTest(CProtocol_VQ3_DB&	vq3_db,
				char*				Operator,		//������
				char*				MOID,			//MOID
				char*				TestID,			//����ID
				CRtuParameter		Parameter);		//OTDR���Բ���
	char* getFilepath();
	__int8 getRetcode();
	MDTest();
	~MDTest();
};

#endif // !defined(AFX_MDTEST_H__A7B8BEFC_3506_4AF9_BA2A_EC727574708D__INCLUDED_)
