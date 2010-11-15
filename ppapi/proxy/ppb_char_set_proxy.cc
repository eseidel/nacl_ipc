// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/ppb_char_set_proxy.h"

#include "ppapi/c/dev/ppb_char_set_dev.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/proxy/plugin_dispatcher.h"
#include "ppapi/proxy/ppapi_messages.h"
#include "ppapi/proxy/serialized_var.h"

namespace pp {
namespace proxy {

namespace {

char* UTF16ToCharSet(const uint16_t* utf16, uint32_t utf16_len,
                     const char* output_char_set,
                     PP_CharSet_ConversionError on_error,
                     uint32_t* output_length) {
  bool output_is_success = false;
  std::string result;
  PluginDispatcher::Get()->Send(new PpapiHostMsg_PPBCharSet_UTF16ToCharSet(
      INTERFACE_ID_PPB_CHAR_SET,
      string16(reinterpret_cast<const char16*>(utf16), utf16_len),
      std::string(output_char_set), static_cast<int32_t>(on_error),
      &result, &output_is_success));
  if (!output_is_success)
    return NULL;

  *output_length = static_cast<uint32_t>(result.size());
  char* ret_val = static_cast<char*>(malloc(result.size() + 1));
  memcpy(ret_val, result.c_str(), result.size() + 1);
  return ret_val;
}

uint16_t* CharSetToUTF16(const char* input, uint32_t input_len,
                         const char* input_char_set,
                         PP_CharSet_ConversionError on_error,
                         uint32_t* output_length) {
  bool output_is_success = false;
  string16 result;
  PluginDispatcher::Get()->Send(new PpapiHostMsg_PPBCharSet_CharSetToUTF16(
      INTERFACE_ID_PPB_CHAR_SET,
      std::string(input, input_len),
      std::string(input_char_set), static_cast<int32_t>(on_error),
      &result, &output_is_success));
  if (!output_is_success)
    return NULL;

  *output_length = static_cast<uint32_t>(result.size());
  uint16_t* ret_val = static_cast<uint16_t*>(
      malloc((result.size() + 1) * sizeof(uint16_t)));
  memcpy(ret_val, result.c_str(), (result.size() + 1) * sizeof(uint16_t));
  return ret_val;
}

PP_Var GetDefaultCharSet(PP_Module pp_module) {
  ReceiveSerializedVarReturnValue result;
  PluginDispatcher::Get()->Send(new PpapiHostMsg_PPBCharSet_GetDefaultCharSet(
      INTERFACE_ID_PPB_CHAR_SET, pp_module, &result));
  return result.Return(PluginDispatcher::Get());
}

const PPB_CharSet_Dev ppb_charset_interface = {
  &UTF16ToCharSet,
  &CharSetToUTF16,
  &GetDefaultCharSet
};

}  // namespace

PPB_CharSet_Proxy::PPB_CharSet_Proxy(Dispatcher* dispatcher,
                                     const void* target_interface)
    : InterfaceProxy(dispatcher, target_interface),
      core_interface_(NULL) {
}

PPB_CharSet_Proxy::~PPB_CharSet_Proxy() {
}

const void* PPB_CharSet_Proxy::GetSourceInterface() const {
  return &ppb_charset_interface;
}

InterfaceID PPB_CharSet_Proxy::GetInterfaceId() const {
  return INTERFACE_ID_PPB_CHAR_SET;
}

void PPB_CharSet_Proxy::OnMessageReceived(const IPC::Message& msg) {
  IPC_BEGIN_MESSAGE_MAP(PPB_CharSet_Proxy, msg)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBCharSet_UTF16ToCharSet,
                        OnMsgUTF16ToCharSet)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBCharSet_CharSetToUTF16,
                        OnMsgCharSetToUTF16)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBCharSet_GetDefaultCharSet,
                        OnMsgGetDefaultCharSet)
  IPC_END_MESSAGE_MAP()
}

void PPB_CharSet_Proxy::OnMsgUTF16ToCharSet(const string16& utf16,
                                            const std::string& char_set,
                                            int32_t on_error,
                                            std::string* output,
                                            bool* output_is_success) {
  uint32_t output_len = 0;
  char* result = ppb_char_set_target()->UTF16ToCharSet(
      reinterpret_cast<const uint16_t*>(utf16.c_str()),
      static_cast<uint32_t>(utf16.size()),
      char_set.c_str(), static_cast<PP_CharSet_ConversionError>(on_error),
      &output_len);
  if (result) {
    output->assign(result, output_len);
    GetCoreInterface()->MemFree(result);
    *output_is_success = true;
  } else {
    *output_is_success = false;
  }
}

void PPB_CharSet_Proxy::OnMsgCharSetToUTF16(const std::string& input,
                                            const std::string& char_set,
                                            int32_t on_error,
                                            string16* output,
                                            bool* output_is_success) {
  uint32_t output_len = 0;
  uint16_t* result = ppb_char_set_target()->CharSetToUTF16(
      input.c_str(), static_cast<uint32_t>(input.size()),
      char_set.c_str(), static_cast<PP_CharSet_ConversionError>(on_error),
      &output_len);
  if (result) {
    output->assign(reinterpret_cast<const char16*>(result), output_len);
    GetCoreInterface()->MemFree(result);
    *output_is_success = true;
  } else {
    *output_is_success = false;
  }
}

void PPB_CharSet_Proxy::OnMsgGetDefaultCharSet(
    PP_Module module,
    SerializedVarReturnValue result) {
  result.Return(dispatcher(),
                ppb_char_set_target()->GetDefaultCharSet(module));
}

const PPB_Core* PPB_CharSet_Proxy::GetCoreInterface() {
  if (!core_interface_) {
    core_interface_ = static_cast<const PPB_Core*>(
        dispatcher()->GetLocalInterface(PPB_CORE_INTERFACE));
  }
  return core_interface_;
}

}  // namespace proxy
}  // namespace pp
