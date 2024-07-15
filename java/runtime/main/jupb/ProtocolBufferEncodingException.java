/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

import java.io.IOException;

public class ProtocolBufferEncodingException extends IOException {
  public ProtocolBufferEncodingException(String description) {
    super(description);
  }
}
