/*
 * I2C.c
 *
 * Created: 9/26/2014 9:26:31 PM
 *  Author: Jonathan LaFavor and Zak Walton
 */ 

#include "I2C.h"
#include "RTC.h"

/*************************************************
Function that initializes the 2-wire communication interface,
also known as I2C, on PC5 and PC6
*************************************************/

void I2C_Init(void){
	
	//set PC5 and PC6 as output pins
	DDRC |= _BV(PC5) | _BV(PC6);
	
	//Initialize I2C bit rate to 100kHz with pre-scalar of 0
	TWSR = 0;
	TWBR = ((F_CPU/F_SCL)-16)/2;
	
}

/*************************************************
Function that sends TWI Start Bit
*************************************************/

void I2C_Start(){
	//send Start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	//Wait for TWINT flag
	while(!(TWCR & _BV(TWINT)));
}

/*************************************************
Function that takes in and sends slave address and R/W bit
*************************************************/

void I2C_Addr(uint8_t address, uint8_t read_write){
	//Put SLA+R/W byte into TWDR
	TWDR = address << 1 | read_write;
	//Send Message
	TWCR = _BV(TWINT) | _BV(TWEN);
	//Wait for ack
	while(!(TWCR & _BV(TWINT)));
}

/*************************************************
Function that takes in and sends a data byte to the slave
*************************************************/

void I2C_Write(uint8_t data){
	//put data to be written in TWDR
	TWDR = data;
	//Write data to slave
	TWCR = _BV(TWINT) | _BV(TWEN);
	//wait for ack
	while(!(TWCR &_BV(TWINT)));
}

/*************************************************
Function that reads and returns a data byte that was
sent from the slave
*************************************************/

uint8_t I2C_Read(void){
	//Enable nack
	TWCR = _BV(TWINT) | _BV(TWEN);
	//wait for TWINT to indicate BYTE received from slave
	while(!(TWCR & _BV(TWINT)));
	//return byte
	return (TWDR);
}

/*************************************************
Function that Stops I2C communication
*************************************************/

void I2C_Stop(void){
	//Send stop condition
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

/*************************************************
Function that takes in the slave address, the device memory address that
you want to write to as well as the data you want to write and sends
it over I2C to the slave
*************************************************/

void I2C_SendMessage(uint8_t slave_addr, uint8_t device_reg, uint8_t data){
	//start communication
	I2C_Start();
	//address slave in write mode
	I2C_Addr(RTC_ADDRESS, WRITE);
	//specify device register to put data in
	I2C_Write(device_reg);
	//send data byte
	I2C_Write(data);
	//stop communication
	I2C_Stop();
}

/*************************************************
Function that takes in a device memory address that you want
to read from and reads the data at that address and then
returns the data byte that was read
*************************************************/

uint8_t I2C_RecieveMessage(uint8_t slave_addr, uint8_t device_reg){
	//variable to store received data in
	uint8_t read_data;
	//start communication
	I2C_Start();
	//address slave in write mode
	I2C_Addr(RTC_ADDRESS, WRITE);
	//specify device register to read from
	I2C_Write(device_reg);
	//start another communication
	I2C_Start();
	//address slave in read mode
	I2C_Addr(RTC_ADDRESS, READ);
	//read and store data from slave
	read_data = I2C_Read();
	//stop communication
	I2C_Stop();
	//return byte
	return (read_data);
}