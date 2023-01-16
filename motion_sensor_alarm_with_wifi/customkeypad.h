#include <Adafruit_Keypad.h> 
#ifndef CUSTOMKEYPAD_H
#define CUSTOKEYPAD_H
class CustomKeypad{
    private:
        const byte rows = 4;                     // four rows       
        const byte cols = 4;                     // four columns
        char keys[4][4] = {
            {'1','2','3','A'},
            {'4','5','6','B'},
            {'7','8','9','C'},
            {'*','0','#','D'},
        };
        Adafruit_Keypad keypad_;
        String psw_to_print; //password that will be printed in the LCD
        unsigned long time_keypad_reading, previous_time_keypad_reading = 0; //avoid delay function with these two variables
        void KeypadEventfunc(keypadEvent eKey);
    public:
        char is_psw_correct = 0; //0: wait for the password, 1: wrong password, 2: correct password
        void begin();
        void tick();
        CustomKeypad(byte rowPins[4], byte colPins[4]);
        void keypad_reading();
};
#endif
