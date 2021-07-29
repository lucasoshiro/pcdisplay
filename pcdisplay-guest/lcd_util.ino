#include "lcd_util.hpp"
#include <math.h>
#include <string.h>

#ifdef DISPLAY_TEXT_16_2
LiquidCrystal lcd (12, 11, 6, 5, 4, 3);

void printCenter (int l, char *s) {
    int spc = (16 - strlen (s)) / 2;
    lcd.clear ();
    lcd.setCursor (spc, l);
    lcd.write (s);
}

void draw_percent (int line, int percent) {
    int fill = round (percent * 16. / 100.);
    
    lcd.setCursor (0, line);
    for (int i = 0; i < fill; i++) lcd.write (byte (0));

    if (fill == 0) {
        lcd.write (byte (1));
        fill++;
    }

    for (int i = fill; i <= 14; i++) lcd.write (byte (2));

    if (fill < 16) {
        lcd.setCursor (15, line);
        lcd.write (byte (3));
    }
}

void clear_line_section (int line, int index, int end) {
    lcd.setCursor (index, line);
    for (int i = index; i < end; i++) lcd.write (' ');
}

#endif

RotatingLine::RotatingLine (char *s, int line) {
    this->len = strlen (s);
    this->s = new char[this->len + 1];
    strcpy (this->s, s);
    this->line = line;
    this->first = 0;
    this->skip_count = -1;
}

RotatingLine::~RotatingLine () {
    delete[] this->s;
}

void RotatingLine::print () {
    int i;

    this->skip_count = (this->skip_count + 1) % ROTATING_SKIP;
    if (this->skip_count != 0) return;
    
    #ifdef DISPLAY_TEXT_16_2
    lcd.setCursor (0, this->line);
    #endif

    if (this->len < 16) {
        #ifdef DISPLAY_TEXT_16_2
        lcd.print (this->s);
        clear_line_section (this->line, this->len, 16);
        #endif
        return;
    }

    for (i = 0; i < 16; i++) {
        #ifdef DISPLAY_TEXT_16_2
        lcd.write (this->s[(this->first + i) % this->len]);
        #endif
    }
    
    #ifdef DISPLAY_TEXT_16_2
    clear_line_section (this->line, i, 17);
    #endif

    this->first = (this->first + 1) % this->len;
}

int RotatingLine::same_str (char *s) {
    return strcmp(s, this->s) == 0;
}

void RotatingLine::reset (char *s) {
    this->len = strlen (s);
    delete[] this->s;
    this->s = new char[this->len + 1];
    strcpy (this->s, s);
    this->first = 0;
    this->skip_count = -1;
}
