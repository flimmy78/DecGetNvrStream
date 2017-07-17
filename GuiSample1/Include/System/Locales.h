

#ifndef __CLOCALES__
#define __CLOCALES__


#include <map>
#include <list>
#include "Language.h"
#include "System/File.h"
#include "Object.h"
#include "APIs/DVRDEF.H"
#include "config-x.h"

//#define UCS_FONT_MAGIC "RealVision ucs font"
#define UCS_FONT_MAGIC "dahua ucs font"

struct UCS_FONT_HEADER
{
	char  magic[16];	//��־
	uint size;		//�ֿ��ܴ�С
	uint blocks;	//����
};

struct UCS_FONT_BLOCK
{
	ushort start;		//������ʼֵ
	ushort end;		//�������ֵ
	ushort width;		//������
	ushort height;	//����߶�
	uint roffs;	//�����������ƫ��
	uint xoffs;	//������չ����ƫ��
};

enum FONTSIZE
{
	FONTSIZE_SMALL,
	FONTSIZE_NORMAL,
	FONTSIZE_ALL
};

enum FONT_STYLE
{
	FS_NORMAL = 0x0000,		///< ��������
	FS_BOLD = 0x0001,		///< ������
	FS_SMALL = 0x0002,
	FS_ITALIC	= 0x0004,	///< б����
	FS_OUTLINE	= 0x0008,	///< ����(����)Ч��
	FS_SCALING	= 0x0010,	///< �Բ����ʵĵ��������������
};

typedef std::map<VD_PCSTR, VD_PCSTR, strless<VD_PCSTR> > MAPSTRING;
class CLocales:public CObject{
public:
	PATTERN_SINGLETON_DECLARE(CLocales);
private:
	CLocales();
	~CLocales();
public:
	//װ��һ���ַ�������, �������ΪUCS-2����, ����ֵΪ������
	uchar GetOneFontFromFile(ushort code, uchar *p);
	void SetLanguage(int index);
	ushort GetCharCode(VD_PCSTR pch, int *pn);
	VD_BOOL GetFontSize(VD_SIZE *pSize, FONTSIZE fontsize = FONTSIZE_NORMAL);
	uchar GetCharRaster(ushort code, uchar* p = NULL, FONTSIZE fontsize = FONTSIZE_NORMAL);
	//���ַ����ҳ��ú���
	int FindCommonChar(ushort code);
	int GetTextExtent(VD_PCSTR str, int len=1024, FONT_STYLE fs = FS_NORMAL);

	MAPSTRING m_mapStrings;
private:
	int m_iLanguage;
	UCS_FONT_HEADER m_UFH;
	UCS_FONT_BLOCK *m_pUFB;
	VD_BOOL m_bFontValid;
	CFile m_FileFont;
//#ifdef ENC_SHOW_SMALL_FONT
	VD_BOOL m_bFontValidSmall;
	CFile m_FileFontSmall;
	UCS_FONT_BLOCK *m_pUFBSmall;
	UCS_FONT_HEADER m_UFHSmall;
	int m_nFontBytesSmall;
	VD_SIZE m_sizeFontSmall;
    uchar* m_pASCIIFontSmall;
//#endif

	CFile m_FileStrings;
//	char* m_pbuf;
	VD_SIZE m_sizeFont;
	int m_nFontBytes;
	int m_nCommonChars;			//�����ָ���
	ushort* m_pCommonChars;		//����������
	uchar* m_pCommonFont;		//����������
	uchar* m_pASCIIFont;			//ASCII�ַ�����
	
#if 1//csp modify 20131208
private:
	int ReloadString(char *str, int debug = 0);
	int ReverseString(char *str, int left, int right, int debug = 0);
	int Utf8ToUnicode(char* pch, ushort* strUni, int nMaxLen);
	void ReorderUniStr(ushort* displaystr, int len);
	int IsArabic(ushort code);
	int IsHebrew(ushort code);
#endif
};

VD_PCSTR LOADSTR(VD_PCSTR key);
VD_PCSTR GetParsedString(VD_PCSTR key);
#define g_Locales (*CLocales::instance())

#endif


