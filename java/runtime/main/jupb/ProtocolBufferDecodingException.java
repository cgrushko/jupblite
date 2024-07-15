/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

import java.io.IOException;

public class ProtocolBufferDecodingException extends IOException {
  public ProtocolBufferDecodingException(String description) {
    super(description);
  }
}
