#include "info.hpp"
#include "bitmap.hpp"
#include <LiquidCrystal.h>
#include <string.h>
#include "states.hpp"
#include "lcd_util.hpp"
#include "config.h"

char serial_buffer[256];
state_t current_state;
int current_request;
int received_last_request;
int change_screen = 0;
int connected;

int read_serial () {
    char *c;
    unsigned long begin = millis ();

    for (c = serial_buffer; ; c++) {
        while (Serial.available () <= 0) {
            if (millis () - begin > 1000) return 1;
        }
        
        *c = Serial.read ();
        if (*c == '\n' || *c == '\0') {
            *c = '\0';
            break;
        }
    }
    
    return 0;
}

void try_wait_connection () {
    int disconnected = 0;

    do {
        Serial.println ("HELLO \"PCDISPLAY\"");
        disconnected = read_serial () && !IS_HELLO (serial_buffer);
        delay (100);
    } while (disconnected);
}

void schedule_screen_change () {
    change_screen = 1;
}

void handle_screen_change () {
    if (change_screen) {
        current_state = (current_state + 1) % NUM_STATES;
        delay (100);
        change_screen = 0;
    }
}

void send_requests () {
    Serial.println (REQUEST_COMMANDS[current_request]);
}

void handle_responses () {
    if (read_serial ()) connected = 0;
    else {
        parse (serial_buffer);
        current_request = (current_request + 1) % NUM_REQUESTS;
    }
}

void setup () {
    Serial.begin (9600);
    
    #ifdef DISPLAY_TEXT_16_2
    lcd.createChar (0, charFilledBlock);
    lcd.createChar (1, charBeginEmptyBlock);
    lcd.createChar (2, charMidEmptyBlock);
    lcd.createChar (3, charEndEmptyBlock);
    lcd.createChar (4, charDegrees);
    #endif

    #ifdef DISPLAY_GRAPHIC_128_64
    u8g.setColorIndex(1);
    #endif

    pinMode (13, OUTPUT);

    #ifdef DISPLAY_TEXT_16_2
    pinMode (2, INPUT);

    attachInterrupt (digitalPinToInterrupt (2), schedule_screen_change, RISING);
    #endif

    digitalWrite (13, LOW);

    #ifdef DISPLAY_TEXT_16_2
    lcd.begin (16, 2);
    #endif

    delay (500);
    
    #ifdef DISPLAY_TEXT_16_2
    lcd.setCursor (0, 0);
    lcd.print ("Starting...");
    lcd.setCursor (0, 0);
    #endif

    info_init ();
    delay (500);
}

void before () {
    try_wait_connection ();

    connected = 1;
    
    Serial.println ("NAME");
    if (read_serial ()) return;
    parse (serial_buffer);
    
    Serial.println ("SYSINFO");
    if (read_serial ()) return;
    parse (serial_buffer);

    for (int i = 0; i < NUM_REQUESTS; i++) {
        Serial.println (REQUEST_COMMANDS[i]);
        if (read_serial ()) return;
        parse (serial_buffer);
    }

    current_state = 0;
    current_request = 0;
}

void main_loop () {
    while (connected) {
        handle_screen_change ();
        send_requests ();
        handle_responses ();
        draw[current_state] ();
        delay (100);
    }
}

void loop () {
    before ();
    main_loop ();

    #ifdef DISPLAY_TEXT_16_2
    lcd.clear ();
    lcd.setCursor (0, 0);
    lcd.print ("Connection lost");
    #endif

    delay (1000);
}
