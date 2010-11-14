// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/message_loop.h"
#include "base/metrics/histogram.h"

MessageLoop::MessageLoop(Type type)
  : type_(type)
{
  NOTIMPLEMENTED();
}

MessageLoop::~MessageLoop() {
  NOTIMPLEMENTED();
}

void MessageLoop::AddDestructionObserver(DestructionObserver*) {
  NOTIMPLEMENTED();
}

void MessageLoop::RemoveDestructionObserver(DestructionObserver*) {
  NOTIMPLEMENTED();
}

MessageLoop::DestructionObserver::~DestructionObserver() {
  NOTIMPLEMENTED();
}

bool MessageLoop::DoWork() {
  NOTIMPLEMENTED();
  return false;
}

bool MessageLoop::DoDelayedWork(base::TimeTicks*) {
  NOTIMPLEMENTED();
  return false;
}

bool MessageLoop::DoIdleWork() {
  NOTIMPLEMENTED();
  return false;
}

void MessageLoop::PostDelayedTask(const tracked_objects::Location&, Task*, int64) {
  NOTIMPLEMENTED();
}

void MessageLoop::PostTask(const tracked_objects::Location&, Task*) {
  NOTIMPLEMENTED();
}

void MessageLoop::PostNonNestableTask(tracked_objects::Location const&, Task*) {
  NOTIMPLEMENTED();
}

void MessageLoop::PostNonNestableDelayedTask(tracked_objects::Location const&, Task*, long long) {
  NOTIMPLEMENTED();
}

void MessageLoop::Run() {
  NOTIMPLEMENTED();
}

void MessageLoop::RunAllPending() {
  NOTIMPLEMENTED();
}

void MessageLoop::Quit() {
  NOTIMPLEMENTED();
}

void MessageLoop::QuitNow() {
  NOTIMPLEMENTED();
}

bool MessageLoop::NestableTasksAllowed() const {
  NOTIMPLEMENTED();
  return false;
}

void MessageLoop::SetNestableTasksAllowed(bool) {
  NOTIMPLEMENTED();
}

MessageLoop* MessageLoop::current() {
  // Just enough to make a few more unit tests not crash,
  // but isn't a real implementation of this function.
  static MessageLoop* sDummyLoop = new MessageLoop();
  NOTIMPLEMENTED();
  return sDummyLoop;
}

bool MessageLoop::PendingTask::operator<(const PendingTask&) const {
  NOTIMPLEMENTED();
  return false;
}
