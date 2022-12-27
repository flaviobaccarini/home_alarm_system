#include <Adafruit_Keypad.h>             
#include <LiquidCrystal.h>
#include <string.h> 
#include <Password.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "functions.h"
#include "customkeypad.h"
#include "wifi_http_request.h"
#include "definitions.h"
#include "states.h"

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/alarm_on/with/key/cXtsDENM9GH34LAdr78CzSHUpjrfQDEzmjdXFcJryeq"; // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=57&value2=25";
Wifi_http_request client = Wifi_http_request(HOST_NAME, PATH_NAME, queryString, blueLed_for_wifi);

extern Password password = Password("1234");  // password
extern LiquidCrystal lcd (rs,en,d4,d3,d2,d1);

// Create a variable to store the current time
extern unsigned long currentMillis = 100; // for blink without delay funct
// Create a variable to store the previous time the LED was updated
extern unsigned long previousMillis = 0; // for blink without delay funct
unsigned long time_change_case;
unsigned long time_buzzer; // for buzzer without delay function
unsigned long time_buzzer_prev = 0; // previous time for buzzer without delay

byte rowPins[4] = {r1, r2, r3, r4}; //connect to the row pinouts of the keypad
byte colPins[4] = {c1, c2, c3, c4}; //connect to the column pinouts of the keypad
CustomKeypad customKeypad = CustomKeypad(rowPins, colPins); 

int sensorData = 0;
State STATE = State::OFFLINE;
State previous_STATE;

void setup()
  {
    client.begin();
    customKeypad.begin();
    //Serial.begin(9600);  //Used for troubleshooting
    pinMode(sensorz, INPUT);
    pinMode(redLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(buzzer, OUTPUT); 
    pinMode(blueLed_for_wifi, OUTPUT);
    //Serial.println("sistem startup"); //Used for troubleshooting
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("ALARM OFF");
    client.connection();
  }


void loop()
{
  customKeypad.tick();
  switch(STATE)
  {
    case State::OFFLINE:
      { 
        if(client.client_connection == 0)
        {
          client.connection();
        }
        digital_three_led_pins(LOW, HIGH, LOW);
        customKeypad.keypad_reading();

        if (customKeypad.is_psw_correct == 2)
        {
          STATE = State::WAITING;
          //next_STATE = State::PIR_ACTIVATED;
          previous_STATE = State::OFFLINE;
          time_change_case = millis();
          lcd_print("ACTIVATION");
          
        }
        else if (customKeypad.is_psw_correct == 1)
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
                           customKeypad.is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       5000, State::PIR_ACTIVATED);
      }

      if(previous_STATE == State::OFFLINE && 
                           customKeypad.is_psw_correct == 1)
      {
        handle_waiting(100, redLed,
                       1000, State::INCORRECT);
      }

      if(previous_STATE == State::BUZZER_ACTIVATED &&
                           customKeypad.is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       5000, State::OFFLINE);
      }

      if(previous_STATE == State::BUZZER_ACTIVATED &&
                           customKeypad.is_psw_correct == 1)
      {
        handle_waiting(100, redLed,
                       1000, State::INCORRECT);
      }

      if(previous_STATE == State::PIR_ACTIVATED &&
                           customKeypad.is_psw_correct == 2)
      {
        handle_waiting(100, greenLed,
                       2000, State::OFFLINE);
      }

      if(previous_STATE == State::PIR_ACTIVATED &&
                           customKeypad.is_psw_correct == 1)
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
        digital_three_led_pins(HIGH, HIGH, HIGH);
      }
      else
      {
        customKeypad.keypad_reading();
        if (customKeypad.is_psw_correct == 2)
        {
          previous_STATE = State::PIR_ACTIVATED;
          time_change_case = millis();
          lcd_print("DEACTIVATION");
          STATE = State::WAITING;
        }
        else if (customKeypad.is_psw_correct == 1)
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
      customKeypad.keypad_reading();
      
      for(double x = 0; x < 0.92; x += 0.01)
        {  // Elegant Alarm Tone
          time_buzzer = millis();
          if (time_buzzer - time_buzzer_prev >= 1)
          {
            time_buzzer_prev = time_buzzer;
            tone(buzzer, 1000, 10);
          }
        }  
        
        if (client.client_connection)
        {
          client.send_http_request();
          client.stop();
        }
      if (customKeypad.is_psw_correct == 2)
      {
        STATE = State::WAITING;
        previous_STATE = State::BUZZER_ACTIVATED;
        time_change_case = millis();     
        lcd_print("ALARM OFF");
      }
      else if (customKeypad.is_psw_correct == 1)
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
      digital_three_led_pins(LOW, LOW, HIGH);    
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::BUZZER_ACTIVATED)
    {
      digital_three_led_pins(HIGH, HIGH, HIGH);         
    }    

    if (next_STATE == State::INCORRECT &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digital_three_led_pins(LOW, LOW, HIGH);             
    }   

    if (next_STATE == State::OFFLINE &&
        previous_STATE == State::PIR_ACTIVATED)
    {
      digital_three_led_pins(LOW, HIGH, LOW);      
      sensorData = LOW;    
      lcd_print("ALARM OFF");
    }   
    customKeypad.is_psw_correct = 0;      
  }
}


