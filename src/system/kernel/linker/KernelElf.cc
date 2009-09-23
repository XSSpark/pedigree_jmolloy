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

#include <linker/KernelElf.h>
#include <utilities/utility.h>
#include <processor/Processor.h>
#include <processor/VirtualAddressSpace.h>
#include <processor/PhysicalMemoryManager.h>
#include <Log.h>

KernelElf KernelElf::m_Instance;

bool KernelElf::initialise(const BootstrapStruct_t &pBootstrap)
{
  #if defined(X86_COMMON)

    // Calculate the range
    physical_uintptr_t start = pBootstrap.addr;
    physical_uintptr_t end   = pBootstrap.addr + pBootstrap.num * pBootstrap.size;
    for (size_t i = 1; i < pBootstrap.num; i++)
    {
      ElfSectionHeader_t *pSh = reinterpret_cast<ElfSectionHeader_t*>(pBootstrap.addr + i * pBootstrap.size);

      if ((pSh->flags & SHF_ALLOC) != SHF_ALLOC)
        if (pSh->addr >= end)
          end = pSh->addr + pSh->size;
    }

    // Allocate the range
    // TODO: PhysicalMemoryManager::nonRamMemory?
    PhysicalMemoryManager &physicalMemoryManager = PhysicalMemoryManager::instance();
    if (physicalMemoryManager.allocateRegion(m_AdditionalSections,
                                            (end - start + PhysicalMemoryManager::getPageSize() - 1) / PhysicalMemoryManager::getPageSize(),
                                            PhysicalMemoryManager::continuous,
                                            VirtualAddressSpace::KernelMode,
                                            start)
        == false)
    {
      return false;
    }

  #endif

  // Get the string table
  const char *tmpStringTable = reinterpret_cast<const char*>(reinterpret_cast<ElfSectionHeader_t*>(pBootstrap.addr + pBootstrap.shndx * pBootstrap.size)->addr);

  // Search for the symbol/string table and adjust sections
  for (size_t i = 1; i < pBootstrap.num; i++)
  {
    ElfSectionHeader_t *pSh = reinterpret_cast<ElfSectionHeader_t*>(pBootstrap.addr + i * pBootstrap.size);

    #if defined(X86_COMMON)

      // Adjust the section
      if ((pSh->flags & SHF_ALLOC) != SHF_ALLOC)
      {
        pSh->addr = reinterpret_cast<uintptr_t>(m_AdditionalSections.convertPhysicalPointer<void>(pSh->addr));
        pSh->offset = pSh->addr;
      }

    #endif

    // Save the symbol/string table
    const char *pStr = tmpStringTable + pSh->name;

    if (pSh->type == SHT_SYMTAB)
    {
      m_pSymbolTable = reinterpret_cast<ElfSymbol_t*> (pSh->addr);
      m_nSymbolTableSize = pSh->size;
    }
    else if (!strcmp(pStr, ".strtab"))
    {
      m_pStringTable = reinterpret_cast<char*> (pSh->addr);
    }
    else if (!strcmp(pStr, ".shstrtab"))
    {
      m_pShstrtab = reinterpret_cast<char*> (pSh->addr);
    }
    else if (!strcmp(pStr, ".debug_frame"))
    {
      m_pDebugTable = reinterpret_cast<uint8_t*> (pSh->addr);
      m_nDebugTableSize = pSh->size;
    }
  }

  // Initialise remaining member variables
  #if defined(X86_COMMON)
    m_pSectionHeaders = m_AdditionalSections.convertPhysicalPointer<ElfSectionHeader_t>(pBootstrap.addr);
  #else
    m_pSectionHeaders = reinterpret_cast<ElfSectionHeader_t*>(pBootstrap.addr);
  #endif
  m_nSectionHeaders = pBootstrap.num;

  if (m_pSymbolTable && m_pStringTable)
  {
    ElfSymbol_t *pSymbol = reinterpret_cast<ElfSymbol_t *>(m_pSymbolTable);

    const char *pStrtab = reinterpret_cast<const char *>(m_pStringTable);

    for (size_t i = 1; i < m_nSymbolTableSize / sizeof(ElfSymbol_t); i++)
    {
      const char *pStr;

      if (ST_TYPE(pSymbol->info) == 3)
      {
        // Section type - the name will be the name of the section header it refers to.
        ElfSectionHeader_t *pSh = &m_pSectionHeaders[pSymbol->shndx];
        // If it's not allocated, it's a link-once-only section that we can ignore.
        if (!(pSh->flags & SHF_ALLOC))
        {
          pSymbol++;
          continue;
        }
        // Grab the shstrtab
        pStr = reinterpret_cast<const char*> (m_pShstrtab) + pSh->name;
      }
      else
        pStr = pStrtab + pSymbol->name;

      // Insert the symbol into the symbol table.
      SymbolTable::Binding binding;
      switch (ST_BIND(pSymbol->info))
      {
        case 0: // STB_LOCAL
          binding = SymbolTable::Local;
          break;
        case 1: // STB_GLOBAL
          binding = SymbolTable::Global;
          break;
        case 2: // STB_WEAK
          binding = SymbolTable::Weak;
          break;
        default:
          binding = SymbolTable::Global;
      }

      if (*pStr != '\0')
      {
        m_SymbolTable.insert(String(pStr), binding, this, pSymbol->value);
      }
      pSymbol++;
    }
  }

  return true;
}

KernelElf::KernelElf() :
  #if defined(X86_COMMON)
  m_AdditionalSections("Kernel ELF Sections"),
  #endif
  m_Modules(), m_LoadedModules(), m_PendingModules(), m_ModuleAllocator()
{
}

KernelElf::~KernelElf()
{
}

#ifdef PPC_COMMON
#define MOD_START 0xe1000000
#else
#ifdef X86
#define MOD_START 0xfa000000
#else
#define MOD_START 0xFFFFFFFFF0000000
#endif
#endif
#define MOD_LEN 0x400000

static size_t g_Progress = 0;

Module *KernelElf::loadModule(uint8_t *pModule, size_t len)
{
  // The module memory allocator requires dynamic memory - this isn't initialised until after our constructor
  // is called, so check here if we've loaded any modules yet. If not, we can initialise our memory allocator.
  if (m_Modules.count() == 0)
  {
    m_ModuleAllocator.free(MOD_START, MOD_LEN);
  }

  Module *module = new Module;

  module->buffer = pModule;
  module->buflen = len;

  if (!module->elf.create(pModule, len))
  {
    FATAL ("Module load failed (1)");
    delete module;
    return 0;
  }

  uintptr_t loadBase = 0;
  if (!module->elf.loadModule(pModule, len, loadBase, &m_SymbolTable))
  {
    FATAL ("Module load failed (2)");
    delete module;
    return 0;
  }

  //  Load the module debug table (if any)
  if(module->elf.debugFrameTableLength())
  {
    uint8_t*  pDebug = new uint8_t[m_nDebugTableSize + module->elf.debugFrameTableLength()];
    if(UNLIKELY(!pDebug))
    {
      ERROR ("Could not load module debug frame information.");
    }
    else
    {
      memcpy(pDebug, m_pDebugTable, m_nDebugTableSize);
      memcpy( pDebug + m_nDebugTableSize,
              reinterpret_cast<const void*>(module->elf.debugFrameTable()),
              module->elf.debugFrameTableLength());
      m_nDebugTableSize+= module->elf.debugFrameTableLength();
      m_pDebugTable = pDebug;
      NOTICE("Added debug module debug frame information.");
    }
  }

  // Look up the module's name and entry/exit functions, and dependency list.
  module->name = *reinterpret_cast<const char**> (module->elf.lookupSymbol("g_pModuleName"));
  module->entry = *reinterpret_cast<void (**)()> (module->elf.lookupSymbol("g_pModuleEntry"));
  module->exit = *reinterpret_cast<void (**)()> (module->elf.lookupSymbol("g_pModuleExit"));
  module->depends = reinterpret_cast<const char **> (module->elf.lookupSymbol("g_pDepends"));
  DEBUG("KERNELELF: Preloaded module " << module->name);

  g_Progress ++;
  if (g_BootProgress)
      g_BootProgress("moduleload", g_Progress);

  m_Modules.pushBack(module);

  // Can we load this module yet?
  if (moduleDependenciesSatisfied(module))
  {
    executeModule(module);

    g_Progress ++;
    if (g_BootProgress)
        g_BootProgress("moduleexec", g_Progress);

    // Now check if we've allowed any currently pending modules to load.
    bool somethingLoaded = true;
    while (somethingLoaded)
    {
      somethingLoaded = false;
      for (Vector<Module*>::Iterator it = m_PendingModules.begin();
           it != m_PendingModules.end();
           it++)
      {
        if (moduleDependenciesSatisfied(*it))
        {
          executeModule(*it);
          g_Progress ++;
          if (g_BootProgress)
              g_BootProgress("moduleexec", g_Progress);

          m_PendingModules.erase(it);
          somethingLoaded = true;
          break;
        }
      }
    }
  }
  else
  {
    m_PendingModules.pushBack(module);
  }

  return module;
}

void KernelElf::unloadModules()
{
    for (Vector<Module*>::Iterator it = m_LoadedModules.end()-1;
        it != m_LoadedModules.begin();
        it--)
    {
        Module *module = *it;
        NOTICE("KERNELELF: Unloading module " << module->name);

        g_Progress --;
        if (g_BootProgress)
            g_BootProgress("moduleunload", g_Progress);

        if(module->exit)
            module->exit();

        // Check for a destructors list and execute.
        uintptr_t startDtors = module->elf.lookupSymbol("start_dtors");
        uintptr_t endDtors = module->elf.lookupSymbol("end_dtors");

        if (startDtors && endDtors)
        {
            uintptr_t *iterator = reinterpret_cast<uintptr_t*>(startDtors);
            while (iterator < reinterpret_cast<uintptr_t*>(endDtors))
            {
                void (*fp)(void) = reinterpret_cast<void (*)(void)>(*iterator);
                fp();
                iterator++;
            }
        }

        m_SymbolTable.eraseByElf(&module->elf);

        g_Progress --;
        if (g_BootProgress)
            g_BootProgress("moduleunloaded", g_Progress);

        m_LoadedModules.erase(it);
        //m_Modules.erase(it);
        NOTICE("KERNELELF: Module " << module->name << " unloaded.");
    }
}

bool KernelElf::moduleDependenciesSatisfied(Module *module)
{
  int i = 0;
  if (module->depends == 0) return true;

  while (module->depends[i] != 0)
  {
    bool found = false;
    for (size_t j = 0; j < m_LoadedModules.count(); j++)
    {
      if (!strcmp(m_LoadedModules[j]->name, module->depends[i]))
      {
        found = true;
        break;
      }
    }
    if (!found){return false;}
    i++;
  }
  return true;
}

void KernelElf::executeModule(Module *module)
{
  m_LoadedModules.pushBack(module);

  if (!module->elf.finaliseModule(module->buffer, module->buflen))
  {
    FATAL ("KERNELELF: Module relocation failed");
    return;
  }

  // Check for a constructors list and execute.
  uintptr_t startCtors = module->elf.lookupSymbol("start_ctors");
  uintptr_t endCtors = module->elf.lookupSymbol("end_ctors");

  if (startCtors && endCtors)
  {
    uintptr_t *iterator = reinterpret_cast<uintptr_t*>(startCtors);
    while (iterator < reinterpret_cast<uintptr_t*>(endCtors))
    {
      void (*fp)(void) = reinterpret_cast<void (*)(void)>(*iterator);
      fp();
      iterator++;
    }
  }

  NOTICE("KERNELELF: Executing module " << module->name);

  if (module->entry)
    module->entry();

  NOTICE("KERNELELF: Module " << module->name << " finished executing");
}

uintptr_t KernelElf::globalLookupSymbol(const char *pName)
{
  return m_SymbolTable.lookup(String(pName), this);
}

const char *KernelElf::globalLookupSymbol(uintptr_t addr, uintptr_t *startAddr)
{
  /// \todo This shouldn't match local or weak symbols.
  // Try a lookup in the kernel.
  const char *ret;

  if ((ret = lookupSymbol(addr, startAddr)))
    return ret;

  // OK, try every module.
  for (Vector<Module*>::Iterator it = m_Modules.begin();
       it != m_Modules.end();
       it++)
  {
    if ((ret = (*it)->elf.lookupSymbol(addr, startAddr)))
      return ret;
  }
  WARNING_NOLOCK("KERNELELF: GlobalLookupSymbol(" << Hex << addr << ") failed.");
  return 0;
}
