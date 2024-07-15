// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef JAVA_CODEGEN_HELPERS_H__
#define JAVA_CODEGEN_HELPERS_H__

#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "google/protobuf/descriptor.h"

enum JavaType {
  JAVATYPE_INT,
  JAVATYPE_LONG,
  JAVATYPE_FLOAT,
  JAVATYPE_DOUBLE,
  JAVATYPE_BOOLEAN,
  JAVATYPE_STRING,
  JAVATYPE_BYTES,
  JAVATYPE_ENUM,
  JAVATYPE_MESSAGE
};

JavaType GetJavaType(const google::protobuf::FieldDescriptor *field);
absl::string_view PrimitiveTypeName(JavaType type);
std::string DefaultValue(const google::protobuf::FieldDescriptor *field);
bool IsDefaultValueJavaDefault(const google::protobuf::FieldDescriptor *field);
absl::string_view PrimitiveTypeName(const google::protobuf::FieldDescriptor *descriptor);
absl::string_view BoxedPrimitiveTypeName(JavaType type);
absl::string_view BoxedPrimitiveTypeName(const google::protobuf::FieldDescriptor *descriptor);

#endif  // JAVA_CODEGEN_HELPERS_H__
