#pragma once
#include <arch/i686/irq.h>

void keyHandler(Registers* regs);
void resetKeyboardErrorCount();