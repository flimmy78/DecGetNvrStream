#ifndef _CAMERA_H_
#define _CAMERA_H_

//#include "iflytype.h"
#include "common_basetypes.h"

#define _hikvision_
#define _YT_CONTROL_OPT_

//����ͷ������
#define CAM_TYPE_PELCO_D				0
#define CAM_TYPE_PELCO_P				1
#define CAM_TYPE_B01					2
#define CAM_TYPE_SAMSUNG				3

//pw 2010/7/23
#define CAM_TYPE_YAAN					4
#define CAM_TYPE_HY0					5

#define CAM_TYPE_jabsco_d				6
#define CAM_TYPE_PELCOD_2				7

#define CAM_TYPE_WV_CS850				8
#define CAM_TYPE_WV_CS650				9


//����ͷ���Ʒ�ʽ
#define CAM_CTRLWAY_GENERAL				0//����
#define CAM_CTRLWAY_REVERSE				1//����

//����ͷ�Ŀ�������
#define CAM_COMMAND_STOPALL				0//ֹͣ

#define CAM_COMMAND_MOVEUP				1//�����ƶ�
#define CAM_COMMAND_MOVEDOWN			2//�����ƶ�
#define CAM_COMMAND_MOVELEFT			3//�����ƶ�
#define CAM_COMMAND_MOVERIGHT			4//�����ƶ�

#define CAM_COMMAND_ZOOMTELE			5//��������ͷ
#define CAM_COMMAND_ZOOMWIDE			6//��Զ����ͷ

#define CAM_COMMAND_FOCUSFAR			7//�������Զ
#define CAM_COMMAND_FOCUSNEAR			8//���������

#define CAM_COMMAND_IRISOPEN			9//��Ȧ+
#define CAM_COMMAND_IRISCLOSE			10//��Ȧ-

#define CAM_COMMAND_SETPRESET			11//��������ͷԤ�õ�
#define CAM_COMMAND_SHOTPRESET			12//��������ͷԤ�õ�
#define CAM_COMMAND_CLRPRESET			13//ɾ������ͷԤ�õ�

#define CAM_COMMAND_STARTPATTERN		14//��¼�켣��ʼ
#define CAM_COMMAND_STOPPATTERN			15//��¼�켣����
#define CAM_COMMAND_RUNPATTERN			16//���й켣

#define CAM_COMMAND_STARTCRUISE			17//�����Զ�Ѳ��
#define CAM_COMMAND_STOPCRUISE			18//ֹͣ�Զ�Ѳ��
#define CAM_COMMAND_INSERTCRUISEPOS		19//���Ѳ����
#define CAM_COMMAND_DELETECRUISEPOS		20//ɾ��Ѳ����
#define CAM_COMMAND_CLEARCRUISE			21//���Ѳ��·��
#define CAM_COMMAND_SHOWCRUISE			22//��ʾѲ��·��

#define CAM_COMMAND_LIGHTOPEN			23//�ƹ����ʹ��
#define CAM_COMMAND_LIGHTCLOSE			24//�ƹ����ͣ��			

#define CAM_COMMAND_WIPEROPEN			25//��ˢ����ʹ��
#define CAM_COMMAND_WIPERCLOSE			26//��ˢ����ͣ��	

#define CAM_COMMAND_SETAUX				27//�򿪸�������
#define CAM_COMMAND_CLRAUX				28//�����������

#define CAM_COMMAND_ON					29//������ͷ
#define CAM_COMMAND_0FF					30//�ر�����ͷ
#define CAM_COMMAND_REMOTERESET			31//Զ�̸�λ
#define CAM_COMMAND_MOVEHOME			32//�ع�
#define CAM_COMMAND_FLIP				33//��ת180��

#define CAM_COMMAND_AUTOFOCUS			34//�Զ��۽�
#define CAM_COMMAND_MANUALFOCUS			35//�ֶ��۽�
#define CAM_COMMAND_AUTOIRIS			36//�Զ����ڹ�Ȧ
#define CAM_COMMAND_MANUALIRIS			37//�ֶ����ڹ�Ȧ
#define CAM_COMMAND_DIGITALZOOMOPEN		38//���ֱ䱶��
#define CAM_COMMAND_DIGITALZOOMCLOSE	39//���ֱ䱶��
#define CAM_COMMAND_WHITEBALANCE		40//��ƽ��ģʽ
#define CAM_COMMAND_RAYREDEEMOPEN		41//����ģʽ��
#define CAM_COMMAND_RAYREDEEMCLOSE		42//����ģʽ��

#define CAM_COMMAND_ZOOMSPEED			43//���ñ佹�ٶ�
#define CAM_COMMAND_FOCUSSPEED			44//���þ۽��ٶ�

#define CAM_COMMAND_STARTLINESCAN		45//��ʼ��ɨ
#define CAM_COMMAND_STOPLINESCAN		46//ֹͣ��ɨ
#define CAM_COMMAND_SETLINESCAN			47//������ɨ

#define CAM_COMMAND_AUTOSCAN			48//�Զ�ɨ��
//��������
#define CAM_SUCCESS						0//�ɹ�
#define CAM_ERROR_PARAM					1//��������
#define CAM_ERROR_TYPE					2//��֧�ֵ�����ͷ����
#define CAM_ERROR_PROTOCOL_TYPE			3//��֧�ֵ�����ͷЭ������
#define CAM_ERROR_PROTOCOL_PARAM		4//��֧�ֵ�����ͷЭ�����
#define CAM_ERROR_NOACK					5//����ͷû�л�Ӧ��Ϣ
#define CAM_ERROR_NORESPONE				6//����ͷû����Ӧ
#define CAM_ERROR_UNKNOWN				7//δ֪����

//��ƽ��ģʽ
#define WHITE_BALANCE_MANUAL			0//��ƽ���ֶ�ģʽ
#define WHITE_BALANCE_AUTO				1//��ƽ���Զ�ģʽ
#define WHITE_BALANCE_INSIDE			2//��ƽ������ģʽ
#define WHITE_BALANCE_OUTSIDE			3//��ƽ������ģʽ
#define WHITE_BALANCE_LOCK				4//��ƽ������ģʽ
#define WHITE_BALANCE_ATW				5//��ƽ��ATWģʽ

//���������Χ
#define MAX_PAN_SPEED					15//pan speed 0-MAX_PAN_SPEED
#define MAX_TILT_SPEED					15//tilt speed 0-MAX_TILT_SPEED
#define MAX_ZOOM_SPEED					15//zoom speed 0-MAX_ZOOM_SPEED
#define MAX_FOCUS_SPEED					15//focus speed 0-MAX_FOCUS_SPEED
#define MAX_PRESET_NUM					128//preset limit 0-MAX_PRESET_NUM
#define MAX_CRUISE_PATH_NUM				16//ÿ����̨�����MAX_CRUISE_PATH_NUM��Ѳ��·��
#define MAX_CRUISE_POS_NUM				16//ÿ��Ѳ��·�������MAX_CRUISE_POS_NUM��Ѳ����
#define MAX_CRUISE_SPEED				9//���Ѳ���ٶ�
#define MAX_DWELL_TIME					255//Ѳ�������פ��ʱ��

#define MAX_PROTOCOL_LEN				256//Э�����󳤶�
#define MAX_PARAM_NUM					72//����������

#define STOP_TYPE_POS					0
#define PAN_SPEED_POS					0
#define TILT_SPEED_POS					1
#define ZOOM_SPEED_POS					1
#define FOCUS_SPEED_POS					1
#define AUX_INDEX_POS					1
#define LIGHT_POS						1
#define WIPER_POS						1
#define WHITE_BALANCE_POS				1

#define CRUISE_PATH_POS					0

#define CRUISE_INDEX_POS				0
#define PRESET_INDEX_POS				1
#define DWELL_TIME_POS					2
#define CRUISE_SPEED_POS				3

#define LINE_SCAN_START_POS				0
#define LINE_SCAN_END_POS				1
#define PATTERN_POS						0
#define SENDER_ADDR_POS					(MAX_PARAM_NUM-1)

//Note:Ѳ��·���š�Ԥ�õ㡢����ͷ��ַ����1��ʼ

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	u8	m_byType;				//����ͷ������
	u32 m_dwId;					//����ͷ�ĵ�ַ
	u8	m_byCmdId;				//��������
    u8	m_abyParam[MAX_PARAM_NUM];//����
	u8	m_byCtrlWay;			//����ͷ��������Ʒ�ʽ
}TCamInfo;
typedef struct
{
	u32 m_dwLen;					//Э���ʵ�ʳ���
	u8	m_abyData[MAX_PROTOCOL_LEN];//Э������
	u32 m_dwInterval;
}TCamData;

u16 GetCamProtocol(TCamInfo *ptCamInfo,TCamData *ptCamData);

#ifdef __cplusplus
}
#endif

#endif
