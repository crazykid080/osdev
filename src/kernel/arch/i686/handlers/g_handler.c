#include "g_handler.h"
#include "timer.h"
#include "keyboard/keyboard.h"
#include "divbyzero.h"
#include "doublefault.h"


void registerHandlers(){
    i686_IRQ_RegisterHandler(0x0, timer);
    i686_IRQ_RegisterHandler(0x1, keyHandler);
    i686_ISR_RegisterHandler(0x0, divZeroHandler);
    i686_ISR_RegisterHandler(0x8, doubleFault);
}