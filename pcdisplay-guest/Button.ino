#include "Button.hpp"

Button::Button (int pin) : pin (pin) {
    oldstate = LOW;
}

void Button::begin () {
    pinMode (pin, HIGH);
}

bool Button::pressed () {
    int newstate = digitalRead (pin);
    bool ret = false;
    
    if (newstate == HIGH && oldstate == LOW) ret = true;
    
    oldstate = newstate;
    return ret;
}