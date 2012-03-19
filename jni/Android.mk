ifndef MACHETE_PATH
MACHETE_PATH := $(call my-dir)
endif

LIBZIP_PATH := $(MACHETE_PATH)/deps/libzip
LIBPNG_PATH := $(MACHETE_PATH)/deps/libpng
LIBOPENAL_PATH := $(MACHETE_PATH)/deps/libopenal
LIBTREMOR_PATH := $(MACHETE_PATH)/deps/libtremor

include $(MACHETE_PATH)/deps/libopenal/Android.mk
include $(MACHETE_PATH)/deps/libzip/Android.mk
include $(MACHETE_PATH)/deps/libpng/Android.mk
include $(MACHETE_PATH)/deps/libtremor/Android.mk

LOCAL_PATH := $(MACHETE_PATH)/machete/machete
include $(CLEAR_VARS)
LOCAL_MODULE := machete
LOCAL_C_INCLUDES := $(MACHETE_PATH)/deps/libzip/ $(MACHETE_PATH)/deps/libpng/ $(MACHETE_PATH)/deps/libtremor/ $(LIBOPENAL_PATH)
LOCAL_STATIC_LIBRARIES := zip png tremor
LOCAL_SHARED_LIBRARIES := openal
LOCAL_SRC_FILES := \
	widget/extended.cpp \
	widget/widget.cpp \
	thread/thread.cpp \
	storage/sqlite3.c \
	storage/sql.cpp \
	sound/sound.cpp \
	platform/platform.cpp \
	input/touch.cpp \
	graphics/draw.cpp \
	graphics/shader.cpp \
	draw/bundle.cpp \
	draw/element.cpp \
	draw/font.cpp \
	draw/meta.cpp \
	draw/asha.cpp \
	data/mbd.cpp \
	common/callback.cpp \
	anim/func.cpp \
	anim/tween.cpp \
	android/com_maton_machete_MacheteNative.cpp \
	android/utils.cpp \
	android/AndroidPlatform.cpp
LOCAL_LDLIBS := -lGLESv2 -ldl -llog -lz
include $(BUILD_STATIC_LIBRARY)

