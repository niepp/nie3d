LOCAL_PATH := $(call my-dir)

#the binary file directory
nie_bin_PATH := ../../../../bin/android

thirdparty_PATH := ../../../../thirdparty

# thirdparty libs
include $(CLEAR_VARS) 
LOCAL_MODULE    := ZLib
LOCAL_SRC_FILES := $(thirdparty_PATH)/ZLib/android/lib/$(TARGET_ARCH_ABI)/libZLib.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := Png
LOCAL_SRC_FILES := $(thirdparty_PATH)/LibPNG/android/lib/$(TARGET_ARCH_ABI)/libPng.a
include $(PREBUILT_STATIC_LIBRARY)

# prebuild libs
include $(CLEAR_VARS) 
LOCAL_MODULE    := core
LOCAL_SRC_FILES := $(nie_bin_PATH)/$(TARGET_ARCH_ABI)/libcore.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS) 
LOCAL_MODULE    := oglrenderer
LOCAL_SRC_FILES := $(nie_bin_PATH)/$(TARGET_ARCH_ABI)/liboglrenderer.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS) 
LOCAL_MODULE    := engine
LOCAL_SRC_FILES := $(nie_bin_PATH)/$(TARGET_ARCH_ABI)/libengine.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := main

LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../../../thirdparty \
					  $(LOCAL_PATH)/../../../../thirdparty/zlib \
					  $(LOCAL_PATH)/../../../	\
					  $(LOCAL_PATH)/../../

LOCAL_SRC_FILES    := ../../androidapp.cpp \
					../../main.cpp

LOCAL_LDLIBS       := -llog -landroid -lEGL -lGLESv2

LOCAL_STATIC_LIBRARIES += engine
LOCAL_STATIC_LIBRARIES += oglrenderer
LOCAL_STATIC_LIBRARIES += core
LOCAL_STATIC_LIBRARIES += android_native_app_glue
LOCAL_STATIC_LIBRARIES += Png
LOCAL_STATIC_LIBRARIES += ZLib

LOCAL_CFLAGS       := -O3 -fPIC
LOCAL_ARM_MODE 	   := arm
TARGET_ARCH 	   := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := false
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)