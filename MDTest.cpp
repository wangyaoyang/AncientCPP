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
//�������Ժ���
/////////////////////////////////////////////
__int8 MDTest::OMTest( CProtocol_VQ3_DB& vq3_db,char* Operator,char* MOID,char* TestID,CRtuParameter Parameter )
{
	// 1.��ȡ���Բ���
	CMailBox&	Mailbox = vq3_db.getMailbox();
	memset( m_MOID,0x00,MOID_LEN );
	memcpy( m_MOID,MOID,MOID_LEN );
	memset( m_TestID,0x00,TESTID_LEN );
	memcpy( m_TestID,TestID,TESTID_LEN );

	Dist_Node_Add	= GetPrivateProfileInt( "VQ3_Head","Dest_Node_Add",0x1003,MDTEST_INI_FILE_NAME );	//Ŀ��������ַ
	Dist_FE_Add		= GetPrivateProfileInt( "VQ3_Head","Dest_FE_Add",0x23,MDTEST_INI_FILE_NAME );		//Ŀ�Ľ��̵�ַ
	Source_Node_Add	= GetPrivateProfileInt( "VQ3_Head","Source_Node_Add",0x1002,MDTEST_INI_FILE_NAME );	//Դ������ַ
	Source_FE_Add	= GetPrivateProfileInt( "VQ3_Head","Source_FE_Add",0x44,MDTEST_INI_FILE_NAME );		//Դ���̵�ַ
	Working_Flag	= GetPrivateProfileInt( "VQ3_Head","Working_Flag",1,MDTEST_INI_FILE_NAME );			//������־����Ϊ0��////???////(��Ϊ1)
	Tran_Command	= GetPrivateProfileInt( "VQ3_Head","Tran_Command",1,MDTEST_INI_FILE_NAME );			//���Ͳ�����=1	
	Message_Lenth	= 0;																				//������Ӧ�ñ��ĵ��ֽ���
	Command_Code	= MD_TEST_REQUEST;																	//Command Code = 85
	Time_Flag		= GetPrivateProfileInt( "VQ3_Head","Time_Flag",0x0841,MDTEST_INI_FILE_NAME );		//ʱ���(0000100001000001)һ��һ��һʱ
	TDN_Add			= GetPrivateProfileInt( "VQ3_Head","TDN_Add",0,MDTEST_INI_FILE_NAME );				//վ��ַ
	NE_Add			= GetPrivateProfileInt( "VQ3_Head","NE_Add",0,MDTEST_INI_FILE_NAME );				//�������ַ

	
	// 2.���Ͳ�������
	long	length	= 0;
	char*	Vq3_MDTest_Packet = NULL;		//Vq3���ģ�����A+����ʱ����+Ӧ�ñ���
	char*	myApp_Packet = NULL;				//Ӧ�ñ���

	myApp_Packet = Pack(&length,
						T_MEMORY|MOID_LEN,		m_MOID,
						T_MEMORY|TESTID_LEN,	m_TestID,
						T_MEMORY|PARAMETER_LEN,	&Parameter );
	//	ƴװVQ3����Vq3_MDTest_Packet	///////////////////////////
	Message_Lenth	= length + 7;
	Vq3_MDTest_Packet = PackVq3( length, myApp_Packet );
	if(myApp_Packet) {delete myApp_Packet;	myApp_Packet = NULL;}
	//  ���͵�����						///////////////////////////
	Mailbox.ClearBox();
	Mailbox.SendMail( MAILSLOT_NAME_NTS,Vq3_MDTest_Packet,length );

	// 3.ѭ��������䣬ֱ���н�����߳�ʱ	
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
		if( m_RetCode[0]==0x01 )				//���͵������Ժ�10���ڣ�Ӧ���յ�һ����Ӧ(������86,������1)
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
	
	// 4.��д������־
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
	//������Ҫ�޸�2001.03.22
	vq3_db.setAddress( 0x1002,0x46,0x1002,0x45 );
	vq3_db.sendRequest(DB_APPEND_REQUEST,"OperationLog",keyword,myFieldsList);
	return myStatus;
}


/////////////////////////////////////////////////////////
//    ������������ȡ�ļ�·��
////////////////////////////////////////////////////////
void MDTest::unpackResponse( char* vq3_string )
{
	long	length = 0;
	char*	app_string = UnPackVq3(length,vq3_string);
	memset( m_TestID,0x00,TESTID_LEN);
	memset( m_RetCode,0x00,RETCODE_LEN );
	memset( m_FilePath,0x00,FILEPATH_LEN);
	if ( Command_Code==MD_TEST_RESPONSE)		//���ؽ��0x86
		UnPack(	app_string,
				T_MEMORY|TESTID_LEN,	m_TestID,
				T_MEMORY|RETCODE_LEN,	m_RetCode,
				T_MEMORY|FILEPATH_LEN,	m_FilePath );
	if ( app_string ) app_string = NULL;
}


char* MDTest::getFilepath()		{ return m_FilePath; }
__int8 MDTest::getRetcode()		{ return m_RetCode[0]; }
