#ifndef _CIRCLEBUF_H_
#define _CIRCLEBUF_H_

typedef struct bufinfo
{
	s32 nBufId;//ÿ��buf����һ��id
	u8* nBufAddr;
	u32 nWrOffset;
	u32 nRdOffset;
	u32 nWrOffsetPast;
	u32 nRdOffsetPast;
	u32 nLength;
	u32 nLenUsed;
	sem_t sem;
}SCircleBufInfo;

enum EMERRORCODE					//函数返回错误码枚举
{
	EM_REC_SUCCESS = 0,				//成功
	EM_REC_INVALID_PARA = 1000,		//无效参数
	EM_REC_SEMINIT_FAILED, 			//sem_init error
	EM_REC_MALLOC_ERR,				//malloc失败
	EM_REC_BUF_NO_SPACE,			//写buf时数据太长，buf剩余空间不足
	EM_REC_BUF_NO_DATA,				//读buf时，要读取的长度大于buf内已存数据的总长
	EM_REC_RECORDCREATEBUF,			//创建录像缓冲失败
	EM_REC_RECORDDESTROYBUF,		//销毁录像缓冲失败
	EM_REC_FRAMETOOBIG,				//帧数据太长，缓冲区放不下了
	EM_REC_NODISK,					//没有硬盘，不进行初始化
	EM_REC_HASNOTINIT,				//还没有执行过初始化
	EM_REC_HASBEENINIT,				//已经执行过初始化
	EM_REC_HASBEENDEINIT,			//已经执行过反初始化
	EM_REC_CANNOTOPENNEWFILE,		//不能打开/创建一个新文件用于录像
	EM_REC_ERRGETRECPATH,			//获得录像文件路径错误
	EM_REC_ERROPENNEWFILE,			//打开文件失败
};


s32 CreateCircleBuf(SCircleBufInfo* info);
s32 DestroyCircleBuf(SCircleBufInfo* info);
s32 ResetCircleBuf(SCircleBufInfo* info);
//s32 GetBufWriteOffset(SCircleBufInfo* info);
//s32 GetBufReadOffset(SCircleBufInfo* info);
s32 WriteDataToBuf(SCircleBufInfo* info, u8* data_addr, u32 length);
s32 ReadDataFromBuf(SCircleBufInfo* info, u8* data_addr, u32 length);
s32 ResumeCircleBufToPast(SCircleBufInfo* info, u8 nWr, u8 nRd);
s32 SkipCircleBuf(SCircleBufInfo* info, u8 nWr, u8 nRd, u32 nSkipLen);

#endif //_CIRCLEBUF_H_

