// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/weak_ptr.h"

namespace base {
namespace internal {

WeakReference::Flag::Flag(Flag** handle) : handle_(handle) {
}

WeakReference::Flag::~Flag() {
  if (handle_)
    *handle_ = NULL;
}

void WeakReference::Flag::AddRef() const {
  DCHECK(CalledOnValidThread());
  RefCounted<Flag>::AddRef();
}

void WeakReference::Flag::Release() const {
  DCHECK(CalledOnValidThread());
  RefCounted<Flag>::Release();
}

WeakReference::WeakReference() {
}

WeakReference::WeakReference(Flag* flag) : flag_(flag) {
}

WeakReference::~WeakReference() {
}

bool WeakReference::is_valid() const {
  return flag_ && flag_->is_valid();
}

WeakReferenceOwner::WeakReferenceOwner() : flag_(NULL) {
}

WeakReferenceOwner::~WeakReferenceOwner() {
  Invalidate();
}

WeakReference WeakReferenceOwner::GetRef() const {
  if (!flag_)
    flag_ = new WeakReference::Flag(&flag_);
  return WeakReference(flag_);
}

void WeakReferenceOwner::Invalidate() {
  if (flag_) {
    flag_->Invalidate();
    flag_ = NULL;
  }
}

WeakPtrBase::WeakPtrBase() {
}

WeakPtrBase::WeakPtrBase(const WeakReference& ref) : ref_(ref) {
}

WeakPtrBase::~WeakPtrBase() {
}

}  // namespace internal
}  // namespace base
