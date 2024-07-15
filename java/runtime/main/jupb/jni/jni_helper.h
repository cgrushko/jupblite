// Copyright 2024 Carmi Grushko.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <jni.h>

void assertNoPendingJniException(JNIEnv *env);
void logErrorMessageAndDie(const char* message);