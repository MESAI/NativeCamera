LOCAL_PATH := $(call my-dir)
 
 
 
 
include $(CLEAR_VARS)
LOCAL_MODULE := opencv-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libopencv_java.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera1-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r4.2.0.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera2-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r4.1.1.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera3-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r4.0.3.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera4-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r4.0.0.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera5-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r3.0.1.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera6-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r2.3.3.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
LOCAL_MODULE := camera7-prebuilt
LOCAL_SRC_FILES = build/libs/$(TARGET_ARCH_ABI)/libnative_camera_r2.2.0.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/build/include
include $(PREBUILT_SHARED_LIBRARY)
 
include $(CLEAR_VARS)
OPENGLES_LIB  := -lGLESv1_CM
OPENGLES_DEF  := -DUSE_OPENGL_ES_1_1
LOCAL_MODULE    := NativeCamera
LOCAL_SHARED_LIBRARIES := opencv-prebuilt 
LOCAL_SRC_FILES := CameraRenderer.cpp
LOCAL_LDLIBS +=  $(OPENGLES_LIB) -llog -ldl
 
include $(BUILD_SHARED_LIBRARY)