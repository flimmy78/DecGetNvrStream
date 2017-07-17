

#ifndef __GRAPHICS_API_H__
#define __GRAPHICS_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup GraphicsAPI API Graphics
/// ��ͼ���豸�ӿڣ���ȡFrameBuffer��֡���壩����������Overlay�����ӣ������ȵȡ�
/// ��Ⱦ�������ϲ�ʵ�֣���ʱ��֧��Ӳ�����١�
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///             GraphicsCreate                  
///         +---------|------------------+      
///         |   GraphicsSetVstd          |
///         |         |            GraphicsSetAlpha
///         |  GraphicsGetBuffer  GraphicsSetColorKey
///         |         |                  |   
///         +---------|------------------+  
///            GraphicsDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// ͼ��֡����ṹ
typedef struct GRAPHICS_SURFACE
{
	uchar *mem;	///< �Դ��ָ�롣
	uchar *mem0;	///< ������
	uchar *mem1;	///< ������
	int pitch;	///< ��ȣ����Դ�һ��ɨ���ߵĿ�ʼ����һ��ɨ���ߵĿ�ʼ�洢�ռ��ƫ�ƣ��ֽ�Ϊ��λ��
	int width;	///< ���ȣ�����Ϊ��λ��
	int height;	///< ��߶ȣ�����Ϊ��λ��
	int format;	///< ��ɫ��ʽ��ȡgraphics_color_format_t����ֵ��ֵ��
} GRAPHICS_SURFACE;

/// ͼ����ɫ��ʽ����
enum graphics_color_format_t{  
	GRAPHICS_CF_RGB555,	///< 16λ��ʾһ�����ء���ɫ����λ����ΪXRRRRRGG GGGBBBBB��
	GRAPHICS_CF_RGB565,	///< 16λ��ʾһ�����ء���ɫ����λ����ΪRRRRRGGG GGGBBBBB��
};


/// ����ͼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int GraphicsCreate(void);
int GraphicsCreate2(void);
int Graphics2Create(int index);


/// ����ͼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int GraphicsDestory(void);
int GraphicsDestory2(void);
int Graphics2Destroy(int index);


/// ������Ƶ��ʽ��
/// 
/// \param [in]  dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetVstd(uint dwStandard);	
int Graphics2SetVstd(int index, uint dwStandard);	


/// ��ȡͼ�λ��ƻ��塣��Ƶ��ʽ�ı�ʱ���ܻ�ı䣬Ӧ�ó�����Ҫ���»�ȡ��
/// 
/// \param [out] pBuffer ָ����ʾ���������ṹGRAPHICS_SURFACE��ָ�롣GDIֱ��ͨ��
///        �Դ��ָ������Դ档ͼ���豸ֻ��ѡ��GDI֧�ֵ���ɫ��ʽ�е�һ�֣����GDI
///        ����֧�֣�����Ҫ�����µ���ɫ��ʽ��������GDI���ܡ�
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int GraphicsGetBuffer(GRAPHICS_SURFACE * pBuffer);
int GraphicsGetBuffer2(GRAPHICS_SURFACE * pBuffer);
int Graphics2GetSurface(int index, GRAPHICS_SURFACE * pBuffer);


/// ����ͼ�ε�͸���ȡ�ͼ����ͼ����ȫ����Χ�ڰ���͸���ȵ��ӡ�
/// 
/// \param [in] alpha Ҫ���õĵ�ǰ͸���ȣ�ֵԽ�ͱ�ʾ����ʱͼ����ɫ�ķ���Խ�ͣ�Ҳ��
///        Խ͸����ȡֵ0-255��
/// \param [in] delta ͸�����Զ�����ֵ��ÿ֡ʱ��͸���ȼ��ϴ�ֵ��ֱ��͸���ȱ�����
///        ����СΪֹ��ֵΪ0��ʾ�����䡣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetAlpha(int alpha, int delta);
int Graphics2SetAlpha(int index, int alpha, int delta);


/// ����������ɫ����ͼ����ͼ��������ɫ����ʱ��ͼ����Դ�㣬ͼ����Ŀ�Ĳ㣬ͼ�ε���
/// ��ɫ���򽫱���Ƶ�滻��
/// 
/// \param [in] dwKeyLow ��Ӧ����ɫ�������ɫ������Сֵ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \param [in] dwKeyHigh ��Ӧ����ɫ�������ɫ�������ֵ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetColorKey(uint dwKeyLow, uint dwKeyHigh);
int Graphics2SetColorKey(int index, uint dwKeyLow, uint dwKeyHigh);


/// ���ù��λ�ã����ú����ȵ����ڸ����ꡣ����ģ�����ʹ�á�
/// 
/// \param [in] x ���x���ꡣ
/// \param [in] y ���y���ꡣ
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetCursorPos(int x, int y);

/// ����GUI�ֱ���
/// \param [in] w �ֱ����е�x����
/// \param [in[ h �ֱ����е�y����
/// \return 0 ���óɹ�
/// \retval < 0 ����ʧ��
int GraphicsSetResolution(int w, int h);

/// @} end of group

//int GraphicsDeFlicker(VD_RECT* pRect, int level);

int GraphicsShowCursor(void);
int GraphicsHideCursor(void);
int GraphicsGetCursorPos(VD_PPOINT pPoint);
int GraphicsSetCursorPos(int x, int y);
int GraphicsGetCursorBuf(GRAPHICS_SURFACE *pBuffer);

/// ����OSD����������
/// \param [in] rect Ҫ������������
/// \param [in] resv ����
/// \return 0 ���óɹ�
/// \retval < 0 ����ʧ��
int GraphicsDeFlicker(VD_RECT *pRect, int resv);

//ͬ����cvbs
int GraphicsSyncVgaToCvbs(void);

typedef enum
{
	EM_GRAPH_OUTPUT_CVBS = 0,
	EM_GRAPH_OUTPUT_VGA,
} emOutputType;

int GraphicsSetOutput(emOutputType emOut);
emOutputType GraphicsGetOutput(void);


typedef enum
{
	EM_GRAPH_CHIP_HISI3515 = 0,
	EM_GRAPH_CHIP_HISI3520,
	EM_GRAPH_CHIP_HISI3531,
	EM_GRAPH_CHIP_HISI3521,
	EM_GRAPH_CHIP_HISI3520A,
} emChipType;

int GraphicsSetChipType(emChipType emChip);
emChipType GraphicsGetChipType(void);

int GraphicShowJpegPic(unsigned char nShow, char* pJPEGFileName);
#ifdef __cplusplus
}
#endif

#endif //__GRAPHICS_API_H__

