#include "prom.h"

prom_entry prom;
void *prom_chosen;
void *prom_stdout;
void *prom_screen;
void *prom_mmu;

void *call_prom(const char *service, int nargs, int nret, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6)
{
  prom_args pa;
  pa.service = service;
  
  pa.nargs = nargs;
  pa.nret = nret;
  pa.args[0] = a1;
  pa.args[1] = a2;
  pa.args[2] = a3;
  pa.args[3] = a4;
  pa.args[4] = a5;
  pa.args[5] = a6;
  for (int i = 6; i < 10; i++)
    pa.args[i] = 0;
  
  prom (&pa);
  if (nret > 0)
    return pa.args[nargs];
  else
    return 0;
}

int prom_get_chosen(char *name, void *mem, int len)
{
  return prom_getprop(prom_chosen, name, mem, len);
}

void prom_init(prom_entry pe)
{
  prom = pe;

  prom_chosen = prom_finddevice("/chosen");
  if (prom_chosen == reinterpret_cast<void*>(-1))
    prom_exit();
  if (prom_get_chosen (const_cast<char*>("stdout"), &prom_stdout, sizeof(prom_stdout)) <= 0)
    prom_exit();
  if (prom_get_chosen (const_cast<char*>("mmu"), &prom_mmu, sizeof(prom_mmu)) <= 0)
    prom_exit();
  prom_screen = prom_finddevice("screen");
  if (prom_screen == reinterpret_cast<void*>(-1))
    prom_exit();
}

void *prom_finddevice(const char *dev)
{
  return call_prom("finddevice", 1, 1, reinterpret_cast<void*>(const_cast<char*>(dev)));
}

int prom_exit()
{
  call_prom("exit", 0, 0);
  return 0;
}

int prom_getprop(void *dev, char *name, void *buf, int len)
{
  return reinterpret_cast<int>
    (call_prom ("getprop", 4, 1, dev, reinterpret_cast<void*>(name),
                buf, reinterpret_cast<void*>(len)));
}

void prom_putchar(char c)
{
  if (c == '\n')
    call_prom ("write", 3, 1, prom_stdout, reinterpret_cast<void*>(const_cast<char*>("\r\n")), reinterpret_cast<void*>(2));
  else
    call_prom ("write", 3, 1, prom_stdout, reinterpret_cast<void*>(&c), reinterpret_cast<void*>(1));
}

void prom_map(unsigned int phys, unsigned int virt, unsigned int size)
{
  call_prom("call-method", 6,1,  reinterpret_cast<void*>(const_cast<char*>("map")),
            reinterpret_cast<void*>(prom_mmu), reinterpret_cast<void*>(-1),
            reinterpret_cast<void*>(size), reinterpret_cast<void*>(virt),
            reinterpret_cast<void*>(phys));
}
