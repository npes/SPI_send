/*
 * SPI-MASTER.cpp
 *
 * Created: 1/31/2017 8:57:47 AM
 * Author : kun/nisi
 * 
 * This is the send part of an ATmega328 SPI demonstration program.
 * Connect pins 16,17,18,19,GND to an atmega328 running the receive SPI program 
 * Read the comment in the receive program :-)
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void SPIMasterInit(void);
void SPIMasterSend (uint8_t data);
void delay_ms(uint16_t ms);
void delay_us(uint16_t us);	
	
int main(void)
	{
	//Initialize Master SPI
	SPIMasterInit(); 
		while (1)
		{
			for (int i=0; i<7; i++)
			{
			SPIMasterSend(0x01<<i);
			delay_ms((i==0) ? 1*(i+100) : i*(i+100)); //ternary operator if/else http://www.tutorialspoint.com/ansi_c/ternary_operator_examples.htm
			}
			
			for (int i=7; i>=0; i--)
			{
			SPIMasterSend(0x01<<i);
			delay_ms(i*(i+100));
			}		
		}
	}

// initialize SPI as master	
void SPIMasterInit(void)
{
	//Set MOSI, SCK, SS as output
	DDRB |= (1<<PINB3) | (1<<PINB5) | (1<<PINB2);
	//Set SS to high
	PORTB|= (1<<PINB2);
	//Enable master SPI at clock rate 16mHz/16 = 1mHz
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

//Send a byte of data in parameter
void SPIMasterSend (uint8_t data)
{
	//Slave select, slave is selected when SS pins is low
	PORTB &= ~(1<<PINB2);
	//Send data
	SPDR = data;
	//Wait for transfer to complete
	while (!(SPSR &(1<<SPIF)));
	//SS to high
	PORTB |=(1<<PINB2);
}
	
//alternative delay functions, needed if they are used dynamic
void delay_ms(uint16_t ms) //milliseconds
{
	while(ms--)
	{
		_delay_ms(1);
	}
}
void delay_us(uint16_t us) //microseconds
{
	while(us--)
	{
		_delay_us(1);
	}
}