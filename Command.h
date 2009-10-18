#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>

/* unique priorities */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_CONTINUE_ACTION 1003
#define PRIORITY_SELECT_ITEM 1002
#define PRIORITY_CLOSE_SELECT_PAGE 1001
#define PRIORITY_LOOK 1000
#define PRIORITY_TURN_MAX 999

/* some common answers */
#define CLOSE_PAGE ' '
#define DESELECT_PAGE '\\'
#define HANDS '-'
#define NO 'n'
#define QUIT "#quit\n"
#define SELECT_PAGE ','
#define YES 'y'

class Command {
public:

	Command(const std::string& command = "", int priority = ILLEGAL_PRIORITY) : _command(command), _priority(priority) {
	}

	Command(const Command& c) : _command(c._command), _priority(c._priority) {
	}

	const std::string& command() const {
		return _command;
	}

	const std::string& command(const std::string& command) {
		_command = command;
		return this->command();
	}

	int priority() const {
		return _priority;
	}

	int priority(int priority) {
		_priority = priority;
		return this->priority();
	}

	bool operator==(const Command& c) const {
		return _priority == c._priority && _command == _command;
	}

private:
	std::string _command;
	int _priority;
};

std::ostream& operator<<(std::ostream& os, const Command& c);
#endif
