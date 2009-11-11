#include "Command.h"

using namespace std;

/* constructors */
Command::Command(const std::string& command, int priority) : _command(command), _priority(priority) {
}

Command::Command(unsigned char key, int priority) : _command(string(1, key)), _priority(priority) {
}

/* destructor */
Command::Command(const Command& c) : _command(c._command), _priority(c._priority) {
}

/* methods */
const std::string& Command::command() const {
	return _command;
}

const std::string& Command::command(const std::string& command) {
	_command = command;
	return this->command();
}

int Command::priority() const {
	return _priority;
}

int Command::priority(int priority) {
	_priority = priority;
	return this->priority();
}

/* operator overloading */
bool Command::operator==(const Command& c) const {
	return _priority == c._priority && _command == _command;
}

ostream & operator<<(ostream& os, const Command& c) {
	return os << "(" << c.priority() << "): " << c.command();
}
