/*
 * Copyright (c) 2008 James Molloy, Jörg Pfähler, Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _PEDIGREE_SYSCALL_H
#define _PEDIGREE_SYSCALL_H

// If you change this, ensure you change src/system/include/processor/Syscalls.h !
#define NATIVE_SYSCALL_SERVICE 3

#ifdef X86
#include "native-syscall-i686.h"
#define SYSCALL_TARGET_FOUND
#endif

#ifdef X64
#include "native-syscall-amd64.h"
#define SYSCALL_TARGET_FOUND
#endif

#ifdef PPC_COMMON
#include "native-syscall-ppc.h"
#define SYSCALL_TARGET_FOUND
#endif

#ifndef SYSCALL_TARGET_FOUND
#error Syscall target not found!
#endif


#endif
