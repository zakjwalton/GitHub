/*
 * LCD_PCD8544.h
 *
 * Created: 9/26/2014 9:26:02 PM
 *  Author: Jonathan
 */ 
#include <stdio.h>
#include <avr/io.h>
#include "RTC.h"

#ifndef LCD_PCD8544_H_
#define LCD_PCD8544_H_

// LCD signals and pins
#define LCD_RESET_PIN PB0 // Reset pin
#define LCD_SCK       PB5 // Serial Clock Pin
#define LCD_MOSI      PB3 // Master Out Slave In pin
#define LCD_D_OR_C    PD4 // Data or Command pin
#define LCD_SS        PB2 // Slave Select pin

// LCD signals ports
#define LCD_RESET_PORT	   PORTB // Reset pin
#define LCD_SCK_PORT       PORTB // Serial Clock Pin
#define LCD_MOSI_PORT      PORTB // Master Out Slave In pin
#define LCD_D_OR_C_PORT    PORTD // Data or Command pin
#define LCD_SS_PORT        PORTB // Slave Select pin


// LCD pin data direction registers
#define LCD_DD_RESET   DDRB // Reset pin
#define LCD_DD_SCK     DDRB // Serial Clock pin
#define LCD_DD_MOSI    DDRB // Master Out Slave In pin
#define LCD_DD_D_OR_C  DDRD // Data or Command pin
#define LCD_DD_SS      DDRB // Slave Select pin

// LCD inline commands
#define LCD_RESET         LCD_RESET_PORT  &= ~_BV(LCD_RESET_PIN)
#define LCD_NRESET        LCD_RESET_PORT  |=  _BV(LCD_RESET_PIN)
#define LCD_DATA_MODE     LCD_D_OR_C_PORT |=  _BV(LCD_D_OR_C)
#define LCD_COMMAND_MODE  LCD_D_OR_C_PORT &= ~_BV(LCD_D_OR_C)
#define LCD_DISABLE       LCD_SS_PORT     |=  _BV(LCD_SS)
#define LCD_ENABLE        LCD_SS_PORT     &= ~_BV(LCD_SS)

//defines for large number display
#define INVERTED 1
#define NOTINVERTED 0
#define MINUTES 0
#define HOURS 1
#define AMPM 2
#define MUSIC_BELL 3

// Defining the BYTE
typedef uint8_t BYTE;

//Function that initializes SPI to talk the the LCD
void LCD_SPI_initialize();
//Function that sends initialization parameters to the LCD
void LCD_initialize();
//Function used to send a command to the LCD
void LCD_send_command(BYTE command_to_send);
//Function that sends data (or pixels) to write to the LCD
void LCD_send_data(BYTE data_to_send);
//Function that displays the splash screen on the LCD
void LCD_display_splash();
//Function that clears the LCD screen
void LCD_clear_screen();
//Function that goes to a specified x and y position on the LCD
void LCD_goto(int column, int row);
//Function that prints a character to the LCD
void LCD_print_char(unsigned char char_to_print);
//Function that prints an inverted character to the LCD
void LCD_print_inverted_char(unsigned char char_to_print);
//Function that prints a string to the LCD
void LCD_print_string(const char *string, BYTE inverted);
//Function that prints a large number on the screen 10x16
void LCD_print_large_number(unsigned int number_to_print, uint8_t x, uint8_t y, uint8_t inverted);
//Function that sets the y position
void LCD_set_y(int y_to_set);
//Function that prints 3 strings on the bottom menu
void LCD_print_bottom_menu(const char *button1, const char *button2, const char *button3);
//Function that prints the stereo signal strength
void LCD_print_signal_strength(int signal_strength);
//Function that prints the radio station frequency
void LCD_print_station(double station);
//Function that prints a single number to the LCD
void LCD_print_single_number(int single_digit_to_print);
//Function that prints a double number to the LCD
void LCD_print_double_number(int number_to_print);
//Function that prints a large double number to the LCD
void LCD_print_largedouble_number(int number_to_print, int x, int y, uint8_t hours, uint8_t inverted);
//Function that prints the stereo indication to the LCD
void LCD_print_stereo_indicator(int is_stereo);
//Function that displays a horizontal string on the LCD
void LCD_horizontal_scroll_string(int scroll_speed, char *string_to_scroll);
//Function used to shift an array
void LCD_shift_array(char array_to_shift[]);
//Function that prints the time display on the screen
void LCD_print_time_display(time_t current_time, int current_temperature, char *button1, char *button2, char *a1, char *a2 );
//Function that prints the radio display
void LCD_print_radio_display(double station, int signal_strength, int stereo, char *button1, char *button2, char *button3, char *RBDS);
//Function that prints the alarm setting display on the LCD
void LCD_print_alarm_display(alarm_t alarmtime,uint8_t selection);
//Function that prints the menu screen with the current selection inverted
void LCD_print_menu(uint8_t selection);
#endif /* LCD_PCD8544_H_ */