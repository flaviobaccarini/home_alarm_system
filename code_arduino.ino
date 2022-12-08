#include <Keypad.h>             // library for keyboard
#include<LiquidCrystal.h>
#include <string.h> 

#define MAX_PASSWORD_LENGTH 20
#define STRING_TERMINATOR '\0'

class Password {
public:
	Password(char* pass);
	
	void set(char* pass);
	bool is(char* pass);
	bool append(char character);
	void reset();
	bool evaluate();
	
	//char* getPassword();
	//char* getGuess();
	
	//operators
	Password &operator=(char* pass);
	bool operator==(char* pass);
	bool operator!=(char* pass);
	Password &operator<<(char character);
	
private:
	char* target;
	char guess[ MAX_PASSWORD_LENGTH ];
	byte currentIndex;
};



//construct object in memory, set all variables
Password::Password(char* pass){
	set( pass );
	reset();
}

//set the password
void Password::set(char* pass){
	target = pass;
}

//evaluate a string, is it equal to the password?
bool Password::is(char* pass){ 
	byte i=0;
	while (*pass && i<MAX_PASSWORD_LENGTH){
		guess[i] = pass[i];
		i++;
	}
	return evaluate();
}

//append a char to the guessed password
bool Password::append(char character){ 
	if (currentIndex+1==MAX_PASSWORD_LENGTH){
		return false;
	}else{
		guess[currentIndex++] = character;
		guess[currentIndex] = STRING_TERMINATOR; //ensure a valid c string
	}
	return true;
}

//reset the guessed password, one can guess again
void Password::reset(){ 
	currentIndex = 0;
	guess[currentIndex] = STRING_TERMINATOR;
}

//is the current guessed password equal to the target password?
bool Password::evaluate(){ 
	char pass = target[0];
	char guessed = guess[0];
	for (byte i=1; i<MAX_PASSWORD_LENGTH; i++){
		
		//check if guessed char is equal to the password char
		if (pass==STRING_TERMINATOR && guessed==STRING_TERMINATOR){
			return true; //both strings ended and all previous characters are equal 
		}else if (pass!=guessed || pass==STRING_TERMINATOR || guessed==STRING_TERMINATOR){
			return false; //difference OR end of string has been reached
		}
		
		//read next char
		pass = target[i];
		guessed = guess[i];
	}
	return false; //a 'true' condition has not been met
}

//set password using operator =
Password &Password::operator=(char* pass){
	set( pass );
	return *this;
}

//test password using ==
bool Password::operator==(char* pass){
	return is( pass );
}

//test password using !=
bool Password::operator!=(char* pass){
	return !is( pass );
}

//append to currently guessed password using operator <<
Password &Password::operator<<(char character){
	append( character );
	return *this;
}

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
bool siren_activated = 0;
int ledBlink;
bool blinking = 0;
bool sistem = 0; // 0 for off, 1 for on 
double time_wrong_psw = 10000000000.;
double actual_time;

LiquidCrystal lcd (0,1,2,3,4,5);

void setup()
  {
  keypad.addEventListener(keypadEvent);
  //Serial.begin(9600);  //Used for troubleshooting
  pinMode(sensorz, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT); 
  //Serial.println("sistem startup"); //Used for troubleshooting
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
  keypad.getKey();

  if (sistem == 0)
  {
    // alarm is off
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, HIGH);
    // Serial.println("System is OFF !"); // Used for troubleshooting
  }
  else
  {
    // alarm is on
    actual_time = millis();
    if(actual_time - time_wrong_psw > 10000)
    {
      time_wrong_psw = 10000000000.;
      clean_line_lcd(0, 1, "SIREN OFF");
    }
    if (blinking == 0)
      {
        blinking = 1;
        blink_func(yellowLed, 500, 5);
      }
    sensorData = digitalRead(sensorz);
    if(sensorData == HIGH)
    {
      if (siren_activated == 0)
      {
      siren_activated = 1;
      //Serial.println("Sensor activated, siren activated");
      lcd_print("ALARM ON", 1, "SIREN ON");
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, HIGH);
      }
    }
    if (siren_activated == 1)
      {
        keypad.getKey();
        for(double x = 0; x < 0.92; x += 0.01)
          {  // Elegant Alarm Tone
            tone(buzzer, sinh(x+8.294), 10);
            delay(1);
          }   
      } 
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
        checkPassword();
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


void checkPassword(){
  if (password.evaluate()){
    //Serial.println("Success"); //Used for troubleshooting
    lcd_print("SUCCESS");
    password.reset();
    if (sistem == 0)
    {  
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, LOW);
      //Serial.println("Alarm system"); //Used for troubleshooting
      blink_func(greenLed, 100, 5);
      sistem = 1;
      lcd_print("ALARM ON", 1, "SIREN OFF");
      digitalWrite(greenLed, HIGH);
    }
    else
    {
      noTone(buzzer);
      blink_func(yellowLed, 100, 5);
      lcd.print("ALARM OFF");
      sistem = 0;
      siren_activated = 0;
      blinking = 0;
    }
  }
  else{
    if(sistem == 0)
    {
      //Serial.println("Wrong"); //Used for troubleshooting
      lcd_print("ALARM OFF", 1, "WRONG, RETRY");
      blink_func(redLed, 100, 5);
      password.reset();
    }
    else
    {
      lcd_print("ALARM ON", 1, "WRONG, RETRY");
      time_wrong_psw = millis();
      blink_func(redLed, 100, 5);
      password.reset();
    }
  }
}

void blink_func(char Led, int delay_time, int nr_times)
{
  for(int i = 0; i != nr_times; i++)
  {
    digitalWrite(Led, HIGH);
    delay(delay_time);
    digitalWrite(Led, LOW);
    delay(delay_time);
  }
}

void clean_line_lcd(int col, int row, String string_to_write)
{
  lcd.setCursor(col, row);
  lcd.print("                             ");
  lcd.setCursor(col, row);
  lcd.print(string_to_write);
}

