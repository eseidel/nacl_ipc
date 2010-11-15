// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PPB_FONT_PROXY_H_
#define PPAPI_PROXY_PPB_FONT_PROXY_H_

#include "base/basictypes.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/proxy/interface_proxy.h"

struct PPB_Font_Dev;

namespace pp {
namespace proxy {

struct PPBFont_DrawTextAt_Params;
struct SerializedFontDescription;
class SerializedVarReceiveInput;

class PPB_Font_Proxy : public InterfaceProxy {
 public:
  PPB_Font_Proxy(Dispatcher* dispatcher, const void* target_interface);
  virtual ~PPB_Font_Proxy();

  const PPB_Font_Dev* ppb_font_target() const {
    return static_cast<const PPB_Font_Dev*>(target_interface());
  }

  // InterfaceProxy implementation.
  virtual const void* GetSourceInterface() const;
  virtual InterfaceID GetInterfaceId() const;
  virtual void OnMessageReceived(const IPC::Message& msg);

 private:
  // Message handlers.
  void OnMsgCreate(PP_Module pp_module,
                   const SerializedFontDescription& in_description,
                   PP_Resource* result,
                   SerializedFontDescription* out_description,
                   std::string* out_metrics);
  void OnMsgDrawTextAt(SerializedVarReceiveInput text,
                       const PPBFont_DrawTextAt_Params& params,
                       PP_Bool* result);
  void OnMsgMeasureText(PP_Resource font,
                        SerializedVarReceiveInput text,
                        PP_Bool text_is_rtl,
                        PP_Bool override_direction,
                        int32_t* result);
  void OnMsgCharacterOffsetForPixel(PP_Resource font,
                                    SerializedVarReceiveInput text,
                                    PP_Bool text_is_rtl,
                                    PP_Bool override_direction,
                                    int32_t pixel_pos,
                                    uint32_t* result);
  void OnMsgPixelOffsetForCharacter(PP_Resource font,
                                    SerializedVarReceiveInput text,
                                    PP_Bool text_is_rtl,
                                    PP_Bool override_direction,
                                    uint32_t char_offset,
                                    int32_t* result);

  DISALLOW_COPY_AND_ASSIGN(PPB_Font_Proxy);
};

}  // namespace proxy
}  // namespace pp

#endif  // PPAPI_PROXY_PPB_FONT_PROXY_H_
