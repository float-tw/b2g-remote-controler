LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	gsnap.c

LOCAL_C_INCLUDES += \
	external/jpeg

LOCAL_SHARED_LIBRARIES:= libjpeg

LOCAL_MODULE:= gsnap
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
