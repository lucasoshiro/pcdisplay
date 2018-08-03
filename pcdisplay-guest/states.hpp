#ifndef _STATES
#define _STATES
#include "lcd_util.hpp"

enum state_t {SYSINFO, TIME, CPU, RAM, TEMP, NET, MEDIA};

extern void (*draw[7]) (LiquidCrystal lcd);

void draw_sysinfo (LiquidCrystal lcd);

void draw_time (LiquidCrystal lcd);

void draw_cpu (LiquidCrystal lcd);

void draw_ram (LiquidCrystal lcd);

void draw_temp (LiquidCrystal lcd);

void draw_net (LiquidCrystal lcd);

void draw_media (LiquidCrystal lcd);
#endif
