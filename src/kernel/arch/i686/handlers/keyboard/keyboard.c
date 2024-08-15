#include "keyboard.h"
#include "scancodes.h"
#include <libc/stdio.h>
#include <debug.h>
#include <arch/i686/io.h>
#include <arch/i686/hardware/vga_text.h>
#include <libc/string.h>

static const char* module = "Keyboard Handler";

uint8_t errorCount = 0;
uint8_t faultCount = 0;

//Helper functions to allow for future changes in graphics modes
void keyPutc(char in){
    putc(in);
}

void keyBackspace(){
    vga_backspace();
}

void keyHandler(Registers* regs){
    uint16_t recv = i686_inb(0x60);
    uint16_t scancode = recv - 1;
    uint16_t keypress = recv & ~0x80;
    if(recv == 0xE0){
        //Extended byte... continue for now, no need to worry about it.
        log_debug(module, "Extended byte. Ignoring.");
        return;
    }
    if(keypress > (sizeof(qwerty_Scancodes)/sizeof(qwerty_Scancodes[0]))){ //Gets the length of the array instead of size
        log_err(module, "ERROR: input 0x%x exceeds size of scancode table", recv);
        errorCount++;
        if(errorCount > 8){
            log_crit(module, "More than 8 unknown keypresses! What's going on?");
            faultCount++;
            if(faultCount >= 3){
                log_crit(module, "Too many faults!");
                i686_Panic();
            }
        }
        return;
    } else if(recv < 0x80){
        const char* result = qwerty_Scancodes[scancode];
        log_debug(module, "Based on scancode table, you pressed key %s", result);
        if(strcmp(result, "KEY_SPACE")){
            keyPutc(' ');
            return;
        } else if (strcmp(result, "KEY_ENTER")){
            log_debug(module, "Attempting to add new line");
            keyPutc('\n');
            return;
        } else if(strcmp(result, "KEY_BACKSPACE")){
            keyBackspace();
            return;
        }
        //If result is a singular letter, put it on the screen
        for (size_t i = 0; i < sizeof(letters); i++){
            if(letters[i] == result){
                puts(letters[i]);
                return;
            }
        }
        return;
    } else {
        //log_debug("Keyboard handler", "SCANCODE DEBUG: 0x%x", (recv^0x80));
        log_debug(module, "Based on scancode table, you released key %s", qwerty_Scancodes[(recv ^ 0x80)-1]);
        return;
    }
}

void resetKeyboardErrorCount(){
    errorCount = 0;
}
