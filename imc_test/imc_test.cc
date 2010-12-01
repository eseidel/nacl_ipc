// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <string.h>
#include <nacl/nacl_imc.h>

using namespace nacl;

const Handle kSocketToHost = 10;

void test_send()
{
    const char* message = "This is a test!\n";
    int rc = Send(kSocketToHost, message, strlen(message), kDontWait);
    printf("NaCl Sent: %i\n\n", rc);
}

void test_receive()
{
    char buffer[100];
    bzero(buffer, 100);
    int rc = Receive(kSocketToHost, buffer, 100, kDontWait);
    printf("NaCl Got: %i (%s)\n\n", rc, buffer);
}

int main(int argc, char** argv) {
    printf("Starting NaCl Process\n");
    test_send();
    //test_receive();
    return 0;
}
