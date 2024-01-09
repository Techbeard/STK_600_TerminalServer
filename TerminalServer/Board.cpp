/*
* Board.cpp
*
* Created: 17.02.2022 01:39:53
* Author: Valentin Fischer und Leonie Wundram
* ESE
*/

#include "Board.h"

// default constructor
Board::Board(SerialHandler *Serial)
{
	_Serial = Serial;
}

void Board::init()
{

	// init LEDs on PORT B

	DDRB = 0xFF;  // Set Port B to Output
	PORTB = 0xFF; // Switch off led (low active)

	// init Taster Input on Port A

	DDRA = 0x00;

	//Initialization of Timer1

	// 1 Hz (8000000/((31249+1)*256))
	OCR1A = 31249;
	// CTC
	TCCR1B |= (1 << WGM12);
	// Prescaler 256
	TCCR1B |= (1 << CS12);
	// Output Compare Match A Interrupt Enable
	TIMSK1 |= (1 << OCIE1A);
}

void Board::setLED(int Ledx, LedSate state)
{
	int Output = 0;
	ledToBlink &= ~(1 << Ledx); // disabled blinkmode from ledx

	switch (state)
	{
	case off:

		PORTB |= (1 << Ledx);

		break;
	case on:
		PORTB &= ~(1 << Ledx); //bitwise negation

		break;

	default:
		break;
	}

	//PORTB = Output;
}

int Board::readLED(int Ledx)
{
	int ButtonState = !(PINB & (1 << Ledx)); //  first ! is to make 0 and 1
	return ButtonState;
}

int Board::readTaster(int Tasterx)

{

	int TasterState = !(PINA & (1 << Tasterx)); //first ! is to make 0 and 1
	return TasterState;
}

void Board::trapISR()
{
	if (!setTrap)
		return;

	if (trapCount >= 5) // trigger all 60s
	{
		trap = true; // trigger Trap
		trapCount = 0;
	}
	trapCount++;
}

void Board::help()
{

	_Serial->SendString("Folgende Befehle koennen eingegeben werden:\r\n");
	_Serial->SendString("Led x an	-	Led einschalten (x = 0 - 7) \r\n");
	_Serial->SendString("LED x aus	-	Led ausschalten (x = 0 - 7)\r\n");
	_Serial->SendString("LED x blink	-	LED blinken lassen (x = 0 - 7) \r\n");
	_Serial->SendString("Taster x	-	Den Zustand des Tasters ausgeben (x = 0 - 7)\r\n");
	_Serial->SendString("LED x		-	Den Zustand der LED asugeben (x = 0 - 7)\r\n");
	_Serial->SendString("Trap 0		-	Die Trap-Funktion ausschalten \r\n");
	_Serial->SendString("Trap 1		-	Die Trap-Funktion einschalten \r\n");
	_Serial->SendString("help		-	Eine Uebersicht ueber die moeglichen Befehle mit der zughoerigen Syntax anzeigen\r\n");
}

void Board::setBlink(int ledx)
{

	ledToBlink |= 1 << ledx;
}

void Board::BlinkISR()
{
	PORTB ^= ledToBlink; // toggle leds from bitmask
}

void Board::checkTrap()
{
	if (trap == true)
	{
		trap = false;
		char *trapstr = {"\r\nTrap wurde ausgeloest\r\n"};
		_Serial->SendString(trapstr);
	}
}
