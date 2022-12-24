#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void keypadEvent(KeypadEvent eKey);
char checkPassword();
void blink_func(unsigned long current,
                unsigned long delay_time,
                unsigned long previous,
                int ledPin);
void lcd_print(String first_row, bool is_there_second_row = 0, String second_row = "");
void clean_line_lcd(int col, int row, String string_to_write);
void handle_waiting(unsigned long blink_time, 
                    int pinLed,
                    unsigned long interval_to_wait,
                    State next_STATE);
void digital_write_three_leds(int redState, int yellowState, int greenState);

#endif