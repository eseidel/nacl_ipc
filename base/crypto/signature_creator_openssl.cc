// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/crypto/signature_creator.h"

#include <openssl/evp.h>

#include "base/logging.h"
#include "base/openssl_util.h"
#include "base/scoped_ptr.h"

namespace base {

// static
SignatureCreator* SignatureCreator::Create(RSAPrivateKey* key) {
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  scoped_ptr<SignatureCreator> result(new SignatureCreator);
  result->key_ = key;
  if (!EVP_SignInit_ex(result->sign_context_, EVP_sha1(), NULL))
    return NULL;
  return result.release();
}

SignatureCreator::SignatureCreator()
    : sign_context_(EVP_MD_CTX_create()) {
}

SignatureCreator::~SignatureCreator() {
  EVP_MD_CTX_destroy(sign_context_);
}

bool SignatureCreator::Update(const uint8* data_part, int data_part_len) {
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  return EVP_SignUpdate(sign_context_, data_part, data_part_len) == 1;
}

bool SignatureCreator::Final(std::vector<uint8>* signature) {
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  EVP_PKEY* key = key_->key();
  signature->resize(EVP_PKEY_size(key));

  unsigned int len = 0;
  int rv = EVP_SignFinal(sign_context_, signature->data(), &len, key);
  if (!rv) {
    signature->clear();
    return false;
  }
  signature->resize(len);
  return true;
}

}  // namespace base
