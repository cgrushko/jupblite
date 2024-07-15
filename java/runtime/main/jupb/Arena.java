/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.util.HashSet;

public class Arena {
  static {
    // Class-load Messages so JNI can find it.
    Messages.forceLoadClass();

    System.loadLibrary("javaupbruntime");
  }

  static final void forceLoadClass() {
    // no-op
  }

  private static final ReferenceQueue<Arena> referenceQueue = new ReferenceQueue<>();
  private static final HashSet<PhantomReference> refs = new HashSet<>();

  static {
    new Thread() {
      @Override
      public void run() {
        while (true) {
          try {
            Reference<?> ref = referenceQueue.remove();
            freeUpbArena(((ArenaPhantomReference) ref).pointerToFree);
            synchronized (refs) {
              refs.remove(ref);
            }
            ref.clear();
          } catch (InterruptedException e) {
            // Continue running; this thread should never be interrupted.
          }
        }
      }
    }.start();
  }

  /**
   * Holder for Arena object to contain default instances for all messages/extensions in the
   * program.
   */
  private static class DefaultInstanceArenaHolder {
    private static final Arena INSTANCE = new Arena();
  }

  public static Arena getDefaultInstanceArena() {
    return DefaultInstanceArenaHolder.INSTANCE;
  }

  public static void fuse(Arena a, Arena b) {
    if (a == b) {
      return;
    }
    upb_Arena_Fuse(a.pointer, b.pointer);
  }

  public final long pointer;

  public Arena() {
    pointer = initNative();
    synchronized (refs) {
      refs.add(new ArenaPhantomReference(this, referenceQueue, pointer));
    }
  }

  private native long initNative();

  private static native void freeUpbArena(long pointer);

  private static native void upb_Arena_Fuse(long pointer1, long pointer2);

  private static class ArenaPhantomReference extends PhantomReference<Arena> {
    public final long pointerToFree;

    public ArenaPhantomReference(
        Arena referent, ReferenceQueue<? super Arena> q, long pointerToFree) {
      super(referent, q);
      this.pointerToFree = pointerToFree;
    }
  }
}
