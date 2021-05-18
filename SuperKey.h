/******************************************************************/
/***         Header of SuperKey.cpp                             ***/
/******************************************************************/
#ifndef	__SUPERKEY_HEADER__
#define	__SUPERKEY_HEADER__

#include "SUPERK5.H"

#define	SUPERKEY_PRODUCT_NO		3
#define	SUPERKEY_MAX_STATIONS	4
#define	SUPERKEY_VQ3_TRANS		5
#define	SUPERKEY_SYSTEM_MOID	6
#define	SUPERKEY_MD_MAILBOX		7

#define	SUPERKEY_MODI_NEW_BIT	11
#define	SUPERKEY_MODI_NEW_SUB	12
#define	SUPERKEY_MODI_ACK		13
#define	SUPERKEY_MODI_CONTROL	14
#define	SUPERKEY_MODI_MEASURE	15


#define	SUPERKEY_CONST_I	27

#define	ALGORITHM_0H		0x1972
#define	ALGORITHM_0L		0x0612
#define	ALGORITHM_1H		0x1977
#define	ALGORITHM_1L		0x0404
#define	ALGORITHM_2H		0x0000
#define	ALGORITHM_2L		0x0000
#define	ALGORITHM_3H		0x0000
#define	ALGORITHM_3L		0x0000
#define	ALGORITHM_4H		0x0000
#define	ALGORITHM_4L		0x0000
#define	ALGORITHM_5H		0x0000
#define	ALGORITHM_5L		0x0000

#define MODI_NEW_BIT		0x00B8
#define MODI_NEW_SUB		0x00C3
#define	MODI_ACK			0x0039
#define	MODI_CONTROL		0x0003
#define	MODI_MEASURE		0x000F

#define	PRODUCT_NO			0x0000
#define	MAX_STATIONS		0x0020
#define	MD_MAILBOX_NO		0x0034
#define	TM90_SYSTEM_MOID	0x0000
#define	VQ3_TRANS_CMD		0x0001
#define	TM90_SCADA_NUM		0x056B

#define	SOFTWARE_NUM		TM90_SCADA_NUM

//##ModelId=39C1EC0E00FC
class CSuperKey
{
private:
	//##ModelId=39C1EC0E0174
	short			m_Attribute[6];
	//##ModelId=39C1EC0E016E
	SUPERKEY_DATA	m_SKD;
public:
	//##ModelId=39C1EC0E016B
	short			m_Password0;
	//##ModelId=39C1EC0E016A
	short			m_Password1;
	//##ModelId=39C1EC0E0156
	short			m_Password2;
	//##ModelId=39C1EC0E014C
	char			m_ErrorCode[48];
	//##ModelId=39C1EC0E0142
	short			m_ProductNo;
	//##ModelId=39C1EC0E0124
	short			m_MaxStations;
	//##ModelId=39C1EC0E0110
	short			m_MdMailboxNo;
	//##ModelId=39C1EC0E0106
	short			m_LocalAddress;
private:
	//##ModelId=39C1EC0E0229
	short	SuperKey_ClearSKD(void);
	//##ModelId=39C1EC0E0217
	short	SuperKey_GenerateA(short,short,short*);
	//##ModelId=39C1EC0E0215
	short	SuperKey_GenerateB(short);
	//##ModelId=39C1EC0E020C
	short	SuperKey_GenerateD(short,short,short*);
	//##ModelId=39C1EC0E0202
	short	SuperKey_SetGroupAttribute(short,short);
public:
	//##ModelId=39C1EC0E0201
	CSuperKey();
	//##ModelId=39C1EC0E0200
	~CSuperKey();
	//		These functions are uesed by both the software designer & software user.
	//##ModelId=39C1EC0E01D8
	short	SuperKey_Init(void);
	//##ModelId=39C1EC0E01CE
	short	SuperKey_Close(void);
	//##ModelId=39C1EC0E01B2
	short	SuperKey_Read(short,short,short*);
	//##ModelId=39C1EC0E01AA
	short	SuperKey_Write(short,short,short);
	//##ModelId=39C1EC0E01A6
	short	SuperKey_Encrypt(short,short,short*);
	//		These functions are for the designer only.
	//##ModelId=39C1EC0E019E
	short	SuperKey_SetUserPassword(short,short,short);
	//##ModelId=39C1EC0E019C
	short	SuperKey_HardwareTest(void);
	//##ModelId=39C1EC0E018A
	short	SuperKey_PredefineGroup(short);
	//##ModelId=39C1EC0E0183
	short	SuperKey_TestGroup(short,short*);
	//##ModelId=39C1EC0E0181
	CString	SuperKey_ExplainErrorCode(short);
	//		These functions are for the user only.
	//##ModelId=39C1EC0E017E
	short	SuperKey_DecodeUserData(short,short*);
};

typedef	struct SENSEPASS
{
	short	password0;
	short	password1;
	short	password2;
} SENSE_PASSWORD;

short SenseDog_Read(SENSE_PASSWORD userpass,short address,short* data);
short SenseDog_Decode(SENSE_PASSWORD userpass,short address,short* data);

#endif	//__SUPERKEY_HEADER__
