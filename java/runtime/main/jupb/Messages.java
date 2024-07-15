/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

import java.lang.reflect.Field;
import java.nio.charset.StandardCharsets;
import sun.misc.Unsafe;

public class Messages {
  private static final Unsafe UNSAFE;
  private static final boolean is64bit;

  static void forceLoadClass() {
    // No-op.
  }

  static {
    System.loadLibrary("javaupbruntime");

    try {
      Field f = Unsafe.class.getDeclaredField("theUnsafe");
      f.setAccessible(true);
      UNSAFE = (Unsafe) f.get(null);
    } catch (Exception e) {
      throw new Error(e);
    }

    is64bit = getIs64();
  }

  public static long createMessage(Minitable minitable, Arena arena) {
    return upb_Message_New(arena.pointer, minitable.pointer);
  }

  public static long deepCopy(JupbMessageLite message, Arena arena) {
    return upb_Message_DeepClone(message.pointer, message.minitable.pointer, arena.pointer);
  }

  public static void decodeMessage(byte[] buf, JupbMessageLite message)
      throws ProtocolBufferDecodingException {
    upb_Decode(buf, message.pointer, message.minitable.pointer, message.arena.pointer);
  }

  public static byte[] encodeMessage(JupbMessageLite message) throws ProtocolBufferEncodingException {
    return upb_Encode(message.pointer, message.minitable.pointer, message.arena.pointer);
  }

  private static native long upb_Message_New(long arenaPtr, long minitablePtr);

  private static native long upb_Message_DeepClone(
      long messagePtr, long minitablePtr, long arenaPtr);

  public static boolean UPB_PTR_AT_boolean(long pointer) {
    return UNSAFE.getByte(pointer) != 0;
  }

  public static float UPB_PTR_AT_float(long pointer) {
    return UNSAFE.getFloat(pointer);
  }

  public static int getInt(long msgPointer, long offset32, long offset64) {
    return UNSAFE.getInt(msgPointer + UPB_SIZE(offset32, offset64));
  }

  public static int getInt(
      long msgPointer,
      int offset32,
      int offset64,
      int hasbit_offset32,
      int hasbit_offset64,
      int hasbit_mask32,
      int hasbit_mask64,
      int defaultValue) {
    return getHasBit(msgPointer, hasbit_offset32, hasbit_offset64, hasbit_mask32, hasbit_mask64)
        ? getInt(msgPointer, offset32, offset64)
        : defaultValue;
  }

  public static long getLong(long msgPointer, long offset32, long offset64) {
    return UNSAFE.getLong(msgPointer + UPB_SIZE(offset32, offset64));
  }

  public static long getLong(
      long msgPointer,
      int offset32,
      int offset64,
      int hasbit_offset32,
      int hasbit_offset64,
      int hasbit_mask32,
      int hasbit_mask64,
      long defaultValue) {
    return getHasBit(msgPointer, hasbit_offset32, hasbit_offset64, hasbit_mask32, hasbit_mask64)
        ? getLong(msgPointer, offset32, offset64)
        : defaultValue;
  }

  public static double UPB_PTR_AT_double(long pointer) {
    return UNSAFE.getDouble(pointer);
  }

  public static long UPB_PTR_AT_long(long pointer) {
    return UNSAFE.getLong(pointer);
  }

  public static void UPB_PTR_AT_boolean(long pointer, boolean value) {
    UNSAFE.putByte(pointer, value ? (byte) 1 : (byte) 0);
  }

  public static void UPB_PTR_AT_float(long pointer, float value) {
    UNSAFE.putFloat(pointer, value);
  }

  public static void setInt(long msgPointer, long offset, int value) {
    UNSAFE.putInt(msgPointer + offset, value);
  }

  public static void setLong(long msgPointer, long offset, long value) {
    UNSAFE.putLong(msgPointer + offset, value);
  }

  public static void setMessage(long msgPointer, int offset32, int offset64, long value) {
    if (is64bit) {
      UNSAFE.putLong(msgPointer + offset64, value);
    } else {
      UNSAFE.putInt(msgPointer + offset32, (int) value);
    }
  }

  public static long getMessagePointer(long msgPointer, int offset32, int offset64) {
    return is64bit ? UNSAFE.getLong(msgPointer + offset64) : UNSAFE.getInt(msgPointer + offset32);
  }

  public static void UPB_PTR_AT_double(long pointer, double value) {
    UNSAFE.putDouble(pointer, value);
  }

  public static void UPB_PTR_AT_long(long pointer, long value) {
    UNSAFE.putLong(pointer, value);
  }

  public static String UPB_PTR_AT_String(long pointer) {
    String result = UPB_PTR_AT_String_internal(pointer);
    return result == null ? "" : result;
  }

  // @Nullable
  public static native String UPB_PTR_AT_String_internal(long pointer);

  public static void UPB_PTR_AT_String(long arenaPointer, long pointer, String value) {
    UPB_PTR_AT_String_internal(arenaPointer, pointer, value.getBytes(StandardCharsets.UTF_8));
  }

  public static native void UPB_PTR_AT_String_internal(
      long arenaPointer, long pointer, byte[] value);

  public static long UPB_SIZE(long size32, long size64) {
    return is64bit ? size64 : size32;
  }

  public static String UPB_STR(String str32, String str64) {
    return is64bit ? str64 : str32;
  }

  /* package-private */ static void setHasBit(long messagePointer, long offset, byte mask) {
    byte val = (byte) (UNSAFE.getByte(messagePointer + offset) | mask);
    UNSAFE.putByte(messagePointer + offset, val);
  }

  public static boolean getHasBit(
      long messagePointer, int offset32, int offset64, int mask32, int mask64) {
    return (UNSAFE.getByte(messagePointer + UPB_SIZE(offset32, offset64))
            & UPB_SIZE(mask32, mask64))
        != 0;
  }

  public static native void _upb_clearhas(long messagePointer, int index);

  /** Don't call directly - use is64bit directly. */
  private static native boolean getIs64();

  private static native void upb_Decode(
      byte[] buf, long msgPointer, long minitablePointer, long arenaPointer)
      throws ProtocolBufferDecodingException;

  private static native byte[] upb_Encode(long msgPointer, long minitablePointer, long arenaPointer)
      throws ProtocolBufferEncodingException;
}
