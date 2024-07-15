/* Copyright 2024 Carmi Grushko.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

import static com.google.common.truth.Truth.assertThat;

import jupb.Arena;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import upb_test.Tiny.BabySteps;
import upb_test.Tiny.Submessage;

@RunWith(JUnit4.class)
public class TinyProtoTest {
  private final Arena arena = new Arena();

  @Test
  public void defaultInstance() throws Exception {
    BabySteps babySteps = BabySteps.getDefaultInstance();
    assertThat(babySteps.hasFoo()).isFalse();
    assertThat(babySteps.getFoo()).isEqualTo(43);
    assertThat(babySteps.hasBar()).isFalse();
    assertThat(babySteps.getBar()).isEqualTo(0);
  }

  @Test
  public void instantiation() throws Exception {
    BabySteps babySteps = BabySteps.newBuilder(arena).setFoo(123).setBar(17).build();
    assertThat(babySteps.hasFoo()).isTrue();
    assertThat(babySteps.getFoo()).isEqualTo(123);
    assertThat(babySteps.hasBar()).isTrue();
    assertThat(babySteps.getBar()).isEqualTo(17);

    babySteps = BabySteps.newBuilder(arena).build();
    assertThat(babySteps.hasFoo()).isFalse();
    assertThat(babySteps.getFoo()).isEqualTo(43);
    assertThat(babySteps.hasBar()).isFalse();
    assertThat(babySteps.getBar()).isEqualTo(0);
  }

  @Test
  public void reuseBuilder() throws Exception {
    BabySteps.Builder babyStepsBuilder = BabySteps.newBuilder(arena).setFoo(123);

    BabySteps babySteps1 = babyStepsBuilder.build();
    assertThat(babySteps1.hasFoo()).isTrue();
    assertThat(babySteps1.getFoo()).isEqualTo(123);
    assertThat(babySteps1.hasBar()).isFalse();
    assertThat(babySteps1.getBar()).isEqualTo(0);

    babyStepsBuilder.setBar(17);

    BabySteps babySteps2 = babyStepsBuilder.build();
    assertThat(babySteps2.hasFoo()).isTrue();
    assertThat(babySteps2.getFoo()).isEqualTo(123);
    assertThat(babySteps2.hasBar()).isTrue();
    assertThat(babySteps2.getBar()).isEqualTo(17);

    // Test that changes to babyStepsBuilder after babySteps1 was created from it
    // are not reflected in babySteps1.
    assertThat(babySteps1.hasBar()).isFalse();
    assertThat(babySteps1.getBar()).isEqualTo(0);
  }

  @Test
  public void primitiveFields() throws Exception {
    BabySteps.Builder babyStepsBuilder = BabySteps.newBuilder(arena);
    babyStepsBuilder.setFoo(Integer.MAX_VALUE);
    babyStepsBuilder.setBar(Long.MAX_VALUE);
    BabySteps babySteps = babyStepsBuilder.build();

    assertThat(babySteps.getFoo()).isEqualTo(Integer.MAX_VALUE);
    assertThat(babySteps.getBar()).isEqualTo(Long.MAX_VALUE);

    BabySteps encodeDecoder = BabySteps.parseFrom(arena, babySteps.toByteArray());
    assertThat(encodeDecoder.getFoo()).isEqualTo(Integer.MAX_VALUE);
    assertThat(encodeDecoder.getBar()).isEqualTo(Long.MAX_VALUE);
  }

  @Test
  public void messageFields() throws Exception {
    BabySteps.Builder babyStepsBuilder = BabySteps.newBuilder(arena);
    Submessage.Builder submessageBuilder = Submessage.newBuilder(arena);

    submessageBuilder.setBaz(321);
    babyStepsBuilder.setSubmsg(submessageBuilder.build());
    BabySteps babyStep1 = babyStepsBuilder.build();

    submessageBuilder.setBaz(123);
    babyStepsBuilder.setSubmsg(submessageBuilder.build());
    BabySteps babyStep2 = babyStepsBuilder.build();

    // Test that changes to babyStepsBuilder after babySteps1 was created from it
    // are not reflected in babySteps1.
    assertThat(babyStep1.getSubmsg().getBaz()).isEqualTo(321);
    assertThat(babyStep2.getSubmsg().getBaz()).isEqualTo(123);

    BabySteps encodeDecoder = BabySteps.parseFrom(arena, babyStep1.toByteArray());
    assertThat(encodeDecoder.getSubmsg().getBaz()).isEqualTo(321);
  }

  @Test
  public void encodeDecode() throws Exception {
    BabySteps original = BabySteps.newBuilder(arena).setFoo(123).setBar(17).build();

    byte[] data = original.toByteArray();
    assertThat(data).isNotNull();

    BabySteps decoded = BabySteps.parseFrom(arena, data);

    assertThat(decoded.hasFoo()).isTrue();
    assertThat(decoded.getFoo()).isEqualTo(123);
    assertThat(decoded.hasBar()).isTrue();
    assertThat(decoded.getBar()).isEqualTo(17);
  }
}
