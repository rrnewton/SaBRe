/* Copyright © 2026 Software Reliability Group, Imperial College London
 *
 * This file is part of SaBRe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/*
 * RUN: %{cc} -I%S/../includes -I%S/../arch/x86_64 %S/../arch/x86_64/x86_decoder.c %s -o %t
 * RUN: %t
 */

#if defined(__x86_64__) || defined(__i386__)

#include "x86_decoder.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

static void check_instruction(const unsigned char *bytes, size_t len,
                              unsigned short expected) {
  const char *next = (const char *)bytes;
  unsigned short decoded = next_inst(&next, true, NULL, NULL, NULL, NULL, NULL);

  assert(decoded == expected);
  assert(next == (const char *)bytes + len);
}

int main(void) {
  static const unsigned char movq_to_xmm[] = {0x66, 0x48, 0x0f, 0x6e, 0xc7};
  static const unsigned char unpack_qwords[] = {0x66, 0x0f, 0x6c, 0xc0};
  static const unsigned char store_unaligned[] = {0x0f, 0x11, 0x83, 0xd8,
                                                  0x02, 0x00, 0x00};

  check_instruction(movq_to_xmm, sizeof(movq_to_xmm), 0x0f6e);
  check_instruction(unpack_qwords, sizeof(unpack_qwords), 0x0f6c);
  check_instruction(store_unaligned, sizeof(store_unaligned), 0x0f11);
  return 0;
}

#else

int main(void) { return 0; }

#endif
