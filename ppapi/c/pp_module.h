// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_MODULE_H_
#define PPAPI_C_PP_MODULE_H_

/**
 * @file
 * Defines the API ...
 *
 * @addtogroup PP
 * @{
 */

#include "ppapi/c/pp_stdint.h"

/**
 * A module uniquely identifies one plugin library. The identifier is an opaque
 * handle assigned by the browser to the plugin. It is guaranteed never to be
 * 0, so a plugin can initialize it to 0 to indicate a "NULL handle."
 */
typedef int64_t PP_Module;

/**
 * @}
 * End addtogroup PP
 */

#endif  // PPAPI_C_PP_MODULE_H_
