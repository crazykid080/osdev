#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include "hal/hal.h"
#include <arch/i686/handlers/g_handler.h>
#include <boot/bootparams.h>
#include <arch/i686/hardware/vga_text.h>
#include "debug.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();

void __attribute__((section(".entry"))) start(BootParams* bootParams){
    memset(&__bss_start, 0, (&__end) - (&__bss_start));
    vga_clrscr();
    HAL_initialize();
    registerHandlers();
    printf("Boot Device: %x\n", bootParams->BootDevice);
    printf("Memory region count: %d\n", bootParams->Memory.RegionCount);
    for (int i = 0; i < bootParams->Memory.RegionCount; i++){
        printf("MEM: start=0x%llx length=0x%llx: %d\n",
                                                    bootParams->Memory.Regions[i].Begin,
                                                    bootParams->Memory.Regions[i].Length,
                                                    bootParams->Memory.Regions[i].Type);
    }
    
    //crash_me();
    //__asm("int $0x7");
    printf("Hello world from kernel!!!\n");
end:
    for (;;);
}
