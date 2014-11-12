/*
 * LCD_PCD8544.c
 *
 * Created: 9/26/2014 9:26:31 PM
 *  Author: Jonathan
 */ 

//Includes
#include "LCD_PCD8544.h"
#include <avr/pgmspace.h>
#include <string.h>
#include "Timers.h"

// Array for alarm splash screen
const unsigned char alarm_clock_ringing [] PROGMEM = {
		0xFF, 0x81, 0x55, 0xBF, 0x8F, 0x39, 0x55, 0x83, 0xFF, 0x91, 0x29, 0x4F, 0xEF, 0xED, 0x51, 0x03,
		0xFF, 0x87, 0x53, 0x39, 0x7D, 0x39, 0x03, 0x87, 0xFF, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x9F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0x5F, 0x6F, 0x6F, 0x6F, 0x1F, 0x5F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xDF, 0xDF, 0xBF,
		0xBF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFD, 0xF7, 0xF9, 0xF5, 0xBB, 0xD5, 0x69, 0xD5, 0xEB, 0x75, 0xA9,
		0x55, 0xAB, 0x11, 0xFF, 0xFF, 0x07, 0x57, 0xFF, 0x3F, 0xE7, 0x57, 0x0F, 0xFF, 0x07, 0xA7, 0xBF,
		0xBF, 0x97, 0x87, 0x0F, 0xFF, 0x1F, 0x4F, 0xA7, 0xB7, 0xB7, 0x27, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x9F, 0xD3, 0xDC, 0x1F, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x8F,
		0x47, 0x03, 0x23, 0x21, 0x20, 0x80, 0x40, 0xA0, 0x40, 0x80, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0x03, 0x00, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFB, 0xF6, 0xFB, 0x5E, 0xAB, 0x5E, 0xA4, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0x3E, 0xBC, 0x7C, 0xFE,
		0xBF, 0xFE, 0x3C, 0x7D, 0x3F, 0xFF, 0x3C, 0xBC, 0xBF, 0xFE, 0xBC, 0xDC, 0xFF, 0xFF, 0xFD, 0xFC,
		0xFF, 0xFF, 0xFD, 0xFF, 0xFD, 0x3F, 0x9F, 0x5F, 0x68, 0x08, 0x0B, 0x19, 0x1C, 0x3C, 0x77, 0xE3,
		0x00, 0x2C, 0x15, 0x6C, 0x9C, 0xD4, 0x14, 0x90, 0x84, 0x01, 0x2C, 0xCA, 0xB1, 0xB0, 0xA0, 0x40,
		0x40, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA8, 0x54, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xB6, 0x5B, 0xB5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFC, 0xFE, 0xFD, 0xFF, 0x3C, 0xBF, 0x7C, 0xFE, 0xBC, 0xFF, 0x3C, 0x7D, 0x3C, 0xFF, 0x3A, 0xB6,
		0xBF, 0xFF, 0xBF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE2, 0xDF, 0xBD, 0xF0, 0xE0, 0xC0, 0x00, 0x60,
		0xF0, 0xB8, 0x4E, 0x03, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0xF0, 0xE0, 0x00, 0x51, 0x00, 0x02, 0x01,
		0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xA0, 0x50, 0xA8, 0x55, 0xEA,
		0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDE, 0xAD, 0xD2, 0xFF,
		0xFF, 0x3F, 0x3F, 0xFF, 0xFF, 0xBF, 0x3F, 0x7F, 0xFC, 0xBE, 0x3D, 0x3F, 0xFC, 0xFF, 0xBC, 0x3E,
		0xFC, 0x3F, 0xBC, 0xFD, 0xFC, 0xBF, 0x3A, 0x76, 0xFF, 0x7F, 0x3F, 0xBF, 0xBF, 0x3F, 0xBF, 0x7E,
		0xFF, 0xFE, 0xFC, 0xFC, 0x7F, 0xEA, 0xD5, 0xA0, 0x40, 0x1F, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x8F,
		0x20, 0x14, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x40, 0x80, 0x40, 0xA0, 0xD4, 0xE4, 0xF8, 0xF8,
		0xF1, 0xF1, 0xF1, 0xE3, 0x33, 0x3F, 0x17, 0x9B, 0x01, 0x81, 0x80, 0x00, 0x81, 0x81, 0x81, 0xC3,
		0x73, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0xE5, 0xE9, 0xFD, 0xFD, 0xEA, 0xE0, 0xFF, 0xF0, 0xEA, 0xEF,
		0xEF, 0xEF, 0xEA, 0xF0, 0xFF, 0xF0, 0xEA, 0xED, 0xFD, 0xFD, 0xEA, 0xE0, 0xFF, 0xFF, 0xFE, 0xEB,
		0xE9, 0xFD, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xD7, 0xAF, 0xDE, 0xA6, 0x9C, 0xDD,
		0xFD, 0xDA, 0xFD, 0xFA, 0xFD, 0xFA, 0xFD, 0xFA, 0xFD, 0xFA, 0xFD, 0xFE, 0xFD, 0xFE, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF8, 0xF9, 0xFC, 0xFF, 0xF9,
		0xF9, 0xF9, 0xFD, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF,
};

// Array of ASCII characters in order of ascii conversion value so they can be referenced this way by passing a char into array index
// Small font is
// Array of characters for ASCII values
static const unsigned char smallFont[] PROGMEM =
{
	0x00, 0x00, 0x00, 0x00, 0x00, // sp
	0x00, 0x00, 0x2f, 0x00, 0x00, // !
	0x00, 0x07, 0x00, 0x07, 0x00, // "
	0x14, 0x7f, 0x14, 0x7f, 0x14, // #
	0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
	0xc4, 0xc8, 0x10, 0x26, 0x46, // %
	0x36, 0x49, 0x55, 0x22, 0x50, // &
	0x00, 0x05, 0x03, 0x00, 0x00, // '
	0x00, 0x1c, 0x22, 0x41, 0x00, // (
	0x00, 0x41, 0x22, 0x1c, 0x00, // )
	0x14, 0x08, 0x3E, 0x08, 0x14, // *
	0x08, 0x08, 0x3E, 0x08, 0x08, // +
	0x00, 0x00, 0x50, 0x30, 0x00, // ,
	0x10, 0x10, 0x10, 0x10, 0x10, // -
	0x00, 0x60, 0x60, 0x00, 0x00, // .
	0x20, 0x10, 0x08, 0x04, 0x02, // /
	0x3E, 0x41, 0x41, 0x41, 0x3E, // 0
	0x00, 0x42, 0x7F, 0x40, 0x00, // 1
	0x42, 0x61, 0x51, 0x49, 0x46, // 2
	0x21, 0x41, 0x45, 0x4B, 0x31, // 3
	0x18, 0x14, 0x12, 0x7F, 0x10, // 4
	0x27, 0x45, 0x45, 0x45, 0x39, // 5
	0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
	0x01, 0x71, 0x09, 0x05, 0x03, // 7
	0x36, 0x49, 0x49, 0x49, 0x36, // 8
	0x06, 0x49, 0x49, 0x29, 0x1E, // 9
	0x00, 0x36, 0x36, 0x00, 0x00, // :
	0x00, 0x56, 0x36, 0x00, 0x00, // ;
	0x08, 0x14, 0x22, 0x41, 0x00, // <
	0x14, 0x14, 0x14, 0x14, 0x14, // =
	0x00, 0x41, 0x22, 0x14, 0x08, // >
	0x02, 0x01, 0x51, 0x09, 0x06, // ?
	0x32, 0x49, 0x59, 0x51, 0x3E, // @
	0x7E, 0x11, 0x11, 0x11, 0x7E, // A
	0x7F, 0x49, 0x49, 0x49, 0x36, // B
	0x3E, 0x41, 0x41, 0x41, 0x22, // C
	0x7F, 0x41, 0x41, 0x22, 0x1C, // D
	0x7F, 0x49, 0x49, 0x49, 0x41, // E
	0x7F, 0x09, 0x09, 0x09, 0x01, // F
	0x3E, 0x41, 0x49, 0x49, 0x7A, // G
	0x7F, 0x08, 0x08, 0x08, 0x7F, // H
	0x00, 0x41, 0x7F, 0x41, 0x00, // I
	0x20, 0x40, 0x41, 0x3F, 0x01, // J
	0x7F, 0x08, 0x14, 0x22, 0x41, // K
	0x7F, 0x40, 0x40, 0x40, 0x40, // L
	0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
	0x7F, 0x04, 0x08, 0x10, 0x7F, // N
	0x3E, 0x41, 0x41, 0x41, 0x3E, // O
	0x7F, 0x09, 0x09, 0x09, 0x06, // P
	0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
	0x7F, 0x09, 0x19, 0x29, 0x46, // R
	0x46, 0x49, 0x49, 0x49, 0x31, // S
	0x01, 0x01, 0x7F, 0x01, 0x01, // T
	0x3F, 0x40, 0x40, 0x40, 0x3F, // U
	0x1F, 0x20, 0x40, 0x20, 0x1F, // V
	0x3F, 0x40, 0x38, 0x40, 0x3F, // W
	0x63, 0x14, 0x08, 0x14, 0x63, // X
	0x07, 0x08, 0x70, 0x08, 0x07, // Y
	0x61, 0x51, 0x49, 0x45, 0x43, // Z
	0x00, 0x7F, 0x41, 0x41, 0x00, // [
	0x55, 0x2A, 0x55, 0x2A, 0x55, // 55
	0x00, 0x41, 0x41, 0x7F, 0x00, // ]
	0x04, 0x02, 0x01, 0x02, 0x04, // ^
	0x40, 0x40, 0x40, 0x40, 0x40, // _
	0x00, 0x01, 0x02, 0x04, 0x00, // '
	0x20, 0x54, 0x54, 0x54, 0x78, // a
	0x7F, 0x48, 0x44, 0x44, 0x38, // b
	0x38, 0x44, 0x44, 0x44, 0x20, // c
	0x38, 0x44, 0x44, 0x48, 0x7F, // d
	0x38, 0x54, 0x54, 0x54, 0x18, // e
	0x08, 0x7E, 0x09, 0x01, 0x02, // f
	0x0C, 0x52, 0x52, 0x52, 0x3E, // g
	0x7F, 0x08, 0x04, 0x04, 0x78, // h
	0x00, 0x44, 0x7D, 0x40, 0x00, // i
	0x20, 0x40, 0x44, 0x3D, 0x00, // j
	0x7F, 0x10, 0x28, 0x44, 0x00, // k
	0x00, 0x41, 0x7F, 0x40, 0x00, // l
	0x7C, 0x04, 0x18, 0x04, 0x78, // m
	0x7C, 0x08, 0x04, 0x04, 0x78, // n
	0x38, 0x44, 0x44, 0x44, 0x38, // o
	0x7C, 0x14, 0x14, 0x14, 0x08, // p
	0x08, 0x14, 0x14, 0x18, 0x7C, // q
	0x7C, 0x08, 0x04, 0x04, 0x08, // r
	0x48, 0x54, 0x54, 0x54, 0x20, // s
	0x04, 0x3F, 0x44, 0x40, 0x20, // t
	0x3C, 0x40, 0x40, 0x20, 0x7C, // u
	0x1C, 0x20, 0x40, 0x20, 0x1C, // v
	0x3C, 0x40, 0x30, 0x40, 0x3C, // w
	0x44, 0x28, 0x10, 0x28, 0x44, // x
	0x0C, 0x50, 0x50, 0x50, 0x3C, // y
	0x44, 0x64, 0x54, 0x4C, 0x44, // z
	0x00, 0x06, 0x09, 0x09, 0x06  // º
};

//Array for numbers
const unsigned char single_numbers[] PROGMEM =
{
	0x3E, 0x41, 0x41, 0x41, 0x3E, // 0
	0x00, 0x42, 0x7F, 0x40, 0x00, // 1
	0x42, 0x61, 0x51, 0x49, 0x46, // 2
	0x21, 0x41, 0x45, 0x4B, 0x31, // 3
	0x18, 0x14, 0x12, 0x7F, 0x10, // 4
	0x27, 0x45, 0x45, 0x45, 0x39, // 5
	0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
	0x01, 0x71, 0x09, 0x05, 0x03, // 7
	0x36, 0x49, 0x49, 0x49, 0x36, // 8
	0x06, 0x49, 0x49, 0x29, 0x1E, // 9
};

//Array of 10x16 numbers
// Character bitmaps for Perpetua Titling MT 13pt
static const uint8_t large_font[] PROGMEM =
{
	// @0 '0' (10 pixels wide)
	//
	//   #####
	//  ### ###
	//  ##   ##
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	//  ##   ##
	//  ### ###
	//   #####
	0xF0, 0xFC, 0xFE, 0x06, 0x02, 0x06, 0xFE, 0xFC, 0xF0, 0x00,
	0x07, 0x1F, 0x3F, 0x30, 0x20, 0x30, 0x3F, 0x1F, 0x07, 0x00,

	// @20 '1' (10 pixels wide)
	//
	//     ##
	//   ####
	//  #####
	// #  ###
	//    ###
	//    ###
	//    ###
	//    ###
	//    ###
	//    ###
	//    ###
	//    ###
	// #########
	0x10, 0x08, 0x0C, 0xFC, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x20, 0x20, 0x3F, 0x3F, 0x3F, 0x20, 0x20, 0x20, 0x00,

	// @40 '2' (10 pixels wide)
	//
	//  ######
	// ###  ###
	// ##    ###
	//       ###
	//       ###
	//       ##
	//      ##
	//     ##
	//    ##
	//   ##
	//  ##     #
	// #########
	// #########
	0x0C, 0x0E, 0x06, 0x02, 0x02, 0x86, 0xFE, 0x7C, 0x38, 0x00,
	0x30, 0x38, 0x3C, 0x36, 0x33, 0x31, 0x30, 0x30, 0x38, 0x00,

	// @60 '3' (10 pixels wide)
	//
	//  ######
	// ###  ###
	// ##    ###
	//       ###
	//       ###
	//      ###
	//   #####
	//      ###
	//       ###
	//       ###
	// ##    ###
	// ###  ###
	//  ######
	0x0C, 0x0E, 0x86, 0x82, 0x82, 0xC6, 0xFE, 0x7C, 0x38, 0x00,
	0x18, 0x38, 0x30, 0x20, 0x20, 0x31, 0x3F, 0x1F, 0x0E, 0x00,

	// @80 '4' (10 pixels wide)
	//
	//      ###
	//     ####
	//    # ###
	//    # ###
	//   #  ###
	//  #   ###
	//  #   ###
	// #    ### #
	// ##########
	//      ###
	//      ###
	//      ###
	//     #####
	0x00, 0xC0, 0x20, 0x18, 0x04, 0xFE, 0xFE, 0xFE, 0x00, 0x00,
	0x03, 0x02, 0x02, 0x02, 0x22, 0x3F, 0x3F, 0x3F, 0x22, 0x03,

	// @100 '5' (10 pixels wide)
	//        #
	//  #######
	//  #######
	//  ##
	//  ##
	//  ##
	//  ######
	//  #   ###
	//       ###
	//       ###
	//       ###
	// ##    ###
	// ###  ###
	//  ######
	0x00, 0xFE, 0x7E, 0x46, 0x46, 0xC6, 0xC6, 0x87, 0x00, 0x00,
	0x18, 0x38, 0x30, 0x20, 0x20, 0x30, 0x3F, 0x1F, 0x0F, 0x00,

	// @120 '6' (10 pixels wide)
	//
	//      ###
	//    ###
	//   ###
	//  ###
	//  ###
	// ### ###
	// #### ###
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	//  ### ###
	//   #####
	0xC0, 0xF0, 0xF8, 0xBC, 0x4C, 0xC6, 0xC2, 0x82, 0x00, 0x00,
	0x0F, 0x1F, 0x3F, 0x30, 0x20, 0x30, 0x3F, 0x1F, 0x0F, 0x00,

	// @140 '7' (10 pixels wide)
	//
	// #########
	// #########
	// #      ##
	//       ##
	//       ##
	//      ##
	//      ##
	//     ##
	//     ##
	//    ##
	//    ##
	//   ##
	//   ##
	0x0E, 0x06, 0x06, 0x06, 0x06, 0xC6, 0xF6, 0x3E, 0x0E, 0x00,
	0x00, 0x00, 0x30, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00,

	// @160 '8' (10 pixels wide)
	//
	//   #####
	//  ### ###
	// ###   ###
	// ###   ###
	// ###   ###
	//  ### ###
	//   #####
	//  ### ###
	// ###   ###
	// ###   ###
	// ###   ###
	//  ### ###
	//   #####
	0x38, 0x7C, 0xFE, 0xC6, 0x82, 0xC6, 0xFE, 0x7C, 0x38, 0x00,
	0x0E, 0x1F, 0x3F, 0x31, 0x20, 0x31, 0x3F, 0x1F, 0x0E, 0x00,

	// @180 '9' (10 pixels wide)
	//
	//   #####
	//  ### ###
	// ###   ###
	// ###   ###
	// ###   ###
	// ###   ###
	//  ### ####
	//   ### ###
	//       ###
	//      ###
	//      ###
	//     ###
	//   ###
	0x78, 0xFC, 0xFE, 0x86, 0x02, 0x86, 0xFE, 0xFC, 0xF8, 0x00,
	0x00, 0x00, 0x21, 0x21, 0x31, 0x1C, 0x1F, 0x0F, 0x03, 0x00,

	//space
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	
};

//Print large 5x16 colon
static const uint8_t large_colon[] PROGMEM =
{
	// @200 ':' (10 pixels wide)
	//
	//
	//
	//
	//
	//
	//    ###
	//    ###
	//    ###
	//
	//
	//    ###
	//    ###
	//    ###
	0x00, 0xC0, 0xC0, 0xC0, 0x00,
	0x00, 0x39, 0x39, 0x39, 0x00

};

//Array with menu text
const char menu_text[6][12] =
{
	"Tune","Presets","Set Time","Set Alarm 1","Set Alarm 2", "Back"
};


/*--------------------------------------------------------------------------------------------------
Name : LCD_SPI_initialize
Description : Initializing ATmega328P SPI for use with the Nokia 5110 LCD w/ PCD8544 Controller
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/
void LCD_SPI_initialize()
{
	// Set data direction for LCD output control pins
	LCD_DD_RESET  |= _BV(LCD_RESET_PIN);
	LCD_DD_SCK    |= _BV(LCD_SCK);
	LCD_DD_MOSI   |= _BV(LCD_MOSI);     
	LCD_DD_D_OR_C |= _BV(LCD_D_OR_C);   
	LCD_DD_SS     |= _BV(LCD_SS);    
	
	// Setting SPI clock generator to Fclk/16 and enabling SPI peripheral and setting in master mode
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_initialize
Description : Initialize the LCD Controller Setup parameters
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/
void LCD_initialize()
{
	// Enabling the LCD
	LCD_ENABLE;
	
	// Set the reset pin (active low) (resetting) 
	LCD_RESET;
	
	// Wait for 1 ms
	hw_delay(1);
	
	// Set reset pin (active low) (setting)
	LCD_NRESET;
	
	// Clear the screen
	LCD_clear_screen();
	
	// Disable the LCD 
	LCD_DISABLE;
	
	/*				 Setting the default mode for the LCD
	 *
	 *Default mode: 
	 *				Horizontal addressing (0,0) in upper left
	 *				Display in normal mode
	 *				Temp. Coeff. of 2 (17 mV/K)
	 *				Setting operation voltage to approx. 7V with Vop = 65
	 *				Mux 1:48
	 */
	
	// Set the display to normal mode
	LCD_send_command(0x0C);
	
	// Set the cursor to (0,0)
	LCD_goto(0,0);
	
	// Set LCD in Horizontal addressing mode / Extended instruction mode / Power down disabled mode
	LCD_send_command(0x21);
	
	// Set the temperature coefficient to 2 (17 mV/K)
	LCD_send_command(0x04);
	
	// Set operation voltage to approximately 7V
	LCD_send_command(0xA0);
	
	// Set mux rate 1:48
	//was 0x16
	LCD_send_command(0x16);
	
	// Set back into normal instruction mode
	LCD_send_command(0x20);
	
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_send_command
Description : Sends a command through SPI to Nokia 5110 LCD w/ PCD8544 Controller
Argument(s) : BYTE command_to_send
Return value : None.
--------------------------------------------------------------------------------------------------*/
void LCD_send_command(BYTE command_to_send)
{
	
	// Enable LCD
	LCD_ENABLE;
	
	// Set the LCD in command mode
	LCD_COMMAND_MODE;
	
	// Writing to the SPI data register which auto. shifts out data
	SPDR = command_to_send;
	
	// Wait while the SPIF flag is low
	while(!(SPSR & _BV(SPIF)))
	{
		// Do nothing
	}
	SPSR &= ~_BV(SPIF);
	
		// Disable LCD
	LCD_DISABLE;
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_send_data
Description : Sends data through SPI to Nokia 5110 LCD w/ PCD8544 Controller to write pixels on the screen
Argument(s) : BYTE data_to_send
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_send_data(BYTE data_to_send)
{
	// Enable LCD
	LCD_ENABLE;
	
	// Set Data Mode
	LCD_DATA_MODE;
	
	// Store data to send in SPI data register
	SPDR = data_to_send;
	
	// Wait while the SPIF flag is low
	while(!(SPSR & _BV(SPIF)))
	{
		// Do nothing
	}
	
	// Disable LCD
	LCD_DISABLE;
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_display_splash
Description : Displays the splash screen on the LCD
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_display_splash(void)
{
	int i;
	
	for(i = 0; i < sizeof(alarm_clock_ringing); i++)
	{
		LCD_send_data(pgm_read_byte(&(alarm_clock_ringing[i])));
	}
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_clear_screen
Description : Write a 0 to every pixel on the screen in order to clear the screen
Argument(s) : None.
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_clear_screen(void)
{
	int i;
	
	for(i = 0; i < 504; i++)
	{
		LCD_send_data(0x00);
	}
	
	// Return to home position
	LCD_goto(0,0);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_goto
Description : Moves the address pointer to the specified column and row on the LCD
Argument(s) : 
	column - coumn to go to
	row - row to go to
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_goto(int column, int row)
{
	// Set the x address
	LCD_send_command(0x80 | column);
	
	// Set the y address
	LCD_send_command(0x40 | row);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_set_y
Description : Tells the LCD to point to a specific y location on the screen
Argument(s) : y_to_set - y-position to move to
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_set_y(int y_to_set)
{
	// Set the y address
	LCD_send_command(0x40 | y_to_set);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_char
Description : Prints the specified character to the LCD
Argument(s) : char_to_print
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_char(unsigned char char_to_print)
{
	unsigned char j;
	
	for(j=0; j<5; j++)
	{
		LCD_send_data((pgm_read_byte(&(smallFont [(char_to_print-32)*5 + j])) ));
	}
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_inverted_char
Description : Prints the specified character inverted on the LCD
Argument(s) : char_to_print - character to print inverted
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_inverted_char(unsigned char char_to_print)
{
	unsigned char j;
	
	for(j=0; j<5; j++)
	{
		LCD_send_data(~(pgm_read_byte(&(smallFont [(char_to_print-32)*5 + j])) ));
	}
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_single_number
Description : Prints single number to the LCD
Argument(s) : single_digit_to_print - number to print to the LCD
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_single_number(int single_digit_to_print)
{
	unsigned char j;
	
	for(j=0; j<5; j++)
	{
		LCD_send_data((pgm_read_byte(&(single_numbers [5*single_digit_to_print + j]))));
	}
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_large_number
Description : Prints 10x16 number to the LCD
Argument(s) : 
	number_to_print - large number to print to the LCD
	x - x-position to print the number at
	y - y-position to print the number at
	inverted - (1) print the number inverted (0) print the number normal
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_large_number(unsigned int number_to_print, uint8_t x, uint8_t y, uint8_t inverted)
{
	int i,j;
	
	// Write 41 bytes then a space and increment y and repeat until repeated 6 times
	for(i = 0; i < 2; i++)
	{
		LCD_goto(x,y+i);
		for(j = 0; j < 10; j++)
		{
			if(inverted)
			{
				LCD_send_data(~pgm_read_byte(&(large_font[20*number_to_print + j + (i*10)])));
			}
			else
			{
				LCD_send_data(pgm_read_byte(&(large_font[20*number_to_print + j + (i*10)])));
			}
			
		}
	}
	LCD_set_y(y);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_double_number
Description : Prints a double number to the screen (2 digits 0-9, not type double)
Argument(s) : number_to_print - number from 0-99 to print
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_double_number(int number_to_print)
{
	unsigned char j;
	int temp;
	
	temp = number_to_print / 10;
	
	for(j=0; j<5; j++)
	{
		LCD_send_data((pgm_read_byte(&(single_numbers [5*temp + j]))));
	}
	
	temp = number_to_print - temp*10;
	
	for(j=0; j<5; j++)
	{
		LCD_send_data((pgm_read_byte(&(single_numbers [5*temp + j]))));
	}
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_largedouble_number
Description : Prints large double number on LCD (number 0-99)
Argument(s) : 
	number_to_print - Large double number to print 0-99
	x - x-position of number
	y - y-position of number
	hours - (1) printing hours (0) printing minutes
	inverted - (1) print number inverted (0) print number normal
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_largedouble_number(int number_to_print, int x, int y, uint8_t hours, uint8_t inverted)
{
	int temp1;
	int temp2;
	
	temp1 = number_to_print / 10;
	temp2 = number_to_print % 10;
	//print hours if less than 10
	if((temp1 == 0) && hours)
	{
		LCD_goto(x,y);
		//print space first if number is less than 10
		LCD_print_large_number(10, x, y, NOTINVERTED);
		if(inverted)
		{
			LCD_goto(x,y);
			LCD_print_large_number(temp2, x+10,y, INVERTED);
		} 
		else
		{
			LCD_goto(x,y);
			LCD_print_large_number(temp2, x+10,y, NOTINVERTED);
		}
	}
	//print two numbers (number > 9)
	else
	{
		if(inverted)
		{
			LCD_goto(x,y);
			LCD_print_large_number(temp1, x, y, INVERTED);
			LCD_goto(x,y);
			LCD_print_large_number(temp2, x+10,y, INVERTED);
		}
		else
		{
			LCD_goto(x,y);
			LCD_print_large_number(temp1, x, y, NOTINVERTED);
			LCD_goto(x,y);
			LCD_print_large_number(temp2, x+10,y, NOTINVERTED);
		}
		
	}
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_large_colon
Description : Print a 5x16 colon to the screen for large time display
Argument(s) : 
	x - x-position of colon
	y - y-position of colon
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_large_colon(uint8_t x, uint8_t y){
	int i,j;
	
	// Write 41 bytes then a space and increment y and repeat until repeated 6 times
	for(i = 0; i < 2; i++)
	{
		LCD_goto(x,y+i);
		for(j = 0; j < 10; j++)
		{
			LCD_send_data(pgm_read_byte(&(large_colon[j + (i*5)])));
		}
	}
	LCD_set_y(y);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_string
Description : Prints a string to the LCD
Argument(s) : 
	*string - pointer to string to print
	inverted - (1) print inverted string (0) print normal string
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_string(const char *string, BYTE inverted)
{
	// Prints character of current index of string until string ends
	if(inverted){
		while ( *string )
		{
			// Prints character of current index of string
			LCD_print_inverted_char( *string++ );
		}
	}
	else{
		while ( *string )
		{
			// Prints character of current index of string
			LCD_print_char( *string++ );
		}
	}
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_bottom_menu
Description : Print 3 labels on the bottom row of the screen for button labels
Argument(s) : 
	*button1 - pointer to button1 label
	*button2 - pointer to button2 label
	*button3 - pointer to button3 label
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_bottom_menu(const char *button1, const char *button2, const char *button3)
{
	int tot_string_length, temp_calc, max_space, i;
	
	//Finding total number of characters
	tot_string_length = strlen(button1) + strlen(button2) + strlen(button3);
	
	temp_calc = 16 - (tot_string_length);
	
	max_space = temp_calc / 2;
	
	// Set cursor to bottom left
	LCD_goto(0,5);
	
	// print first button command heading
	LCD_print_string(button1,0);
	
	for(i = 0; i < max_space; i++)
	{
		// print a space after the first command
		LCD_print_char(' ');
	}
	
	// print second button command heading
	LCD_print_string(button2,0);
	
	for(i = 0; i < max_space; i++)
	{
		// print a space after the first command
		LCD_print_char(' ');
	}
	// print third button command heading
	LCD_print_string(button3,0);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_signal_strength
Description : Prints the signal strength using a bar indicator
Argument(s) : signal_strength - integer representation of signal strength
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_signal_strength(int signal_strength)
{
	int i;
	LCD_goto(64,0);
	LCD_send_data(0xFF);
	
	// Printing the full bar strength
	for(i = 0; i < signal_strength; i++)
	{
		LCD_send_data(0xBD);
	}
	
	for(i = signal_strength; i < 18; i++)
	{
		LCD_send_data(0x81);
	}
	LCD_send_data(0xFF);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_station
Description : Prints the specified station to the screen
Argument(s) : station - station to print
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_station(double station)
{
	int temp_calc;
	double left_over;
	LCD_goto(0,0);
	
	temp_calc = station / 100;
	LCD_print_single_number(temp_calc);
	left_over = station - (100*temp_calc);
	
	temp_calc = left_over / 10;
	LCD_print_single_number(temp_calc);
	left_over = left_over - (10*temp_calc);
	
	temp_calc = left_over / 1;
	LCD_print_single_number(temp_calc);
	left_over = left_over - (1*temp_calc);
	
	LCD_print_char('.');
	
	LCD_print_single_number(left_over*10);
}
	
/*--------------------------------------------------------------------------------------------------
Name : LCD_print_stereo_indicator
Description : Prints S for stereo and NS for no stereo
Argument(s) : is_stereo - (1) stereo (0) no stereo
Return value : None.
--------------------------------------------------------------------------------------------------*/	
	
void LCD_print_stereo_indicator(int is_stereo)
{
	if(is_stereo)
	{
		LCD_goto(79,1);
		LCD_print_char('S');
	}
	else
	{
		LCD_goto(74, 1);
		LCD_print_char('N');
		LCD_print_char('S');
	}
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_horizontal_scroll_string
Description : Function that scrolls a string across the screen
Argument(s) : 
	scroll_speed - speed to scroll at
	string[] - string to scroll across screen
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_horizontal_scroll_string(int scroll_speed, char string[])
{
	int i = 0;
	
	
	LCD_goto(0,3);
	
	// Continuous scrolling loop horizontally across the screen
	while((string[i]) && (i < 16))
	{
		LCD_print_char(string[i]);
		i++;
	}
	
	LCD_shift_array(string); 
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_shift_array
Description : Shifts an array to the left and puts the cutoff part on the end
Argument(s) : array_to_shift[] - array to be shifted
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_shift_array(char array_to_shift[])
{
	int string_length;
	char temp_var;
	int i;
	string_length = strlen(array_to_shift);
	
	if(string_length < 16)
	{
		strncat(array_to_shift,"                 ", 16-string_length);
		string_length = strlen(array_to_shift);
	}
	
	temp_var = array_to_shift[0];
	for(i = 0; i < string_length - 1; i++)
	{
		array_to_shift[i] = array_to_shift[i+1];
	}
	array_to_shift[string_length - 1] = temp_var;
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_time_display
Description : Print main time display to the LCD
Argument(s) : 
	current_time - structure containing current time
	current_temperature - current temp from thermistor
	*button1 - pointer to string for button 1 label
	*button2 - pointer to string for button 2 label
	*a1 - pointer to string for alarm1 on/off
	*a2 - pointer to string for alarm2 on/off
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_time_display(time_t current_time, int current_temperature, char *button1, char *button2, char *a1, char *a2 )
{
	// Example of Time display
	// Printing time portion
	LCD_goto(0,0);
	LCD_print_double_number(current_time.hour);
	LCD_print_char(':');
	LCD_print_double_number(current_time.minute);
	LCD_print_char(':');
	LCD_print_double_number(current_time.second);
	LCD_print_char(' ');
	if(current_time.AM_PM)
	{
		LCD_print_string("PM",0);
	}
	else
	{
		LCD_print_string("AM",0);
	}
	
	// Printing date portion
	LCD_goto(0,2);
	LCD_print_double_number(current_time.month);
	LCD_print_char('/');
	LCD_print_double_number(current_time.day);
	LCD_print_char('/');
	LCD_print_double_number(current_time.year);
	
	// Printing Temperature portion
	LCD_goto(64,1);
	LCD_print_double_number(current_temperature);
	LCD_print_string(" C",0);
	
	// Printing bottom menu
	LCD_goto(65,4);
	LCD_print_string(a1,0);
	LCD_print_bottom_menu(button1, button2, a2);
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_alarm_display
Description : Prints alarm setting display with the selected parameter inverted
Argument(s) : 
	alarmtime - structure holding alarm information
	selection - parameter selected
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_alarm_display(alarm_t alarmtime, uint8_t selection)
{
	int i;
	
	LCD_clear_screen();
	LCD_goto(0,0);
	if(alarmtime.alarm_number == 1)
	{
		LCD_print_string("A1:",0);
	}
	else
	{
		LCD_print_string("A2:",0);
	}
	
	switch (selection)
	{
		case (MINUTES):
			LCD_print_largedouble_number(alarmtime.hour, 18,1,HOURS, NOTINVERTED);
			LCD_print_large_colon(38,1);
			LCD_print_largedouble_number(alarmtime.minute, 43, 1, MINUTES, INVERTED);
			LCD_print_string(" ",0);
			LCD_set_y(2);
			if(alarmtime.AM_PM){
				LCD_print_string("PM",0);
			}
			else{
				LCD_print_string("AM",0);
			}
			break;
		case (HOURS):
			LCD_print_largedouble_number(alarmtime.hour, 18,1,HOURS, INVERTED);
			LCD_print_large_colon(38,1);
			LCD_print_largedouble_number(alarmtime.minute, 43, 1, MINUTES, NOTINVERTED);
			LCD_print_string(" ",0);
			LCD_set_y(2);
			if(alarmtime.AM_PM){
				LCD_print_string("PM",NOTINVERTED);
			}
			else{
				LCD_print_string("AM",NOTINVERTED);
			}
			break;
		case (AMPM):
			LCD_print_largedouble_number(alarmtime.hour, 18,1,HOURS, NOTINVERTED);
			LCD_print_large_colon(38,1);
			LCD_print_largedouble_number(alarmtime.minute, 43, 1, MINUTES, NOTINVERTED);
			LCD_print_string(" ",0);
			LCD_set_y(1);
			for(i=0;i<12;i++){
				LCD_send_data(0x80);
			}
			LCD_goto(68,2);
			if(alarmtime.AM_PM){
				LCD_send_data(0xFF);
				LCD_print_string("PM",INVERTED);
				LCD_send_data(0xFF);
			}
			else{
				LCD_send_data(0xFF);
				LCD_print_string("AM",INVERTED);
				LCD_send_data(0xFF);
			}
			break;
	}
	
	LCD_print_bottom_menu("Down", "Set", "Up");
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_menu
Description : Prints main menu with the current selection inverted
Argument(s) : selection - selected state 0-5
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_menu(uint8_t selection)
{
	LCD_goto(31,0);
	LCD_print_string(menu_text[0],selection == 0);
	LCD_goto(23,1);
	LCD_print_string(menu_text[1],selection == 1);
	LCD_goto(21,2);
	LCD_print_string(menu_text[2],selection == 2);
	LCD_goto(13,3);
	LCD_print_string(menu_text[3],selection == 3);
	LCD_goto(13,4);
	LCD_print_string(menu_text[4],selection == 4);
	LCD_goto(31,5);
	LCD_print_string(menu_text[5],selection == 5);
	
}

/*--------------------------------------------------------------------------------------------------
Name : LCD_print_radio_display
Description : Prints the radio display on the screen
Argument(s) : 
	station - station current playing
	signal_strength - value of stereo signal strength
	is_stereo - whether or not we have stereo reception
	*button1 - button 1 label
	*button2 - button 2 label
	*button2 - button3 label
	*RBDS - RBDS string
Return value : None.
--------------------------------------------------------------------------------------------------*/

void LCD_print_radio_display(double station, int signal_strength, int is_stereo, char *button1, char *button2, char *button3, char *RBDS )
{
	// Printing the station
	LCD_print_station(station);
	
	// Printing the signal strength
	LCD_print_signal_strength(signal_strength);
	
	// Printing the stereo Indicator
	LCD_print_stereo_indicator(is_stereo);
	
	//
	LCD_horizontal_scroll_string(1,RBDS);
	
	// Printing bottom menu
	LCD_print_bottom_menu(button1, button2, button3);
	
}