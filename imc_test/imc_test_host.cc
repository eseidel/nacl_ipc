// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <spawn.h>

namespace nacl {

typedef int Handle; /** < PENDING: doc */
const Handle kInvalidHandle(-1); /** < PENDING: doc */

struct IOVec {
  void*   base;
  size_t  length;
};
const size_t kHandleCountMax = 8;   // TBD
const int kMessageTruncated = 0x1;  /**< The trailing portion of a message was
                                     *   discarded. */
const int kHandlesTruncated = 0x2;  /**< Not all the handles were received. */

struct MessageHeader {
  IOVec*    iov;            /**< scatter/gather array */
  uint32_t  iov_length;     /**< number of elements in iov */
  Handle*   handles;        /**< array of handles to be transferred */
  uint32_t  handle_count;   /**< number of handles in handles */
  int       flags;
};

const int kDontWait = 0x1;  /**< Enables non-blocking operation */

int SocketPair(Handle pair[2]);
int SendDatagram(Handle socket, const MessageHeader* message, int flags);
int Send(Handle socket, const void* buffer, size_t length, int flags);
int ReceiveDatagram(Handle socket, MessageHeader* message, int flags);
int Receive(Handle socket, void* buffer, size_t length, int flags);

}  // namespace nacl

using namespace nacl;

int send_message(int nacl_socket, const char* message, int length)
{
    struct iovec iov;
    bzero(&iov, sizeof(iov));
    iov.iov_base = (void*)message;
    iov.iov_len = length;

    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int rc = sendmsg(nacl_socket, &msg, 0);
    printf("Host Sent: %i\n", rc);
    return rc;
}

int receive_message(int nacl_socket)
{
    char buffer[80];
    bzero(&buffer, sizeof(buffer));

    struct iovec iov;
    bzero(&iov, sizeof(iov));
    iov.iov_base = buffer;
    iov.iov_len  = sizeof(buffer);
    
    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    
    int rc = recvmsg(nacl_socket, &msg, 0);

    printf("Host waiting\n");
    //int rc = Receive(nacl_socket, buffer, 80, kDontWait);
    // For some reason we seem to be getting a msg structure in the iov??
    printf("Host Got: %i (%s)\n", rc, buffer);
    for (int x = 0; x < sizeof(buffer); x++) {
        printf("%i ", buffer[x]);
    }
    return rc;
}

// This function exists to rule out that the python code was doing the socket setup incorrectly.
void launch_nacl(int child_socket)
{
    pid_t child_pid = 0;
    const char* ldr_path = getenv("LDR");
    if (!ldr_path) {
        printf("LDR environment variable required\n");
        exit(1);
    }
    const char* nexe_path = "imc_test.nexe";
    const char* kChildIMCId = "10";
    char* imc_map_string;
    asprintf(&imc_map_string, "%s:%i", kChildIMCId, child_socket);
    const char* const argv[] = { "sel_ldr", "-i", imc_map_string, "--", nexe_path, 0};
    int rc = posix_spawn(&child_pid, ldr_path, NULL, NULL, (char**)argv, NULL);
    printf("Launched nacl: %i\n", child_pid);
}

int main(int argc, char** argv) {
    printf("Starting Host Process\n");
    int nacl_socket;
    if (argc > 1) {
        nacl_socket = strtol(argv[1], 0, 0);
        printf("Using pre-launched nacl on socket: %i\n", nacl_socket);
    } else {
        int fds[2];
        int rc = SocketPair(fds);
        printf("Created socket pair.  rc= %i\n", rc);
        nacl_socket = fds[0];
        launch_nacl(fds[1]);
    }
    const char* helloMessage = "Hello there!\n";
    //send_message(nacl_socket, helloMessage, strlen(helloMessage));

    receive_message(nacl_socket);
    return 0;
}
