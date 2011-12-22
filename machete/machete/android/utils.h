#pragma once

extern "C" {
#include <png.h>
}
#include <stdio.h>
#include <zip.h>

#ifndef LOG_TAG
#include <android/log.h>

#define  LOG_TAG    "machete"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

zip* loadAPK (const char* apkPath);

//Filename will be looked up in the apk (should start with assets/ or res/
png_byte* loadTextureFromPNG (zip* APKArchive, const char* filename, int &width, int &height);

unsigned int loadFile(zip* APKArchive, const char* filename, void **data);

