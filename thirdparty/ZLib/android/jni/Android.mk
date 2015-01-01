LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := ZLib
LOCAL_C_INCLUDES   := $(LOCAL_PATH)/../../

LOCAL_SRC_FILES    := ../../adler32.c  \
			 ../../compress.c  \
			 ../../crc32.c  \
			 ../../deflate.c  \
			 ../../gzclose.c  \
			 ../../gzlib.c  \
			 ../../gzread.c  \
			 ../../gzwrite.c  \
			 ../../infback.c  \
			 ../../inffast.c  \
			 ../../inflate.c  \
			 ../../inftrees.c  \
			 ../../trees.c  \
			 ../../uncompr.c  \
			 ../../zutil.c

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS       := -O3 -fPIC
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE           := ZLibShared
LOCAL_STATIC_LIBRARIES := ZLib
include $(BUILD_SHARED_LIBRARY)