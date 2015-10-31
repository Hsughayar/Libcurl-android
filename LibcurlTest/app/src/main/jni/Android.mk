LOCAL_PATH := $(call my-dir)

#cURL prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := curl-prebuilt
LOCAL_SRC_FILES := \
  ../../../../../curl/$(TARGET_ARCH_ABI)/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := native
LOCAL_C_INCLUDES := $(LOCAL_PATH)/native/_headers
# --------------------------------------------------- 
# -----------------(sources)-------------------------
LOCAL_SRC_FILES := native/_sources/tools/browser.cpp
# --------------------------------------------------- 
# --------------------------------------------------- 
LOCAL_STATIC_LIBRARIES := curl-prebuilt 
COMMON_CFLAGS := -Werror -DANDROID 

ifeq ($(TARGET_ARCH),arm)
  LOCAL_CFLAGS := -mfpu=vfp -mfloat-abi=softfp -fno-short-enums
endif

LOCAL_CFLAGS += $(COMMON_CFLAGS)
LOCAL_LDLIBS := -lz -llog -Wl,-s
LOCAL_CPPFLAGS += -std=gnu++0x
LOCAL_C_INCLUDES += \
  $(NDK_PATH)/platforms/$(TARGET_PLATFORM)/arch-$(TARGET_ARCH)/usr/include \
  $(LOCAL_PATH)/../../../../../curl/include

include $(BUILD_SHARED_LIBRARY)