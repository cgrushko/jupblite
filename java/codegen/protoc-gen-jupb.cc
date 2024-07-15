// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <string>

#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "google/protobuf/descriptor.pb.h"
#include "inja-3.4.0/inja.hpp"
#include "nlohmann/json.hpp"
#include "src/google/protobuf/compiler/java/names.h"
#include "upb/mini_table/internal/field.h"
#include "upb/util/def_to_proto.h"
#include "upb_generator/common.h"
#include "upb_generator/file_layout.h"
#include "upb_generator/plugin.h"
#include "java/codegen/helpers.h"
#include "tools/cpp/runfiles/runfiles.h"

#include "upb/port/def.inc"

using namespace inja;
using namespace upb::generator;
using json = nlohmann::json;
using namespace google::protobuf::compiler::java;
using namespace google::protobuf;
using bazel::tools::cpp::runfiles::Runfiles;

namespace {

struct JupbCompilerOptions {
  bool lite = false;
};

bool ParseOptions(Plugin *plugin, JupbCompilerOptions *options) {
  for (const auto &pair : ParseGeneratorParameter(plugin->parameter())) {
    if (pair.first == "lite") {
      options->lite = true;
    } else {
      plugin->SetError(absl::Substitute("Unknown parameter: $0", pair.first));
      return false;
    }
  }

  if (!options->lite) {
    plugin->SetError("Only 'lite' is currently supported");
    return false;
  }

  return true;
}

google::protobuf::FileDescriptorProto upbFileProtoToCppFileDescriptorProto(
    const UPB_DESC(FileDescriptorProto) * file_proto) {
  upb::Arena arena;
  size_t serialized_size;
  const char *serialized = google_protobuf_FileDescriptorProto_serialize(
      file_proto, arena.ptr(), &serialized_size);

  google::protobuf::FileDescriptorProto cppFileDescriptorProto;
  cppFileDescriptorProto.ParseFromArray(serialized, serialized_size);

  return cppFileDescriptorProto;
}

std::string onlyLastName(const std::string &javafqn) {
  int lastDot = javafqn.find_last_of(".");
  if (lastDot == -1)
    return javafqn;
  return javafqn.substr(javafqn.find_last_of(".") + 1);
}

absl::string_view PrimitiveGetter(JavaType type) {
  switch (type) {
  case JAVATYPE_INT:
    return "getInt";
  case JAVATYPE_LONG:
    return "getLong";
  case JAVATYPE_FLOAT:
    return "getFloat";
  case JAVATYPE_DOUBLE:
    return "getDouble";
  case JAVATYPE_BOOLEAN:
    return "getBoolean";
  case JAVATYPE_STRING:
    return "getString";
  case JAVATYPE_BYTES:
    return "getByteString";
  case JAVATYPE_ENUM:
    return "";
  case JAVATYPE_MESSAGE:
    return "";

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return {};
}

absl::string_view PrimitiveSetter(JavaType type) {
  switch (type) {
  case JAVATYPE_INT:
    return "setInt";
  case JAVATYPE_LONG:
    return "setLong";
  case JAVATYPE_FLOAT:
    return "setFloat";
  case JAVATYPE_DOUBLE:
    return "setDouble";
  case JAVATYPE_BOOLEAN:
    return "setBoolean";
  case JAVATYPE_STRING:
    return "setString";
  case JAVATYPE_BYTES:
    return "setByteString";
  case JAVATYPE_ENUM:
    return "";
  case JAVATYPE_MESSAGE:
    return "";

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  ABSL_LOG(FATAL) << "Can't get here.";
  return {};
}

json processField(const upb::FieldDefPtr &field32,
                  const upb::FieldDefPtr &field64,
                  const google::protobuf::FieldDescriptor *cppFieldDescriptor) {
  json result{{"kind", "unset"}};

  result["deprecated"] = cppFieldDescriptor->options().deprecated();
  result["name"] = CapitalizedFieldName(cppFieldDescriptor);

  // todo: remove once filled by all branches below
  result["type"] = "undefined_yet";

  if (!IsDefaultValueJavaDefault(cppFieldDescriptor)) {
    result["default"] = DefaultValue(cppFieldDescriptor);
  }

  bool hasbit = upb_MiniTableField_HasPresence(field32.mini_table());
  result["has_presence"] = hasbit;
  if (hasbit) {
    result["hasbit_offset32"] =
        UPB_PRIVATE(_upb_MiniTableField_HasbitOffset)(field32.mini_table());
    result["hasbit_mask32"] =
        UPB_PRIVATE(_upb_MiniTableField_HasbitMask)(field32.mini_table());
    result["hasbit_offset64"] =
        UPB_PRIVATE(_upb_MiniTableField_HasbitOffset)(field64.mini_table());
    result["hasbit_mask64"] =
        UPB_PRIVATE(_upb_MiniTableField_HasbitMask)(field64.mini_table());
  }
  result["offset32"] =
      UPB_PRIVATE(_upb_MiniTableField_Offset)(field32.mini_table());
  result["offset64"] =
      UPB_PRIVATE(_upb_MiniTableField_Offset)(field64.mini_table());

  if (field32.IsSequence()) {

  } else {
    if (field32.IsPrimitive()) {
      result["kind"] = "singular_primitive";
      JavaType javaType = GetJavaType(cppFieldDescriptor);
      result["type"] = PrimitiveTypeName(javaType);
      result["getter"] = PrimitiveGetter(javaType);
      result["setter"] = PrimitiveSetter(javaType);
    }

    if (field32.IsSubMessage()) {
      result["kind"] = "singular_message";
      result["type"] = ClassName(cppFieldDescriptor->message_type());
      result["getter"] = "getMessage";
      result["setter"] = "setMessage";
    }
  }
  return result;
}

json collectSubmessagesForMinitable(
    const upb::MessageDefPtr &message,
    const google::protobuf::Descriptor *cppMessageDescriptor) {
  json result = json::array();
  for (int i = 0; i < cppMessageDescriptor->field_count(); i++) {
    const FieldDescriptor *field = cppMessageDescriptor->field(i);
    if (field->type() == FieldDescriptor::TYPE_MESSAGE) {
      result.emplace_back(json::object(
          {{"index", i},
           {"classname",
            ClassName(cppMessageDescriptor->field(i)->message_type())}}));
    }
  }
  return result;
}

json processMessage(const upb::MessageDefPtr &message32,
                    const upb::MessageDefPtr &message64,
                    const google::protobuf::Descriptor *cppMessageDescriptor) {
  json result;
  result["Classname"] = onlyLastName(ClassName(cppMessageDescriptor));
  result["deprecated"] = cppMessageDescriptor->options().deprecated();
  result["mini_descriptor"] = message32.MiniDescriptorEncode();
  if (message32.MiniDescriptorEncode() != message64.MiniDescriptorEncode()) {
    // sanity check - remove when other code is ready.
    ABSL_LOG(FATAL) << "what";
  }
  result["message_types_to_link"] =
      collectSubmessagesForMinitable(message32, cppMessageDescriptor);

  json fields = json::array();
  for (int i = 0; i < message32.field_count(); i++) {
    fields.emplace_back(processField(message32.field(i), message64.field(i),
                                     cppMessageDescriptor->field(i)));
  }
  result["fields"] = fields;

  json nested_messages = json::array();
  for (int i = 0; i < message32.nested_message_count(); i++) {
    nested_messages.emplace_back(
        processMessage(message32.nested_message(i), message64.nested_message(i),
                       cppMessageDescriptor->nested_type(i)));
  }
  result["nested_messages"] = nested_messages;

  return result;
}

std::string JavaPackageToDir(std::string package_name) {
  std::string package_dir = absl::StrReplaceAll(package_name, {{".", "/"}});
  if (!package_dir.empty())
    absl::StrAppend(&package_dir, "/");
  return package_dir;
}

void GenerateFile(const Runfiles* runfiles, 
            const upb::FileDefPtr &file32, const upb::FileDefPtr &file64,
                  const google::protobuf::FileDescriptor *cppFileDescriptor,
                  const JupbCompilerOptions &options, Plugin *plugin) {

  Environment env(runfiles->Rlocation("_main/java/codegen/"));

  env.add_callback("subtemplate", 3, [&env](Arguments &args) {
    auto filename = args[0]->get<std::string>();
    Template temp = env.parse_template(filename);
    json data;
    data[args[1]->get<std::string>()] = *args[2];
    return env.render(temp, data);
  });

  json data;
  data["filename"] = file32.name();
  data["deprecated"] = google_protobuf_FileOptions_deprecated(file32.options());
  std::string javaPackage = FileJavaPackage(
      cppFileDescriptor, google::protobuf::compiler::java::Options());
  data["java_package"] = javaPackage;
  std::string className = onlyLastName(ClassName(cppFileDescriptor));
  data["Classname"] = className;

  std::vector<json> messages;
  for (int i = 0; i < file32.toplevel_message_count(); i++) {
    messages.push_back(processMessage(file32.toplevel_message(i),
                                      file64.toplevel_message(i),
                                      cppFileDescriptor->message_type(i)));
  }
  data["messages"] = messages;

  std::string package_dir = JavaPackageToDir(javaPackage);
  std::string java_filename = absl::StrCat(package_dir, className, ".java");
  plugin->AddOutputFile(java_filename,
                        env.render_file("file_template.inja", data));
}

absl::string_view ToStringView(upb_StringView str) {
  return absl::string_view(str.data, str.size);
}

} // namespace

int main(int argc, char **argv) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0], &error));
  if (runfiles == nullptr) {
          ABSL_LOG(FATAL) << "can't find runfiles. Error: " << error;
  }
  upb::generator::Plugin plugin;
  JupbCompilerOptions options;
  if (!ParseOptions(&plugin, &options))
    return 0;
  google::protobuf::DescriptorPool cppPool;
  upb::DefPool pool32;
  upb::DefPool pool64;
  pool32._SetPlatform(kUpb_MiniTablePlatform_32Bit);
  pool64._SetPlatform(kUpb_MiniTablePlatform_64Bit);
  plugin.GenerateFilesRaw(
      [&](const UPB_DESC(FileDescriptorProto) * file_proto, bool generate) {
        const google::protobuf::FileDescriptor *cppFileDescriptor =
            cppPool.BuildFile(upbFileProtoToCppFileDescriptorProto(file_proto));
        upb::Status status;
        upb::FileDefPtr file32 = pool32.AddFile(file_proto, &status);
        if (!file32) {
          absl::string_view name =
              ToStringView(UPB_DESC(FileDescriptorProto_name)(file_proto));
          ABSL_LOG(FATAL) << "Couldn't add file " << name
                          << " to DefPool: " << status.error_message();
        }
        upb::FileDefPtr file64 = pool64.AddFile(file_proto, &status);

        if (generate) {
          GenerateFile(runfiles.get(), file32, file64, cppFileDescriptor, options, &plugin);
        }
      });
  return 0;
}
