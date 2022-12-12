#include <Keypad.h>             // library for keyboard
#include<LiquidCrystal.h>
#include <string.h> 
#include <Password.h>

Password password = Password("1234");  // password
String psw_to_print; 

const byte rows = 4;                     // four rows       
const byte cols = 4;                     // three columns
char keys[rows][cols] = {                // keys on keypad

{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'},

};

byte rowPins[rows] = {13,12,11,10};
byte colPins[cols] = {9,8,7,6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

#define sensorz A3      // pin for PIR sensor data
#define redLed A2        //  pin for red led
#define greenLed A0        // pin for green led
#define yellowLed A1    // pin for blue led
#define buzzer A4

int sensorData;
LiquidCrystal lcd (0,1,2,3,4,5);

// Create a variable to store the current time
unsigned long currentMillis;
// Create a variable to store the previous time the LED was updated
unsigned long previousMillis = 0;
unsigned long time_change_case;
State STATE = State::OFFLINE;
State previous_STATE;
State next_STATE;
char is_psw_correct = 0;
unsigned long time_buzzer;
unsigned long time_buzzer_prev = 0;


enum class State {
  OFFLINE, // initial state
  WAITING, // waiting state
  PIR_ACTIVATED, // PIR sensor is active
  BUZZER_ACTIVATED, // BUZZER is active
  INCORRECT // password is incorrect
};

void setup()
  {
  keypad.addEventListener(keypadEvent);
  pinMode(sensorz, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT); 
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("ALARM OFF");
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

void loop()
{
  switch(STATE)
  {
    case State::OFFLINE:
      { 
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, LOW);
        digitalWrite(yellowLed, HIGH);
        keypad.getKey(); 

        if (is_psw_correct == 2)
        {
          STATE = State::WAITING;
          //next_STATE = State::PIR_ACTIVATED;
          previous_STATE = State::OFFLINE;
          time_change_case = millis();
          lcd_print("ACTIVATION");
          
        }
        else if (is_psw_correct == 1)
        {
          STATE = State::WAITING;
          //next_STATE = State::INCORRECT;
          previous_STATE = State::OFFLINE;
          time_change_case = millis();
          lcd_print("WRONG, RETRY");
        }
      }
    break;

    case State::WAITING:
    {
      if(previous_STATE == State::OFFLINE &&
                           is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       5000, State::PIR_ACTIVATED);
      }

      if(previous_STATE == State::OFFLINE && 
                           is_psw_correct == 1)
      {
        handle_waiting(100, redLed,
                       1000, State::INCORRECT);
      }

      if(previous_STATE == State::BUZZER_ACTIVATED &&
                           is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       5000, State::OFFLINE);
      }

      if(previous_STATE == State::BUZZER_ACTIVATED &&
                           is_psw_correct == 1)
      {
        handle_waiting(100, redLed,
                       1000, State::INCORRECT);
      }

      if(previous_STATE == State::PIR_ACTIVATED &&
                           is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       2000, State::OFFLINE);
      }

      if(previous_STATE == State::PIR_ACTIVATED &&
                           is_psw_correct == 1)
      {
        handle_waiting(100, redLed,
                       500, State::INCORRECT);
      }
    }
    break;

    case State::PIR_ACTIVATED:
    { 
      sensorData = digitalRead(sensorz);
      if (sensorData == HIGH)
      {
        STATE = State::BUZZER_ACTIVATED;
        lcd_print("ALARM ON", 1, "SIREN ON");
        digitalWrite(redLed, HIGH);
        digitalWrite(yellowLed, HIGH);
        digitalWrite(greenLed, HIGH);
      }
      else
      {
        keypad.getKey();
        if (is_psw_correct == 2)
        {
          previous_STATE = State::PIR_ACTIVATED;
          time_change_case = millis();
          lcd_print("DEACTIVATION");
          STATE = State::WAITING;
        }
        else if (is_psw_correct == 1)
        {
          previous_STATE = State::PIR_ACTIVATED;
          time_change_case = millis();
          STATE = State::WAITING;
          lcd_print("ALARM ON", 1, "WRONG, RETRY");
        }
      }
    }
    break;

    case State::BUZZER_ACTIVATED:
    {
      keypad.getKey();

        for(double x = 0; x < 0.92; x += 0.01)
          {  // Elegant Alarm Tone
            time_buzzer = millis();
            if (time_buzzer - time_buzzer_prev >= 1){
                time_buzzer_prev = time_buzzer;
                tone(buzzer, sinh(x+8.294), 10);
              }
          }  
          
      if (is_psw_correct == 2)
      {
        STATE = State::WAITING;
        previous_STATE = State::BUZZER_ACTIVATED;
        time_change_case = millis();     
        lcd_print("ALARM OFF");
      }
      else if (is_psw_correct == 1)
      {
        STATE = State::WAITING;
        previous_STATE = State::BUZZER_ACTIVATED;
        time_change_case = millis();
      }
    }
    break;

    case State::INCORRECT:
    {
      STATE = previous_STATE;
      if (previous_STATE == State::OFFLINE)
      {
        lcd_print("ALARM OFF", 1, "WRONG, RETRY");
      }

      if (previous_STATE == State::BUZZER_ACTIVATED)
      {
        lcd_print("ALARM ON", 1, "WRONG, RETRY");
      }
    }
    break;
  }

}

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
} // LOOP PARENTHESIS


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
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);          
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::BUZZER_ACTIVATED)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, HIGH);          
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);          
    }   

    if (next_STATE == State::OFFLINE &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, HIGH);      
      sensorData = LOW;    
      lcd_print("ALARM OFF");
    }   
    is_psw_correct = 0;      
  }
}

