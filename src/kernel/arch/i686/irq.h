#pragma once

#include "isr.h"

#define PIC_REMAP_OFFSET 0x20

typedef void (*IRQHandler)(Registers* regs);

void i686_IRQ_Handler(Registers* regs);

void i686_IRQ_Initialize();
void i686_IRQ_RegisterHandler(int irq, IRQHandler handler);