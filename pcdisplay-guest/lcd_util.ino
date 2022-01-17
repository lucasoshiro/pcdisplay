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

#ifdef DISPLAY_GRAPHIC_128_64
#include <U8glib.h>
U8GLIB_ST7920_128X64_1X u8g (6, 5, 4 ,7);

void drawPercent (int i, int j, int perc) {
    const int size = 21;
    const int height = 3;

    u8g.drawFrame (i, j, size, height);
    u8g.drawBox (i, j, size * perc / 100, height);
}
#endif

#ifdef DISPLAY_TEXT_16_2
#define ROTATING_MAX_LENGTH 16
#endif

#ifdef DISPLAY_GRAPHIC_128_64
#define ROTATING_MAX_LENGTH 21
#endif

#ifdef DISPLAY_TEXT_16_2
RotatingLine::RotatingLine (
    char *s,
    int line
    ) {

    this->len = strlen (s);
    this->s = new char[this->len + 1];
    strcpy (this->s, s);

    this->line = line;
    this->first = -1;
    this->skip_count = -1;
}
#endif


#ifdef DISPLAY_GRAPHIC_128_64
RotatingLine::RotatingLine (
    char *s,
    u8g_uint_t x,
    u8g_uint_t y
    ) {
    this->len = strlen (s);
    this->s = new char[this->len + 1];
    strcpy (this->s, s);

    this->x = x;
    this->y = y;

    this->first = 0;
    this->skip_count = 0;
}
#endif


RotatingLine::~RotatingLine () {
    delete[] this->s;
}

void RotatingLine::print () {
    int i;

#ifdef DISPLAY_TEXT_16_2
    this->update();
    if (this->skip_count != 0) return;

    lcd.setCursor (0, this->line);
#endif

    if (this->len <= ROTATING_MAX_LENGTH) {

#ifdef DISPLAY_TEXT_16_2
        lcd.print (this->s);
        clear_line_section (this->line, this->len, 16);
#endif

#ifdef DISPLAY_GRAPHIC_128_64
        u8g.drawStr (this->x, this->y, this->s);
#endif
        return;
    }

#ifdef DISPLAY_TEXT_16_2
    for (i = 0; i < 16; i++) {
        lcd.write (this->s[(this->first + i) % this->len]);
    }

    clear_line_section (this->line, i, 17);
#endif

#ifdef DISPLAY_GRAPHIC_128_64
    {
        char *s_tmp = new char[this->len * 2 + 4];
        strcpy(s_tmp, this->s);
        strcat(s_tmp, "  ");
        strcat(s_tmp, this->s);

        if (this->first + ROTATING_MAX_LENGTH < this->len * 2 + 4)
            s_tmp[this->first + ROTATING_MAX_LENGTH] = '\0';

        u8g.drawStr (this->x, this->y, s_tmp + this->first);
        delete[] s_tmp;
    }
#endif
}

void RotatingLine::update() {
    this->skip_count++;
    this->skip_count %= ROTATING_SKIP;

    if (this->skip_count == 0) {
        this->first++;
        this->first %= this->len;
    }
}

int RotatingLine::same_str (char *s) {
    return strcmp(s, this->s) == 0;
}

void RotatingLine::reset (char *s) {
    this->len = strlen (s);
    delete[] this->s;
    this->s = new char[this->len + 1];
    strcpy (this->s, s);
    this->first = -1;
    this->skip_count = -1;
}
