// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "java/codegen/helpers.h"
#include "absl/strings/substitute.h"
#include "src/google/protobuf/compiler/java/names.h"

using namespace google::protobuf;
using namespace google::protobuf::compiler::java;

FieldDescriptor::Type GetType(const FieldDescriptor *field) {
  return field->type();
}

JavaType GetJavaType(const FieldDescriptor *field) {
  switch (GetType(field)) {
  case FieldDescriptor::TYPE_INT32:
  case FieldDescriptor::TYPE_UINT32:
  case FieldDescriptor::TYPE_SINT32:
  case FieldDescriptor::TYPE_FIXED32:
  case FieldDescriptor::TYPE_SFIXED32:
    return JAVATYPE_INT;

  case FieldDescriptor::TYPE_INT64:
  case FieldDescriptor::TYPE_UINT64:
  case FieldDescriptor::TYPE_SINT64:
  case FieldDescriptor::TYPE_FIXED64:
  case FieldDescriptor::TYPE_SFIXED64:
    return JAVATYPE_LONG;

  case FieldDescriptor::TYPE_FLOAT:
    return JAVATYPE_FLOAT;

  case FieldDescriptor::TYPE_DOUBLE:
    return JAVATYPE_DOUBLE;

  case FieldDescriptor::TYPE_BOOL:
    return JAVATYPE_BOOLEAN;

  case FieldDescriptor::TYPE_STRING:
    return JAVATYPE_STRING;

  case FieldDescriptor::TYPE_BYTES:
    return JAVATYPE_BYTES;

  case FieldDescriptor::TYPE_ENUM:
    return JAVATYPE_ENUM;

  case FieldDescriptor::TYPE_GROUP:
  case FieldDescriptor::TYPE_MESSAGE:
    return JAVATYPE_MESSAGE;

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return JAVATYPE_INT;
}

absl::string_view PrimitiveTypeName(JavaType type) {
  switch (type) {
  case JAVATYPE_INT:
    return "int";
  case JAVATYPE_LONG:
    return "long";
  case JAVATYPE_FLOAT:
    return "float";
  case JAVATYPE_DOUBLE:
    return "double";
  case JAVATYPE_BOOLEAN:
    return "boolean";
  case JAVATYPE_STRING:
    return "java.lang.String";
  case JAVATYPE_BYTES:
    return "com.google.protobuf.ByteString";
  case JAVATYPE_ENUM:
    return {};
  case JAVATYPE_MESSAGE:
    return {};

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return {};
}

bool AllAscii(absl::string_view text) {
  for (size_t i = 0; i < text.size(); i++) {
    if ((text[i] & 0x80) != 0) {
      return false;
    }
  }
  return true;
}

std::string DefaultValue(const FieldDescriptor *field) {
  // Switch on CppType since we need to know which default_value_* method
  // of FieldDescriptor to call.
  switch (field->cpp_type()) {
  case FieldDescriptor::CPPTYPE_INT32:
    return absl::StrCat(field->default_value_int32());
  case FieldDescriptor::CPPTYPE_UINT32:
    // Need to print as a signed int since Java has no unsigned.
    return absl::StrCat(static_cast<int32_t>(field->default_value_uint32()));
  case FieldDescriptor::CPPTYPE_INT64:
    return absl::StrCat(field->default_value_int64(), "L");
  case FieldDescriptor::CPPTYPE_UINT64:
    return absl::StrCat(static_cast<int64_t>(field->default_value_uint64())) +
           "L";
  case FieldDescriptor::CPPTYPE_DOUBLE: {
    double value = field->default_value_double();
    if (value == std::numeric_limits<double>::infinity()) {
      return "Double.POSITIVE_INFINITY";
    } else if (value == -std::numeric_limits<double>::infinity()) {
      return "Double.NEGATIVE_INFINITY";
    } else if (value != value) {
      return "Double.NaN";
    } else {
      return absl::StrCat(value, "D");
    }
  }
  case FieldDescriptor::CPPTYPE_FLOAT: {
    float value = field->default_value_float();
    if (value == std::numeric_limits<float>::infinity()) {
      return "Float.POSITIVE_INFINITY";
    } else if (value == -std::numeric_limits<float>::infinity()) {
      return "Float.NEGATIVE_INFINITY";
    } else if (value != value) {
      return "Float.NaN";
    } else {
      return absl::StrCat(value, "F");
    }
  }
  case FieldDescriptor::CPPTYPE_BOOL:
    return field->default_value_bool() ? "true" : "false";
  case FieldDescriptor::CPPTYPE_STRING:
    if (GetType(field) == FieldDescriptor::TYPE_BYTES) {
      if (field->has_default_value()) {
        // See comments in Internal.java for gory details.
        return absl::Substitute(
            "com.google.protobuf.Internal.bytesDefaultValue(\"$0\")",
            absl::CEscape(field->default_value_string()));
      } else {
        return "com.google.protobuf.ByteString.EMPTY";
      }
    } else {
      if (AllAscii(field->default_value_string())) {
        // All chars are ASCII.  In this case CEscape() works fine.
        return absl::StrCat("\"", absl::CEscape(field->default_value_string()),
                            "\"");
      } else {
        // See comments in Internal.java for gory details.
        return absl::Substitute(
            "com.google.protobuf.Internal.stringDefaultValue(\"$0\")",
            absl::CEscape(field->default_value_string()));
      }
    }

  case FieldDescriptor::CPPTYPE_ENUM:
    return absl::StrCat(ClassName(field->enum_type()), ".",
                        field->default_value_enum()->name());

  case FieldDescriptor::CPPTYPE_MESSAGE:
    return absl::StrCat(ClassName(field->message_type()),
                        ".getDefaultInstance()");

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return "";
}

bool IsDefaultValueJavaDefault(const FieldDescriptor *field) {
  // Switch on CppType since we need to know which default_value_* method
  // of FieldDescriptor to call.
  switch (field->cpp_type()) {
  case FieldDescriptor::CPPTYPE_INT32:
    return field->default_value_int32() == 0;
  case FieldDescriptor::CPPTYPE_UINT32:
    return field->default_value_uint32() == 0;
  case FieldDescriptor::CPPTYPE_INT64:
    return field->default_value_int64() == 0L;
  case FieldDescriptor::CPPTYPE_UINT64:
    return field->default_value_uint64() == 0L;
  case FieldDescriptor::CPPTYPE_DOUBLE:
    return field->default_value_double() == 0.0;
  case FieldDescriptor::CPPTYPE_FLOAT:
    return field->default_value_float() == 0.0;
  case FieldDescriptor::CPPTYPE_BOOL:
    return field->default_value_bool() == false;
  case FieldDescriptor::CPPTYPE_ENUM:
    return field->default_value_enum()->number() == 0;
  case FieldDescriptor::CPPTYPE_STRING:
  case FieldDescriptor::CPPTYPE_MESSAGE:
    return false;

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return false;
}

absl::string_view PrimitiveTypeName(const FieldDescriptor *descriptor) {
  return PrimitiveTypeName(GetJavaType(descriptor));
}

absl::string_view BoxedPrimitiveTypeName(JavaType type) {
  switch (type) {
  case JAVATYPE_INT:
    return "java.lang.Integer";
  case JAVATYPE_LONG:
    return "java.lang.Long";
  case JAVATYPE_FLOAT:
    return "java.lang.Float";
  case JAVATYPE_DOUBLE:
    return "java.lang.Double";
  case JAVATYPE_BOOLEAN:
    return "java.lang.Boolean";
  case JAVATYPE_STRING:
    return "java.lang.String";
  case JAVATYPE_BYTES:
    return "com.google.protobuf.ByteString";
  case JAVATYPE_ENUM:
    return {};
  case JAVATYPE_MESSAGE:
    return {};

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return {};
}

absl::string_view BoxedPrimitiveTypeName(const FieldDescriptor *descriptor) {
  return BoxedPrimitiveTypeName(GetJavaType(descriptor));
}
