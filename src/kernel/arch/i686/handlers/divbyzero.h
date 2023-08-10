#pragma once
#include <arch/i686/isr.h>

void resetDivCounter();
void divZeroHandler(Registers* regs);