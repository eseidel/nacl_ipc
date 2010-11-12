// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/thread_checker.h"
#include "base/scoped_ptr.h"
#include "base/simple_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

#ifndef NDEBUG

// Simple class to exersice the basics of ThreadChecker.
// Both the destructor and DoStuff should verify that they were
// called on the same thread as the constructor.
class ThreadCheckerClass : public ThreadChecker {
 public:
  ThreadCheckerClass() {}

  // Verifies that it was called on the same thread as the constructor.
  void DoStuff() {
    DCHECK(CalledOnValidThread());
  }

  void DetachFromThread() {
    ThreadChecker::DetachFromThread();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadCheckerClass);
};

// Calls ThreadCheckerClass::DoStuff on another thread.
class CallDoStuffOnThread : public base::SimpleThread {
 public:
  CallDoStuffOnThread(ThreadCheckerClass* thread_checker_class)
      : SimpleThread("call_do_stuff_on_thread"),
        thread_checker_class_(thread_checker_class) {
  }

  virtual void Run() {
    thread_checker_class_->DoStuff();
  }

 private:
  ThreadCheckerClass* thread_checker_class_;

  DISALLOW_COPY_AND_ASSIGN(CallDoStuffOnThread);
};

// Deletes ThreadCheckerClass on a different thread.
class DeleteThreadCheckerClassOnThread : public base::SimpleThread {
 public:
  DeleteThreadCheckerClassOnThread(ThreadCheckerClass* thread_checker_class)
      : SimpleThread("delete_thread_checker_class_on_thread"),
        thread_checker_class_(thread_checker_class) {
  }

  virtual void Run() {
    thread_checker_class_.reset();
  }

 private:
  scoped_ptr<ThreadCheckerClass> thread_checker_class_;

  DISALLOW_COPY_AND_ASSIGN(DeleteThreadCheckerClassOnThread);
};

TEST(ThreadCheckerTest, CallsAllowedOnSameThread) {
  scoped_ptr<ThreadCheckerClass> thread_checker_class(
      new ThreadCheckerClass);

  // Verify that DoStuff doesn't assert.
  thread_checker_class->DoStuff();

  // Verify that the destructor doesn't assert.
  thread_checker_class.reset();
}

TEST(ThreadCheckerTest, DestructorAllowedOnDifferentThread) {
  scoped_ptr<ThreadCheckerClass> thread_checker_class(
      new ThreadCheckerClass);

  // Verify that the destructor doesn't assert
  // when called on a different thread.
  DeleteThreadCheckerClassOnThread delete_on_thread(
      thread_checker_class.release());

  delete_on_thread.Start();
  delete_on_thread.Join();
}

TEST(ThreadCheckerTest, DetachFromThread) {
  scoped_ptr<ThreadCheckerClass> thread_checker_class(
      new ThreadCheckerClass);

  // Verify that DoStuff doesn't assert when called on a different thread after
  // a call to DetachFromThread.
  thread_checker_class->DetachFromThread();
  CallDoStuffOnThread call_on_thread(thread_checker_class.get());

  call_on_thread.Start();
  call_on_thread.Join();
}

#if GTEST_HAS_DEATH_TEST

TEST(ThreadCheckerDeathTest, MethodNotAllowedOnDifferentThread) {
  ASSERT_DEBUG_DEATH({
      scoped_ptr<ThreadCheckerClass> thread_checker_class(
          new ThreadCheckerClass);

      // Verify that DoStuff asserts when called on a different thread.
      CallDoStuffOnThread call_on_thread(thread_checker_class.get());

      call_on_thread.Start();
      call_on_thread.Join();
    }, "");
}

TEST(ThreadCheckerDeathTest, DetachFromThread) {
  ASSERT_DEBUG_DEATH({
      scoped_ptr<ThreadCheckerClass> thread_checker_class(
          new ThreadCheckerClass);

      // Verify that DoStuff doesn't assert when called on a different thread
      // after a call to DetachFromThread.
      thread_checker_class->DetachFromThread();
      CallDoStuffOnThread call_on_thread(thread_checker_class.get());

      call_on_thread.Start();
      call_on_thread.Join();

      // Verify that DoStuff asserts after moving to another thread.
      thread_checker_class->DoStuff();
    }, "");
}

#endif  // GTEST_HAS_DEATH_TEST

#endif  // NDEBUG
