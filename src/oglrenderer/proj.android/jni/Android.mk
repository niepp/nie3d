LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := oglrenderer
LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../../../thirdparty	\
					$(LOCAL_PATH)/../../../../thirdparty/zlib  \
					$(LOCAL_PATH)/../../../	\
					$(LOCAL_PATH)/../../

LOCAL_SRC_FILES    := 	../../ogldefine.cpp \
					../../ogldevice.cpp \
					../../oglgeobuffer.cpp \
					../../oglinputlayout.cpp \
					../../oglmaterialtemplate.cpp \
					../../oglrenderer.cpp \
					../../oglrendertarget.cpp \
					../../oglshader.cpp \
					../../ogltexture.cpp \
					../../oglwindowtarget.cpp

LOCAL_CFLAGS       := -O3 -fPIC
LOCAL_ARM_MODE 	   := arm
TARGET_ARCH 	   := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := false
include $(BUILD_STATIC_LIBRARY)
