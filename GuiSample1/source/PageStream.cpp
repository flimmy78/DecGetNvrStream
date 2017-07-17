/*add by liu*/
#include <stdio.h>
#include <string.h>
#include "GUI/Pages/PageStream.h"
#include "GUI/Pages/BizData.h"
#include "GUI/Pages/PageInfoFrameWork.h"
#include "../../Common/inc/tlsdk/Ipcamera.h"
/*
const char* szStreamTitle[5] =
{
	"&CfgPtn.Channel",
	"&CfgPtn.MainResolution",
	"&CfgPtn.BandWidth",
	"&CfgPtn.SubResolution",
	"&CfgPtn.BandWidth",
};
*/
//yaogang modify 20140918
std::string szStreamTitle[5];

//yaogang modify 20150916
const char* szTotal[4] = 
{
	"&CfgPtn.TotalStream",	//��ǰʹ�ô���
	//�������
	"&CfgPtn.TotalCapacity",	//�ܽ������
	//NVR��
	"&CfgPtn.TotalCapacity2",	//����Ƶ�ܴ���
	"&CfgPtn.MainCapacity",	//�������洢����
};
//�������
const char* szTotalCapacity = "144Mbps";
//NVR��
const char* szTotalCapacity2 = "216Mbps";
const char* szMainCapacity = "160Mbps";



/*
const char* szTotalCapacity[2] =
{
	"144Mbps", //16*8Mbps
	"57600",
};
*/


								
CPageStream::CPageStream( VD_PCRECT pRect,VD_PCSTR psz,VD_BITMAP* icon /*= NULL*/,CPage * pParent /*= NULL*/ )
:CPageFrame(pRect, psz, icon, pParent)
{
	SetMargin(0,0,0,0);
	m_page = 0;

	m_maxchn = GetVideoMainNum();
	m_maxPage = m_maxchn%MAX_STREAM_INFO ? (m_maxchn+MAX_STREAM_INFO)/MAX_STREAM_INFO : m_maxchn/MAX_STREAM_INFO;
	printf("%s m_maxchn: %d, m_maxPage: %d\n", __func__, m_maxchn, m_maxPage);
	int i, j;
	CRect rt;
	
	//yaogang modify 20150916
	nNVROrDecoder = 1; //Ծ��: 1 nvr��2 ��Ѳ��������3 �л�������
	
	SBizDvrInfo DvrInfo;
	if (BizGetDvrInfo(&DvrInfo) == 0)
	{
		nNVROrDecoder = DvrInfo.nNVROrDecoder;
	}
	else
	{
		printf("********************%s -- nNVROrDecoder invalid\n", __func__);
	}
	
////yaogang modify 20140918
	szStreamTitle[0] = GetParsedString("&CfgPtn.Channel");//ͨ��
	
	szStreamTitle[1] = GetParsedString("&CfgPtn.MainStream");//�������ֱ���
	szStreamTitle[1] += GetParsedString("&CfgPtn.Resolution");
	
	szStreamTitle[2] = GetParsedString("&CfgPtn.MainStream");//����������
	szStreamTitle[2] += GetParsedString("&CfgPtn.BandWidth");
	
	szStreamTitle[3] = GetParsedString("&CfgPtn.SubStream");//�������ֱ���
	szStreamTitle[3] += GetParsedString("&CfgPtn.Resolution");
	
	szStreamTitle[4] = GetParsedString("&CfgPtn.SubStream");//����������
	szStreamTitle[4] += GetParsedString("&CfgPtn.BandWidth");
//

	/*��ɫ���*/
	pFrame = CreateTableBox(CRect(17, 60, m_Rect.Width()-17, m_Rect.Height()-17), this, 1, 1);
	pFrame->SetFrameColor(VD_RGB(56,108,148));
	/*�ڿ�*/
	//�����
	//ͨ��|  �������ֱ���|  ����(Kb/s)  |   �������ֱ���|   ����(Kb/s)
	CRect tableRT1(22, 68, m_Rect.Width()-22, m_Rect.Height()-310);

	//yaogang modify 20140918
	pTab1 = CreateTableBox(tableRT1, this, 5, 1);//5��1��
	pTab1->SetColWidth(0, TEXT_WIDTH*3);
	pTab1->SetColWidth(1, TEXT_WIDTH*6);//�������ֱ���
	pTab1->SetColWidth(2, TEXT_WIDTH*5);//����������
	pTab1->SetColWidth(3, TEXT_WIDTH*6);//�������ֱ���
	//pTab1->SetColWidth(4, TEXT_WIDTH*4);//����������

	//title ��
	for (i=0; i<5; i++)
	{
		pTab1->GetTableRect(i, 0, &rt);
		//yaogang modify 20140918
		pStream1[i] = CreateStatic(CRect(rt.left+tableRT1.left+3, rt.top+tableRT1.top, rt.right+tableRT1.left, rt.bottom+tableRT1.top), this, szStreamTitle[i].c_str());
		pStream1[i]->SetBkColor(VD_RGB(67,77,87));
		pStream1[i]->SetTextColor(VD_GetSysColor(COLOR_CTRLTEXT));
		
		pStream1[i]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	//pTab1->SetColWidth(0, 20);
	
	//��ͨ����
	CRect tableRT2;
	tableRT2.left = 22;
	tableRT2.top = tableRT1.bottom;
	tableRT2.right = m_Rect.Width()-22-25;
	tableRT2.bottom = m_Rect.Height()-80;
	//yaogang modify 20140918
	pTab2 = CreateTableBox(tableRT2, this, 5, MAX_STREAM_INFO);//5��8��
	pTab2->SetColWidth(0, TEXT_WIDTH*3);
	pTab2->SetColWidth(1, TEXT_WIDTH*6);//�������ֱ���
	pTab2->SetColWidth(2, TEXT_WIDTH*5);//����������
	pTab2->SetColWidth(3, TEXT_WIDTH*6);//�������ֱ���
	//pTab1->SetColWidth(4, TEXT_WIDTH*4);//����������

	/*
	for (i=0; i<2; i++)
	{
		pTab2->SetColWidth(i, tableRT1.Width()/3);
	}
	*/
	
	//�����ʾ������
	for (j = 0; j < 5; j++)//��
	{
		for (i=0; i<MAX_STREAM_INFO; i++)//��
		{
			pTab2->GetTableRect(j, i, &rt);
			pStream2[i+j*MAX_STREAM_INFO] = CreateStatic(CRect(rt.left+tableRT2.left+3, rt.top+tableRT2.top, rt.right+tableRT2.left, rt.bottom+tableRT2.top), this, "");
			pStream2[i+j*MAX_STREAM_INFO]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
		}
	}

	//��ʾ��0��chnID
	char chnid[4];
	for (i=0; i<MAX_STREAM_INFO; i++)
	{
		sprintf(chnid, "%d", i+1);
		pStream2[i]->SetText(chnid);
	}
	/*
	for (i=0; i<MAX_STREAM_INFO; i++)
	{
		CRect rt;
		pTab2->GetTableRect(0, i, &rt);
		pStream2[i] = CreateStatic(CRect(rt.left+tableRT2.left+3, rt.top+tableRT2.top, rt.right+tableRT2.left, rt.bottom+tableRT2.top), this, szChannel[i]);
		pStream2[i]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	for (i=0; i<MAX_STREAM_INFO; i++)
	{
		CRect rt;
		pTab2->GetTableRect(1, i, &rt);
		pStream2[i+MAX_STREAM_INFO] = CreateStatic(CRect(rt.left+tableRT2.left+3, rt.top+tableRT2.top, rt.right+tableRT2.left, rt.bottom+tableRT2.top), this, "");
		pStream2[i+MAX_STREAM_INFO]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	for (i=0; i<MAX_STREAM_INFO; i++)
	{
		CRect rt;
		pTab2->GetTableRect(2, i, &rt);
		pStream2[i+2*MAX_STREAM_INFO] = CreateStatic(CRect(rt.left+tableRT2.left+3, rt.top+tableRT2.top, rt.right+tableRT2.left, rt.bottom+tableRT2.top), this, "");
		pStream2[i+2*MAX_STREAM_INFO]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	*/
	//yaogang modify 20150916
	//ͳ�ƿ�
	if (nNVROrDecoder != 1) //�����
	{
		CRect tableRT3;
		tableRT3.left = 22;
		tableRT3.top = tableRT2.bottom;
		tableRT3.right = m_Rect.Width()-22;
		tableRT3.bottom = m_Rect.Height()-25;
		//yaogang modify 20140918
		pTab3 = CreateTableBox(tableRT3, this, 2, 2);//2��2��

		for (i=0; i<2; i++)
		{
			//CRect rt;
			pTab3->GetTableRect(0, i, &rt);
			pStream3[i] = CreateStatic(CRect(rt.left+tableRT3.left+3, rt.top+tableRT3.top, rt.right+tableRT3.left, rt.bottom+tableRT3.top), this, szTotal[i]);
			pStream3[i]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
		}
		for (i=0; i<2; i++)
		{
			//CRect rt;
			pTab3->GetTableRect(1, i, &rt);
			pStream3[i+2] = CreateStatic(CRect(rt.left+tableRT3.left+3, rt.top+tableRT3.top, rt.right+tableRT3.left, rt.bottom+tableRT3.top), this, "");
			pStream3[i+2]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
			
		}
		pStream3[3]->SetText(szTotalCapacity);
	}
	else //NVR
	{
		CRect tableRT3, tableRT4;
		tableRT3.left = 22;
		tableRT3.top = tableRT2.bottom;
		tableRT3.right = m_Rect.Width()-22;
		tableRT3.bottom = m_Rect.Height()-50;
		
		pTab3 = CreateTableBox(tableRT3, this, 2, 1);//2��1�е�ǰʹ�ô���

		pTab3->GetTableRect(0, 0, &rt);
		pStream3[0] = CreateStatic(CRect(rt.left+tableRT3.left+3, rt.top+tableRT3.top, rt.right+tableRT3.left, rt.bottom+tableRT3.top), this, szTotal[0]);
		pStream3[0]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);

		pTab3->GetTableRect(1, 0, &rt);
		pStream3[2] = CreateStatic(CRect(rt.left+tableRT3.left+3, rt.top+tableRT3.top, rt.right+tableRT3.left, rt.bottom+tableRT3.top), this, "");
		pStream3[2]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
		
		//����Ƶ�ܴ���----- �������洢����
		tableRT4.left = 22;
		tableRT4.top = tableRT3.bottom;
		tableRT4.right = m_Rect.Width()-22;
		tableRT4.bottom = m_Rect.Height()-25;
		
		pTab4 = CreateTableBox(tableRT4, this, 4, 1);//2��1�е�ǰʹ�ô���

		pTab4->GetTableRect(0, 0, &rt);
		pStream4[0] = CreateStatic(CRect(rt.left+tableRT4.left+3, rt.top+tableRT4.top, rt.right+tableRT4.left, rt.bottom+tableRT4.top), this, szTotal[2]);
		pStream4[0]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);

		pTab4->GetTableRect(1, 0, &rt);
		pStream4[1] = CreateStatic(CRect(rt.left+tableRT4.left+3, rt.top+tableRT4.top, rt.right+tableRT4.left, rt.bottom+tableRT4.top), this, szTotalCapacity2);
		pStream4[1]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);

		pTab4->GetTableRect(2, 0, &rt);
		pStream4[2] = CreateStatic(CRect(rt.left+tableRT4.left+3, rt.top+tableRT4.top, rt.right+tableRT4.left, rt.bottom+tableRT4.top), this, szTotal[3]);
		pStream4[2]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);

		pTab4->GetTableRect(3, 0, &rt);
		pStream4[3] = CreateStatic(CRect(rt.left+tableRT4.left+3, rt.top+tableRT4.top, rt.right+tableRT4.left, rt.bottom+tableRT4.top), this, szMainCapacity);
		pStream4[3]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	//yaogang modify 20140918
	/*
	for (i=0; i<2; i++)
	{
		//CRect rt;
		pTab3->GetTableRect(2, i, &rt);
		pStream3[i+4] = CreateStatic(CRect(rt.left+tableRT3.left+3, rt.top+tableRT3.top, rt.right+tableRT3.left, rt.bottom+tableRT3.top), this, "");
		pStream3[i+4]->SetTextAlign(VD_TA_XCENTER|VD_TA_YCENTER);
	}
	*/
	/*����*/
	pScroll = CreateScrollBar(CRect(tableRT2.right, //��߽�
									tableRT2.top,//����
									tableRT2.right+25, //�ұ߽�
									tableRT2.bottom), //�ױ߽�
									this,
									scrollbarY,
									0,
									(m_maxPage-1)*10, 
									1, 
									(CTRLPROC)&CPageStream::OnTrackMove);


}

/*
���Ҫ���ϣ���Ȼ���뱨��
 undefined reference to `vtable for CPageStream'
 collect2: ld returned 1 exit status
*/
CPageStream::~CPageStream()
{

}

//add by liu chn:0-15
int CPageStream::ShowStreamInfo(uint param)
{
	int StreamKbps[2] = {0};//����������
	int chn = 0;
	char buf[20] = {0};//eg: xxxx kbps
	int totalStreamKbps = 0;
	int video_width[2] = {0};//����������
	int video_height[2]  = {0};//����������
	//printf("***********ShowStreamInfo\n");
	//char resulotion[10];//eg: 1920*1080
	int issub = 0;
	int row = 0;
	
	for(chn=0; chn<m_maxchn; chn++)
	{
		for (issub = 0; issub< 2; issub++)
		{
			StreamKbps[issub] = Get_StreamKbps(chn + issub*m_maxchn);
			if(-1 == StreamKbps[issub])
			{
				printf("ShowStreamInfo:Stream cha%d issub: %d\n", chn, issub);
				return -1;
			}
			//yaogang modify 20140819
			IPC_GetStreamResolution(chn + issub*m_maxchn, &video_width[issub], &video_height[issub]);
			
			if (StreamKbps[issub] == 0)
			{
				video_height[issub] = video_width[issub] = 0;
			}
			
			totalStreamKbps += StreamKbps[issub];			
		}

		//��ʾ
		if ((chn >= m_page*MAX_STREAM_INFO)
			&& (chn < (m_page+1)*MAX_STREAM_INFO))
		{
			row = chn - m_page*MAX_STREAM_INFO;

			sprintf(buf, "%d", chn+1);//ͨ��
			pStream2[row]->SetText(buf); 

			sprintf(buf, "%d*%d", video_width[0], video_height[0]);//�������ֱ���
			pStream2[row + MAX_STREAM_INFO]->SetText(buf);
			
			sprintf(buf, "%d Kbps",StreamKbps[0]);//����������
			pStream2[row + 2*MAX_STREAM_INFO]->SetText(buf);
			
			sprintf(buf, "%d*%d", video_width[1], video_height[1]);//�������ֱ���
			pStream2[row + 3*MAX_STREAM_INFO]->SetText(buf);
			
			sprintf(buf, "%d Kbps",StreamKbps[1]);//����������
			pStream2[row + 4*MAX_STREAM_INFO]->SetText(buf);
		}
		
	}
	
	
	sprintf(buf,"%d Kbps",totalStreamKbps);
	pStream3[2]->SetText(buf);
	//pStream3[3]->SetText(szTotalCapacity);
	
	return 0;
	
	
}


void CPageStream::OnTrackMove()
{
	int i;
	int pos = pScroll->GetPos();
	
    if (m_page==pos) {
        return;
    }
	
    m_page = pos;

/*	
	if(0 == m_page)//��ʾͨ��1-8
	{
		for (i=0; i<MAX_STREAM_INFO; i++)
		{
			pStream2[i]->SetText(szChannel[i]);
		}

	}
	else //��ʾͨ��9-16
	{
		for (i=0; i<MAX_STREAM_INFO; i++)
		{
			pStream2[i]->SetText(szChannel[i+MAX_STREAM_INFO]);
		}

	}
	*/
	ShowStreamInfo(0);
}


VD_BOOL CPageStream::UpdateData(UDM mode)
{
	
	if(UDM_OPEN == mode)
	{
		m_StreamTimer = new CTimer("Stream_Timer");
		m_StreamTimer->Start(this, (VD_TIMERPROC)&CPageStream::ShowStreamInfo, 1000,1000);

	}
	else if(UDM_CLOSED == mode)
	{
		m_StreamTimer->Stop();
	}
	
	return TRUE;
}




