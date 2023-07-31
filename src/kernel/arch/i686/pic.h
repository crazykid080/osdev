#pragma once

#include <stdint.h>

#define PIC1_COMMAND_PORT           0x20
#define PIC1_DATA_PORT              0x21
#define PIC2_COMMAND_PORT           0xA0
#define PIC2_DATA_PORT              0xA1

void i686_PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void i686_PIC_SendEndOfInterrupt(int irq);
void i686_PIC_Disable();
void i686_PIC_Mask(int irq);
void i686_PIC_Unmask(int irq);
uint16_t i686_PIC_ReadIrqRequestRegister();
uint16_t i686_PIC_ReadInServiceRegister();