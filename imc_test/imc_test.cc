// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <nacl/nacl_imc.h>

using namespace nacl;

const Handle kSocketToHost = 10;

int main(int argc, char** argv) {
    printf("Foo\n");
    char buffer[100];
    int rc = Receive(kSocketToHost, buffer, 100, kDontWait);
    printf("Receive: %i\n\n", rc);
    return 0;
}
