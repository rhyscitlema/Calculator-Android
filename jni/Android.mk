# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
FOLDER := ../../

include $(CLEAR_VARS)

LOCAL_MODULE    := mfet_jni

LOCAL_SRC_FILES := jni_mfet.c
LOCAL_SRC_FILES += $(FOLDER)/lib_std/lib_std.c
LOCAL_SRC_FILES += $(FOLDER)/libmfet/libmfet.c
LOCAL_SRC_FILES += $(FOLDER)/librodt/librodt.c
LOCAL_SRC_FILES += $(FOLDER)/algorithms/list.c
LOCAL_SRC_FILES += $(FOLDER)/algorithms/avl.c

LOCAL_C_INCLUDES := $(FOLDER)/lib_std/
LOCAL_C_INCLUDES += $(FOLDER)/libmfet/
LOCAL_C_INCLUDES += $(FOLDER)/librodt/
LOCAL_C_INCLUDES += $(FOLDER)/algorithms/

include $(BUILD_SHARED_LIBRARY)

