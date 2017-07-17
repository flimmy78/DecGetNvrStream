#ifndef _PAGEPTZTYPES_H_
#define _PAGEPTZTYPES_H_

#define TOURPATH_MAX	32
#define TOURPIONT_MAX	128
#define PTZMOVESPEED_MAX	10

//SBizTourPath
typedef struct
{
    uchar nPresetPos; //Ԥ�õ��
    unsigned short nDwellTime; //����ʱ��
    uchar nSpeed; //�ٶȹ�10��
} SGuiTourPoint;

typedef struct
{
    uchar nPathNo;
    SGuiTourPoint sTourPoint[TOURPIONT_MAX];
} SGuiTourPath;

//SBizPtzPara
typedef struct
{
	uchar nIsPresetSet[TOURPIONT_MAX]; //预置点位置最多128个//0表示不存在 ；1表示存在
	SGuiTourPath sTourPath[TOURPATH_MAX];
} SGuiPtzAdvancedPara;

typedef struct
{
	int nPresetCount[TOURPIONT_MAX];
	int nTourPathCount[TOURPATH_MAX];
}sGuiPtzAdvanPara;

typedef struct
{
	u8 bEnable;
	u8 nAddress;
	u8 nBaudRate;
	u8 nProtocol;
} STabParaUart;

#endif

