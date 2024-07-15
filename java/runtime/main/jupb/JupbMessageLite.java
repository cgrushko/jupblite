/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

public abstract class JupbMessageLite implements Cloneable {
  protected Minitable minitable;
  protected long pointer;
  protected Arena arena;

  /**
   * Returns a new JupbMessageLite, of the same concrete type of 'this', in the specified arena, wrapping
   * the specified pointer.
   *
   * <p>This is similar to a regular constructor, but the runtime type of the result is a strict
   * subclass of JupbMessageLite, not JupbMessageLite.
   */
  protected JupbMessageLite instantiate(Arena arena, long pointer) {
    JupbMessageLite cloned;
    try {
      // Creates an object whose runtime type is a _subclass_ of JupbMessageLite.
      cloned = (JupbMessageLite) clone();
    } catch (CloneNotSupportedException e) {
      throw new RuntimeException(e);
    }
    cloned.pointer = pointer;
    cloned.arena = arena;
    return cloned;
  }

  protected JupbMessageLite(Minitable minitable, long pointer, Arena arena) {
    this.minitable = minitable;
    this.pointer = pointer;
    this.arena = arena;
  }

  protected JupbMessageLite(Arena arena, Minitable minitable) {
    this(minitable, Messages.createMessage(minitable, arena), arena);
  }

  protected JupbMessageLite(Arena arena, String minidescriptor) {
    this(arena, new jupb.Minitable(minidescriptor));
  }

  protected JupbMessageLite(
      Arena arena, String minidescriptor, int fieldIndex1, JupbMessageLite submessage1) {
    this(
        arena,
        new jupb.Minitable(minidescriptor, fieldIndex1, submessage1.minitable));
  }

  protected JupbMessageLite(
      Arena arena,
      String minidescriptor,
      int fieldIndex1,
      JupbMessageLite submessage1,
      int fieldIndex2,
      JupbMessageLite submessage2) {
    this(
        arena,
        new jupb.Minitable(
            minidescriptor,
            fieldIndex1,
            submessage1.minitable,
            fieldIndex2,
            submessage2.minitable));
  }

  protected JupbMessageLite(
      Arena arena,
      String minidescriptor,
      int fieldIndex1,
      JupbMessageLite submessage1,
      int fieldIndex2,
      JupbMessageLite submessage2,
      int fieldIndex3,
      JupbMessageLite submessage3) {
    this(
        arena,
        new jupb.Minitable(
            minidescriptor,
            fieldIndex1,
            submessage1.minitable,
            fieldIndex2,
            submessage2.minitable,
            fieldIndex3,
            submessage3.minitable));
  }

  protected JupbMessageLite(
      Arena arena,
      String minidescriptor,
      int fieldIndex1,
      JupbMessageLite submessage1,
      int fieldIndex2,
      JupbMessageLite submessage2,
      int fieldIndex3,
      JupbMessageLite submessage3,
      int fieldIndex4,
      JupbMessageLite submessage4) {
    this(
        arena,
        new jupb.Minitable(
            minidescriptor,
            fieldIndex1,
            submessage1.minitable,
            fieldIndex2,
            submessage2.minitable,
            fieldIndex3,
            submessage3.minitable,
            fieldIndex4,
            submessage4.minitable));
  }

  protected JupbMessageLite(
      Arena arena,
      String minidescriptor,
      int fieldIndex1,
      JupbMessageLite submessage1,
      int fieldIndex2,
      JupbMessageLite submessage2,
      int fieldIndex3,
      JupbMessageLite submessage3,
      int fieldIndex4,
      JupbMessageLite submessage4,
      int fieldIndex5,
      JupbMessageLite submessage5) {
    this(
        arena,
        new jupb.Minitable(
            minidescriptor,
            fieldIndex1,
            submessage1.minitable,
            fieldIndex2,
            submessage2.minitable,
            fieldIndex3,
            submessage3.minitable,
            fieldIndex4,
            submessage4.minitable,
            fieldIndex5,
            submessage5.minitable));
  }

  protected JupbMessageLite(
      Arena arena,
      String minidescriptor,
      int fieldIndex1,
      JupbMessageLite submessage1,
      int fieldIndex2,
      JupbMessageLite submessage2,
      int fieldIndex3,
      JupbMessageLite submessage3,
      int fieldIndex4,
      JupbMessageLite submessage4,
      int fieldIndex5,
      JupbMessageLite submessage5,
      int fieldIndex6,
      JupbMessageLite submessage6) {
    this(
        arena,
        new jupb.Minitable(
            minidescriptor,
            fieldIndex1,
            submessage1.minitable,
            fieldIndex2,
            submessage2.minitable,
            fieldIndex3,
            submessage3.minitable,
            fieldIndex4,
            submessage4.minitable,
            fieldIndex5,
            submessage5.minitable,
            fieldIndex6,
            submessage6.minitable));
  }

  public Arena getArena() {
    return arena;
  }

  protected <T extends JupbMessageLite> T getMessage(int offset32, int offset64, T defaultValue) {
    long submsgPointer = Messages.getMessagePointer(pointer, offset32, offset64);
    if (submsgPointer == 0) {
      return defaultValue;
    }
    // We're using |this|'s arena here, even though submsgPointer might be owned by another arena.
    // This is ok, because if submsgPointer belongs to another arena, it was set into this message
    // through a setter, which always fuzes arenas together - see setMessage below.
    return (T) defaultValue.instantiate(arena, submsgPointer);
  }

  public byte[] toByteArray() {
    try {
      return Messages.encodeMessage(this);
    } catch (ProtocolBufferEncodingException e) {
      throw new RuntimeException(e);
    }
  }

  public static class Builder<MessageType extends JupbMessageLite> {
    private final Arena arena;
    protected MessageType instance;
    private boolean isDirty;

    protected Builder(Arena arena, MessageType instance) {
      this.arena = arena;
      this.instance = instance;
    }

    protected void copyOnWrite() {
      if (isDirty) {
        return;
      }
      isDirty = true;
      instance = (MessageType) instance.instantiate(arena, Messages.deepCopy(instance, arena));
    }

    public void setInt(int offset32, int offset64, int value) {
      copyOnWrite();
      Messages.setInt(instance.pointer, Messages.UPB_SIZE(offset32, offset64), value);
    }

    public void setInt(
        int offset32,
        int offset64,
        int hasbit_offset32,
        int hasbit_offset64,
        int hasbit_mask32,
        int hasbit_mask64,
        int value) {
      this.setInt(offset32, offset64, value);
      Messages.setHasBit(
          instance.pointer,
          Messages.UPB_SIZE(hasbit_offset32, hasbit_offset64),
          (byte) Messages.UPB_SIZE(hasbit_mask32, hasbit_mask64));
    }

    public void setLong(int offset32, int offset64, long value) {
      copyOnWrite();
      Messages.setLong(instance.pointer, Messages.UPB_SIZE(offset32, offset64), value);
    }

    public void setLong(
        int offset32,
        int offset64,
        int hasbit_offset32,
        int hasbit_offset64,
        int hasbit_mask32,
        int hasbit_mask64,
        long value) {
      this.setLong(offset32, offset64, value);
      Messages.setHasBit(
          instance.pointer,
          Messages.UPB_SIZE(hasbit_offset32, hasbit_offset64),
          (byte) Messages.UPB_SIZE(hasbit_mask32, hasbit_mask64));
    }

    public void setMessage(int offset32, int offset64, JupbMessageLite value) {
      copyOnWrite();
      Messages.setMessage(instance.pointer, offset32, offset64, value.pointer);
      Arena.fuse(arena, value.arena);
    }

    public void setMessage(
        int offset32,
        int offset64,
        int hasbit_offset32,
        int hasbit_offset64,
        int hasbit_mask32,
        int hasbit_mask64,
        JupbMessageLite value) {
      setMessage(offset32, offset64, value);
      Messages.setHasBit(
          instance.pointer,
          Messages.UPB_SIZE(hasbit_offset32, hasbit_offset64),
          (byte) Messages.UPB_SIZE(hasbit_mask32, hasbit_mask64));
    }

    public void setMessage(int offset32, int offset64, Builder<MessageType> value) {
      setMessage(offset32, offset64, value.build());
    }

    public void setMessage(
        int offset32,
        int offset64,
        int hasbit_offset32,
        int hasbit_offset64,
        int hasbit_mask32,
        int hasbit_mask64,
        Builder<MessageType> value) {
      setMessage(
          offset32,
          offset64,
          hasbit_mask32,
          hasbit_mask64,
          hasbit_mask32,
          hasbit_mask64,
          value.build());
    }

    public MessageType build() {
      isDirty = false;
      return instance;
    }
  }
}
