## Where to start

1. Run a test that uses a JUPB proto:

```
$ bazel build --check_visibility=false --linkopt=-Wl,-no_fixup_chains --features=-supports_dynamic_linker --proto_toolchain_for_javalite=//bazel:jupblite_toolchain --sandbox_debug --verbose_failures //java/test:TinyProtoTest && \
    bazel-bin/java/test/TinyProtoTest
```

2. Print a JUPB to stdout and format using google-java-format:

```
$ bazel build @com_google_protobuf//:protoc

$ reset ; \
 bazel build --check_visibility=false //java/codegen:protoc-gen-jupb && bazel-bin/external/protobuf~/protoc '--PLUGIN_jupb_out=lite:output.jar' '--plugin=protoc-gen-PLUGIN_jupb=bazel-bin/java/codegen/protoc-gen-jupb' -I. tiny.proto && \
 unzip -p output.jar upb_test/Tiny.java | google-java-format -
```

## License

Copyright 2024 Carmi Grushko.  All rights reserved.

Use of this source code is governed by a BSD-style
license that can be found in the LICENSE file.
