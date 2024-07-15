/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

package jupb;

import java.nio.charset.StandardCharsets;

public class Minitable {
  static {
    System.loadLibrary("javaupbruntime");
  }

  final long pointer;

  public Minitable(String serializedMinitable) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
  }

  public Minitable(String serializedMinitable, int fieldIndex1, Minitable subminitable1) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
  }

  public Minitable(
      String serializedMinitable,
      int fieldIndex1,
      Minitable subminitable1,
      int fieldIndex2,
      Minitable subminitable2) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex2, subminitable2.pointer);
  }

  public Minitable(
      String serializedMinitable,
      int fieldIndex1,
      Minitable subminitable1,
      int fieldIndex2,
      Minitable subminitable2,
      int fieldIndex3,
      Minitable subminitable3) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex2, subminitable2.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex3, subminitable3.pointer);
  }

  public Minitable(
      String serializedMinitable,
      int fieldIndex1,
      Minitable subminitable1,
      int fieldIndex2,
      Minitable subminitable2,
      int fieldIndex3,
      Minitable subminitable3,
      int fieldIndex4,
      Minitable subminitable4) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex2, subminitable2.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex3, subminitable3.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex4, subminitable4.pointer);
  }

  public Minitable(
      String serializedMinitable,
      int fieldIndex1,
      Minitable subminitable1,
      int fieldIndex2,
      Minitable subminitable2,
      int fieldIndex3,
      Minitable subminitable3,
      int fieldIndex4,
      Minitable subminitable4,
      int fieldIndex5,
      Minitable subminitable5) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex2, subminitable2.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex3, subminitable3.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex4, subminitable4.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex5, subminitable5.pointer);
  }

  public Minitable(
      String serializedMinitable,
      int fieldIndex1,
      Minitable subminitable1,
      int fieldIndex2,
      Minitable subminitable2,
      int fieldIndex3,
      Minitable subminitable3,
      int fieldIndex4,
      Minitable subminitable4,
      int fieldIndex5,
      Minitable subminitable5,
      int fieldIndex6,
      Minitable subminitable6) {
    this.pointer =
        upb_MiniTable_Build(
            serializedMinitable.getBytes(StandardCharsets.UTF_8),
            Arena.getDefaultInstanceArena().pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex1, subminitable1.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex2, subminitable2.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex3, subminitable3.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex4, subminitable4.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex5, subminitable5.pointer);
    upb_MiniTable_SetSubMessage(this.pointer, fieldIndex6, subminitable6.pointer);
  }

  private static final native long upb_MiniTable_Build(byte[] serializedMinitable, long arenaPtr);

  private static final native void upb_MiniTable_SetSubMessage(
      long minitable, int fieldIndex, long submessageMinitable);
}
