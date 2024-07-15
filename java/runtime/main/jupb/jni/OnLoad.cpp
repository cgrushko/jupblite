// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "arena.h"
#include "messages.h"
#include "minitable.h"
#include "jni_helper.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* _Nullable /* reserved */) {
  JNIEnv* env;

  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    logErrorMessageAndDie(
        "GetEnv failed");
  }

if (!env) {
    logErrorMessageAndDie(
        "Need to pass a valid JNIEnv pointer to initialization routine");
  }


  Arena::registerNatives(env);
  Messages::registerNatives(env);
  Minitable::registerNatives(env);
  return JNI_VERSION_1_6;
}