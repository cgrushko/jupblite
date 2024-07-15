// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "messages.h"
#include "upb/message/copy.h"
#include "jni_helper.h"
#include <jni.h>
#include <stdint.h>
#include <array>
#include <cstring>
#include "upb/wire/decode.h"
#include "upb/wire/encode.h"
#include "upb/mini_table/internal/field.h"

// MUST be last included header.
#include "upb/port/def.inc"

namespace {
jstring _Nullable utf8ToJString(JNIEnv* env, const char* utf8) {
  if (!utf8) {
    return NULL;
  }

  jstring result = NULL;

  size_t utf8Length;
  size_t modifiedUtf8StringLength;
  uint8_t* modifiedUtf8String;

  if (modifiedUtf8String == NULL) {
    result = env->NewStringUTF((char*)utf8);
  } else {
    result = env->NewStringUTF((char*)modifiedUtf8String);
    free(modifiedUtf8String);
  }

  assertNoPendingJniException(env);
  return result;
}

} // namespace

namespace Messages {

jlong upb_Message_New(
    JNIEnv* env,
    jobject /* thisz */,
    jlong arenaPointer,
    jlong minitablesPointer) {
  return (jlong)::upb_Message_New(
      (const upb_MiniTable*) minitablesPointer,
      (upb_Arena*)arenaPointer);
}

jlong upb_Message_DeepClone(
    JNIEnv* env,
    jobject /* thisz */,
    jlong msgPointer,
    jlong minitablesPointer,
    jlong arenaPointer) {
  return (jlong)::upb_Message_DeepClone(
      (const upb_Message*) msgPointer,
      (const upb_MiniTable*) minitablesPointer,
      (upb_Arena*)arenaPointer);
}

jstring _Nullable get_UPB_PTR_AT_String_internal(
    JNIEnv* env,
    jobject /* thisz */,
    jlong stringViewPointer) {
  upb_StringView* str = ((upb_StringView*)(stringViewPointer));
  char* null_terminated = new char[str->size + 1];
  memcpy(null_terminated, str->data, str->size);
  null_terminated[str->size] = 0;
  return utf8ToJString(env, null_terminated);
}

void set_UPB_PTR_AT_String_internal(
    JNIEnv* env,
    jobject /* thisz */,
    jlong arenaPointer,
    jlong stringViewPointer,
    jbyteArray utf8String) {
  jint len = env->GetArrayLength(utf8String);
  upb_StringView* str = ((upb_StringView*)(stringViewPointer));
  void* dest = upb_Arena_Malloc((upb_Arena*)arenaPointer, len);
  ;
  if (dest == NULL) {
    // TODO: throw
  }
  str->data = (const char*)dest;
  str->size = len;

  void* data = env->GetPrimitiveArrayCritical(utf8String, NULL);
  ::memcpy(dest, data, len);
  env->ReleasePrimitiveArrayCritical(utf8String, data, JNI_ABORT);
}

jboolean getIs64(JNIEnv* env, jobject /* thisz */) {
#if UINTPTR_MAX == 0xffffffff
  return false;
#else
  return true;
#endif
}

void upb_Decode(
    JNIEnv* env,
    jobject /* thisz */,
    jbyteArray buf,
    jlong msgPointer,
    jlong minitablePointer,
    jlong arenaPointer) {
  jint len = env->GetArrayLength(buf);
  void* data = env->GetPrimitiveArrayCritical(buf, NULL);

  // upb_DecodeStatus result =
  upb_DecodeStatus result = ::upb_Decode(
      (const char*)data,
      len,
      (upb_Message*)msgPointer,
      (const upb_MiniTable*)minitablePointer,
      nullptr /* extension registry */,
      0 /* options */,
      (upb_Arena*)arenaPointer);

  env->ReleasePrimitiveArrayCritical(buf, data, JNI_ABORT);

  if (result != kUpb_DecodeStatus_Ok) {
    // TODO: throw ProtocolBufferDecodingException
  }
}

jbyteArray _Nullable upb_Encode(
    JNIEnv* env,
    jobject /* thisz */,
    jlong msgPointer,
    jlong minitablePointer,
    jlong arenaPointer) {
  size_t size;
  char* buf;
  upb_EncodeStatus result = ::upb_Encode(
      (upb_Message*)msgPointer,
      (const upb_MiniTable*)minitablePointer,
      0 /* options */,
      (upb_Arena*)arenaPointer,
      &buf,
      &size);

  if (result != kUpb_EncodeStatus_Ok) {
    // TODO: throw ProtocolBufferEncodingException
    return NULL;
  }

  if (!buf || size == 0) {
    // TODO: throw ProtocolBufferEncodingException
    return NULL;
  }

  jbyteArray ret = env->NewByteArray(size);
  if (ret == NULL) {
    return NULL; //  out of memory error thrown
  }
  env->SetByteArrayRegion(ret, 0, size, (jbyte*)buf);
  return ret;
}

namespace {
auto constexpr kClassName = "jupb/Messages";

std::array<JNINativeMethod, 7> methods = {{
    // {"_upb_Message_New", "(JIIJ)J", (void*)Messages::_upb_Message_New},    
    {"upb_Message_New", "(JJ)J", (void*)Messages::upb_Message_New},
    {"upb_Message_DeepClone", "(JJJ)J", (void*)Messages::upb_Message_DeepClone},
    {"UPB_PTR_AT_String_internal",
     "(J)Ljava/lang/String;",
     (void*)Messages::get_UPB_PTR_AT_String_internal},
    {"UPB_PTR_AT_String_internal",
     "(JJ[B)V",
     (void*)Messages::set_UPB_PTR_AT_String_internal},
    {"getIs64", "()Z", (void*)Messages::getIs64},
    {"upb_Decode", "([BJJJ)V", (void*)Messages::upb_Decode},
    {"upb_Encode", "(JJJ)[B", (void*)Messages::upb_Encode},
}};

} // namespace

void registerNatives(JNIEnv* env) {
  jclass clazz = env->FindClass(kClassName);
  if (clazz == nullptr) {
    logErrorMessageAndDie("Can't find class Messages");
  }
  assertNoPendingJniException(env);
  env->RegisterNatives(clazz, methods.begin(), methods.size());
  assertNoPendingJniException(env);
}

} // namespace Messages

#include "upb/port/undef.inc"