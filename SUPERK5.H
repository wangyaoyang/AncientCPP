/*   (C) Copyright 1996 YaHui Technologies Inc. */


/*  SuperKey API functions. */

#define KeyAPIOpen          0  /* Open SuperKey */
#define KeyAPIInit          1  /* Clear Timer & EncryptRegister */
#define KeyAPIReadTimer     2  /* Read current Timer */
#define KeyAPIReadMemory    3  /* Read Memory cell */
#define KeyAPIWriteMemory   4  /* Write Memory cell */
#define KeyAPIEncrypt       5  /* Data Encrypt */
#define KeyAPIUnlock        6  /* Group Unlock */
#define KeyAPIClose         7  /* Close SuperKey */
#define KeyAPISetAttribute  8  /* Set group Attribute */
#define KeyAPISetUserPass   9  /* Set User Password */

/*  SuperKey API error codes. */

#define APISuccess          0  /* Success */
#define APIAddressOverflow  1  /* Invalid Address */
#define APITimerOverflow    2  /* Timer overflow */
#define APINotReadWrite     3  /* can't read & write */
#define APINotWrite         4  /* can't write,it's read_only cell */
#define APINotUnlock        5  /* Can't Unlock */
#define APINotEncrypt       6  /* Can't Encrypt */
#define APIUnlockDataErr    7  /* input Unlock Data error */
#define APINotProgramPass   8  /* it's UserPass,can't Program */
#define APIInvalidFunction  9  /* Invalid function */
#define APINotOpen          10 /* not Open yet, Please Open first */
#define APINoSuperKey       11 /* No Superkey or password error */
#define APIKeyOnWait        12
#define APINoKeyDriver          15 /* cant create driver,for winNT only */

/*  Group Attribute Format
   Example1: to set a group with a Timer and a Encrypt, the Attribute
	     must be: ( KeyAttrBlank | KeyAttrTimer | KeyAttrEncrypt )
   Example2: to set a Locked group with Lock Data, the Attribute
	     must be: ( KeyAttrBlank | KeyAttrClose | KeyAttrLock )
*/
#define KeyAttrBlank        0       /* default attribute */
#define KeyAttrClose        1       /* Locked group */
#define KeyAttrLock         4       /* set Lock data */
#define KeyAttrTimer        8       /* set Timer */
#define KeyAttrCounter      16      /* set Counter */
#define KeyAttrReadOnly     32      /* set normal cells read_only */
#define KeyAttrEncrypt      128     /* set Encrypt */
#define KeyAttrEAddr        0x100   /* for Encrypt */
#define KeyAttrEPPass       0x400   /* for Encrypt */
#define KeyAttrEUPass       0x800   /* for Encrypt */
#define KeyAttrEPrev        0x1000  /* for Encrypt */
#define KeyAttrETimerL      0x2000  /* for Encrypt */
#define KeyAttrETimerH      0x4000  /* for Encrypt */

/*  Structure of SUPERKEY_DATA. */

typedef struct {
 unsigned short ReturnFlag;   /* return error code for each function */
 unsigned short FunctionCode; /* function code */
 unsigned short MemAddress;   /* memory address or group number */
 unsigned short Password0;    /* program password or user password */
 unsigned short Password1;
 unsigned short Password2;
 unsigned short MemContent;   /* content for Read&Write or group Attribute */
 unsigned short Encrypt;      /* data input & output for Encrypt */
 unsigned short Timer;        /* timer */
 unsigned short Lock0;        /* unlock data or NewPassword */
 unsigned short Lock1;
 unsigned short Lock2;
 unsigned short Reserved[32]; /* reserved for system, can't change ! */
} SUPERKEY_DATA;

/*  SuperKey API function prototype. */

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI PutData(int data,char *str,int pos);
int WINAPI GetData(char *str,int pos);
void WINAPI SUPERKEY( SUPERKEY_DATA * );

#ifdef __cplusplus
}
#endif
