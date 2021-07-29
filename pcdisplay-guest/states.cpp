#include "states.hpp"
#include "config.h"
#include "info.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>


static int log2 (unsigned long n);

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
};

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
    char unit_str[][3] = {"B", "kB", "MB", "GB"};
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

static int log2 (unsigned long n) {
    int l;
    for (l = -1; n > 0; l++, n >>= 1);
    return l;
}
