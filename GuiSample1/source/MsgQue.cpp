#include "MultiTask/MsgQue.h"
#include "MultiTask/Guard.h"

CMsgQue::CMsgQue(int size /* = 1024 */)// : m_Queue(size)
{
/************************************************************************
	��Ϣ���еĳ�ʼ��������
	1��һ���԰Ѷ��е�������Ϣ�ṹ����󶼴�������;
	2������Щ���󶼼ӵ����ж���m_FreeQueue;
	3������Ϣ���пɷ��ʱ�־��Ϊ��Ч��
************************************************************************/
	for (int i = 0; i < MSG_PRIORITY; i++)
	{
//		m_aMQIterator[i] = m_Queue.begin();
	}
	m_nMaxMsg = size;
	m_nMsg = 0;
	m_bMsgFlg = TRUE;
}

CMsgQue::~CMsgQue()
{

}

VD_BOOL CMsgQue::SendMessage(uint msg, uint wpa /* = 0 */, uint lpa /* = 0 */, uint priority /* = 0 */)
{
	VD_MSG l_MSG;
	MSGQUEUE::iterator pi;

	m_Mutex.Enter();
	if(m_nMsg >= m_nMaxMsg)
	{
		m_Mutex.Leave();
		return FALSE;
	}
	if(priority >= MSG_PRIORITY)
	{
		m_Mutex.Leave();
		return FALSE;
	}
	if (!m_bMsgFlg) 
	{
		m_Mutex.Leave();
		return FALSE;
	}

	//�����Ϣ���⴦��, �ϲ�����ƶ���Ϣ
	if(msg == XM_MOUSEMOVE && !m_Queue.empty())
	{
		for (pi = m_Queue.begin(); pi != m_Queue.end(); pi++)
		{
			if((*pi).msg >= XM_LBUTTONDOWN && (*pi).msg <= XM_MBUTTONDBLCLK)
			{
				break;
			}
			if((*pi).msg == XM_MOUSEMOVE)
			{
				(*pi).wpa = wpa;
				(*pi).lpa = lpa;
				(*pi).time = SystemGetMSCount();
				m_Mutex.Leave();
				return TRUE;
			}
		}
	}
/************************************************************************
	������Ϣ:
	1���������ȼ��Ѹ���Ϣ�������m_Queue��
//	2���ڸö����в��Ҹ���Ϣ�ڵ㣬ֱ���Ҳ����ýڵ���˳�ѭ�����Ӹú������أ�
************************************************************************/
	l_MSG.msg = msg;
	l_MSG.wpa = wpa;
	l_MSG.lpa = lpa;
	l_MSG.time = SystemGetMSCount();
//	m_aMQIterator[priority] = m_Queue.insert(m_aMQIterator[priority], l_MSG);
//printf("<<<<<<<<<<l_MSG.msg==%x l_MSG.wpa==%x l_MSG.lpa==%x\n",	l_MSG.msg, l_MSG.wpa, l_MSG.lpa);
	m_Queue.push_front(l_MSG);
	m_nMsg++;
	m_Mutex.Leave();

	m_Semaphore.Post();
	return TRUE;
}

VD_BOOL CMsgQue::RecvMessage(VD_MSG *pMsg, VD_BOOL wait /* = TRUE */)
{
/************************************************************************
	������Ϣ������ȴ���һֱ�ȵ�����Ϣʱ���أ�����ֱ�ӷ��ء�
	1������Ϣæ����m_QueueȡԪ�أ����ȡ�ɹ�����ֱ�ӷ��أ�
	2������ѭ������Ϣæm_Queue��ȡԪ�أ�ֱ��ȡ�ɹ����˳�ѭ����
************************************************************************/
	if(wait)
	{
		m_Semaphore.Pend();
	}

	CGuard guard(m_Mutex);
//	trace("RecvMessage %d\n", m_nMsg);

	if (m_Queue.empty()) 
	{
//		tracepoint();
		return FALSE;
	}
	if(!wait)
	{
		m_Semaphore.Pend();
	}
	assert(m_nMsg);
	*pMsg = m_Queue.back();
	m_Queue.pop_back();
//	printf("pMsg==%x  l_MSG.msg==%x l_MSG.wpa==%x l_MSG.lpa==%x\n",	pMsg, l_MSG.msg, l_MSG.wpa, l_MSG.lpa);
	m_nMsg--;

	return TRUE;
}

void CMsgQue::QuitMessage ()
{
	CGuard guard(m_Mutex);

	m_bMsgFlg = FALSE;
}

void CMsgQue::ClearMessage()
{
	CGuard guard(m_Mutex);

	int n = m_nMsg;
	for(int i = 0; i < n; i++)
	{
		m_Semaphore.Pend();
		m_Queue.pop_back();
		m_nMsg--;
	}
}

int CMsgQue::GetMessageCount()
{
	CGuard guard(m_Mutex);

	return m_nMsg;
}

int CMsgQue::GetMessageSize()
{
	CGuard guard(m_Mutex);

	return m_nMaxMsg;
}

void CMsgQue::SetMessageSize(int size)
{
	CGuard guard(m_Mutex);

	m_nMaxMsg = size;
}
