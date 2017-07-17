/*****************************************************************************
   ģ����      : KDV system
   �ļ���      : kdvsys.c
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV�����������궨��
   ����        : ����
   �汾        : V0.9  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2002/01/30  0.9         ����        ����
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "kdvsys.h"
#include "public.h"

#define INI_OPT

/*====================================================================
    ������      ��OpenProfile
    ����        ���������ļ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����tKeyHandle *lpKeyHandle, Profile����ָ��   
                  LPCSTR lpszProfileName, Profile�ļ���
                  BYTE  byOpenMode, �򿪷�ʽ
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport BOOL OpenProfile( tKeyHandle *lpKeyHandle,
                  LPCSTR lpszProfileName,
                  BYTE  byOpenMode
                )
{
    TCHAR    achMode[5];

    switch( byOpenMode )
    {
    case PROFILE_READONLY:
        strcpy( achMode, "rt" );
        break;
    case PROFILE_READWRITE:
    case PROFILE_WRITEONLY:
        strcpy( achMode, "rt+" );
        break;
    default:
        return( FALSE );
    }

    *lpKeyHandle = ( tKeyHandle )fopen( lpszProfileName, achMode );
    if( ( FILE * )(*lpKeyHandle) == NULL )
    {
#ifdef _DEBUG
        printf( "Open file %s failed!\n", lpszProfileName );
#endif
        return( FALSE );
    }
    else
    {
#ifdef _DEBUG
        printf( "Open file %s succeeded! KeyHandle = %d\n", 
            lpszProfileName, *lpKeyHandle );
#endif
        return( TRUE );
    }
}*/
   
/*====================================================================
    ������      ��CloseProfile
    ����        ���ر������ļ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport BOOL CloseProfile ( tKeyHandle lpKeyHandle )
{
    int iReturn = fclose( ( FILE * )lpKeyHandle );

    if( iReturn == 0 )
    {
#ifdef _DEBUG
        printf( "Close file %d succeeded!\n", lpKeyHandle );
#endif
        return( TRUE );
    }
    else
    {
#ifdef _DEBUG
        printf( "Close file %d failed!\n", lpKeyHandle );
#endif
        return( FALSE );
    }
} */   

/*====================================================================
    ������      ��WriteKeyValueString
    ����        ���ӵ�ǰ�α괦��ʼ����ָ���ؼ���������û����ö�Ӧ����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  SDWORD sdwOffsetCurLine, Ҫд����п�ʼ��ƫ����
                  SDWORD sdwOffsetNextLine, ��һ�п�ʼ��ƫ����
                  LPCSTR lpszKeyName, Ҫд��Ĺؼ�����
                  LPCSTR lpszKeyValue, Ҫд��Ĺؼ���ֵ�����ΪNULL��
                    ɾ������
    ����ֵ˵��  ����ǰ�½�β��ƫ������ʧ�ܷ���-1
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/31    1.0         Li, Yi        ����
====================================================================*/
SDWORD WriteKeyValueString( FILE *stream, 
                SDWORD sdwOffsetCurLine, SDWORD sdwOffsetNextLine, 
                LPCSTR lpszKeyName, LPCSTR lpszKeyValue )
{
	DWORD dwLen;
	LPSTR lpszBuf = NULL;
	
	ASSERT(lpszKeyName != NULL);
	
	/* copy the remaining content in buffer */
	fseek(stream, 0, SEEK_END);
	dwLen = ftell(stream) - sdwOffsetNextLine;
	
	TCHAR nBuf[dwLen];//zlb20111117 ȥ������malloc
	//printf("WriteKeyValueString:dwLen=%ld,sizeof(TCHAR)=%d,sizeof(nBuf)=%d\n",dwLen,sizeof(TCHAR),sizeof(nBuf));
	
	if(dwLen != 0)
	{
		lpszBuf = nBuf;
		fseek(stream, sdwOffsetNextLine, SEEK_SET);
		dwLen = fread(lpszBuf, sizeof(TCHAR), dwLen, stream);
	}
	
	/* write the new value or delete it */
	fseek(stream, sdwOffsetCurLine, SEEK_SET);
	if(lpszKeyValue != NULL)
	{
		fputs((LPCSTR)lpszKeyName, stream);
		fputs((LPCSTR)" = ", stream);
		fputs((LPCSTR)lpszKeyValue, stream);
		fputs((LPCSTR)STR_RETURN, stream);
	}
	
	/* write the remaining content in the buffer back */
	if(dwLen != 0)
	{
		fwrite(lpszBuf, sizeof(TCHAR), dwLen, stream);
	}
	
	//if( lpszBuf )//zlb20111117 ȥ������malloc
	//{
	//    free(lpszBuf);
	//}
	
	return(ftell(stream));
}

#ifdef INI_OPT
u32 GoSection(FILE *stream, LPCSTR lpszSectionName, BOOL bCreate)
{
	TCHAR achKeyValue[MAX_LINE_LEN + 1];
	SDWORD nCurInd;
	
	if(FALSE != GetRegKeyString2(stream, "SECTIONS", lpszSectionName, NULL, achKeyValue, sizeof(achKeyValue)))
	{
		if((strlen(achKeyValue) > 2) && ((*achKeyValue == '0') && ((*(achKeyValue+1) == 'x') || (*(achKeyValue+1) == 'X'))))
		{
			sscanf(achKeyValue, "%lx", &nCurInd);//csp modify
		}
		else
		{
			nCurInd = atoi(achKeyValue);
		}
		
		if(nCurInd < 0)//cw_test
		{
			nCurInd = 0;
		}
		
		/* move to the section */
		if(fseek(stream, nCurInd, SEEK_SET) != 0)
		{
			printf("GoSection %s fseek failed\n", lpszSectionName);
			return 0;
		}
		
		return ftell(stream);
	}
	
	return 0;
} 

SDWORD CalculateSection(FILE *stream, LPCSTR lpszSectionName, BOOL bCreate)
{
	TCHAR achRow[MAX_LINE_LEN + 1];
	TCHAR achSectionCopy[MAX_SECTIONNAME_LEN + 1];
	DWORD dwLen;
	LPSTR lpszReturn;
	
	ASSERT(strlen(lpszSectionName) <= MAX_SECTIONNAME_LEN);
	
	s32 nCurReal = -1;
	int nTryTime = 2;
	
	/* copy the section name and change it */
	strncpy(achSectionCopy, lpszSectionName, MAX_SECTIONNAME_LEN);
	achSectionCopy[MAX_SECTIONNAME_LEN] = '\0';
	Trim(achSectionCopy);
	StrUpper(achSectionCopy);
	
	do
	{
		nCurReal = ftell(stream);
		//PUBPRT("here");
		/* if error or arrive at the end of file */
		if(fgets(achRow, MAX_LINE_LEN, stream) == NULL)
		{
			nTryTime--;
			if(nTryTime <= 0)
			{
				if(bCreate && feof(stream))/* create the section */
				{
					fputs((LPCSTR)STR_RETURN, stream);
					fputs((LPCSTR)"[", stream);
					fputs((LPCSTR)lpszSectionName, stream);
					fputs((LPCSTR)"]", stream);
					fputs((LPCSTR)STR_RETURN, stream);
					
					//csp modify
					//fseek(stream, 0, SEEK_END);
					//return (ftell(stream));
					return nCurReal;
				}
				else
				{
					return (-1);
				}
			}
			else
			{
				fseek(stream, 0, SEEK_SET);
			}
		}
		//PUBPRT("here");
		/* eliminate the return key */
		if((lpszReturn = strstr(achRow, _T("\r\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		else if((lpszReturn = strstr(achRow, _T("\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		Trim(achRow);
		dwLen = strlen(achRow);
		
		/* must be a section */
		if(dwLen <= 2 || achRow[0] != '[' || achRow[dwLen - 1] != ']')
		{
			continue;
		}
		//PUBPRT("here");
		/* verify the section name */
		strncpy(achRow, achRow + 1, dwLen - 2);
		achRow[dwLen - 2] = '\0';
		Trim(achRow);
		StrUpper(achRow);
		if(strcmp(achRow, achSectionCopy) == 0)/* identical */
		{
			//printf("achRow:%s\n", achRow);
			//PUBPRT("here");
			return nCurReal;
		}
		//PUBPRT("here");	
	}while(TRUE);
}
#endif

/*====================================================================
    ������      : MoveToSection
    ����        �����α��Ƶ�ָ���Ķ�����һ�п�ʼ��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  LPCSTR lpszSectionName, Profile�еĶ���
                  BOOL bCreate, ���û�иö����Ƿ񴴽�
    ����ֵ˵��  ������ƫ����Offset�����ʧ�ܷ���-1
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
SDWORD MoveToSection(FILE *stream, LPCSTR lpszSectionName, BOOL bCreate)
{
	TCHAR achRow[MAX_LINE_LEN + 1];
	TCHAR achSectionCopy[MAX_SECTIONNAME_LEN + 1];
	DWORD dwLen;
	LPSTR lpszReturn;
	
	ASSERT(strlen(lpszSectionName) <= MAX_SECTIONNAME_LEN);
	
	/* move from the beginning */
	if(fseek(stream, 0, SEEK_SET) != 0)
	{
		//printf("MoveToSection %s fseek failed 222\n", lpszSectionName);
		return (-1);
	}
	
#ifdef INI_OPT
	s32 nCurGet = 0, nAction = 0;//s32 nCurGet = 0, nCurReal = 0, nAction = 0;//csp modify
	int nTryTime = 2;
	
	if(0 != strcmp("SECTIONS", lpszSectionName))
	{
		nAction = 1;
		nCurGet = GoSection(stream, lpszSectionName, FALSE);
		if(0 == nCurGet)
		{
			//printf("GoSection %s failed\n", lpszSectionName);
			
			/* move from the beginning */
			if(fseek(stream, 0L, SEEK_SET) != 0)
			{
				return -1;
			}
		}
	}
	else
	{
		nAction = 0;
		/* move from the end */
		//if(fseek( stream, -1000L, SEEK_END) != 0)
		if(fseek(stream, -2000L, SEEK_END) != 0)//cw_ptz
		{
			//printf("MoveToSection %s fseek failed 222\n", lpszSectionName);
			return (-1);
		}
		bCreate = TRUE;
		nTryTime = 1;
	}
#endif
	
	/* copy the section name and change it */
	strncpy(achSectionCopy, lpszSectionName, MAX_SECTIONNAME_LEN);
	achSectionCopy[MAX_SECTIONNAME_LEN] = '\0';
	Trim(achSectionCopy);
	StrUpper(achSectionCopy);
	
	do
	{
		/* if error or arrive at the end of file */
		if(fgets(achRow, MAX_LINE_LEN, stream) == NULL)
		{
		#ifdef INI_OPT
			nTryTime--;
			if(nTryTime <= 0)
			{
		#endif
				if(bCreate && feof(stream))/* create the section */
				{
					fputs((LPCSTR)STR_RETURN, stream);
					fputs((LPCSTR)"[", stream);
					fputs((LPCSTR)lpszSectionName, stream);
					fputs((LPCSTR)"]", stream);
					fputs((LPCSTR)STR_RETURN, stream);
					fseek(stream, 0, SEEK_END);
					return (ftell(stream));
				}
				else
				{
					return (-1);
				}
		#ifdef INI_OPT
			}
			else
			{
				fseek(stream, 0, SEEK_SET);
			}
		#endif
		}
		
		/* eliminate the return key */
		if((lpszReturn = strstr(achRow, _T("\r\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		else if((lpszReturn = strstr(achRow, _T("\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		
		Trim(achRow);
		dwLen = strlen(achRow);
		
		/* must be a section */
		if(dwLen <= 2 || achRow[0] != '[' || achRow[dwLen - 1] != ']')
		{
			continue;
		}
		
		/* verify the section name */
		strncpy(achRow, achRow + 1, dwLen - 2);
		achRow[dwLen - 2] = '\0';
		Trim(achRow);
		StrUpper(achRow);
		if(strcmp(achRow, achSectionCopy) == 0)/* identical */
		{
			return (ftell(stream));
		}
	}while(TRUE);
}

/*====================================================================
    ������      ��ReadWriteKeyValueString
    ����        ���ӵ�ǰ�α괦��ʼ����ָ���ؼ���������û����ö�Ӧ����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, Profile����   
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
                  LPSTR lpszKeyValue, ���ػ����õĹؼ��ֶ�Ӧֵ�ַ���ָ�룬
                    ��С��Ԥ�ȷ��䣬���Ϊд��ʽ�Ҹ�ָ��ΪNULL����ɾ����
                    ��
                  SDWORD sdwOffsetCurSection, ��ǰҪ��д��Section����ʼ
                    ƫ�������Ӹ�Section��ʼ
                  DWORD dwBufSize, ���ػ����õĹؼ��ֶ�Ӧֵ�ַ���Buffer��
                    ��С���粻�����Զ��ضϣ����һ���ֽ�����'\0'
                  BOOL bWrite, �Ƿ�д�����д�Ļ����Զ�����
    ����ֵ˵��  ����ǰ�½�β��ƫ������ʧ�ܷ���-1��READ������ֵ������
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
SDWORD ReadWriteKeyValueString( FILE *stream, LPCSTR lpszKeyName, 
                        LPSTR lpszKeyValue, SDWORD sdwOffsetCurSection, 
                        DWORD dwBufSize, BOOL bWrite )
{
	TCHAR achRow[MAX_LINE_LEN + 1];
	TCHAR achRowKeyName[MAX_KEYNAME_LEN + 1];
	TCHAR achKeyNameCopy[MAX_KEYNAME_LEN + 1];
	LPSTR lpszEqualPos;
	LPSTR lpszReturn;
	DWORD dwLen;
	SDWORD sdwOffsetCurLine;
	SDWORD sdwOffsetNextLine;
	
	ASSERT(lpszKeyName != NULL && strlen(lpszKeyName) <= MAX_KEYNAME_LEN);
	ASSERT(!bWrite || (lpszKeyValue != NULL && strlen(lpszKeyValue) <= MAX_VALUE_LEN));
	
	/* copy the Key name and change it */
	strncpy(achKeyNameCopy, lpszKeyName, MAX_KEYNAME_LEN);
	achKeyNameCopy[MAX_KEYNAME_LEN] = '\0';
	Trim(achKeyNameCopy);
	StrUpper(achKeyNameCopy);
	
	fseek(stream, sdwOffsetCurSection, SEEK_SET);
	
	do
	{
		/* record the offset in every line */
		sdwOffsetCurLine = ftell(stream);
		
		if(fgets(achRow, MAX_LINE_LEN, stream) == NULL)
		{
			/* the end */
			if(bWrite && lpszKeyValue != NULL && feof(stream))/* create the Key */
			{
				sdwOffsetNextLine = sdwOffsetCurLine;
				return (WriteKeyValueString(stream, sdwOffsetCurLine, sdwOffsetNextLine, lpszKeyName, lpszKeyValue));
			}
			else
			{
				return (-1);
			}
		}
		
		/* eliminate the return key */
		if((lpszReturn = strstr(achRow, _T("\r\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		else if((lpszReturn = strstr(achRow, _T("\n"))) != NULL)
		{
			lpszReturn[0] = '\0';
		}
		Trim(achRow);
		dwLen = strlen(achRow);
		
		if(dwLen == 0) continue;
		
		/* if arrive at another section */
		if(achRow[0] == '[')
		{
			if(bWrite && lpszKeyValue != NULL)
			{
				sdwOffsetNextLine = sdwOffsetCurLine = sdwOffsetCurSection;
				return (WriteKeyValueString( stream, sdwOffsetCurLine, sdwOffsetNextLine, lpszKeyName, lpszKeyValue));
			}
			else
			{	
				return (-1);
			}
		}
		
		/* if arrive at a comment or cannot find a '=', continue */
		if(strstr(achRow, PROFILE_COMMENT1) == achRow || 
			strstr( achRow, PROFILE_COMMENT2 ) == achRow || 
			strstr( achRow, PROFILE_COMMENT3 ) == achRow || 
			strstr( achRow, PROFILE_COMMENT4 ) == achRow || 
			(lpszEqualPos = strchr(achRow, '=')) == NULL)
		{
			continue;
		}
		
		/* verify the key name */
		memcpy(achRowKeyName, achRow, (lpszEqualPos - achRow)*sizeof(TCHAR));
		achRowKeyName[lpszEqualPos - achRow] = '\0';
		Trim(achRowKeyName);
		StrUpper(achRowKeyName);
		if(strcmp(achRowKeyName, achKeyNameCopy) != 0)/* not identical */
		{
			continue;
		}
		
		/* return or set the key value string */
		if(bWrite)
		{
			sdwOffsetNextLine = ftell(stream);
			return (WriteKeyValueString(stream, sdwOffsetCurLine, sdwOffsetNextLine, lpszKeyName, lpszKeyValue));
		}
		else
		{
			Trim(lpszEqualPos + 1);
			strncpy(lpszKeyValue, lpszEqualPos + 1, dwBufSize - 1);
			lpszKeyValue[dwBufSize - 1] = '\0';
			return (ftell(stream));
		}
	}while(TRUE);
}

/*====================================================================
    ������      ��GetRegKeyInt
    ����        ���������ļ��ж�ȡһ������ֵ
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����
                  LPCSTR lpszSectionName, Profile�еĶ���   
                  LPCSTR lpszKeyName, ���ڵĹؼ�����    
                  const SDWORD  sdwDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  SDWORD  *sdwReturnValue, ����ֵ                         
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL GetRegKeyInt( LPCSTR lpszProfileName, 
                   LPCSTR lpszSectionName, 
                   LPCSTR lpszKeyName, 
                   SDWORD  sdwDefault,
                   SDWORD  *sdwReturnValue 
                 )
{
	TCHAR   achKeyValue[MAX_VALUE_LEN + 1];
	BOOL    bResult;
	
	*sdwReturnValue = sdwDefault;
	
	bResult = GetRegKeyString(lpszProfileName, lpszSectionName, lpszKeyName, NULL, achKeyValue, sizeof(achKeyValue));
	
	if(bResult != FALSE)
	{
		if((strlen(achKeyValue) > 2) && ((*achKeyValue == '0') && ((*(achKeyValue+1) == 'x') || (*(achKeyValue+1) == 'X'))))
		{
			sscanf(achKeyValue, "%lx", sdwReturnValue);//csp modify
		}
		else
		{
			*sdwReturnValue = atoi(achKeyValue);
		}
	}
	
	return (bResult);
}
    
/*====================================================================
    ������      ��GetRegKeyStringTable
    ����        ���������ļ��ж�ȡһ�ű��������������
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  LPSTR  lpszReturnValue, �����ִ�
                  LPSTR *lpszEntryArray, �����ַ�������ָ��
                  SDWORD *sdwEntryNum, �ַ���������Ŀ���ɹ��󷵻��ַ�����
                        ��ʵ����Ŀ���紫�����Ŀ���������ļ�¼���贫��
                  DWORD  dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/02/06    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL GetRegKeyStringTable( LPCSTR lpszProfileName, 
                   LPCSTR lpszSectionName, 
                   LPCSTR lpszDefault, 
                   LPSTR *lpszEntryArray, 
                   DWORD *dwEntryNum,
                   DWORD dwBufSize 
                 )
{
	printf("111111111111111\n");
	
	TCHAR achKeyName[MAX_KEYNAME_LEN + 1];
	BOOL  bResult = TRUE;
	DWORD dwFileEntryNum;/* number of entries from profile */
	DWORD dwReadEntryNum;/* real number of entries read from profile */
	DWORD dwLoop;
	FILE  *stream;
	
	/* copy the default string */    
	if(lpszDefault != NULL)
	{
		printf("222222222222222222\n");
		for(dwLoop = 0; dwLoop < *dwEntryNum; dwLoop++)
		{
			printf("333333333333333333333333\n");
			strncpy(lpszEntryArray[dwLoop], lpszDefault, dwBufSize - 1);
			printf("222222222222222222\n");
			lpszEntryArray[dwLoop][dwBufSize - 1] = '\0';
		}
	}
	
	bResult = GetRegKeyInt(lpszProfileName, lpszSectionName, (LPCSTR)STR_ENTRY_NUM, 1, (SDWORD*)(&dwFileEntryNum));/* get the number of table entries from profile */
	if(bResult == FALSE) return (FALSE);
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		return (FALSE);
	}
	
#ifdef INI_OPT
	s32 nCurGet = 0, nCurReal = 0, nAction = 0;
	if(0 != strcmp("SECTIONS", lpszSectionName))
	{
		nAction = 1;
		nCurGet = GoSection(stream, lpszSectionName, FALSE);
	 	if(0 == nCurGet)
		{
			/* move from the beginning */
			if(fseek(stream, 0L, SEEK_SET) != 0)
			{
				PUBPRT("HERE err");
				fclose(stream);
				return (FALSE);
			}
		}
		
		if((nCurReal = CalculateSection(stream, lpszSectionName, TRUE)) == -1)
		{
			PUBPRT("HERE err");
		    fclose(stream);
		    return (FALSE);
		}
	}
#endif
	
	dwReadEntryNum = 0;
	for(dwLoop = 0; dwLoop < min(dwFileEntryNum, *dwEntryNum); dwLoop++)
	{
#ifdef INI_OPT
		if(1 == nAction)
		{
			fseek(stream, nCurReal, SEEK_SET);
		}
#endif
		sprintf(achKeyName, _T("%s%lu"), STR_ENTRY, dwLoop);
		bResult = GetRegKeyString2(stream, lpszSectionName, achKeyName, NULL, lpszEntryArray[dwReadEntryNum], dwBufSize);
		if(bResult == FALSE)
		{
			break;//continue;//csp modify
		}
		dwReadEntryNum++;
	}
	
	*dwEntryNum = dwReadEntryNum;
	
	fclose(stream);
	
#ifdef INI_OPT
	if(1 == nAction)
	{
		if(nCurReal != nCurGet)
		{
			SetRegKeyInt(lpszProfileName, "SECTIONS", lpszSectionName, nCurReal);
		}
	}
#endif
	
	return (TRUE);
}
    
/*====================================================================
    ������      ��GetRegKeyString
    ����        ���������ļ��ж�ȡһ�ַ���ֵ
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����  
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
                  LPCSTR lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  LPSTR  lpszReturnValue, �����ִ�
                  DWORD  dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL GetRegKeyString( LPCSTR lpszProfileName,
                      LPCSTR lpszSectionName,
                      LPCSTR lpszKeyName, 
                      LPCSTR lpszDefault, 
                      LPSTR  lpszReturnValue, 
                      DWORD  dwBufSize 
                    )
{
	FILE *stream;
	BOOL bResult;
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		return (FALSE);
	}
	
#ifdef INI_OPT
	s32 nCurGet = 0, nCurReal = 0, nAction = 0;
	if(0 != strcmp("SECTIONS", lpszSectionName))
	{
		nAction = 1;
		
		nCurGet = GoSection(stream, lpszSectionName, FALSE);
	 	if(0 == nCurGet)
	 	{
	 		//csp modify
	 		//printf("GetRegKeyString:GoSection <%s> failed\n",lpszSectionName);
			
			/* move from the beginning */
			if(fseek(stream, 0L, SEEK_SET) != 0)
			{
				PUBPRT("HERE err");
				fclose(stream);
				return (FALSE);
			}
	 	}
		
		if((nCurReal = CalculateSection(stream, lpszSectionName, TRUE)) == -1)
		{
			PUBPRT("HERE err");
		    fclose(stream);
		    return (FALSE);
		}
		
		fseek(stream, nCurReal, SEEK_SET);
	}
#endif
	
	bResult = GetRegKeyString2(stream, lpszSectionName, lpszKeyName, lpszDefault, lpszReturnValue, dwBufSize);
	
	fclose(stream);
	
#ifdef INI_OPT
	if(1 == nAction)
	{
		if(nCurReal != nCurGet)
		{
			SetRegKeyInt(lpszProfileName, "SECTIONS", lpszSectionName, nCurReal);
		}
	}
#endif
	
	return (bResult);
}
    
/*====================================================================
    ������      ��GetRegKeyString2
    ����        �������ļ�����������ļ��ж�ȡһ�ַ���ֵ�����ļ�����
                    "rb"��ʽ��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����FILE *stream, �ļ����
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
                  LPCSTR lpszDefault, ʧ��ʱ���ص�Ĭ��ֵ
                  LPSTR  lpszReturnValue, �����ִ�
                  DWORD  dwBufSize, �����ִ�BUFFER�ĳ��ȣ��粻����ضϣ���
                        ��һ���ֽ�����'\0'
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL GetRegKeyString2( FILE *stream,
                      LPCSTR lpszSectionName,
                      LPCSTR lpszKeyName, 
                      LPCSTR lpszDefault, 
                      LPSTR  lpszReturnValue, 
                      DWORD  dwBufSize 
                    )
{
	SDWORD sdwOffset;
	
	ASSERT(lpszSectionName != NULL && lpszKeyName != NULL && lpszReturnValue != NULL);
	
	ASSERT(stream != NULL);
	
	/* copy the default string */    
	if(lpszDefault != NULL)
	{
		strncpy(lpszReturnValue, lpszDefault, dwBufSize - 1);
		lpszReturnValue[dwBufSize - 1] = '\0';
	}
	
	/* find the section first */
	if((sdwOffset = MoveToSection(stream, lpszSectionName, FALSE)) == -1)
	{
#if _DEBUG
		printf("Searching section [%s] in file %lu failed!\n", lpszSectionName, (DWORD)stream);
#endif
		//PUBPRT("HERE err");
		return (FALSE);
	}
	
	/* continue to find the right key */
	if(ReadWriteKeyValueString(stream, lpszKeyName, lpszReturnValue, sdwOffset, dwBufSize, FALSE) == -1)
	{
#if _DEBUG
		printf("Searching key [%s] in file %lu failed!\n", lpszKeyName, (DWORD)stream);
#endif
		//PUBPRT("HERE err");
		return (FALSE);
	}
	
	return (TRUE);
}
    
/*====================================================================
    ������      ��SetRegKeyInt
    ����        ���������ļ���д��һ�µ������͵�ֵ��������������Զ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
                  const SDWORD sdwValue, �µ�����ֵ
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL SetRegKeyInt( LPCSTR lpszProfileName,
                   LPCSTR lpszSectionName, 
                   LPCSTR lpszKeyName, 
                   const SDWORD sdwValue   
                 )
{
	TCHAR achKeyValue[MAX_VALUE_LEN + 1];
	BOOL bResult;
	
	sprintf((LPSTR)achKeyValue, _T("%ld"), sdwValue);
	
	bResult = SetRegKeyString(lpszProfileName, lpszSectionName, lpszKeyName, achKeyValue);
	return (bResult);
}    

/*====================================================================
    ������      ��SetRegKeyString
    ����        ���������ļ���д��һ�µ��ַ����͵�ֵ��������������Զ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����   
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
                  LPCSTR lpszValue, �µ��ִ�ֵ, NULL���ʾɾ������
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL SetRegKeyString( LPCSTR lpszProfileName, 
                      LPCSTR lpszSectionName, 
                      LPCSTR lpszKeyName, 
                      LPCSTR lpszValue
                    )
{
	SDWORD sdwOffset;
	FILE *stream;
	LPSTR lpszBuf = NULL;
	
	ASSERT(lpszSectionName != NULL && lpszKeyName != NULL);
	
	stream = fopen(lpszProfileName, _T("rb+"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		return (FALSE);
	}
	
	/* find the section first, if not exist, create it */
	if((sdwOffset = MoveToSection(stream, lpszSectionName, TRUE)) == -1)
	{
/*#ifdef _DEBUG
		printf( "Creating section [%s] in file %s failed!\n", lpszSectionName, lpszProfileName );
#endif*/
		fclose(stream);
		return (FALSE);
	}

	/* continue to find the right key and write the value */
	if((sdwOffset = ReadWriteKeyValueString(stream, lpszKeyName, (LPSTR)lpszValue, sdwOffset, 0, TRUE)) == -1)
	{
/*#ifdef _DEBUG
		printf("Searching for or Writing Key %s in file %s failed!\n", lpszKeyName, lpszProfileName);
#endif*/
		fclose(stream);
		return (FALSE);
	}
	
	/* record the whole new file */
	TCHAR nBuf[sdwOffset];//zlb20111117 ȥ������malloc
	lpszBuf = nBuf;
	fseek(stream, 0, SEEK_SET);
	sdwOffset = fread(lpszBuf, sizeof(TCHAR), sdwOffset, stream);
	
	fclose(stream);
	
	/* rewrite the file */
	stream = fopen(lpszProfileName, _T("wb"));
	ASSERT(stream != NULL);
	fwrite(lpszBuf, sizeof(TCHAR), sdwOffset, stream);
	
	fclose(stream);
	
	//free(lpszBuf);//zlb20111117 ȥ������malloc
	
	return (TRUE);
}

#if 0//csp modify
/*====================================================================
    ������      ��DeleteRegKey
    ����        ���������ļ���ɾ��һ��Key
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·���� 
                  LPCSTR lpszSectionName, Profile�еĶ���
                  LPCSTR lpszKeyName, ���ڵĹؼ�����
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/02/01    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL DeleteRegKey( LPCSTR lpszProfileName, 
                      LPCSTR lpszSectionName, 
                      LPCSTR lpszKeyName 
                    )
{
	BOOL bResult;
	
	bResult = SetRegKeyString(lpszProfileName, lpszSectionName, lpszKeyName, NULL);
	
	return( bResult );
}
#endif

#if 0//csp modify
#ifdef _VXWORKS_
extern STATUS	routeAdd( TCHAR *destination, TCHAR *gateway );
extern STATUS	mRouteAdd( TCHAR *, TCHAR *, long, int, int );

/*====================================================================
    ������      ��ReadRouteTable
    ����        ������RouteTable��Ϣ������·��
    �㷨ʵ��    ��
    ����ȫ�ֱ�������
    �������˵����LPCSTR lpszProfileName, �ļ�����������·����
    ����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/06/26    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL ReadRouteTable( LPCSTR lpszProfileName )
{
    TCHAR   achEntry[MAX_VALUE_LEN + 1];
    TCHAR   chSeps[] = " \t";       /* space or tab as seperators */
    TCHAR   *pchToken;
    DWORD   dwLoop;
    BOOL    bResult = TRUE;
    DWORD   dwEntryNum;
    TCHAR   achTemp[64];
    TCHAR   achDestIpAddr[32];
    TCHAR   achNextHopIpAddr[32];
    DWORD   dwMask;
    TCHAR   *pchStopStr;

    /* get the number of entry */
    bResult = GetRegKeyInt( lpszProfileName, SECTION_ipRouteTable, STR_ENTRY_NUM, 
                0, ( SDWORD * )&dwEntryNum );

    /* analyze entry strings */
    for( dwLoop = 0; dwLoop < dwEntryNum; dwLoop++ )
    {
        sprintf( achTemp, "%s%lu", STR_ENTRY, dwLoop );
		bResult = GetRegKeyString( lpszProfileName, SECTION_ipRouteTable, achTemp, 
			"cannot find the entry", achEntry, sizeof( achEntry ) ) ;

        if( !bResult )
			continue;

		/* ipRouteDest */
        pchToken = strtok( achEntry, chSeps );
        if( pchToken == NULL )
        {
            printf( "Wrong profile while reading %s\n!", FIELD_ipRouteDest );
            bResult = FALSE;
        }
        else
        {
            strncpy( achDestIpAddr, pchToken, sizeof( achDestIpAddr ) );
			achDestIpAddr[sizeof( achDestIpAddr ) - 1] = '\0';
        }
        /* ipRouteNextHop */
        pchToken = strtok( NULL, chSeps );
        if( pchToken == NULL )
        {
            printf( "Wrong profile while reading %s\n!", FIELD_ipRouteNextHop );
            bResult = FALSE;
        }
        else
        {
            strncpy( achNextHopIpAddr, pchToken, sizeof( achNextHopIpAddr ) );
			achNextHopIpAddr[sizeof( achNextHopIpAddr ) - 1] = '\0';
        }
        /* ipRouteMask */
        pchToken = strtok( NULL, chSeps );
        if( pchToken == NULL || ( memcmp( pchToken, "0x", 2 ) != 0 && memcmp( pchToken, "0X", 2 ) != 0 ) )
        {
            printf( "Wrong profile while reading %s\n!", FIELD_ipRouteMask );
            bResult = FALSE;
        }
        else
        {
            dwMask = strtoul( pchToken + 2, &pchStopStr, 16 );
        }

		/* add into route table */
#ifndef VXWORKS_SIMU
		if( dwMask == 0xffffffff )
		{
			if( routeAdd( achDestIpAddr, achNextHopIpAddr ) == ERROR )
			{
				printf( "Add route Dest %s, NextHop %s failed!\n", achDestIpAddr, achNextHopIpAddr );
			}
			else
			{
				printf( "Add route Dest %s, NextHop %s, Mask 0x%lX succeeded!\n", 
					achDestIpAddr, achNextHopIpAddr, dwMask );
			}
		}
		else
		{
			if( mRouteAdd( achDestIpAddr, achNextHopIpAddr, dwMask, 0, 0 ) == ERROR )
			{
				printf( "Add route Dest %s, NextHop %s, Mask 0x%lX failed!\n", 
					achDestIpAddr, achNextHopIpAddr, dwMask );
			}
			else
			{
				printf( "Add route Dest %s, NextHop %s, Mask 0x%lX succeeded!\n", 
					achDestIpAddr, achNextHopIpAddr, dwMask );
			}
		}


#endif
	}
    
    return( bResult );
}
#endif

#ifdef _LINUX_
	BOOL routeAdd( TCHAR* destination, TCHAR* gateway ){return 0;}
	BOOL mRouteAdd( TCHAR* temp1, TCHAR* temp2, long temp3, int temp4, int temp5 ){return 0;}
	DllExport BOOL ReadRouteTable( LPCSTR lpszProfileName ){return 0;}
#endif
#endif

/*====================================================================
    ������      ��Trim
    ����        ��ȥ���ַ�����ǰ���������ո��Լ�TAB��
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPSTR lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void Trim( LPSTR lpszSrc )
{
    SDWORD    sdwRightIndex;
    SDWORD    sdwLeftIndex;

    sdwLeftIndex = 0;                           /* process left side first */
    while( ( lpszSrc[sdwLeftIndex] == ' ' || lpszSrc[sdwLeftIndex] == 0x09 ) 
        && lpszSrc[sdwLeftIndex] != '\0' )
    {
        sdwLeftIndex++;
    }
    sdwRightIndex = strlen( lpszSrc ) - 1;      /* then right side */
    while( ( lpszSrc[sdwRightIndex] == ' ' || lpszSrc[sdwRightIndex] == 0x09 ) 
        && sdwRightIndex > 0 )
    {
        sdwRightIndex--;
    }

    if( sdwRightIndex >= sdwLeftIndex )
    {
        strncpy( lpszSrc, lpszSrc + sdwLeftIndex, sdwRightIndex - sdwLeftIndex + 1 );
        lpszSrc[ sdwRightIndex - sdwLeftIndex + 1] = '\0';
    }
    else
    {
        lpszSrc[0] = '\0';
    }
}

/*====================================================================
    ������      ��StrUpper
    ����        ���ַ���ת���ɴ�д
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPSTR lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void StrUpper( LPSTR lpszSrc )
{
    WORD    wLoop;
    TCHAR    chDifference = 'A' - 'a';

    for( wLoop = 0; wLoop < strlen( lpszSrc ); wLoop++ )
    {
        if ( lpszSrc[wLoop] >= 'a' && lpszSrc[wLoop] <= 'z' )
        {
            lpszSrc[wLoop] += chDifference;
        }
    }
}

/*====================================================================
    ������      ��StrLower
    ����        ���ַ���ת����Сд
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����LPSTR lpszSrc, �����Լ������ַ���
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
DllExport void StrLower( LPSTR lpszSrc )
{
    WORD    wLoop;
    TCHAR    chDifference = 'A' - 'a';

    for( wLoop = 0; wLoop < strlen( lpszSrc ); wLoop++ )
    {
        if ( lpszSrc[wLoop] >= 'A' && lpszSrc[wLoop] <= 'Z' )
        {
            lpszSrc[wLoop] -= chDifference;
        }
    }
}

/*====================================================================
    ������      ��TableMemoryFree
    ����        ���ͷŵ���̬���루malloc���ı�ṹ�ڴ�
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����void **ppMem, ָ������ָ��
                  DWORD dwEntryNum, ָ�������С
    ����ֵ˵��  ��void
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/01/30    1.0         Li, Yi        ����
====================================================================*/
/*DllExport void TableMemoryFree( void **ppMem, DWORD dwEntryNum )
{
    DWORD    dwLoop;

    if( ppMem == NULL )
    {
        return;
    }

    for( dwLoop = 0; dwLoop < dwEntryNum; dwLoop++ )
    {
        if( ppMem[dwLoop] != NULL )
        {
            free( ppMem[dwLoop] );
        }
    }

    free( ppMem );
}*/

/*====================================================================
    ������      ��IsValidTime
    ����        ����֤�Ƿ�Ϸ�����
    �㷨ʵ��    ������ѡ�
    ����ȫ�ֱ�������
    �������˵����WORD wYear, ��
                  UCHAR byMonth, ��
                  UCHAR byDay, ��
                  UCHAR byHour, ʱ
                  UCHAR byMin, ��
                  UCHAR bySec, ��
    ����ֵ˵��  ���Ϸ�����TRUE�����𷵻�FALSE
----------------------------------------------------------------------
    �޸ļ�¼    ��
    ��  ��      �汾        �޸���        �޸�����
    02/03/12    1.0         Li, Yi        ����
====================================================================*/
DllExport BOOL IsValidTime( WORD wYear, UCHAR byMonth, UCHAR byDay, 
                           UCHAR byHour, UCHAR byMin, UCHAR bySec )
{
    UCHAR   byMaxDay;
    
    if( byMonth > 12 || byMonth < 1 || byHour >= 24 || byMin >= 60 || bySec >= 60 )
        return( FALSE );

    switch( byMonth )
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if( byDay < 1 || byDay > 31 )
            return( FALSE );
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if( byDay < 1 || byDay > 30 )
            return( FALSE );
        break;
    case 2:
        if( ( wYear % 100 != 0 && wYear % 4 == 0 ) || wYear % 400 == 0 )
            byMaxDay = 29;
        else
            byMaxDay = 28;

        if( byDay < 1 || byDay > byMaxDay )
            return( FALSE );
        break;
    default:
        return( FALSE );
        //break;
    }

    return( TRUE );
}


/*====================================================================
������      ��SetTableRegKeyString
����        ���������ļ��еı����ĳһ�ض���ѡ��д��һ���µ��ַ�����ֵ��
�㷨ʵ��    ������ѡ�
����ȫ�ֱ�������
�������˵����LPCSTR lpszProfileName,    �ļ�����������·����   
LPCSTR lpszSectionName,    Profile�еĶ���
BYTE   arossCoordinate,    Ҫ���õ��µ�ֵ��ˮƽ�����λ�á�
BYTE   verticalCoordinate, Ҫ���õ��µ�ֵ�ڴ�ֱ�����λ�á�
LPCSTR lpszValue, �µ��ִ�ֵ, NULL���ʾɾ������
����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
04/09/27    1.1         ���        ����
====================================================================*/
DllExport BOOL SetTableRegKeyString( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszLineName,
									BYTE   arossCoordinate, 
									BYTE   verticalCoordinate,
									LPCSTR  lpszValue
									)
{
	TCHAR achKeyName[MAX_KEYNAME_LEN + 1];
	BOOL  bResult = TRUE;
	int   dwLoop;
	
	TCHAR ptrBlank[8] = _T("     ");//Ϊ5���ո�
	TCHAR ptrOther[2550] = _T("  ");
	TCHAR ptrTokenAddBlank[50] = {0};
	
	FILE  *stream;
	TCHAR *pchToken;
	TCHAR lpzrwTable[2550] = {0};
	TCHAR chSeps[] = _T(" \t");/* space or tab as seperators */
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		return (FALSE);
	}
	
	sprintf(achKeyName, _T("%s%d"), lpszLineName, (int)arossCoordinate-1);
	
	bResult = GetRegKeyString2(stream, lpszSectionName, achKeyName, NULL, lpzrwTable, 2550);
	
	fclose(stream);//csp modify
	
	dwLoop = 1;
	pchToken = strtok(lpzrwTable, chSeps);
	if(pchToken == NULL)
	{
		printf(_T("Read configure %s failed line:%d\n!"), lpszSectionName, __LINE__);
		bResult = FALSE;
	}
	while(pchToken!=NULL)
	{
		if(pchToken!=NULL)
		{
			if(dwLoop==(verticalCoordinate))
			{
				strcpy(ptrTokenAddBlank,lpszValue);
			}
			else
			{
				strcpy(ptrTokenAddBlank,pchToken);
			}
			strcat(ptrTokenAddBlank,ptrBlank);
			strcat(ptrOther,ptrTokenAddBlank);
			memset(ptrTokenAddBlank,0,sizeof(ptrTokenAddBlank));//memset(ptrTokenAddBlank,0,sizeof(20));//why???//csp modify
			pchToken = strtok(NULL, chSeps);
			dwLoop++;
		}
	}
	
	bResult = SetRegKeyString(lpszProfileName, lpszSectionName, achKeyName, ptrOther);
	if(bResult==FALSE) return (FALSE);
	
	//fclose(stream);//csp modify
	
	return (TRUE);
}

/*====================================================================
������      ��GetTableRegKeyString
����        ���������ļ��еı����ĳһ�ض���ѡ��д��һ���µ��ַ�����ֵ��
�㷨ʵ��    ������ѡ�
����ȫ�ֱ�������
�������˵����LPCSTR lpszProfileName,    �ļ�����������·����   
LPCSTR lpszSectionName,    Profile�еĶ���
BYTE   arossCoordinate,    Ҫ���õ��µ�ֵ��ˮƽ�����λ�á�
BYTE   verticalCoordinate, Ҫ���õ��µ�ֵ�ڴ�ֱ�����λ�á�
LPCSTR lpszValue,���ص��ִ�ֵ, NULL���ʾɾ������
����ֵ˵��  ��TRUE/FALSE
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
04/09/27    1.1         ���        ����
====================================================================*/
char* GetTableRegKeyString( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									BYTE   arossCoordinate, 
									BYTE   verticalCoordinate,
									LPSTR  lpszValue
									)
{
	TCHAR   achKeyName[MAX_KEYNAME_LEN + 1];
	BOOL    bResult = TRUE;
	int     dwLoop;
	
	FILE    *stream;
	TCHAR   *pchToken;
	TCHAR   lpzrwTable[2550]={0};
	TCHAR   chSeps[] = _T(" \t");/* space or tab as seperators */
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		return (FALSE);
	}
	
#ifdef INI_OPT
	s32 nCurGet = 0, nCurReal = 0, nAction = 0;
	if(0 != strcmp("SECTIONS", lpszSectionName))
	{
		nAction = 1;
		nCurGet = GoSection(stream, lpszSectionName, FALSE);
	 	if(0 == nCurGet)
	 	{
	 		printf("GetTableRegKeyString:GoSection [%s] failed\n",lpszSectionName);
			
			/* move from the beginning */
			if(fseek(stream, 0L, SEEK_SET) != 0)
			{
				PUBPRT("HERE err");
				fclose(stream);
				return (FALSE);
			}
	 	}
		
		if((nCurReal = CalculateSection(stream, lpszSectionName, TRUE)) == -1)
		{
			PUBPRT("HERE err");
		    fclose(stream);
		    return(FALSE);
		}
		
		fseek(stream, nCurReal, SEEK_SET);
	}
#endif
	
	sprintf(achKeyName, _T("%s%d"), STR_ENTRY, arossCoordinate-1);
	
	bResult = GetRegKeyString2(stream, lpszSectionName, achKeyName, NULL, lpzrwTable, 2550);
	
	fclose(stream);//csp modify
	
	dwLoop = 1;
	
	pchToken = strtok(lpzrwTable, chSeps);
	
	if(pchToken == NULL)
	{
		printf(_T("Read configure %s failed line:%d\n!"), lpszSectionName, __LINE__);
		bResult = FALSE;
	}
	
	bResult = FALSE;//csp modify
	while(pchToken!=NULL)
	{
		if(pchToken!=NULL)
		{
			if(dwLoop==(verticalCoordinate))
			{
				memcpy(lpszValue, pchToken, 64);
				
				bResult = TRUE;//csp modify
				break;//csp modify//here is a bug
			}
			else
			{
				pchToken = strtok(NULL, chSeps);
				dwLoop++;
			}
		}
	}
	
	//fclose(stream);//csp modify
	
#ifdef INI_OPT
	if(1 == nAction)
	{
		if(nCurReal != nCurGet)
		{
			SetRegKeyInt(lpszProfileName, "SECTIONS", lpszSectionName, nCurReal);
		}
	}
#endif
	
	if(bResult==FALSE)
	{
		return (FALSE);
	}
	
    return (lpszValue);
}

/*====================================================================
������      ��SetTableRegKeyInt
����        ���������ļ��еı����ĳһ�ض���ѡ��д��һ���µ��ַ�����ֵ��
�㷨ʵ��    ������ѡ�
����ȫ�ֱ�������
�������˵����LPCSTR lpszProfileName,    �ļ�����������·����   
              LPCSTR lpszSectionName,    Profile�еĶ���
              BYTE   arossCoordinate,    Ҫ���õ��µ�ֵ��ˮƽ�����λ�á�
              BYTE   verticalCoordinate, Ҫ���õ��µ�ֵ�ڴ�ֱ�����λ�á�
              const SDWORD sdwValue      �µ�����ֵ, NULL���ʾɾ������
����ֵ˵��  : TRUE/FALSE
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
04/09/28    1.1         ���        ����
====================================================================*/
DllExport BOOL SetTableRegKeyInt( LPCSTR lpszProfileName, 
								  LPCSTR lpszSectionName, 
								  LPCSTR lpszLineName,
								  BYTE   arossCoordinate, 
								  BYTE   verticalCoordinate,
								  const SDWORD sdwValue 
								   )
{
	TCHAR achKeyValueInt[MAX_VALUE_LEN + 1];
	BOOL  bResult;
	
	sprintf((LPSTR)achKeyValueInt, _T("%ld"), sdwValue);
	bResult = SetTableRegKeyString(lpszProfileName, lpszSectionName, lpszLineName, arossCoordinate, verticalCoordinate, achKeyValueInt);
	return (bResult);
}

//yzw add
DllExport BOOL GetRegKeyU64( LPCSTR lpszProfileName, 
                   LPCSTR lpszSectionName, 
                   LPCSTR lpszKeyName, 
                   UINT64  sdwDefault, 
                   UINT64  *sdwReturnValue 
                 )
{
	TCHAR achKeyValue[MAX_VALUE_LEN + 1];
	BOOL  bResult;
	
	*sdwReturnValue = sdwDefault;
	
	bResult = GetRegKeyString(lpszProfileName, lpszSectionName, lpszKeyName, NULL, achKeyValue, sizeof(achKeyValue));
	
    if(bResult != FALSE)
    {
    	if((strlen(achKeyValue) > 2) && ((*achKeyValue == '0') && ((*(achKeyValue+1) == 'x') || (*(achKeyValue+1) == 'X'))))
		{
			sscanf(achKeyValue, "%llx", sdwReturnValue);
		}
		else
		{
			sscanf(achKeyValue, "%lld", sdwReturnValue);
		}
    }
    
    return (bResult);
}

DllExport BOOL SetRegKeyU64( LPCSTR lpszProfileName,
				  LPCSTR lpszSectionName, 
				  LPCSTR lpszKeyName, 
				  const UINT64 sdwValue   
				)
{
	TCHAR achKeyValue[MAX_VALUE_LEN + 1];
	BOOL  bResult;
	
	sprintf((LPSTR)achKeyValue, _T("0x%llx"), sdwValue);
	
	bResult = SetRegKeyString(lpszProfileName, lpszSectionName, lpszKeyName, achKeyValue);
	
	return (bResult);
}

#define INI_NEW//cw_test

#ifdef INI_NEW
#include <pthread.h>
static TCHAR lpszProfileNameLast[64] = {0};
static TCHAR lpszKeyNameLast[64] = {0} ;
static TCHAR lpszSectionNameLast[64] = {0};
static TCHAR lpzrwTableStatic[2550]= {0};
static pthread_mutex_t INI_MUTEX = PTHREAD_MUTEX_INITIALIZER;
#endif

DllExport BOOL GetStringFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									LPSTR  lpszValue, 
									DWORD  dwBufSize 
									)
{
	TCHAR achKeyName[MAX_KEYNAME_LEN + 1];
	BOOL  bResult = FALSE;
	int   dwLoop;
	int   len = 0;
	FILE  *stream = NULL;
	TCHAR *pchToken;
	TCHAR lpzrwTable[2550]={0};//cw_test 2550
	TCHAR chSeps[] = _T(" \t");/* space or tab as seperators */
	
#ifdef INI_OPT//cw_test
	//PUBPRT("HERE");
	s32 nCurGet = 0, nCurReal = 0, nAction = 0;
#endif
	
#ifdef INI_NEW
	u8 flag_closefile = 1;
	if(lpszSectionName == NULL)
	{
		printf("cw^^^^^hello lpszSectionName error\n");
		//csp modify
		//exit(0);
		return (FALSE);
	}	
	if(lpszKeyName == NULL)
	{
		printf("cw^^^^^hello lpszKeyName error\n");
		//csp modify
		//exit(0);
		return (FALSE);
	}
	
	pthread_mutex_lock(&INI_MUTEX);
	
	if(0 == strcmp(lpszProfileNameLast,lpszProfileName))
	{
		if(0 == strcmp(lpszSectionNameLast,lpszSectionName))
		{
			if(0 == strcmp(lpszKeyNameLast,lpszKeyName))
			{
				memset(lpzrwTable,0,sizeof(lpzrwTable));
				strcpy(lpzrwTable,lpzrwTableStatic);
				flag_closefile = 0;
				goto INI_SET;
			}
		}
	}
	#endif
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		
		//csp modify
		#ifdef INI_NEW
		pthread_mutex_unlock(&INI_MUTEX);
		#endif
		
		return (FALSE);
	}
	
#ifdef INI_OPT
	if(0 != strcmp("SECTIONS", lpszSectionName))
	{
		nAction = 1;
		nCurGet = GoSection(stream, lpszSectionName, FALSE);
	 	if(0 == nCurGet)
	 	{
	 		//csp modify
	 		//printf("GetStringFromTableWithKey:GoSection {%s} failed\n",lpszSectionName);
			
			/* move from the beginning */
			if(fseek( stream, 0L, SEEK_SET) != 0)
			{
				PUBPRT("HERE err");
				fclose(stream);
				
				//csp modify
				#ifdef INI_NEW
				pthread_mutex_unlock(&INI_MUTEX);
				#endif
				
				return (FALSE);
			}
	 	}
		//PUBPRT("HERE");
		if((nCurReal = CalculateSection(stream, lpszSectionName, TRUE)) == -1)
		{
			//PUBPRT("HERE err");
			fclose(stream);
			
			//csp modify
			#ifdef INI_NEW
			pthread_mutex_unlock(&INI_MUTEX);
			#endif
			
			return (FALSE);
		}
		//PUBPRT("HERE");
		
		fseek(stream, nCurReal, SEEK_SET);
	}
#endif
	
	sprintf(achKeyName, _T("%s"), lpszKeyName);
	bResult = GetRegKeyString2(stream, lpszSectionName, achKeyName, NULL, lpzrwTable, 2550);
	//if(strcmp(achKeyName, "NetworkPara") == 0)
	//{
	//	printf("achKeyName=%s,lpzrwTable=%s\n",achKeyName,lpzrwTable);
	//}
	
	//PUBPRT("HERE");
	
#ifdef INI_NEW
	memset(lpzrwTableStatic,0,sizeof(lpzrwTableStatic));
	strcpy(lpzrwTableStatic,lpzrwTable);
	
INI_SET:
#endif
	
	dwLoop = 1;
	
	if(lpzrwTable == NULL)
	{
		bResult = FALSE;
	}
	
	pchToken = strtok(lpzrwTable, chSeps);
	if(pchToken == NULL)
	{
		bResult = FALSE;
	}
	
	bResult = FALSE;
	while(pchToken!=NULL)
	{
		if(pchToken!=NULL)
		{
			//PUBPRT("HERE");
			len = min(dwBufSize - 1, strlen(pchToken));
			if(dwLoop==(verticalCoordinate))
			{
				//memcpy(lpszValue, pchToken, 64);
				strncpy(lpszValue, pchToken, len);
				lpszValue[len] = '\0';
				bResult = TRUE;
				break;
			}
			else
			{
				pchToken = strtok(NULL, chSeps);
				dwLoop++;
			}
		}
	}
	
#ifdef INI_NEW
	if(flag_closefile != 0)
	{
		fclose(stream);
	}
#else
	fclose(stream);
#endif
	
#ifdef INI_OPT
	//PUBPRT("HERE");
	if(1 == nAction)
	{
		if(nCurReal != nCurGet)
		{
			SetRegKeyInt(lpszProfileName, "SECTIONS", lpszSectionName, nCurReal);
		}
	}
	//PUBPRT("HERE");
#endif
	
#ifdef INI_NEW
	memset(lpszProfileNameLast,0,sizeof(lpszProfileNameLast));
	memset(lpszKeyNameLast,0,sizeof(lpszKeyNameLast));
	memset(lpszSectionNameLast,0,sizeof(lpszSectionNameLast));
	strcpy(lpszProfileNameLast,lpszProfileName);
	strcpy(lpszKeyNameLast,lpszKeyName);
	strcpy(lpszSectionNameLast,lpszSectionName);
	pthread_mutex_unlock(&INI_MUTEX);
#endif
	
	return bResult;
}									

DllExport BOOL GetValueFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									SDWORD  *sdwReturnValue 
									)
{
	TCHAR str[128+1] = {0};
	BOOL ret = GetStringFromTableWithKey(lpszProfileName, lpszSectionName, lpszKeyName, verticalCoordinate, str, sizeof(str)-1);
	if(ret)
	{
		if((strlen(str) > 2) && ((*str == '0') && ((*(str+1) == 'x') || (*(str+1) == 'X'))))
		{
			sscanf(str, "%lx", sdwReturnValue);//csp modify
		}
		else
		{
			*sdwReturnValue = atoi(str);
		}
	}
	return ret;
}

DllExport BOOL GetU64ValueFromTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									UINT64 *sdwReturnValue 
									)
{
	TCHAR str[128+1] = {0};
	BOOL ret = GetStringFromTableWithKey(lpszProfileName, lpszSectionName, lpszKeyName, verticalCoordinate, str, sizeof(str)-1);
	if(ret)
	{
		if((strlen(str) > 2) && ((*str == '0') && ((*(str+1) == 'x') || (*(str+1) == 'X'))))
		{
			sscanf(str, "%llx", sdwReturnValue);
		}
		else
		{
			sscanf(str, "%lld", sdwReturnValue);
		}
	}
	return ret;
}

#define INI_WRITE_NEW

#ifdef INI_WRITE_NEW
static TCHAR lpszKeyNameLast_read[64] = {0} ;
static TCHAR lpszSectionNameLast_read[64] = {0};
static TCHAR lpzrwTable_read_static[2550]= {0};
static pthread_mutex_t INI_MUTEX_READ = PTHREAD_MUTEX_INITIALIZER;
#endif

DllExport BOOL SetStringToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									LPSTR  lpszValue 
									)
{
	TCHAR achKeyName[MAX_KEYNAME_LEN + 1];
	BOOL  bResult = TRUE;
	int   dwLoop;
	
	TCHAR ptrBlank[8] = _T("\t");//Ϊ5���ո�
	TCHAR ptrOther[2550] = _T("  ");
	TCHAR ptrTokenAddBlank[50] = {0};
	
	//PUBPRT("Here");
	
	FILE  *stream = NULL;
	TCHAR *pchToken;
	TCHAR lpzrwTable[2550] = {0};
	TCHAR chSeps[] = _T(" \t");/* space or tab as seperators */
	char  flag = 0;
	
#ifdef INI_WRITE_NEW
	u8 flag_closefile = 1;//csp modify
	sprintf(achKeyName, _T("%s"), lpszKeyName);
	pthread_mutex_lock(&INI_MUTEX_READ);
	if(0 == strcmp(lpszSectionNameLast_read,lpszSectionName))
	{
		if(0 == strcmp(lpszKeyNameLast_read,lpszKeyName))
		{
			flag_closefile = 0;//csp modify
			goto INI_WRITE;
		}
	}
#endif
	
	stream = fopen(lpszProfileName, _T("rb"));
	if(stream == NULL)
	{
		printf(_T("Open file %s failed!\n"), lpszProfileName);
		
		//csp modify
		#ifdef INI_WRITE_NEW
		pthread_mutex_unlock(&INI_MUTEX_READ);
		#endif
		
		return (FALSE);
	}
	
	//PUBPRT("Here");
	
#ifndef INI_WRITE_NEW
	sprintf(achKeyName, _T("%s"), lpszKeyName);
#endif
	
	bResult = GetRegKeyString2(stream, lpszSectionName, achKeyName, NULL, lpzrwTable, 2550);
	//PUBPRT("Here");
	
#ifdef INI_WRITE_NEW
	memset(lpzrwTable_read_static,0,sizeof(lpzrwTable_read_static));
	strcpy(lpzrwTable_read_static,lpzrwTable);
	
INI_WRITE:
	strcpy(lpzrwTable,lpzrwTable_read_static);
#endif
	
	char pTourPathtmp[16]="TourPath";
	if(0 == strncasecmp(lpszKeyName,pTourPathtmp,strlen(pTourPathtmp)))
	{
		if(verticalCoordinate==1)
		{
			if(lpzrwTable!=NULL)
			{
				memset(lpzrwTable,0,sizeof(lpzrwTable));
			}
		}
	}
	
	dwLoop = 1;
	pchToken = strtok(lpzrwTable, chSeps);
	if(pchToken == NULL)
	{
		//printf( _T("Read configure %s failed\n!"), lpszSectionName );
		bResult = FALSE;
	}
/*
	printf("SetStringToTableWithKey: \n");
	int i;
	for(i=0; i<6; i++)
		printf("0x%x\n", lpszValue[i]);
*/	
	while(pchToken!=NULL)
	{
		if(pchToken!=NULL)
		{
			if(dwLoop==(verticalCoordinate))
			{
				strcpy(ptrTokenAddBlank,lpszValue);
				flag = 1;
			}
			else
			{
				strcpy(ptrTokenAddBlank,pchToken);
			}
			strcat(ptrTokenAddBlank,ptrBlank);
			strcat(ptrOther,ptrTokenAddBlank);
			memset(ptrTokenAddBlank,0,sizeof(ptrTokenAddBlank));//memset(ptrTokenAddBlank,0,sizeof(20));//why???//csp modify
			pchToken = strtok(NULL, chSeps);
			dwLoop++;
		}
	}
	
	if(flag == 0)
	{
		while(verticalCoordinate > dwLoop)
		{
			strcpy(ptrTokenAddBlank,"0");
			strcat(ptrTokenAddBlank,ptrBlank);
			strcat(ptrOther,ptrTokenAddBlank);
			memset(ptrTokenAddBlank,0,sizeof(ptrTokenAddBlank));//memset(ptrTokenAddBlank,0,sizeof(20));//why???//csp modify
			dwLoop++;
		}
		strcpy(ptrTokenAddBlank,lpszValue);
		strcat(ptrOther,ptrTokenAddBlank);
	}
	
	//csp modify
	if(flag_closefile != 0)//csp modify
	{
		fclose(stream);//cw_test
	}
	
#ifdef INI_WRITE_NEW
	if(0 == strcmp(lpzrwTable_read_static,ptrOther))
	{
		pthread_mutex_unlock(&INI_MUTEX_READ);
		return 1;
	}
	else
	{
		strcpy(lpzrwTable_read_static,ptrOther);
	}
#endif
	
	//PUBPRT("Here");
	bResult = SetRegKeyString(lpszProfileName, lpszSectionName, achKeyName, ptrOther);
	//PUBPRT("Here");
	
	//csp modify
	//if(flag_closefile != 0)//csp modify
	//{
	//	fclose(stream);//cw_test
	//}
	
    if(bResult == FALSE)
	{
		//csp modify
		#ifdef INI_WRITE_NEW
		pthread_mutex_unlock(&INI_MUTEX_READ);
		#endif
		
		return (FALSE);  
    }
	
#ifdef INI_WRITE_NEW
	memset(lpszKeyNameLast_read,0,sizeof(lpszKeyNameLast_read));
	memset(lpszSectionNameLast_read,0,sizeof(lpszSectionNameLast_read));
	strcpy(lpszKeyNameLast_read,lpszKeyName);
	strcpy(lpszSectionNameLast_read,lpszSectionName);
	pthread_mutex_unlock(&INI_MUTEX_READ);
#endif
	
	return (TRUE);
}									

DllExport BOOL SetValueToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									SDWORD sdwReturnValue 
									)
{
	TCHAR achKeyValueInt[MAX_VALUE_LEN + 1];
	BOOL  bResult;
	
	sprintf((LPSTR)achKeyValueInt, _T("%ld"), sdwReturnValue);
	bResult = SetStringToTableWithKey(lpszProfileName, lpszSectionName, lpszKeyName, verticalCoordinate,achKeyValueInt);
	return (bResult);
}

DllExport BOOL SetU64ValueToTableWithKey( LPCSTR lpszProfileName, 
									LPCSTR lpszSectionName, 
									LPCSTR lpszKeyName, 
									WORD   verticalCoordinate, 
									UINT64 sdwReturnValue 
									)
{
	TCHAR achKeyValueInt[MAX_VALUE_LEN + 1];
	BOOL  bResult;
	
	sprintf((LPSTR)achKeyValueInt, _T("0x%llx"), sdwReturnValue);
	bResult = SetStringToTableWithKey(lpszProfileName, lpszSectionName, lpszKeyName, verticalCoordinate,achKeyValueInt);
	return (bResult);
}
//end

