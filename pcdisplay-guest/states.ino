#include "states.hpp"
#include "info.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

void (*draw[7]) () = {draw_sysinfo,
                      draw_time,
                      draw_cpu,
                      draw_ram,
                      draw_temp,
                      draw_net,
                      draw_media};

void draw_sysinfo (LiquidCrystal lcd) {
    lcd.setCursor (0, 0);
    lcd.print (INFO.computer_name);
    clear_line_section (lcd, 0, strlen (INFO.computer_name), 17);
    lcd.setCursor (0, 1);
    lcd.print (INFO.sys_info);
    clear_line_section (lcd, 1, strlen (INFO.sys_info), 17);
        
}

void draw_time (LiquidCrystal lcd) {
    char buffer[2][17];

    sprintf (buffer[0], "%02d/%02d/%d", INFO.day, INFO.month, INFO.year);
    sprintf (buffer[1], "%d:%d", INFO.hour, INFO.min);

    lcd.setCursor (0, 0);
    lcd.print (buffer[0]);
    clear_line_section (lcd, 0, strlen (buffer[0]), 17);

    lcd.setCursor (0, 1);
    lcd.print (buffer[1]);
    clear_line_section (lcd, 1, strlen (buffer[1]), 17);
}

void draw_cpu (LiquidCrystal lcd) {
    char line[17];

    sprintf (line, "CPU: %d%%", INFO.cpu_usage);
    
    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (lcd, 0, strlen (line), 17);
    draw_percent (lcd, 2, INFO.cpu_usage);
}

void draw_ram (LiquidCrystal lcd) {
    char line[17];
    sprintf (line, "RAM: %.2f/%.2f GB",
             (double) INFO.ram_used / (1 << 30),
             (double) INFO.ram_total / (1 << 30));

    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (lcd, 0, strlen (line), 17);
    draw_percent (lcd, 2, 100 * INFO.ram_used / INFO.ram_total);
}

void draw_temp (LiquidCrystal lcd) {
    char line[17];

    sprintf (line, "CPU Temp: %d C", INFO.temp);

    lcd.setCursor (0, 0);
    lcd.print (line);
    clear_line_section (lcd, 0, strlen (line), 17);
}

void draw_net (LiquidCrystal lcd) {
}

void draw_media (LiquidCrystal lcd) {
    
}


