/* 
* Interpreter.h
*
* Created: 16.02.2022 23:53:09
* Author: Valentin Fischer, Leonie Wundram
* ESE
*/

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <avr/io.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Board.h"


class Interpreter
{


public:
	Interpreter(Board *b, SerialHandler *serial);
	void stringHandler(char *str);

private:
	
	void strToLower(char *str);
	
	char *command;
	char *param[4];
	uint8_t numParam = 0;
	Board *_board;
	SerialHandler *_serial;

}; //Interpreter

#endif //__INTERPRETER_H__
