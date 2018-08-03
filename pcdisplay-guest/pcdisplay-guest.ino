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
int change_screen = 0;

int read_serial () {
    char *c;
    unsigned long begin = millis ();

    for (c = serial_buffer; ; c++) {
        while (Serial.available () <= 0) {
            if (millis () - begin > 1000) return 1;
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
        disconnected = read_serial ();// && !IS_HELLO (serial_buffer);
        delay (100);
    } while (disconnected);
}

void schedule_screen_change () {
    change_screen = 1;
}

void setup () {
    Serial.begin (9600);
    
    lcd.createChar (0, charFilledBlock);
    lcd.createChar (1, charBeginEmptyBlock);
    lcd.createChar (2, charMidEmptyBlock);
    lcd.createChar (3, charEndEmptyBlock);
    lcd.createChar (4, charDegrees);
    
    pinMode (13, OUTPUT);
    pinMode (2, INPUT);

    attachInterrupt (digitalPinToInterrupt (2), schedule_screen_change, RISING);

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
    read_serial ();
    parse (serial_buffer);
    
    Serial.println ("SYSINFO");
    read_serial ();
    parse (serial_buffer);
    
    current_state = 0;

    for (;;) {
        if (change_screen) {
            current_state = (current_state + 1) % 5;
            delay (100);
            change_screen = 0;
        }

        draw[current_state] (lcd);
        delay (500);
    }
}
