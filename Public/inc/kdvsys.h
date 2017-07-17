/*****************************************************************************
   ģ����      : KDV system
   �ļ���      : kdvsys.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV�����������궨��
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/01/30  0.9         ����        ����
******************************************************************************/
#ifndef _KDV_SYS_H_
#define _KDV_SYS_H_

/*#include "osp.h"*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "unicodedef.h"

#ifndef WIN32
/*#include <windows.h>*/
#define min(a,b) (a>b ? b : a)
#endif

#ifdef WIN32
/*    #ifdef KDVSYS_EXPORTS*/
        #define DllExport __declspec(dllexport)
/*    #else                  
        #define DllExport __declspec(dllimport)
    #endif*/
#else            /* VxWorks*/
    #define DllExport
#endif

#ifdef _LINUX_
#define min(a,b) (a>b ? b : a)
#endif

/* ����س� */
#if defined(WIN32) || defined(_LINUX_)
    #define     STR_RETURN          (LPCSTR)"\r\n"
#elif defined( VXWORKS_SIMU )
    #define     STR_RETURN          (LPCSTR)"\r\n"
#else            /* VxWorks */
    #define     STR_RETURN          (LPCSTR)"\n"
#endif

/* SNMP����FTP�ļ��ָ�� */
#define CHAR_FTPFILE_SEPARATOR		(char)'|'	/* ��ͬ�ļ�֮��ָ��� */
#define CHAR_FTPFILETYPE_SEPARATOR	(char)':'	/* �ļ���������֮��ָ��� */
#define CHAR_EXE_TYPE				(char)'E'	/* �������ļ����� */
#define CHAR_CFG_TYPE				(char)'C'	/* �����ļ����� */
#define CHAR_RES_TYPE				(char)'R'   /* ��Դ�ļ����� */

/* ����Profile�ļ��б����������ַ������� */
#define STR_ENTRY_NUM			(LPCSTR)"EntryNum"
#define STR_ENTRY				(LPCSTR)"Entry"
#define SECTION_ipRouteTable	(LPCSTR)"ipRouteTable"
#define FIELD_ipRouteDest		(LPCSTR)"ipRouteDest"
#define FIELD_ipRouteNextHop	(LPCSTR)"ipRouteNextHop"
#define FIELD_ipRouteMask		(LPCSTR)"ipRouteMask"

/* OID length */
#define OID_IP_ADDR_LEN     4 
#define OID_ID_LEN          1 

/* maximum size of physical address */
#define MAXSIZE_PHYADDR     16

/* Type definition */
#ifndef BOOL
#define BOOL	int
#endif
#ifndef BYTE
#define BYTE	unsigned char
#endif
#ifndef UCHAR
#define UCHAR	unsigned char
#endif
#ifndef WORD
#define WORD	unsigned short
#endif

#ifndef __H264__
#ifndef DWORD
#define DWORD	unsigned long
#endif
#else
#ifndef DWORD
#define DWORD	unsigned int
#endif
#endif

#ifndef WORD16
#define WORD16	WORD
#endif

//#ifndef WORD32
//#define WORD32	DWORD
//#endif

#ifndef __H264__
#ifndef WORD32
#define WORD32	unsigned long
#endif
#else
#ifndef WORD32
#define WORD32	unsigned int
#endif	
#endif

#ifndef SWORD
#define SWORD	short
#endif
#ifndef SDWORD
#define SDWORD	long
#endif
#ifndef UINT8
#define UINT8	BYTE
#endif
#ifndef UINT16
#define UINT16	WORD
#endif
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#ifndef UINT64
#define UINT64  unsigned long long
#endif


typedef UCHAR   MAC_TYPE[6];

//#ifndef _LINUX_
/* Value definition */
#ifndef TRUE
#define TRUE    (BOOL)1
#endif
#ifndef FALSE
#define FALSE   (BOOL)0
#endif
//#endif //_LINUX_

/* ·������ */
#if defined( WIN32 )    /* WIN32 */
#define PATH_FTP            (LPCSTR)"ftp"
#define PATH_CONFIG         (LPCSTR)"conf"
#define PATH_BIN			(LPCSTR)"."
#define PATH_RES			(LPCSTR)"res"
#elif defined( _LINUX_ )
#define PATH_FTP            (LPCSTR)"ftp"
//#define PATH_CONFIG         (LPCSTR)"conf"
#define PATH_CONFIG         (LPCSTR)"config"
#define PATH_BIN			(LPCSTR)"."
#define PATH_RES			(LPCSTR)"res"
#elif defined( _VXWORKS_ )
#define PATH_FTP            (LPCSTR)"/ftp"
#define PATH_CONFIG         (LPCSTR)"/conf"
#define PATH_BIN			(LPCSTR)""
#define PATH_RES			(LPCSTR)"res"
#endif

/* semTake��ʱ */
#define WAIT_SEM_TIMEOUT    2000    /* ticks */

/* SNMP PORT macro */
#define  SNMP_PORT          1161         /* port for snmp */
#define  TRAP_PORT          162         /* port for snmp traps */

/* SNMP TRAP�Զ���specificTrap */
#define SPECIFICTRAP_COLD_RESTART		1L
#define SPECIFICTRAP_POWERON			1L
#define SPECIFICTRAP_POWEROFF			2L
#define SPECIFICTRAP_ALARM_GENERATE     3L
#define SPECIFICTRAP_ALARM_RESTORE      4L

/* PrintMsg() typeȡֵ */
#define MESSAGE_INFO            (BYTE)0x1
#define MESSAGE_WARNING         (BYTE)0x2
#define MESSAGE_ERROR           (BYTE)0x3

#define MESSAGE_BUFFER_LEN      (WORD)1024   /* ��Ϣ�������󳤶� */

#ifndef MAX_PATH
    /* �������·������ */
    #define MAX_PATH            (WORD)256
#endif


#ifndef MAX_LINE_LEN
    /* ����һ�е���󳤶� */
    #define MAX_LINE_LEN        (WORD)1024*3
#endif

#define MAX_SECTIONNAME_LEN     (WORD)50
#define MAX_KEYNAME_LEN         (WORD)50
#define MAX_VALUE_LEN           (WORD)255

/* OpenProfile() byOpenMode ȡֵ */
#define PROFILE_READONLY        (BYTE)0x1       /*ֻ��*/
#define PROFILE_WRITE           (BYTE)0x2       /*��д*/
#define PROFILE_READWRITE       PROFILE_WRITE   /*��д*/
#define PROFILE_WRITEONLY       (BYTE)0x4       /*ֻд*/

#define PROFILE_MAX_OPEN        (WORD)20        /* ���ļ��������Ŀ */

typedef DWORD                   tKeyHandle;

#define INVALID_KEYHANDLE       (WORD)0xFFFF    /* ��Ч���ļ���� */

/* Profile�ļ�ע�ͷ����� */
#define PROFILE_COMMENT1        (LPCSTR)";"
#define PROFILE_COMMENT2        (LPCSTR)"//"
#define PROFILE_COMMENT3        (LPCSTR)"--"
#define PROFILE_COMMENT4        (LPCSTR)"#"

/* struct definition */
typedef struct
{
    WORD    dwAddrLen;                      /* Length of address */
    UCHAR   phyAddress[MAXSIZE_PHYADDR];    /* physical address value */
} PHYADDR_STRUCT;

typedef struct
{
    UCHAR   byDtiNo;
    UCHAR   byE1No;
} MCU_E1_STRUCT;

typedef struct
{
    UCHAR   byDtiNo;
    UCHAR   byE1No;
    UCHAR   byTsNo;
} MCU_TS_STRUCT;

typedef struct
{
    DWORD   dwTsNumber;                 /* TS number */
    MCU_TS_STRUCT   *ptMcuTs;           /* MCU TS array */
} MCU_TS_ARRAY_STRUCT;

typedef struct
{
    UCHAR   byE1No;
    UCHAR   byTsNo;
} MT_TS_STRUCT;

/* -----------------------  ϵͳ���ú���  ----------------------------- */
#define LOWORD16(l)     ((WORD16)(l))
#define HIWORD16(l)     ((WORD16)(((WORD32)(l) >> 16) & 0xFFFF))
#define MAKEDWORD(l,h)  ((WORD32)(((WORD)(l)) | ((WORD32)((WORD)(h))) << 16))

#ifndef _WINDEF_  /* Ϊ���ܹ���Windows��ʹ�� */
    #define LOBYTE(w)       ((BYTE)(w))
    #define HIBYTE(w)       ((BYTE)(((WORD16)(w) >> 8) & 0xFF))
    #define MAKEWORD(l,h)  ((WORD)(((BYTE)(l)) | ((WORD)((BYTE)(h))) << 8))
#endif

#ifdef _DEBUG 
	#undef ASSERT
	#define ASSERT(exp)    \
        { \
	      if ( !( exp ) ) \
		  { \
            printf((LPSTR)"Assert File %s, Line %d (%s)\n", \
	                 __FILE__, __LINE__, (LPSTR)#exp ); \
		  } \
        }
#else
    #undef ASSERT
    #define ASSERT( exp )    {}
#endif

/* ��ȡ�����ļ��ຯ�� */
DllExport BOOL GetRegKeyInt( LPCSTR lpszProfileName,    /* �ļ�����������·����*/
                   LPCSTR lpszSectionName,      /* Profile�еĶ���   */
                   LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                   const SDWORD sdwDefault,     /* ʧ��ʱ���ص�Ĭ��ֵ*/
                   SDWORD  *sdwReturnValue      /* ����ֵ            */
                 );
    /* �������ļ��ж�ȡһ������ֵ */

DllExport BOOL GetRegKeyStringTable( LPCSTR lpszProfileName,    /* �ļ�����������·����*/
                   LPCSTR lpszSectionName,      /* Profile�еĶ���   */        
                   LPCSTR lpszDefault,          /* ʧ��ʱ���ص�Ĭ��ֵ*/
                   LPSTR *lpszEntryArray,       /* �����ַ�������ָ�� */
                   DWORD *dwEntryNum,           /* �ַ���������Ŀ���ɹ��󷵻��ַ�������ʵ����
                                                Ŀ���紫�����Ŀ���������ļ�¼���贫�� */
                   DWORD dwBufSize              /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                 );
    /* �������ļ��ж�ȡһ�ű��������������
 */

DllExport BOOL GetRegKeyString( LPCSTR lpszProfileName, /* �ļ�����������·����*/
                      LPCSTR lpszSectionName,   /* Profile�еĶ���   */
                      LPCSTR lpszKeyName,       /* ���ڵĹؼ�����    */
                      LPCSTR lpszDefault,       /* ʧ��ʱ���ص�Ĭ��ֵ*/
                      LPSTR lpszReturnValue,    /* �����ִ�          */
                      DWORD dwBufSize           /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                    );
    /* �������ļ��ж�ȡһ�ַ�����ֵ */
    
DllExport BOOL GetRegKeyString2( FILE *stream,  /* �ļ���� */
                      LPCSTR lpszSectionName,   /* Profile�еĶ���   */
                      LPCSTR lpszKeyName,       /* ���ڵĹؼ�����    */
                      LPCSTR lpszDefault,       /* ʧ��ʱ���ص�Ĭ��ֵ*/
                      LPSTR lpszReturnValue,    /* �����ִ�          */
                      DWORD dwBufSize           /* �����ִ��ĳ��ȣ��粻����ضϣ���
                                                ��һ���ֽ�����'\0'    */
                    );
    /* �����ļ�����������ļ��ж�ȡһ�ַ���ֵ�����ļ�����"rb"��ʽ�� */
    
DllExport BOOL SetRegKeyInt( LPCSTR lpszProfileName,    /* �ļ�����������·����*/
                   LPCSTR lpszSectionName,      /* Profile�еĶ���   */
                   LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                   const SDWORD  sdwValue       /* �µ�����ֵ        */
                 );
    /* �������ļ���д��һ�µ������͵�ֵ��������������Զ����� */

DllExport BOOL SetRegKeyString( LPCSTR lpszProfileName, /* �ļ�����������·����*/
                      LPCSTR lpszSectionName,   /* Profile�еĶ���   */
                      LPCSTR lpszKeyName,       /* ���ڵĹؼ�����    */
                      LPCSTR lpszValue          /* �µ��ִ�ֵ, NULL���ʾɾ������ */
                    );
    /* �������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ����� */

#if 0//csp modify
DllExport BOOL DeleteRegKey( LPCSTR lpszProfileName,    /* �ļ�����������·����*/
                      LPCSTR lpszSectionName,   /* Profile�еĶ���   */
                      LPCSTR lpszKeyName        /* ���ڵĹؼ�����    */
                    );
    /* �������ļ���ɾ��һ��Key */
#endif

//ljl add
char* GetTableRegKeyString( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									BYTE   arossCoordinate, 
									BYTE   verticalCoordinate,
									LPSTR  lpszValue
									);

DllExport BOOL SetTableRegKeyString( LPCSTR lpszProfileName,   // �ļ�����������·����   
							LPCSTR lpszSectionName,   // Profile�еĶ���
							LPCSTR lpszLineName,
							BYTE   arossCoordinate,   // Ҫ���õ��µ�ֵ��ˮƽ�����λ�á�
							BYTE   verticalCoordinate, //Ҫ���õ��µ�ֵ�ڴ�ֱ�����λ�á�
							LPCSTR lpszValue        //�µ��ִ�ֵ, NULL���ʾɾ������
							);

DllExport BOOL SetTableRegKeyInt( LPCSTR lpszProfileName, // �ļ�����������·����   
								 LPCSTR lpszSectionName,  // Profile�еĶ���
								 LPCSTR lpszLineName,
								 BYTE   arossCoordinate,  // Ҫ���õ��µ�ֵ��ˮƽ�����λ�á�
								 BYTE   verticalCoordinate, //Ҫ���õ��µ�ֵ�ڴ�ֱ�����λ�á�
								 const SDWORD sdwValue    //�µ�����ֵ, NULL���ʾɾ������
								 );
//ljl add

//yzw add
DllExport BOOL GetRegKeyU64( LPCSTR lpszProfileName, 
                   LPCSTR lpszSectionName, 
                   LPCSTR lpszKeyName, 
                   UINT64  sdwDefault,
                   UINT64  *sdwReturnValue 
                 );

DllExport BOOL SetRegKeyU64( LPCSTR lpszProfileName,
				  LPCSTR lpszSectionName, 
				  LPCSTR lpszKeyName, 
				  const UINT64 sdwValue   
				);

DllExport BOOL GetStringFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                  					WORD   verticalCoordinate,
									LPSTR  lpszValue,
									DWORD dwBufSize
									);
									
DllExport BOOL GetValueFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                  					WORD   verticalCoordinate,
									SDWORD  *sdwReturnValue 
									);

DllExport BOOL GetU64ValueFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                  					WORD   verticalCoordinate,
									UINT64 *sdwReturnValue 
									);
									
DllExport BOOL SetStringToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                 					WORD   verticalCoordinate,
									LPSTR  lpszValue
									);
									
DllExport BOOL SetValueToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                  					WORD   verticalCoordinate,
									SDWORD sdwReturnValue 
									);

DllExport BOOL SetU64ValueToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName,          /* ���ڵĹؼ�����    */
                  					WORD   verticalCoordinate,
									UINT64 sdwReturnValue 
									);

//end

#if defined(_VXWORKS_) || defined(_LINUX_)
DllExport BOOL ReadRouteTable( const TCHAR * lpszProfileName );
#endif
	/* �������ļ��ж�ȡ·�ɱ����� */

/* �ַ��������ຯ�� */
DllExport void Trim( LPSTR lpszSrc );        /* ȥ���ַ�����ǰ���������ո��Լ�TAB�� */
DllExport void StrUpper( LPSTR lpszSrc );    /* �ַ���ת���ɴ�д */
DllExport void StrLower( LPSTR lpszSrc );    /* �ַ���ת����Сд */

/* �ڴ洦����*/
/*====================================================================
    ����        ���ͷŵ���̬���루malloc���ı�ṹ�ڴ�
    �������˵����void **ppMem, ָ������ָ��
                  DWORD dwEntryNum, ָ�������С
    ����ֵ˵��  ��void
====================================================================*/
/* DllExport void TableMemoryFree( void **ppMem, DWORD dwEntryNum ); */

/*====================================================================
    ����        ����֤�Ƿ�Ϸ�����
    �������˵����WORD wYear, ��
                  UCHAR byMonth, ��
                  UCHAR byDay, ��
                  UCHAR byHour, ʱ
                  UCHAR byMin, ��
                  UCHAR bySec, ��
    ����ֵ˵��  ���Ϸ�����TRUE�����𷵻�FALSE
====================================================================*/
DllExport BOOL IsValidTime( WORD wYear, UCHAR byMonth, UCHAR byDay, 
                           UCHAR byHour, UCHAR byMin, UCHAR bySec );


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _KDV_SYS_H_ */

/* end of file sys.h */

