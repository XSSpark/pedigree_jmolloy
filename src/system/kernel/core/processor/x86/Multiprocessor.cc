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
 
#ifdef MULTIPROCESSOR
 
#include <processor/Processor.h>
#include "gdt.h"
#include "InterruptManager.h"
#include "../x86_common/Multiprocessor.h"
#include <process/initialiseMultitasking.h>

#include "../../../machine/x86_common/Pc.h"

void Multiprocessor::applicationProcessorStartup()
{
  // Initialise this processor's interrupt handling
  X86InterruptManager::initialiseProcessor();

  // Signal the Bootstrap processor that this processor is started and the BSP can continue
  // to boot up other processors
  m_ProcessorLock1.release();

  // Wait until the GDT is initialised and the first 4MB identity mapping removed
  m_ProcessorLock2.acquire();
  m_ProcessorLock2.release();

  // Load the GDT
  X86GdtManager::initialiseProcessor();

  // Invalidate the first 4MB identity mapping
  Processor::invalidate(0);

  // Initialise the machine-specific interface
  Pc::instance().initialiseProcessor();

  // We need to synchronize the -init section invalidation
  Processor::invalidate(0);

  // Start multitasking and ensure there is a spare idle thread for this CPU.
  initialiseMultitaskingPerProcessor();

  // Call the per-processor code in main.cc
  extern void apMain();
  apMain();
}

#endif
