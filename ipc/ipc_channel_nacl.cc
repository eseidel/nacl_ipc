// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ipc/ipc_channel.h"

namespace IPC {

Channel::Channel(const std::string&, Mode, Listener*) {
  NOTIMPLEMENTED();
}

Channel::~Channel() {
  NOTIMPLEMENTED();
}

bool Channel::Connect() {
  NOTIMPLEMENTED();
  return false;
}

bool Channel::Send(Message*) {
  NOTIMPLEMENTED();
  return false;
}

}
