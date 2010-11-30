// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/serialized_structs.h"

#include "ppapi/c/dev/ppb_font_dev.h"
#include "ppapi/c/pp_rect.h"

namespace pp {
namespace proxy {

void SerializedFontDescription::SetFromPPFontDescription(
    Dispatcher* dispatcher,
    const PP_FontDescription_Dev& desc,
    bool source_owns_ref) {
  if (source_owns_ref)
    face = SerializedVarSendInput(dispatcher, desc.face);
  else
    SerializedVarReturnValue(&face).Return(dispatcher, desc.face);

  family = desc.family;
  size = desc.size;
  weight = desc.weight;
  italic = desc.italic;
  small_caps = desc.small_caps;
  letter_spacing = desc.letter_spacing;
  word_spacing = desc.word_spacing;
}

void SerializedFontDescription::SetToPPFontDescription(
    Dispatcher* dispatcher,
    PP_FontDescription_Dev* desc,
    bool dest_owns_ref) const {
  if (dest_owns_ref) {
    ReceiveSerializedVarReturnValue face_return_value;
    *static_cast<SerializedVar*>(&face_return_value) = face;
    desc->face = face_return_value.Return(dispatcher);
  } else {
    desc->face = SerializedVarReceiveInput(face).Get(dispatcher);
  }

  desc->family = static_cast<PP_FontFamily_Dev>(family);
  desc->size = size;
  desc->weight = static_cast<PP_FontWeight_Dev>(weight);
  desc->italic = italic;
  desc->small_caps = small_caps;
  desc->letter_spacing = letter_spacing;
  desc->word_spacing = word_spacing;
}

}  // namespace proxy
}  // namespace pp
