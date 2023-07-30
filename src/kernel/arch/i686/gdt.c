#include "gdt.h"
#include <stdint.h>

typedef struct{
    uint16_t LimitLow;              // The segment limit (0-15)
    uint16_t BaseLow;               // Base (0-15)
    uint8_t BaseMiddle;             // Base (16-25)
    uint8_t Access;                 // Access control
    uint8_t FlagsLimitHi;           // Limit (16-19) |  flags
    uint8_t BaseHigh;               // Base (24-31)
}__attribute__((packed)) GDTEntry;


typedef struct{
    uint16_t Limit;                 // sizeof(gdt) - 1
    GDTEntry* Ptr;                  // Address of GDT

} __attribute__((packed)) GDTDescriptor;


typedef enum{
    GDT_ACCESS_CODE_READABLE            = 0x02,
    GDT_ACCESS_DATA_WRITABLE            = 0x02,

    GDT_ACCESS_CODE_CONFORMING          = 0x02,
    GDT_ACCESS_DATA_DIRECTION_NORMAL    = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN      = 0x04,

    GDT_ACCESS_DATA_SEGMENT             = 0x10,
    GDT_ACCESS_CODE_SEGMENT             = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS           = 0x00,

    GDT_ACCESS_RING0                    = 0x00,
    GDT_ACCESS_RING1                    = 0x20,
    GDT_ACCESS_RING2                    = 0x40,
    GDT_ACCESS_RING3                    = 0x60,

    GDT_ACCESS_PRESENT                  = 0x80,
} GDT_ACCESS;

typedef enum {
    GDT_FLAG_64BIT                      = 0X20,
    GDT_FLAG_32BIT                      = 0X40,
    GDT_FLAG_16BIT                      = 0X00,

    GDT_FLAG_GRANULARITY_1B             = 0X00,
    GDT_FLAG_GRANULARITY_4K             = 0X80,
} GDT_FLAGS;

#define GDT_LIMIT_LOW(limit)                (limit & 0xFFFF) //Get lower 16 bits
#define GDT_BASE_LOW(base)                  (base & 0xFFFF) //Get lower 16 bits
#define GDT_BASE_MIDDLE(base)               ((base >> 16) & 0xFF) //Get middle bits by shifting 16 bits and then getting the first 8
#define GDT_FLAGS_LIMIT_HI(limit, flags)    ((limit >> 16) & 0xF) | (flags & 0xF0) //Combine flags with high limits
#define GDT_BASE_HIGH(base)                 ((base >> 24) & 0xFF)


#define GDT_ENTRY(base, limit, access, flags){  \
    GDT_LIMIT_LOW(limit),                       \
    GDT_BASE_LOW(base),                         \
    GDT_BASE_MIDDLE(base),                       \
    access,                                     \
    GDT_FLAGS_LIMIT_HI(limit, flags),            \
    GDT_BASE_HIGH(base)                         \
}

GDTEntry g_GDT[] = {
    //NULL descriptor
    GDT_ENTRY(0, 0, 0, 0),
    GDT_ENTRY(0,
              0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0| GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)
    ),
    GDT_ENTRY(0,
              0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0| GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)
    )
};

GDTDescriptor g_GDTDescriptor = {
    sizeof(g_GDT) - 1, g_GDT
};

void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);

void i686_GDT_Initialize(){
    i686_GDT_Load(&g_GDTDescriptor, i686_GDT_CODE_SEGMENT, i686_GDT_DATA_SEGMENT);
}