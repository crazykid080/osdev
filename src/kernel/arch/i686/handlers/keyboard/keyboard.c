#include "keyboard.h"
#include "scancodes.h"
#include <stdio.h>
#include <debug.h>
#include <arch/i686/io.h>

void keyHandler(Registers* regs){
    uint16_t recv = i686_inb(0x60);
    uint16_t scancode = recv - 1;
    uint16_t keypress = recv & ~0x80;

    if(keypress > (sizeof(qwerty_Scancodes)/sizeof(qwerty_Scancodes[0]))){ //Gets the length of the array instead of size
        log_err("Keyboard handler", "ERROR: input 0x%x exceeds size of scancode table", recv);
        return;
    }
    if(recv == 0xE0){
        //Extended byte... wait for nexr interrupt... how?
    }
    if(recv < 0x80){
        log_debug("Keyboard handler", "Based on scancode table, you pressed key %s", qwerty_Scancodes[scancode]);
        return;
    } else {
        log_debug("Keyboard handler", "SCANCODE DEBUG: 0x%x", (recv^0x80));
        log_debug("Keyboard handler", "Based on scancode table, you released key %s", qwerty_Scancodes[(recv ^ 0x80)-1]);
        return;
    }
}
