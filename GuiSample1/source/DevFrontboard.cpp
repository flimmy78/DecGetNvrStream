#include "Devices/DevFrontboard.h"
#include "GUI/Pages/BizData.h"
//#include "Functions/Alarm.h"
//#include "Console/Console.h"

#if 0//csp modify
static uchar s_keyThreeIndex[] = {
	0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
};

static uchar s_keyTwoIndex[] = {
	0x2a, 0x2b, 0x2c, 0x24, 0x25, 0x29, 0x2e	
};

static uchar PanelKeys[][9] = {
	// 0xfe           0xfd          0xfb        0xf7         0xef        0xdf            0xbf       0x7f
	{KEY_SHUT,       KEY_ESC,     KEY_LEFT,    KEY_0,       KEY_UP,     KEY_RET,      KEY_DOWN,   KEY_RIGHT }, // 0x2a --0xf7δ����, �����ø�ֵ�滻
	{KEY_0 ,		 KEY_0, 	  KEY_0,	   KEY_PREV,	KEY_0,	    KEY_0,KEY_0,    KEY_0,}, 		// 0x2b -- 0xfdӦΪMUTE��ʱֻ��0xf7��0xdf��Ч
	//{KEY_ESC,		 KEY_0, 	  KEY_REC,	   KEY_PREV,	KEY_SLOW,	KEY_PAUSE,	   KEY_FAST,    KEY_NEXT,}, // 0x2c --0xfeΪESC2��ʱ��ESC����, 0xfdӦΪENTER2
	{KEY_ESC,		 KEY_0, 	  KEY_REC,	   KEY_PREV,	KEY_SLOW,	KEY_PLAY,	   KEY_FAST,    KEY_NEXT,},
	{KEY_1, 		 KEY_2, 	  KEY_3,	   KEY_4,		KEY_5,		 KEY_6, 	   KEY_7,	   KEY_8,},		// 0x24 -- ��ʱֻ��0xbf��0x7f��Ч
	{KEY_0, 		 KEY_0, 	  KEY_0,	   KEY_0,		KEY_0,		 KEY_0, 	   KEY_0,	   KEY_0,},		// 0x25 -- ֻ������0xfb������δ����
	{KEY_9, 		 KEY_10, 	  KEY_11,	   KEY_12,		KEY_13,		 KEY_14, 	   KEY_15,	   KEY_16,},		// 0x29 -- ȫ����Ч
	//{KEY_10PLUS, 	 KEY_10PLUS,  KEY_0,	   KEY_0,		KEY_0,		 KEY_0, 	   KEY_0,	   KEY_0,},		// 0x2e -- 0xfbΪ���ˣ�����KEY_0δ����
	{KEY_10PLUS, 	 KEY_10PLUS,  KEY_BACK,	   KEY_MAIN,		KEY_SHIFT,		 KEY_FUNC, 	   KEY_SPLIT,	   KEY_0,},// 0x2e -- 0xfe,0xfd,0x7fδ����
};
/* delete by xie
static uchar RemoteKeys[] = {	//ң�ؼ���ֵ��RKEY_START��ʼ
	KEY_10PLUS, KEY_STOP, KEY_PLAY, KEY_NEXT, KEY_SPLIT, KEY_3, KEY_2, KEY_1, KEY_0, KEY_6, KEY_5, KEY_4, 
	KEY_SLOW, KEY_9, KEY_8, KEY_7,	KEY_PREV, KEY_FUNC, KEY_0, KEY_REC, KEY_SHIFT, KEY_UP, 
	KEY_ESC, KEY_ADDR,	KEY_BACK, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_FAST, KEY_PGUP, KEY_RET, KEY_PGDN,
};
*/
//add by xie 
static uchar RemoteKeys[] = {	//ң�ؼ���ֵ��RKEY_START��ʼ
	KEY_10PLUS, KEY_FAST, KEY_SLOW, KEY_PLAY, KEY_SPLIT, KEY_3, KEY_2, KEY_1, KEY_0, KEY_6, KEY_5, KEY_4, 
	KEY_BACK, KEY_9, KEY_8, KEY_7,	KEY_REC, KEY_FUNC, KEY_0, KEY_SHIFT, KEY_SHIFT, KEY_UP, 
	KEY_ESC, KEY_ADDR,	KEY_BACK, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_PREV, KEY_NEXT, KEY_RET, KEY_MAIN,
};
//end
#endif

void CDevFrontboard::ThreadProc()
{
	char nDvrIdStr[10] = {0};
	int nDvrId = 0;
	int nLastDvrId = -1;
	uchar keys[8], *p;
	uint state = 0;
	uchar flag_wait_up = 0;
	uchar last_key = 0;
	int ret;
	
	p = keys;
	
	uchar nLastKey = 0;
	uchar nCount = 0;
	uchar nChnNum = GetVideoMainNum();
	uchar nSystemLocked = 0;
	
	printf("GUI CDevFrontboard::ThreadProc pthread going...\n");
	
	GetDevId(nDvrIdStr, sizeof(nDvrIdStr));
	FrontBoardSetDvrId((u32)atoi(nDvrIdStr));
	
	while(m_bLoop)
	{
		while(!FrontBoardReqEnable()) sleep(1);
		
		ret = FrontboardRead(p, 1);
		if(ret != 0)
		{
			if(nLastKey)//cw_panel ???bug
			{
				nCount++;
				if(nCount >= 8)
				{
					m_sigInput(XM_KEYDOWNDELAY, nLastKey, FB_FLAG_NONE);
					nLastKey = 0;				
				}
			}
			SystemSleep(100);
			continue;
		}
		
		nSystemLocked = 0;
		GetSystemLockStatus(&nSystemLocked);
		if(nSystemLocked)
		{
			continue;
		}
		
		if(*p)
		{
			printf("cw^^^,key  = %d\n",*p);//cw_panel
			if((last_key) && (*p != last_key))
			{
				m_sigInput(XM_KEYUP, last_key, FB_FLAG_NONE);
				SystemSleep(100);
			}
			
			m_sigInput(XM_KEYDOWN, keys[0], FB_FLAG_NONE);
			
			flag_wait_up = 1;
			last_key = keys[0];
			
			if((nChnNum >= 10) && (last_key >= KEY_0) && (last_key < KEY_10))
			{
				if((nLastKey == 0) && (last_key > KEY_0) && ((last_key - KEY_1) < (nChnNum / 10)))
				{
					nLastKey = last_key;
					nCount = 0;
				}
				else
				{
					nLastKey = 0;
				}
			}
		}
		else
		{
			if(flag_wait_up)
			{
				m_sigInput(XM_KEYUP, last_key, FB_FLAG_NONE);
				flag_wait_up = 0;
				last_key = 0;
			}
			else
			{
				m_sigInput(XM_KEEPALIVE, last_key, FB_FLAG_NONE);
			}
		}		
		
		SystemSleep(100);
	}
}

#if 0//csp modify 20121222
void CDevFrontboard::RunThread()
{
	m_bLoop = TRUE;
	ThreadProc();
}
#endif

/*
void CDevFrontboard::ThreadProc()
{
	uchar keys[8], *p;
	uint state = 0;
	int bytes_read;
	int ret;
	int i;

	while(m_bLoop) 
	{
		//trace("FrontboardRead..............\n");
		//����4���ֽ�
		p = keys;
		bytes_read = 0;
		while(bytes_read < 4)
		{
			ret = FrontboardRead(p, 4);
			//������ʱ�Ĵ���
			if(ret <= 0)
			{
				//trace("CDevFrontboard::ThreadProc FrontboardRead failed\n");
				p = keys;//��λ
				bytes_read = 0;
				SystemSleep(100);
				continue;
			}

			//��һ�ζ�, �ҵ�����ͷ�ֽ�0xaa
			if(bytes_read == 0)
			{
				for(i = 0; i < ret; i++)
				{
					if(keys[i] == 0xaa || keys[i] == 0xff)
					{
						break;
					}
				}
				if(i)
				{
					ret -= i;
					if(ret)
					{
						memcpy(keys + i, keys, ret);
					}
				}
			}

			//�޸ļ���
			p += ret;
			bytes_read += ret;
		}

		//��ֵת���뷢���ź�
		if(Translate(keys))
		{
			if(keys[3])
			{
				state = XM_KEYDOWN;
			}
			else
			{                                                                        
				state = XM_KEYUP;
			}
		}
		else //��������
		{
			state = XM_KEEPALIVE;
		}
		
		m_sigInput(state, keys[2], keys[1]);
	}
}
*/

CDevFrontboard* CDevFrontboard::_instance = NULL;

CDevFrontboard* CDevFrontboard::instance(void)
{
	if(NULL == _instance)
	{
		_instance = new CDevFrontboard();
	}
	return _instance;
}

CDevFrontboard::CDevFrontboard() : CThread("DevFrontboard", TP_FRONTBOARD), m_sigInput(4), m_RemoteKeyTimer("RemoteKey")
{
	//printf("CDevFrontboard::CDevFrontboard()>>>>\n");
	
	m_dwLedState = 0;
	
	FrontboardCreate();
	CreateThread();
}

CDevFrontboard::~CDevFrontboard()
{
	//DestroyThread();
	FrontboardDestory();
}

void CDevFrontboard::OnStartAlive()
{
#if 0//csp modify
	//Ϊ�˴����Ѿ�����ȥ��-U ��Ʒ������������(ǰ�����������޸�),��LB,GBE��δ����
	CGuard guard(m_Mutex);
	
	uchar buf[4];
	buf[0] = 0xaa;
	buf[1] = FB_KEEPALIVE;
	buf[2] = 0x00;
	buf[3] = 0x01;
	FrontboardWrite(buf, 4);
#endif
}

//�ػ���ʱ����Ҫֹͣ���������������᲻������������ϵ�ǰ�������µ�ǰ�������Ѿ������������⡣
void CDevFrontboard::OnStopAlive()
{
#if 0//csp modify
	CGuard guard(m_Mutex);
	
	uchar buf[4];
	buf[0] = 0xaa;
	buf[1] = FB_KEEPALIVE;
	buf[2] = 0x00;
	buf[3] = 0x00;
	FrontboardWrite(buf, 4);
#endif
}

//��������
void CDevFrontboard::KeepAlive()
{
#if 0//csp modify
	CGuard guard(m_Mutex);
	
	uchar buf[4];
	buf[0] = 0xaa;
	buf[1] = FB_KEEPALIVE;
	buf[2] = 0x00;
	buf[3] = 0x02;
	FrontboardWrite(buf, 4);
	
	SystemSleep(10);
#endif
}

#if 0//csp modify
VD_BOOL CDevFrontboard::Translate(uchar * pKeys)
{
	int i = 0, j = 0;

	switch(pKeys[1])
	{
		//ң�ؼ�
		case 0x1a:	
			pKeys[1] = FB_FLAG_REMOTE;
			i = pKeys[2] - 0x80;
			if (i < 0 || i >= (int)sizeof(RemoteKeys))
			{
				pKeys[2] = KEY_0;
			}
			else
			{
				pKeys[2] = RemoteKeys[i];
			}
			if(pKeys[3]) //����ʵ�ĵ�����Ϣʱ, �رն�ʱ��, ��ʹ��ģ��ĵ�����Ϣ
			{
				pKeys[3] = 0x00;
				m_RemoteKeyTimer.Stop();
			}
			else //����û�е�����Ϣ, ͨ����ʱ��(400ms)������ģ��ĵ�����Ϣ
			{
				pKeys[3] = 0x01;
				m_RemoteKeyTimer.Start(this, (VD_TIMERPROC)&CDevFrontboard::OnRemoteKeyUp, 400, 0, pKeys[2]);
			}
			break;

		//����
		case 0x24:	
		case 0x25:	
		case 0x29:	
		case 0x2a:
		case 0x2b:	
		case 0x2c:  
		case 0x2e:  
			for (i = 0; i < (int)sizeof(s_keyThreeIndex)/(int)sizeof(s_keyThreeIndex[0]); i++)
			{
				if (s_keyThreeIndex[i] == pKeys[3 - 1])
					break;
			}
			if (i >= (int)sizeof(s_keyThreeIndex)/(int)sizeof(s_keyThreeIndex[0]))
			{
				printf("KEY3:0x%08x translate failed!\n", *(int*)pKeys);
				return false;
			}
			for (j = 0; j < (int)sizeof(s_keyTwoIndex)/(int)sizeof(s_keyTwoIndex[0]); j++)
			{
				if (s_keyTwoIndex[j] == pKeys[2 - 1])
					break;
			}
			if (j >= (int)sizeof(s_keyTwoIndex)/(int)sizeof(s_keyTwoIndex[0]))
			{
				printf("KEY2:0x%08x translate failed!\n", *(int*)pKeys);
				return false;
			}
			pKeys[2] = PanelKeys[j][i];
			pKeys[3] = pKeys[3] ? 0xee : 0x00;
			pKeys[1] = FB_FLAG_NONE;
			
			break; 
			
		// ��֧�ַ����
		/*case 0x2d:  //ֱ����[pKeys[2]��Ϊ���,������λ��ת��
			pKeys[2] = PanelKeys[pKeys[1] - 0x10][pKeys[2]];
			pKeys[3] = pKeys[3] ? 0xee : 0x00;
			pKeys[1] = FB_FLAG_SPEC;
			if(pKeys[2] == 0 || pKeys[2] == 1)//�ٶ�1
			{
				pKeys[1] |= FB_FLAG_SPEED1;
			}
			else if(pKeys[2] == 5 || pKeys[2] == 7)//�ٶ�2
			{
				pKeys[1] |= FB_FLAG_SPEED2;
			}
			else if(pKeys[2] == 6 || pKeys[2] == 8)//�ٶ�3
			{
				pKeys[1] |= FB_FLAG_SPEED3;
			}
			break;*/
			
		case 0x21:  //ֱ����[pKeys[2]��Ϊ���,������λ��ת��
			//pKeys[2] = PanelKeys[pKeys[1] - 0x10][pKeys[2]];
			//pKeys[3] = pKeys[3] ? 0xee : 0x00;
			pKeys[1] = FB_FLAG_SPEC;
			if(pKeys[2] == 0x7f) //��Ȧ
			{
				if ((pKeys[3] < 0xf9) && (pKeys[3] > 0xf0))
				{
					pKeys[2] = KEY_LEFT;
				}
				else if((pKeys[3] < 0x09) && (pKeys[3] > 0x00))
				{
					pKeys[2] = KEY_RIGHT;
				}
				else
				{
					return false;
				}
			}
			else if(pKeys[2] ==0xfe) //��Ȧ
			{
				if (pKeys[3] == 0x01)
				{
					pKeys[2] = KEY_UP;
				}
				else if(pKeys[3] == 0x02)
				{
					pKeys[2] = KEY_DOWN;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}			
			break;
		//ģ���������ļ�ֵ��ֱ��ʹ�ã�����ת��
		case 0xfe:
			pKeys[1] = FB_FLAG_NONE;
			break;
			
		default:
			return false;
	}
	return true;
}
#endif

void CDevFrontboard::OnRemoteKeyUp(uint param)
{
	m_sigInput(XM_KEYUP, param, FB_FLAG_REMOTE);
}

void CDevFrontboard::LigtenLed(uint iLed, VD_BOOL bOpen, uint iChn)
{
#if 0//csp modify
	CGuard guard(m_Mutex);
	
	uchar adbBuffer[4];
	adbBuffer[0] = 0xaa;	
	adbBuffer[1] = iLed;
	adbBuffer[2] = iChn;
	adbBuffer[3] = bOpen;
	FrontboardWrite(adbBuffer, 4);
	
	SystemSleep(10);
#endif
}

void CDevFrontboard::LigtenLedAll(VD_BOOL bOpen)
{
#if 0//csp modify
	for(int i = FB_LED_READY; i < FB_LED_END; i++)
	{
		CGuard guard(m_Mutex);
		
		uchar adbBuffer[4];
		adbBuffer[0] = 0xaa;	
		adbBuffer[1] = i;
		adbBuffer[2] = 0;
		adbBuffer[3] = bOpen;
		FrontboardWrite(adbBuffer, 4);
		
		SystemSleep(10);
	}
#endif
}

void CDevFrontboard::Shutdown()
{
#if 0//csp modify
	CGuard guard(m_Mutex);
	
	uchar buf[4];
	buf[0] = 0xaa;
	buf[1] = FB_SHUTDOWN;
	buf[2] = 0x00;
	buf[3] = 0x00;
	FrontboardWrite(buf, 4);
	
	SystemSleep(10);
#endif
}

void CDevFrontboard::Reboot()
{
#if 0//csp modify
	CGuard guard(m_Mutex);
	
	uchar buf[4];
	buf[0] = 0xaa;
	buf[1] = FB_REBOOT;
	buf[2] = 0x00;
	buf[3] = 0x00;
	FrontboardWrite(buf, 4);
	
	SystemSleep(10);
#endif
}

VD_BOOL CDevFrontboard::AttachInput(CObject * pObj, SIG_DEV_FB_INPUT pProc)
{
	if(m_sigInput.Attach(pObj, pProc) < 0)
	{
		return FALSE;
	}
	return TRUE;
}

VD_BOOL CDevFrontboard::DetachInput(CObject * pObj, SIG_DEV_FB_INPUT pProc)
{
	if(m_sigInput.Detach(pObj, pProc) < 0)
	{
		return FALSE;
	}
	return TRUE;
}

void CDevFrontboard::SendSignal(int msg,int wpa,int lpa)
{
	m_sigInput(msg, wpa, lpa);
}

