#ifndef _STATES
#define _STATES
#include "lcd_util.hpp"

#define NUM_STATES 7

enum state_t {SYSINFO, TIME, CPU, RAM, TEMP, NET, MEDIA};

extern void (*draw[7]) ();

void draw_sysinfo ();

void draw_time ();

void draw_cpu ();

void draw_ram ();

void draw_temp ();

void draw_net ();

void draw_media ();
#endif
