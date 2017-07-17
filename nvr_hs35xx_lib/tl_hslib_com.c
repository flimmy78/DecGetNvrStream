/******************************************************************************

	TL hslib common
	
	2008-12-16	created by kong
	2010-11-11	modify by andyrew

******************************************************************************/
#include "common.h"
#include "vio_common.h"

extern int venc_init(void);	//come form lib_venc.c
extern int unsigned long long venc_get_sys_pts(int chn, unsigned long long pts);
extern void venc_update_time(int chn);

extern unsigned char GetPlayingStatus();//csp modify 20140406

#ifdef FAST_SWITCH_PREVIEW
extern int nvr_preview_vdec_open(int vdec_first_chn, int vdec_chn_num);
#endif

unsigned char is_decoder_techwell = 0;
unsigned char is_decoder_nvp = 0;
unsigned char is_decoder_rn631x = 0;

//csp modify 20140525
unsigned char no_audio_chip = 0;

struct lib_global_info lib_gbl_info;
struct lib_global_info *plib_gbl_info = NULL;

int g_client_id = 0;

void rebootSlaveByCmd(void)
{
	return;
}

/*****************************************************************************
* function : start vpss. VPSS chn with frame
*****************************************************************************/
HI_S32 SAMPLE_COMM_VPSS_Start(HI_S32 s32FirstGrpIdx, HI_S32 s32GrpCnt, SIZE_S *pstSize, HI_S32 s32ChnCnt,VPSS_GRP_ATTR_S *pstVpssGrpAttr)
{
	VPSS_GRP VpssGrp;
	VPSS_CHN VpssChn;
	VPSS_GRP_ATTR_S stGrpAttr;
	VPSS_CHN_ATTR_S stChnAttr;
	VPSS_GRP_PARAM_S stVpssParam;
	HI_S32 s32Ret;
	HI_S32 i, j;
	
	/*** Set Vpss Grp Attr ***/
	if(NULL == pstVpssGrpAttr)
	{
		stGrpAttr.u32MaxW = pstSize->u32Width;
		stGrpAttr.u32MaxH = pstSize->u32Height;
		#ifdef HI3535
		stGrpAttr.bDciEn = HI_FALSE;
		#else
		stGrpAttr.bDrEn = HI_FALSE;
		stGrpAttr.bDbEn = HI_FALSE;
		#endif
		stGrpAttr.bIeEn = HI_FALSE;
		stGrpAttr.bNrEn = HI_FALSE;//HI_TRUE : 800*600ʱHDMI��˸
		stGrpAttr.bHistEn = HI_FALSE;
		stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;//VPSS_DIE_MODE_AUTO;
		stGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
		//printf("SAMPLE_COMM_VPSS_Start u32MaxW=%d,u32MaxH=%d\n",stGrpAttr.u32MaxW,stGrpAttr.u32MaxH);
	}
	else
	{
		memcpy(&stGrpAttr,pstVpssGrpAttr,sizeof(VPSS_GRP_ATTR_S));
	}
	
	for(i = s32FirstGrpIdx; i < s32FirstGrpIdx + s32GrpCnt; i++)
	{
		VpssGrp = i;
		
		/*** create vpss group ***/
		//#ifdef HI3531
		//s32Ret = HI_MPI_VPSS_CreatGrp(VpssGrp, &stGrpAttr);
		//#else
		s32Ret = HI_MPI_VPSS_CreateGrp(VpssGrp, &stGrpAttr);
		//#endif
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("HI_MPI_VPSS_CreateGrp%d failed with %#x!\n", VpssGrp, s32Ret);
			return HI_FAILURE;
		}
		else
		{
			//LIB_PRT("HI_MPI_VPSS_CreateGrp%d success with %#x!\n", VpssGrp, s32Ret);
		}
		
		/*** get vpss param ***/
		//#ifdef HI3531
		//s32Ret = HI_MPI_VPSS_GetParam(VpssGrp, 0, &stVpssParam);
		//#else
		s32Ret = HI_MPI_VPSS_GetGrpParam(VpssGrp, &stVpssParam);
		//#endif
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("failed with %#x!\n", s32Ret);
			//return HI_FAILURE;
		}
		
		#ifdef HI3535
		#else
		stVpssParam.u32MotionThresh = 0;
		//stVpssParam.u32NrWforTsr = 4;
		stVpssParam.u32DiStrength = 0;
		#endif
		
		/*** set vpss param ***/
		//#ifdef HI3531
		//s32Ret = HI_MPI_VPSS_SetParam(VpssGrp, 0, &stVpssParam);
		//#else
		s32Ret = HI_MPI_VPSS_SetGrpParam(VpssGrp, &stVpssParam);
		//#endif
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
		
		//csp modify
		int start_chn = 0;
		if(VpssGrp >= ARG_VI_NUM_MAX)
		{
			start_chn = VPSS_PRE0_CHN;
			s32ChnCnt = start_chn + 1;
		}
		else
		{
			//csp modify 20140406
			if(GetPlayingStatus())
			{
				//start_chn = 0;
				//s32ChnCnt = VPSS_PRE0_CHN + 1;
				start_chn = VPSS_PRE0_CHN;
				s32ChnCnt = start_chn + 1;
			}
			else
			{
				//csp modify 20140318
				start_chn = VPSS_LSTR_CHN;
				s32ChnCnt = VPSS_PRE0_CHN + 1;
			}
		}
		
		/*** enable vpss chn, with frame ***/
		for(j=start_chn; j<s32ChnCnt; j++)
		{
			VpssChn = j;
			
			s32Ret = HI_MPI_VPSS_GetChnAttr(VpssGrp, VpssChn, &stChnAttr);
			
			/* Set Vpss Chn attr */
			if((j == 0) || (j == 4))
				stChnAttr.bSpEn = HI_FALSE;
			else
				stChnAttr.bSpEn = HI_TRUE;
			
			#ifdef HI3535
			stChnAttr.bBorderEn = HI_FALSE;//HI_TRUE;
			stChnAttr.stBorder.u32Color = 0xff00;
			stChnAttr.stBorder.u32LeftWidth = 2;
			stChnAttr.stBorder.u32RightWidth = 2;
			stChnAttr.stBorder.u32TopWidth = 2;
			stChnAttr.stBorder.u32BottomWidth = 2;
			#else
			stChnAttr.bFrameEn = HI_FALSE;//HI_TRUE;
			stChnAttr.stFrame.u32Color[VPSS_FRAME_WORK_LEFT] = 0xff00;
			stChnAttr.stFrame.u32Color[VPSS_FRAME_WORK_RIGHT] = 0xff00;
			stChnAttr.stFrame.u32Color[VPSS_FRAME_WORK_BOTTOM] = 0xff00;
			stChnAttr.stFrame.u32Color[VPSS_FRAME_WORK_TOP] = 0xff00;
			stChnAttr.stFrame.u32Width[VPSS_FRAME_WORK_LEFT] = 2;
			stChnAttr.stFrame.u32Width[VPSS_FRAME_WORK_RIGHT] = 2;
			stChnAttr.stFrame.u32Width[VPSS_FRAME_WORK_TOP] = 2;
			stChnAttr.stFrame.u32Width[VPSS_FRAME_WORK_BOTTOM] = 2;
			#endif
			
			s32Ret = HI_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stChnAttr);
			if (s32Ret != HI_SUCCESS)
			{
				LIB_PRT("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
				return HI_FAILURE;
			}
			
			#if 0
			VPSS_CHN_MODE_S stVpssMode;
			s32Ret = HI_MPI_VPSS_GetChnMode(VpssGrp,VpssChn,&stVpssMode);
			if(s32Ret != HI_SUCCESS)
			{
				printf("VPSS[%d,%d] HI_MPI_VPSS_GetChnMode failed\n",VpssGrp,VpssChn);
				//return s32Ret;
			}
			
			printf("VPSS[%d,%d] stVpssMode:(%d,%d,%d,%d)\n",VpssGrp,VpssChn,stVpssMode.u32Width,stVpssMode.u32Height,stVpssMode.enChnMode,stVpssMode.bDouble);
			
			stVpssMode.enChnMode = VPSS_CHN_MODE_AUTO;
			stVpssMode.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
			stVpssMode.u32Width = pstSize->u32Width;
			stVpssMode.u32Height = pstSize->u32Height;
			stVpssMode.bDouble = HI_TRUE;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp,VpssChn,&stVpssMode);
			if(s32Ret != HI_SUCCESS)
			{
				printf("HI_MPI_VPSS_SetChnMode failed:0x%08x\n",s32Ret);
				return s32Ret;
			}
			#endif
			
			s32Ret = HI_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
			if (s32Ret != HI_SUCCESS)
			{
				LIB_PRT("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
				return HI_FAILURE;
			}
		}
		
		/*** start vpss group ***/
		s32Ret = HI_MPI_VPSS_StartGrp(VpssGrp);
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
			return HI_FAILURE;
		}
	}
	
	return HI_SUCCESS;
}

/*****************************************************************************
* function : disable vpss dev
*****************************************************************************/
HI_S32 SAMPLE_COMM_VPSS_Stop(HI_S32 s32FirstGrpIdx, HI_S32 s32GrpCnt, HI_S32 s32ChnCnt)
{
	HI_S32 i, j;
	HI_S32 s32Ret = HI_SUCCESS;
	VPSS_GRP VpssGrp;
	VPSS_CHN VpssChn;
	
	for(i = s32FirstGrpIdx; i < s32FirstGrpIdx + s32GrpCnt; i++)
	{
		VpssGrp = i;
		
		#if 0
		s32Ret = HI_MPI_VPSS_ResetGrp(VpssGrp);
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("failed with %#x!\n", s32Ret);
			//return HI_FAILURE;
		}
		#endif
		
		s32Ret = HI_MPI_VPSS_StopGrp(VpssGrp);
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
		
		for(j=0; j<s32ChnCnt; j++)
		{
			VpssChn = j;
			s32Ret = HI_MPI_VPSS_DisableChn(VpssGrp, VpssChn);
			if (s32Ret != HI_SUCCESS)
			{
				LIB_PRT("failed with %#x!\n", s32Ret);
				return HI_FAILURE;
			}
		}
		
		s32Ret = HI_MPI_VPSS_DestroyGrp(VpssGrp);
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
	}
	
	return HI_SUCCESS;
}

HI_S32 SAMPLE_EnableVpss(int firstVpssGrp, int VpssGrpcnt)
{
	VPSS_GRP VpssGrp;
	HI_S32 s32Ret;
	
	for(VpssGrp = firstVpssGrp; VpssGrp < VpssGrpcnt + firstVpssGrp; VpssGrp++)
	{
		s32Ret = HI_MPI_VPSS_StartGrp(VpssGrp);
		if (s32Ret != HI_SUCCESS)
		{
			LIB_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
			return HI_FAILURE;
		}
	}
	
	return HI_SUCCESS;
}

HI_S32 SAMPLE_StartVpss(SIZE_S size, int chncnt)
{
	SIZE_S stSize;
	HI_S32 s32Ret;
	
	int i;
	for(i = 0; i < chncnt; i++)
	{
		stSize.u32Width = size.u32Width;
		stSize.u32Height = size.u32Height;
		
		s32Ret = SAMPLE_COMM_VPSS_Start(i, 1, &stSize, 5, NULL);
		if(HI_SUCCESS != s32Ret)
		{
			LIB_PRT("Start Vpss failed!\n");
			return HI_FAILURE;
		}
	}
	
	return HI_SUCCESS;
}

static HI_S32 SAMPLE_InitMPP(void)
{
	VB_CONF_S stVbConf;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32AlignWidth = 16;
	MPP_SYS_CONF_S stSysConf = {0};
	
	/******************************************
	step  1: init global  variable 
	******************************************/
	memset(&stVbConf,0,sizeof(VB_CONF_S));
	
	if(TL_BOARD_TYPE_NR2116 == TL_HSLIB_TYPE)
	{
	#if 0//#ifdef HI3535
		printf("NR3516 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 2;
		stVbConf.astCommPool[0].u32BlkSize = 1920*1200*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 8;
		stVbConf.astCommPool[1].u32BlkSize = 768*576*3/2;
		stVbConf.astCommPool[1].u32BlkCnt = 64;
	#else
		printf("NR2116 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	#endif
	}
	else if(TL_BOARD_TYPE_NR3116 == TL_HSLIB_TYPE)
	{
		printf("NR3116 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	else if(TL_BOARD_TYPE_NR3132 == TL_HSLIB_TYPE)
	{
		printf("NR3132 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	else if(TL_BOARD_TYPE_NR3124 == TL_HSLIB_TYPE)
	{
		printf("NR3124 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	else if(TL_BOARD_TYPE_NR1004 == TL_HSLIB_TYPE)
	{
		printf("NR1004 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	else if(TL_BOARD_TYPE_NR1008 == TL_HSLIB_TYPE)
	{
		printf("NR1008 config$$$$$$$$$$$$$$$$$$$$$$$$\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	else
	{
		printf("unknown nvr board type\n");
		stVbConf.u32MaxPoolCnt = 4;
		stVbConf.astCommPool[0].u32BlkSize = 720*576*3/2;
		stVbConf.astCommPool[0].u32BlkCnt = 4;
	}
	
	/******************************************
	step 2: mpp system init. 
	******************************************/
	HI_MPI_SYS_Exit();
	HI_MPI_VB_Exit();
	
	s32Ret = HI_MPI_VB_SetConf(&stVbConf);
	if (HI_SUCCESS != s32Ret)
	{
	    LIB_PRT("HI_MPI_VB_SetConf failed!\n");
	    return HI_FAILURE;
	}
	
	s32Ret = HI_MPI_VB_Init();
	if (HI_SUCCESS != s32Ret)
	{
	    LIB_PRT("HI_MPI_VB_Init failed!\n");
	    return HI_FAILURE;
	}
	
	stSysConf.u32AlignWidth = u32AlignWidth;
	s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
	if (HI_SUCCESS != s32Ret)
	{
	    LIB_PRT("HI_MPI_SYS_SetConf failed\n");
	    return HI_FAILURE;
	}
	
	s32Ret = HI_MPI_SYS_Init();
	if (HI_SUCCESS != s32Ret)
	{
	    LIB_PRT("HI_MPI_SYS_Init failed!\n");
	    return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

#include "lib_vdec.h"

typedef struct
{
	int chn;
	venc_frame_type_e type;
	unsigned long long pts;
	unsigned int len;
	unsigned short width;
	unsigned short height;
	unsigned long long local_pts;
}SVencFrameHeader;

extern unsigned int getTimeStamp();

int WriteFrameToPreviewQueue(real_stream_s *stream)
{
	SVencFrameHeader header;
	unsigned int t;
	int queue_chn;//��������һ�����У�������������ͬ
	queue_chn = stream->chn < ARG_CHN_MAX ? stream->chn : (stream->chn - ARG_CHN_MAX);

	t = getTimeStamp(); //ms
	/*
	if (stream->chn < 16)
	{
		printf("%s pts: %u\n", __func__, t);

	}
	*/
	header.local_pts = t;
	header.local_pts *= 1000; //us
	header.chn = stream->chn;
	header.type = stream->frame_type;
	header.pts = stream->pts;
	header.len = stream->len;
	header.width = stream->width;
	header.height = stream->height;
	
	pthread_mutex_lock(&plib_gbl_info->preview_queue_lock[queue_chn]);
	
	s32 ret = WriteDataToBuf(&plib_gbl_info->preview_frame_queue[queue_chn], (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock[queue_chn]);
		return -1;
	}
	ret = WriteDataToBuf(&plib_gbl_info->preview_frame_queue[queue_chn], stream->data, stream->len);
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [2]\n", __FUNCTION__);
		ResumeCircleBufToPast(&plib_gbl_info->preview_frame_queue[queue_chn], 1, 0);
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock[queue_chn]);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock[queue_chn]);
	
	return 0;
}

int WriteFrameToVencMainQueue(real_stream_s *stream)
{
	#if 1
	venc_stream_s out_stream;
	out_stream.chn = stream->chn;
	out_stream.data = stream->data;
	out_stream.len = stream->len;
	out_stream.pts = venc_get_sys_pts(stream->chn, stream->pts);
	out_stream.type = stream->frame_type;
	out_stream.rsv = 0;//stream->rsv;
	out_stream.width = stream->width;
	out_stream.height = stream->height;
	
	if(plib_gbl_info->pMainStreamCB != NULL)
	{
		plib_gbl_info->pMainStreamCB(&out_stream);
	}
	#else
	SVencFrameHeader header;
	header.chn = stream->chn;
	header.type = stream->type;
	header.pts = stream->pts;
	header.len = stream->len;
	header.width = stream->width;
	header.height = stream->height;
	
	pthread_mutex_lock(&plib_gbl_info->venc_main_queue_lock);
	
	s32 ret = WriteDataToBuf(&plib_gbl_info->venc_main_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		if(stream->chn == 0) fprintf(stderr, "%s: chn%d write header failed\n", __FUNCTION__, stream->chn);
		pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
		return -1;
	}
	ret = WriteDataToBuf(&plib_gbl_info->venc_main_frame_queue, stream->data, stream->len);
	if(0 != ret)
	{
		if(stream->chn == 0) fprintf(stderr, "%s: chn%d write data failed\n", __FUNCTION__, stream->chn);
		ResumeCircleBufToPast(&plib_gbl_info->venc_main_frame_queue, 1, 0);
		pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
	#endif
	
	return 0;
}

int WriteFrameToVencSubQueue(real_stream_s *stream)
{
	#if 1
	venc_stream_s out_stream;
	out_stream.chn = stream->chn - ARG_VI_NUM_MAX;
	out_stream.data = stream->data;
	out_stream.len = stream->len;
	out_stream.pts = stream->pts;
	out_stream.type = stream->frame_type;
	out_stream.rsv = 0;//stream->rsv;
	out_stream.width = stream->width;
	out_stream.height = stream->height;
	
	//csp modify 20140318
	#ifdef P2P_SUB_STREAM
	if(plib_gbl_info->venc[out_stream.chn].venc_min_start)
	{
		if(plib_gbl_info->pSubStreamCB != NULL)
		{
			plib_gbl_info->pSubStreamCB(&out_stream);
		}
	}
	//csp modify 20140406
	//printf("WriteFrameToVencSubQueue chn=%d venc_third_start=%d,last_pre_mode=%d,frametype=%d\n",out_stream.chn,plib_gbl_info->venc[out_stream.chn].venc_third_start,plib_gbl_info->last_pre_mode,out_stream.type);
	if(plib_gbl_info->venc[out_stream.chn].venc_third_start && plib_gbl_info->last_pre_mode == PREVIEW_CLOSE/* && out_stream.type == FRAME_TYPE_I*/)
	{
		//printf("chn%d third stream here...\n",out_stream.chn);
		if(plib_gbl_info->pThirdStreamCB != NULL)
		{
			plib_gbl_info->pThirdStreamCB(&out_stream);
		}
	}
	#else
	if(plib_gbl_info->pSubStreamCB != NULL)
	{
		plib_gbl_info->pSubStreamCB(&out_stream);
	}
	#endif
	#else
	SVencFrameHeader header;
	header.chn = stream->chn - ARG_VI_NUM_MAX;
	header.type = stream->type;
	header.pts = stream->pts;
	header.len = stream->len;
	header.width = stream->width;
	header.height = stream->height;
	
	pthread_mutex_lock(&plib_gbl_info->venc_sub_queue_lock);
	
	s32 ret = WriteDataToBuf(&plib_gbl_info->venc_sub_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
		return -1;
	}
	ret = WriteDataToBuf(&plib_gbl_info->venc_sub_frame_queue, stream->data, stream->len);
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [2]\n", __FUNCTION__);
		ResumeCircleBufToPast(&plib_gbl_info->venc_sub_frame_queue, 1, 0);
		pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
	#endif
	
	return 0;
}

s32 ReadFrameFromPreviewQueue(unsigned char *in_buf, unsigned int in_len, real_stream_s *stream)
{
	if((in_buf == NULL) || (in_len <= 0) || (stream == NULL))
	{
		fprintf(stderr,"Invalid parameter for function %s !\n", __FUNCTION__);
		return -1;
	}
	
	SVencFrameHeader header;
	memset(&header, 0, sizeof(header));
	
	pthread_mutex_lock(&plib_gbl_info->preview_queue_lock);
	
	s32 ret = 0;
	ret = ReadDataFromBuf(&plib_gbl_info->preview_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
		return -1;
	}
	
	//printf("chn%d frame len:%u\n", header.chn, header.len);
	
	if(header.len > in_len)
	{
		ret = SkipCircleBuf(&plib_gbl_info->preview_frame_queue, 0, 1, header.len);
		if(0 != ret)
		{
			ResumeCircleBufToPast(&plib_gbl_info->preview_frame_queue, 0, 1);
			pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
			return -1;
		}
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
		return -1;
	}
	
	ret = ReadDataFromBuf(&plib_gbl_info->preview_frame_queue, in_buf, header.len);
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [2]\n", __FUNCTION__);
		ResumeCircleBufToPast(&plib_gbl_info->preview_frame_queue, 0, 1);
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
	
	stream->chn = header.chn;
	stream->data = in_buf;
	stream->len = header.len;
	stream->pts = header.pts;
	stream->frame_type = header.type;
	
	return 0;
}

s32 ReadFrameFromVencMainQueue(unsigned char *in_buf, unsigned int in_len, real_stream_s *stream)
{
	#if 1
	return -1;
	#else
	if((in_buf == NULL) || (in_len <= 0) || (stream == NULL))
	{
		fprintf(stderr,"Invalid parameter for function %s !\n", __FUNCTION__);
		return -1;
	}
	
	SVencFrameHeader header;
	memset(&header, 0, sizeof(header));
	
	pthread_mutex_lock(&plib_gbl_info->venc_main_queue_lock);
	
	s32 ret = 0;
	ret = ReadDataFromBuf(&plib_gbl_info->venc_main_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
		return -1;
	}
	
	//printf("chn%d frame len:%u\n", header.chn, header.len);
	
	if(header.len > in_len)
	{
		fprintf(stderr, "%s: buffer overflow,skip...\n", __FUNCTION__);
		ret = SkipCircleBuf(&plib_gbl_info->venc_main_frame_queue, 0, 1, header.len);
		if(0 != ret)
		{
			fprintf(stderr, "%s: skip buffer failed\n", __FUNCTION__);
			ResumeCircleBufToPast(&plib_gbl_info->venc_main_frame_queue, 0, 1);
			pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
			return -1;
		}
		pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
		return -1;
	}
	
	ret = ReadDataFromBuf(&plib_gbl_info->venc_main_frame_queue, in_buf, header.len);
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [2]\n", __FUNCTION__);
		ResumeCircleBufToPast(&plib_gbl_info->venc_main_frame_queue, 0, 1);
		pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->venc_main_queue_lock);
	
	stream->chn = header.chn;
	stream->data = in_buf;
	stream->len = header.len;
	stream->pts = header.pts;
	stream->type = header.type;
	
	return 0;
	#endif
}

s32 ReadFrameFromVencSubQueue(unsigned char *in_buf, unsigned int in_len, real_stream_s *stream)
{
	#if 1
	return -1;
	#else
	if((in_buf == NULL) || (in_len <= 0) || (stream == NULL))
	{
		fprintf(stderr,"Invalid parameter for function %s !\n", __FUNCTION__);
		return -1;
	}
	
	SVencFrameHeader header;
	memset(&header, 0, sizeof(header));
	
	pthread_mutex_lock(&plib_gbl_info->venc_sub_queue_lock);
	
	s32 ret = 0;
	ret = ReadDataFromBuf(&plib_gbl_info->venc_sub_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
		return -1;
	}
	
	//printf("chn%d frame len:%u\n", header.chn, header.len);
	
	if(header.len > in_len)
	{
		ret = SkipCircleBuf(&plib_gbl_info->venc_sub_frame_queue, 0, 1, header.len);
		if(0 != ret)
		{
			ResumeCircleBufToPast(&plib_gbl_info->venc_sub_frame_queue, 0, 1);
			pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
			return -1;
		}
		pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
		return -1;
	}
	
	ret = ReadDataFromBuf(&plib_gbl_info->venc_sub_frame_queue, in_buf, header.len);
	if(0 != ret)
	{
		fprintf(stderr, "function: %s [2]\n", __FUNCTION__);
		ResumeCircleBufToPast(&plib_gbl_info->venc_sub_frame_queue, 0, 1);
		pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
		return -1;
	}
	
	pthread_mutex_unlock(&plib_gbl_info->venc_sub_queue_lock);
	
	stream->chn = header.chn;
	stream->data = in_buf;
	stream->len = header.len;
	stream->pts = header.pts;
	stream->type = header.type;
	
	return 0;
	#endif
}

extern unsigned char is_nvr_preview_chn_open(int chn);
extern int nvr_preview_vdec_write(int chn, vdec_stream_s *pin_stream);

int GetPreviewQueueFirstFramePTS(unsigned long long *pIpcPts, unsigned long long *pLocalPts, int *pChn) // us
{
	s32 ret = 0;
	SVencFrameHeader header;
	memset(&header, 0, sizeof(header));
	
	pthread_mutex_lock(&plib_gbl_info->preview_queue_lock);
	
	ret = ReadDataFromBuf(&plib_gbl_info->preview_frame_queue, (u8 *)&header, sizeof(SVencFrameHeader));
	if(0 != ret)
	{
		//if (EM_REC_BUF_NO_DATA != ret)
			fprintf(stderr, "function: %s [1]\n", __FUNCTION__);
		
		pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
		return -1;
	}

	*pIpcPts = header.pts;
	*pLocalPts = header.local_pts;
	*pChn = header.chn;
	
	ResumeCircleBufToPast(&plib_gbl_info->preview_frame_queue, 0, 1);

	pthread_mutex_unlock(&plib_gbl_info->preview_queue_lock);
	return 0;
}

static volatile unsigned int PrePtsChn[32] = {0};
static unsigned int cnt, yg_pts;

void *NVRPreviewFxn(void *arg)
{
	printf("NVRPreviewFxn, pid:%ld\n", pthread_self());
	
	unsigned char FrameBuf[768*1024];
	real_stream_s stream;
	vdec_stream_s in_stream;
	unsigned long long IpcPts, LocalPts;
	unsigned int cur_pts;
	int chn, sendflag;
	int ret;
	
	while(1)
	{
		//��Ҫ����:������Ӳ�̸�ʽ����������ͻ
		if(plib_gbl_info->preview_frame_queue.nLenUsed <= 0)
		{
			usleep(1);
			continue;
		}

		sendflag = 0;
		//while (1)//�����е�һ֡��ʱ���+ 200ms < ��ǰʱ��
		{
			cur_pts = getTimeStamp(); //ms
			if (GetPreviewQueueFirstFramePTS(&IpcPts, &LocalPts, &chn) == 0)
			{
				//Ԥ����ʾ200ms֮ǰ��֡
				//printf("%s local pts: %u, cur pts: %u\n", __func__, pts, cur_pts);
				IpcPts = IpcPts/1000;
				LocalPts = LocalPts/1000;

				
				
				if (LocalPts+500 > cur_pts)
				{
					//break;
					usleep(1);
					continue;
				}

				if (PrePtsChn[chn] == 0)
				{
					PrePtsChn[chn] = cur_pts;
					sendflag = 1;

					if (chn == 0)
					{
						yg_pts = cur_pts;
						cnt = 0;
					}
				}
				else
				{
					if ( (chn == 0) && (cur_pts >= (yg_pts+1000)) )
					{
						printf("%s chn0 send preview frame count: %d\n", __func__, cnt);
						yg_pts = cur_pts;
						cnt = 0;
					}
					
					if (cur_pts >= (PrePtsChn[chn] + 40))
					{
						//PrePtsChn[chn] += 40;
						PrePtsChn[chn] = cur_pts;
						sendflag = 1;

						if (chn == 0)
						{
							cnt++;
						}
					}
					else
					{
						usleep(1);
						continue;
					}					
				}

				if (sendflag)
				{
					do{
						memset(&stream, 0, sizeof(stream));

						ret = ReadFrameFromPreviewQueue(FrameBuf, sizeof(FrameBuf), &stream);
						if(0 != ret)
						{
						usleep(1);
						//break;
						continue;
						}

						memset(&in_stream, 0, sizeof(in_stream));
						in_stream.rsv = 0;
						in_stream.pts = stream.pts;
						in_stream.data = stream.data;
						in_stream.len = stream.len;
						nvr_preview_vdec_write(stream.chn, &in_stream);
					} while (stream.len < 50);
				}
			}
			else
			{
				usleep(1);
			}
		}
#if 0
		if (GetPreviewQueueFirstFramePTS(&pts) == 0)
		{
			printf("%s pts: %u\n", __func__, pts);
			
		}
		
		real_stream_s stream;
		memset(&stream, 0, sizeof(stream));
		
		int ret = ReadFrameFromPreviewQueue(FrameBuf, sizeof(FrameBuf), &stream);
		if(0 != ret)
		{
			usleep(1);
			continue;
		}
		
		vdec_stream_s in_stream;
		in_stream.rsv = 0;
		in_stream.pts = stream.pts;
		in_stream.data = stream.data;
		in_stream.len = stream.len;
		nvr_preview_vdec_write(stream.chn, &in_stream);
		
		/*if(stream.chn == ARG_VI_NUM_MAX)
		{
			int i;
			for(i=1;i<ARG_VI_NUM_MAX;i++)
			{
				nvr_preview_vdec_write(ARG_VI_NUM_MAX+i, &in_stream);
			}
		}*/
#endif

	}
	
	return 0;
}

int DealStreamState(int chn, real_stream_state_e msg)
{
	
	if(chn < 0 || chn >= (ARG_VI_NUM_MAX*2))
	{
		return -1;
	}
	
	if(chn >= ARG_VI_NUM_MAX)
	{
		chn -= ARG_VI_NUM_MAX;
		
		switch(msg)
		{
		case REAL_STREAM_STATE_STOP:
		case REAL_STREAM_STATE_LOST:
			printf("chn%d sub stream lost\n",chn+ARG_VI_NUM_MAX);
			plib_gbl_info->bySubWaitIFrame[chn] = 1;
			plib_gbl_info->bySubStreamOpen[chn] = 0;
			unsigned char ret = is_nvr_preview_chn_open(chn);
			if(ret == 2)//ͨ��Ԥ��ʹ��������
			{
				//yaogang modify 20140918
				pthread_mutex_lock(&plib_gbl_info->preview_chn_lock[chn]);
				HI_MPI_VDEC_StopRecvStream(chn);
				HI_MPI_VDEC_ResetChn(chn);
				HI_MPI_VDEC_StartRecvStream(chn);
				pthread_mutex_unlock(&plib_gbl_info->preview_chn_lock[chn]);
				
				#ifdef FAST_SWITCH_PREVIEW
				VO_CHN VoChn = chn;
				#else
				VO_CHN VoChn = chn-plib_gbl_info->last_mode_arg;
				#endif
				//HI_MPI_VO_ChnRefresh(VO_DEVICE_HD, VoChn);
				//HI_MPI_VO_ChnHide(VO_DEVICE_HD, VoChn);
				//HI_MPI_VO_ChnShow(VO_DEVICE_HD, VoChn);
				int i = 0;
				for(i=0;i<1;i++)
				{
					//HI_S32 s32Ret = HI_MPI_VO_ClearChnBuffer(VO_DEVICE_HD, VoChn, HI_TRUE);
					//printf("VoChn:%d clear result:%d\n",VoChn,s32Ret);
					//printf("VoChn: %d, chn: %d msg:%d\n",VoChn, chn, msg);
					HI_MPI_VO_ClearChnBuffer(VO_DEVICE_HD, VoChn, HI_TRUE);
					//if(i == 0) usleep(40*1000);
				}
		
			}
			break;
		case REAL_STREAM_STATE_START:
		case REAL_STREAM_STATE_LINK:
			//printf("chn%d sub stream link\n",chn);
			if(IPC_GetLinkStatus(chn+ARG_VI_NUM_MAX))
			{
				plib_gbl_info->bySubStreamOpen[chn] = 1;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch(msg)
		{
		case REAL_STREAM_STATE_STOP:
		case REAL_STREAM_STATE_LOST:
			printf("chn%d main stream lost\n",chn);
			plib_gbl_info->byMainWaitIFrame[chn] = 1;
			plib_gbl_info->byMainStreamOpen[chn] = 0;
			venc_update_time(chn);
			unsigned char ret = is_nvr_preview_chn_open(chn);
			if(ret == 1)//ͨ��Ԥ��ʹ��������
			{
				pthread_mutex_lock(&plib_gbl_info->preview_chn_lock[chn]);
				HI_MPI_VDEC_StopRecvStream(chn);
				HI_MPI_VDEC_ResetChn(chn);
				HI_MPI_VDEC_StartRecvStream(chn);
				pthread_mutex_unlock(&plib_gbl_info->preview_chn_lock[chn]);
				
				#ifdef FAST_SWITCH_PREVIEW
				VO_CHN VoChn = chn;
				#else
				VO_CHN VoChn = chn-plib_gbl_info->last_mode_arg;
				#endif
				//HI_MPI_VO_ChnRefresh(VO_DEVICE_HD, VoChn);
				//HI_MPI_VO_ChnHide(VO_DEVICE_HD, VoChn);
				//HI_MPI_VO_ChnShow(VO_DEVICE_HD, VoChn);
				int i = 0;
				for(i=0;i<1;i++)
				{
					//HI_MPI_VO_ClearChnBuffer(VO_DEVICE_HD, VoChn, HI_TRUE);
					//if(i == 0) usleep(40*1000);
					//printf("VoChn: %d, chn: %d msg:%d\n",VoChn, chn, msg);
					HI_MPI_VO_ClearChnBuffer(VO_DEVICE_HD, VoChn, HI_TRUE);
				}
				
			}
			break;
		case REAL_STREAM_STATE_START:
		case REAL_STREAM_STATE_LINK:
			//printf("chn%d main stream link\n",chn);
			venc_update_time(chn);
			if(IPC_GetLinkStatus(chn))
			{
				plib_gbl_info->byMainStreamOpen[chn] = 1;
			}
			break;
		default:
			break;
		}
	}
	
	return 0;
}

extern int DealRealAudio(real_stream_s *stream, unsigned int dwContext);

int DealRealStream(real_stream_s *stream, unsigned int dwContext)
{
	if(stream == NULL || stream->chn != dwContext)
	{
		printf("DealRealStream: param error\n");
		return -1;
	}
	
	//printf("###chn%d real data...\n", stream->chn);
	
	if(stream->media_type != MEDIA_PT_H264)
	{
		#ifdef HI3535
			return DealRealAudio(stream, dwContext);
			//return 0;
		#else
			return 0;//yg modify 20140913
		#endif
	}
	
	//printf("###chn%d real video data...\n", stream->chn);
	
	#if 1
	int channel = stream->chn;
	
	/*if(channel == 8 || channel == 24)
	{
		if(channel == 8)
		{
			plib_gbl_info->main_frame_count[8]++;
			printf("chn%d frames:%d size:%d\n",channel,plib_gbl_info->main_frame_count[8],stream->len);
		}
		else
		{
			plib_gbl_info->sub_frame_count[8]++;
			printf("chn%d frames:%d size:%d\n",channel,plib_gbl_info->sub_frame_count[8],stream->len);
		}
		//printf("chn%d size:%d\n",channel,stream->len);
		return 0;
	}*/
	
	if(channel >= ARG_VI_NUM_MAX)
	{
		plib_gbl_info->sub_frame_count[channel-ARG_VI_NUM_MAX]++;
		
		if(plib_gbl_info->sub_stream_size[channel-ARG_VI_NUM_MAX].u32Width != stream->width)
		{
			plib_gbl_info->sub_stream_size[channel-ARG_VI_NUM_MAX].u32Width = stream->width;
		}
		if(plib_gbl_info->sub_stream_size[channel-ARG_VI_NUM_MAX].u32Height != stream->height)
		{
			plib_gbl_info->sub_stream_size[channel-ARG_VI_NUM_MAX].u32Height = stream->height;
		}
		
		//csp modify 20140406
		//printf("DealRealStream chn=%d venc_third_start=%d,last_pre_mode=%d,frametype=%d\n",channel-ARG_VI_NUM_MAX,plib_gbl_info->venc[channel-ARG_VI_NUM_MAX].venc_third_start,plib_gbl_info->last_pre_mode,stream->frame_type);
		if(plib_gbl_info->venc[channel-ARG_VI_NUM_MAX].venc_min_start || (plib_gbl_info->venc[channel-ARG_VI_NUM_MAX].venc_third_start && plib_gbl_info->last_pre_mode == PREVIEW_CLOSE))
		{
			//if(channel == 19) printf("DealRealStream:chn%d sub stream write to queue\n",channel-ARG_VI_NUM_MAX);
			if(WriteFrameToVencSubQueue(stream) < 0)
			{
				//do nothing
			}
		}
		
		/*if(channel == 16)
		{
			printf("chn%d frames:%d size:%d\n",channel,plib_gbl_info->sub_frame_count[channel-ARG_VI_NUM_MAX],stream->len);
			
			static int saved = 0;
			if(!saved)
			{
				static FILE *fp = NULL;
				if(fp == NULL)
				{
					//fp = fopen("/root/myusb/longse.h264","wb");
					fp = fopen("/mnt/longse.h264","wb");
				}
				if(fp != NULL)
				{
					fwrite(stream->data, stream->len, 1, fp);
					if(ftell(fp) > (2<<20))
					{
						printf("save stream over\n");
						
						fclose(fp);
						fp = NULL;
						
						saved = 1;
					}
				}
			}
		}*/
	}
	else
	{
		plib_gbl_info->main_frame_count[channel]++;
		
		if(plib_gbl_info->main_stream_size[channel].u32Width != stream->width)
		{
			plib_gbl_info->main_stream_size[channel].u32Width = stream->width;
		}
		if(plib_gbl_info->main_stream_size[channel].u32Height != stream->height)
		{
			plib_gbl_info->main_stream_size[channel].u32Height = stream->height;
		}
		
		plib_gbl_info->mdevent[channel] = stream->mdevent;
		if(stream->mdevent)
		{
			//printf("chn%d get md event\n",channel);
		}
		
		if(plib_gbl_info->venc[channel].start)
		{
			if(WriteFrameToVencMainQueue(stream) < 0)
			{
				//usleep(1);
			}
		}
		
		//if((channel == 0) && (plib_gbl_info->main_frame_count[channel] % 1500) == 0)
		//{
		//	printf("chn%d frames:%d time:%ld\n",channel,plib_gbl_info->main_frame_count[channel],time(NULL));
		//}
		
		/*if(channel == 3)
		{
			printf("chn%d frames:%d size:%d\n",channel,plib_gbl_info->main_frame_count[channel],stream->len);
			
			static int saved = 0;
			if(!saved)
			{
				static FILE *fp = NULL;
				if(fp == NULL)
				{
					fp = fopen("/mnt/ipc.h264","wb");
				}
				if(fp != NULL)
				{
					fwrite(stream->data, stream->len, 1, fp);
					if(ftell(fp) > (2<<20))
					{
						printf("save stream over\n");
						
						fclose(fp);
						fp = NULL;
						
						saved = 1;
					}
				}
			}
		}*/
	}
	
	//������
	int real_chn = stream->chn;
	if(real_chn >= ARG_VI_NUM_MAX)
	{
		real_chn -= ARG_VI_NUM_MAX;
	}
	
	unsigned char ret = is_nvr_preview_chn_open(real_chn);
	if(!ret)
	{
		//csp modify 20140318
		#ifdef P2P_SUB_STREAM
		if(plib_gbl_info->venc[real_chn].venc_third_start)
		{
			if(plib_gbl_info->last_pre_mode != PREVIEW_CLOSE)
			{
				
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		#else
		return 0;
		#endif
	}
	else if(ret == 1 && stream->chn >= ARG_VI_NUM_MAX)
	{
		#ifdef FAST_SWITCH_PREVIEW
		//return 0;
		#else
		return 0;
		#endif
	}
	else if(ret == 2 && stream->chn < ARG_VI_NUM_MAX)
	{
		#ifdef FAST_SWITCH_PREVIEW
		//return 0;
		#else
		return 0;
		#endif
	}
	
	return WriteFrameToPreviewQueue(stream);
	
	vdec_stream_s in_stream;
	in_stream.rsv = 0;
	in_stream.pts = stream->pts;
	in_stream.data = stream->data;
	in_stream.len = stream->len;
	
	pthread_mutex_lock(&plib_gbl_info->preview_chn_lock[real_chn]);
	
	int rtn = nvr_preview_vdec_write(stream->chn, &in_stream);
	
	pthread_mutex_unlock(&plib_gbl_info->preview_chn_lock[real_chn]);
	
	return rtn;
	
	/*nvr_preview_vdec_write(stream->chn, &in_stream);
	if(stream->chn == ARG_VI_NUM_MAX)
	{
		int i;
		for(i=1;i<ARG_VI_NUM_MAX;i++)
		{
			nvr_preview_vdec_write(ARG_VI_NUM_MAX+i, &in_stream);
		}
	}
	return 0;*/
	#else
	vdec_stream_s in_stream;
	in_stream.rsv = 0;
	in_stream.pts = stream->pts;
	in_stream.data = stream->data;
	in_stream.len = stream->len;
	return nvr_preview_vdec_write(stream->chn, &in_stream);
	#endif
}

#ifdef HI3531
static HI_S32 SAMPLE_COMM_VO_MemConfig(VO_DEV VoDev, HI_CHAR *pcMmzName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stMppChnVO;
	
    /* config vo dev */
    stMppChnVO.enModId  = HI_ID_VOU;
    stMppChnVO.s32DevId = VoDev;
    stMppChnVO.s32ChnId = 0;
    s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVO, pcMmzName);
    if(s32Ret)
    {
        LIB_PRT("HI_MPI_SYS_SetMemConf ERR!\n");
        return HI_FAILURE;
    } 
    
    return HI_SUCCESS;
}
#endif

HI_S32 VPSS_VO_Memconfig(void)
{
#ifdef HI3531
	HI_S32 i, s32Ret;
	MPP_CHN_S stMppChnVpss;
	HI_CHAR * pcMmzName;
	
	// init vpss mem
	for(i=0;i<64;i++)
	{
		stMppChnVpss.enModId  = HI_ID_VPSS;
		stMppChnVpss.s32DevId = i;
		stMppChnVpss.s32ChnId = 0;
		
		if(0 == (i%2))
		{
			pcMmzName = "ddr1";
		}
		else
		{
			pcMmzName = "ddr1";
		}
		
		/*vpss*/
		s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVpss, pcMmzName);
		if(HI_SUCCESS != s32Ret)
		{
			LIB_PRT("Vpss HI_MPI_SYS_SetMemConf ERR!\n");
			return HI_FAILURE;
		}
	}
	
	// init vo mem
	s32Ret = SAMPLE_COMM_VO_MemConfig(SAMPLE_VO_DEV_DHD0, "ddr1");
	if(HI_SUCCESS != s32Ret)
	{
		LIB_PRT("SAMPLE_COMM_VO_MemConfig failed with %d!\n", s32Ret);
		return HI_FAILURE;
	}
	s32Ret = SAMPLE_COMM_VO_MemConfig(SAMPLE_VO_DEV_DHD1, "ddr1");
	if(HI_SUCCESS != s32Ret)
	{
		LIB_PRT("SAMPLE_COMM_VO_MemConfig failed with %d!\n", s32Ret);
		return HI_FAILURE;
	}
	s32Ret = SAMPLE_COMM_VO_MemConfig(SAMPLE_VO_DEV_DSD0, "ddr1");
	if(HI_SUCCESS != s32Ret)
	{
		LIB_PRT("SAMPLE_COMM_VO_MemConfig failed with %d!\n", s32Ret);
		return HI_FAILURE;
	}
#endif
	
	return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VDEC_MemConfig(HI_VOID)
{
	HI_S32 i = 0;
	HI_S32 s32Ret = HI_SUCCESS;
	
	HI_CHAR * pcMmzName;
	MPP_CHN_S stMppChnVDEC;
	
	/* VDEC chn max is 32*/
	for(i=0;i<32;i++)
	{
		stMppChnVDEC.enModId = HI_ID_VDEC;
		stMppChnVDEC.s32DevId = 0;
		stMppChnVDEC.s32ChnId = i;
		
		if(0 == (i%2))
		{
			pcMmzName = NULL;
		}
		else
		{
			pcMmzName = "ddr1";
		}
		
		s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVDEC,pcMmzName);
		if(s32Ret)
		{
			LIB_PRT("VDEC%d HI_MPI_SYS_SetMemConf failed with %#x!\n", i, s32Ret);
			return HI_FAILURE;
		}
	}
	
	return HI_SUCCESS;
}

void tl_hslib_init(tl_board_type_e type)
{
	HI_S32 s32Ret;
	int chn;
	//SIZE_S stSize;
	
	printf("hslib build in %s\n\n", __DATE__" "__TIME__);
	
	char *b = NULL;
	if(type == TL_BOARD_TYPE_NR2116)
	{
		b = "NR2116";
	}
	else if(type == TL_BOARD_TYPE_NR3116)
	{
		b = "NR3116";
	}
	else if(type == TL_BOARD_TYPE_NR3132)
	{
		b = "NR3132";
	}
	else if(type == TL_BOARD_TYPE_NR3124)
	{
		b = "NR3124";
	}
	else if(type == TL_BOARD_TYPE_NR1004)
	{
		b = "NR1004";
	}
	else if(type == TL_BOARD_TYPE_NR1008)
	{
		b = "NR1008";
	}
	else
	{
		printf("\n<ERROR>hslib_init fail!!!!!!!!!!!!! system will not work, not support board type=%d\n", type);
		
		while(1)
		{
			sleep(1);
		}
	}
	printf("running in hs35xx board(%s)\n", b);
	
	memset(&lib_gbl_info, 0x00, sizeof(struct lib_global_info));
	plib_gbl_info = &lib_gbl_info;
	lib_gbl_info.is_pal = 1;//default is PAL
	TL_HSLIB_TYPE = type;
	
	//csp modify
	lib_gbl_info.last_pre_mode = PREVIEW_CLOSE;
	lib_gbl_info.last_mode_arg = 0;
	
	//csp modify
	s32 ret;
	pthread_t pid;
	
	//csp modify
	for (chn=0; chn < ARG_CHN_MAX; chn++)
	{
		pthread_mutex_init(&lib_gbl_info.preview_queue_lock[chn], NULL);
		lib_gbl_info.preview_frame_queue[chn].nBufId = 0;
		lib_gbl_info.preview_frame_queue[chn].nLength = 3*1024*1024;
		ret = CreateCircleBuf(&lib_gbl_info.preview_frame_queue[chn]);
		if(0 != ret)
		{
			//fprintf(stderr, "<ERROR-1>hslib_init CreateCircleBuf fail!!!!!!!!!!!!!\n");
			fprintf(stderr, "<ERROR-1>hslib_init CreateCircleBuf chn%d fail!!!!!!!!!!!!!\n", chn);
			
			while(1)
			{
				sleep(1);
			}
		}
	}
	//csp modify
	pthread_mutex_init(&lib_gbl_info.venc_main_queue_lock, NULL);
	pthread_mutex_init(&lib_gbl_info.venc_sub_queue_lock, NULL);
	//lib_gbl_info.venc_main_frame_queue.nBufId = 0;
	//lib_gbl_info.venc_main_frame_queue.nLength = 3*1024*1024;
	//ret = CreateCircleBuf(&lib_gbl_info.venc_main_frame_queue);
	//if(0 != ret)
	//{
	//	fprintf(stderr, "<ERROR-2>hslib_init CreateCircleBuf fail!!!!!!!!!!!!!\n");
	//	
	//	while(1)
	//	{
	//		sleep(1);
	//	}
	//}
	//lib_gbl_info.venc_sub_frame_queue.nBufId = 0;
	//lib_gbl_info.venc_sub_frame_queue.nLength = (1*1024*1024)/2;
	//ret = CreateCircleBuf(&lib_gbl_info.venc_sub_frame_queue);
	//if(0 != ret)
	//{
	//	fprintf(stderr, "<ERROR-3>hslib_init CreateCircleBuf fail!!!!!!!!!!!!!\n");
	//	
	//	while(1)
	//	{
	//		sleep(1);
	//	}
	//}
	
	//csp modify
	pthread_create(&pid, NULL, NVRPreviewFxn, NULL);
	
	//csp modify
	if(TL_BOARD_TYPE_NR2116 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 1;//����1��������ʹ��������Ԥ��
	}
	else if(TL_BOARD_TYPE_NR3116 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 4;//����4��������ʹ��������Ԥ��
	}
	else if(TL_BOARD_TYPE_NR3132 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 4;//����4��������ʹ��������Ԥ��
	}
	else if(TL_BOARD_TYPE_NR3124 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 4;//����4��������ʹ��������Ԥ��
	}
	else if(TL_BOARD_TYPE_NR1004 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 1;//����1��������ʹ��������Ԥ��
	}
	else if(TL_BOARD_TYPE_NR1008 == TL_HSLIB_TYPE)
	{
		lib_gbl_info.preview_chn_threshold = 1;//����1��������ʹ��������Ԥ��
	}
	
	#ifdef FAST_SWITCH_PREVIEW
	if(lib_gbl_info.preview_chn_threshold > 1)
	{
		lib_gbl_info.preview_chn_threshold = 1;
	}
	#endif

	#ifdef HI3535
	//yaogang modify 20140918
	//�Ļ�����������
	lib_gbl_info.preview_chn_threshold = 4;//����4��������ʹ��������Ԥ��
	#endif
	
	//lib_gbl_info.sCmdHdr = CreateMsgCmd(sizeof(ipc_unit));
	//if(lib_gbl_info.sCmdHdr == NULL)
	//{
	//	fprintf(stderr, "<ERROR>hslib_init CreateMsgCmd fail!!!!!!!!!!!!!\n");
	//	
	//	while(1)
	//	{
	//		sleep(1);
	//	}
	//}
	
	plib_gbl_info->fd_tw286x = open("/dev/tw_286x", O_RDWR);
	if(plib_gbl_info->fd_tw286x < 0)
	{
		perror("open /dev/tw_286x");
		printf("<ERROR>hslib_init fail!!!!!!!!!!!!!please Check whether loaded the tw_286x.ko driver!\n");
		return; 
	}
	
	//��ͬ��ƽ̨�豸����һ����Ҫ��������
	#ifdef HI3531
	plib_gbl_info->fd_tl = open("/dev/tl_R9800", O_RDWR);
	#else
	plib_gbl_info->fd_tl = open("/dev/tl_R9700", O_RDWR);
	#endif
	if(plib_gbl_info->fd_tl < 0)
	{
		perror("open /dev/tl_Rxxx");
		printf("<ERROR>hslib_init fail!!!!!!!!!!!!! please Check GPIO driver!\n");
		return;
	}
	
	//csp modify for audio
	{
		unsigned int buf[5];
		if(ioctl(plib_gbl_info->fd_tw286x, TL_AUTO_CHIP_TYPE, buf) < 0)
		{
			printf("<ERROR>tl_hslib_init fail!!!!!!!!!!!!! TL_AUTO_CHIP_TYPE: system will not work!\n");
		}
		
		if(buf[0] == CHIP_TW2968_1 || buf[0] == CHIP_TW2968_2 || buf[0] == CHIP_TW2964_1)
		{
			is_decoder_techwell = 1;
			
			//csp modify 20140525
			if(buf[0] == CHIP_TW2968_1 || buf[0] == CHIP_TW2968_2 || buf[0] == CHIP_TW2964_1)
			{
				tw286x_wr(0, 0x40, 0x00);
				
				unsigned char chip_id = 0xff;
				tw286x_rd(0, 0xFF, &chip_id);//csp modify 20140612
				
				if(chip_id != 0xF0 && chip_id != 0xE8)
				{
					no_audio_chip = 1;
				}
			}
		}
		else if(buf[0] == CHIP_TW2868_1 || buf[0] == CHIP_TW2868_2 || buf[0] == CHIP_TW2866_1)
		{
			is_decoder_techwell = 1;
			
			//csp modify 20140525
			if(buf[0] == CHIP_TW2868_1|| buf[0] == CHIP_TW2868_2)
			{
				tw286x_wr(0, 0x40, 0x00);
				
				unsigned char chip_id = 0xff;
				tw286x_rd(0, 0xFF, &chip_id);//csp modify 20140612
				
				if(chip_id != 0xB0)
				{
					no_audio_chip = 1;
				}
			}
		}
		else if(buf[0] == CHIP_NVP1914_1 || buf[0] == CHIP_NVP1918_1 || buf[0] == CHIP_NVP1114B_1 || buf[0] == CHIP_NVP1118B_1)
		{
			is_decoder_nvp = 1;
		}
		else if(buf[0] == CHIP_RN6314A_1 || buf[0] == CHIP_RN6318A_1)
		{
			is_decoder_rn631x = 1;
		}
		else//csp modify 20140525
		{
			#ifndef HI3535
			no_audio_chip = 1;
			#endif
		}
		
		printf("is_decoder_techwell=%d,is_decoder_nvp=%d,is_decoder_rn631x=%d,decoder_chip=%d,no_audio_chip=%d\n",
			is_decoder_techwell,is_decoder_nvp,is_decoder_rn631x,buf[0],no_audio_chip);
	}
	
	if(0 != (g_client_id & TL_PAL))
	{
		lib_gbl_info.is_pal = 1;
	}
	else if(0 != (g_client_id & TL_NTSC))
	{
		lib_gbl_info.is_pal = 0;
	}
	printf("Hi352x hslib config for %s\n", IS_PAL() ? "PAL" : "NTSC");
	
	if(is_decoder_nvp)
	{
		nvp_wr(0, 0, 0x78, 0x88);
		nvp_wr(0, 0, 0x79, 0x88);
		nvp_wr(0, 0, 0xf8, 0x88);
		nvp_wr(0, 0, 0xf9, 0x88);
	}
	
	s32Ret = SAMPLE_InitMPP();
	if(0 != s32Ret)
	{
		printf("<ERROR>hslib_init fail, SAMPLE_InitMPP!!!!!!!!!!!!! system will not work!\n");
		return;
	}
	
	//MPP version
	MPP_VERSION_S stMppVersion;
	HI_MPI_SYS_GetVersion(&stMppVersion);
	printf("Mpp version: %s\n", stMppVersion.aVersion);
	
	#if defined(HI3531)
	VO_DEVICE_CVBS = SAMPLE_VO_DEV_DSD0;
	VO_DEVICE_HD   = SAMPLE_VO_DEV_DHD1;
	#elif defined(HI3535)
	VO_DEVICE_CVBS = SAMPLE_VO_DEV_DSD0;
	VO_DEVICE_HD   = SAMPLE_VO_DEV_DHD0;
	#else
	VO_DEVICE_CVBS = SAMPLE_VO_DEV_DSD1;
	VO_DEVICE_HD   = SAMPLE_VO_DEV_DHD0;
	#endif
	
	#ifdef HI3531
	s32Ret = VPSS_VO_Memconfig();
	if(0 != s32Ret)
	{
		printf("<ERROR>hslib_init fail, VPSS_VO_Memconfig!!!!!!!!!!!!! system will not work!\n");
		return;
	}
	
	s32Ret = SAMPLE_COMM_VDEC_MemConfig();
	if(0 != s32Ret)
	{
		LIB_PRT("<ERROR>hslib_init fail, SAMPLE_COMM_VDEC_MemConfig failed with %d!!! system will not work!\n", s32Ret);
		return;
	}
	#endif
	
	if(TL_BOARD_TYPE_NR2116 == TL_HSLIB_TYPE || TL_BOARD_TYPE_NR3116 == TL_HSLIB_TYPE)
	{
		ARG_VI_NUM_MAX = 16;
		ARG_VO_NUM_MAX = 16;
		
		//stSize.u32Width = 1280;
		//stSize.u32Height = 720;
		
		//s32Ret = SAMPLE_StartVpss(stSize, ARG_VI_NUM_MAX);
		//if(0 != s32Ret)
		//{
		//	printf("<ERROR>hslib_init fail, SAMPLE_StartVpss!!!!!!!!!!!!! system will not work!\n");
		//	return;
		//}
		
		s32Ret = vio_enable_vo_all(VO_MODE_16MUX);
		if(0 != s32Ret)
		{
			printf("<ERROR>hslib_init fail, vio_enable_vo_all!!!!!!!!!!!!! system will not work!\n");
			return;
		}
	}
	else if(TL_BOARD_TYPE_NR3132 == TL_HSLIB_TYPE)
	{
		ARG_VI_NUM_MAX = 32;
		ARG_VO_NUM_MAX = 36;
		
		s32Ret = vio_enable_vo_all(VO_MODE_36MUX);
		if(0 != s32Ret)
		{
			printf("<ERROR>hslib_init fail, vio_enable_vo_all!!!!!!!!!!!!! system will not work!\n");
			return;
		}
	}
	else if(TL_BOARD_TYPE_NR3124 == TL_HSLIB_TYPE)
	{
		ARG_VI_NUM_MAX = 24;
		ARG_VO_NUM_MAX = 25;
		
		s32Ret = vio_enable_vo_all(VO_MODE_25MUX);
		if(0 != s32Ret)
		{
			printf("<ERROR>hslib_init fail, vio_enable_vo_all!!!!!!!!!!!!! system will not work!\n");
			return;
		}
		
		//SIZE_S stSize;
		//stSize.u32Width = 720;
		//stSize.u32Height = 576;
		//SAMPLE_COMM_VPSS_Start(0, 64, &stSize, 5, NULL);
		//SAMPLE_COMM_VPSS_Stop(0, 64, 5);
		//printf("start-stop-vpss\n");
	}
	else if(TL_BOARD_TYPE_NR1004 == TL_HSLIB_TYPE)
	{
		ARG_VI_NUM_MAX = 4;
		ARG_VO_NUM_MAX = 4;
		
		s32Ret = vio_enable_vo_all(VO_MODE_4MUX);
		if(0 != s32Ret)
		{
			printf("<ERROR>hslib_init fail, vio_enable_vo_all!!!!!!!!!!!!! system will not work!\n");
			return;
		}
	}
	else if(TL_BOARD_TYPE_NR1008 == TL_HSLIB_TYPE)
	{
		ARG_VI_NUM_MAX = 8;
		ARG_VO_NUM_MAX = 9;
		
		s32Ret = vio_enable_vo_all(VO_MODE_9MUX);
		if(0 != s32Ret)
		{
			printf("<ERROR>hslib_init fail, vio_enable_vo_all!!!!!!!!!!!!! system will not work!\n");
			return;
		}
	}

	//printf("%s ARG_VI_NUM_MAX: %d; ARG_VO_NUM_MAX: %d\n", __func__, ARG_VI_NUM_MAX, ARG_VO_NUM_MAX);
	//lib_gbl_info.last_pre_mode = PREVIEW_CLOSE;//default:preview close
	
	pthread_mutex_init(&plib_gbl_info->lock_md, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_venc, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_venc_min, NULL);
	pthread_mutex_init(&plib_gbl_info->preview_lock, NULL);
	pthread_mutex_init(&plib_gbl_info->i2c_bank_lock, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_ai_aenc, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_adec_ao, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_adec_ao_ref_count, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_vi_set, NULL);
	pthread_mutex_init(&plib_gbl_info->lock_venc_pts, NULL);
	
	//csp modify 20140318
	pthread_mutex_init(&plib_gbl_info->lock_venc_third, NULL);
	
	s32Ret = venc_init();
	if(0 != s32Ret)
	{
		printf("<ERROR>hslib_init fail, venc_init!!!!!!!!!!!!! system will not work!\n");
		return;
	}
	
	//NVR used for NR1008 audio bug
	tl_audio_open();
	
#ifdef FAST_SWITCH_PREVIEW
	int i=0;
	for(i=0;i<ARG_VI_NUM_MAX;i++)
	{
		pthread_mutex_init(&plib_gbl_info->preview_chn_lock[i], NULL);
		plib_gbl_info->byDecoderType[i] = 0;
	}
	vio_bind_vi2vo_all(0, ARG_VI_NUM_MAX, 0, 0, 0);
	nvr_preview_vdec_open(0, ARG_VI_NUM_MAX);
#endif
	
	IPC_RegisterCallback(DealRealStream, DealStreamState);
	IPC_Init(ARG_VI_NUM_MAX);
	
	printf("hslib_init ok!\n");
	
	return;
}

void tl_hslib_init_c(tl_board_type_e type, int client_id)
{
	g_client_id = client_id;
	printf("hslib_init_c g_client_id = %d\n", g_client_id);
	
	tl_hslib_init(type);
	
	return;
}

//����ָ��ͨ����Ӧ�����������
//���������Ϊ���������ͨ��֮ǰ֮ǰ���õ����������
int tl_set_ipcamera(int channel, ipc_unit *ipcam)
{
	if(channel >= ARG_VI_NUM_MAX)
	{
		return -1;
	}
	
	if(ipcam == NULL)
	{
		return -1;
	}
	
	//return WriteMsgCmd(plib_gbl_info->sCmdHdr, ipcam);
	
	plib_gbl_info->ipcam[channel] = *ipcam;
	return IPC_Set(channel, ipcam);
}

//��ȡָ��ͨ����Ӧ�����������
int tl_get_ipcamera(int channel, ipc_unit *ipcam)
{
	if(channel >= ARG_VI_NUM_MAX)
	{
		return -1;
	}
	
	if(ipcam == NULL)
	{
		return -1;
	}
	
	*ipcam = plib_gbl_info->ipcam[channel];
	
	return 0;
}

