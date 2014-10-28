/*
 * LCD_PCD8544.h
 *
 * Created: 9/26/2014 9:26:02 PM
 *  Author: Jonathan
 */ 
#include <stdio.h>
#include <avr/io.h>

#ifndef LCD_PCD8544_H_
#define LCD_PCD8544_H_

#ifndef TIME_T
#define TIME_T
typedef struct time time_t;
struct time{
	uint8_t year,
	month,
	day,
	day_of_week,
	hour,
	minute,
	second,
	AM_PM;  // AM = 0 PM = 1
};
#endif

typedef struct{
	int previous_state,
	current_state,
	next_state;
}state_t;


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

// Defining the BYTE
typedef uint8_t BYTE;

void LCD_SPI_initialize();
void LCD_initialize();
void LCD_send_command(BYTE command_to_send);
void LCD_send_data(BYTE data_to_send);
void LCD_display_splash();
void LCD_clear_screen();
void delay(int ms);
void one_ms_delay();
void LCD_goto(int column, int row);
void timer0_initialize();
void LCD_print_char(unsigned char char_to_print);
void LCD_print_string(const char *string);
void LCD_print_test();
void LCD_print_large_number(unsigned int number_to_print, int x_offset);
void LCD_set_y(int y_to_set);
void LCD_print_bottom_menu(const char *button1, const char *button2, const char *button3);
void LCD_print_signal_strength(int signal_strength);
void LCD_print_station(double station);
void LCD_print_single_number(int single_digit_to_print);
void LCD_print_double_number(int number_to_print);
void LCD_print_largedouble_number(int number_to_print, int x, int y);
void LCD_print_stereo_indicator(int is_stereo);
void LCD_horizontal_scroll_string(int scroll_speed, char *string_to_scroll);
void LCD_shift_array(char array_to_shift[]);
void LCD_print_time_display(time_t current_time, int current_temperature, char *button1, char *button2, char *button3 );
void LCD_print_radio_display(double station, int signal_strength, int stereo, char *button1, char *button2, char *button3, char *RBDS);
void LCD_print_alarm_display(time_t alarmtime, const char *button1, const char *button2, const char *button3);
#endif /* LCD_PCD8544_H_ */