#include "keyboard.h"
#include "scancodes.h"
#include <stdio.h>
#include <arch/i686/io.h>

void keyHandler(Registers* regs){
    i686_iowait();
    uint16_t recv = i686_inb(0x60);
    uint16_t scancode = recv - 1;
    uint16_t keypress = recv & ~0x80;
    
    if(keypress > sizeof(qwerty_Scancodes)-1){
        printf("ERROR: input %x exceeds size of scancode table\n", recv);
        __asm("int $0x6");
    }
    if(recv < 0x80){
        printf("Based on scancode table, you pressed key %s\n", qwerty_Scancodes[scancode]);
    } else {
        printf("Based on scancode table, you released key ");
        printf(qwerty_Scancodes[(recv ^ 0x80)-1]);
        printf("\n");
    }
}