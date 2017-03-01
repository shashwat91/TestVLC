LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE:=STATIC
include /media/shashwat/OS/NVPACK/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on

include $(BUILD_SHARED_LIBRARY)
