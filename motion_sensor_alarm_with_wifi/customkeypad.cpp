#include "customkeypad.h"
#include "functions.h"
extern Password password;
extern LiquidCrystal lcd;

/*
CustomKeypad is a class that is based on the Adafruit keypad class
adding some other custom functions like KeypadEventfunc and keypad_reading.
It is not possible to use the classical Keypad library (for Arduino), since
there is a conflict between the Keypad library and the WiFi module.
*/

void CustomKeypad::KeypadEventfunc(keypadEvent eKey){
    /*
        Function that takes as input a Keypad event by the user.
        The function prints the password on the LCD display and
        evaluates the password if the confirm button is pressed (*)
        or it resets the password if the reset button is pressed (#).
        If other buttons are pressed the char pressed is stored inside
        the password.

        Parameters:
        ===========
        eKey: keypadEvent
              Keypad event in which there is a KEY char that 
              represents the button pressed.
    */
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
                    is_psw_correct = checkPassword(); // 1: wrong psw, 2: correct psw
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
    /*
        Begin function for the Adafruit keypad (the keypad 
        is set if the PINs are correct).
    */
    keypad_.begin();
}
        
void CustomKeypad::tick(){
    /*
        Tick function for the Adafruit keypad (activation of the keypad.)
    */
    keypad_.tick();
}


CustomKeypad::CustomKeypad(byte rowPins[4], byte colPins[4]) : 
        keypad_(makeKeymap(keys), rowPins, colPins, rows, cols) 
{
    /*
        Constructor for the class that exploits the constructor
        of the Adafruit keypad library.
    */
}

void CustomKeypad::keypad_reading()
{
    /*
        Method to handle the reading of the password inserted by the user.
        There is a delay of 250 milliseconds between one reading 
        and the next one because to avoid possible noise.
    */
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
