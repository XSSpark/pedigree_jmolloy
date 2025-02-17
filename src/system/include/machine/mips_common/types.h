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

#ifndef KERNEL_MACHINE_MIPS_COMMON_TYPES_H
#define KERNEL_MACHINE_MIPS_COMMON_TYPES_H

#include <processor/types.h>

/** @addtogroup kernelmachinemipscommon
 * @{ */

/** Define a type for IRQ identifications */
typedef uint8_t MIPSCommonirq_id_t;

#define KSEG0(addr) (addr|0x80000000)
#define KSEG1(addr) (addr|0xa0000000)
#define IS_KUSEG(addr) (addr<0x80000000)
#define IS_KSEG0(addr) (addr>=0x80000000 && addr<0xa0000000)
#define IS_KSEG1(addr) (addr>=0xa0000000 && addr<0xc0000000)
#define IS_KSEG2(addr) (addr>=0xc0000000)

/** @} */

#endif
