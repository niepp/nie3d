LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := core
LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../../../thirdparty	\
					$(LOCAL_PATH)/../../../../thirdparty/ZLib  \
					$(LOCAL_PATH)/../../	  \
					$(LOCAL_PATH)/../../unzip  \
					$(LOCAL_PATH)/../../math 	\
					$(LOCAL_PATH)/../../renderer

LOCAL_SRC_FILES    := ../../unzip/unzip.cpp \
					../../unzip/ioapi.cpp  \
					../../log.cpp 	\
					../../config.cpp 	\
					../../fileutils.cpp  \
					../../timer.cpp 	\
					../../variant.cpp 	\
					../../xmlutils.cpp 	\
					../../math/color.cpp \
					../../renderer/renderstate.cpp \
					../../renderer/shadermacro.cpp

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS       := -O3 -fPIC
LOCAL_ARM_MODE 	   := arm
TARGET_ARCH 	   := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := false
include $(BUILD_STATIC_LIBRARY)
