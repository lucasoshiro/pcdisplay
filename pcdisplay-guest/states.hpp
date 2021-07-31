#ifndef _STATES
#define _STATES

#include "lcd_util.hpp"
#include "config.h"

#ifdef DISPLAY_TEXT_16_2
enum state_t {SYSINFO, TIME, CPU, RAM, TEMP, NET, MEDIA, NUM_STATES};

extern void (*draw[NUM_STATES]) ();

void draw_sysinfo ();

void draw_time ();

void draw_cpu ();

void draw_ram ();

void draw_temp ();

void draw_net ();

void draw_media ();

#endif

#ifdef DISPLAY_GRAPHIC_128_64
enum state_t {ALL_INFO, NUM_STATES};

extern void (*draw[NUM_STATES]) ();

void draw_info ();
#endif

#endif
