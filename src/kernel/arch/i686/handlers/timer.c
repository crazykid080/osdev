#include "timer.h"
#include <libc/stdio.h>
#include <debug.h>
#include "divbyzero.h"
#include "keyboard/keyboard.h"

void timer(Registers* regs){
    resetDivCounter();
    resetKeyboardErrorCount();
    //log_debug("Timer handler", ".");
    return;
}