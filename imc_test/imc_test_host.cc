// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <spawn.h>

int send_message(int nacl_socket, char* message, int length)
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
    printf("Host Got: %i (%s)\n", rc, buffer);
    for (int x = 0; x < sizeof(buffer); x++) {
        printf("%i ", buffer[x]);
    }
    return rc;
}

void launch_nacl(int child_socket)
{
    pid_t child_pid = 0;
    const char* ldr_path = "/Projects/native_client/native_client/scons-out/opt-mac-x86-32/obj/src/trusted/service_runtime/sel_ldr";
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
    if (argc > 1)
        nacl_socket = strtol(argv[1], 0, 0);
    else {
        int fds[2];
        socketpair(AF_UNIX, SOCK_DGRAM, 0, fds);
        nacl_socket = fds[0];
        launch_nacl(fds[1]);
    }
    const char* helloMessage = "Hello there!\n";

    //send_message(nacl_socket, helloMessage, strlen(helloMessage))
    receive_message(nacl_socket);
    return 0;
}
