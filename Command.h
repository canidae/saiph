#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>
#include <string>
#include "Globals.h"

class Command {
public:
	std::string command;
	int priority;

	Command(const std::string &command = "", int priority = ILLEGAL_PRIORITY) : command(command), priority(priority) {}
	Command(const Command &c) : command(c.command), priority(c.priority) {}

	bool operator==(const Command &c) const {return priority == c.priority && command == command;}
};

std::ostream &operator<<(std::ostream &os, const Command &c) {return os << "(" << c.priority << "): " << c.command;}
#endif
