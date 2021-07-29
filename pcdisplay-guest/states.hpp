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

#else
enum state_t {NUM_STATES};
extern void (*draw[NUM_STATES]) ();
#endif

#endif
