/*
* SerialHandler.h
*
* Created: 16.02.2022 19:14:19
* Author: Valentin Fischer und Leonie Wundram
* ESE
*/

#ifndef __SERIALHANDLER_H__
#define __SERIALHANDLER_H__

#include <avr/io.h>
#include <string.h>
#include "ringbuf.h"
#include <stdio.h>

class SerialHandler
{
private:


	const static char CHAR_CR = 0x0D;		 // character f. carriage return
	const static char CHAR_LF = 0x0A;		 // Character f. Line Feed
	const static char CHAR_BEEP = 0x07;		 // Character f. Beep
	const static char CHAR_BSP = 0x08;		 // character f. backspace
	const static char CHAR_BSP_PUTTY = 0x7F; // character f. backspace
	const static int CHARBUF_SIZ = 100;		 // Default buffer size
	const static int TX_CHARBUF_SIZ = 1000;	 // Ringbuffer Size  ! attention memory is dynamically allocated !

	ringbuf_t ringBuff = ringbuf_new(TX_CHARBUF_SIZ);
	char SerialOut = 0;

	const static int EOL_STR_LEN = 3; // length of EOL-Strings
	char EOL_Str[EOL_STR_LEN] = {CHAR_CR, CHAR_LF, '\0'};

	const static int BEEP_STR_LEN = 2; //length of Beep-Strings
	char Beep_Str[BEEP_STR_LEN] = {CHAR_BEEP, '\0'};

public:
	bool OutAlert = false;
	bool newCmd = false;
	bool trap = false;

	
	void init();
	void TxInterrupt();
	void RxInterrupt();
	void IniTerminalBeep();
	void SendString(char *s);
	void sendChar(char c);

	typedef struct CharBuffer
	{
		char Buffer[CHARBUF_SIZ] = {0};
		unsigned short BufPtr = 0;
	} CharBuffer_t;

	CharBuffer_t RXData;
	CharBuffer_t TmpBuf;

}; //SerialHandler

#endif //__SERIALHANDLER_H__
