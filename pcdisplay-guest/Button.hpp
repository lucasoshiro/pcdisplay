#ifndef _BUTTON
#define _BUTTON

class Button {
public:
    Button (int pin);
    void begin ();
    bool pressed ();
private:
    int oldstate;
    int pin;
};

#endif
