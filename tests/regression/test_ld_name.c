/*  Copyright © 2019 Software Reliability Group, Imperial College London
 *
 *  This file is part of SaBRe.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

/*
 * RUN: rm -rf %t1.dir %t1.actual
 * RUN: mkdir -p %t1.dir
 * RUN: %{cc} %s -o %t1.dir/client
 * RUN: for name in ld ld.bfd ld.gold ld.lld ld-new ld-linux-tool ld-new.so-helper ld.software; do cp %t1.dir/client %t1.dir/$name && %{sbr} %{sbr-id} -- %t1.dir/$name >> %t1.actual || exit $?; done
 * RUN: printf "ordinary ld client\nordinary ld client\nordinary ld client\nordinary ld client\nordinary ld client\nordinary ld client\nordinary ld client\nordinary ld client\n" > %t1.expected
 * RUN: diff %t1.actual %t1.expected
 * RUN: %{sbr} %{sbr-trc} -- %t1.dir/ld-new > /dev/null 2> %t1.trace
 * RUN: grep 'write(1' %t1.trace
 */

#include <stddef.h>

static long raw_write(void) {
  static const char msg[] = "ordinary ld client\n";
  long result;

  __asm__ volatile("syscall"
                   : "=a"(result)
                   : "a"(1), "D"(1), "S"(msg), "d"(sizeof(msg) - 1)
                   : "rcx", "r11", "memory");
  return result;
}

int main(void) { return raw_write() < 0; }
