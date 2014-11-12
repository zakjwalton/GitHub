/*
 * I2C.h
 *
 * Created: 9/26/2014 9:26:02 PM
 *  Author: Jonathan LaFavor and Zak Walton
 */ 

#include	<stdio.h>
#include	<avr/io.h>

#ifndef		I2C_H_
#define		I2C_H_

//I2C Control Macros
#define		F_CPU	16000000UL
#define		F_SCL	100000UL
#define		READ	0x01
#define		WRITE	0x00

//Function that initializes the 2-wire communication interface,
//also known as I2C, on PC5 and PC6
void I2C_Init(void);
//Function that sends TWI Start Bit
void I2C_Start();
//Function that takes in and sends slave address and R/W bit
void I2C_Addr(uint8_t address, uint8_t read_write);
//Function that takes in and sends a data byte to the slave
void I2C_Write(uint8_t data);
//Function that reads and returns a data byte that was
//sent from the slave
uint8_t I2C_Read(void);
//Function that Stops I2C communication
void I2C_Stop(void);
//Function that takes in the slave address, the device memory address that
//you want to write to as well as the data you want to write and sends
//it over I2C to the slave
void I2C_SendMessage(uint8_t slave_addr, uint8_t device_reg, uint8_t data);
//Function that takes in a device memory address that you want
//to read from and reads the data at that address and then
//returns the data byte that was read
uint8_t I2C_RecieveMessage(uint8_t slave_addr, uint8_t device_reg);

#endif