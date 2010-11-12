# Copyright 2010, The Native Client SDK Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can
# be found in the LICENSE file.

# Makefile for the Hello World example.

.PHONY: all clean

CCFILES = hello_world_module.cc \
          hello_world.cc \
          npp_gate.cc \
          ipc/ipc_switches.cc \
          ipc/file_descriptor_set_posix.cc \
          ipc/ipc_channel_proxy.cc \
          ipc/ipc_logging.cc \
          ipc/ipc_message.cc \
          ipc/ipc_message_utils.cc \
          ipc/ipc_sync_channel.cc \
          ipc/ipc_sync_message.cc \
          ipc/ipc_sync_message_filter.cc \
          base/logging.cc \
          base/lock.cc \
          base/lock_impl_posix.cc \
          base/command_line.cc \
          base/base_switches.cc \
          base/file_path.cc \
          base/utf_string_conversions.cc \
          base/pickle.cc \
          base/ref_counted.cc \
          base/task.cc \
          base/task_queue.cc \
          base/string16.cc \
          base/platform_thread_posix.cc \
          base/string_number_conversions.cc \
          base/utf_string_conversion_utils.cc \
          base/string_util.cc \
          base/stringprintf.cc \
          base/string_piece.cc \
          base/ref_counted_memory.cc \
          base/thread_local_posix.cc \
          base/debug/stack_trace.cc \
          base/time.cc \
          base/atomicops_internals_x86_gcc.cc \
          base/at_exit.cc \
          base/third_party/dmg_fp/dtoa.cc \
          base/third_party/dmg_fp/g_fmt.cc \
          base/tracked.cc \
          base/tracked_objects.cc \
          base/debug/debugger.cc \
          base/values.cc \
          base/json/json_reader.cc \
          base/json/json_writer.cc \
          base/lazy_instance.cc \
          base/waitable_event_posix.cc \
          base/waitable_event_watcher_posix.cc \
          base/thread_local_storage_posix.cc \
          base/json/string_escape.cc \
          base/condition_variable_posix.cc \
          base/safe_strerror_posix.cc \
          base/time_posix.cc \
          base/sys_string_conversions_linux.cc \


OBJECTS_X86_32 = $(CCFILES:%.cc=%_x86_32.o)
OBJECTS_X86_64 = $(CCFILES:%.cc=%_x86_64.o)

# We could import the sdk into the git repo if we really wanted.
NACL_SDK_ROOT = /Projects/native_client_sdk_0_1_507_1


CFLAGS = -Wall -Wno-long-long -pthread -DXP_UNIX -Werror
INCLUDES = -I$(CURDIR) \
           -I$(NACL_SDK_ROOT)
LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          $(ARCH_FLAGS)
OPT_FLAGS = -O2

all: check_variables hello_world_x86_32.nexe hello_world_x86_64.nexe

# common.mk has rules to build .o files from .cc files.
-include $(NACL_SDK_ROOT)/examples/common.mk

hello_world_x86_32.nexe: $(OBJECTS_X86_32)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

hello_world_x86_64.nexe: $(OBJECTS_X86_64)
	$(CPP) $^ $(LDFLAGS) -m64 -o $@

clean:
	-$(RM) $(OBJECTS_X86_32) $(OBJECTS_X86_64) \
	    hello_world_x86_32.nexe hello_world_x86_64.nexe

# This target is used by the SDK build system to produce a pre-built version
# of the .nexe.  You do not need to call this target.
install_prebuilt: hello_world_x86_32.nexe hello_world_x86_64.nexe
	-$(RM) $(OBJECTS_X86_32) $(OBJECTS_X86_64)
