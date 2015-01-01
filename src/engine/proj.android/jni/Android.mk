LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := engine
LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../../../thirdparty	\
					$(LOCAL_PATH)/../../../../thirdparty/zlib  \
					$(LOCAL_PATH)/../../../	\
					$(LOCAL_PATH)/../../

LOCAL_SRC_FILES    := ../../cameranode.cpp		\
					../../cloth.cpp				\
					../../gridnode.cpp			\
					../../lightnode.cpp			\
					../../material.cpp			\
					../../meshnode.cpp			\
					../../meshresource.cpp		\
					../../resourcecache.cpp		\
					../../postprocess.cpp		\
					../../postprocessdof.cpp	\
					../../rendercontext.cpp		\
					../../rendernode.cpp		\
					../../resource.cpp			\
					../../scene.cpp				\
					../../scenenode.cpp			\
					../../texture.cpp			\
					../../utilityrenderer.cpp

LOCAL_CFLAGS       := -O3 -fPIC
LOCAL_ARM_MODE 	   := arm
TARGET_ARCH 	   := arm
LOCAL_ALLOW_UNDEFINED_SYMBOLS := false
include $(BUILD_STATIC_LIBRARY)
