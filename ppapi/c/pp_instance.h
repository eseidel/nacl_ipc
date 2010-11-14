// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_INSTANCE_H_
#define PPAPI_C_PP_INSTANCE_H_

/**
 * @file
 * Defines the API ...
 *
 * @addtogroup PP
 * @{
 */

#include "ppapi/c/pp_stdint.h"

/** A PP_Instance uniquely identifies one plugin instance, which is one time
 * that a page says \<embed>. There can be multiple instances of the same plugin
 * type on a page that will all be in the same module.
 *
 * The identifier is an opaque handle assigned by the browser to the plugin. It
 * is guaranteed never to be 0, so a plugin can initialize it to 0 to
 * indicate a "NULL handle."
 */
typedef int64_t PP_Instance;

/**
 * @}
 * End addtogroup PP
 */

#endif  // PPAPI_C_PP_INSTANCE_H_
