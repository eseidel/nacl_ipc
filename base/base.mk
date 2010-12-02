.PHONY:

OBJECTS += \
          base/atomicops_internals_x86_gcc.o \
          base/at_exit.o \
          base/base_switches.o \
          base/command_line.o \
          base/condition_variable_posix.o \
          base/debug/debugger.o \
          base/debug/debugger_posix.o \
          base/debug/stack_trace.o \
          base/debug/stack_trace_nacl.o \
          base/file_path.o \
          base/json/json_reader.o \
          base/json/json_writer.o \
          base/json/string_escape.o \
          base/lazy_instance.o \
          base/lock.o \
          base/lock_impl_posix.o \
          base/logging.o \
          base/message_loop.o \
          base/message_loop_proxy.o \
          base/message_loop_proxy_impl.o \
          base/message_pump.o \
          base/message_pump_default.o \
          base/metrics/histogram.o \
          base/non_thread_safe.o \
          base/pickle.o \
          base/platform_thread_posix.o \
          base/ref_counted.o \
          base/ref_counted_memory.o \
          base/safe_strerror_posix.o \
          base/simple_thread.o \
          base/string16.o \
          base/stringprintf.o \
          base/string_number_conversions.o \
          base/string_piece.o \
          base/string_split.o \
          base/string_util.o \
          base/sys_string_conversions_linux.o \
          base/task.o \
          base/task_queue.o \
          base/third_party/dmg_fp/dtoa.o \
          base/third_party/dmg_fp/g_fmt.o \
          base/third_party/icu/icu_utf.o \
          base/third_party/nspr/prtime.o \
          base/thread.o \
          base/thread_checker.o \
          base/thread_collision_warner.o \
          base/thread_local_posix.o \
          base/thread_local_storage_posix.o \
          base/thread_restrictions.o \
          base/time.o \
          base/time_posix.o \
          base/tracked.o \
          base/tracked_objects.o \
          base/utf_string_conversions.o \
          base/utf_string_conversion_utils.o \
          base/values.o \
          base/vlog.o \
          base/waitable_event_posix.o \
          base/waitable_event_watcher_posix.o \
          base/weak_ptr.o \

TEST_OBJECTS += \
          base/at_exit_unittest.o \
          base/atomicops_unittest.o \
          base/command_line_unittest.o \
          base/condition_variable_unittest.o \
          base/debug/stack_trace_unittest.o \
          base/file_path_unittest.o \
          base/json/json_reader_unittest.o \
          base/json/json_writer_unittest.o \
          base/json/string_escape_unittest.o \
          base/lazy_instance_unittest.o \
          base/lock_unittest.o \
          base/logging_unittest.o \
          base/message_loop_proxy_impl_unittest.o \
          base/metrics/histogram_unittest.o \
          base/pickle_unittest.o \
          base/platform_thread_unittest.o \
          base/ref_counted_unittest.o \
          base/string16_unittest.o \
          base/string_number_conversions_unittest.o \
          base/string_piece_unittest.o \
          base/string_split_unittest.o \
          base/string_util_unittest.o \
          base/sys_string_conversions_unittest.o \
          base/thread_local_storage_unittest.o \
          base/thread_local_unittest.o \
          base/thread_unittest.o \
          base/time_unittest.o \
          base/tracked_objects_unittest.o \
          base/utf_string_conversions_unittest.o \
          base/values_unittest.o \
          base/vlog_unittest.o \
          base/waitable_event_unittest.o \

# Crashes with "** Fault in NaCl untrusted code"
# Needs to be reduced, as that's likely a crash in SDK code.
#          message_loop_unittest.o \
# Similar crash in WaitableEventWatcherTest.BasicSignal in "untrusted code"
#          waitable_event_watcher_unittest.o \
