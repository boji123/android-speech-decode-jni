#ifndef _COMMON_H_
#define _COMMON_H_

#define NAMESIZE 20
#define DIMENSION 39   //��˹ά��
#define NAME 200
#define SIZEA 30
                                        //��Ƶ�ļ�·�����ļ���


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>



#include <android/log.h>
#define LOG_TAG "JNI"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#endif
