#include "info.hpp"
#include "specialCharacters.hpp"
#include <LiquidCrystal.h>
#include <string.h>
#include "states.hpp"
#include "lcd_util.hpp"

LiquidCrystal lcd (12, 11, 6, 5, 4, 3);

char serial_buffer[512];
state_t current_state;
int received_last_request;

int read_serial () {
    char *c;
    unsigned long begin = millis ();

    // if (Serial.available () <= 0) return 1;

    for (c = serial_buffer; ; c++) {
        while (Serial.available () <= 0) {
            if (millis () - begin > 100) return 1;
        }
        
        *c = Serial.read ();
        if (*c == '\n') {
            *c = '\0';
            break;
        }
    }
    
    return 0;
}

void try_wait_connection () {
    int disconnected = 0;

    do {
        Serial.println ("HELLO");
        disconnected = read_serial ();
        delay (100);
    } while (disconnected);
}

void setup () {
    Serial.begin (9600);
    
    lcd.createChar (0, charFilledBlock);
    lcd.createChar (1, charBeginEmptyBlock);
    lcd.createChar (2, charMidEmptyBlock);
    lcd.createChar (3, charEndEmptyBlock);
    lcd.createChar (4, charDegrees);
    
    pinMode (13, OUTPUT);
    digitalWrite (13, LOW);
    lcd.begin (16, 2);
    delay (500);
    
    lcd.setCursor (0, 0);
    lcd.print ("Starting...");
    lcd.setCursor (0, 0);
    info_init ();
    delay (500);
}

void loop () {
    try_wait_connection ();
    Serial.println ("NAME");
    parse (serial_buffer);
    Serial.println ("INFO");
    parse (serial_buffer);
    current_state = 0;

    for (;;) {
        // Serial.println ("MEDIA");

        // if (!read_serial ()) 
        //     parse (serial_buffer);

        // lcd.clear ();
        // lcd.setCursor (0, 0);
        // lcd.print (INFO.media_title);
        // lcd.setCursor (0, 1);
        // lcd.print (INFO.media_artist);

        // int v = (unsigned long) analogRead (A0) * 100 / 1024;
        // sprintf (serial_buffer, "VOLUME %d", v);
        // Serial.println (serial_buffer);
        // read_serial ();
        // delay (20);

        Serial.println ("CPU");
        if (!read_serial ()) parse (serial_buffer);
        draw_cpu (lcd);
        delay (100);
    }
}
