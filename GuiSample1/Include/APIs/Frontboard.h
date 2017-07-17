

#ifndef __FRONTBOARD_API_H__
#define __FRONTBOARD_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup FrontboardAPI API Frontboard
/// ǰ������API�����ں͸���ǰ��������豸ͨѶ����ȡ��ֵ����ָʾ�ƿ������
/// ����USB�ӿ��ϵı�׼104�������������̼�ֵת����Ҳ����ͨ������ӿڷ���������
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///              FrontboardCreate                  
///         +---------|      
///         |    FrontboardRead    
///         |    FrontboardWrite   
///         +---------|      	
///             FrontboardDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int FrontboardCreate(void);


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int FrontboardDestory(void);


/// ��ǰ������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \return>0  �������ֽ���
/// \retval <=0 ��ʧ��
int FrontboardRead(void* pData, uint nBytes);


/// дǰ������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \return>0  д����ֽ���
/// \retval <=0  дʧ��
int FrontboardWrite(void* pData, uint nBytes);
void FrontBoardSetDvrId(uint nId);
void FrontBoardGetReUsedKey(uchar key, uchar* pKeyNew);

void FrontBoardEnable( int bEn );
int FrontBoardReqEnable( void );

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__FRONTBOARD_API_H__

