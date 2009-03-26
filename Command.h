#ifndef _COMMAND_H
#define _COMMAND_H

#include <string>
#include "Globals.h"

class Command {
public:
	const std::string command;
	const int priority;

	Command(std::string command = "", int priority = ILLEGAL_PRIORITY) : command(command), priority(priority) {}
};
#endif
