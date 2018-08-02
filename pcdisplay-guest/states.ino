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
    
}

void draw_time (LiquidCrystal lcd) {
    
}

void draw_cpu (LiquidCrystal lcd) {
    char line[17];

    sprintf (line, "CPU: %d%%", INFO.cpu_usage);
    
    lcd.setCursor (0, 0);
    lcd.print (line);
    for (int i = strlen (line) + 1; i < 16; i++) lcd.write (' ');
    draw_percent (lcd, 2, INFO.cpu_usage);
}

void draw_ram (LiquidCrystal lcd) {
    char line[17];
    sprintf (line, "RAM: %.2f/%.2f",
             (double) INFO.ram_used / (1 << 30),
             (double) INFO.ram_total / (1 << 30));

    lcd.setCursor (0, 0);
    lcd.print (line);
    for (int i = strlen (line) + 1; i < 16; i++) lcd.write (' ');
    draw_percent (lcd, 2, 100 * INFO.ram_used / INFO.ram_total);
}

void draw_temp (LiquidCrystal lcd) {
    
}

void draw_net (LiquidCrystal lcd) {
    
}

void draw_media (LiquidCrystal lcd) {
    
}


