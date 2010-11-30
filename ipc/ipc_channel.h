// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_H_
#define IPC_IPC_CHANNEL_H_
#pragma once

#include "base/compiler_specific.h"
#include "ipc/ipc_message.h"

namespace IPC {

//------------------------------------------------------------------------------

class Channel : public Message::Sender {
  // Security tests need access to the pipe handle.
  friend class ChannelTest;

 public:
  // Implemented by consumers of a Channel to receive messages.
  class Listener {
   public:
    virtual ~Listener() {}

    // Called when a message is received.
    virtual void OnMessageReceived(const Message& message) = 0;

    // Called when the channel is connected and we have received the internal
    // Hello message from the peer.
    virtual void OnChannelConnected(int32 peer_pid) {}

    // Called when an error is detected that causes the channel to close.
    // This method is not called when a channel is closed normally.
    virtual void OnChannelError() {}
  };

  enum Mode {
    MODE_NONE,
    MODE_SERVER,
    MODE_CLIENT,
    MODE_NAMED_SERVER,
    MODE_NAMED_CLIENT
  };

  enum {
    // The maximum message size in bytes. Attempting to receive a
    // message of this size or bigger results in a channel error.
    kMaximumMessageSize = 128 * 1024 * 1024,

    // Ammount of data to read at once from the pipe.
    kReadBufferSize = 4 * 1024
  };

  // Initialize a Channel.
  //
  // |channel_id| identifies the communication Channel.
  // |mode| specifies whether this Channel is to operate in server mode or
  // client mode.  In server mode, the Channel is responsible for setting up the
  // IPC object, whereas in client mode, the Channel merely connects to the
  // already established IPC object.
  // |listener| receives a callback on the current thread for each newly
  // received message.
  //
  Channel(const std::string& channel_id, Mode mode, Listener* listener);

  ~Channel();

  // Connect the pipe.  On the server side, this will initiate
  // waiting for connections.  On the client, it attempts to
  // connect to a pre-existing pipe.  Note, calling Connect()
  // will not block the calling thread and may complete
  // asynchronously.
  bool Connect() WARN_UNUSED_RESULT;

  // Close this Channel explicitly.  May be called multiple times.
  void Close();

  // Modify the Channel's listener.
  void set_listener(Listener* listener);

  // Send a message over the Channel to the listener on the other end.
  //
  // |message| must be allocated using operator new.  This object will be
  // deleted once the contents of the Message have been sent.
  virtual bool Send(Message* message);

#if defined(OS_POSIX) && !defined(OS_NACL)
  // On POSIX an IPC::Channel wraps a socketpair(), this method returns the
  // FD # for the client end of the socket.
  // This method may only be called on the server side of a channel.
  //
  // If the kTestingChannelID flag is specified on the command line then
  // a named FIFO is used as the channel transport mechanism rather than a
  // socketpair() in which case this method returns -1.
  int GetClientFileDescriptor() const;
#endif  // defined(OS_POSIX)

 protected:
  // Used in Chrome by the TestSink to provide a dummy channel implementation
  // for testing. TestSink overrides the "interesting" functions in Channel so
  // no actual implementation is needed. This will cause un-overridden calls to
  // segfault. Do not use outside of test code!
  Channel() : channel_impl_(0) { }

 private:
  // PIMPL to which all channel calls are delegated.
  class ChannelImpl;
  ChannelImpl *channel_impl_;

  // The Hello message is internal to the Channel class.  It is sent
  // by the peer when the channel is connected.  The message contains
  // just the process id (pid).  The message has a special routing_id
  // (MSG_ROUTING_NONE) and type (HELLO_MESSAGE_TYPE).
  enum {
    HELLO_MESSAGE_TYPE = kuint16max  // Maximum value of message type (uint16),
                                     // to avoid conflicting with normal
                                     // message types, which are enumeration
                                     // constants starting from 0.
  };
};

}  // namespace IPC

#endif  // IPC_IPC_CHANNEL_H_
