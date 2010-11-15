// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPB_TESTING_PROXY_H_
#define PPAPI_PROXY_PPB_TESTING_PROXY_H_

#include "base/basictypes.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/proxy/interface_proxy.h"

struct PP_Point;
struct PPB_Testing_Dev;

namespace pp {
namespace proxy {

class PPB_Testing_Proxy : public InterfaceProxy {
 public:
  PPB_Testing_Proxy(Dispatcher* dispatcher, const void* target_interface);
  virtual ~PPB_Testing_Proxy();

  const PPB_Testing_Dev* ppb_testing_target() const {
    return static_cast<const PPB_Testing_Dev*>(target_interface());
  }

  // InterfaceProxy implementation.
  virtual const void* GetSourceInterface() const;
  virtual InterfaceID GetInterfaceId() const;
  virtual void OnMessageReceived(const IPC::Message& msg);

 private:
  // Message handlers.
  void OnMsgReadImageData(PP_Resource device_context_2d,
                          PP_Resource image,
                          const PP_Point& top_left,
                          PP_Bool* result);
  void OnMsgRunMessageLoop(bool* dummy);
  void OnMsgQuitMessageLoop();
  void OnMsgGetLiveObjectCount(PP_Module module_id, uint32_t* result);

  DISALLOW_COPY_AND_ASSIGN(PPB_Testing_Proxy);
};

}  // namespace proxy
}  // namespace pp

#endif  // PPAPI_PROXY_PPB_TESTING_PROXY_H_
