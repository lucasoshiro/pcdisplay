#ifndef _LCD_UTIL
#define _LCD_UTIL

#include <LiquidCrystal.h>

void print_center (LiquidCrystal lcd, int l, char *s);

void draw_percent (LiquidCrystal lcd, int line, int percent);
#endif
