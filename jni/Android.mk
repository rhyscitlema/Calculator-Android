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

LOCAL_MODULE := rfet_jni

LOCAL_CFLAGS += -std=c99 -Wall

LOCAL_SRC_FILES := jni_rfet.c

LOCAL_SRC_FILES += $(FOLDER)lib_std/_math.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_value.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_stdio.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_strfun.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_string.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_malloc.c
LOCAL_SRC_FILES += $(FOLDER)lib_std/_texts.c

LOCAL_SRC_FILES += $(FOLDER)librfet/rfet.c
LOCAL_SRC_FILES += $(FOLDER)librfet/component.c
LOCAL_SRC_FILES += $(FOLDER)librfet/expression.c
LOCAL_SRC_FILES += $(FOLDER)librfet/structures.c
LOCAL_SRC_FILES += $(FOLDER)librfet/operations.c

LOCAL_SRC_FILES += $(FOLDER)librodt/mouse.c
LOCAL_SRC_FILES += $(FOLDER)librodt/timer.c
LOCAL_SRC_FILES += $(FOLDER)librodt/tools.c
LOCAL_SRC_FILES += $(FOLDER)librodt/userinterface_.c
LOCAL_SRC_FILES += $(FOLDER)librodt/outsider.c

LOCAL_SRC_FILES += $(FOLDER)algorithms/avl.c
LOCAL_SRC_FILES += $(FOLDER)algorithms/list.c
LOCAL_SRC_FILES += $(FOLDER)algorithms/heap.c
LOCAL_SRC_FILES += $(FOLDER)algorithms/graph.c

LOCAL_C_INCLUDES := $(FOLDER)lib_std/
LOCAL_C_INCLUDES += $(FOLDER)librfet/
LOCAL_C_INCLUDES += $(FOLDER)librodt/
LOCAL_C_INCLUDES += $(FOLDER)algorithms/

include $(BUILD_SHARED_LIBRARY)

