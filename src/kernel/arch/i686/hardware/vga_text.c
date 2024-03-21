#include "vga_text.h"
#include "../io.h"

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x7;
int g_ScreenX = 0, g_ScreenY = 0;
uint8_t* g_ScreenBuffer = (uint8_t*)0xB8000;

void vga_putchr(int x, int y, char c){
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

void vga_putcolor(int x, int y, uint8_t color){
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

void vga_setScreenColor(uint8_t color){
    for (int y = 0; y < SCREEN_HEIGHT; y++){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            vga_putcolor(x, y, color);
        }
    }
}

char vga_getchr(int x, int y){
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t vga_getcolor(int x, int y){
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void vga_setcursor(int x, int y){
    int pos = y * SCREEN_WIDTH + x;

    i686_outb(0x3D4, 0x0F);
    i686_outb(0x3D5, (uint8_t)(pos & 0xFF));
    i686_outb(0x3D4, 0x0E);
    i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_clrscr(){
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++){
            vga_putchr(x, y, '\0');
            vga_putcolor(x, y, DEFAULT_COLOR);
        }

    g_ScreenX = 0;
    g_ScreenY = 0;
    vga_setcursor(g_ScreenX, g_ScreenY);
}

void vga_backspace(){
    g_ScreenX--;
    g_ScreenBuffer[2 * (g_ScreenY * SCREEN_WIDTH + g_ScreenX)] = ' ';
    vga_setcursor(g_ScreenX, g_ScreenY);
}

void vga_scrollback(int lines){
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++){
            vga_putchr(x, y - lines, vga_getchr(x, y));
            vga_putcolor(x, y - lines, vga_getcolor(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++){
            vga_putchr(x, y, '\0');
            vga_putcolor(x, y, DEFAULT_COLOR);
        }

    g_ScreenY -= lines;
}

void vga_putc(char c){
    switch (c){
        case '\n':
            g_ScreenX = 0;
            g_ScreenY++;
            break;
    
        case '\t':
            for (int i = 0; i < 4 - (g_ScreenX % 4); i++)
                vga_putc(' ');
            break;

        case '\r':
            g_ScreenX = 0;
            break;

        default:
            vga_putchr(g_ScreenX, g_ScreenY, c);
            g_ScreenX++;
            break;
    }

    if (g_ScreenX >= SCREEN_WIDTH){
        g_ScreenY++;
        g_ScreenX = 0;
    }
    if (g_ScreenY >= SCREEN_HEIGHT)
        vga_scrollback(1);

    vga_setcursor(g_ScreenX, g_ScreenY);
}