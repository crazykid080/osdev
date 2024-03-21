#pragma once
#include <stdint.h>

void vga_putcolor(int x, int y, uint8_t color);
uint8_t vga_getcolor(int x, int y);
void vga_setScreenColor(uint8_t color);
void vga_clrscr();
void vga_putc(char c);
void vga_backspace();