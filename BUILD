load("@rules_proto//proto:defs.bzl", "proto_lang_toolchain", "proto_library")

proto_library(
    name = "tiny_proto",
    srcs = ["tiny.proto"],
)

java_lite_proto_library(
    name = "tiny_jupb_proto",
    deps = [":tiny_proto"],
)

cc_library(
    name = "jni_headers",
    srcs = [
        "@local_jdk//:include/darwin/jni_md.h",
        "@local_jdk//:include/jni.h",
    ],
    includes = [
        "external/local_jdk/include",
        "external/local_jdk/include/darwin",
    ],
    visibility = [
        "//visibility:public",
    ],
)

alias(
    name = "format",
    actual = "//tools/format",
)
