// MDTest.cpp: implementation of the MDTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MDTest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MDTest::MDTest()
{
}

MDTest::~MDTest()
{
}

/////////////////////////////////////////////
//点名测试函数
/////////////////////////////////////////////
__int8 MDTest::OMTest( CProtocol_VQ3_DB& vq3_db,char* Operator,char* MOID,char* TestID,CRtuParameter Parameter )
{
	// 1.获取测试参数
	CMailBox&	Mailbox = vq3_db.getMailbox();
	memset( m_MOID,0x00,MOID_LEN );
	memcpy( m_MOID,MOID,MOID_LEN );
	memset( m_TestID,0x00,TESTID_LEN );
	memcpy( m_TestID,TestID,TESTID_LEN );

	Dist_Node_Add	= GetPrivateProfileInt( "VQ3_Head","Dest_Node_Add",0x1003,MDTEST_INI_FILE_NAME );	//目的主机地址
	Dist_FE_Add		= GetPrivateProfileInt( "VQ3_Head","Dest_FE_Add",0x23,MDTEST_INI_FILE_NAME );		//目的进程地址
	Source_Node_Add	= GetPrivateProfileInt( "VQ3_Head","Source_Node_Add",0x1002,MDTEST_INI_FILE_NAME );	//源主机地址
	Source_FE_Add	= GetPrivateProfileInt( "VQ3_Head","Source_FE_Add",0x44,MDTEST_INI_FILE_NAME );		//源进程地址
	Working_Flag	= GetPrivateProfileInt( "VQ3_Head","Working_Flag",1,MDTEST_INI_FILE_NAME );			//工作标志（恒为0）////???////(恒为1)
	Tran_Command	= GetPrivateProfileInt( "VQ3_Head","Tran_Command",1,MDTEST_INI_FILE_NAME );			//传送层命令=1	
	Message_Lenth	= 0;																				//待定，应用报文的字节数
	Command_Code	= MD_TEST_REQUEST;																	//Command Code = 85
	Time_Flag		= GetPrivateProfileInt( "VQ3_Head","Time_Flag",0x0841,MDTEST_INI_FILE_NAME );		//时间标(0000100001000001)一月一日一时
	TDN_Add			= GetPrivateProfileInt( "VQ3_Head","TDN_Add",0,MDTEST_INI_FILE_NAME );				//站地址
	NE_Add			= GetPrivateProfileInt( "VQ3_Head","NE_Add",0,MDTEST_INI_FILE_NAME );				//功能体地址

	
	// 2.发送测试请求
	long	length	= 0;
	char*	Vq3_MDTest_Packet = NULL;		//Vq3报文，包含A+命令时间表等+应用报文
	char*	myApp_Packet = NULL;				//应用报文

	myApp_Packet = Pack(&length,
						T_MEMORY|MOID_LEN,		m_MOID,
						T_MEMORY|TESTID_LEN,	m_TestID,
						T_MEMORY|PARAMETER_LEN,	&Parameter );
	//	拼装VQ3报文Vq3_MDTest_Packet	///////////////////////////
	Message_Lenth	= length + 7;
	Vq3_MDTest_Packet = PackVq3( length, myApp_Packet );
	if(myApp_Packet) {delete myApp_Packet;	myApp_Packet = NULL;}
	//  发送到邮箱						///////////////////////////
	Mailbox.ClearBox();
	Mailbox.SendMail( MAILSLOT_NAME_NTS,Vq3_MDTest_Packet,length );

	// 3.循环检测邮箱，直到有结果或者超时	
	MAIL_MESSAGE mail;
	__int8		myStatus;
	myStatus = 0;
	memset( m_TestID,0x00,TESTID_LEN);
	memset( m_RetCode,0x00,RETCODE_LEN );
	memset( m_FilePath,0x00,FILEPATH_LEN);
	for( int i=0;i<100;i++,Sleep(100) )
	{
		if( Mailbox.RecvMail( mail ) <= 0 ) continue;
		unpackResponse( mail.m_message  );
		if( m_RetCode[0]==0x01 )				//发送点名测试后10秒内，应该收到一个响应(命令码86,返回码1)
		{
			myStatus = 1;
			int	max_wait_times = GetPrivateProfileInt( "MDTEST_TIME","wait_milisecond",120000,MDTEST_INI_FILE_NAME )/100;
			for( int i=0; i<max_wait_times; i++,Sleep(100) )
			{
				if( Mailbox.RecvMail( mail ) <= 0 ) continue;
				unpackResponse( mail.m_message  );
				if (m_RetCode[0]==0x03)	{ myStatus=2;break; }
			}
			if (myStatus==2) break;
		}
	}
	
	// 4.填写操作日志
	time_t	myTime;
	char tmpbuf[32];
	time( &myTime ); 
	CTime myCTime( myTime );
	sprintf( tmpbuf,"%s",myCTime.Format("%Y-%m-%d %H:%M:%S") );	//2001-03-09 16:17:18
	
	CFieldsList		myFieldsList;
	CField			myFields[8];
	CField			keyword;
	char			myParameter[200];
	memcpy( myParameter,&Parameter,200 );
	myFields[0].Set( "objectname",TestID,T_STRING );
	myFields[1].Set( "operator",Operator,T_STRING );
	myFields[2].Set( "cmdcode",&Command_Code,T_INT8 );
	myFields[3].Set( "cmdparameter",myParameter,T_MEMORY,200 );
	myFields[4].Set( "resultsucc",&myStatus,T_INT8 );
	myFields[5].Set( "resultcode",&m_RetCode[0],T_INT32 );
	myFields[6].Set( "resultdata",m_FilePath,T_STRING );
	myFields[7].Set( "operatetime",tmpbuf,T_STRING );
	myFieldsList.AddTail(&myFields[0]);
	myFieldsList.AddTail(&myFields[1]);
	myFieldsList.AddTail(&myFields[2]);
	myFieldsList.AddTail(&myFields[3]);
	myFieldsList.AddTail(&myFields[4]);
	myFieldsList.AddTail(&myFields[5]);
	myFieldsList.AddTail(&myFields[6]);
	myFieldsList.AddTail(&myFields[7]);
	vq3_db.Command_Code = 0x1C;
	//可能需要修改2001.03.22
	vq3_db.setAddress( 0x1002,0x46,0x1002,0x45 );
	vq3_db.sendRequest(DB_APPEND_REQUEST,"OperationLog",keyword,myFieldsList);
	return myStatus;
}


/////////////////////////////////////////////////////////
//    将结果解包，获取文件路径
////////////////////////////////////////////////////////
void MDTest::unpackResponse( char* vq3_string )
{
	long	length = 0;
	char*	app_string = UnPackVq3(length,vq3_string);
	memset( m_TestID,0x00,TESTID_LEN);
	memset( m_RetCode,0x00,RETCODE_LEN );
	memset( m_FilePath,0x00,FILEPATH_LEN);
	if ( Command_Code==MD_TEST_RESPONSE)		//返回结果0x86
		UnPack(	app_string,
				T_MEMORY|TESTID_LEN,	m_TestID,
				T_MEMORY|RETCODE_LEN,	m_RetCode,
				T_MEMORY|FILEPATH_LEN,	m_FilePath );
	if ( app_string ) app_string = NULL;
}


char* MDTest::getFilepath()		{ return m_FilePath; }
__int8 MDTest::getRetcode()		{ return m_RetCode[0]; }
