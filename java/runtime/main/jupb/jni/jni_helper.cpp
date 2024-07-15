// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include "jni_helper.h"

void assertNoPendingJniException(JNIEnv *env) {
    if (env->ExceptionCheck() == JNI_TRUE) {
        env->ExceptionDescribe();
        abort();
    }
}

void logErrorMessageAndDie(const char* message) {
    fputs(message, stderr);
    abort();
}