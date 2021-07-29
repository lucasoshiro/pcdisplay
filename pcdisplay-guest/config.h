#ifndef _CONFIG
#define _CONFIG

// Use 16x2 character display or 128x64 graphic display

//#define DISPLAY_TEXT_16_2
//#define DISPLAY_GRAPHIC_128_64

#if !(defined(DISPLAY_TEXT_16_2) ^ defined(DISPLAY_GRAPHIC_128_64))
#error "Display type not defined"
#endif

#endif
