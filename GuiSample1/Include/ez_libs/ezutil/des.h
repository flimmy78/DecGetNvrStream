/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * des.h - _explain_
 *
 * Copyright (C) 2007 dahua Technologies, All Rights Reserved.
 *
 * $Id: des.h 2023 2007-5-24 15:00:09Z wujj $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2007-5-24 15:00:09 WuJunjie 10221 Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _DES_H
#define _DES_H

#ifdef __cplusplus
extern "C"
{
#endif

	/*! ���ܽӿ�
	  * \param  pResult ���ܺ�Ļ���λ�� 
			����ĳ��� >= ((iOrigLen+7)/8)*8 
			����iOrigLen������8�ı�������������
			pResult����ΪpOrig�����ǻḲ��ԭ����
	  * \param  pOrig �����ܵĻ���λ��
	  * \param  iOrigLen �����ܻ��泤��
	  * \param  pKey ��Կ ����16�ֽں�ֻȡǰ16�ֽ�
	  * \param  iKeylen ��Կ����
	  * \return : <0ʧ�ܣ����ܺ����ݳ���(Byte)
	  */
	int DesEncrypt(char *pResult,
	               char *pOrig,
	               long iOrigLen,
	               const char *pKey,
	               int iKeylen);

	/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

	/*! ���ܽӿ�
	  * \param  pResult ���ܺ�Ļ���λ��
	  * \param  pOrig �����ܵĻ���λ��
	  * \param  iOrigLen �����ܻ��泤��
	  * \param  pKey ��Կ
	  * \param  iKeylen ��Կ����
	  * \return : <0ʧ��
	  */
	int DesDecrypt(char *pResult,
	               char *pOrig,
	               long iOrigLen,
	               const char *pKey,
	               int iKeylen);

	/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	/*! ���ܽӿ�
	  * \param  pResult ���ܺ�Ļ���λ�� 
			����ĳ��� >= ((iOrigLen+7)/8)*8 
			����iOrigLen������8�ı�������������
			pResult����ΪpOrig�����ǻḲ��ԭ����
	  * \param  pOrig �����ܵĻ���λ��
	  * \param  iOrigLen �����ܻ��泤��
	  * \param  pKey ��Կ ����8�ֽں�ֻȡǰ8�ֽ�
	  * \param  iKeylen ��Կ����
	  * \return : <0ʧ�ܣ����ܺ����ݳ���(Byte)
	  */
	int ez_des_ecb_encrypt(unsigned char *pResult,
	                    unsigned char *pOrig,
	                    long iOrigLen,
	                    const char *pKey,
	                    int iKeylen);

	int ez_des_ecb_decrypt(unsigned char *pResult,
	                    unsigned char *pOrig,
	                    long iOrigLen,
	                    const char *pKey,
	                    int iKeylen);

#ifdef __cplusplus
}
#endif

#endif //_DES_H
