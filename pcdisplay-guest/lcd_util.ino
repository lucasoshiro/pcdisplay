#include "lcd_util.hpp"
#include <math.h>

void printCenter (LiquidCrystal lcd, int l, char *s) {
    int spc = (16 - strlen (s)) / 2;
    lcd.clear ();
    lcd.setCursor (spc, l);
    lcd.write (s);
}

void draw_percent (LiquidCrystal lcd, int line, int percent) {
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

void clear_line_section (LiquidCrystal lcd, int line, int index, int end) {
    lcd.setCursor (index, line);
    for (int i = index; i < end; i++) lcd.write (' ');
}
