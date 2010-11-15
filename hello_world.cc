// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include "base/at_exit.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    printf("Hello, world.\n");
    base::AtExitManager at_exit_manager;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
