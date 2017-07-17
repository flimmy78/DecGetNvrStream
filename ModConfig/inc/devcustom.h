#ifndef _DEVCUSTOM_H_
#define _DEVCUSTOM_H_

#define DVR_CUSTOM_PATH  "/tmp/data/cfg/DvrCustom.ini"

//���ñ���
#define DVR_CONFIG_INFO 			"DvrCfg"
#define DVR_CONFIG_INFO_SAVETYPE	"CfgWay"
#define DVR_CONFIG_INFO_FORMAT		"CfgFormat"
#define DVR_CONFIG_INFO_FLASHDEV	"CfgFlashDev"
#define DVR_CONFIG_INFO_FILEPATH	"CfgPath"
#define DVR_CONFIG_INFO_DEFAULTPATH	"CfgPathDefault"
#define DVR_CONFIG_INFO_ZIP			"CfgZip"
#define DVR_CONFIG_INFO_OFFSET		"Offset"
#define DVR_CONFIG_INFO_LENGTH		"Length"
//

//DVR����
#define DVR_PROPERTY				"DvrProperty"
#define DVR_PROPERTY_VERSION		"Version"
#define DVR_PROPERTY_MODEL			"Model"
#define DVR_PROPERTY_PRODUCTNUMBER	"Productnumber" //9624
#define DVR_PROPERTY_PREVIEWNUM		"PreviewNum"
#define DVR_PROPERTY_VIDEOMAINNUM	"VidMainNum"
#define DVR_PROPERTY_VIDEOSUBNUM	"VidSubNum"
#define DVR_PROPERTY_AUDIONUM		"AudNum"
#define DVR_PROPERTY_SNAPNUM		"SnapNum"
#define DVR_PROPERTY_VOIPNUM		"VoipNum"
#define DVR_PROPERTY_RECNUM			"RecNum"
#define DVR_PROPERTY_SENSORNUM		"SensorNum"
#define DVR_PROPERTY_ALARMOUTNUM	"AlarmoutNum"
#define DVR_PROPERTY_BUZZNUM		"BuzzNum"
#define DVR_PROPERTY_MAXHDDNUM		"MaxHddNum"
#define DVR_PROPERTY_MAINFRAMESIZE	"MaxMainFrameSize"
#define DVR_PROPERTY_SUBFRAMESIZE	"MaxSubFrameSize"
#define DVR_PROPERTY_AUDIOFRAMESIZE	"MaxAudFrameSize"
#define DVR_PROPERTY_SNAPFRAMESIZE	"MaxSnapFrameSize"
//
#define DVR_PROPERTY_OSDOPERATOR	"OsdOperator"
#define DVR_PROPERTY_STROSDNUM		"StrOsdNum"
#define DVR_PROPERTY_IMGOSDNUM		"ImgOsdNum"
#define DVR_PROPERTY_RECTOSDNUM		"RectOsdNum"
#define DVR_PROPERTY_RECOSDRGB		"RectOsdRgb"
#define DVR_PROPERTY_LINEOSDNUM		"LineOsdNum"
#define DVR_PROPERTY_REFWIDTH		"RefWidth"
#define DVR_PROPERTY_REFHEIGHT		"RefHeight"
//
#define DVR_PROPERTY_AUDIO_ENCTYPE		"AudioEncType"
#define DVR_PROPERTY_AUDIO_SAMPLERATE	"SampleRate"
#define DVR_PROPERTY_AUDIO_SAMPLEDURAT	"SampleDuration"
#define DVR_PROPERTY_AUDIO_SAMPLESIZE	"SampleSize"
#define DVR_PROPERTY_AUDIO_BITWIDTH		"BitWidth"
//
#define DVR_PROPERTY_VOIP_ENCTYPE		"VoipEncType"
#define DVR_PROPERTY_VOIP_SAMPLERATE	"VoipSampleRate"
#define DVR_PROPERTY_VOIP_SAMPLEDURAT	"VoipSampleDuration"
#define DVR_PROPERTY_VOIP_SAMPLESIZE	"VoipSampleSize"
#define DVR_PROPERTY_VOIP_BITWIDTH		"VoipBitWidth"
//
#define DVR_PROPERTY_MD_AREA_ROWS		"MDAreaRows"
#define DVR_PROPERTY_MD_AREA_COLS		"MDAreaCols"
#define DVR_PROPERTY_MD_SENSE_MIN		"MDSenseMin"
#define DVR_PROPERTY_MD_SENSE_MAX		"MDSenseMax"
//
#define DVR_PROPERTY_SCH_SEGMENTS		"SchSegments"
#define DVR_PROPERTY_OUTPUT_SYNC		"OutputSync"
#define DVR_PROPERTY_DEVICE_TYPE	 	"NVROrDecoder"	//����NVR�ͽ�����--- Ծ��

//

//ȡֵ�б�
#define DVR_CFGPATTERN				"CfgPattern"
#define DVR_CFGPATTERN_LANGSUP "LangSup" // ���Է����ַ�����־(&CfgPtn.LangEng &CfgPtn.Sch CfgPtn.LangTch.....)
#define DVR_CFGPATTERN_LANGSRC "LangSrc" // ֧�ֵ�������Դ�ļ��б�(StringsEng.txt StringsSch.txt StringsTch.txt....)
#define DVR_CFGPATTERN_PREVIEWMODE "PreviewMode" // ģʽ�����ַ�����־(&CfgPtn.Split1 &CfgPtn.Split4 &CfgPtn.Split6.....)
#define DVR_CFGPATTERN_BITRATE "BitRate" // λ�ʷ����ַ�����־(&CfgPtn.Brt8 &CfgPtn.Brt16 &CfgPtn.Brt32.....)
#define DVR_CFGPATTERN_BITTYPE "BitType" // λ�����ͷ����ַ�����־(&CfgPtn.Cbr &CfgPtn.Vbr)
#define DVR_CFGPATTERN_PICLEVEL "PicLevel" // ͼ�����������ַ�����־(&CfgPtn.PicBest &CfgPtn.PicBetter &CfgPtn.PicGood &CfgPtn.Normal &CfgPtn.PicBad  &CfgPtn.PicWorse &CfgPtn.Picworst)
#define DVR_CFGPATTERN_VMAINRESOLH_NUM "VMainResolH" // ������ͨ��֧�ֵķֱ����ַ�����־(&CfgPtn.Qcif &CfgPtn.Cif &CfgPtn.HD1 &CfgPtn.4cif &CfgPtn.D1)
#define DVR_CFGPATTERN_VMAINRESOL "VMainResol" // ������ͨ��֧�ֵķֱ����ַ�����־(&CfgPtn.Qcif &CfgPtn.Cif &CfgPtn.HD1 &CfgPtn.4cif &CfgPtn.D1)
#define DVR_CFGPATTERN_VMAINRESOL2 "VMainResol2"// ������ͨ��֧�ֵķֱ����ַ�����־(&CfgPtn.Qcif &CfgPtn.Cif &CfgPtn.HD1 &CfgPtn.4cif &CfgPtn.D1)
#define DVR_CFGPATTERN_VSUBRESOL "VSubResol" // ������ͨ��֧�ֵķֱ����ַ�����־(&CfgPtn.Qcif &CfgPtn.Cif &CfgPtn.HD1 &CfgPtn.4cif &CfgPtn.D1)
#define DVR_CFGPATTERN_VMOBRESOL "VMobResol" // �ֻ�����ͨ��֧�ֵķֱ����ַ�����־(&CfgPtn.Cif &CfgPtn.D1 &CfgPtn.720P &CfgPtn.1080P)

#define DVR_CFGPATTERN_FPSPAL "FpsPal" // P�Ƴ�����֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_FPSNTSC "FpsNtsc" // N�Ƴ�����֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_FPSPALD1 "FpsPalD1" // P��D1��֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_FPSNTSCD1 "FpsNtscD1" // N��D1��֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_FPSPALCIF "FpsPalCif" // P��Cif��֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_FPSNTSCCIF "FpsNtscCif" // N��Cif��֡�ʷ����ַ�����־(&CfgPtn.Fps1 &CfgPtn.Fps2 &CfgPtn.Fps3.....)
#define DVR_CFGPATTERN_STREAMTYPE "StreamType" // ¼���������ͷ����ַ�����־(&CfgPtn.Vid &CfgPtn.AV)
#define DVR_CFGPATTERN_VIDEOSTANDARD "VideoStandard" // ��Ƶ��ʽ�����ַ�����־(&CfgPtn.PAL &CfgPtn.NTSC)
#define DVR_CFGPATTERN_OUTPUT "Output" // ��Ƶ��������ַ�����־(&CfgPtn.Cvbs &CfgPtn.Vga800x600 &CfgPtn.Vga1024x768 &CfgPtn.Vga1280x1024)
#define DVR_CFGPATTERN_SWITCHPICTURE "DWellSwitchPicture"  //��Ѳ����ѡ�� 
/*
CfgPtn.DWellSinglePicture ������
CfgPtn.DWellFourPictures �Ļ���
CfgPtn.DWellNinePictures �Ż���
*/

#define DVR_CFGPATTERN_LOCKTIME "LockTime" // ������ʱ��ѡ����ַ�����־(&CfgPtn.NeverLock &CfgPtn.30s &CfgPtn.60s &CfgPtn.3x60s &CfgPtn.5x60s)
#define DVR_CFGPATTERN_DATEFORMAT "DateFormat" // ���ڸ�ʽ�����ַ�����־(&CfgPtn.YYYYMMDD &CfgPtn.MMDDYYYY &CfgPtn.DDMMYYYY)
#define DVR_CFGPATTERN_TIMESTANDARD "TimeStandard" // ʱ����ʽ�����ַ�����־(&CfgPtn.24h &CfgPtn.12h)
#define DVR_CFGPATTERN_SITEMAINOUT "SiteMainOut" // �ֳ�����������ַ�����־(&CfgPtn.SiteOut1x1 &CfgPtn.SiteOut2x2....)
#define DVR_CFGPATTERN_INTERVAL "Interval" // ʱ���������ַ�����־
#define DVR_CFGPATTERN_SENSORTYPE "SensorType" // ���������ͷ����ַ�����־(&CfgPtn.LowLevel/&CfgPtn.NormalClose &CfgPtn.HighLevel/&CfgPtn.NormalOpen)
#define DVR_CFGPATTERN_ALARMOUTTYPE "AlarmoutType" // ����������ͷ����ַ�����־(&CfgPtn.LowLevel/&CfgPtn.NormalClose &CfgPtn.HighLevel/&CfgPtn.NormalOpen)
#define DVR_CFGPATTERN_DELAY "Delay" // ��ʱʱ��ѡ����ַ�����־(&CfgPtn.5s &CfgPtn.10s &CfgPtn.20s &CfgPtn.30s &CfgPtn.60s &CfgPtn.120s &CfgPtn.Always) 
#define DVR_CFGPATTERN_PRETIME "PreTime" // 
#define DVR_CFGPATTERN_RECDELAY "RecDelay" // ¼����ʱʱ��ѡ����ַ�����־(&CfgPtn.10s &CfgPtn.15s &CfgPtn.20s &CfgPtn.30s &CfgPtn.60s &CfgPtn.2x60 &CfgPtn.3x60 &CfgPtn.5x60) 
#define DVR_CFGPATTERN_RECEXPIRE "RecExpire" // ¼���������ѡ����ַ�����־(&CfgPtn.1day &CfgPtn.2days...&CfgPtn.60days &CfgPtn.Never) 
#define DVR_CFGPATTERN_SCHEDULETYP "ScheduleTyp" // ��������ѡ����ַ�����־(&CfgPtn.Weekly &CfgPtn.Monthly &CfgPtn.Everyday) 
#define DVR_CFGPATTERN_PTZLINKTYPE "PtzLinkType" // ��̨��������ѡ����ַ�����־(&CfgPtn.NULL &CfgPtn.LinkPreset &CfgPtn.LinkPatrol &CfgPtn.LinkLocus)  
#define DVR_CFGPATTERN_BAUDRATE "BaudRate" // ������ѡ����ַ�����־(&CfgPtn.Baud110 &CfgPtn.Baud300 &CfgPtn.Baud600 &CfgPtn.Baud1200 &CfgPtn.Baud2400 &CfgPtn.Baud4800 &CfgPtn.Baud9600 &CfgPtn.Baud19200 &CfgPtn.Baud38400 &CfgPtn.Baud57600 &CfgPtn.Baud115200 &CfgPtn.Baud230400 &CfgPtn.Baud460800 &CfgPtn.Baud921600)
#define DVR_CFGPATTERN_DATABIT "DataBit" // ����λ����ѡ����ַ�����־(&CfgPtn.Databit8 &CfgPtn.Databit7 &CfgPtn.Databit6)  
#define DVR_CFGPATTERN_STOPBIT "StopBit" // ֹͣλ����ѡ����ַ�����־(&CfgPtn.StopBit1 &CfgPtn.StopBit2)
#define DVR_CFGPATTERN_CHECKTYPE "CheckType" // У����������ѡ����ַ�����־(&CfgPtn.CheckNull &CfgPtn.CheckOdd &CfgPtn.CheckEven)
#define DVR_CFGPATTERN_FLOWCTRLTYPE "FlowCtrlType" // ���ط�ʽ����ѡ����ַ�����־(&CfgPtn.Null &CfgPtn.Hardware &CfgPtn.XonXoff)
#define DVR_CFGPATTERN_DDNSDOMAIN "DDNSDomain" // DDNS�����ַ���(popdvr 3322 dyndns)
#define DVR_CFGPATTERN_DDNSUPINTVL "DDNSUpdateTime" // DDNS IP���¼���ַ�����־
#define DVR_CFGPATTERN_VIDEOSRCTYPE "VideoSrcType" // ��Ƶ����Դ �ַ�����־
//
//yaogang modify 20141210 ShenGuang
//ȡֵ�б�
#define DVR_CFGPATTERN_SNAP_RES "SnapRes"
#define DVR_CFGPATTERN_SNAP_QUALITY "SnapQuality"
#define DVR_CFGPATTERN_SNAP_INTERVAL "SnapInterval"
#define DVR_CFGPATTERN_RepairName "RepairName" //����ϵͳ����
#define DVR_CFGPATTERN_RepairType "RepairType"//����ϵͳ����
#define DVR_CFGPATTERN_Maintain "Maintain"//ά��ά��
#define DVR_CFGPATTERN_Test "Test"//����
#define DVR_CFGPATTERN_AlarmInput "AlarmInput" //����ͼƬ����: �������


#endif //_DEVCUSTOM_H_

