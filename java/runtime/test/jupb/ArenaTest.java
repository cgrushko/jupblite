/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
package jupb;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public final class ArenaTest {
  /**
   * Create an Arena object, null it and hint the VM it should garbage-collect. Locally, placing an
   * fputs in arena.cpp's freeUpbArena lets one assert that the native memory has been freed.
   */
  @Test
  public void nativeMemoryIsFreed() throws Exception {
    Arena arena = new Arena();
    arena = null;
    System.gc();
    Thread.sleep(1_000);
  }
}
