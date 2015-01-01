LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := Png
LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../

LOCAL_SRC_FILES    := ../../png.c 		\
					../../pngerror.c 	\
					../../pngget.c 	\
					../../pngmem.c 	\
					../../pngpread.c 	\
					../../pngread.c 	\
					../../pngrio.c  	\
					../../pngrtran.c 	\
					../../pngrutil.c 	\
					../../pngset.c 	\
					../../pngtrans.c 	\
					../../pngwio.c 	\
					../../pngwrite.c 	\
					../../pngwtran.c 	\
					../../pngwutil.c

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS       := -O3 -fPIC
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE           := PngShared
LOCAL_STATIC_LIBRARIES := Png
include $(BUILD_SHARED_LIBRARY)