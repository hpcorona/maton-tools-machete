#pragma once

#ifndef LOG_TAG
#include <android/log.h>

#define  LOG_TAG    "machete"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#include <jni.h>

extern jobject TheActivity;
extern JavaVM* javaVM;

JNIEnv* GetEnv();
