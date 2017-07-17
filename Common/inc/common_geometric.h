#ifndef _GEOMETRIC_H_
#define _GEOMETRIC_H_

#include "common_basetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    ������صĽṹ��
*/

//���νṹ��
typedef struct
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
} SRect, *SPRect;

//��ṹ��
typedef struct
{
    s32 x;  //������x
    s32 y; //������y
} SPoint;

#ifdef __cplusplus
}
#endif

#endif
