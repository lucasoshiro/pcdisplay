#ifndef _BLOCKS
#define _BLOCKS

#ifdef DISPLAY_TEXT_16_2
byte charFilledBlock[8] = {
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111
};

byte charBeginEmptyBlock[8] = {
    B11111111,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B11111111
};

byte charMidEmptyBlock[8] = {
    B11111111,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111
};

byte charEndEmptyBlock[8] = {
    B11111111,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B11111111
};

byte charDegrees[8] = {
    B00000111,
    B00000101,
    B00000111,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
};


byte charPlay[8] = {
    B00011000,
    B00011100,
    B00011110,
    B00011111,
    B00011110,
    B00011100,
    B00010000,
    B00000000
};

byte charPause[8] = {
    B00000000,
    B00001010,
    B00001010,
    B00001010,
    B00001010,
    B00001010,
    B00001010,
    B00000000
};
#endif

#ifdef DISPLAY_GRAPHIC_128_64
#include <U8glib.h>

const uint8_t play[] U8G_PROGMEM = {
    0x0,
    0x40,
    0x60,
    0x70,
    0x78,
    0x70,
    0x60,
    0x40
};

const uint8_t cpu[] U8G_PROGMEM = {
    0x1, 0x50,
    0x3, 0xf8,
    0x6, 0xc, 
    0x2, 0x8, 
    0x6, 0xc, 
    0x2, 0x8, 
    0x6, 0xc, 
    0x3, 0xf8,
    0x1, 0x50 
};

const uint8_t ram[] U8G_PROGMEM = {
    0x0, 0x0,  
    0x0, 0x0, 
    0xf, 0xfe,
    0xa, 0xaa,
    0x8, 0x2, 
    0x8, 0x2, 
    0xf, 0xfe 
};

const uint8_t termo[] U8G_PROGMEM = {
    0x20,
    0x50,
    0x50,
    0x50,
    0x50,
    0x50,
    0x88,
    0xa8,
    0x88,
    0x70
};

const uint8_t upArrow[] U8G_PROGMEM = {
    0x8,
    0x1c,
    0x3e,
    0x8,
    0x8,
    0x8,
    0x8
};

const uint8_t downArrow[] U8G_PROGMEM = {
    0x8,
    0x8,
    0x8,
    0x8,
    0x3e,
    0x1c,
    0x8
};

const uint8_t degree[] U8G_PROGMEM = {
    0x4,
    0xa,
    0x4
};
#endif

#endif
