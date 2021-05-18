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

#define		TimeSignLen			2		//时间标长度，    2个字节
#define		MOID_LEN			12		//MOID长度，     12个字节
#define		TESTID_LEN			43		//测试ID长度，   43个字节
#define		PARAMETER_LEN		200		//测试参数长度，200个字节
#define		RETCODE_LEN			8		//返回结果码长度， 8个字节
#define		FILEPATH_LEN		64		//返回文件路经长度

#define	MAILSLOT_NAME_BASE			"PROCESS_MBX"
#define	MAILSLOT_NAME_NTS			"NTS_MBX"


//////////////////////////////////////////////////////////////////////////
////    This is come from wyy
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct RTU_CMD_STRUCT
{	//RTU命令格式
	char	m_device_name[8];				//	例如：RTU_01 
	char	m_ip_address[16];				//	168.116.100.121
	char	m_community[16];				//	public
	char	m_rtu_command[8];				//	开始测试：startTST
	char	m_reserved[16];					//	保留16 BYTE，共64 BYTE
}RTU_COMMAND;
////////////////////////////////////////////////////////////////////////////////
class CRtuParameter
{	//RTU测试及参数操作报文
public:
	OTDR_PARAMS			m_otdr_param;		//OTDR参数，占用131字节
	DWORD				m_device_descrip;	//设备描述(保留)
	BYTE				m_device_num;		//相关设备数量（保留）
	RTU_COMMAND			m_rtu_command;		//RTU命令描述，占用64字节
public:
	//整个结构共占用200字节
	CRtuParameter ( void ) { memset( this,0x00,200 ); }
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class MDTest  : public CProtocol_VQ3 
{
private:
	char			m_MOID[12];				//测试参数MOID
	char			m_TestID[43];			//测试参数TestID
	char			m_FilePath[64];			//返回,测量后得到的曲线路径
	char			m_RetCode[8];			//返回,结果码

private:
	void unpackResponse( char* vq3_string );


/////////////////////////////////////////////////////////////////////
////    以下命令可用
/////////////////////////////////////////////////////////////////////
public:
	//函数返回值(0——没有收到MD应答；
	//			 1——收到MD应答，RTU无应答；
	//			 2——测试成功)
	__int8 OMTest(CProtocol_VQ3_DB&	vq3_db,
				char*				Operator,		//操作人
				char*				MOID,			//MOID
				char*				TestID,			//测试ID
				CRtuParameter		Parameter);		//OTDR测试参数
	char* getFilepath();
	__int8 getRetcode();
	MDTest();
	~MDTest();
};

#endif // !defined(AFX_MDTEST_H__A7B8BEFC_3506_4AF9_BA2A_EC727574708D__INCLUDED_)
