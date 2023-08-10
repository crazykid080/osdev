#include "timer.h"
#include <stdio.h>
#include <debug.h>
#include "divbyzero.h"

void timer(Registers* regs){
    resetDivCounter();
    //log_debug("Timer handler", ".");
    return;
}