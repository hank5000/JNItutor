LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS :=-llog
LOCAL_MODULE    := JNItutor
LOCAL_SRC_FILES := JNItutor.cpp

include $(BUILD_SHARED_LIBRARY)
