// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "arena.h"
#include "jni_helper.h"
#include "upb/mem/arena.h"
#include <array>
#include <stdio.h>
#include <stdlib.h>

namespace Arena {

jlong initNative(JNIEnv* env, jobject /* thisz */) {
  return (jlong) upb_Arena_New();
}

void freeUpbArena(JNIEnv* env, jobject thisz, jlong upbArenaPointer) {
  upb_Arena_Free((upb_Arena*)upbArenaPointer);
}

void upb_Arena_Fuse(JNIEnv* env, jobject /* thisz */, jlong pointer1, jlong pointer2) {
  upb_Arena_Fuse((upb_Arena*)pointer1, (upb_Arena*)pointer2);
}

namespace {
auto constexpr kClassName = "jupb/Arena";

std::array<JNINativeMethod, 3> methods = {{
    {"initNative",
     "()J",
     (void*)Arena::initNative},
    {"freeUpbArena", "(J)V", (void*)Arena::freeUpbArena},
    {"upb_Arena_Fuse", "(JJ)V", (void*)Arena::upb_Arena_Fuse},
}};

} // namespace

void registerNatives(JNIEnv* env) {
  jclass clazz = env->FindClass(kClassName);
  assertNoPendingJniException(env);
  env->RegisterNatives(clazz, methods.begin(), methods.size());
  assertNoPendingJniException(env);
}

} // namespace Arena
