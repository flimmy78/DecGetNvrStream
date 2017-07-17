

#ifndef __MOUSE_API_H__
#define __MOUSE_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup MouseAPI API Mouse
/// ���ʹ������豸API��
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           MouseCreate
///                |--------+
///           MouseGetData  |
///                |--------+
///           MouseDestory
///                |
///    ===========================
/// \endcode
/// @{

#define MOUSE_LBUTTON			0x1			///< ���״̬���롣
#define MOUSE_RBUTTON			0x2			///< �Ҽ�״̬���롣
#define MOUSE_MBUTTON			0x4			///< �м�״̬���롣
#define MOUSE_TOUCHSCREEN		0x80		///< ���������ݡ�

/// ����������ݽṹ
typedef struct MOUSE_DATA
{
	/// ���������3��������״̬����1��ʾ�������£���0��ʾ�����������\b MOUSE_XXX
	/// ����õ���Ӧ������״̬�����λ��Ӧ��������־����1��ʾΪ���������ݣ���0
	/// ��ʾ������ݡ� ���������µ���������Ϊ�����
	unsigned char	key;

#if 1
	/// x����ƫ�ƣ��������ң���������ȡֵ[-32767, 32768]�����ʹ�á�
	signed short x;

	/// y����ƫ�ƣ��������ϣ��������£�ȡֵ[-32767, 32768]�����ʹ�á�
	signed short y;

	/// ����ƫ�ƣ�������󣬸�����ǰ��ȡֵ[-32767, 32768]�����ʹ�á�
	signed short z;
#else
	/// x����ƫ�ƣ��������ң���������ȡֵ[-127,128]�����ʹ�á�
	signed char		x;

	/// y����ƫ�ƣ��������ϣ��������£�ȡֵ[-127,128]�����ʹ�á�
	signed char		y;

	/// ����ƫ�ƣ�������󣬸�����ǰ��ȡֵ[-127,128]�����ʹ�á�
	signed char		z;
#endif

	/// ����x���꣬���������ҡ�������ʹ�á�
	unsigned short	ax;

	/// ����y���꣬���������¡�������ʹ�á�
	unsigned short	ay;	
} MOUSE_DATA;


/// ��������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int MouseCreate(void);


/// ��������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int MouseDestory(void);


/// һֱ�ȵ�����һ������������¼��ŷ��أ��������������������豻�ε��򲻴��ڣ�
/// �������ش��󡣳���ʱӦ�ó�����Ҫ��ʱ���ԣ����ڲ�С��500ms����ʵ���Ȳ�ι��ܡ�
/// 
/// \param [out] pData ָ�����ʱ��ṹMOUSE_DATA��ָ�롣
/// \retval 0 ���ɹ�
/// \retval <0 ��ʧ��
int MouseGetData(MOUSE_DATA * pData);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__MOUSE_API_H__

