// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREAD_RESTRICTIONS_H_
#define BASE_THREAD_RESTRICTIONS_H_

#include "base/basictypes.h"

namespace base {

// ThreadRestrictions helps protect threads that should not block from
// making blocking calls.  It works like this:
//
// 1) If a thread should not be allowed to make IO calls, mark it:
//      base::ThreadRestrictions::SetIOAllowed(false);
//    By default, threads *are* allowed to make IO calls.
//    In Chrome browser code, IO calls should be proxied to the File thread.
//
// 2) If a function makes a call that will go out to disk, check whether the
//    current thread is allowed:
//      base::ThreadRestrictions::AssertIOAllowed();
//
// ThreadRestrictions does nothing in release builds; it is debug-only.
//
// Style tip: where should you put AssertIOAllowed checks?  It's best
// if you put them as close to the disk access as possible, at the
// lowest level.  This rule is simple to follow and helps catch all
// callers.  For example, if your function GoDoSomeBlockingDiskCall()
// only calls other functions in Chrome and not fopen(), you should go
// add the AssertIOAllowed checks in the helper functions.

class ThreadRestrictions {
 public:
  // Constructing a ScopedAllowIO temporarily allows IO for the current
  // thread.  Doing this is almost certainly always incorrect.
  class ScopedAllowIO {
   public:
    ScopedAllowIO() { previous_value_ = SetIOAllowed(true); }
    ~ScopedAllowIO() { SetIOAllowed(previous_value_); }
   private:
    // Whether IO is allowed when the ScopedAllowIO was constructed.
    bool previous_value_;

    DISALLOW_COPY_AND_ASSIGN(ScopedAllowIO);
  };

#ifndef NDEBUG
  // Set whether the current thread to make IO calls.
  // Threads start out in the *allowed* state.
  // Returns the previous value.
  static bool SetIOAllowed(bool allowed);

  // Check whether the current thread is allowed to make IO calls,
  // and DCHECK if not.  See the block comment above the class for
  // a discussion of where to add these checks.
  static void AssertIOAllowed();
#else
  // In Release builds, inline the empty definitions of these functions so
  // that they can be compiled out.
  static bool SetIOAllowed(bool allowed) { return true; }
  static void AssertIOAllowed() {}
#endif

 private:
  ThreadRestrictions();  // class for namespacing only
};

}  // namespace base

#endif  // BASE_THREAD_RESTRICTIONS_H_
