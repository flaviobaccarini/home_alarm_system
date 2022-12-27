#include "customkeypad.h"
#include "functions.h"
extern Password password;
extern LiquidCrystal lcd;

void CustomKeypad::KeypadEventfunc(keypadEvent eKey){
    switch(eKey.bit.EVENT)
    {
        case KEY_JUST_PRESSED:
            char eKey_char = (char)eKey.bit.KEY;
            //Serial.print("Pressed: ");
            //Serial.println(eKey_char);
            psw_to_print += eKey_char;
            //Serial.println(psw_to_print);
            clean_line_lcd(0, 1, "PIN: ");
            lcd.print(psw_to_print);
            switch (eKey_char)
            {
                case '*': 
                {
                    is_psw_correct = checkPassword();
                    psw_to_print = "";
                    break;
                }
                case '#':
                {
                    password.reset(); 
                    psw_to_print = "";
                    break;
                } 
                default: password.append(eKey_char);
            }
    }
} // funct parenthesis

void CustomKeypad::begin()
{
    keypad_.begin();
}
        
void CustomKeypad::tick(){
    keypad_.tick();
}


CustomKeypad::CustomKeypad(byte rowPins[4], byte colPins[4]) : 
        keypad_(makeKeymap(keys), rowPins, colPins, rows, cols) {}

void CustomKeypad::keypad_reading()
{
    while(keypad_.available())
    {
        time_keypad_reading = millis();
        if (time_keypad_reading - previous_time_keypad_reading > 250)
        {
            keypadEvent e = keypad_.read();
            KeypadEventfunc(e);
            previous_time_keypad_reading = time_keypad_reading;
        }
    }
}
