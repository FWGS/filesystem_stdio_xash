# AMXXOnAndroid
# Copyright (C) 2017 a1batross

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(XASH3D_CONFIG)

LOCAL_MODULE := filesystem_stdio

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include $(XASH3D_SRC)/engine

LOCAL_CPPFLAGS += -std=c++0x

LOCAL_SRC_FILES := src/filesystem_impl.cpp src/interface.cpp

include $(BUILD_SHARED_LIBRARY)
