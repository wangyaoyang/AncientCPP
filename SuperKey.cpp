#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "SuperKey.h"

#define	ALGORITHM_DESCRIP_0	0x0500	// 0001,1101,0000,0000
#define	ALGORITHM_DESCRIP_1	0x0500	// 0001,1101,0000,0000
#define	ALGORITHM_DESCRIP_2	0x0500	// 0001,1101,0000,0000
#define	ALGORITHM_DESCRIP_3	0x0500	// 0001,1101,0000,0000
#define	ALGORITHM_DESCRIP_4	0x0500	// 0001,1101,0000,0000
#define	ALGORITHM_DESCRIP_5	0x0500	// 0001,1101,0000,0000

CSuperKey::CSuperKey(void)
{
	m_Password0 = 0;
	m_Password1 = 0;
	m_Password2 = 0;
	m_ProductNo = PRODUCT_NO;
	m_MaxStations = MAX_STATIONS;
	m_MdMailboxNo = MD_MAILBOX_NO;
	m_LocalAddress = TM90_SYSTEM_MOID;
	memset( m_ErrorCode,0,48 );
	for( int i=0;i<6;i++ ) m_Attribute[ i ] = KeyAttrBlank;
	SuperKey_ClearSKD();
}

CSuperKey::~CSuperKey(void)
{
}

short CSuperKey::SuperKey_Init(void)
{
	m_SKD.Password0 = m_Password0;
	m_SKD.Password1 = m_Password1;
	m_SKD.Password2 = m_Password2;
	m_SKD.FunctionCode = KeyAPIOpen;
	SUPERKEY(&m_SKD);
	if(m_SKD.ReturnFlag) return m_SKD.ReturnFlag;
	m_SKD.FunctionCode = KeyAPIInit;
	SUPERKEY(&m_SKD);
	if(m_SKD.ReturnFlag) return m_SKD.ReturnFlag;
	return 0;
}

short CSuperKey::SuperKey_Close(void)
{
	m_SKD.FunctionCode = KeyAPIClose;
	SUPERKEY(&m_SKD);
	return 0;
}

short CSuperKey::SuperKey_ClearSKD(void)
{
	m_SKD.ReturnFlag = 0;
	m_SKD.FunctionCode = 0;
	m_SKD.MemAddress = 0;
	m_SKD.Password0 = 0;
	m_SKD.Password1 = 0;
	m_SKD.Password2 = 0;
	m_SKD.MemContent = 0;
	m_SKD.Encrypt = 0;
	m_SKD.Timer = 0;
	m_SKD.Lock0 = 0;
	m_SKD.Lock1 = 0;
	m_SKD.Lock2 = 0;
	for( int i=0;i<32;i++ ) m_SKD.Reserved[i] = 0;
	return 0;
}

short CSuperKey::SuperKey_HardwareTest(void)
{
	short	ReturnFlag_w;
	short	ReturnFlag_r;
	short	address;

	for( address=0; address<6; address++ ) SuperKey_SetGroupAttribute( address,0 );
	for( memset( m_ErrorCode,0,48 ),address=0; address<48; address++ ) {
		m_SKD.MemAddress = address;
		m_SKD.MemContent = 1;
		m_SKD.FunctionCode = KeyAPIWriteMemory;
		SUPERKEY(&m_SKD);
		ReturnFlag_w = m_SKD.ReturnFlag;
		m_SKD.FunctionCode = KeyAPIReadMemory;
		SUPERKEY(&m_SKD);
		ReturnFlag_r = m_SKD.ReturnFlag;
		if( ReturnFlag_w || ReturnFlag_r || m_SKD.MemContent != 1 ) {
			if( ReturnFlag_w )			m_ErrorCode[ address ] |= 0x01;
			if( ReturnFlag_r )			m_ErrorCode[ address ] |= 0x02;
			if( m_SKD.MemContent != 1 )	m_ErrorCode[ address ] |= 0x04;
			TRACE("\n Bad unit[ Address=%02d,Code= %2X ]",address,m_ErrorCode[ address ]);
		}
		else TRACE("\n Good unit[ Address=%02d ]",address);
	}
	for( address=0; address<48; address++ ) if( m_ErrorCode[ address ] ) return 1;
	return 0;
}

CString CSuperKey::SuperKey_ExplainErrorCode(short Code)
{
	switch( Code ) {
	case 0:		return( CString("Success") );
	case 1:		return( CString("Invalid Address") );
	case 2:		return( CString("Timer overflow") );
	case 3:		return( CString("Can't read & write") );
	case 4:		return( CString("Can't write,it's read_only cell") );
	case 5:		return( CString("Can't Unlock") );
	case 6:		return( CString("Can't Encrypt") );
	case 7:		return( CString("Input Unlock Data error") );
	case 8:		return( CString("It's UserPass,can't Program") );
	case 9:		return( CString("Invalid function") );
	case 10:	return( CString("Not Open yet, Please Open first") );
	case 11:	return( CString("No Superkey or password error") );
	case 12:	return( CString("Wait") );
	case 15:	return( CString("Can't create driver,for Windows NT only") );
	default:;
	}
	return( CString("Undefined error code.") );
}

short CSuperKey::SuperKey_SetGroupAttribute(short Group,short Attribute)
{
	m_SKD.Password0 = m_Password0;
	m_SKD.Password1 = m_Password1;
	m_SKD.Password2 = m_Password2;
	m_SKD.MemAddress = Group;
	m_SKD.MemContent = Attribute;
	m_SKD.FunctionCode = KeyAPISetAttribute;
	SUPERKEY(&m_SKD);
	return m_SKD.ReturnFlag;
}

short CSuperKey::SuperKey_Read( short Group,short Unit,short* Data )
{
	m_SKD.MemAddress = Group*8 + Unit;
	m_SKD.FunctionCode = KeyAPIReadMemory;
	SUPERKEY(&m_SKD);
	if( m_SKD.ReturnFlag ) return m_SKD.ReturnFlag;
	*Data = m_SKD.MemContent;
	return 0;
}

short CSuperKey::SuperKey_Write( short Group,short Unit,short Data )
{
	m_SKD.MemAddress = Group*8 + Unit;
	m_SKD.MemContent = Data;
	m_SKD.FunctionCode = KeyAPIWriteMemory;
	SUPERKEY(&m_SKD);
	return m_SKD.ReturnFlag;
}

short CSuperKey::SuperKey_Encrypt( short Group,short Source,short* Target )
{
	m_SKD.MemAddress = Group;
	m_SKD.Encrypt = Source;
	m_SKD.FunctionCode = KeyAPIEncrypt;
	SUPERKEY(&m_SKD);
	if(m_SKD.ReturnFlag) return m_SKD.ReturnFlag;
	*Target = m_SKD.Encrypt;
	return 0;
}

short CSuperKey::SuperKey_SetUserPassword(short Pass0,short Pass1,short Pass2)
{
	m_SKD.Lock0 = Pass0;
	m_SKD.Lock1 = Pass1;
	m_SKD.Lock2 = Pass2;
	m_SKD.FunctionCode = KeyAPISetUserPass;
	SUPERKEY(&m_SKD);
	return m_SKD.ReturnFlag;
}

short CSuperKey::SuperKey_GenerateB(short Group)
{
	short	B = 0;
	srand( (unsigned)time( NULL ) );					//	产生随即数 B
	B = (short) rand()/2;
	return SuperKey_Write( Group,2,B );			//	将 B 写入第 Group 组,第 2 单元
}

short CSuperKey::SuperKey_GenerateD(short Group,short Source,short* Target)
{
	short	code = 0;
	short	A = Source;
	short	B = 0;
	short	C = 0;
	short	D = 0;

	code = SuperKey_Read( Group,2,&B );					//	从第 Group 组,第 2 单元
	if( code ) return code;								//	读出 B
	code = SuperKey_Encrypt( Group, B, &C );			//	调用第 Group 组的算法
	if( code ) return code;								//	把随即数 B 变换为 C
	D = A + C + SUPERKEY_CONST_I;
	*Target = D;
	return 0;
}

short CSuperKey::SuperKey_GenerateA(short Group,short Source,short* Target)
{
	short	code = 0;
	short	A = 0;
	short	B = 0;
	short	C = 0;
	short	D = Source;

	code = SuperKey_Read( Group,2,&B );					//	从第 Group 组,第 2 单元
	if( code ) return code;								//	读出 B
	code = SuperKey_Encrypt( Group, B, &C );			//	调用第 Group 组的算法
	if( code ) return code;								//	把随即数 B 变换为 C
	A = D - C - SUPERKEY_CONST_I;
	*Target = A;
	return 0;
}

short CSuperKey::SuperKey_PredefineGroup(short Group)
{
	short	code = 0;
	short	data = 0;

	switch( Group ) {
	case 0:
		m_Attribute[ 0 ] = ALGORITHM_DESCRIP_0 | KeyAttrEncrypt;
		code = SuperKey_SetGroupAttribute( Group, m_Attribute[ 0 ] );		if( code ) return code;
		code = SuperKey_Write( Group,0,ALGORITHM_0L );						if( code ) return code;
		code = SuperKey_Write( Group,1,ALGORITHM_0H );						if( code ) return code;
		code = SuperKey_GenerateB( Group );									if( code ) return code;
		code = SuperKey_Write( Group,3,m_ProductNo );						if( code ) return code;
		code = SuperKey_Write( Group,4,m_MaxStations );						if( code ) return code;
		code = SuperKey_GenerateD( Group,VQ3_TRANS_CMD,&data );				if( code ) return code;
		code = SuperKey_Write( Group,5,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,m_LocalAddress,&data );			if( code ) return code;
		code = SuperKey_Write( Group,6,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,m_MdMailboxNo,&data );				if( code ) return code;
		code = SuperKey_Write( Group,7,data );								if( code ) return code;
		m_Attribute[ Group ] |= KeyAttrReadOnly;
		code = SuperKey_SetGroupAttribute( Group, m_Attribute[ Group ] );	if( code ) return code;
		break;
	case 1:
		m_Attribute[ Group ] = ALGORITHM_DESCRIP_1 | KeyAttrEncrypt;
		code = SuperKey_SetGroupAttribute( Group, m_Attribute[ Group ] );	if( code ) return code;
		code = SuperKey_Write( Group,0,ALGORITHM_1L );						if( code ) return code;
		code = SuperKey_Write( Group,1,ALGORITHM_1H );						if( code ) return code;
		code = SuperKey_GenerateB( Group );									if( code ) return code;
		code = SuperKey_GenerateD( Group,MODI_NEW_BIT,&data );				if( code ) return code;
		code = SuperKey_Write( Group,3,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,MODI_NEW_SUB,&data );				if( code ) return code;
		code = SuperKey_Write( Group,4,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,MODI_ACK,&data );					if( code ) return code;
		code = SuperKey_Write( Group,5,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,MODI_CONTROL,&data );				if( code ) return code;
		code = SuperKey_Write( Group,6,data );								if( code ) return code;
		code = SuperKey_GenerateD( Group,MODI_MEASURE,&data );				if( code ) return code;
		code = SuperKey_Write( Group,7,data );								if( code ) return code;
		m_Attribute[ Group ] |= KeyAttrReadOnly;
		code = SuperKey_SetGroupAttribute( Group, m_Attribute[ Group ] );	if( code ) return code;
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:;
	}
	return 0;
}

short CSuperKey::SuperKey_TestGroup(short Group,short* data)
{
	short	code = 0;
	short	i = 0;

	for( i=0;i<8;i++ ) data[i] = 0;
	switch( Group ) {
	case 0:
		code = SuperKey_Read( Group,3,&data[3] );						if( code ) return code;
		code = SuperKey_Read( Group,4,&data[4] );						if( code ) return code;
		code = SuperKey_Read( Group,5,&data[5] );						if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+5,&data[5] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+6,&data[6] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+7,&data[7] );			if( code ) return code;
		break;
	case 1:
		code = SuperKey_DecodeUserData( Group*8+3,&data[3] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+4,&data[4] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+5,&data[5] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+6,&data[6] );			if( code ) return code;
		code = SuperKey_DecodeUserData( Group*8+7,&data[7] );			if( code ) return code;
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:;
	}
	return 0;
}

short CSuperKey::SuperKey_DecodeUserData( short address,short* data)
{
	short	code;
	short	Group = address/8;
	short	Unit = address%8;

	code = SuperKey_Read( Group,Unit,data );					if( code ) return code;
	code = SuperKey_GenerateA( Group,*data,data );				if( code ) return code;
	return 0;
}

short SenseDog_Read(SENSE_PASSWORD userpass,short address,short* data)
{
	CSuperKey	superkey;
	short		code;
	short		Group = address/8;
	short		Unit = address%8;

	superkey.m_Password0 = userpass.password0;
	superkey.m_Password1 = userpass.password1;
	superkey.m_Password2 = userpass.password2;

	for( int i=0;i<3;i++ ) {
		if( code=superkey.SuperKey_Init() ) { superkey.SuperKey_Close(); continue; }
		if( code=superkey.SuperKey_Read(Group,Unit,data) ) { superkey.SuperKey_Close(); continue; }
		break;
	}
	superkey.SuperKey_Close();
	return code; 
/*
	switch( address ) {
	case 3:		*data = 0x97;	break;
	case 4:		*data = 0x20;	break;
	case 5:		*data = 0x01;	break;
	case 6:		*data = 0x00;	break;
	case 7:		*data = 0x34;	break;
	case 11:	*data = 0xB8;	break;
	case 12:	*data = 0xC3;	break;
	case 13:	*data = 0x39;	break;
	case 14:	*data = 0x03;	break;
	case 15:	*data = 0x0F;	break;
	default:;
	}
*/
	return 0;
}

short SenseDog_Decode(SENSE_PASSWORD userpass,short address,short* data)
{
	CSuperKey	superkey;
	short		code = 0;
	short		Group = address/8;
	short		Unit = address%8;

	superkey.m_Password0 = userpass.password0;
	superkey.m_Password1 = userpass.password1;
	superkey.m_Password2 = userpass.password2;

	for( int i=0;i<3;i++ ) {
		if( code=superkey.SuperKey_Init() ) { superkey.SuperKey_Close(); continue; }
		if( code=superkey.SuperKey_DecodeUserData(address,data) ) { superkey.SuperKey_Close(); continue; }
		break;
	}
	superkey.SuperKey_Close();
	return code;
	/*
	switch( address ) {
	case 3:		*data = 0x97;	break;
	case 4:		*data = 0x20;	break;
	case 5:		*data = 0x01;	break;
	case 6:		*data = 0x00;	break;
	case 7:		*data = 0x34;	break;
	case 11:	*data = 0xB8;	break;
	case 12:	*data = 0xC3;	break;
	case 13:	*data = 0x39;	break;
	case 14:	*data = 0x03;	break;
	case 15:	*data = 0x0F;	break;
	default:;
	}
	*/
	return 0;
}