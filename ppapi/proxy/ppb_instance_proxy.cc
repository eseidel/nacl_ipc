// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/ppb_instance_proxy.h"

#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/proxy/plugin_dispatcher.h"
#include "ppapi/proxy/ppapi_messages.h"
#include "ppapi/proxy/serialized_var.h"

namespace pp {
namespace proxy {

namespace {

PP_Var GetWindowObject(PP_Instance instance) {
  Dispatcher* dispatcher = PluginDispatcher::Get();
  ReceiveSerializedVarReturnValue result;
  dispatcher->Send(new PpapiHostMsg_PPBInstance_GetWindowObject(
      INTERFACE_ID_PPB_INSTANCE, instance, &result));
  return result.Return(dispatcher);
}

PP_Var GetOwnerElementObject(PP_Instance instance) {
  Dispatcher* dispatcher = PluginDispatcher::Get();
  ReceiveSerializedVarReturnValue result;
  dispatcher->Send(new PpapiHostMsg_PPBInstance_GetOwnerElementObject(
          INTERFACE_ID_PPB_INSTANCE, instance, &result));
  return result.Return(dispatcher);
}

PP_Bool BindGraphics(PP_Instance instance, PP_Resource device) {
  PP_Bool result;
  PluginDispatcher::Get()->Send(new PpapiHostMsg_PPBInstance_BindGraphics(
      INTERFACE_ID_PPB_INSTANCE, instance, device, &result));
  return result;
}

PP_Bool IsFullFrame(PP_Instance instance) {
  PP_Bool result;
  PluginDispatcher::Get()->Send(new PpapiHostMsg_PPBInstance_IsFullFrame(
      INTERFACE_ID_PPB_INSTANCE, instance, &result));
  return result;
}

PP_Var ExecuteScript(PP_Instance instance, PP_Var script, PP_Var* exception) {
  Dispatcher* dispatcher = PluginDispatcher::Get();
  ReceiveSerializedException se(dispatcher, exception);
  if (se.IsThrown())
    return PP_MakeUndefined();

  ReceiveSerializedVarReturnValue result;
  dispatcher->Send(new PpapiHostMsg_PPBInstance_ExecuteScript(
      INTERFACE_ID_PPB_INSTANCE, instance,
      SerializedVarSendInput(dispatcher, script), &se, &result));
  return result.Return(dispatcher);
}

const PPB_Instance instance_interface = {
  &GetWindowObject,
  &GetOwnerElementObject,
  &BindGraphics,
  &IsFullFrame,
  &ExecuteScript
};

}  // namespace

PPB_Instance_Proxy::PPB_Instance_Proxy(Dispatcher* dispatcher,
                                       const void* target_interface)
    : InterfaceProxy(dispatcher, target_interface) {
}

PPB_Instance_Proxy::~PPB_Instance_Proxy() {
}

const void* PPB_Instance_Proxy::GetSourceInterface() const {
  return &instance_interface;
}

InterfaceID PPB_Instance_Proxy::GetInterfaceId() const {
  return INTERFACE_ID_PPB_INSTANCE;
}

void PPB_Instance_Proxy::OnMessageReceived(const IPC::Message& msg) {
  IPC_BEGIN_MESSAGE_MAP(PPB_Instance_Proxy, msg)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBInstance_GetWindowObject,
                        OnMsgGetWindowObject)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBInstance_GetOwnerElementObject,
                        OnMsgGetOwnerElementObject)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBInstance_BindGraphics,
                        OnMsgBindGraphics)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBInstance_IsFullFrame,
                        OnMsgIsFullFrame)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_PPBInstance_ExecuteScript,
                        OnMsgExecuteScript)
  IPC_END_MESSAGE_MAP()
}

void PPB_Instance_Proxy::OnMsgGetWindowObject(
    PP_Instance instance,
    SerializedVarReturnValue result) {
  result.Return(dispatcher(),
                ppb_instance_target()->GetWindowObject(instance));
}

void PPB_Instance_Proxy::OnMsgGetOwnerElementObject(
    PP_Instance instance,
    SerializedVarReturnValue result) {
  result.Return(dispatcher(),
                ppb_instance_target()->GetOwnerElementObject(instance));
}

void PPB_Instance_Proxy::OnMsgBindGraphics(PP_Instance instance,
                                           PP_Resource device,
                                           PP_Bool* result) {
  *result = ppb_instance_target()->BindGraphics(instance, device);
}

void PPB_Instance_Proxy::OnMsgIsFullFrame(PP_Instance instance,
                                          PP_Bool* result) {
  *result = ppb_instance_target()->IsFullFrame(instance);
}

void PPB_Instance_Proxy::OnMsgExecuteScript(
    PP_Instance instance,
    SerializedVarReceiveInput script,
    SerializedVarOutParam out_exception,
    SerializedVarReturnValue result) {
  result.Return(dispatcher(), ppb_instance_target()->ExecuteScript(
      instance,
      script.Get(dispatcher()),
      out_exception.OutParam(dispatcher())));
}

}  // namespace proxy
}  // namespace pp
