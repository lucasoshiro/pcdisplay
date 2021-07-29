#ifndef _LCD_UTIL
#define _LCD_UTIL

#include "config.h"

#ifdef DISPLAY_TEXT_16_2
#include <LiquidCrystal.h>
#endif

#define ROTATING_SKIP 4

#ifdef DISPLAY_TEXT_16_2
extern LiquidCrystal lcd;

void print_center (int l, char *s);

void draw_percent (int line, int percent);

void clear_line_section (int line, int begin, int end);
#endif

class RotatingLine {
public:
    RotatingLine (char* s, int line);
    ~RotatingLine ();
    void print ();
    int same_str (char *s);
    void reset (char *s);
private:
    char *s;
    int line;
    int first;
    int len;
    int skip_count;
};

#endif
