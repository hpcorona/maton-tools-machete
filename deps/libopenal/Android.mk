# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(LIBOPENAL_PATH)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/OpenAL32/include
LOCAL_MODULE    := openal
LOCAL_SRC_FILES :=\
	OpenAL32/alAuxEffectSlot.c \
	OpenAL32/alBuffer.c        \
	OpenAL32/alDatabuffer.c    \
	OpenAL32/alEffect.c        \
	OpenAL32/alError.c         \
	OpenAL32/alExtension.c     \
	OpenAL32/alFilter.c        \
	OpenAL32/alListener.c      \
	OpenAL32/alSource.c        \
	OpenAL32/alState.c         \
	OpenAL32/alThunk.c         \
	Alc/ALc.c                  \
	Alc/alcConfig.c            \
	Alc/alcEcho.c              \
	Alc/alcModulator.c         \
	Alc/alcReverb.c            \
	Alc/alcRing.c              \
	Alc/alcThread.c            \
	Alc/ALu.c                  \
	Alc/android.c              \
	Alc/bs2b.c                 \
	Alc/null.c
	
LOCAL_LDLIBS := -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
