.PHONY:

OBJECTS += \
          ipc/file_descriptor_set_posix.o \
          ipc/ipc_channel_nacl.o \
          ipc/ipc_channel_proxy.o \
          ipc/ipc_logging.o \
          ipc/ipc_message.o \
          ipc/ipc_message_utils.o \
          ipc/ipc_switches.o \
          ipc/ipc_sync_channel.o \
          ipc/ipc_sync_message.o \
          ipc/ipc_sync_message_filter.o \

TEST_OBJECTS += \
          ipc/file_descriptor_set_posix_unittest.o \
          ipc/ipc_message_unittest.o \

# Hangs, sadly.
#          ipc_sync_channel_unittest.o \

# Can't seem to link w/o ipc_sync_channel_unittest
#          ipc_sync_message_unittest.o \

# Depends on MultiProcessTest (which makes no sense for NaCl)
#          ipc_tests.o \
#          ipc_fuzzing_tests.o \

