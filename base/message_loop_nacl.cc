// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/message_loop.h"

void MessageLoop::AddDestructionObserver(DestructionObserver*) {
  NOTIMPLEMENTED();
}

bool MessageLoop::NestableTasksAllowed() const {
  NOTIMPLEMENTED();
  return false;
}

void MessageLoop::PostDelayedTask(const tracked_objects::Location&, Task*, int64) {
  NOTIMPLEMENTED();
}

void MessageLoop::PostTask(const tracked_objects::Location&, Task*) {
  NOTIMPLEMENTED();
}

void MessageLoop::QuitNow() {
  NOTIMPLEMENTED();
}

void MessageLoop::RemoveDestructionObserver(DestructionObserver*) {
  NOTIMPLEMENTED();
}

void MessageLoop::Run() {
  NOTIMPLEMENTED();
}

void MessageLoop::SetNestableTasksAllowed(bool) {
  NOTIMPLEMENTED();
}

MessageLoop* MessageLoop::current() {
  NOTIMPLEMENTED();
  return 0;
}

MessageLoop::DestructionObserver::~DestructionObserver() {
  NOTIMPLEMENTED();
}
