
// Hochschule Heilbronn
// Studiengang: Electrical System Engineering
// Leonie Wundram und Valentin Fischer
//
// Terminal Server

// Just Jumperconfig STK600:
//
// VTARGET set
// RESET set
// AREF0 set
// AREF1 not set
// switch "Clock" on "INT"

// used Wire connection:
//
// RS232-SPARE (RXD, TXD) <-> PE0, PE1
// ISP/PDI bridge (6p flatflexwire)
//
// Connection config:
// 9600 8,N,1
//
// ------------------------------------------------------------------------------

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include "SerialHandler.h"
#include "Board.h"
#include "Interpreter.h"

SerialHandler Serial;
Board board(&Serial);
//
// TX Complete Interrupt SR
//
ISR(USART0_TX_vect)
{
	Serial.TxInterrupt();
}

//
// RX Complete Interrupt SR
//
ISR(USART0_RX_vect)
{
	Serial.RxInterrupt();
}

//
// ISR Timer1
//
ISR(TIMER1_COMPA_vect) // trigger all 1s
{
	board.BlinkISR();
	board.trapISR();
}

int main(void)
{

	Serial.init();
	board.init();
	Interpreter Inter(&board, &Serial);

	UCSR0B = (1 << RXEN0) | (1 << RXCIE0); // Enable UART0 and Enable RX Complete Interrupt

	sei(); //enable interrupts

	// start sequence

	Serial.SendString("Microcontroller Labor - Terminal Server\r\n");
	Serial.SendString("Valentin Fischer und Leonie Wundram\r\n");
	Serial.SendString("WS21/22\r\n\r\n\r\n\r\n");
	Serial.SendString("Bitte geben Sie einen Befehl ein. Wenn Sie hilfe benoetigen tippen Sie help\r\n\r\n");

	while (1)
	{
		if (Serial.newCmd)
		{
			Serial.newCmd = false;
			Inter.stringHandler(Serial.RXData.Buffer);
		}

		board.checkTrap();
	}
}
