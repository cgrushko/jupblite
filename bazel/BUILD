load("@rules_proto//proto:defs.bzl", "proto_lang_toolchain", "proto_library")

proto_lang_toolchain(
    name = "jupblite_toolchain",
    # keep this in sync w/ LITE_WELL_KNOWN_PROTO_MAP in //:BUILD
    blacklisted_protos = [
        "@com_google_protobuf//:any_proto",
        "@com_google_protobuf//:api_proto",
        "@com_google_protobuf//:duration_proto",
        "@com_google_protobuf//:empty_proto",
        "@com_google_protobuf//:field_mask_proto",
        "@com_google_protobuf//:source_context_proto",
        "@com_google_protobuf//:struct_proto",
        "@com_google_protobuf//:timestamp_proto",
        "@com_google_protobuf//:type_proto",
        "@com_google_protobuf//:wrappers_proto",
    ],
    command_line = "--PLUGIN_jupb_out=lite:$(OUT)",
    plugin = "//java/codegen:protoc-gen-jupb",
    plugin_format_flag = "--plugin=protoc-gen-PLUGIN_jupb=%s",
    progress_message = "Generating jupb proto_library %{label}",
    runtime = "//java/runtime/main/jupb:jupb",
    visibility = ["//visibility:public"],
)
