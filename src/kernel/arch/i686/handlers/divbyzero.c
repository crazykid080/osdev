#include "divbyzero.h"
#include <debug.h>
#include <arch/i686/io.h>

int g_div_counter = 0;

void resetDivCounter(){
    g_div_counter = 0;
}

void divZeroHandler(Registers* regs){
    if(g_div_counter >= 5){
        log_crit("Division by zero handler", "Too many errors occuring quickly!");
        __asm("int $0x8");
    }
    log_crit("Division by zero handler", "Divide by zero");
    log_info("Division by zero handler","eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

    log_info("Division by zero handler","esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x",
            regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds, regs->ss);

    regs->eax = 0xFFFFFFFF;
    regs->ecx = 0xFFFFFFFF;

    g_div_counter++;
}