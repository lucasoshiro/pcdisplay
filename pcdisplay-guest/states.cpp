#include "states.hpp"
#include "config.h"
#include "info.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef DISPLAY_GRAPHIC_128_64
#include "bitmap.hpp"
#endif

void (*draw[NUM_STATES]) () =
{
#ifdef DISPLAY_TEXT_16_2
    draw_sysinfo,
    draw_time,
    draw_cpu,
    draw_ram,
    draw_temp,
    draw_net,
    draw_media
#endif

#ifdef DISPLAY_GRAPHIC_128_64
    draw_info
#endif
};

static int log2 (unsigned long n) {
    int l;
    for (l = -1; n > 0; l++, n >>= 1);
    return l;
}

#ifdef DISPLAY_TEXT_16_2

void draw_sysinfo () {
    lcd.setCursor (0, 0);
    lcd.print (INFO.computer_name);
    clear_line_section (0, strlen (INFO.computer_name), 17);

    lcd.setCursor (0, 1);
    lcd.print (INFO.sys_info);
    clear_line_section (1, strlen (INFO.sys_info), 17);
}

void draw_time () {
    char buffer[2][17];

    sprintf (buffer[0], "%02d/%02d/%04d", INFO.day, INFO.month, INFO.year);
    sprintf (buffer[1], "%02d:%02d", INFO.hour, INFO.min);

    lcd.setCursor (0, 0);
    lcd.print (buffer[0]);
    clear_line_section (0, strlen (buffer[0]), 17);

    lcd.setCursor (0, 1);
    lcd.print (buffer[1]);
    clear_line_section (1, strlen (buffer[1]), 17);
}

void draw_cpu () {
    char line[17];

    sprintf (line, "CPU: %d%%", INFO.cpu_usage);

    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (0, strlen (line), 17);
    draw_percent (2, INFO.cpu_usage);
}

void draw_ram () {
    char line[17];
    char used_str[8], total_str[8];

    float used_gb  = (float) INFO.ram_used  / (1 << 10);
    float total_gb = (float) INFO.ram_total / (1 << 10);

    int used_int  = used_gb,  used_dec  = round((used_gb - used_int)   * 10);
    int total_int = total_gb, total_dec = round((total_gb - total_int) * 10);

    if (used_int < 10) sprintf (used_str, "%d.%d", used_int, used_dec);
    else sprintf (used_str, "%d", used_int);

    if (total_int < 10) sprintf (total_str, "%d.%d", total_int, total_dec);
    else sprintf (total_str, "%d", total_int);

    sprintf (line, "RAM: %s/%s GB", used_str, total_str);

    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (0, strlen (line), 17);
    draw_percent (2, 100 * INFO.ram_used / INFO.ram_total);
}

void draw_temp () {
    char line[17];

    sprintf (line, "CPU Temp: %d %cC", INFO.temp, 4);

    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (0, strlen (line), 17);
    clear_line_section (1, 0, 17);
}

void draw_net () {
    char unit_str[][3] = {" B", "kB", "MB", "GB"};
    char line[128];

    int unit_up   = log2 (INFO.net_up_speed) / 10;
    int unit_down = log2 (INFO.net_down_speed) / 10;

    sprintf (line, "Up:   %lu %s/s",
             INFO.net_up_speed / ((unsigned long) 1 << 10 * unit_up),
             unit_str[unit_up]);
    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (0, strlen (line), 17);

    sprintf (line, "Down: %lu %s/s",
             INFO.net_down_speed / ((unsigned long)1 << 10 * unit_down),
             unit_str[unit_down]);
    lcd.setCursor (0, 1);
    lcd.print (line);
    clear_line_section (1, strlen (line), 17);
}

void draw_media () {
    char *line;

    line = new char[strlen (INFO.media_title) + 10];
    if (INFO.media_track != 0)
        sprintf (line, "%d - %s  ", INFO.media_track, INFO.media_title);
    else
        sprintf (line, "%s  ", INFO.media_title);

    static RotatingLine t (line, 0);
    if (!t.same_str (line)) t.reset(line);
    t.print ();
    delete[] line;

    line = new char[strlen (INFO.media_artist) + 10];
    sprintf (line, "%s  ", INFO.media_artist);

    static RotatingLine a (line, 1);
    if (!a.same_str (line)) a.reset (line);
    a.print ();
    delete[] line;
}

#endif

#ifdef DISPLAY_GRAPHIC_128_64

RotatingLine rl1 ("", 1, 46);
RotatingLine rl2 ("", 1, 55);


static void draw_cpu () {
    char bff[32];

    sprintf (bff, "%4d%%", INFO.cpu_usage);
    u8g.drawStr (93, 0, bff);
    u8g.drawBitmapP (53, 0, 2, 9, cpu);
    drawPercent (70, 3, INFO.cpu_usage);
}

static void draw_ram () {
    char bff[32];

    float used_gb  = (float) INFO.ram_used  / (1 << 10);
    float total_gb = (float) INFO.ram_total / (1 << 10);

    int used_int  = used_gb,  used_dec  = round ((used_gb - used_int)   * 10);
    int total_int = total_gb, total_dec = round ((total_gb - total_int) * 10);

    if (used_int < 10) sprintf (bff, "%d.%dGB", used_int, used_dec);
    else sprintf (bff, "%3dGB", used_int);

    u8g.drawStr (93, 9, bff);
    u8g.drawBitmapP (53, 9, 2, 7, ram);
    drawPercent (70, 12, 100 * used_int / total_int);
}

static void draw_temperature () {
    char bff[32];

    sprintf (bff, "%3d C", INFO.temp);
    u8g.drawStr (93, 18, bff);
    u8g.drawBitmapP (60, 18, 1, 10, termo);
    u8g.drawBitmapP (109, 20, 1, 3, degree);
    drawPercent (70, 21, INFO.temp);
}

static void draw_net () {
    char unit_str[][3] = {"B", "kB", "MB", "GB"};
    char bff[32];

    int unit_up   = log2 (INFO.net_up_speed) / 10;
    int unit_down = log2 (INFO.net_down_speed) / 10;

    sprintf (bff, "%3lu%s/s",
             INFO.net_up_speed / ((unsigned long) 1 << 10 * unit_up),
             unit_str[unit_up]);
    u8g.drawStr (9, 0, bff);
    u8g.drawBitmapP (0, 0, 1, 7, upArrow);

    sprintf (bff, "%3lu%s/s",
             INFO.net_down_speed / ((unsigned long) 1 << 10 * unit_down),
             unit_str[unit_down]);
    u8g.drawStr (9, 9, bff);
    u8g.drawBitmapP (0, 9, 1, 7, downArrow);
}

static void draw_media () {
    char *line;

    // Track
    line = new char[strlen (INFO.media_title) + 24];
    if (INFO.media_track > 0)
        sprintf (line, "%d - %s  ", INFO.media_track, INFO.media_title);
    else
        sprintf (line, "%s  ", INFO.media_title);

    if (!rl1.same_str (line)) rl1.reset(line);
    rl1.print();
    delete[] line;

    // Artist
    line = new char[strlen (INFO.media_artist) + 24];
    sprintf (line, "%s  ", INFO.media_artist);

    if (!rl2.same_str (line)) rl2.reset(line);
    rl2.print();
    delete[] line;
}

static void draw_info_callback () {
    u8g.setFont(u8g_font_6x10);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();

    draw_cpu ();
    draw_ram ();
    draw_temperature ();
    draw_net ();
    draw_media ();
}

void draw_info () {
    u8g.firstPage ();
    rl1.update();
    rl2.update();
    do { draw_info_callback (); } while (u8g.nextPage ());
}
#endif
