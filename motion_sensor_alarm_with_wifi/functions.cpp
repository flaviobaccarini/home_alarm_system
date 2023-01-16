#include "functions.h"
extern Password password;
extern LiquidCrystal lcd;
extern unsigned long currentMillis;
extern unsigned long previousMillis;

char checkPassword(){
  /*
    Function for checking if the password inserted by the user is correct or no.
    If the password is correct, the function returns 2, if the password
    is wrong the function returns 1.

    Returns:
    ========
        char 
        1 if the password is wrong, 2 if the password is correct.
  */
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
  /*
    Function for cleaning what is displayed on one line for the LCD display
    and to write something else (string_to_write).
    The column and row are given by parameter. The string to write in the line
    is passed as parameter too.

    Parameters:
    ========
    col: int 
         Integer number for the column from which we want to clean the display
         and print a new string starting from this column.
    
    row: int
        Integer number for the row we want to clean and print a new string.
      
    string_to_write: String
                     The string we want to print after the cleaning of the
                     selected row.
  */
  lcd.setCursor(col, row);
  lcd.print("                             ");
  lcd.setCursor(col, row);
  lcd.print(string_to_write);
}


void lcd_print(String first_row, bool is_there_second_row, String second_row)
{
  /*
    Function for printing on the LCD display for both the rows if necessary.
    The strings to print are passed as paramaters.
    The first row is always printed, instead the second row is optional (it
    depends on the bool is_there_second_row which the default value is equal
    to 0.)

    Parameters:
    ========
    first_row: String 
               The string we want to print for the first row.
    
    is_there_second_row: bool, default: 0 
                         Bool value that tells us if there is also the second 
                         row to be printed or no. Default value is False (0).
      
    second_row: String, default: ""
                The string we want to pruint for the second row if the 
                is_there_second_row is True (1). Default value is an empty
                string ("").
  */
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
  /*
    Function for writing the status of the three LEDs.
    The function takes as input the state of the LEDs.

    Parameters:
    ========
    redState: bool 
              Bool value for the red LED.
    
    yellowState: bool 
                 Bool value for the yellow LED.

    greenState: bool 
                Bool value for the green LED.
  */
  digitalWrite(redLed, redState);
  digitalWrite(yellowLed, yellowState);
  digitalWrite(greenLed, greenState);
}


void blink_func(unsigned long current,
                unsigned long delay_time,
                unsigned long previous,
                int ledPin)
{
  /*
    Function for blinking one LED.
    The function takes as input the times for the blinking
    operation (no used the delay function) and the pin 
    for the LED that we want to make blink.

    Parameters:
    ========
    current: unsigned long 
             This value represents the current result of 
             the function millis().
    
    delay_time: unsigned long 
                The delay time we need to wait for the blink.
          
    previous: unsigned long 
              This value represents the previous result of 
              the function millis().
    
    ledPin: int
            Integer value that represents the LED we want to make
            blink.

  */
  // Check if the interval has elapsed
  if (current - previous >= delay_time) {
    // Save the current time as the previous time
    previousMillis = currentMillis;
    // Toggle the LED
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}

