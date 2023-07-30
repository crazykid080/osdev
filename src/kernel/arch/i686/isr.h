#pragma once
#include <stdint.h>

typedef struct{

    //Added in the reverse order it was pushed (stack goes down, not up)
    uint32_t ds;
    uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
    uint32_t interrupt, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) Registers;

void i686_ISR_Initialize();

typedef void (*ISRHandler)(Registers* regs);
void i686_ISR_RegisterHandler(int interrupt);