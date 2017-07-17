//
//  "$Id: DevFrontboard.h 247 2008-12-12 06:46:15Z liwj $"
//
//  Copyright (c)2008-2010, RealVision Technology CO., LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __DEV_FRONTBOARD_H__
#define __DEV_FRONTBOARD_H__

#include "System/Signals.h"
#include "APIs/Frontboard.h"
#include "MultiTask/Thread.h"
#include "MultiTask/Timer.h"
#include "System/Object.h"

enum fb_keys_t {
	KEY_0 = 0x01,
	KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,   //0x06
	KEY_6,	KEY_7,	KEY_8,	KEY_9,	KEY_10, //0x0b
	KEY_11, KEY_12, KEY_13, KEY_14, KEY_15, KEY_16,  //0x11
	KEY_CHNPLUS,	KEY_CHNMINUS,				 	//0x13
	KEY_UP,		KEY_DOWN,	KEY_LEFT,	KEY_RIGHT,  //0x17
	KEY_PGUP,	KEY_PGDN, 	KEY_RET,	KEY_ESC,     //0x1b

	KEY_PLAY,	KEY_STOP,	KEY_FAST,	KEY_SLOW,  //0x1f
	KEY_PREV,	KEY_NEXT,	KEY_SPLIT,	KEY_ADDR,  //0x23
	KEY_REC,	KEY_FUNC,	KEY_ALARM,	KEY_SHUT,	KEY_SEARCH,//0x28

	KEY_LOCK,	KEY_10PLUS,	KEY_SHIFT,	KEY_BACK,  //0x2c
	KEY_STEPX,	KEY_MENU,	KEY_INFO,	KEY_SPLIT1,	//0x30
	KEY_SPLIT4,	KEY_SPLIT8,	KEY_SPLIT9,	KEY_SPLIT16,//0x34
	KEY_STEPF,	KEY_STEPB,  KEY_PAUSE,   //0x37

	//add by xie 
	KEY_MAIN,            //�Ҽ��˵�	
	//end

	KEY_MUTE,
	KEY_VOIP,    //0x3a

	KEY_AUTOTUR = 0xa0, /*!< �Զ���Ѳ */

	//������Aϵ�е�ң�ؼ�ֵ
	//��������
	KEY_EMERGENCY_CALL = 0xa8,
	
	//��ȫģʽ
	KEY_SAFE_MODE = 0xa9,
	
	//����,
	KEY_ARMING = 0xaa,

	//����
	KEY_DISARMING = 0xab,


	KEY_PTZ = 0xb0, /*!< ��̨���Ʋ˵� */
	KEY_TELE, KEY_WIDE, /*!< �䱶 */
	KEY_IRIS_CLOSE, KEY_IRIS_OPEN, /*!< ��Ȧ */
	KEY_FOCUS_NEAR,	KEY_FOCUS_FAR, /*!< �۽� */
	KEY_BRUSH, /*!< ��ˢ */
	KEY_LIGHT, /*!< �ƹ� */
	KEY_AUTOPAN, /*!< ��ɨ*/
	KEY_SCAN, /*!< ���Ѳ�� */
	KEY_PATTERN, /*!< ģʽ */
	KEY_SPRESET, /*!< ����Ԥ�õ� */
	KEY_GPRESET, /*!< ת��Ԥ�õ� */
	KEY_DPRESET, /*!< ɾ��Ԥ�õ� */

	//����˵�(0x30 - 0x32)
	KEY_MACHINE_MENU = 0xc0, KEY_MACHINE_PREV, KEY_MACHINE_NEXT,

	KEY_BACKUP = 0xdf,

	// �ָ���������
	KEY_DEFAULT_CFG = 0xee,
	KEY_ALARMIN = 0xfe,
};

enum fb_cmd_t
{
	FB_KEEPALIVE = 0,
	FB_REBOOT,
	FB_SHUTDOWN,
	FB_LED_READY = 0x10,
	FB_LED_ALARM = 0x11,
	FB_LED_RECORD = 0x12,
	FB_LED_HDD = 0x13,
	FB_LED_NETWORK = 0x14,
	FB_LED_STATUS = 0x15,
	FB_LED_CHANNEL=0x16,
	FB_LED_END,
	LED_REMOTE = 0x50,
};

/*��ֵ��־
+------+----+----+------+
|  7-4 | 3  |2--1|  0   |
|------|----|----|------|
| ���� |ң��|�ٶ�|�����|
+------+----+----+------+
*/
enum fb_flags_t {//��Ϣ�����е�һЩ��־
	FB_FLAG_NONE = 0x00,
	FB_FLAG_SPEC = 0x01,
	FB_FLAG_SPEED = 0x06,
	FB_FLAG_SPEED1 = 0x00,
	FB_FLAG_SPEED2 = 0x02,
	FB_FLAG_SPEED3 = 0x04,
	FB_FLAG_SPEED4 = 0x06,
	FB_FLAG_REMOTE = 0x08,
};

//ң���������ֵ
#define RKEY_START	0x80

typedef TSignal3<uint, uint, uint>::SigProc SIG_DEV_FB_INPUT;

class CDevFrontboard : public CThread
{
public:
	CDevFrontboard();
	virtual ~CDevFrontboard();
	
private:
	//VD_BOOL Translate(uchar * pKeys);//csp modify
	
public:
	void LigtenLed(uint iLed, VD_BOOL bOpen, uint iChn = 0);
	void LigtenLedAll(VD_BOOL bOpen);
	void Shutdown();
	void Reboot();
	void OnRemoteKeyUp(uint param);
	VD_BOOL AttachInput(CObject * pObj, SIG_DEV_FB_INPUT pProc);
	VD_BOOL DetachInput(CObject * pObj, SIG_DEV_FB_INPUT pProc);
	virtual void ThreadProc();
	static CDevFrontboard* instance(void); 
	//void RunThread();//csp modify 20121222
	
	void OnStartAlive();
	void OnStopAlive();
	void KeepAlive();
	void SendSignal(int msg,int wpa,int lpa);
	
protected:
	
private:
	TSignal3<uint, uint, uint> m_sigInput;
	uint	m_dwLedState;//ָʾ��״̬
	CTimer	m_RemoteKeyTimer;
	CMutex	m_Mutex;
	static CDevFrontboard* _instance;
};

#endif// __DEV_FRONTBOARD_H__

