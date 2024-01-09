/*
* SerialHandler.cpp
*
* Created: 16.02.2022 19:14:19
* Author: Valentin Fischer und Leonie Wundram
* ESE
*/

#include "SerialHandler.h"

void SerialHandler::init()
{

	UCSR0B = (1 << RXEN0) | (1 << RXCIE0); // Enable UART0 and Enable RX Complete Interrupt

	// Weitere Konfiguration:
	//
	// UMSEL00 und UMSEL01 = 0: Asynchroner Modus
	// UPM00 und UPM01 = 0: Kein Parity
	// USBS0 = 0: 1 Stop-Bit
	// UCSZ00 = UCSZ01 = 1, UCSZ02 = 0: 8 Datenbits
	// UCPOL0 = 0:  Rising XCK0 Edge, Falling XCK0 Edge
	//
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); 

	UBRR0H = 0;	 // Einstellen der Baudrate
	UBRR0L = 51; // Werte aus der Tabelle im Datenblatt
}

void SerialHandler::TxInterrupt()
{

	//
	// Steht ein Beep zum Senden an?
	//
	if (OutAlert == true)
	{

		OutAlert = false;
		ringbuf_memcpy_into(ringBuff, Beep_Str, strlen(Beep_Str));
	}
	else
	{

		if (ringbuf_bytes_used(ringBuff)) // new data on ringbuffer ?
		{

			ringbuf_memcpy_from(&SerialOut, ringBuff, 1);
			UDR0 = SerialOut;
		}
		else
		{
			UCSR0B ^= (1 << TXCIE0); //Deaktivierung des Interrupts f�r den Sender
		}
	}
}

void SerialHandler::RxInterrupt()
{

	char RXChar;

	RXChar = UDR0; // read Serial input register

	sendChar(RXChar);

	if (RXChar == CHAR_CR)
	{

		SendString(EOL_Str); // send new line
		strcpy(RXData.Buffer, TmpBuf.Buffer);
		newCmd = true;

		
		TmpBuf.BufPtr = 0;
		memset(TmpBuf.Buffer, 0, sizeof(TmpBuf.Buffer));
	}
	else if (RXChar == CHAR_BSP || RXChar == CHAR_BSP_PUTTY) // Backspace ?
	{
		if (TmpBuf.BufPtr == 0)
		{
			IniTerminalBeep();
		}
		else
		{
			
			TmpBuf.Buffer[TmpBuf.BufPtr] = 0;
			TmpBuf.BufPtr--;
		}
	}
	else
	{
		TmpBuf.Buffer[TmpBuf.BufPtr] = RXChar;
		TmpBuf.BufPtr++;
	}
}

void SerialHandler::IniTerminalBeep()
{

	SendString(Beep_Str);
}

void SerialHandler::SendString(char *s)
{

	//
	// Keine leeren Zeichenketten senden
	//
	size_t s_length = strlen(s);
	
	if (s_length != 0)
	{

		while (s_length > ringbuf_bytes_free(ringBuff)) //waiting for free buffer
		{
		}

		ringbuf_memcpy_into(ringBuff, s, s_length); // copy String to Ring buffer

		if (!(UCSR0B & (1 << TXCIE0))) // check if not currently sending
		{
			UCSR0B |= (1 << TXEN0) | (1 << TXCIE0); //Send interrupt enabled
			TxInterrupt();							// trigger Subroutine to start the sending
		}
	}
}

void SerialHandler::sendChar(char c)
{
	char cstr[2] = {c, 0};
	SendString(cstr);
}