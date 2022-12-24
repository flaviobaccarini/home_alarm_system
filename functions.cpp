#include "functions.h"
#include <Keypad.h>             
#include <LiquidCrystal.h>
#include <string.h> 
#include <Password.h>
#include "definition.h"
#include "states.h"


//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState())
  {
    case PRESSED:
    //Serial.print("Pressed: ");
    //Serial.println(eKey);
    psw_to_print += eKey;
    clean_line_lcd(0, 1, "PIN: ");
    lcd.print(psw_to_print);
    switch (eKey)
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
      default: password.append(eKey);
    }
  }
}

char checkPassword(){
  if (password.evaluate()){
    //Serial.println("Success"); //Used for troubleshooting
    password.reset();
    return 2;
  }
  else{
    password.reset();
    return 1;
    }
}

void lcd_print(String first_row, bool is_there_second_row = 0, String second_row = "")
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(first_row);
  if(is_there_second_row)
  {
    lcd.setCursor(0, 1);
    lcd.print(second_row);
  }
}


void blink_func(unsigned long current,
                unsigned long delay_time,
                unsigned long previous,
                int ledPin)
{
  // Check if the interval has elapsed
  if (current - previous >= delay_time) {
    // Save the current time as the previous time
    previousMillis = currentMillis;
    // Toggle the LED
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}

void clean_line_lcd(int col, int row, String string_to_write)
{
  lcd.setCursor(col, row);
  lcd.print("                             ");
  lcd.setCursor(col, row);
  lcd.print(string_to_write);
}

void handle_waiting(unsigned long blink_time, 
                    int pinLed,
                    unsigned long interval_to_wait,
                    State next_STATE){
  currentMillis = millis();
  blink_func(currentMillis, 
             previousMillis,
             blink_time,
             pinLed);
  if ((currentMillis - time_change_case) > interval_to_wait)
  { 
    STATE = next_STATE;

    if (next_STATE == State::PIR_ACTIVATED)
    {
      lcd_print("ALARM ON", 1, "SIREN OFF");
      digital_write_three_leds(redState = LOW, yellowState = LOW, greenState = HIGH);
      //digitalWrite(redLed, LOW);
      //digitalWrite(greenLed, HIGH);
      //digitalWrite(yellowLed, LOW);          
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::BUZZER_ACTIVATED)
    {
      digital_write_three_leds(redState = HIGH, yellowState = HIGH, greenState = HIGH);
      //digitalWrite(redLed, HIGH);
      //digitalWrite(greenLed, HIGH);
      //digitalWrite(yellowLed, HIGH);          
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digital_write_three_leds(redState = LOW, yellowState = LOW, greenState = HIGH);
      //digitalWrite(redLed, LOW);
      //digitalWrite(greenLed, HIGH);
      //digitalWrite(yellowLed, LOW);          
    }   

    if (next_STATE == State::OFFLINE &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digital_write_three_leds(redState = LOW, yellowState = HIGH, greenState = LOW);
      //digitalWrite(redLed, LOW);
      //digitalWrite(greenLed, LOW);
      //digitalWrite(yellowLed, HIGH);      
      sensorData = LOW;    
      lcd_print("ALARM OFF");
    }   
    is_psw_correct = 0;      
  }
}


void digital_write_three_leds(bool redState, bool yellowState, bool greenState)
{
  digitalWrite(redLed, redState);
  digitalWrite(yellowLed, yellowState);
  digitalWrite(greenLed, greenState);
}