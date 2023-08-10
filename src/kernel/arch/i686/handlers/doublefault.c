#include "doublefault.h"
#include <debug.h>
#include <arch/i686/io.h>

void doubleFault(Registers* regs){
    log_crit("Double fault", "Recieved a double fault interrupt!");
    log_crit("Double fault","eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

    log_crit("Double fault","esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x",
            regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds, regs->ss);
    i686_Panic();
}