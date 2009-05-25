#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>
#include <string>

/* unique priorities */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_CONTINUE_ACTION 1003
#define PRIORITY_SELECT_ITEM 1002
#define PRIORITY_CLOSE_PAGE 1001
#define PRIORITY_LOOK 1000

class Command {
public:
	std::string command;
	int priority;

	Command(const std::string &command = "", int priority = ILLEGAL_PRIORITY) : command(command), priority(priority) {}
	Command(const Command &c) : command(c.command), priority(c.priority) {}

	bool operator==(const Command &c) const {return priority == c.priority && command == command;}
};

std::ostream &operator<<(std::ostream &os, const Command &c);
#endif
