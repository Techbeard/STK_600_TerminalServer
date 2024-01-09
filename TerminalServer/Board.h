/* 
* Board.h
*
* Created: 17.02.2022 01:39:53
* Author: Valentin Fischer und Leonie Wundram
* ESE
*/

#ifndef __BOARD_H__
#define __BOARD_H__

#include <avr/io.h>
#include "SerialHandler.h"

class Board
{

	
public:
	enum LedSate
	{
		off,
		on,

	};

	Board(SerialHandler *Serial);

	void setLED(int Ledx, LedSate state);
	void help();
	void trapISR();
	void checkTrap();
	int readTaster(int Tasterx);
	int readLED(int Ledx);
	void init();
	void setBlink(int ledx);
	void BlinkISR();

	bool setTrap = false; // trap per default off

private:
	SerialHandler *_Serial;

	uint8_t ledToBlink;
	bool trap = false;
	int trapCount = 0;

}; 

#endif //__BOARD_H__
