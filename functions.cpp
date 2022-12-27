#include "functions.h"
extern Password password;
extern LiquidCrystal lcd;
extern unsigned long currentMillis;
extern unsigned long previousMillis;

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

void clean_line_lcd(int col, int row, String string_to_write)
{
  lcd.setCursor(col, row);
  lcd.print("                             ");
  lcd.setCursor(col, row);
  lcd.print(string_to_write);
}


void lcd_print(String first_row, bool is_there_second_row, String second_row)
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

void digital_three_led_pins(bool redState, bool yellowState, bool greenState)
{
  digitalWrite(redLed, redState);
  digitalWrite(yellowLed, yellowState);
  digitalWrite(greenLed, greenState);
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

