#include <String.h>
#include <Password.h>
#include <LiquidCrystal.h>
#include "definitions.h"
#include "states.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
char checkPassword();
void clean_line_lcd(int col, int row, String string_to_write);
void lcd_print(String first_row, bool is_there_second_row = 0, String second_row = "");
void digital_three_led_pins(bool redState, bool yellowState, bool greenState);

void blink_func(unsigned long current,
                unsigned long delay_time,
                unsigned long previous,
                int ledPin);

#endif