/*
* Interpreter.cpp
*
* Created: 16.02.2022 23:53:09
* Author: Valentin Fischer, Leonie Wundram
* ESE
*/

#include "Interpreter.h"

// default constructor
Interpreter::Interpreter(Board *b, SerialHandler *serial)
{
	_board = b;
	_serial = serial;
} 

void Interpreter::strToLower(char *str)
{

	for (unsigned int i = 0; i < strlen(str); ++i)
	{
		str[i] = tolower((unsigned char)str[i]);
	}
}

void Interpreter::stringHandler(char *str)
{
	strToLower(str);

	command = strtok(str, " ");

	for (uint8_t i = 0; i < sizeof(param); i++)

	{
		char *tok = strtok(NULL, " ");
		if (tok != NULL)
		{

			param[i] = tok;
			numParam = i + 1;
		}
		else
		{
			break;
		}
	}

	int param0;
	sscanf(param[0], "%hhd", &param0); // todo: If param0 != number non-differentiated states can occur

	if (strcmp(command, "led") == 0 && numParam >= 2)
	{

		_board->setLED(param0, _board->on);

		if (strcmp(param[1], "an") == 0)
		{
			_board->setLED(param0, _board->on);

			
		}
		else if (strcmp(param[1], "blink") == 0)
		{
			_board->setBlink(param0);
			
		}
		else // always turns off if not set to on or blink
		{
			_board->setLED(param0, _board->off);
					}
	}
	else if (strcmp(command, "led") == 0 && numParam == 1)
	{
		_serial->SendString("Led Zustand: ");
		_serial->sendChar(_board->readLED(param0) + '0');
		_serial->SendString("\r\n");
	}
	else if (strcmp(command, "taster") == 0 && numParam >= 1)
	{
		_serial->SendString("Taster Zustand: ");
		_serial->sendChar(_board->readTaster(param0) + '0');
		_serial->SendString("\r\n");
	}
	else if (strcmp(command, "trap") == 0 && numParam >= 1)
	{
		_board->setTrap = param0;
	}
	else if (strcmp(command, "help") == 0)
	{
		_board->help();
	}
}


