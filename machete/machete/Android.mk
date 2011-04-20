LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := machete

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/
LOCAL_STATIC_LIBRARIES := libzip libpng libgame

#-Wno-psabi to remove warning about GCC 4.4 va_list warning
#LOCAL_CFLAGS := -DTARGET_ANDROID -DANDROID_NDK -Wno-psabi

LOCAL_DEFAULT_CPP_EXTENSION := cpp 

LOCAL_SRC_FILES := \
	android/GL2.cpp \
	android/com_maton_machete_MacheteNative.cpp \
	android/ResourceManager.cpp \
	ios/RenderingEngineiOS1.cpp \
	ios/RenderingEngineiOS2.cpp \
	android/utils.cpp \
	android/RenderingEngineAndroid1.cpp \
	android/RenderingEngineAndroid2.cpp

#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)
