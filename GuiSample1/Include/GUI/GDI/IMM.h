
#ifndef __IMM_H__
#define __IMM_H__


#include "APIs/DVRDEF.H"
#include "System/Object.h"
#include "System/File.h"

typedef struct _HZMB_HEAD{
	int sec_num;		//������
	int hz_num;			//������
}HZMB_HEAD;

typedef struct _HZMB_SECTOR{
	char str[8];		//�ؼ����ַ���,������ƴ��,ע��,�ʻ��ȵ�
	int offset;			//�������ݿ�ʼƫ��
	int next;			//��һ�����εĽڵ��
}HZMB_SECTOR;

class CIMM : public CObject
{
public:
	CIMM();
	~CIMM();

	VD_BOOL Open(VD_PCSTR table);
	void Close();
	int Filter(VD_PCSTR key);
	VD_PCSTR GetChar(int offset);

private:
	CFile m_FileTable;
	HZMB_HEAD* m_pHead;
	HZMB_SECTOR* m_pSectors;
	char* m_pChars;

	int m_nStartPos;
	int m_nEndPos;
};

#endif //__IMM_H__
