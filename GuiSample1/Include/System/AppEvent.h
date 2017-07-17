#ifndef APP_EVENT
#define APP_EVENT

#include "MultiTask/Guard.h"
#include "MultiTask/Timer.h"
#include "System/pool_allocator.h"
#include "Object.h"
#include "Signals.h"
//#include "Configs/ConfigWorksheet.h"
//#include "Configs/ConfigEvents.h"

#define MAX_EVENT_LATCH_NUMBER 4

typedef enum app_event_action
{
	appEventStart = 0,		// �¼���ʼ
	appEventStop,			// �¼�����
	appEventConfig,			// �¼����ñ仯������Ϊ���ղ���
	appEventLatch,			// �¼���ʱ����������Ϊ�������
	appEventPulse			// �����¼���û����ʼ�ͽ���
}appEventAction;

typedef TSignal5<appEventCode, int, appEventAction, const EVENT_HANDLER *, const CConfigTable*> SIG_EVENT;

typedef struct tagEVENT_LATCH
{
	void*	handler;			// ��Ҫ�����¼������Ĵ�����
	SIG_EVENT::SigProc proc;	// �ص�����
	int		second;				// �¼�������ʱ��
}EVENT_LATCH;

typedef struct tagEVENT_ACTIVE
{
	VD_BOOL			inSection;			// ��ʱ���֮��
	VD_BOOL			inStopping;			// ����ֹͣ����Ϊ��ʱ�Ĵ��ڣ�����û��ɾ��
	const EVENT_HANDLER*	parameter;			// �¼��������
	const CONFIG_WORKSHEET*	workSheet;			// �¼���������
	EVENT_LATCH		latch[MAX_EVENT_LATCH_NUMBER]; // �¼���������
	int				latchNumber;		// �¼���������
}EVENT_ACTIVE;

typedef struct tagEVENT_INFO
{
	appEventCode	code;
	int				index;
	const EVENT_HANDLER*	parameter;			// �¼��������
}EVENT_INFO;

class CAppEventManager : public CObject
{
	typedef std::map<int, EVENT_ACTIVE, std::less<int>, pool_allocator<EVENT_ACTIVE> > MAP_EVENT;

private:
	CAppEventManager(void);
	void onTimer(uint arg);
	void checkTimeSectionAll();

public:
	PATTERN_SINGLETON_DECLARE(CAppEventManager);

	VD_BOOL attach(void* handler, SIG_EVENT::SigProc proc);
	VD_BOOL detach(void* handler, SIG_EVENT::SigProc proc);
	VD_BOOL checkTimeSection(const CONFIG_WORKSHEET *workSheet);
	void notify(appEventCode code, int index = 0, appEventAction action = appEventStart, const EVENT_HANDLER *param = NULL, const CONFIG_WORKSHEET *workSheet = NULL, const CConfigTable* data = NULL);
	static const char*getEventName(appEventCode code);
	void getNextEvent();
	VD_BOOL getNextEvent(void* handler, EVENT_INFO* info);
	void latchEvent(void* handler, SIG_EVENT::SigProc proc, appEventCode code, int index, int second);
	void dump();

private:
	SIG_EVENT m_sigEventHandler;
	MAP_EVENT m_mapEventActive;
	CMutex m_mutex;
	CTimer m_timer;
	SYSTEM_TIME m_oldTime;
	VD_BOOL	m_latchAllowed;		// �Ƿ�������ʱ���������޸Ļ��߶�ʱʱ��ε�����������ʱ���¼�������������ʱ
	MAP_EVENT::iterator m_pi;	// �¼�����������
};

#endif

