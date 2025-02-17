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

#include "PhysicalMemoryManager.h"
#include <processor/VirtualAddressSpace.h>
#include "../ppc32/VirtualAddressSpace.h"
#include <Log.h>
#include <panic.h>
#include <processor/MemoryRegion.h>

PpcCommonPhysicalMemoryManager PpcCommonPhysicalMemoryManager::m_Instance;

PhysicalMemoryManager &PhysicalMemoryManager::instance()
{
  return PpcCommonPhysicalMemoryManager::instance();
}

physical_uintptr_t PpcCommonPhysicalMemoryManager::allocatePage()
{
  // If we're in initial mode we just return the next available page.
  // If we're in normal mode we ask the pagestack for another page.
  if (m_InitialMode)
    return (m_NextPage += 0x1000);
  else
    return m_PageStack.allocate();
}
void PpcCommonPhysicalMemoryManager::freePage(physical_uintptr_t page)
{
  // If we're in initial mode it's time to panic.
  if (m_InitialMode)
    panic("freePage called in initial mode!");
  m_PageStack.free(page);
}
bool PpcCommonPhysicalMemoryManager::allocateRegion(MemoryRegion &Region,
                                                     size_t cPages,
                                                     size_t pageConstraints,
                                                     size_t Flags,
                                                     physical_uintptr_t start)
{
  // Allocate a specific physical memory region (always physically continuous)
  if (start != static_cast<physical_uintptr_t>(-1))
  {
    if ((pageConstraints & continuous) != continuous)
      panic("PhysicalMemoryManager::allocateRegion(): function misused");

    // Remove the memory from the range-lists (if desired/possible)
    if ((pageConstraints & nonRamMemory) == nonRamMemory)
    {
      if (m_PhysicalRanges.allocateSpecific(start, cPages * getPageSize()) == false)
        if ((pageConstraints & force) != force)
          return false;
    }

    // Allocate the virtual address space
    uintptr_t vAddress;
    if (m_MemoryRegions.allocate(cPages * PhysicalMemoryManager::getPageSize(),
                                 vAddress)
         == false)
    {
      WARNING("AllocateRegion: MemoryRegion allocation failed.");
      return false;
    }

    // Map the physical memory into the allocated space
    VirtualAddressSpace &virtualAddressSpace = VirtualAddressSpace::getKernelAddressSpace();
    for (size_t i = 0;i < cPages;i++)
      if (virtualAddressSpace.map(start + i * PhysicalMemoryManager::getPageSize(),
                                  reinterpret_cast<void*>(vAddress + i * PhysicalMemoryManager::getPageSize()),
                                  Flags)
          == false)
      {
        m_MemoryRegions.free(vAddress, cPages * PhysicalMemoryManager::getPageSize());
        WARNING("AllocateRegion: VirtualAddressSpace::map failed.");
        return false;
      }

    // Set the memory-region's members
    Region.m_VirtualAddress = reinterpret_cast<void*>(vAddress);
    Region.m_PhysicalAddress = start;
    Region.m_Size = cPages * PhysicalMemoryManager::getPageSize();

    // Add to the list of memory-regions
    PhysicalMemoryManager::m_MemoryRegions.pushBack(&Region);
    return true;
  }
  else
  {
    // Allocate the virtual address space
    uintptr_t vAddress;
    if (m_MemoryRegions.allocate(cPages * PhysicalMemoryManager::getPageSize(),
                                 vAddress)
         == false)
    {
      WARNING("AllocateRegion: MemoryRegion allocation failed.");
      return false;
    }

    uint32_t start = 0;
    VirtualAddressSpace &virtualAddressSpace = VirtualAddressSpace::getKernelAddressSpace();
    // Map the physical memory into the allocated space
    for (size_t i = 0;i < cPages;i++)
    {
      physical_uintptr_t page = m_PageStack.allocate();

      if (virtualAddressSpace.map(page,
                                  reinterpret_cast<void*>(vAddress + i * PhysicalMemoryManager::getPageSize()),
                                  Flags)
          == false)
      {
        WARNING("AllocateRegion: VirtualAddressSpace::map failed.");
        return false;
      }
    }

    // Set the memory-region's members
    Region.m_VirtualAddress = reinterpret_cast<void*>(vAddress);
    Region.m_PhysicalAddress = start;
    Region.m_Size = cPages * PhysicalMemoryManager::getPageSize();

    // Add to the list of memory-regions
    PhysicalMemoryManager::m_MemoryRegions.pushBack(&Region);
    return true;
  }
  return false;
}

PpcCommonPhysicalMemoryManager::PpcCommonPhysicalMemoryManager() :
  m_PageStack(), m_InitialMode(true), m_NextPage(PMM_INITIAL_START),
  m_PhysicalRanges(), m_MemoryRegions()
{
}
PpcCommonPhysicalMemoryManager::~PpcCommonPhysicalMemoryManager()
{
}

void PpcCommonPhysicalMemoryManager::initialise(Translations &translations, uintptr_t ramMax)
{
  // Allocate every page that is currently in Translations.
  for (uintptr_t i = 0; i < ramMax; i += getPageSize())
  {
    bool claimed = false;
    for (unsigned int j = 0; j < translations.getNumTranslations(); j++)
    {
      Translations::Translation t = translations.getTranslation(j);
      if ((i >= t.phys) && (i < t.phys+t.size))
      {
        claimed = true;
        break;
      }
    }
    if ( (i >= PMM_INITIAL_START) && (i < m_NextPage) ) claimed = true;
    if (!claimed) m_PageStack.free(i);
  }

  // Initialise the free physical ranges. The physical ranges
  // are designed to contain areas of physical address space not covered by other
  // allocators (e.g. for PCI mapping), so we don't add anything below ramMax.
  m_PhysicalRanges.free(ramMax, 0x100000000ULL-ramMax);
  for (unsigned int i = 0; i < translations.getNumTranslations(); i++)
  {
    Translations::Translation t = translations.getTranslation(i);
    // Normally we would check allocateSpecific for success, but it may fail as things like PCI address spaces
    // can overlap (and RangeList can't deal with overlapping sections)
    if (t.phys >= ramMax)
      m_PhysicalRanges.allocateSpecific(static_cast<uint64_t>(t.phys), t.size);
  }

  // Initialise the range of virtual space for MemoryRegions
  m_MemoryRegions.free(KERNEL_VIRTUAL_MEMORYREGION_ADDRESS,
                       KERNEL_VIRTUAL_MEMORYREGION_SIZE);

  // ...And we're now in normal mode.
  m_InitialMode = false;
}

void PpcCommonPhysicalMemoryManager::unmapRegion(MemoryRegion *pRegion)
{
  
  for (Vector<MemoryRegion*>::Iterator it = PhysicalMemoryManager::m_MemoryRegions.begin();
       it != PhysicalMemoryManager::m_MemoryRegions.end();
       it++)
  {
    if (*it == pRegion)
    {
      size_t cPages = pRegion->size() / PhysicalMemoryManager::getPageSize();
      uintptr_t start = reinterpret_cast<uintptr_t> (pRegion->virtualAddress());
      VirtualAddressSpace &virtualAddressSpace = VirtualAddressSpace::getKernelAddressSpace();
      for (size_t i = 0;i < cPages;i++)
        virtualAddressSpace.unmap(reinterpret_cast<void*> (start + i * PhysicalMemoryManager::getPageSize()));
      m_MemoryRegions.free(start, pRegion->size());
      PhysicalMemoryManager::m_MemoryRegions.erase(it);
      break;
    }
  }
}

PpcCommonPhysicalMemoryManager::PageStack::PageStack() :
  m_Stack(0), m_StackMax(0), m_StackSize(0)
{
}

physical_uintptr_t PpcCommonPhysicalMemoryManager::PageStack::allocate()
{
  if (m_StackSize == 0) /// \todo Swapspace here.
    panic("No more physical pages left to allocate!");
  return m_Stack[--m_StackSize];
}

void PpcCommonPhysicalMemoryManager::PageStack::free(physical_uintptr_t physicalAddress)
{
  if (m_StackMax <= m_StackSize)
  {
    m_StackMax += 0x1000;
    physical_uintptr_t *oldStack = m_Stack;
    m_Stack = new physical_uintptr_t[m_StackMax];
    // Copy over the old stack
    memcpy(reinterpret_cast<uint8_t*> (m_Stack),
           reinterpret_cast<uint8_t*> (oldStack), m_StackSize);
    // Free the old stack ... note that this will probably recurse into a call
    // to 'free'!
    delete [] oldStack;
  }
  m_Stack[m_StackSize++] = physicalAddress;
}
