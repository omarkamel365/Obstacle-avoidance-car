#define F_CPU 16000000UL
#include <avr/io.h>
#include <MrLcd/MrLCDATmega32.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DIO.h"
#include "Bitwise.h"
#include "Timer0.h"

void MoveForward();
void RotateLeft();
void MoveStop();



static volatile int pulse = 0;
static volatile int i = 0;

int main(void)
{
	MCUCSR|= (1<<JTD);
	MCUCSR|= (1<<JTD);
	
	DDRC = 0XFF;

	int16_t count_a = 0;
	char show_a[16];
	Initialise();
	DDRD = 0b11111011;
	_delay_ms(50);

	Initialise();

	SetBit(GICR,INT0);
SetBit(MCUCR,ISC00);

	sei();
	
	while(1)
	{
		
		SetBit(PORTD,PIND0);
		_delay_us(15);

		ClearBit(PORTD,PIND0);
		count_a = pulse/58;

		Send_A_String("Distance Sensor");
		GoToMrLCDLocation(1,2);
		Send_A_String("Distance=");
		itoa(count_a,show_a,10);
		Send_A_String(show_a);
		Send_A_String(" ");
		GoToMrLCDLocation(13,2);
		Send_A_String("cm");
		GoToMrLCDLocation(1,1);
		
		
		MoveForward();
		if(count_a<=10)
		{
			MoveStop();
			do
			{
				RotateLeft();
			} while (count_a<=30);
			MoveForward();
			
		}

		
		
		
	}
}


ISR(INT0_vect)
{
	if(i == 0)
	{
		SetBit(TCCR1B,CS10);
		i = 1;
	}
	else
	{
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}
}

void MoveForward (void)
{
	PORTC = 0x01;
}
void MoveStop(void)
{
	PORTC = 0X00;
}
void RotateLeft(void)
{
	PORTC =0b00001001;
}