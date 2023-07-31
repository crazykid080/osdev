#include "g_handler.h"
#include "timer.h"
#include "keyboard.h"


void registerHandlers(){
    i686_IRQ_RegisterHandler(0, timer);
    i686_IRQ_RegisterHandler(0x1, keyHandler);
}