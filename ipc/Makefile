.PHONY:

OBJECTS = \
          file_descriptor_set_posix.o \
          ipc_channel_nacl.o \
          ipc_channel_proxy.o \
          ipc_logging.o \
          ipc_message.o \
          ipc_message_utils.o \
          ipc_switches.o \
          ipc_sync_channel.o \
          ipc_sync_message.o \
          ipc_sync_message_filter.o \

TEST_OBJECTS = \
          file_descriptor_set_posix_unittest.o \
          ipc_message_unittest.o \

# Hangs, sadly.
#          ipc_sync_channel_unittest.o \

# Can't seem to link w/o ipc_sync_channel_unittest
#          ipc_sync_message_unittest.o \

# Depends on MultiProcessTest (which makes no sense for NaCl)
#          ipc_tests.o \
#          ipc_fuzzing_tests.o \

PROJECT_ROOT = ..
LIBRARY_NAME = ipc

include $(PROJECT_ROOT)/common.mk
include $(PROJECT_ROOT)/library.mk
