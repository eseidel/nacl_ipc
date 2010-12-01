.PHONY:

OBJECTS = \
          atomicops_internals_x86_gcc.o \
          at_exit.o \
          base_switches.o \
          command_line.o \
          condition_variable_posix.o \
          debug/debugger.o \
          debug/debugger_posix.o \
          debug/stack_trace.o \
          debug/stack_trace_nacl.o \
          file_path.o \
          json/json_reader.o \
          json/json_writer.o \
          json/string_escape.o \
          lazy_instance.o \
          lock.o \
          lock_impl_posix.o \
          logging.o \
          message_loop.o \
          message_loop_proxy.o \
          message_loop_proxy_impl.o \
          message_pump.o \
          message_pump_default.o \
          metrics/histogram.o \
          non_thread_safe.o \
          pickle.o \
          platform_thread_posix.o \
          ref_counted.o \
          ref_counted_memory.o \
          safe_strerror_posix.o \
          simple_thread.o \
          string16.o \
          stringprintf.o \
          string_number_conversions.o \
          string_piece.o \
          string_split.o \
          string_util.o \
          sys_string_conversions_linux.o \
          task.o \
          task_queue.o \
          third_party/dmg_fp/dtoa.o \
          third_party/dmg_fp/g_fmt.o \
          third_party/icu/icu_utf.o \
          third_party/nspr/prtime.o \
          thread.o \
          thread_checker.o \
          thread_collision_warner.o \
          thread_local_posix.o \
          thread_local_storage_posix.o \
          thread_restrictions.o \
          time.o \
          time_posix.o \
          tracked.o \
          tracked_objects.o \
          utf_string_conversions.o \
          utf_string_conversion_utils.o \
          values.o \
          vlog.o \
          waitable_event_posix.o \
          waitable_event_watcher_posix.o \
          weak_ptr.o \

TEST_OBJECTS = \
          at_exit_unittest.o \
          atomicops_unittest.o \
          command_line_unittest.o \
          condition_variable_unittest.o \
          debug/stack_trace_unittest.o \
          file_path_unittest.o \
          json/json_reader_unittest.o \
          json/json_writer_unittest.o \
          json/string_escape_unittest.o \
          lazy_instance_unittest.o \
          lock_unittest.o \
          logging_unittest.o \
          message_loop_proxy_impl_unittest.o \
          metrics/histogram_unittest.o \
          pickle_unittest.o \
          platform_thread_unittest.o \
          ref_counted_unittest.o \
          string16_unittest.o \
          string_number_conversions_unittest.o \
          string_piece_unittest.o \
          string_split_unittest.o \
          string_util_unittest.o \
          sys_string_conversions_unittest.o \
          thread_local_storage_unittest.o \
          thread_local_unittest.o \
          thread_unittest.o \
          time_unittest.o \
          tracked_objects_unittest.o \
          utf_string_conversions_unittest.o \
          values_unittest.o \
          vlog_unittest.o \
          waitable_event_unittest.o \

# Crashes with "** Fault in NaCl untrusted code"
# Needs to be reduced, as that's likely a crash in SDK code.
#          message_loop_unittest.o \
# Similar crash in WaitableEventWatcherTest.BasicSignal in "untrusted code"
#          waitable_event_watcher_unittest.o \

PROJECT_ROOT = ..
LIBRARY_NAME = base

include $(PROJECT_ROOT)/common.mk
include $(PROJECT_ROOT)/library.mk
