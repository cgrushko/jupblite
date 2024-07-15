// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "minitable.h"
#include "jni_helper.h"
#include "upb/mini_descriptor/decode.h"
#include "upb/mini_descriptor/link.h"
#include "upb/mini_table/message.h"
#include <array>
#include <stdio.h>
#include <stdlib.h>

namespace Minitable {

jlong upb_MiniTable_Build(JNIEnv *env, jobject /* thisz */,
                          jbyteArray serializedMinitable, jlong arenaPtr) {
  void *data = env->GetPrimitiveArrayCritical(serializedMinitable, NULL);
  assertNoPendingJniException(env);
  jsize len = env->GetArrayLength(serializedMinitable);
  assertNoPendingJniException(env);

  upb_Status status;
  jlong result = (jlong)upb_MiniTable_Build((const char *)data, len,
                                            (upb_Arena *)arenaPtr, &status);
  if (!result) {
    logErrorMessageAndDie(status.msg);
  }
  env->ReleasePrimitiveArrayCritical(serializedMinitable, data, JNI_ABORT);
  assertNoPendingJniException(env);
  return result;
}

void upb_MiniTable_SetSubMessage(JNIEnv *env, jobject /* thisz */,
                                 jlong minitable, jint fieldIndex,
                                 jlong submessageMinitable) {
  bool result = upb_MiniTable_SetSubMessage(
      (upb_MiniTable *)minitable,
      (upb_MiniTableField*) upb_MiniTable_GetFieldByIndex((upb_MiniTable *)minitable, fieldIndex),
      (const upb_MiniTable *)submessageMinitable);
  if (!result) {
    logErrorMessageAndDie("setSubMessage failed");
  }
}

namespace {
auto constexpr kClassName = "jupb/Minitable";

std::array<JNINativeMethod, 2> methods = {{
    // long upb_MiniTable_Build(byte[] serializedMinitable, long arenaPtr);
    {"upb_MiniTable_Build", "([BJ)J", (void *)Minitable::upb_MiniTable_Build},
    // void upb_MiniTable_SetSubMessage(long minitable, int fieldIndex, long
    // submessageMinitable);
    {"upb_MiniTable_SetSubMessage", "(JIJ)V",
     (void *)Minitable::upb_MiniTable_SetSubMessage},
}};

} // namespace

void registerNatives(JNIEnv *env) {
  jclass clazz = env->FindClass(kClassName);
  assertNoPendingJniException(env);
  env->RegisterNatives(clazz, methods.begin(), methods.size());
  assertNoPendingJniException(env);
}

} // namespace Minitable
